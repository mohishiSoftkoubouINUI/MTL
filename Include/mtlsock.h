





#define _AFX_SOCK_THREAD_STATE _AFX_SOCK_STATE
#define _afxSockThreadState GetafxSockState()


class _AFX_SOCK_STATE
{
	public:
		_AFX_SOCK_STATE()
		{
			m_hSocketWindow = NULL ;
		}

		virtual ~_AFX_SOCK_STATE()
		{
			if (m_pfnSockTerm != NULL)
				m_pfnSockTerm();
		}

	public:
		DWORD m_dwReserved1;    // reserved for version 4.1 only
		HINSTANCE m_hInstSOCK;      // handle of WSOCK32.DLL
		void(*m_pfnSockTerm)(void); // set once initialized


		HWND m_hSocketWindow ;

		CMapPtrToPtr m_mapSocketHandle;
		CMapPtrToPtr m_mapDeadSockets;
		CPtrList m_listSocketNotifications ;
};

_INLINE _AFX_SOCK_STATE* _GetafxSockState()
{
	static _AFX_SOCK_STATE _afxSockState;

	return &_afxSockState;
}

_INLINE _AFX_SOCK_STATE* GetafxSockState()
{
	//return _GetafxSockState();
	_VOLATILE_STATIC_FUNC_T_V(_AFX_SOCK_STATE*, _GetafxSockState)
}


/*============================================================================*/
// CSocketWnd -- internal use only
//  Implementation for sockets notification callbacks.
//  Future versions of MFC may or may not include this exact class.

class CSocket ;

class CSocketWnd : public CWnd
{
	public:

	public:
		DECLARE_MESSAGE_MAP()

	public:
		LRESULT OnSocketDead(WPARAM wParam, LPARAM lParam) ;
		LRESULT OnSocketNotify(WPARAM wParam, LPARAM lParam) ;

};

BEGIN_MESSAGE_MAP(CSocketWnd, CWnd)
	ON_MESSAGE(WM_SOCKET_NOTIFY, &CSocketWnd::OnSocketNotify)
	ON_MESSAGE(WM_SOCKET_DEAD, &CSocketWnd::OnSocketDead)
END_MESSAGE_MAP()




/*============================================================================*/
// CAsyncSocket

class CAsyncSocket : public CObject
{
	//DECLARE_DYNAMIC(CAsyncSocket);

	public:
		CAsyncSocket()
		{
			m_hSocket = INVALID_SOCKET;
		}

		virtual ~CAsyncSocket()
		{
			if (m_hSocket != INVALID_SOCKET)
				Close();
		}

	// Attributes
	public:
		enum { receives = 0, sends = 1, both = 2 };

		SOCKET m_hSocket;

		friend class CSocketWnd;

	public:
		operator SOCKET() const { return m_hSocket; }

		virtual void OnAccept(int nErrorCode) {}
		virtual void OnClose(int nErrorCode) {}
		virtual void OnConnect(int nErrorCode) {}
		virtual void OnReceive(int nErrorCode) {}
		virtual void OnSend(int nErrorCode) {}
		virtual void OnOutOfBandData(int nErrorCode) {}



		virtual BOOL Accept(CAsyncSocket& rConnectedSocket, SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL)
		{
			ASSERT(rConnectedSocket.m_hSocket == INVALID_SOCKET);
			ASSERT(CAsyncSocket::FromHandle(INVALID_SOCKET) == NULL);

			CAsyncSocket::AttachHandle(INVALID_SOCKET, &rConnectedSocket);
			if (CAsyncSocket::FromHandle(INVALID_SOCKET) == NULL)
			{
				// AttachHandle Call has failed
				return FALSE;
			}

			SOCKET hTemp = accept(m_hSocket, lpSockAddr, lpSockAddrLen);

			if (hTemp == INVALID_SOCKET)
			{
				DWORD dwProblem = GetLastError();
				CAsyncSocket::DetachHandle(rConnectedSocket.m_hSocket, FALSE);
				rConnectedSocket.m_hSocket = INVALID_SOCKET;
				SetLastError(dwProblem);
			}
			else if (CAsyncSocket::FromHandle(INVALID_SOCKET) != NULL)
			{
				rConnectedSocket.m_hSocket = hTemp;
				CAsyncSocket::DetachHandle(INVALID_SOCKET, FALSE);
				CAsyncSocket::AttachHandle(hTemp, &rConnectedSocket);
			}

			return (hTemp != INVALID_SOCKET);
		}

		BOOL AsyncSelect(long lEvent =FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE)
		{
			ASSERT(m_hSocket != INVALID_SOCKET);

			_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;
			ASSERT(pState->m_hSocketWindow != NULL);

			return WSAAsyncSelect(m_hSocket, pState->m_hSocketWindow,
									WM_SOCKET_NOTIFY, lEvent) != SOCKET_ERROR;
		}

		BOOL Bind(const SOCKADDR* lpSockAddr, int nSockAddrLen)
		{
			return (SOCKET_ERROR != bind(m_hSocket, lpSockAddr, nSockAddrLen));
		}

		BOOL Bind(UINT nSocketPort, LPCTSTR lpszSocketAddress = NULL)
		{
			USES_CONVERSION;

			SOCKADDR_IN sockAddr;
			memset(&sockAddr, 0, sizeof(sockAddr));

			LPSTR lpszAscii;
			if (lpszSocketAddress != NULL)
			{
				lpszAscii = T2A((LPTSTR)lpszSocketAddress);
				if (lpszAscii == NULL)
				{
					// OUT OF MEMORY
					WSASetLastError(ERROR_NOT_ENOUGH_MEMORY);
					return FALSE;
				}
			}
			else
			{
				lpszAscii = NULL;
			}

			sockAddr.sin_family = AF_INET;

			if (lpszAscii == NULL)
				sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			else
			{
				DWORD lResult = inet_addr(lpszAscii);
				if (lResult == INADDR_NONE)
				{
					WSASetLastError(WSAEINVAL);
					return FALSE;
				}
				sockAddr.sin_addr.s_addr = lResult;
			}

			sockAddr.sin_port = htons((u_short)nSocketPort);

			return Bind((SOCKADDR*)&sockAddr, sizeof(sockAddr));
		}

		virtual void Close()
		{
			if (m_hSocket != INVALID_SOCKET)
			{
				VERIFY(SOCKET_ERROR != closesocket(m_hSocket));
				CAsyncSocket::KillSocket(m_hSocket, this);
				m_hSocket = INVALID_SOCKET;
			}
		}

		BOOL Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen)
		{
			return ConnectHelper(lpSockAddr, nSockAddrLen);
		}

		BOOL Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
		{
			USES_CONVERSION;

			ASSERT(lpszHostAddress != NULL);

			if (lpszHostAddress == NULL)
			{
				WSASetLastError(WSAEINVAL);
				return FALSE;
			}

			SOCKADDR_IN sockAddr;
			memset(&sockAddr, 0, sizeof(sockAddr));

			LPSTR lpszAscii = T2A((LPTSTR)lpszHostAddress);
			if (lpszAscii == NULL)
			{
				WSASetLastError(ERROR_NOT_ENOUGH_MEMORY);
				return FALSE;
			}

			sockAddr.sin_family = AF_INET;
			sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);

			if (sockAddr.sin_addr.s_addr == INADDR_NONE)
			{
				LPHOSTENT lphost;
				lphost = gethostbyname(lpszAscii);
				if (lphost != NULL)
					sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
				else
				{
					WSASetLastError(WSAEINVAL);
					return FALSE;
				}
			}

			sockAddr.sin_port = htons((u_short)nHostPort);

			return Connect((SOCKADDR*)&sockAddr, sizeof(sockAddr));
		}

		virtual BOOL ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen)
		{
			return connect(m_hSocket, lpSockAddr, nSockAddrLen) != SOCKET_ERROR;
		}

		BOOL Create(UINT nSocketPort = 0, int nSocketType = SOCK_STREAM,
					long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
					LPCTSTR lpszSocketAddress = NULL)
		{
			if (Socket(nSocketType, lEvent))
			{
				if (Bind(nSocketPort, lpszSocketAddress))
					return TRUE;
				int nResult = GetLastError();
				Close();
				WSASetLastError(nResult);
			}
			return FALSE;
		}


		static int GetLastError() { return WSAGetLastError(); }

		static void DoCallBack(WPARAM wParam, LPARAM lParam)
		{
			if (wParam == 0 && lParam == 0)
				return;

			// Has the socket be closed - lookup in dead handle list
			CAsyncSocket* pSocket = CAsyncSocket::LookupHandle((SOCKET)wParam, TRUE);

			// If yes ignore message
			if (pSocket != NULL)
				return;

			pSocket = CAsyncSocket::LookupHandle((SOCKET)wParam, FALSE);
			if (pSocket == NULL)
			{
				// Must be in the middle of an Accept call
				pSocket = CAsyncSocket::LookupHandle(INVALID_SOCKET, FALSE);
				ASSERT(pSocket != NULL);

				if (pSocket == NULL)
					return;

				pSocket->m_hSocket = (SOCKET)wParam;
				CAsyncSocket::DetachHandle(INVALID_SOCKET, FALSE);
				CAsyncSocket::AttachHandle(pSocket->m_hSocket, pSocket, FALSE);
			}

			int nErrorCode = WSAGETSELECTERROR(lParam);
			switch (WSAGETSELECTEVENT(lParam))
			{
				case FD_READ:
				{
					fd_set fds;
					int nReady;
					timeval timeout;

					timeout.tv_sec = 0;
					timeout.tv_usec = 0;

					FD_ZERO(&fds);
					FD_SET(pSocket->m_hSocket, &fds);
					nReady = select(0, &fds, NULL, NULL, &timeout);
					if (nReady == SOCKET_ERROR)
						nErrorCode = WSAGetLastError();
					if ((nReady == 1) || (nErrorCode != 0))
						pSocket->OnReceive(nErrorCode);
					break;
				}
				case FD_WRITE:
					pSocket->OnSend(nErrorCode);
					break;
				case FD_OOB:
					pSocket->OnOutOfBandData(nErrorCode);
					break;
				case FD_ACCEPT:
					pSocket->OnAccept(nErrorCode);
					break;
				case FD_CONNECT:
					pSocket->OnConnect(nErrorCode);
					break;
				case FD_CLOSE:
					pSocket->OnClose(nErrorCode);
					break;
				}
		}

		BOOL GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen) 
		{
			return (SOCKET_ERROR != getpeername(m_hSocket, lpSockAddr, lpSockAddrLen));
		}

		BOOL GetPeerName(CString& rPeerAddress, UINT& rPeerPort);

		BOOL GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
		{
			return (SOCKET_ERROR != getsockname(m_hSocket, lpSockAddr, lpSockAddrLen));
		}

		BOOL GetSockName(CString& rSocketAddress, UINT& rSocketPort);

		BOOL GetSockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel = SOL_SOCKET)
		{
			return (SOCKET_ERROR != getsockopt(m_hSocket, nLevel, nOptionName, (LPSTR)lpOptionValue, lpOptionLen));
		}


		BOOL IOCtl(long lCommand, DWORD* lpArgument)
		{
			return (SOCKET_ERROR != ioctlsocket(m_hSocket, lCommand, lpArgument));
		}

		static void KillSocket(SOCKET hSocket, CAsyncSocket* pSocket)
		{
			ASSERT(CAsyncSocket::LookupHandle(hSocket, FALSE) != NULL);

			_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;

			CAsyncSocket::DetachHandle(hSocket, FALSE);
			if (pState->m_hSocketWindow != NULL)
			{
				::PostMessage(pState->m_hSocketWindow, WM_SOCKET_DEAD, (WPARAM)hSocket, 0L);
				CAsyncSocket::AttachHandle(hSocket, pSocket, TRUE);
			}
		}

		BOOL Listen(int nConnectionBacklog = 5)
		{
			return (SOCKET_ERROR != listen(m_hSocket, nConnectionBacklog));
		}

		virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0)
		{
			return recv(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags);
		}

		int ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags = 0)
		{
			return ReceiveFromHelper(lpBuf, nBufLen, lpSockAddr, lpSockAddrLen, nFlags);
		}

		int ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags = 0);

		virtual int ReceiveFromHelper(void* lpBuf, int nBufLen,SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags)
		{
			return recvfrom(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags, lpSockAddr, lpSockAddrLen);
		}

		virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0)
		{
			return send(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags);
		}

		virtual int SendToHelper(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags)
		{
			return sendto(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags, lpSockAddr, nSockAddrLen);
		}

		BOOL SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel = SOL_SOCKET)
		{
			return (SOCKET_ERROR != setsockopt(m_hSocket, nLevel, nOptionName, (LPCSTR)lpOptionValue, nOptionLen));
		}

		int SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags = 0)
		{
			return SendToHelper(lpBuf, nBufLen, lpSockAddr, nSockAddrLen, nFlags);
		}

		int SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress = NULL, int nFlags = 0);



		BOOL ShutDown(int nHow = sends) { return (SOCKET_ERROR != shutdown(m_hSocket, nHow)); }

		BOOL Socket(int nSocketType = SOCK_STREAM, long lEvent =
						FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
						int nProtocolType = 0, int nAddressFormat = PF_INET)
		{
			ASSERT(m_hSocket == INVALID_SOCKET);

			m_hSocket = socket(nAddressFormat, nSocketType, nProtocolType);
			if (m_hSocket != INVALID_SOCKET)
			{
				CAsyncSocket::AttachHandle(m_hSocket, this, FALSE);
				return AsyncSelect(lEvent);
			}
			return FALSE;
		}


		BOOL Attach(SOCKET hSocket, long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);
		SOCKET Detach();


		static void AttachHandle(SOCKET hSocket, CAsyncSocket* pSocket, BOOL bDead = FALSE)
		{
			_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;

			if (!bDead)
			{
				ASSERT(CAsyncSocket::LookupHandle(hSocket, bDead) == NULL);
				if (pState->m_mapSocketHandle.IsEmpty())
				{
					ASSERT(pState->m_mapDeadSockets.IsEmpty());
					ASSERT(pState->m_hSocketWindow == NULL);

					CSocketWnd* pWnd = new CSocketWnd;
					pWnd->m_hWnd = NULL;

					if (!pWnd->CreateEx(0, AfxRegisterWndClass(0),
										_T("Socket Notification Sink"), WS_OVERLAPPED, 0, 0, 0, 0, NULL, NULL))
					{
						TRACE(atlTraceGeneral, 0, "Warning: unable to create socket notify window!\n");
						delete pWnd;
						AfxThrowResourceException();
					}

					ASSERT(pWnd->m_hWnd != NULL);
					ASSERT(CWnd::FromHandlePermanent(pWnd->m_hWnd) == pWnd);
					pState->m_hSocketWindow = pWnd->m_hWnd;
				}
				pState->m_mapSocketHandle.SetAt((void*)hSocket, pSocket);
			}
			else
			{
				void* pvCount;
				INT_PTR nCount;
				if (pState->m_mapDeadSockets.Lookup((void*)hSocket, pvCount))
				{
					nCount = (INT_PTR)pvCount;
					nCount++;
				}
				else
					nCount = 1;
				pState->m_mapDeadSockets.SetAt((void*)hSocket, (void*)nCount);
			}
		}

		static void DetachHandle(SOCKET hSocket, BOOL bDead = FALSE)
		{
			ASSERT(CAsyncSocket::LookupHandle(hSocket, bDead) != NULL);

			_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;
			if (!bDead)
			{
				pState->m_mapSocketHandle.RemoveKey((void*)hSocket);
				if (pState->m_mapSocketHandle.IsEmpty())
				{
					ASSERT(pState->m_hSocketWindow != NULL);
					CWnd* pWnd = CWnd::FromHandlePermanent(pState->m_hSocketWindow);
					ASSERT_VALID(pWnd);

					if (pWnd != NULL)
					{
						pWnd->DestroyWindow();
						delete pWnd;
					}

					pState->m_hSocketWindow = NULL;

					pState->m_mapDeadSockets.RemoveAll();

					while (!pState->m_listSocketNotifications.IsEmpty())
						delete pState->m_listSocketNotifications.RemoveHead();
				}
			}
			else
			{
				void* pvCount;
				INT_PTR nCount;
				if (pState->m_mapDeadSockets.Lookup((void*)hSocket, pvCount))
				{
					nCount = (INT_PTR)pvCount;
					nCount--;
					if (nCount == 0)
						pState->m_mapDeadSockets.RemoveKey((void*)hSocket);
					else
						pState->m_mapDeadSockets.SetAt((void*)hSocket, (void*)nCount);
				}
			}
		}


		static CAsyncSocket* FromHandle(SOCKET hSocket)
		{
			return CAsyncSocket::LookupHandle(hSocket, FALSE);
		}

		static CAsyncSocket* LookupHandle(SOCKET hSocket, BOOL bDead = FALSE)
		{
			CAsyncSocket* pSocket;
			_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;
			if (!bDead)
			{
				pSocket = (CAsyncSocket*)pState->m_mapSocketHandle.GetValueAt((void*)hSocket);
				if (pSocket != NULL)
					return pSocket;
			}
			else
			{
				pSocket = (CAsyncSocket*)
					pState->m_mapDeadSockets.GetValueAt((void*)hSocket);
				if (pSocket != NULL)
					return pSocket;
			}
			return NULL;
		}



};

/*============================================================================*/
// CSocket

class CSocket : public CAsyncSocket
{
	//DECLARE_DYNAMIC(CSocket);

	public:
		CSocket();
		virtual ~CSocket();


	public:
		int m_nTimeOut;

		BOOL* m_pbBlocking;
		int m_nConnectError;


		friend class CSocketWnd;


	public:
		virtual BOOL OnMessagePending()
		{
			MSG msg;
			if (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_REMOVE))
			{
				::DispatchMessage(&msg);
				return FALSE;   // usually return TRUE, but OnIdle usually causes WM_PAINTs
			}
			return FALSE;
		}

		virtual BOOL Accept(CAsyncSocket& rConnectedSocket, SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

		static void AuxQueueAdd(UINT message, WPARAM wParam, LPARAM lParam)
		{
			_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;

			MSG* pMsg = new MSG;
			pMsg->message = message;
			pMsg->wParam = wParam;
			pMsg->lParam = lParam;
			pState->m_listSocketNotifications.AddTail(pMsg);
		}

		virtual void Close()
		{
			if (m_hSocket != INVALID_SOCKET)
			{
				CancelBlockingCall();

				VERIFY(AsyncSelect(0));
				CAsyncSocket::Close();
				m_hSocket = INVALID_SOCKET;
			}
		}

		virtual BOOL ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen)
		{
			if (m_pbBlocking != NULL)
			{
				WSASetLastError(WSAEINPROGRESS);
				return  FALSE;
			}

			m_nConnectError = -1;

			if (!CAsyncSocket::ConnectHelper(lpSockAddr, nSockAddrLen))
			{
				if (GetLastError() == WSAEWOULDBLOCK)
				{
					while (PumpMessages(FD_CONNECT))
					{
						if (m_nConnectError != -1)
						{
							WSASetLastError(m_nConnectError);
							return (m_nConnectError == 0);
						}
					}
				}
				return FALSE;
			}
			return TRUE;
		}

		BOOL Create(UINT nSocketPort = 0, int nSocketType = SOCK_STREAM, LPCTSTR lpszSocketAddress = NULL)
		{
			return CAsyncSocket::Create(nSocketPort, nSocketType, FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE, lpszSocketAddress);
		}

		void CancelBlockingCall()
		{
			if (m_pbBlocking != NULL)
			{
				*m_pbBlocking = FALSE;
				m_pbBlocking = NULL;
			}
		}


		BOOL IsBlocking() { return (m_pbBlocking != NULL); }

		static int ProcessAuxQueue()
		{
			_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;

			if (pState->m_listSocketNotifications.IsEmpty())
				return 0;

			int nCount = 0;
			while (!pState->m_listSocketNotifications.IsEmpty())
			{
				nCount++;

				MSG* pMsg = (MSG*)pState->m_listSocketNotifications.RemoveHead();
				ASSERT(pMsg != NULL);
				if (pMsg->message == WM_SOCKET_NOTIFY)
				{
					CAsyncSocket::DoCallBack(pMsg->wParam, pMsg->lParam);
				}
				else
				{
					ASSERT(CAsyncSocket::LookupHandle((SOCKET)pMsg->wParam, TRUE) != NULL);
					CAsyncSocket::DetachHandle((SOCKET)pMsg->wParam, TRUE);
				}
				delete pMsg;
			}
			return nCount;
		}

		virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0)
		{
			if (m_pbBlocking != NULL)
			{
				WSASetLastError(WSAEINPROGRESS);
				return  FALSE;
			}
			int nResult;
			while ((nResult = CAsyncSocket::Receive(lpBuf, nBufLen, nFlags)) == SOCKET_ERROR)
			{
				if (GetLastError() == WSAEWOULDBLOCK)
				{
					if (!PumpMessages(FD_READ))
						return SOCKET_ERROR;
				}
				else
					return SOCKET_ERROR;
			}
			return nResult;
		}

		virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0)
		{
			if (m_pbBlocking != NULL)
			{
				WSASetLastError(WSAEINPROGRESS);
				return  FALSE;
			}

			int nLeft, nWritten;
			PBYTE pBuf = (PBYTE)lpBuf;
			nLeft = nBufLen;

			while (nLeft > 0)
			{
				nWritten = SendChunk(pBuf, nLeft, nFlags);
				if (nWritten == SOCKET_ERROR)
					return nWritten;

				nLeft -= nWritten;
				pBuf += nWritten;
			}
			return nBufLen - nLeft;
		}

		virtual BOOL PumpMessages(UINT uStopFlag)
		{
			// The same socket better not be blocking in more than one place.
			ASSERT(m_pbBlocking == NULL);

			_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;

			ASSERT(pState->m_hSocketWindow != NULL);

			BOOL bBlocking = TRUE;
			m_pbBlocking = &bBlocking;
			CWinApp* pThread = AfxGetApp();

			// This is not a timeout in the WinSock sense, but more
			// like a WM_KICKIDLE to keep message pumping alive
			UINT_PTR nTimerID = ::SetTimer(pState->m_hSocketWindow, 1, m_nTimeOut, NULL);

			if (nTimerID == 0)
				AfxThrowResourceException();

			BOOL bPeek = TRUE;

			while (bBlocking)
			{
				TRY
				{
					MSG msg;
					if (::PeekMessage(&msg, pState->m_hSocketWindow,
						WM_SOCKET_NOTIFY, WM_SOCKET_DEAD, PM_REMOVE))
					{
						if (msg.message == WM_SOCKET_NOTIFY && (SOCKET)msg.wParam == m_hSocket)
						{
							if (WSAGETSELECTEVENT(msg.lParam) == FD_CLOSE)
							{
								break;
							}
							if (WSAGETSELECTEVENT(msg.lParam) == uStopFlag)
							{
								if (uStopFlag == FD_CONNECT)
									m_nConnectError = WSAGETSELECTERROR(msg.lParam);
								break;
							}
						}
						if (msg.wParam != 0 || msg.lParam != 0)
							CSocket::AuxQueueAdd(msg.message, msg.wParam, msg.lParam);

						bPeek = TRUE;
					}
					else if (::PeekMessage(&msg, pState->m_hSocketWindow, WM_TIMER, WM_TIMER, PM_REMOVE))
					{
						break;
					}

					if (bPeek && ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
					{
						if (OnMessagePending())
						{
							// allow user-interface updates
							ASSERT(pThread);
							pThread->OnIdle(-1);
						}
						else
						{
							bPeek = FALSE;
						}
					}
					else
					{
						// no work to do -- allow CPU to sleep
						WaitMessage();
						bPeek = TRUE;
					}
				}
					CATCH_ALL(e)
				{
					TRACE(atlTraceGeneral, 0, "Error: caught exception in PumpMessage - continuing.\n");
					DELETE_EXCEPTION(e);
					bPeek = TRUE;
				}
				END_CATCH_ALL
			}

			::KillTimer(pState->m_hSocketWindow, nTimerID);

			if (!bBlocking)
			{
				WSASetLastError(WSAEINTR);
				return FALSE;
			}
			m_pbBlocking = NULL;

			::PostMessage(pState->m_hSocketWindow, WM_SOCKET_NOTIFY, 0, 0);

			return TRUE;
		}

		virtual int ReceiveFromHelper(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags)
		{
			if (m_pbBlocking != NULL)
			{
				WSASetLastError(WSAEINPROGRESS);
				return  FALSE;
			}
			int nResult;
			while ((nResult = CAsyncSocket::ReceiveFromHelper(lpBuf, nBufLen, lpSockAddr, lpSockAddrLen, nFlags)) == SOCKET_ERROR)
			{
				if (GetLastError() == WSAEWOULDBLOCK)
				{
					if (!PumpMessages(FD_READ))
						return SOCKET_ERROR;
				}
				else
					return SOCKET_ERROR;
			}
			return nResult;
		}

		int SendChunk(const void* lpBuf, int nBufLen, int nFlags)
		{
			int nResult;
			while ((nResult = CAsyncSocket::Send(lpBuf, nBufLen, nFlags)) == SOCKET_ERROR)
			{
				if (GetLastError() == WSAEWOULDBLOCK)
				{
					if (!PumpMessages(FD_WRITE))
						return SOCKET_ERROR;
				}
				else
					return SOCKET_ERROR;
			}
			return nResult;
		}

		virtual int SendToHelper(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags)
		{
			if (m_pbBlocking != NULL)
			{
				WSASetLastError(WSAEINPROGRESS);
				return SOCKET_ERROR;
			}
			int nResult;
			while ((nResult = CAsyncSocket::SendToHelper(lpBuf, nBufLen, lpSockAddr, nSockAddrLen, nFlags)) == SOCKET_ERROR)
			{
				if (GetLastError() == WSAEWOULDBLOCK)
				{
					if (!PumpMessages(FD_WRITE))
						return SOCKET_ERROR;
				}
				else
					return SOCKET_ERROR;
			}
			return nResult;
		}


		static CSocket* FromHandle(SOCKET hSocket)
		{
			return (CSocket*)CAsyncSocket::LookupHandle(hSocket, FALSE);
		}
		BOOL Attach(SOCKET hSocket) { return CAsyncSocket::Attach(hSocket); }
};



_INLINE LRESULT CSocketWnd::OnSocketDead(WPARAM wParam, LPARAM lParam)
{
	CSocket::AuxQueueAdd(WM_SOCKET_DEAD, wParam, lParam);
	CSocket::ProcessAuxQueue();
	return 0L;
}

_INLINE LRESULT CSocketWnd::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	CSocket::AuxQueueAdd(WM_SOCKET_NOTIFY, wParam, lParam);
	CSocket::ProcessAuxQueue();
	return 0L;
}



/////////////////////////////////////////////////////////////////////////////
// Global functions


_INLINE void AfxSocketTerm()
{
	_AFX_SOCK_STATE* pState = GetafxSockState();
	if (pState->m_hInstSOCK != NULL)
	{
		if (pState->m_pfnSockTerm != NULL)
			WSACleanup();
		FreeLibrary(pState->m_hInstSOCK);// handle of WSOCK32.DLL
		pState->m_hInstSOCK = NULL;
	}
}

_INLINE BOOL AfxSocketInit(WSADATA* lpwsaData = NULL)
{
	_AFX_SOCK_STATE* pState = GetafxSockState();
	if (pState->m_pfnSockTerm == NULL)
	{
		// initialize Winsock library
		WSADATA wsaData;
		if (lpwsaData == NULL)
			lpwsaData = &wsaData;

		WORD wVersionRequested = MAKEWORD(1, 1);
		int nResult = WSAStartup(wVersionRequested, lpwsaData);
		if (nResult != 0)
			return FALSE;

		if (LOBYTE(lpwsaData->wVersion) != 1 || HIBYTE(lpwsaData->wVersion) != 1)
		{
			WSACleanup();
			WSASetLastError(WSAVERNOTSUPPORTED);
			return FALSE;
		}

		// setup for termination of sockets
		pState->m_pfnSockTerm = &AfxSocketTerm;
	}

	return TRUE;
}

