

#include "mtldef.h"
#include "mtlinl.h"
#include "mtltcls.h"
#include "mtlafx.h"
#include "mtlfile.h"
#include "mtlgdi.h"
#include "mtlmenu.h"

#include "mtlwmh.h"
#include "mtlddx.h"
#include "mtlwin.h"
#include "mtlwinct.h"
#include "mtlwind.h"
#include "mtldlg.h"
#include "mtlprop.h"
#include "mtlbar.h"
#include "mtlbardk.h"
#include "mtlfrm.h"
#include "mtlcdlgs.h"

#include "mtlview.h"
#include "mtlviewct.h"
#include "mtldoc.h"
#include "mtlmdi.h"

#include "mtlwinsp.h"


#define _afxFileSection _T("Recent File List")
#define _afxFileEntry _T("File%d")
#define _afxPreviewSection _T("Settings")
#define _afxPreviewEntry _T("PreviewPages")




class CModuleThreadState : public CThreadState
{
	public:
		CModuleThreadState()
		{
			m_pToolTip = NULL;
			m_pLastHit = NULL;       // last window to own tooltip
			m_nLastHit = 0;         // last hittest code
			m_pLastInfo = NULL;    // last TOOLINFO structure
			m_nLastStatus = 0;      // last flyby status message
			m_pLastStatus = NULL; // last flyby status control bar

			AfxGetModuleState()->m_thread = this;

		}

	public:
		// list of CFrameWnd objects for thread
		CList<CFrameWnd*> m_frameList;

		// common controls thread state
		CToolTipCtrl*	m_pToolTip;
		CWnd*			m_pLastHit;       // last window to own tooltip
		INT_PTR			m_nLastHit;         // last hittest code
		TOOLINFO*		m_pLastInfo;    // last TOOLINFO structure
		INT_PTR			m_nLastStatus;      // last flyby status message
		CControlBar*	m_pLastStatus; // last flyby status control bar

		CHandleHashList<HWND, CWMHnd>	m_cWnd;

		CHandleList<HMENU, CMenu>	m_cChildMenu;

		CHandleList<HDC, CDC>		m_cChildDC;
		CHandleList<HGDIOBJ, CGdiObject>	m_cChildGdiobj;

		CHandleList<HANDLE, void>	m_cLocalHandle;


	public:
		CWMHnd* FromHandleWnd(HWND hWnd, CWMHnd* pCWMHnd = NULL, HWND hWndOwner = NULL, HANDLE_FUNC_NEW pFuncNew = NULL)
		{
			if (hWnd == NULL) return NULL ;

			return m_cWnd.FromHandle(hWnd, pCWMHnd, hWndOwner, pFuncNew);
		}
		HWND RemoveHandleWnd(HWND hWnd)
		{
			if (hWnd == NULL) return NULL;

			//TRACE("RemoveHandleWnd>S:m_cChildMenu(%d), m_cChildDC(%d), m_cChildGdiobj(%d), m_cLocalHandle(%d)\n",
			//		m_cChildMenu.GetCount(), m_cChildDC.GetCount(), m_cChildGdiobj.GetCount(), m_cLocalHandle.GetCount()) ;

			HWND hRemoveWnd = ((void*)hWnd != HANDLE_ADDRESS_INVALID) ? m_cWnd.RemoveHandle(hWnd) : NULL;

			m_cWnd.RemoveOwner(hWnd);

			m_cChildMenu.RemoveOwner(hWnd);
			m_cChildDC.RemoveOwner(hWnd);
			m_cChildGdiobj.RemoveOwner(hWnd);
			m_cLocalHandle.RemoveOwner(hWnd);

			//TRACE("RemoveHandleWnd>E:m_cChildMenu(%d), m_cChildDC(%d), m_cChildGdiobj(%d), m_cLocalHandle(%d), m_cWnd(%d)\n",
			//	m_cChildMenu.GetCount(), m_cChildDC.GetCount(), m_cChildGdiobj.GetCount(), m_cLocalHandle.GetCount(), m_cWnd.GetCount());

			return hRemoveWnd;
		}


		CMenu* FromHandleMenu(HMENU hMenu, CMenu* pCMenu = NULL, HWND hWndOwner = NULL, HANDLE_FUNC_NEW pFuncNew = NULL)
		{
			if (hMenu == NULL) return NULL;

			return m_cChildMenu.FromHandle(hMenu, pCMenu, hWndOwner, pFuncNew);
		}
		HMENU RemoveHandleMenu(HMENU hMenu)
		{
			if (hMenu == NULL) return NULL;

			return m_cChildMenu.RemoveHandle(hMenu);
		}


		CDC* FromHandleDC(HDC hDC, CDC* pCDC = NULL, HWND hWndOwner = NULL, HANDLE_FUNC_NEW pFuncNew = NULL)
		{
			if (hDC == NULL) return NULL;

			return m_cChildDC.FromHandle(hDC, pCDC, hWndOwner, pFuncNew);
		}
		HDC RemoveHandleDC(HDC hDC)
		{
			if (hDC == NULL) return NULL;

			return m_cChildDC.RemoveHandle(hDC);
		}


		CGdiObject* FromHandleGdiobj(HGDIOBJ hGdiobj, CGdiObject* pCGdiObject = NULL, HWND hWndOwner = NULL, HANDLE_FUNC_NEW pFuncNew = NULL)
		{
			if (hGdiobj == NULL) return NULL;

			return m_cChildGdiobj.FromHandle(hGdiobj, pCGdiObject, hWndOwner, pFuncNew);
		}
		HGDIOBJ RemoveHandleGdiobj(HGDIOBJ hGdiobj)
		{
			if (hGdiobj == NULL) return NULL;

			return m_cChildGdiobj.RemoveHandle(hGdiobj);
		}


		void* FromHandleHnd(HANDLE hHandle, void* pVoid = NULL, HWND hWndOwner = NULL, HANDLE_FUNC_NEW pFuncNew = NULL)
		{
			if (hHandle == NULL) return NULL;

			return m_cLocalHandle.FromHandle(hHandle, pVoid, hWndOwner, pFuncNew);
		}
		HANDLE RemoveHandleHnd(HANDLE hHandle)
		{
			if (hHandle == NULL) return NULL;

			return m_cLocalHandle.RemoveHandle(hHandle);
		}



};

#define AFX_MODULE_THREAD_STATE CModuleThreadState

class CWinThread ;

struct _AFX_THREAD_STARTUP
{
	// following are "in" parameters to thread startup
	_AFX_THREAD_STATE* pThreadState;    // thread state of parent thread
	CWinThread* pThread;    // CWinThread for new thread
	DWORD dwCreateFlags;    // thread creation flags
	//_PNH pfnNewHandler;     // new handler for new thread

	HANDLE hEvent;          // event triggered after success/non-success
	HANDLE hEvent2;         // event triggered after thread is resumed

	// strictly "out" -- set after hEvent is triggered
	BOOL bError;    // TRUE if error during startup
};


class CWinThread : public CCmdTarget
{
	public:
		CWinThread()
		{
			CWinThread(NULL, NULL) ;
		}

		CWinThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam)
		{
			// most threads are deleted when not needed
			m_bAutoDelete = TRUE;
			m_nThreadID = 0;

			m_pfnThreadProc = pfnThreadProc;
			m_pThreadParams = pParam;

			// no HTHREAD until it is created
			m_hThread = NULL;

			m_pMainWnd = NULL ;
		}

		~CWinThread()
		{
			// free thread object
			if (m_hThread != NULL)
				CloseHandle(m_hThread);
		}

	public:
		BOOL m_bAutoDelete;     // enables 'delete this' after thread termination
		DWORD m_nThreadID;      // this thread's ID

		// only valid while running
		HANDLE m_hThread;       // this thread's HANDLE

		// valid after construction
		LPVOID m_pThreadParams; // generic parameters passed to starting function
		AFX_THREADPROC m_pfnThreadProc;

		// Attributes
		CWnd* m_pMainWnd;       // main window (usually same AfxGetApp()->m_pMainWnd)


	public:
		operator HANDLE() const { return this == NULL ? NULL : m_hThread; }


		static UINT _STDCALL _AfxThreadEntry(void* pParam)
		{
			_AFX_THREAD_STARTUP* pStartup = (_AFX_THREAD_STARTUP*)pParam;
			ASSERT(pStartup != NULL);
			ASSERT(pStartup->pThreadState != NULL);
			ASSERT(pStartup->pThread != NULL);
			ASSERT(pStartup->hEvent != NULL);
			ASSERT(!pStartup->bError);

			CWinThread* pThread = pStartup->pThread;

			// pStartup is invlaid after the following
			// SetEvent (but hEvent2 is valid)
			HANDLE hEvent2 = pStartup->hEvent2;

			// allow the creating thread to return from CWinThread::CreateThread
			VERIFY(::SetEvent(pStartup->hEvent));

			// wait for thread to be resumed
			VERIFY(::WaitForSingleObject(hEvent2, INFINITE) == WAIT_OBJECT_0);
			::CloseHandle(hEvent2);

			// first -- check for simple worker thread
			DWORD nResult = 0;
			if (pThread->m_pfnThreadProc != NULL)
			{
				nResult = (*pThread->m_pfnThreadProc)(pThread->m_pThreadParams);
				ASSERT_VALID(pThread);
			}

			// cleanup and shutdown the thread
			AfxEndThread(nResult);

			return 0;   // not reached
		}

		static void AfxEndThread(UINT nExitCode, BOOL bDelete = TRUE)
		{
			// allow C-runtime to cleanup, and exit the thread
			_endthreadex(nExitCode);
		}


		BOOL CreateThread(DWORD dwCreateFlags, UINT nStackSize, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL)
		{
			ENSURE(m_hThread == NULL);  // already created?

			// setup startup structure for thread initialization
			_AFX_THREAD_STARTUP startup; memset(&startup, 0, sizeof(startup));
			startup.pThreadState = AfxGetThreadState();
			startup.pThread = this;
			startup.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
			startup.hEvent2 = ::CreateEvent(NULL, TRUE, FALSE, NULL);
			startup.dwCreateFlags = dwCreateFlags;
			if (startup.hEvent == NULL || startup.hEvent2 == NULL)
			{
				TRACE(traceAppMsg, 0, "Warning: CreateEvent failed in CWinThread::CreateThread.\n");
				if (startup.hEvent != NULL)
					::CloseHandle(startup.hEvent);
				if (startup.hEvent2 != NULL)
					::CloseHandle(startup.hEvent2);
				return FALSE;
			}

			// create the thread (it may or may not start to run)
			m_hThread = (HANDLE)(ULONG_PTR)_beginthreadex(lpSecurityAttrs, nStackSize, &_AfxThreadEntry, &startup,
															dwCreateFlags | CREATE_SUSPENDED, (UINT*)&m_nThreadID);
			if (m_hThread == NULL)
			{
				::CloseHandle(startup.hEvent);
				::CloseHandle(startup.hEvent2);
				return FALSE;
			}

			// start the thread just for MFC initialization
			VERIFY(ResumeThread() != (DWORD)-1);
			VERIFY(::WaitForSingleObject(startup.hEvent, INFINITE) == WAIT_OBJECT_0);
			::CloseHandle(startup.hEvent);

			// if created suspended, suspend it until resume thread wakes it up
			if (dwCreateFlags & CREATE_SUSPENDED)
				VERIFY(::SuspendThread(m_hThread) != (DWORD)-1);

			// if error during startup, shut things down
			if (startup.bError)
			{
				VERIFY(::WaitForSingleObject(m_hThread, INFINITE) == WAIT_OBJECT_0);
				::CloseHandle(m_hThread);
				m_hThread = NULL;
				::CloseHandle(startup.hEvent2);
				return FALSE;
			}

			// allow thread to continue, once resumed (it may already be resumed)
			VERIFY(::SetEvent(startup.hEvent2));
			return TRUE;
		}

		void Delete()
		{
			// delete thread if it is auto-deleting
			if (m_bAutoDelete)
				delete this;
		}

		int GetThreadPriority() { ASSERT(m_hThread != NULL); return ::GetThreadPriority(m_hThread); }

		BOOL PostThreadMessage(UINT message, WPARAM wParam, LPARAM lParam) 
		{ 
			ASSERT(m_hThread != NULL); return ::PostThreadMessage(m_nThreadID, message, wParam, lParam); 
		}

		DWORD ResumeThread() { ASSERT(m_hThread != NULL); return ::ResumeThread(m_hThread); }
		
		BOOL SetThreadPriority(int nPriority) { ASSERT(m_hThread != NULL); return ::SetThreadPriority(m_hThread, nPriority); }
		DWORD SuspendThread() { ASSERT(m_hThread != NULL); return ::SuspendThread(m_hThread); }

};

_INLINE CWinThread* AfxBeginThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam,
									int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0,
									DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL)
{
	ASSERT(pfnThreadProc != NULL);

	CWinThread* pThread = DEBUG_NEW CWinThread(pfnThreadProc, pParam);
	ASSERT_VALID(pThread);

	if (!pThread->CreateThread(dwCreateFlags | CREATE_SUSPENDED, nStackSize, lpSecurityAttrs))
	{
		pThread->Delete();
		return NULL;
	}
	VERIFY(pThread->SetThreadPriority(nPriority));
	if (!(dwCreateFlags & CREATE_SUSPENDED))
		VERIFY(pThread->ResumeThread() != (DWORD)-1);

	return pThread;
}




class CWinApp : public CWinThread, public CModuleThreadState, public CWinMsg
{
	public:
		CWinApp()
		{
			CAfx::Init() ;

			m_pCWinApp = this ;

			m_hInstance = NULL;
			m_pszHelpFilePath = NULL;
			m_pszProfileName = NULL;
			m_pszRegistryKey = NULL;
			m_pszExeName = NULL;
			m_pszAppID = NULL;

#ifndef MTL_REMOVE_CWINAPP_RECENTFILELIST
			m_pRecentFileList = NULL;
#endif	// MTL_REMOVE_CWINAPP_RECENTFILELIST
#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER
			m_pDocManager = NULL;
#endif	// MTL_REMOVE_CWINAPP_DOCMANAGER

			m_lpCmdLine = NULL;

			m_bTaskbarInteractionEnabled = TRUE;

			m_nWaitCursorCount = 0;
			m_hcurWaitCursorRestore = NULL ;

#ifndef MTL_REMOVE_CWINAPP_PRINT	// initialize current printer state
			m_hDevMode = NULL;
			m_hDevNames = NULL;
			m_nNumPreviewPages = 0;     // not specified (defaults to 1)
#endif // MTL_REMOVE_CWINAPP_PRINT

			m_pCmdInfo = NULL ;

			m_atomApp = NULL ;
			m_atomSystemTopic = NULL ;

			m_bRemoveFromMRU = FALSE ;

		}

		~CWinApp()
		{
#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER
			// free doc manager
			if (m_pDocManager != NULL)
				delete m_pDocManager;
#endif	// MTL_REMOVE_CWINAPP_DOCMANAGER

#ifndef MTL_REMOVE_CWINAPP_RECENTFILELIST
			// free recent file list
			if (m_pRecentFileList != NULL)
				delete m_pRecentFileList;
#endif	// MTL_REMOVE_CWINAPP_RECENTFILELIST

			// free cached commandline
			if (m_pCmdInfo != NULL)
				delete m_pCmdInfo;

			// free atoms if used
			if (m_atomApp != NULL)
				::GlobalDeleteAtom(m_atomApp);
			if (m_atomSystemTopic != NULL)
				::GlobalDeleteAtom(m_atomSystemTopic);

			// free various strings allocated with _tcsdup
			free((void*)m_pszAppName);
			free((void*)m_pszRegistryKey);
			free((void*)m_pszExeName);
			free((void*)m_pszHelpFilePath);
			free((void*)m_pszProfileName);

			CAfx::Term();
		}


	public:
		// メッセージマップ
		DECLARE_MESSAGE_MAP()

	public:
		static CWinApp* m_pCWinApp;

		// Restart Manager support
		DWORD m_dwRestartManagerSupportFlags; // What aspects of restart/recovery does the application support?


		// This module's hInstance.
		HINSTANCE m_hInstance;

		// Pointer to the command-line.
		LPTSTR m_lpCmdLine;

		// Initial state of the application's window; normally,
		// this is an argument to ShowWindow().
		int m_nCmdShow;

		/// Application User Model ID.</summary>
		LPCTSTR m_pszAppID;

		// Running args (can be changed in InitInstance)

		// Human-redable name of the application. Normally set in
		// constructor or retreived from AFX_IDS_APP_TITLE.
		LPCTSTR m_pszAppName;

		// Name of registry key for this application. See
		// SetRegistryKey() member function.
		LPCTSTR m_pszRegistryKey;

		// Executable name (no spaces).
		LPCTSTR m_pszExeName;

		// Default based on this module's path.
		LPCTSTR m_pszHelpFilePath;

		// Default based on this application's name.
		LPCTSTR m_pszProfileName;



#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER
		// Pointer to CDocManager used to manage document templates
		// for this application instance.
		CDocManager* m_pDocManager;
#endif	// MTL_REMOVE_CWINAPP_DOCMANAGER

		BOOL m_bTaskbarInteractionEnabled;


		int m_nWaitCursorCount;         // for wait cursor (>0 => waiting)
		HCURSOR m_hcurWaitCursorRestore; // old cursor to restore after wait cursor

#ifndef MTL_REMOVE_CWINAPP_RECENTFILELIST
		CRecentFileList* m_pRecentFileList;
#endif	// MTL_REMOVE_CWINAPP_RECENTFILELIST

#ifndef MTL_REMOVE_CWINAPP_PRINT	// initialize current printer state
		HGLOBAL m_hDevMode;             // printer Dev Mode
		HGLOBAL m_hDevNames;            // printer Device Names
		UINT m_nNumPreviewPages; // Desired number of preview pages
#endif // MTL_REMOVE_CWINAPP_PRINT

		CCommandLineInfo* m_pCmdInfo;

		ATOM m_atomApp, m_atomSystemTopic;   // for DDE open

		BOOL m_bRemoveFromMRU ;

		// help mode used by the app
		AFX_HELP_TYPE m_eHelpType;

	public:

		static CModuleState* GetCModuleState()
		{
			static CModuleState cModuleState ;
		
			return &cModuleState ;
		}


		void OnAppExit()
		{
			// same as double-clicking on main window close box
			ASSERT(m_pMainWnd != NULL);
			m_pMainWnd->SendMessage(WM_CLOSE);
		}

		void OnFilePrintSetup()
		{
			CPrintDialog pd(TRUE);
			DoPrintDialog(&pd);
		}


#ifndef MTL_REMOVE_CWINAPP_RECENTFILELIST
		BOOL OnOpenRecentFile(UINT nID)
		{
			ASSERT_VALID(this);
			ENSURE(m_pRecentFileList != NULL);

			ENSURE_ARG(nID >= ID_FILE_MRU_FILE1);
			ENSURE_ARG(nID < ID_FILE_MRU_FILE1 + (UINT)m_pRecentFileList->GetSize());
			int nIndex = nID - ID_FILE_MRU_FILE1;
			ASSERT((*m_pRecentFileList)[nIndex].GetLength() != 0);

			TRACE(traceAppMsg, 0, "MRU: open file (%d) '%s'.\n", (nIndex)+1, (LPCTSTR)(*m_pRecentFileList)[nIndex]);

			m_bRemoveFromMRU = TRUE;
			if (OpenDocumentFile((*m_pRecentFileList)[nIndex]) == NULL)
			{
				if (m_bRemoveFromMRU)
				{
					m_pRecentFileList->Remove(nIndex);
				}
			}
			m_bRemoveFromMRU = TRUE;

			return TRUE;
		}

		void OnUpdateRecentFileMenu(CCmdUI* pCmdUI)
		{
			ASSERT_VALID(this);
			ENSURE_ARG(pCmdUI != NULL);
			if (m_pRecentFileList == NULL) // no MRU files
				pCmdUI->Enable(FALSE);
			else
				m_pRecentFileList->UpdateMenu(pCmdUI);
		}


		void AddToRecentFileList(LPCTSTR lpszPathName)
		{
			ASSERT_VALID(this);
			ENSURE_ARG(lpszPathName != NULL);

			if (m_pRecentFileList != NULL)
			{
				m_pRecentFileList->Add(lpszPathName, m_pszAppID);
			}
		}
#endif	// MTL_REMOVE_CWINAPP_RECENTFILELIST


	public:
		virtual void OnFileNew()
		{
#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER
			if (m_pDocManager != NULL)
				m_pDocManager->OnFileNew();
#endif	// MTL_REMOVE_CWINAPP_DOCMANAGER
		}

		/////////////////////////////////////////////////////////////////////////////

		virtual void OnFileOpen()
		{
#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER
			m_pDocManager->OnFileOpen();
#endif	// MTL_REMOVE_CWINAPP_DOCMANAGER
		}

		// override to change idle processing
		virtual BOOL OnIdle(int lCount)
		{
			ASSERT_VALID(this);

			if (lCount <= 0)
			{
				// send WM_IDLEUPDATECMDUI to the main window
				CWnd* pMainWnd = (CWnd*)m_pMainWnd;
				if (pMainWnd != NULL && pMainWnd->m_hWnd != NULL && pMainWnd->IsWindowVisible())
				{
					pMainWnd->WindowProc(WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
					pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0, TRUE, TRUE);
				}

				// send WM_IDLEUPDATECMDUI to all frame windows
				AFX_MODULE_THREAD_STATE* pState = AfxGetModuleState()->m_thread;

				POSITION pos = pState->m_frameList.GetHeadPosition() ;

				while (pos != NULL)
				{
					CFrameWnd* pFrameWnd = pState->m_frameList.GetNext(pos);

					if (pFrameWnd->m_hWnd != NULL && pFrameWnd != pMainWnd)
					{
						if (pFrameWnd->m_nShowDelay == SW_HIDE)
							pFrameWnd->ShowWindow(pFrameWnd->m_nShowDelay);
						if (pFrameWnd->IsWindowVisible() || pFrameWnd->m_nShowDelay >= 0)
						{
							pFrameWnd->WindowProc(WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
							pFrameWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0, TRUE, TRUE);
						}
						if (pFrameWnd->m_nShowDelay > SW_HIDE)
							pFrameWnd->ShowWindow(pFrameWnd->m_nShowDelay);
						pFrameWnd->m_nShowDelay = -1;
					}

				}

#if 0
#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER
				// call doc-template idle hook
				pos = NULL;
				if (m_pDocManager != NULL)
					pos = m_pDocManager->GetFirstDocTemplatePosition();

				while (pos != NULL)
				{
					CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(pos);
					ASSERT_KINDOF(CDocTemplate, pTemplate);
					// if auto-save is enabled, the auto-save will be performed in the
					// document's OnIdle method, called from the template's OnIdle method
					pTemplate->OnIdle();
				}
#endif // MTL_REMOVE_CWINAPP_DOCMANAGER
#endif
			}
			else if (lCount >= 0)
			{
				TRACE("OnIdle>:lCount=%d\n", lCount) ;
				if (lCount <= 1)
				{
					TRACE("OnIdle>S:m_cChildMenu(%d), m_cChildDC(%d), m_cChildGdiobj(%d), m_cLocalHandle(%d), m_cWnd(%d)\n",
						m_cChildMenu.GetCount(), m_cChildDC.GetCount(), m_cChildGdiobj.GetCount(), m_cLocalHandle.GetCount(), m_cWnd.GetCount());

					RemoveHandleWnd((HWND)HANDLE_ADDRESS_INVALID);

					TRACE("OnIdle>E:m_cChildMenu(%d), m_cChildDC(%d), m_cChildGdiobj(%d), m_cLocalHandle(%d), m_cWnd(%d)\n",
						m_cChildMenu.GetCount(), m_cChildDC.GetCount(), m_cChildGdiobj.GetCount(), m_cLocalHandle.GetCount(), m_cWnd.GetCount());
				}
			}

			return lCount < 1;  // nothing more to do if lCount >= 1
		}

		// Help Command Handlers
		void OnHelp()
		{
			((CFrameWnd*)m_pMainWnd)->OnHelp();
		}



#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER
		void AddDocTemplate(CDocTemplate* pTemplate)
		{
			if (m_pDocManager == NULL)
				m_pDocManager = new CDocManager();
			m_pDocManager->AddDocTemplate(pTemplate);
		}
#endif	// MTL_REMOVE_CWINAPP_DOCMANAGER


		void CloseAllDocuments(BOOL bEndSession)
		{
#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER
			if (m_pDocManager != NULL)
				m_pDocManager->CloseAllDocuments(bEndSession);
#endif	// MTL_REMOVE_CWINAPP_DOCMANAGER
		}

		// Under Win32, a reg key may not be deleted unless it is empty.
		// Thus, to delete a tree,  one must recursively enumerate and
		// delete all of the sub-keys.

		LONG DelRegTree(HKEY hParentKey, const CString& strKeyName, CAtlTransactionManager* pTM = NULL)
		{
			return AfxDelRegTreeHelper(hParentKey, strKeyName, pTM);
		}

		void DoEnableModeless(BOOL bEnable) { }

		int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt)
		{
			return ShowAppMessageBox(this, lpszPrompt, nType, nIDPrompt);
		}

		void DoWaitCursor(int nCode)
		{
			// 0 => restore, 1=> begin, -1=> end
			ENSURE_ARG(nCode == 0 || nCode == 1 || nCode == -1);

			m_nWaitCursorCount += nCode;
			if (m_nWaitCursorCount > 0)
			{
				HCURSOR hcurPrev = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
				if (nCode > 0 && m_nWaitCursorCount == 1)
					m_hcurWaitCursorRestore = hcurPrev;
			}
			else
			{
				// turn everything off
				m_nWaitCursorCount = 0;     // prevent underflow
				::SetCursor(m_hcurWaitCursorRestore);
			}

		}


		// prompt for file name - used for open and save as
		BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
			// if pTemplate==NULL => all document templates
		{
#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER
			ENSURE(m_pDocManager != NULL);
			return m_pDocManager->DoPromptFileName(fileName, nIDSTitle, lFlags, bOpenFileDialog, pTemplate);
#else
			return FALSE ;
#endif	// MTL_REMOVE_CWINAPP_DOCMANAGER
		}


		INT_PTR DoPrintDialog(CPrintDialog* pPD)
		{
#ifndef MTL_REMOVE_CWINAPP_PRINT
			ENSURE_VALID(pPD);
			UpdatePrinterSelection(FALSE);

			pPD->m_pd.hDevMode = m_hDevMode;
			pPD->m_pd.hDevNames = m_hDevNames;
			INT_PTR nResponse = pPD->DoModal();

			// if OK or Cancel is selected we need to update cached devMode/Names
			while (nResponse != IDOK && nResponse != IDCANCEL)
			{
				switch (CommDlgExtendedError())
				{
					// CommDlg cannot give these errors after NULLing these handles
					case PDERR_PRINTERNOTFOUND:
					case PDERR_DNDMMISMATCH:
					case PDERR_DEFAULTDIFFERENT:
						if (pPD->m_pd.hDevNames != NULL)
						{
							ASSERT(m_hDevNames == pPD->m_pd.hDevNames);
							AfxGlobalFree(pPD->m_pd.hDevNames);
							pPD->m_pd.hDevNames = NULL;
							m_hDevNames = NULL;
						}

						if (pPD->m_pd.hDevMode)
						{
							ASSERT(m_hDevMode == pPD->m_pd.hDevMode);
							AfxGlobalFree(pPD->m_pd.hDevMode);
							pPD->m_pd.hDevMode = NULL;
							m_hDevMode = NULL;
						}
						break;

					default:
						return nResponse;       // do not update cached devMode/Names
				}

				nResponse = pPD->DoModal();
			}

			// refresh current CWinApp cache of printer device information
			m_hDevMode = pPD->m_pd.hDevMode;
			m_hDevNames = pPD->m_pd.hDevNames;

			return nResponse;
#else // MTL_REMOVE_CWINAPP_PRINT
			return IDCANCEL ;
#endif // MTL_REMOVE_CWINAPP_PRINT
		}

		BOOL Enable3dControls() { return TRUE; }
		BOOL Enable3dControlsStatic() { return TRUE; }

		void EnableModeless(BOOL bEnable) { DoEnableModeless(bEnable); }

		void EnableShellOpen()
		{
			ASSERT(m_atomApp == NULL && m_atomSystemTopic == NULL); // do once
			if (m_atomApp != NULL || m_atomSystemTopic != NULL)
			{
				return;
			}

			CString strShortName;
			AfxGetModuleFileName(AfxGetInstanceHandle(), strShortName);

			// strip out path
			CString strFileName = ::PathFindFileName(strShortName);
			// strip out extension
			LPTSTR pszFileName = (LPTSTR)(LPCTSTR)strFileName ;
			::PathRemoveExtension(pszFileName);
			strFileName.ReleaseBuffer();

			m_atomApp = ::GlobalAddAtom(strFileName);
			m_atomSystemTopic = ::GlobalAddAtom(_T("system"));
		}


		BOOL EnableTaskbarInteraction(BOOL bEnable = TRUE)
		{
			if (m_pMainWnd != NULL)
				return FALSE;

			m_bTaskbarInteractionEnabled = bEnable;
			return TRUE;
		}

		virtual int ExitInstance() // default will 'delete this'
		{
			if (m_pCmdInfo == NULL ||
				(m_pCmdInfo->m_nShellCommand != CCommandLineInfo::AppUnregister &&
				m_pCmdInfo->m_nShellCommand != CCommandLineInfo::AppRegister))
			{
#ifndef _USRDLL
				SaveStdProfileSettings();
#endif
			}

			return 0;
		}

		// returns key for HKEY_CURRENT_USER\"Software"\RegistryKey\ProfileName
		// creating it if it doesn't exist
		// responsibility of the caller to call RegCloseKey() on the returned HKEY
		HKEY GetAppRegistryKey(CAtlTransactionManager* pTM = NULL)
		{
			HKEY hAppKey = NULL;
			HKEY hSoftKey = NULL;
			HKEY hCompanyKey = NULL;

			LSTATUS lStatus = ::RegOpenKeyEx(HKEY_CURRENT_USER, _T("software"), 0, KEY_WRITE | KEY_READ, &hSoftKey);

			if (lStatus == ERROR_SUCCESS)
			{
				DWORD dw;

				lStatus = ::RegCreateKeyEx(hSoftKey, m_pszRegistryKey, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL, &hCompanyKey, &dw);

				if (lStatus == ERROR_SUCCESS)
				{
					lStatus = ::RegCreateKeyEx(hCompanyKey, m_pszProfileName, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL, &hAppKey, &dw);
				}
			}
			if (hSoftKey != NULL)
				RegCloseKey(hSoftKey);
			if (hCompanyKey != NULL)
				RegCloseKey(hCompanyKey);

			return hAppKey;
		}

		BOOL GetPrinterDeviceDefaults(PRINTDLG* pPrintDlg)
		{
#ifndef MTL_REMOVE_CWINAPP_PRINT
			UpdatePrinterSelection(m_hDevNames == NULL); //force default if no current
			if (m_hDevNames == NULL)
				return FALSE;               // no printer defaults

			ENSURE_ARG(pPrintDlg != NULL);
			pPrintDlg->hDevNames = m_hDevNames;
			pPrintDlg->hDevMode = m_hDevMode;

			::GlobalUnlock(m_hDevNames);
			::GlobalUnlock(m_hDevMode);
			return TRUE;
#else // MTL_REMOVE_CWINAPP_PRINT
			return FALSE;               // no printer defaults
#endif // MTL_REMOVE_CWINAPP_PRINT
		}

		// returns key for:
		//      HKEY_CURRENT_USER\"Software"\RegistryKey\AppName\lpszSection
		// creating it if it doesn't exist.
		// responsibility of the caller to call RegCloseKey() on the returned HKEY
		HKEY GetSectionKey(LPCTSTR lpszSection, CAtlTransactionManager* pTM = NULL)
		{
			HKEY hSectionKey = NULL;
			HKEY hAppKey = GetAppRegistryKey(pTM);
			if (hAppKey == NULL)
				return NULL;

			DWORD dw;

			::RegCreateKeyEx(hAppKey, lpszSection, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL, &hSectionKey, &dw);

			RegCloseKey(hAppKey);
			return hSectionKey;
		}

		UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
		{
#ifndef MTL_REMOVE_CWINAPP_PROFILEREGISTRY
			if (m_pszRegistryKey != NULL) // use registry
			{
				HKEY hSecKey = GetSectionKey(lpszSection);
				if (hSecKey == NULL)
					return nDefault;
				DWORD dwValue;
				DWORD dwType;
				DWORD dwCount = sizeof(DWORD);
				LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType, (LPBYTE)&dwValue, &dwCount);
				RegCloseKey(hSecKey);
				if (lResult == ERROR_SUCCESS)
				{
					return (UINT)dwValue;
				}
				return nDefault;
			}
			else
#endif // MTL_REMOVE_CWINAPP_PROFILEREGISTRY
			{
				ASSERT(m_pszProfileName != NULL);
				return ::GetPrivateProfileInt(lpszSection, lpszEntry, nDefault, m_pszProfileName);
			}
		}

		CString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL)
		{
#ifndef MTL_REMOVE_CWINAPP_PROFILEREGISTRY
			if (m_pszRegistryKey != NULL)
			{
				HKEY hSecKey = GetSectionKey(lpszSection);
				if (hSecKey == NULL)
					return lpszDefault;
				CString strValue;
				DWORD dwType = REG_NONE;
				DWORD dwCount = 0;
				LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType, NULL, &dwCount);
				if (lResult == ERROR_SUCCESS)
				{
					lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType, (LPBYTE)strValue.GetBuffer(dwCount / sizeof(TCHAR)), &dwCount);
					strValue.ReleaseBuffer();
				}
				RegCloseKey(hSecKey);
				if (lResult == ERROR_SUCCESS)
				{
					return strValue;
				}
				return lpszDefault;
			}
			else
#endif // MTL_REMOVE_CWINAPP_PROFILEREGISTRY
			{
				ASSERT(m_pszProfileName != NULL);

				if (lpszDefault == NULL)
					lpszDefault = _T("");	// don't pass in NULL
				TCHAR szT[4096];
				DWORD dw = ::GetPrivateProfileString(lpszSection, lpszEntry, lpszDefault, szT, _countof(szT), m_pszProfileName);
				ASSERT(dw < 4095);
				return szT;
			}
		}


		void HideApplication()
		{
			ASSERT_VALID(m_pMainWnd);

			// hide the application's windows before closing all the documents
			m_pMainWnd->ShowWindow(SW_HIDE);
			m_pMainWnd->ShowOwnedPopups(FALSE);

			// put the window at the bottom of zorder, so it isn't activated
			m_pMainWnd->SetWindowPos(&CWnd::wndBottom, 0, 0, 0, 0,
										SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		}

		virtual BOOL InitInstance()
		{
			return TRUE ;
		}

		static BOOL IsIdleMessage(MSG* pMsg)
		{
			// These messages should NOT cause idle processing
			switch (pMsg->message)
			{
				case WM_MOUSEMOVE:
#ifndef _WIN32_WCE
				case WM_NCMOUSEMOVE:
#endif // !_WIN32_WCE
				case WM_PAINT:
				case 0x0118:	// WM_SYSTIMER (caret blink)
					return FALSE;
			}

			return TRUE;
		}


		HCURSOR LoadCursor(LPCTSTR lpszResourceName) const
		{
			return ::LoadCursor(AfxFindResourceHandle(lpszResourceName, ATL_RT_GROUP_CURSOR), 
								lpszResourceName);
		}
		HCURSOR LoadCursor(UINT nIDResource) const
		{
			return ::LoadCursorW(AfxFindResourceHandle(ATL_MAKEINTRESOURCE(nIDResource), ATL_RT_GROUP_CURSOR), 
								ATL_MAKEINTRESOURCEW(nIDResource));
		}
		HICON LoadIcon(LPCTSTR lpszResourceName) const
		{
			return ::LoadIcon(AfxFindResourceHandle(lpszResourceName, ATL_RT_GROUP_ICON), 
								lpszResourceName);
		}
		HICON LoadIcon(UINT nIDResource) const
		{
			return ::LoadIconW(AfxFindResourceHandle(ATL_MAKEINTRESOURCE(nIDResource), ATL_RT_GROUP_ICON), 
								ATL_MAKEINTRESOURCEW(nIDResource));
		}
		HCURSOR LoadOEMCursor(UINT nIDCursor) const
		{
			return ::LoadCursorW(NULL, ATL_MAKEINTRESOURCEW(nIDCursor));
		}
		HICON LoadOEMIcon(UINT nIDIcon) const
		{
			return ::LoadIconW(NULL, ATL_MAKEINTRESOURCEW(nIDIcon));
		}
		HCURSOR LoadStandardCursor(LPCTSTR lpszCursorName) const
		{
			return ::LoadCursor(NULL, lpszCursorName);
		}
		HICON LoadStandardIcon(LPCTSTR lpszIconName) const
		{
			return ::LoadIcon(NULL, lpszIconName);
		}

		// Load MRU file list and last preview state.
		void LoadStdProfileSettings(UINT nMaxMRU = _AFX_MRU_COUNT)
		{
			if (nMaxMRU != 0)
			{
#ifndef MTL_REMOVE_CWINAPP_RECENTFILELIST
				// create file MRU since nMaxMRU not zero
				m_pRecentFileList = new CRecentFileList(0, _afxFileSection, _afxFileEntry, nMaxMRU);
				m_pRecentFileList->ReadList();
#endif	// MTL_REMOVE_CWINAPP_RECENTFILELIST
			}

#ifndef MTL_REMOVE_CWINAPP_PRINT
			// 0 by default means not set
			m_nNumPreviewPages = GetProfileInt(_afxPreviewSection, _afxPreviewEntry, 0);
#endif // MTL_REMOVE_CWINAPP_PRINT
		}

		virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName)
		{
#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER
			return m_pDocManager->OpenDocumentFile(lpszFileName);
#else
			return NULL ;
#endif	// MTL_REMOVE_CWINAPP_DOCMANAGER
		}

		// Parse the command line for stock options and commands.
		void ParseCommandLine(CCommandLineInfo& rCmdInfo)
		{
			for (int i = 1; i < __argc; i++)
			{
				LPCTSTR pszParam = __targv[i];
				BOOL bFlag = FALSE;
				BOOL bLast = ((i + 1) == __argc);
				if (pszParam[0] == '-' || pszParam[0] == '/')
				{
					// remove flag specifier
					bFlag = TRUE;
					++pszParam;
				}
				rCmdInfo.ParseParam(pszParam, bFlag, bLast);
			}
		}


		virtual BOOL PreTranslateMessage(MSG* pMsg)
		{
			// walk from target to main window

			HWND hWnd ;

			CWMHnd* pMainWnd = m_pMainWnd;
			if (m_pMainWnd != NULL)
				hWnd = pMainWnd->GetSafeHwnd() ;
			else
				hWnd = ::GetActiveWindow();

			if (CWnd::WalkPreTranslateTree(hWnd, pMsg))
				return TRUE;

			// in case of modeless dialogs, last chance route through main
			//   window's accelerator table
			if (pMainWnd != NULL)
			{
				HWND hMsgWnd = pMsg->hwnd ;
				if (CWMHnd::GetTopLevelParent(hMsgWnd) != hWnd)
					return pMainWnd->PreTranslateMessage(pMsg);
			}

			return FALSE;   // no special processing
		}

		BOOL ProcessShellCommand(CCommandLineInfo& rCmdInfo)
		{
			BOOL bResult = TRUE;
			switch (rCmdInfo.m_nShellCommand)
			{
				case CCommandLineInfo::FileNew:
					if (!AfxGetApp()->OnCmdMsg(ID_FILE_NEW, 0, NULL, NULL))
						OnFileNew();
					if (m_pMainWnd == NULL)
						bResult = FALSE;
					break;

					// If we've been asked to open a file, call OpenDocumentFile()
				case CCommandLineInfo::FileOpen:
					if (!OpenDocumentFile(rCmdInfo.m_strFileName))
						bResult = FALSE;
					break;

					// If we've been asked to register, exit without showing UI.
					// Registration was already done in InitInstance().
				case CCommandLineInfo::AppRegister:
				{
					Register();
					bResult = FALSE;    // that's all we do

					// If nobody is using it already, we can use it.
					// We'll flag that we're unregistering and not save our state
					// on the way out. This new object gets deleted by the
					// app object destructor.

					if (m_pCmdInfo == NULL)
					{
						m_pCmdInfo = new CCommandLineInfo;
						m_pCmdInfo->m_nShellCommand = CCommandLineInfo::AppUnregister;
					}
					break;
				}

				// If we've been asked to unregister, unregister and then terminate
				case CCommandLineInfo::AppUnregister:
				{
					BOOL bUnregistered = Unregister();

					bResult = FALSE;    // that's all we do

					// If nobody is using it already, we can use it.
					// We'll flag that we're unregistering and not save our state
					// on the way out. This new object gets deleted by the
					// app object destructor.

					if (m_pCmdInfo == NULL)
					{
						m_pCmdInfo = new CCommandLineInfo;
						m_pCmdInfo->m_nShellCommand = CCommandLineInfo::AppUnregister;
					}
				}
				break;
			}
			return bResult;
		}

		virtual BOOL PumpMessage()
		{
			if (!::GetMessage(&m_msgCur, NULL, NULL, NULL))
			{
				// Note: prevents calling message loop things in 'ExitInstance'
				// will never be decremented
				return FALSE;
			}

			// process this message

			if (m_msgCur.message != WM_KICKIDLE && !PreTranslateMessage(&m_msgCur))
			{
				::TranslateMessage(&m_msgCur);
				::DispatchMessage(&m_msgCur);
			}

			return TRUE;
		}

		BOOL Register()
		{
			return TRUE;
		}

#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER
		void RegisterShellFileTypes(BOOL bCompat = FALSE)
		{
			ENSURE(m_pDocManager != NULL);
			m_pDocManager->RegisterShellFileTypes(bCompat);
		}

		void UnregisterShellFileTypes()
		{
			ENSURE(m_pDocManager != NULL);
			m_pDocManager->UnregisterShellFileTypes();
		}
#endif // MTL_REMOVE_CWINAPP_DOCMANAGER

		// message loop
		virtual int Run()
		{
			BOOL bIdle = TRUE;
			LONG lIdleCount = 0;

			// acquire and dispatch messages until a WM_QUIT message is received.
			for (;;)
			{
				// phase1: check to see if we can do idle work
				while (bIdle &&
					!::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE))
				{
					// call OnIdle while in bIdle state
					if (!OnIdle(lIdleCount++))
						bIdle = FALSE; // assume "no idle" state
				}

				// phase2: pump messages while available
				do
				{
					// pump message, but quit on WM_QUIT
					if (!PumpMessage())
						return ExitInstance();

					// reset "no idle" state after pumping "normal" message
					if (IsIdleMessage(&m_msgCur))
					{
						bIdle = TRUE;
						lIdleCount = 0;
					}

				} while (::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE));
			}

			return 0 ;
		}

		BOOL SaveAllModified()
		{
#ifndef	MTL_REMOVE_CWINAPP_DOCMANAGER
			if (m_pDocManager != NULL)
				return m_pDocManager->SaveAllModified();
#endif	// MTL_REMOVE_CWINAPP_DOCMANAGER
			return TRUE;
		}

		void SaveStdProfileSettings()
		{
			ASSERT_VALID(this);

#ifndef MTL_REMOVE_CWINAPP_RECENTFILELIST
			if (m_pRecentFileList != NULL)
				m_pRecentFileList->WriteList();
#endif	// MTL_REMOVE_CWINAPP_RECENTFILELIST

#ifndef MTL_REMOVE_CWINAPP_PRINT
			if (m_nNumPreviewPages != 0)
				WriteProfileInt(_afxPreviewSection, _afxPreviewEntry, m_nNumPreviewPages);
#endif // MTL_REMOVE_CWINAPP_PRINT

		}

		void SetDialogBkColor(COLORREF clrCtlBk = 0, COLORREF clrCtlText = 0) {}

		BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
		{
#ifndef MTL_REMOVE_CWINAPP_PROFILEREGISTRY
			if (m_pszRegistryKey != NULL)
			{
				HKEY hSecKey = GetSectionKey(lpszSection);
				if (hSecKey == NULL)
					return FALSE;
				LONG lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_DWORD, (LPBYTE)&nValue, sizeof(nValue));
				RegCloseKey(hSecKey);
				return lResult == ERROR_SUCCESS;
			}
			else
#endif // MTL_REMOVE_CWINAPP_PROFILEREGISTRY
			{
				TCHAR szT[16];
				wsprintf(szT, _T("%d"), nValue);
				return ::WritePrivateProfileString(lpszSection, lpszEntry, szT, m_pszProfileName);
			}
		}

		BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
		{
			ASSERT(lpszSection != NULL);
#ifndef MTL_REMOVE_CWINAPP_PROFILEREGISTRY
			if (m_pszRegistryKey != NULL)
			{
				LONG lResult;
				if (lpszEntry == NULL) //delete whole section
				{
					HKEY hAppKey = GetAppRegistryKey();
					if (hAppKey == NULL)
						return FALSE;
					lResult = ::RegDeleteKey(hAppKey, lpszSection);
					RegCloseKey(hAppKey);
				}
				else if (lpszValue == NULL)
				{
					HKEY hSecKey = GetSectionKey(lpszSection);
					if (hSecKey == NULL)
						return FALSE;
					// necessary to cast away const below
					lResult = ::RegDeleteValue(hSecKey, (LPTSTR)lpszEntry);
					RegCloseKey(hSecKey);
				}
				else
				{
					HKEY hSecKey = GetSectionKey(lpszSection);
					if (hSecKey == NULL)
						return FALSE;
					lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_SZ,
						(LPBYTE)lpszValue, (static_cast<DWORD>(_tcslen(lpszValue)) + 1)*sizeof(TCHAR));
					RegCloseKey(hSecKey);
				}
				return lResult == ERROR_SUCCESS;
			}
			else
#endif // MTL_REMOVE_CWINAPP_PROFILEREGISTRY
			{
				ASSERT(m_pszProfileName != NULL);
				ASSERT(lstrlen(m_pszProfileName) < 4095); // can't read in bigger
				return ::WritePrivateProfileString(lpszSection, lpszEntry, lpszValue, m_pszProfileName);
			}
		}


		void SetAppID(LPCTSTR lpcszAppID)
		{
			m_pszAppID = lpcszAppID;
		}

		///////////////////////////////////////////////////////////////////////////
		// CWinApp Initialization

		void SetCurrentHandles()
		{
			// Note: there are a number of _tcsdup (aka strdup) calls that are
			// made here for the exe path, help file path, etc.  In previous
			// versions of MFC, this memory was never freed.  In this and future
			// versions this memory is automatically freed during CWinApp's
			// destructor.  If you are freeing the memory yourself, you should
			// either remove the code or set the pointers to NULL after freeing
			// the memory.

			// get path of executable
			TCHAR szBuff[_MAX_PATH];
			DWORD dwRet = ::GetModuleFileName(m_hInstance, szBuff, _MAX_PATH);
			ASSERT(dwRet != 0 && dwRet != _MAX_PATH);

			LPTSTR lpszExt = ::PathFindExtension(szBuff);
			ASSERT(lpszExt != NULL);

			ASSERT(*lpszExt == '.');
			*lpszExt = 0;       // no suffix

			TCHAR szExeName[_MAX_PATH];
			TCHAR szTitle[256];
			TCHAR szAppID[256];

			// get the exe title from the full path name [no extension]
			dwRet = AfxGetFileName(szBuff, szExeName, _MAX_PATH);
			ASSERT(dwRet == 0);

			if (m_pszExeName == NULL)
			{
				m_pszExeName = _tcsdup(szExeName); // save non-localized name
			}

			// m_pszAppName is the name used to present to the user
			if (m_pszAppName == NULL)
			{
				if (AfxLoadString(AFX_IDS_APP_TITLE, szTitle) != 0)
				{
					m_pszAppName = _tcsdup(szTitle);    // human readable title
				}
				else
				{
					m_pszAppName = _tcsdup(m_pszExeName);   // same as EXE
				}
			}

			// application user model ID
			if (m_pszAppID == NULL)
			{
				if (AfxLoadString(AFX_IDS_APP_ID, szAppID) != 0)
				{
					m_pszAppID = _tcsdup(szAppID);
				}
				else
				{
					m_pszAppID = _T("");   // empty
				}
			}

			// get path of .HLP file or .CHM (HtmlHelp) file
			if (m_pszHelpFilePath == NULL)
			{
				lstrcpy(lpszExt, (m_eHelpType == afxHTMLHelp) ? _T(".CHM") : _T(".HLP"));
				m_pszHelpFilePath = _tcsdup(szBuff);
				*lpszExt = '\0';       // back to no suffix
			}

			if (m_pszProfileName == NULL)
			{
				//lstrcat(szExeName, _T(".INI")); // will be enough room in buffer
				//m_pszProfileName = _tcsdup(szExeName);
				lstrcpy(lpszExt, _T(".INI"));
				m_pszProfileName = _tcsdup(szBuff);
				*lpszExt = '\0';       // back to no suffix
			}
		}

		void SetRegistryKey(LPCTSTR lpszRegistryKey)
		{
			free((void*)m_pszRegistryKey);
			m_pszRegistryKey = _tcsdup(lpszRegistryKey);
		}


		// Helper for message boxes; can work when no CWinApp can be found
		static int ShowAppMessageBox(CWinApp *pApp, LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt)
		{
			HWND hWndTop;
			HWND hWnd = CWnd::GetSafeOwner_(NULL, &hWndTop);

			// determine icon based on type specified
			if ((nType & MB_ICONMASK) == 0)
			{
				switch (nType & MB_TYPEMASK)
				{
				case MB_OK:
				case MB_OKCANCEL:
					nType |= MB_ICONEXCLAMATION;
					break;

				case MB_YESNO:
				case MB_YESNOCANCEL:
					nType |= MB_ICONQUESTION;
					break;

				case MB_ABORTRETRYIGNORE:
				case MB_RETRYCANCEL:
					// No default icon for these types, since they are rarely used.
					// The caller should specify the icon.
					break;
				}
			}

#ifdef _DEBUG
			if ((nType & MB_ICONMASK) == 0)
				TRACE(traceAppMsg, 0, "Warning: no icon specified for message box.\n");
#endif

			TCHAR szAppName[_MAX_PATH];
			szAppName[0] = '\0';
			LPCTSTR pszAppName;
			if (pApp != NULL)
				pszAppName = pApp->m_pszAppName;
			else
			{
				pszAppName = szAppName;
				DWORD dwLen = GetModuleFileName(NULL, szAppName, _MAX_PATH);
				if (dwLen == _MAX_PATH)
					szAppName[_MAX_PATH - 1] = '\0';
			}

			int nResult = ::MessageBox(hWnd, lpszPrompt, pszAppName, nType);

			return nResult;
		}

		BOOL Unregister()
		{
			HKEY    hKey = 0;
			TCHAR   szBuf[_MAX_PATH + 1];
			LONG    cSize = 0;
			BOOL    bRet = TRUE;

			// Remove profile information -- the registry entries exist if
			// SetRegistryKey() was used.

			if (m_pszRegistryKey)
			{
				ENSURE(m_pszProfileName != NULL);

				CString strKey = _T("Software\\");
				strKey += m_pszRegistryKey;
				CString strSubKey = strKey + _T("\\") + m_pszProfileName;

				DelRegTree(HKEY_CURRENT_USER, strSubKey);

				// If registry key is empty then remove it

				DWORD   dwResult;
				if ((dwResult = ::RegOpenKeyEx(HKEY_CURRENT_USER, strKey, 0, KEY_ENUMERATE_SUB_KEYS, &hKey)) ==
					ERROR_SUCCESS)
				{
					if (::RegEnumKey(hKey, 0, szBuf, _MAX_PATH) == ERROR_NO_MORE_ITEMS)
						DelRegTree(HKEY_CURRENT_USER, strKey);
					::RegCloseKey(hKey);
				}
				if (RegQueryValue(HKEY_CURRENT_USER, strSubKey, szBuf, &cSize) == ERROR_SUCCESS)
					bRet = TRUE;
			}
			return bRet;
		}


		void UpdatePrinterSelection(BOOL bForceDefaults)
		{
#ifndef MTL_REMOVE_CWINAPP_PRINT
			if (!bForceDefaults && m_hDevNames != NULL)
			{
				LPDEVNAMES lpDevNames = (LPDEVNAMES)::GlobalLock(m_hDevNames);
				ENSURE(lpDevNames != NULL);
				if (lpDevNames->wDefault & DN_DEFAULTPRN)
				{
					CPrintDialog pd(TRUE);
					if (!pd.GetDefaults())
						return;

					if (pd.m_pd.hDevNames == NULL)
					{
						// Printer was default, but now there are no printers at all!
						if (m_hDevMode != NULL)
							AfxGlobalFree(m_hDevMode);
						AfxGlobalFree(m_hDevNames);
						m_hDevMode = NULL;
						m_hDevNames = NULL;
					}
					else if (
						lstrcmp((LPCTSTR)lpDevNames + lpDevNames->wDriverOffset,
						pd.GetDriverName()) != 0 ||
						lstrcmp((LPCTSTR)lpDevNames + lpDevNames->wDeviceOffset,
						pd.GetDeviceName()) != 0 ||
						lstrcmp((LPCTSTR)lpDevNames + lpDevNames->wOutputOffset,
						pd.GetPortName()) != 0)
					{
						// Printer was default, and default has changed...assume default
						if (m_hDevMode != NULL)
							AfxGlobalFree(m_hDevMode);
						AfxGlobalFree(m_hDevNames);
						m_hDevMode = pd.m_pd.hDevMode;
						m_hDevNames = pd.m_pd.hDevNames;
					}
					else
					{
						// Printer was default, and still is...keep the same
						if (pd.m_pd.hDevMode != NULL)
							AfxGlobalFree(pd.m_pd.hDevMode);
						if (pd.m_pd.hDevNames != NULL)
							AfxGlobalFree(pd.m_pd.hDevNames);
					}
				}
			}
			else
			{
				// First time or Forced -- Get defaults
				CPrintDialog pd(TRUE);
				pd.GetDefaults();

				if (m_hDevMode != NULL)
					AfxGlobalFree(m_hDevMode);
				if (m_hDevNames != NULL)
					AfxGlobalFree(m_hDevNames);

				m_hDevMode = pd.m_pd.hDevMode;
				m_hDevNames = pd.m_pd.hDevNames;
			}
#endif // MTL_REMOVE_CWINAPP_PRINT
		}


};

BEGIN_MESSAGE_MAP(CWinApp, CCmdTarget)
	// Global File commands
	ON_COMMAND(ID_APP_EXIT, &CWinApp::OnAppExit)
#ifndef MTL_REMOVE_CWINAPP_RECENTFILELIST
	// MRU - most recently used file menu
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, &CWinApp::OnUpdateRecentFileMenu)
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, &CWinApp::OnOpenRecentFile)
#endif // MTL_REMOVE_CWINAPP_RECENTFILELIST
END_MESSAGE_MAP()


_INLINE CWinApp* AfxGetApp() { return CWinApp::m_pCWinApp; }
_INLINE LPCTSTR AfxGetAppName() { return AfxGetApp()->m_pszAppName ; } 
_INLINE MSG* AfxGetCurrentMessage() { return &AfxGetApp()->m_msgCur; }
_INLINE HINSTANCE AfxGetInstanceHandle() { return AfxGetApp()->m_hInstance; }
_INLINE CWnd* AfxGetMainWnd() { CWnd* pCWnd = AfxGetApp()->m_pMainWnd; if (pCWnd != NULL) return pCWnd; return CWnd::GetActiveWindow() ; }
_INLINE AFX_MODULE_STATE* AfxGetModuleState() { return (AFX_MODULE_STATE*)CWinApp::GetCModuleState(); }
_INLINE AFX_MODULE_THREAD_STATE* AfxGetModuleThreadState() { return (AFX_MODULE_THREAD_STATE*)AfxGetApp(); }
_INLINE HINSTANCE AfxGetResourceHandle() { return AfxGetApp()->m_hInstance; }
_INLINE _AFX_THREAD_STATE* AfxGetThreadState() { return (_AFX_THREAD_STATE*)AfxGetApp(); }
_INLINE CWinThread* AfxGetThread() { return AfxGetApp() ; }
_INLINE BOOL AfxPumpMessage() { return AfxGetApp()->PumpMessage(); }


_INLINE void AfxClassInit(CRuntimeClass* pNewClass) { AfxGetModuleState()->m_classList.AddHead(pNewClass); }

#define AfxFindResourceHandle(lpszResource, lpszType) AfxGetResourceHandle()
#define _AFX_CMDTARGET_GETSTATE() (AfxGetModuleState())


CLASS_HANDLE_CREATE(CWMHnd, HWND, CWMHnd, Wnd)
CLASS_HANDLE_CREATE(CWnd, HWND, CWMHnd, Wnd)
CLASS_HANDLE_CREATE(CDialog, HWND, CWMHnd, Wnd)

CLASS_HANDLE_CREATE(CMenu, HMENU, CMenu, Menu)

CLASS_HANDLE_CREATE(CDC, HDC, CDC, DC)

CLASS_HANDLE_CREATE(CGdiObject, HGDIOBJ, CGdiObject, Gdiobj)
CLASS_HANDLE_CREATE(CBitmap, HBITMAP, CGdiObject, Gdiobj)
CLASS_HANDLE_CREATE(CBrush, HBRUSH, CGdiObject, Gdiobj)
CLASS_HANDLE_CREATE(CFont, HFONT, CGdiObject, Gdiobj)
CLASS_HANDLE_CREATE(CPalette, HPALETTE, CGdiObject, Gdiobj)
CLASS_HANDLE_CREATE(CPen, HPEN, CGdiObject, Gdiobj)
CLASS_HANDLE_CREATE(CRgn, HRGN, CGdiObject, Gdiobj)

CLASS_HANDLE_CREATE(CImageListMTL, HIMAGELIST, void, Hnd)


_INLINE BOOL AfxHelpEnabled()
{
	if (AfxGetApp() == NULL)
		return FALSE;

	// help is enabled if the app has a handler for ID_HELP
	AFX_CMDHANDLERINFO info;

	// check main window first
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd != NULL && pWnd->OnCmdMsg(ID_HELP, CN_COMMAND, NULL, &info))
		return TRUE;

	// check app last
	return AfxGetApp()->OnCmdMsg(ID_HELP, CN_COMMAND, NULL, &info);
}

_INLINE int AfxMessageBox(LPCTSTR lpszText, UINT nType, UINT nIDHelp)
{
	CWinApp* pApp = AfxGetApp();
	return CWinApp::ShowAppMessageBox(pApp, lpszText, nType, nIDHelp);
}

_INLINE int AfxMessageBox(UINT nIDPrompt, UINT nType, UINT nIDHelp)
{
	CString string;
	if (!string.LoadString(nIDPrompt))
	{
		TRACE(traceAppMsg, 0, "Error: failed to load message box prompt string 0x%04x.\n", nIDPrompt);
		ASSERT(FALSE);
	}
	if (nIDHelp == (UINT)-1)
		nIDHelp = nIDPrompt;
	return AfxMessageBox(string, nType, nIDHelp);
}

_INLINE void AfxOleOnReleaseAllObjects()
{
	// don't shut down the application if user is in control of the app
	//if (AfxOleGetUserCtrl())
	//	return;

	//AfxOleSetUserCtrl(TRUE);    // avoid re-entry

	// shut down the application
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL && pApp->m_pMainWnd != NULL)
	{
		// destroy the main window (only if enabled)
		if (pApp->m_pMainWnd->IsWindowEnabled())
		{
			// the main window will post WM_QUIT as part of its shutdown
			pApp->m_pMainWnd->DestroyWindow();
		}
	}
	else //if (!afxContextIsDLL)
	{
		// no main window, so just post WM_QUIT.
		AfxPostQuitMessage(0);
	}
}

_INLINE void AfxOleLockApp()
{
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	InterlockedIncrement(&pModuleState->m_nObjectCount);
}

_INLINE void AfxOleUnlockApp()
{
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	ASSERT(pModuleState->m_nObjectCount != 0);
	if (InterlockedDecrement(&pModuleState->m_nObjectCount) == 0)
	{
		// allow application to shut down when all the objects have
		//  been released
		::AfxOleOnReleaseAllObjects();
	}
}


/*============================================================================*/
// class CWaitCursor

/////////////////////////////////////////////////////////////////////////////
// CCmdTarget routines that delegate to the WinApp

_INLINE void CCmdTarget::BeginWaitCursor()
{
	AfxGetApp()->DoWaitCursor(1);
}
_INLINE void CCmdTarget::EndWaitCursor()
{
	AfxGetApp()->DoWaitCursor(-1);
}
_INLINE void CCmdTarget::RestoreWaitCursor()
{
	AfxGetApp()->DoWaitCursor(0);
}


class CWaitCursor
{
	// Construction/Destruction
public:
	CWaitCursor() { AfxGetApp()->BeginWaitCursor(); }
	~CWaitCursor() { AfxGetApp()->EndWaitCursor(); }

	// Operations
public:
	void Restore() { AfxGetApp()->RestoreWaitCursor(); }
};




