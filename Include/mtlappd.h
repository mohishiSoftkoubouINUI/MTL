



_INLINE UINT_PTR CALLBACK CCommonDialog::_AfxCommDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static UINT _afxMsgLBSELCHANGE = 0;
	static UINT _afxMsgSHAREVI = 0;
	static UINT _afxMsgFILEOK = 0;
	//static UINT _afxMsgCOLOROK = 0;
	static UINT _afxMsgHELP = 0;
	static UINT _afxMsgSETRGB = 0;

	if (hWnd == NULL)
		return 0;

	CREATEWINDOWPARAM& cwp = GetHookOldCbtFilter();
	if ((cwp.pCWnd != NULL) && (((CCommonDialog*)cwp.pCWnd)->m_hWnd == NULL))
	{
		CCommonDialog* pThis = (CCommonDialog*)cwp.pCWnd;
		pThis->FromHandle(hWnd, pThis, NULL);
		((CWMHnd*)pThis)->SubclassWindow(hWnd);
		cwp.pCWnd = NULL;
	}

	if (message == WM_INITDIALOG)
	{
		_afxMsgLBSELCHANGE = ::RegisterWindowMessage(LBSELCHSTRING);
		_afxMsgSHAREVI = ::RegisterWindowMessage(SHAREVISTRING);
		_afxMsgFILEOK = ::RegisterWindowMessage(FILEOKSTRING);
		//_afxMsgCOLOROK = ::RegisterWindowMessage(COLOROKSTRING);
		_afxMsgHELP = ::RegisterWindowMessage(HELPMSGSTRING);
		_afxMsgSETRGB = ::RegisterWindowMessage(SETRGBSTRING);
		return (UINT)AfxDlgProc(hWnd, message, wParam, lParam);
	}

	if (message == _afxMsgHELP ||
		(message == WM_COMMAND && LOWORD(wParam) == pshHelp))
	{
		// just translate the message into the AFX standard help command.
		::SendMessage(hWnd, WM_COMMAND, ID_HELP, 0);
		return 1;
	}

	if (message < 0xC000)
	{
		// not a ::RegisterWindowMessage message
		return 0;
	}

	// assume it is already wired up to a permanent one
	CDialog* pDlg = (CDialog*)CWnd::FromHandle(hWnd);
	if (!pDlg)
	{
		// someone else sent us a registered message. Ignore it
		return 0;
	}
	ASSERT_KINDOF(CDialog, pDlg);

	if (pDlg->IsKindOf(RUNTIME_CLASS(CFileDialogMTL)))
	{
		// If we're exploring then we are not interested in the Registered messages
		if (((CFileDialog*)pDlg)->m_ofn.Flags & OFN_EXPLORER)
			return 0;
	}

	// RegisterWindowMessage - does not copy to lastState buffer, so
	// CWnd::GetCurrentMessage and CWnd::Default will NOT work
	// while in these handlers

	// Dispatch special commdlg messages through our virtual callbacks
	if (message == _afxMsgSHAREVI)
	{
		ASSERT_KINDOF(CFileDialog, pDlg);
		return ((CFileDialog*)pDlg)->OnShareViolation((LPCTSTR)lParam);
	}
	else if (message == _afxMsgFILEOK)
	{
		ASSERT_KINDOF(CFileDialog, pDlg);

		((CFileDialog*)pDlg)->m_pofnTemp = (OPENFILENAME*)lParam;
		BOOL bResult = ((CFileDialog*)pDlg)->OnFileNameOK();
		((CFileDialog*)pDlg)->m_pofnTemp = NULL;

		return bResult;
	}
	else if (message == _afxMsgLBSELCHANGE)
	{
		ASSERT_KINDOF(CFileDialog, pDlg);
		((CFileDialog*)pDlg)->OnLBSelChangedNotify((UINT)wParam, LOWORD(lParam),
			HIWORD(lParam));
		return 0;
	}
#if 0
	else if (message == _afxMsgCOLOROK)
	{
		ASSERT_KINDOF(CColorDialog, pDlg);
		return ((CColorDialog*)pDlg)->OnColorOK();
	}
#endif
	else if (message == _afxMsgSETRGB)
	{
		// nothing to do here, since this is a SendMessage
		return 0;
	}
	return 0;
}





_INLINE CControlBar::~CControlBar()
{
	ASSERT_VALID(this);

	DestroyWindow();    // avoid PostNcDestroy problems

	// also done in OnDestroy, but done here just in case
	if (m_pDockSite != NULL)
		m_pDockSite->RemoveControlBar(this);

	// free docking context
	CDockContext* pDockContext = m_pDockContext;
	m_pDockContext = NULL;
	delete pDockContext;

	// free array
	if (m_pData != NULL)
	{
		ASSERT(m_nCount != 0);
		free(m_pData);
	}

	AFX_MODULE_THREAD_STATE* pModuleThreadState = AfxGetModuleThreadState();
	if (pModuleThreadState->m_pLastStatus == this)
	{
		pModuleThreadState->m_pLastStatus = NULL;
		pModuleThreadState->m_nLastStatus = (INT_PTR)(-1);
	}
}

_INLINE void CControlBar::OnDestroy()
{
	if (AfxGetModuleThreadState()->m_pLastStatus == this)
	{
		SetStatusText((INT_PTR)(-1));
	}

	if (m_pDockSite != NULL)
	{
		m_pDockSite->RemoveControlBar(this);
		m_pDockSite = NULL;
	}

	CWnd::OnDestroy();
}

_INLINE void CControlBar::OnTimer(UINT_PTR nIDEvent)
{
	if (GetKeyState(VK_LBUTTON) < 0)
		return;

	AFX_MODULE_THREAD_STATE* pModuleThreadState = AfxGetModuleThreadState();

	// get current mouse position for hit test
	CPoint point; GetCursorPos(&point);
	ScreenToClient(&point);
	INT_PTR nHit = OnToolHitTest(point, NULL);
	if (nHit >= 0)
	{
		CWnd *pParent = GetTopLevelParent();
		// determine if status bar help should go away
		if (!IsTopParentActive())
		{
			nHit = -1;
		}
		else
		{
			ENSURE(pParent);
			if (!pParent->IsWindowEnabled())
			{
				nHit = -1;
			}
		}

		// remove status help if capture is set
		HWND hWndTip = pModuleThreadState->m_pToolTip->GetSafeHwnd();
		CWnd* pCapture = GetCapture();
		if (pCapture != this && pCapture->GetSafeHwnd() != hWndTip &&
			pCapture->GetTopLevelParent() == pParent)
		{
			nHit = -1;
		}
	}
	else
	{
		pModuleThreadState->m_nLastStatus = (INT_PTR)(-1);
	}

	// make sure it isn't over some other app's window
	if (nHit >= 0)
	{
		ClientToScreen(&point);
		HWND hWnd = ::WindowFromPoint(point);
		if (hWnd == NULL || (hWnd != m_hWnd && !::IsChild(m_hWnd, hWnd) &&
			pModuleThreadState->m_pToolTip->GetSafeHwnd() != hWnd))
		{
			nHit = -1;
			pModuleThreadState->m_nLastStatus = (INT_PTR)(-1);
		}
	}

	// handle the result
	if (nHit < 0)
	{
		if (pModuleThreadState->m_nLastStatus == (INT_PTR)(-1))
			KillTimer(AFX_TIMER_ID_TIMER_CHECK);
		SetStatusText((INT_PTR)(-1));
	}

	// set status text after initial timeout
	if (nIDEvent == AFX_TIMER_ID_TIMER_WAIT)
	{
		KillTimer(AFX_TIMER_ID_TIMER_WAIT);
		if (nHit >= 0)
			SetStatusText(nHit);
	}
}

_INLINE BOOL CControlBar::PreTranslateMessage(MSG* pMsg)
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);

	// allow tooltip messages to be filtered
	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;

	UINT message = pMsg->message;
	CWnd* pOwner = GetOwner();

	// handle CBRS_FLYBY style (status bar flyby help)
	if (((m_dwStyle & CBRS_FLYBY) ||
		message == WM_LBUTTONDOWN || message == WM_LBUTTONUP) &&
		((message >= WM_MOUSEFIRST && message <= AFX_WM_MOUSELAST) ||
		(message >= WM_NCMOUSEFIRST && message <= WM_NCMOUSELAST)))
	{
		AFX_MODULE_THREAD_STATE* pModuleThreadState = AfxGetModuleThreadState();

		// gather information about current mouse position
		CPoint point = pMsg->pt;
		ScreenToClient(&point);
		TOOLINFO ti; memset(&ti, 0, sizeof(AFX_OLDTOOLINFO));
		ti.cbSize = sizeof(TOOLINFO);
		INT_PTR nHit = OnToolHitTest(point, &ti);
		if (ti.lpszText != LPSTR_TEXTCALLBACK)
			free(ti.lpszText);
		BOOL bNotButton = message == WM_LBUTTONDOWN && (ti.uFlags & TTF_NOTBUTTON);
		if (message != WM_LBUTTONDOWN && GetKeyState(VK_LBUTTON) < 0)
			nHit = pModuleThreadState->m_nLastStatus;

		// update state of status bar
		if (nHit < 0 || bNotButton)
		{
			if (GetKeyState(VK_LBUTTON) >= 0 || bNotButton)
			{
				SetStatusText((INT_PTR)(-1));
				KillTimer(AFX_TIMER_ID_TIMER_CHECK);
			}
		}
		else
		{
			if (message == WM_LBUTTONUP)
			{
				SetStatusText((INT_PTR)(-1));
				ResetTimer(AFX_TIMER_ID_TIMER_CHECK, 200);
			}
			else
			{
				if ((m_nStateFlags & statusSet) || GetKeyState(VK_LBUTTON) < 0)
					SetStatusText(nHit);
				else if (nHit != pModuleThreadState->m_nLastStatus)
					ResetTimer(AFX_TIMER_ID_TIMER_WAIT, 300);
			}
		}
		pModuleThreadState->m_nLastStatus = nHit;
	}

	// since 'IsDialogMessage' will eat frame window accelerators,
	//   we call all frame windows' PreTranslateMessage first
	while (pOwner != NULL)
	{
		// allow owner & frames to translate before IsDialogMessage does
		if (pOwner->PreTranslateMessage(pMsg))
			return TRUE;

		// try parent frames until there are no parent frames
		pOwner = pOwner->GetParentFrame();
	}

	// filter both messages to dialog and from children
	if (::IsWindow(m_hWnd))
		return PreTranslateInput(pMsg);
	return FALSE;
}

_INLINE BOOL CControlBar::SetStatusText(INT_PTR nHit)
{
	CWnd* pOwner = (CWnd*)GetOwner();

	if (nHit == -1)
	{
		// handle reset case
		AfxGetApp()->m_pLastStatus = NULL;
		if (m_nStateFlags & statusSet)
		{
			pOwner->SendMessage(WM_POPMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
			m_nStateFlags &= ~statusSet;
			return TRUE;
		}
		KillTimer(AFX_TIMER_ID_TIMER_WAIT);
	}
	else
	{
		// handle setnew case
		if (!(m_nStateFlags & statusSet) || AfxGetApp()->m_nLastStatus != nHit)
		{
			AfxGetApp()->m_pLastStatus = this;
			pOwner->SendMessage(WM_SETMESSAGESTRING, nHit);
			m_nStateFlags |= statusSet;
			ResetTimer(AFX_TIMER_ID_TIMER_CHECK, 200);
			return TRUE;
		}
	}
	return FALSE;
}



_INLINE BOOL CDialog::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (CWMHnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	if ((nCode != CN_COMMAND && nCode != CN_UPDATE_COMMAND_UI) || !IS_COMMAND_ID(nID) || nID >= 0xf000)
	{
		// control notification or non-command button or system command
		return FALSE;       // not routed any further
	}

	// if we have an owner window, give it second crack
	CWMHnd* pOwner = GetParent();
	if (pOwner != NULL)
	{
		TRACE(traceCmdRouting, 1, "Routing command id 0x%04X to owner window.\n", nID);

		ASSERT(pOwner != this);
		if (pOwner->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL)
	{
		TRACE(traceCmdRouting, 1, "Routing command id 0x%04X to app.\n", nID);

		if (pApp->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	TRACE(traceCmdRouting, 1, "IGNORING command id 0x%04X sent to %hs dialog.\n", nID,
		GetRuntimeClass()->m_lpszClassName);

	return FALSE;
}


_INLINE BOOL CDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
	ENSURE(lpszPathName);

	CFileException* pfe = new CFileException;
	CFile* pFile = GetFile(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, pfe);
	if (pFile == NULL)
	{
		DELETE_EXCEPTION(pfe);
		return FALSE;
	}

	DELETE_EXCEPTION(pfe);

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	TRY
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			Serialize(loadArchive);     // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		DeleteContents();   // remove failed contents

		DELETE_EXCEPTION(e);
		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // start off with unmodified

	return TRUE;
}


_INLINE BOOL CDocument::OnSaveDocument(LPCTSTR lpszPathName)
{
	ENSURE(lpszPathName);

	CFileException *pfe = new CFileException;
	CFile* pFile = GetFile(lpszPathName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, pfe);

	if (pFile == NULL)
	{
		return FALSE;
	}

	CArchive saveArchive(pFile, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = this;

	CWaitCursor wait;
	Serialize(saveArchive);     // save me
	saveArchive.Close();
	ReleaseFile(pFile, FALSE);

	SetModifiedFlag(FALSE);     // back to unmodified

	return TRUE;        // success
}

_INLINE BOOL CDocument::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
// Save the document data to a file
// lpszPathName = path name where to save document file
// if lpszPathName is NULL then the user will be prompted (SaveAs)
// note: lpszPathName can be different than 'm_strPathName'
// if 'bReplace' is TRUE will change file name if successful (SaveAs)
// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CString newName = lpszPathName;

	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(":/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);
			if (newName.GetLength() > _MAX_FNAME)
				newName.ReleaseBuffer(_MAX_FNAME);

			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) && !strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');
				int iStart = 0;
				newName += Tokenize(strExt, _T(";"), iStart);
			}
		}

		if (!AfxGetApp()->DoPromptFileName(newName, bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
											OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			CFile::Remove(newName);
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
	{
		SetPathName(newName);
		OnDocumentEvent(onAfterSaveDocument);
	}

	return TRUE;        // success
}

_INLINE BOOL CDocument::SaveModified() // return TRUE if ok to continue
{
	if (!IsModified())
		return TRUE;        // ok to continue

	// get name/title of document
	CString name;
	if (m_strPathName.IsEmpty())
	{
		// get name based on caption
		name = m_strTitle;
	}
	else
	{
		// get name based on file title of path name
		name = m_strPathName;
		AfxGetFileTitle(m_strPathName, name.GetBuffer(_MAX_PATH), _MAX_PATH);
		name.ReleaseBuffer();
	}

	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
		case IDCANCEL:
			return FALSE;       // don't continue

		case IDYES:
			// If so, either Save or Update, as appropriate
			if (!DoFileSave())
				return FALSE;       // don't continue
			break;

		case IDNO:
			// If not saving changes, revert the document
			break;

		default:
			ASSERT(FALSE);
			break;
	}
	return TRUE;    // keep going
}


_INLINE void CDocument::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
	// store the path fully qualified
	TCHAR szFullPath[_MAX_PATH];
	ENSURE(lpszPathName);

	AfxFullPath(szFullPath, lpszPathName);

	m_strPathName = szFullPath;
	ASSERT(!m_strPathName.IsEmpty());       // must be set to something
	ASSERT_VALID(this);

	// set the document title based on path name
	TCHAR szTitle[_MAX_FNAME];
	if (AfxGetFileTitle(szFullPath, szTitle, _MAX_FNAME) == 0)
		SetTitle(szTitle);

#ifndef MTL_REMOVE_CWINAPP_RECENTFILELIST
	// add it to the file MRU list
	if (bAddToMRU)
		AfxGetApp()->AddToRecentFileList(m_strPathName);
#endif	// MTL_REMOVE_CWINAPP_RECENTFILELIST

	ASSERT_VALID(this);
}


_INLINE void CDocManager::OnFileOpen()
{
	CString newName;
	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
		return; // open cancelled

	AfxGetApp()->OpenDocumentFile(newName);
	// if returns NULL, the user has already been alerted
}

_INLINE BOOL AfxResolveShortcut(CWMHnd* pWnd, LPCTSTR lpszFileIn, LPTSTR lpszFileOut, int cchPath)
{
	AFX_COM com;
	IShellLink* psl = NULL;
	*lpszFileOut = 0;   // assume failure

	if (!pWnd)
		return FALSE;

	SHFILEINFO info;
	if ((SHGetFileInfo(lpszFileIn, 0, &info, sizeof(info), SHGFI_ATTRIBUTES) == 0) ||
		!(info.dwAttributes & SFGAO_LINK))
	{
		return FALSE;
	}

	if (FAILED(com.CreateInstance(CLSID_ShellLink, NULL, IID_IShellLink, (LPVOID*)&psl)) ||
		psl == NULL)
	{
		return FALSE;
	}

	IPersistFile *ppf = NULL;
	if (SUCCEEDED(psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf)))
	{
		CString strFileIn(lpszFileIn);
		if (ppf != NULL && SUCCEEDED(ppf->Load((LPCOLESTR)(LPCTSTR)strFileIn, STGM_READ)))
		{
			/* Resolve the link, this may post UI to find the link */
			if (SUCCEEDED(psl->Resolve(pWnd->GetSafeHwnd(), SLR_ANY_MATCH)))
			{
				psl->GetPath(lpszFileOut, cchPath, NULL, 0);
				ppf->Release();
				psl->Release();
				return TRUE;
			}
		}
		if (ppf != NULL)
			ppf->Release();
	}
	psl->Release();
	return FALSE;
}


_INLINE CDocument* CDocManager::OpenDocumentFile(LPCTSTR lpszFileName, BOOL bAddToMRU)
{
	if (lpszFileName == NULL)
		return NULL;

	// find the highest confidence
	POSITION pos = m_templateList.GetHeadPosition();
	CDocTemplate::Confidence bestMatch = CDocTemplate::noAttempt;
	CDocTemplate* pBestTemplate = NULL;
	CDocument* pOpenDocument = NULL;

	TCHAR szPath[_MAX_PATH];
	ASSERT(lstrlen(lpszFileName) < _countof(szPath));
	TCHAR szTemp[_MAX_PATH];
	if (lpszFileName[0] == '\"')
		++lpszFileName;
	lstrcpyn(szTemp, lpszFileName, _countof(szTemp));
	LPTSTR lpszLast = _tcsrchr(szTemp, '\"');
	if (lpszLast != NULL)
		*lpszLast = 0;

	if (AfxFullPath(szPath, szTemp) == FALSE)
	{
		ASSERT(FALSE);
		return NULL; // We won't open the file. MFC requires paths with
		// length < _MAX_PATH
	}

	TCHAR szLinkName[_MAX_PATH];
	if (AfxResolveShortcut(AfxGetMainWnd(), szPath, szLinkName, _MAX_PATH))
		lstrcpyn(szPath, szLinkName, _countof(szPath));

	while (pos != NULL)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);

		CDocTemplate::Confidence match;
		ASSERT(pOpenDocument == NULL);
		match = pTemplate->MatchDocType(szPath, pOpenDocument);
		if (match > bestMatch)
		{
			bestMatch = match;
			pBestTemplate = pTemplate;
		}
		if (match == CDocTemplate::yesAlreadyOpen)
			break;      // stop here
	}

	if (pOpenDocument != NULL)
	{
		POSITION posOpenDoc = pOpenDocument->GetFirstViewPosition();
		if (posOpenDoc != NULL)
		{
			CView* pView = pOpenDocument->GetNextView(posOpenDoc); // get first one

			CFrameWnd* pFrame = pView->GetParentFrame();

			if (pFrame == NULL)
				TRACE(traceAppMsg, 0, "Error: Can not find a frame for document to activate.\n");
			else
			{
				pFrame->ActivateFrame();

				if (pFrame->GetParent() != NULL)
				{
					CFrameWnd* pAppFrame;
					if (pFrame != (pAppFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd))
					{
						pAppFrame->ActivateFrame();
					}
				}
			}
		}
		else
			TRACE(traceAppMsg, 0, "Error: Can not find a view for document to activate.\n");

		return pOpenDocument;
	}

	if (pBestTemplate == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC);
		return NULL;
	}

	return pBestTemplate->OpenDocumentFile(szPath, bAddToMRU, TRUE);
}






_INLINE void CFrameWnd::OnClose()
{
	if (m_lpfnCloseProc != NULL)
	{
		// if there is a close proc, then defer to it, and return
		// after calling it so the frame itself does not close.
		(*m_lpfnCloseProc)(this);
		return;
	}

	// Note: only queries the active document
	CDocument* pDocument = GetActiveDocument();
	if (pDocument != NULL && !pDocument->CanCloseFrame(this))
	{
		// document can't close right now -- don't close it
		return;
	}
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL && pApp->m_pMainWnd == this)
	{
		// attempt to save all documents
		if (pDocument == NULL && !pApp->SaveAllModified())
			return;     // don't close it

		// hide the application's windows before closing all the documents
		pApp->HideApplication();

		// close all documents first
		pApp->CloseAllDocuments(FALSE);

		// there are cases where destroying the documents may destroy the
		//  main window of the application.
		if (pApp->m_pMainWnd == NULL)
		{
			AfxPostQuitMessage(0);
			return;
		}
	}

	// detect the case that this is the last frame on the document and
	// shut down with OnCloseDocument instead.
	if (pDocument != NULL && pDocument->m_bAutoDelete)
	{
		BOOL bOtherFrame = FALSE;
		POSITION pos = pDocument->GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = pDocument->GetNextView(pos);
			ENSURE_VALID(pView);
			if (pView->GetParentFrame() != this)
			{
				bOtherFrame = TRUE;
				break;
			}
		}
		if (!bOtherFrame)
		{
			pDocument->OnCloseDocument();
			return;
		}

		// allow the document to cleanup before the window is destroyed
		pDocument->PreCloseFrame(this);
	}

	// then destroy the window
	DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd command/message routing

_INLINE BOOL CFrameWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
#ifndef MTL_REMOVE_CFRAMEWND_VIEW
	// pump through current view FIRST
	CView* pView = GetActiveView();
	if (pView != NULL && pView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
#endif	// MTL_REMOVE_CFRAMEWND_VIEW

	// then pump through frame
	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// last but not least, pump through app
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL && pApp->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return FALSE;
}


_INLINE void CFrameWnd::OnDestroy()
{
	DestroyDockBars();

	// reset menu to default before final shutdown
	if (m_hMenuDefault != NULL && ::GetMenu(m_hWnd) != m_hMenuDefault)
	{
		::SetMenu(m_hWnd, m_hMenuDefault);
		ASSERT(::GetMenu(m_hWnd) == m_hMenuDefault);
	}

	// Automatically quit when the main window is destroyed.
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL && pApp->m_pMainWnd == this && pApp->m_eHelpType == afxWinHelp)
	{
		// closing the main application window
		::WinHelp(m_hWnd, NULL, HELP_QUIT, 0L);

		// will call PostQuitMessage in CWnd::OnNcDestroy
	}
	CWnd::OnDestroy();
}

_INLINE void CFrameWnd::OnDropFiles(HDROP hDropInfo)
{
	SetActiveWindow();      // activate us first !
	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	for (UINT iFile = 0; iFile < nFiles; iFile++)
	{
		TCHAR szFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);
		pApp->OpenDocumentFile(szFileName);
	}
	::DragFinish(hDropInfo);
}


_INLINE void CFrameWnd::AddFrameWnd()
{
	// hook it into the CFrameWnd list
	AFX_MODULE_THREAD_STATE* pState = _AFX_CMDTARGET_GETSTATE()->m_thread;
	pState->m_frameList.AddHead(this);
}

_INLINE void CFrameWnd::InitialUpdateFrame(CDocument* pDoc, BOOL bMakeVisible)
{
	// if the frame does not have an active view, set to first pane
#ifndef MTL_REMOVE_CFRAMEWND_VIEW
	CView* pView = NULL;
	if (GetActiveView() == NULL)
	{
		CWnd* pWnd = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
		if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		{
			pView = (CView*)pWnd;
			SetActiveView(pView, FALSE);
		}
	}
#endif	// MTL_REMOVE_CFRAMEWND_VIEW

	if (bMakeVisible)
	{
		// send initial update to all views (and other controls) in the frame
		SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

#ifndef MTL_REMOVE_CFRAMEWND_VIEW
		// give view a chance to save the focus (CFormView needs this)
		if (pView != NULL)
			pView->OnActivateFrame(WA_INACTIVE, this);
#endif	// MTL_REMOVE_CFRAMEWND_VIEW

		// finally, activate the frame
		// (send the default show command unless the main desktop window)
		int nCmdShow = -1;      // default
		CWinApp* pApp = AfxGetApp();
		if (pApp != NULL && pApp->m_pMainWnd == this)
		{
			nCmdShow = pApp->m_nCmdShow; // use the parameter from WinMain
			pApp->m_nCmdShow = -1; // set to default after first time
		}
		ActivateFrame(nCmdShow);

#ifndef MTL_REMOVE_CFRAMEWND_VIEW
		if (pView != NULL)
			pView->OnActivateView(TRUE, pView, pView);
#endif	// MTL_REMOVE_CFRAMEWND_VIEW
	}

	// update frame counts and frame title (may already have been visible)
	if (pDoc != NULL)
		pDoc->UpdateFrameCounts();
	OnUpdateFrameTitle(TRUE);
}

_INLINE void CFrameWnd::RemoveFrameWnd()
{
	// remove this frame window from the list of frame windows
	AFX_MODULE_THREAD_STATE* pState = _AFX_CMDTARGET_GETSTATE()->m_thread;
	pState->m_frameList.Remove(this);
}


_INLINE BOOL CMDIChildWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
									DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext)
{
	if (pParentWnd == NULL)
	{
		CWinApp* pCWinApp = AfxGetApp();
		CWnd* pMainWnd = (CWnd*)pCWinApp->m_pMainWnd;
		ENSURE_VALID(pMainWnd);
		ASSERT_KINDOF(CMDIFrameWnd, pMainWnd);
		pParentWnd = (CMDIFrameWnd*)pMainWnd;
	}
	ASSERT(::IsWindow(pParentWnd->m_hWndMDIClient));

	// insure correct window positioning
	pParentWnd->RecalcLayout();

	// first copy into a CREATESTRUCT for PreCreate
	CREATESTRUCT cs;
	cs.dwExStyle = 0L;
	cs.lpszClass = lpszClassName;
	cs.lpszName = lpszWindowName;
	cs.style = dwStyle;
	cs.x = rect.left;
	cs.y = rect.top;
	cs.cx = rect.right - rect.left;
	cs.cy = rect.bottom - rect.top;
	cs.hwndParent = pParentWnd->m_hWnd;
	cs.hMenu = NULL;
	cs.hInstance = AfxGetInstanceHandle();
	cs.lpCreateParams = (LPVOID)pContext;

	if (!PreCreateWindow(cs))
	{
		PostNcDestroy();
		return FALSE;
	}
	// extended style must be zero for MDI Children (except under Win4)
	ASSERT(cs.hwndParent == pParentWnd->m_hWnd);    // must not change

	// now copy into a MDICREATESTRUCT for real create
	MDICREATESTRUCT mcs;
	mcs.szClass = cs.lpszClass;
	mcs.szTitle = cs.lpszName;
	mcs.hOwner = cs.hInstance;
	mcs.x = cs.x;
	mcs.y = cs.y;
	mcs.cx = cs.cx;
	mcs.cy = cs.cy;
	mcs.style = cs.style & ~(WS_MAXIMIZE | WS_VISIBLE);
	mcs.lParam = (LPARAM)cs.lpCreateParams;

	// create the window through the MDICLIENT window

	HookWindowCreate(this);

	HWND hWnd = (HWND)::SendMessage(pParentWnd->m_hWndMDIClient, WM_MDICREATE, 0, (LPARAM)&mcs);

	UnhookWindowCreate();

	if (hWnd == NULL)
		PostNcDestroy();        // cleanup if MDICREATE fails too soon

	if (hWnd == NULL)
		return FALSE;

	// special handling of visibility (always created invisible)
	if (cs.style & WS_VISIBLE)
	{
		// place the window on top in z-order before showing it
		::BringWindowToTop(hWnd);

		// show it as specified
		if (cs.style & WS_MINIMIZE)
			ShowWindow(SW_SHOWMINIMIZED);
		else if (cs.style & WS_MAXIMIZE)
			ShowWindow(SW_SHOWMAXIMIZED);
		else
			ShowWindow(SW_SHOWNORMAL);

		// make sure it is active (visibility == activation)
		pParentWnd->MDIActivate(this);

		// refresh MDI Window menu
		::SendMessage(pParentWnd->m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0);
	}

	ASSERT(hWnd == m_hWnd);
	return TRUE;
}





_INLINE BOOL CMenu::LoadMenu(LPCTSTR lpszResourceName)
 { 
	return Attach(::LoadMenu(AfxFindResourceHandle(lpszResourceName, RT_MENU), lpszResourceName)); 
}

_INLINE BOOL CMenu::LoadMenu(UINT nIDResource)
{
	return Attach(::LoadMenuW(AfxGetApp()->m_hInstance, MAKEINTRESOURCEW(nIDResource)));
}

#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER

_INLINE CDocument* CMultiDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible)
{
	CDocument* pDocument = CreateNewDocument();
	if (pDocument == NULL)
	{
		TRACE(traceAppMsg, 0, "CDocTemplate::CreateNewDocument returned NULL.\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}
	ASSERT_VALID(pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	CFrameWnd* pFrame = CreateNewFrame(pDocument, NULL);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		delete pDocument;       // explicit delete on error
		return NULL;
	}
	ASSERT_VALID(pFrame);

	if (lpszPathName == NULL)
	{
		// create a new document - with default document name
		SetDefaultTitle(pDocument);

		if (!pDocument->OnNewDocument())
		{
			// user has be alerted to what failed in OnNewDocument
			TRACE(traceAppMsg, 0, "CDocument::OnNewDocument returned FALSE.\n");
			pFrame->DestroyWindow();
			return NULL;
		}

		// it worked, now bump untitled count
		m_nUntitledCount++;
	}
	else
	{
		// open an existing document
		CWaitCursor wait;
		if (!pDocument->OnOpenDocument(lpszPathName))
		{
			// user has be alerted to what failed in OnOpenDocument
			TRACE(traceAppMsg, 0, "CDocument::OnOpenDocument returned FALSE.\n");
			pFrame->DestroyWindow();
			return NULL;
		}
		pDocument->SetPathName(lpszPathName, bAddToMRU);
		pDocument->OnDocumentEvent(CDocument::onAfterOpenDocument);
	}

	InitialUpdateFrame(pFrame, pDocument, bMakeVisible);
	return pDocument;
}

#endif // MTL_REMOVE_CWINAPP_DOCMANAGER


#ifndef MTL_REMOVE_CFRAMEWND_VIEW

_INLINE void CPreviewView::OnDisplayPageNumber(UINT nPage, UINT nPagesDisplayed)
{
	UINT nEndPage = nPage + nPagesDisplayed - 1;

	CWinThread *pThread = AfxGetThread();
	ASSERT(pThread);
	CFrameWnd* pParent = (CFrameWnd*)pThread->m_pMainWnd;
	ASSERT_VALID(pParent);
	ASSERT_KINDOF(CFrameWnd, pParent);

	int nSubString = (nPagesDisplayed == 1) ? 0 : 1;

	CString s;
	BOOL bOK = AfxExtractSubString(s, m_pPreviewInfo->m_strPageDesc, nSubString);
	if (bOK)
	{
		TCHAR szBuf[80];
		int nResult;

		if (nSubString == 0)
		{
			nResult = wsprintf(szBuf, s, nPage);
		}
		else
		{
			nResult = wsprintf(szBuf, s, nPage, nEndPage);
		}

		if (nResult > 0)
		{
			pParent->SendMessage(WM_SETMESSAGESTRING, 0, (LPARAM)(LPVOID)szBuf);
		}
		else
		{
			bOK = FALSE;
		}
	}

	if (!bOK)
	{
		TRACE(traceAppMsg, 0, "Malformed Page Description string. Could not get string %d.\n", nSubString);
	}
}

_INLINE void CPreviewView::OnNumPageChange()
{
	ASSERT(m_nPages == 1 || m_nPages == 2);
	m_nPages = 3 - m_nPages;    // Toggle between 1 and 2
#ifndef MTL_REMOVE_CWINAPP_PRINT
	AfxGetApp()->m_nNumPreviewPages = m_nPages;
#endif // MTL_REMOVE_CWINAPP_PRINT
	m_nZoomOutPages = m_nPages;

	// Just do this to set the status correctly and invalidate
	SetCurrentPage(m_nCurrentPage, TRUE);
}

_INLINE void CPreviewView::OnPreviewPrint()
{
	// cause print (can be overridden by catching the command)
	CFrameWnd *pOrigFrame = m_pOrigView->EnsureParentFrame();

	OnPreviewClose();
	CWinApp *pCWinApp = AfxGetApp();
	ASSERT(pCWinApp);
	CWnd *pMainWnd = pCWinApp->m_pMainWnd;
	ASSERT_VALID(pMainWnd);

	// ensure we print the correct frame
	pOrigFrame->SendMessage(WM_COMMAND, ID_FILE_PRINT);
}

#endif // MTL_REMOVE_CFRAMEWND_VIEW




_INLINE BOOL CPropertySheetMTL::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	if ((nCode != CN_COMMAND && nCode != CN_UPDATE_COMMAND_UI) || !IS_COMMAND_ID(nID) || nID >= 0xf000)
	{
		// control notification or non-command button or system command
		return FALSE;       // not routed any further
	}

	// if we have an owner window, give it second crack
	CWnd* pOwner = GetParent();
	if (pOwner != NULL)
	{
		TRACE(traceCmdRouting, 1, "Routing command id 0x%04X to owner window.\n", nID);

		ASSERT(pOwner != this);
		if (pOwner->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	// last crack goes to the current CWinThread object
	CWinThread* pThread = AfxGetThread();
	if (pThread != NULL)
	{
		TRACE(traceCmdRouting, 1, "Routing command id 0x%04X to app.\n", nID);

		if (pThread->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	TRACE(traceCmdRouting, 1, "IGNORING command id 0x%04X sent to %hs dialog.\n", nID, GetRuntimeClass()->m_lpszClassName);

	return FALSE;
}

_INLINE BOOL CPropertySheet::Create(CWnd* pParentWnd, DWORD dwStyle, DWORD dwExStyle)
{
	ENSURE(0 == (m_psh.dwFlags & PSH_AEROWIZARD));

	_AFX_THREAD_STATE* pState = AfxGetThreadState();

	// Calculate the default window style.
	if (dwStyle == (DWORD)-1)
	{
		pState->m_dwPropStyle = DS_MODALFRAME | DS_3DLOOK | DS_CONTEXTHELP |
								DS_SETFONT | WS_POPUP | WS_VISIBLE | WS_CAPTION;

		// Wizards don't have WS_SYSMENU.
		if (!IsWizard())
			pState->m_dwPropStyle |= WS_SYSMENU;
	}
	else
	{
		pState->m_dwPropStyle = dwStyle;
	}
	pState->m_dwPropExStyle = dwExStyle;

	ASSERT_VALID(this);
	ASSERT(m_hWnd == NULL);

	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));
	AfxDeferRegisterClass(AFX_WNDCOMMCTLSNEW_REG);

#ifdef _UNICODE
	//AfxInitNetworkAddressControl();
#endif

	// finish building PROPSHEETHEADER structure
	BuildPropPageArray();

	m_bModeless = TRUE;
	m_psh.dwFlags |= (PSH_MODELESS | PSH_USECALLBACK);
	m_psh.pfnCallback = AfxPropSheetCallback;
	m_psh.hwndParent = pParentWnd->GetSafeHwnd();

	// hook the window creation process
	HookWindowCreate(this);
	HWND hWnd = (HWND)PropertySheet(&m_psh);

	if (hWnd == (HWND)-1)
	{
		// An error occurred in the creation of the sheet, so assuming that the window
		// was destroyed and cleanup was done via PostNcDestroy, which was called.  If a
		// memory leak is reported, the window was not deleted in class::PostNcDestroy.
		return FALSE;
	}

	// cleanup on failure, otherwise return TRUE
	if (!UnhookWindowCreate())
		PostNcDestroy();    // cleanup if Create fails

	// setting a custom property in our window
	HGLOBAL hMem = GlobalAlloc(GPTR, sizeof(BOOL));
	BOOL* pBool = (BOOL*)(GlobalLock(hMem));
	if (pBool != NULL)
	{
		*pBool = TRUE;
		GlobalUnlock(hMem);
		if (SetProp(this->m_hWnd, PROP_CLOSEPENDING_NAME, hMem) == 0)
		{
			GlobalFree(hMem);
			this->DestroyWindow();
			return FALSE;
		}
	}
	else
	{
		this->DestroyWindow();
		return FALSE;
	}

	if (hWnd == NULL || hWnd == (HWND)-1)
	{
		return FALSE;
	}

	ASSERT(hWnd == m_hWnd);
	return TRUE;
}

_INLINE INT_PTR CPropertySheetMTL::DoModal()
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd == NULL);

	// register common controls
	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));
	AfxDeferRegisterClass(AFX_WNDCOMMCTLSNEW_REG);

#ifdef _UNICODE
	//AfxInitNetworkAddressControl();
#endif

	// finish building PROPSHEETHEADER structure
	BuildPropPageArray();

	// allow OLE servers to disable themselves
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL)
		pApp->EnableModeless(FALSE);

	// find parent HWND
	HWND hWndTop;
	HWND hWndParent = CWnd::GetSafeOwner_(m_pParentWnd->GetSafeHwnd(), &hWndTop);
	m_psh.hwndParent = hWndParent;
	BOOL bEnableParent = FALSE;
	if (hWndParent != NULL && ::IsWindowEnabled(hWndParent))
	{
		::EnableWindow(hWndParent, FALSE);
		bEnableParent = TRUE;
	}
	HWND hWndCapture = ::GetCapture();
	if (hWndCapture != NULL)
		::SendMessage(hWndCapture, WM_CANCELMODE, 0, 0);

	// setup for modal loop and creation
	m_nModalResult = 0;
	if (!(PSH_AEROWIZARD & m_psh.dwFlags))
	{
		m_nFlags |= WF_CONTINUEMODAL;
	}

	INT_PTR nResult = 0;

	// hook for creation of window
	HookWindowCreate(this);
	if (PSH_AEROWIZARD & m_psh.dwFlags)
	{
		nResult = PropertySheet(&m_psh);
		UnhookWindowCreate();
		m_hWnd = NULL;
	}
	else
	{
		m_psh.dwFlags |= PSH_MODELESS;
		HWND hWnd = (HWND)PropertySheet(&m_psh);
		m_psh.dwFlags &= ~PSH_MODELESS;
		UnhookWindowCreate();

		// handle error
		if (hWnd == NULL || hWnd == (HWND)-1)
		{
			m_nFlags &= ~WF_CONTINUEMODAL;
		}

		nResult = m_nModalResult;
		if (ContinueModal())
		{
			// enter modal loop
			DWORD dwFlags = MLF_SHOWONIDLE;
			if (GetStyle() & DS_NOIDLEMSG)
				dwFlags |= MLF_NOIDLEMSG;
			nResult = RunModalLoop(dwFlags);
		}

		// hide the window before enabling parent window, etc.
		if (m_hWnd != NULL)
		{
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW |
							SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		}
	}

	if (bEnableParent)
		::EnableWindow(hWndParent, TRUE);
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
		::SetActiveWindow(hWndParent);

	if (!(PSH_AEROWIZARD & m_psh.dwFlags))
	{
		// cleanup
		DestroyWindow();
	}

	// allow OLE servers to enable themselves
	if (pApp != NULL)
		pApp->EnableModeless(TRUE);
	if (hWndTop != NULL)
		::EnableWindow(hWndTop, TRUE);

	return nResult;
}






_INLINE CDocument* CSingleDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible)
{
	CDocument* pDocument = NULL;
	CFrameWnd* pFrame = NULL;
	BOOL bCreated = FALSE;      // => doc and frame created
	BOOL bWasModified = FALSE;

	if (m_pOnlyDoc != NULL)
	{
		// already have a document - reinit it
		pDocument = m_pOnlyDoc;
		if (!pDocument->SaveModified())
		{
			// set a flag to indicate that the document being opened should not
			// be removed from the MRU list, if it was being opened from there
			AfxGetApp()->m_bRemoveFromMRU = FALSE;
			return NULL;        // leave the original one
		}

		pFrame = (CFrameWnd*)AfxGetMainWnd();
		ASSERT(pFrame != NULL);
		ASSERT_KINDOF(CFrameWnd, pFrame);
		ASSERT_VALID(pFrame);
	}
	else
	{
		// create a new document
		pDocument = CreateNewDocument();
		ASSERT(pFrame == NULL);     // will be created below
		bCreated = TRUE;
	}

	if (pDocument == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}
	ASSERT(pDocument == m_pOnlyDoc);

	if (pFrame == NULL)
	{
		ASSERT(bCreated);

		// create frame - set as main document frame
		BOOL bAutoDelete = pDocument->m_bAutoDelete;
		pDocument->m_bAutoDelete = FALSE;
		// don't destroy if something goes wrong
		pFrame = CreateNewFrame(pDocument, NULL);
		pDocument->m_bAutoDelete = bAutoDelete;
		if (pFrame == NULL)
		{
			AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
			delete pDocument;       // explicit delete on error
			return NULL;
		}
	}

	if (lpszPathName == NULL)
	{
		// create a new document
		SetDefaultTitle(pDocument);

		if (!pDocument->OnNewDocument())
		{
			// user has been alerted to what failed in OnNewDocument
			TRACE(traceAppMsg, 0, "CDocument::OnNewDocument returned FALSE.\n");
			if (bCreated)
				pFrame->DestroyWindow();    // will destroy document
			return NULL;
		}
	}
	else
	{
		CWaitCursor wait;

		// open an existing document
		bWasModified = pDocument->IsModified();
		pDocument->SetModifiedFlag(FALSE);  // not dirty for open

		if (!pDocument->OnOpenDocument(lpszPathName))
		{
			// user has been alerted to what failed in OnOpenDocument
			TRACE(traceAppMsg, 0, "CDocument::OnOpenDocument returned FALSE.\n");
			if (bCreated)
			{
				pFrame->DestroyWindow();    // will destroy document
			}
			else if (!pDocument->IsModified())
			{
				// original document is untouched
				pDocument->SetModifiedFlag(bWasModified);
			}
			else
			{
				// we corrupted the original document
				SetDefaultTitle(pDocument);

				if (!pDocument->OnNewDocument())
				{
					TRACE(traceAppMsg, 0, "Error: OnNewDocument failed after trying "
						"to open a document - trying to continue.\n");
					// assume we can continue
				}
			}
			return NULL;        // open failed
		}
		pDocument->SetPathName(lpszPathName, bAddToMRU);
		pDocument->OnDocumentEvent(CDocument::onAfterOpenDocument);
	}

	if (bCreated && AfxGetApp()->m_pMainWnd == NULL)
	{
		// set as main frame (InitialUpdateFrame will show the window)
		AfxGetApp()->m_pMainWnd = pFrame;
	}
	InitialUpdateFrame(pFrame, pDocument, bMakeVisible);

	return pDocument;
}




_INLINE void CRecentFileList::ReadList()    // reads from registry or ini file
{
	ASSERT(m_arrNames != NULL);
	ASSERT(!m_strSectionName.IsEmpty());
	ASSERT(!m_strEntryFormat.IsEmpty());
	int nLen = m_strEntryFormat.GetLength() + 10;
	LPTSTR pszEntry = new TCHAR[nLen];
	CWinApp* pApp = AfxGetApp();
	for (int iMRU = 0; iMRU < m_nSize; iMRU++)
	{
		wsprintf(pszEntry, m_strEntryFormat, iMRU + 1);
		m_arrNames[iMRU] = pApp->GetProfileString(m_strSectionName, pszEntry, _T(""));
	}
	delete[] pszEntry;
}

_INLINE void CRecentFileList::UpdateMenu(CCmdUI* pCmdUI)
{
	ENSURE_ARG(pCmdUI != NULL);
	ASSERT(m_arrNames != NULL);

	CMenu* pMenu = pCmdUI->m_pMenu;
	if (m_strOriginal.IsEmpty() && pMenu != NULL)
		pMenu->GetMenuString(pCmdUI->m_nID, m_strOriginal, MF_BYCOMMAND);

	if (m_arrNames[0].IsEmpty())
	{
		// no MRU files
		if (!m_strOriginal.IsEmpty())
			pCmdUI->SetText(m_strOriginal);
		pCmdUI->Enable(FALSE);
		return;
	}

	if (pCmdUI->m_pMenu == NULL)
		return;

	int iMRU;
	for (iMRU = 0; iMRU < m_nSize; iMRU++)
		pCmdUI->m_pMenu->DeleteMenu(pCmdUI->m_nID + iMRU, MF_BYCOMMAND);

	TCHAR szCurDir[_MAX_PATH];
	DWORD dwDirLen = GetCurrentDirectory(_MAX_PATH, szCurDir);
	if (dwDirLen == 0 || dwDirLen >= _MAX_PATH)
		return;	// Path too long

	int nCurDir = lstrlen(szCurDir);
	ASSERT(nCurDir >= 0);
	szCurDir[nCurDir] = '\\';
	szCurDir[++nCurDir] = '\0';

	CString strName;
	CString strTemp;
	for (iMRU = 0; iMRU < m_nSize; iMRU++)
	{
		if (!GetDisplayName(strName, iMRU, szCurDir, nCurDir))
			break;

		// double up any '&' characters so they are not underlined
		LPCTSTR lpszSrc = strName;
		LPTSTR lpszDest = strTemp.GetBuffer(strName.GetLength() * 2);
		while (*lpszSrc != 0)
		{
			if (*lpszSrc == '&')
				*lpszDest++ = '&';
			if (_istlead(*lpszSrc))
				*lpszDest++ = *lpszSrc++;
			*lpszDest++ = *lpszSrc++;
		}
		*lpszDest = 0;
		strTemp.ReleaseBuffer();

		// insert mnemonic + the file name
		TCHAR buf[10];
		int nItem = ((iMRU + m_nStart) % _AFX_MRU_MAX_COUNT) + 1;


		// number &1 thru &9, then 1&0, then 11 thru ...
		if (nItem > 10)
			wsprintf(buf, _T("%d "), nItem);
		else if (nItem == 10)
			lstrcpy(buf, _T("1&0 "));
		else
			wsprintf(buf, _T("&%d "), nItem);

		pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++,
							MF_STRING | MF_BYPOSITION, pCmdUI->m_nID++,
							CString(buf) + strTemp);
	}

	// update end menu count
	pCmdUI->m_nIndex--; // point to last menu added
	pCmdUI->m_nIndexMax = pCmdUI->m_pMenu->GetMenuItemCount();

	pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled
}


_INLINE void CRecentFileList::WriteList()
{
	ASSERT(m_arrNames != NULL);
	ASSERT(!m_strSectionName.IsEmpty());
	ASSERT(!m_strEntryFormat.IsEmpty());
	int nLen = m_strEntryFormat.GetLength() + 10;
	LPTSTR pszEntry = new TCHAR[nLen];
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString(m_strSectionName, NULL, NULL);
	for (int iMRU = 0; iMRU < m_nSize; iMRU++)
	{
		wsprintf(pszEntry, m_strEntryFormat, iMRU + 1);
		if (!m_arrNames[iMRU].IsEmpty())
		{
			pApp->WriteProfileString(m_strSectionName, pszEntry, m_arrNames[iMRU]);
		}
	}
	delete[] pszEntry;
}


_INLINE BOOL CToolBar::LoadToolBar(LPCTSTR lpszResourceName)
{
	ASSERT_VALID(this);
	ASSERT(lpszResourceName != NULL);

	// determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
	if (hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return FALSE;

	CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
	if (pData == NULL)
		return FALSE;
	ASSERT(pData->wVersion == 1);

	UINT* pItems = new UINT[pData->wItemCount];
	for (int i = 0; i < pData->wItemCount; i++)
		pItems[i] = pData->items()[i];
	BOOL bResult = SetButtons(pItems, pData->wItemCount);
	delete[] pItems;

	if (bResult)
	{
		// set new sizes of the buttons
		CSize sizeImage(pData->wWidth, pData->wHeight);
		CSize sizeButton(pData->wWidth + 7, pData->wHeight + 7);
		SetSizes(sizeButton, sizeImage);

		// load bitmap now that sizes are known by the toolbar control
		bResult = LoadBitmap(lpszResourceName);
	}

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return bResult;
}

_INLINE BOOL CToolBar::LoadBitmap(LPCTSTR lpszResourceName)
{
	ASSERT_VALID(this);
	ASSERT(lpszResourceName != NULL);

	// determine location of the bitmap in resource fork
	HINSTANCE hInstImageWell = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);
	HRSRC hRsrcImageWell = ::FindResource(hInstImageWell, lpszResourceName, RT_BITMAP);
	if (hRsrcImageWell == NULL)
		return FALSE;

	// load the bitmap
	HBITMAP hbmImageWell = AfxLoadSysColorBitmap(hInstImageWell, hRsrcImageWell);

	// tell common control toolbar about the new bitmap
	if (!AddReplaceBitmap(hbmImageWell))
		return FALSE;

	// remember the resource handles so the bitmap can be recolored if necessary
	m_hInstImageWell = hInstImageWell;
	m_hRsrcImageWell = hRsrcImageWell;
	return TRUE;
}


_INLINE void CView::OnFilePrint()
{
	// get default print info
	CPrintInfo printInfo;
	ASSERT(printInfo.m_pPD != NULL);    // must be set

	if (LOWORD(GetCurrentMessage()->wParam) == ID_FILE_PRINT_DIRECT)
	{
		CCommandLineInfo* pCmdInfo = AfxGetApp()->m_pCmdInfo;

		if (pCmdInfo != NULL)
		{
			if (pCmdInfo->m_nShellCommand == CCommandLineInfo::FilePrintTo)
			{
				printInfo.m_pPD->m_pd.hDC = ::CreateDC(pCmdInfo->m_strDriverName,
					pCmdInfo->m_strPrinterName, pCmdInfo->m_strPortName, NULL);
				if (printInfo.m_pPD->m_pd.hDC == NULL)
				{
					AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
					return;
				}
			}
		}

		printInfo.m_bDirect = TRUE;
	}

	if (OnPreparePrinting(&printInfo))
	{
		// hDC must be set (did you remember to call DoPreparePrinting?)
		ASSERT(printInfo.m_pPD->m_pd.hDC != NULL);

		// gather file to print to if print-to-file selected
		CString strOutput;
		if (printInfo.m_pPD->m_pd.Flags & PD_PRINTTOFILE && !printInfo.m_bDocObject)
		{
			// construct CFileDialog for browsing
			CString strDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULTEXT));
			CString strPrintDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULT));
			CString strFilter(MAKEINTRESOURCE(AFX_IDS_PRINTFILTER));
			CString strCaption(MAKEINTRESOURCE(AFX_IDS_PRINTCAPTION));
			CFileDialog dlg(FALSE, strDef, strPrintDef,
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, NULL, 0);
			dlg.m_ofn.lpstrTitle = strCaption;

			if (dlg.DoModal() != IDOK)
				return;

			// set output device to resulting path name
			strOutput = dlg.GetPathName();
		}

		// set up document info and start the document printing process
		CString strTitle;
		CDocument* pDoc = GetDocument();
		if (pDoc != NULL)
			strTitle = pDoc->GetTitle();
		else
			EnsureParentFrame()->GetWindowText(strTitle);
		DOCINFO docInfo;
		memset(&docInfo, 0, sizeof(DOCINFO));
		docInfo.cbSize = sizeof(DOCINFO);
		docInfo.lpszDocName = strTitle;
		CString strPortName;
		if (strOutput.IsEmpty())
		{
			docInfo.lpszOutput = NULL;
			strPortName = printInfo.m_pPD->GetPortName();
		}
		else
		{
			docInfo.lpszOutput = strOutput;
			AfxGetFileTitle(strOutput,
				strPortName.GetBuffer(_MAX_PATH), _MAX_PATH);
		}

		// setup the printing DC
		CDC dcPrint;
		if (!printInfo.m_bDocObject)
		{
			dcPrint.Attach(printInfo.m_pPD->m_pd.hDC);  // attach printer dc
			dcPrint.m_bPrinting = TRUE;
		}
		OnBeginPrinting(&dcPrint, &printInfo);

		if (!printInfo.m_bDocObject)
			dcPrint.SetAbortProc(CPrintingDialog::_AfxAbortProc);

		// disable main window while printing & init printing status dialog
		// Store the Handle of the Window in a temp so that it can be enabled 
		// once the printing is finished
		CWnd * hwndTemp = AfxGetMainWnd();
		hwndTemp->EnableWindow(FALSE);
		CPrintingDialog dlgPrintStatus(this);

		CString strTemp;
		dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_DOCNAME, strTitle);
		dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PRINTERNAME,
			printInfo.m_pPD->GetDeviceName());
		dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PORTNAME, strPortName);
		dlgPrintStatus.ShowWindow(SW_SHOW);
		dlgPrintStatus.UpdateWindow();

		// start document printing process
		if (!printInfo.m_bDocObject)
		{
			printInfo.m_nJobNumber = dcPrint.StartDoc(&docInfo);
			if (printInfo.m_nJobNumber == SP_ERROR)
			{
				// enable main window before proceeding
				hwndTemp->EnableWindow(TRUE);

				// cleanup and show error message
				OnEndPrinting(&dcPrint, &printInfo);
				dlgPrintStatus.DestroyWindow();
				dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
				AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
				return;
			}
		}

		// Guarantee values are in the valid range
		UINT nEndPage = printInfo.GetToPage();
		UINT nStartPage = printInfo.GetFromPage();

		if (nEndPage < printInfo.GetMinPage())
			nEndPage = printInfo.GetMinPage();
		if (nEndPage > printInfo.GetMaxPage())
			nEndPage = printInfo.GetMaxPage();

		if (nStartPage < printInfo.GetMinPage())
			nStartPage = printInfo.GetMinPage();
		if (nStartPage > printInfo.GetMaxPage())
			nStartPage = printInfo.GetMaxPage();

		int nStep = (nEndPage >= nStartPage) ? 1 : -1;
		nEndPage = (nEndPage == 0xffff) ? 0xffff : nEndPage + nStep;

		VERIFY(strTemp.LoadString(AFX_IDS_PRINTPAGENUM));

		// If it's a doc object, we don't loop page-by-page
		// because doc objects don't support that kind of levity.

		BOOL bError = FALSE;
		if (printInfo.m_bDocObject)
		{
			OnPrepareDC(&dcPrint, &printInfo);
			OnPrint(&dcPrint, &printInfo);
		}
		else
		{
			// begin page printing loop
			for (printInfo.m_nCurPage = nStartPage; printInfo.m_nCurPage != nEndPage; printInfo.m_nCurPage += nStep)
			{
				OnPrepareDC(&dcPrint, &printInfo);

				// check for end of print
				if (!printInfo.m_bContinuePrinting)
					break;

				// write current page
				TCHAR szBuf[80];
				wsprintf(szBuf, strTemp, printInfo.m_nCurPage);

				dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PAGENUM, szBuf);

				// set up drawing rect to entire page (in logical coordinates)
				printInfo.m_rectDraw.SetRect(0, 0,
					dcPrint.GetDeviceCaps(HORZRES),
					dcPrint.GetDeviceCaps(VERTRES));
				dcPrint.DPtoLP(&printInfo.m_rectDraw);

				// attempt to start the current page
				if (dcPrint.StartPage() < 0)
				{
					bError = TRUE;
					break;
				}

				// must call OnPrepareDC on newer versions of Windows because
				// StartPage now resets the device attributes.
				OnPrepareDC(&dcPrint, &printInfo);

				ASSERT(printInfo.m_bContinuePrinting);

				// page successfully started, so now render the page
				OnPrint(&dcPrint, &printInfo);
				if ((nStep > 0) && // pages are printed in ascending order
					(nEndPage > printInfo.GetMaxPage() + nStep)) // out off pages
				{
					// OnPrint may have set the last page
					// because the end of the document was reached.
					// The loop must not continue with the next iteration.
					nEndPage = printInfo.GetMaxPage() + nStep;
				}

				// If the user restarts the job when it's spooling, all 
				// subsequent calls to EndPage returns < 0. The first time
				// GetLastError returns ERROR_PRINT_CANCELLED
				if (dcPrint.EndPage() < 0 && (GetLastError() != ERROR_SUCCESS))
				{
					HANDLE hPrinter;
					if (!OpenPrinter((LPTSTR)(LPCTSTR)printInfo.m_pPD->GetDeviceName(), &hPrinter, NULL))
					{
						bError = TRUE;
						break;
					}

					DWORD cBytesNeeded;
					if (!GetJob(hPrinter, printInfo.m_nJobNumber, 1, NULL, 0, &cBytesNeeded))
					{
						if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
						{
							bError = TRUE;
							break;
						}
					}

					JOB_INFO_1 *pJobInfo;
					if ((pJobInfo = (JOB_INFO_1 *)malloc(cBytesNeeded)) == NULL)
					{
						bError = TRUE;
						break;
					}

					DWORD cBytesUsed;

					BOOL bRet = GetJob(hPrinter, printInfo.m_nJobNumber, 1, LPBYTE(pJobInfo), cBytesNeeded, &cBytesUsed);

					DWORD dwJobStatus = pJobInfo->Status;

					free(pJobInfo);
					pJobInfo = NULL;

					// if job status is restart, just continue
					if (!bRet || !(dwJobStatus & JOB_STATUS_RESTART))
					{
						bError = TRUE;
						break;
					}
				}

				if (!CPrintingDialog::_AfxAbortProc(dcPrint.m_hDC, 0))
				{
					bError = TRUE;
					break;
				}
			}
		}

		// cleanup document printing process
		if (!printInfo.m_bDocObject)
		{
			if (!bError)
				dcPrint.EndDoc();
			else
				dcPrint.AbortDoc();
		}

		hwndTemp->EnableWindow();    // enable main window

		OnEndPrinting(&dcPrint, &printInfo);    // clean up after printing
		dlgPrintStatus.DestroyWindow();

		dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
	}
}

_INLINE BOOL CView::DoPreparePrinting(CPrintInfo* pInfo)
{
#ifndef MTL_REMOVE_CWINAPP_PRINT
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_pPD != NULL);

	if (pInfo->m_pPD->m_pd.nMinPage > pInfo->m_pPD->m_pd.nMaxPage)
		pInfo->m_pPD->m_pd.nMaxPage = pInfo->m_pPD->m_pd.nMinPage;

	// don't prompt the user if we're doing print preview, printing directly,
	// or printing via IPrint and have been instructed not to ask

	CWinApp* pApp = AfxGetApp();
	if (pInfo->m_bPreview || pInfo->m_bDirect ||
		(pInfo->m_bDocObject && !(pInfo->m_dwFlags & PRINTFLAG_PROMPTUSER)))
	{
		if (pInfo->m_pPD->m_pd.hDC == NULL)
		{
			// if no printer set then, get default printer DC and create DC without calling
			//   print dialog.
			if (!pApp->GetPrinterDeviceDefaults(&pInfo->m_pPD->m_pd))
			{
				// bring up dialog to alert the user they need to install a printer.
				if (!pInfo->m_bDocObject || (pInfo->m_dwFlags & PRINTFLAG_MAYBOTHERUSER))
					if (pApp->DoPrintDialog(pInfo->m_pPD) != IDOK)
						return FALSE;
			}

			if (pInfo->m_pPD->m_pd.hDC == NULL)
			{
				// call CreatePrinterDC if DC was not created by above
				if (pInfo->m_pPD->CreatePrinterDC() == NULL)
					return FALSE;
			}
		}

		// set up From and To page range from Min and Max
		pInfo->m_pPD->m_pd.nFromPage = (WORD)pInfo->GetMinPage();
		pInfo->m_pPD->m_pd.nToPage = (WORD)pInfo->GetMaxPage();
	}
	else
	{
		// otherwise, bring up the print dialog and allow user to change things
		// preset From-To range same as Min-Max range
		pInfo->m_pPD->m_pd.nFromPage = (WORD)pInfo->GetMinPage();
		pInfo->m_pPD->m_pd.nToPage = (WORD)pInfo->GetMaxPage();

		if (pApp->DoPrintDialog(pInfo->m_pPD) != IDOK)
			return FALSE;       // do not print
	}

	ASSERT(pInfo->m_pPD != NULL);
	ASSERT(pInfo->m_pPD->m_pd.hDC != NULL);
	if (pInfo->m_pPD->m_pd.hDC == NULL)
		return FALSE;

	pInfo->m_nNumPreviewPages = pApp->m_nNumPreviewPages;
	ENSURE(pInfo->m_strPageDesc.LoadString(AFX_IDS_PREVIEWPAGEDESC));
	return TRUE;
#else //  MTL_REMOVE_CWINAPP_PRINT
	return FALSE;
#endif //  MTL_REMOVE_CWINAPP_PRINT
}






_INLINE void CWMHnd::OnNcDestroy()
{
	// cleanup main and active windows
	CWinApp* pCWinApp = AfxGetApp();
	if (pCWinApp != NULL)
	{
		if (pCWinApp->m_pMainWnd == this)
		{
#ifndef _USRDLL
			AfxPostQuitMessage(0);
#endif
			pCWinApp->m_pMainWnd = NULL;
		}
	}
}





_INLINE void CWnd::OnNcDestroy()
{
	// cleanup main and active windows
	CWMHnd::OnNcDestroy();

	// cleanup tooltip support
	if (m_nFlags & WF_TOOLTIPS)
	{
		CToolTipCtrl* pToolTip = AfxGetModuleThreadState()->m_pToolTip;
		if (pToolTip->GetSafeHwnd() != NULL)
		{
			TOOLINFO ti; memset(&ti, 0, sizeof(TOOLINFO));
			ti.cbSize = sizeof(AFX_OLDTOOLINFO);
			ti.uFlags = TTF_IDISHWND;
			ti.hwnd = m_hWnd;
			ti.uId = (UINT_PTR)m_hWnd;
			pToolTip->SendMessage(TTM_DELTOOL, 0, (LPARAM)&ti);
		}
	}

	CWMHnd::UnsubclassWindow();

	// call special post-cleanup routine
	PostNcDestroy();
}

_INLINE void CWnd::_AfxRelayToolTipMessage(CToolTipCtrl* pToolTip, MSG* pMsg)
{
	// transate the message based on TTM_WINDOWFROMPOINT
	MSG msg = *pMsg;
	msg.hwnd = (HWND)pToolTip->SendMessage(TTM_WINDOWFROMPOINT, 0, (LPARAM)&msg.pt);
	CPoint pt = pMsg->pt;
	if (msg.message >= WM_MOUSEFIRST && msg.message <= AFX_WM_MOUSELAST)
		::ScreenToClient(msg.hwnd, &pt);
	msg.lParam = MAKELONG(pt.x, pt.y);

	// relay mouse event before deleting old tool
	pToolTip->SendMessage(TTM_RELAYEVENT, 0, (LPARAM)&msg);
}

_INLINE void CWnd::CancelToolTips(BOOL bKeys)
{
	// check for active tooltip
	CToolTipCtrl* pToolTip = AfxGetApp()->m_pToolTip;
	if (pToolTip->GetSafeHwnd() != NULL)
		pToolTip->SendMessage(TTM_ACTIVATE, FALSE);

	// check for active control bar fly-by status
	CControlBar* pLastStatus = AfxGetApp()->m_pLastStatus;
	if (bKeys && pLastStatus != NULL && GetKeyState(VK_LBUTTON) >= 0)
		pLastStatus->SetStatusText((INT_PTR)(-1));
}

_INLINE BOOL CWnd::_EnableToolTips(BOOL bEnable, UINT nFlag)
{
	ASSERT(nFlag == WF_TOOLTIPS || nFlag == WF_TRACKINGTOOLTIPS);

	CToolTipCtrl* pToolTip = AfxGetApp()->m_pToolTip;

	if (!bEnable)
	{
		// nothing to do if tooltips not enabled
		if (!(m_nFlags & nFlag))
			return TRUE;

		// cancel tooltip if this window is active
		if (AfxGetApp()->m_pLastHit == this)
			CancelToolTips(TRUE);

		// remove "dead-area" toolbar
		if (pToolTip->GetSafeHwnd() != NULL)
		{
			TOOLINFO ti; memset(&ti, 0, sizeof(TOOLINFO));
			ti.cbSize = sizeof(AFX_OLDTOOLINFO);
			ti.uFlags = TTF_IDISHWND;
			ti.hwnd = m_hWnd;
			ti.uId = (UINT_PTR)m_hWnd;
			pToolTip->SendMessage(TTM_DELTOOL, 0, (LPARAM)&ti);
		}

		// success
		m_nFlags &= ~nFlag;
		return TRUE;
	}

	// if already enabled for tooltips, nothing to do
	if (!(m_nFlags & nFlag))
	{
		// success
		AfxGetModuleState()->m_pfnFilterToolTipMessage = &CWnd::_FilterToolTipMessage;
		m_nFlags |= nFlag;
	}
	return TRUE;
}

_INLINE void CWnd::FilterToolTipMessage(MSG* pMsg)
{
	// this CWnd has tooltips enabled
	UINT message = pMsg->message;
	if ((message == WM_MOUSEMOVE || message == WM_NCMOUSEMOVE ||
		message == WM_LBUTTONUP || message == WM_RBUTTONUP ||
		message == WM_MBUTTONUP) &&
		(GetKeyState(VK_LBUTTON) >= 0 && GetKeyState(VK_RBUTTON) >= 0 && GetKeyState(VK_MBUTTON) >= 0))
	{
		AFX_MODULE_THREAD_STATE* pModuleThreadState = AfxGetModuleThreadState();

		// make sure that tooltips are not already being handled
		CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
		while (pWnd != NULL && !(pWnd->m_nFlags & (WF_TOOLTIPS | WF_TRACKINGTOOLTIPS)))
		{
			pWnd = pWnd->GetParent();
		}
		if (pWnd != this)
		{
			if (pWnd == NULL)
			{
				// tooltips not enabled on this CWnd, clear last state data
				pModuleThreadState->m_pLastHit = NULL;
				pModuleThreadState->m_nLastHit = (INT_PTR)(-1);
			}
			return;
		}

		CToolTipCtrl* pToolTip = pModuleThreadState->m_pToolTip;
		CWnd* pOwner = GetParentOwner();
		if (pToolTip != NULL && pToolTip->GetOwner() != pOwner)
		{
			pToolTip->DestroyWindow();
			delete pToolTip;
			pModuleThreadState->m_pToolTip = NULL;
			pToolTip = NULL;
		}
		if (pToolTip == NULL)
		{
			pToolTip = new CToolTipCtrl;
			if (!pToolTip->Create(pOwner, TTS_ALWAYSTIP))
			{
				delete pToolTip;
				return;
			}
			pToolTip->SendMessage(TTM_ACTIVATE, FALSE);
			pModuleThreadState->m_pToolTip = pToolTip;
		}

		ASSERT_VALID(pToolTip);
		ASSERT(::IsWindow(pToolTip->m_hWnd));

		TOOLINFO ti; memset(&ti, 0, sizeof(TOOLINFO));

		// determine which tool was hit
		CPoint point = pMsg->pt;
		::ScreenToClient(m_hWnd, &point);
		TOOLINFO tiHit; memset(&tiHit, 0, sizeof(TOOLINFO));
		tiHit.cbSize = sizeof(AFX_OLDTOOLINFO);
		INT_PTR nHit = OnToolHitTest(point, &tiHit);

		// build new toolinfo and if different than current, register it
		CWnd* pHitWnd = nHit == -1 ? NULL : this;
		if (pModuleThreadState->m_nLastHit != nHit || pModuleThreadState->m_pLastHit != pHitWnd)
		{
			if (nHit != -1)
			{
				// add new tool and activate the tip
				ti = tiHit;
				ti.uFlags &= ~(TTF_NOTBUTTON | TTF_ALWAYSTIP);
				if (m_nFlags & WF_TRACKINGTOOLTIPS)
					ti.uFlags |= TTF_TRACK;
				VERIFY(pToolTip->SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti));
				if ((tiHit.uFlags & TTF_ALWAYSTIP) || IsTopParentActive())
				{
					// allow the tooltip to popup when it should
					pToolTip->SendMessage(TTM_ACTIVATE, TRUE);
					if (m_nFlags & WF_TRACKINGTOOLTIPS)
						pToolTip->SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);

					// bring the tooltip window above other popup windows
					::SetWindowPos(pToolTip->m_hWnd, HWND_TOP, 0, 0, 0, 0,
						SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);
				}
			}
			else
			{
				pToolTip->SendMessage(TTM_ACTIVATE, FALSE);
			}

			// relay mouse event before deleting old tool
			_AfxRelayToolTipMessage(pToolTip, pMsg);

			// now safe to delete the old tool
			if (pModuleThreadState->m_pLastInfo != NULL &&
				pModuleThreadState->m_pLastInfo->cbSize >= sizeof(AFX_OLDTOOLINFO))
				pToolTip->SendMessage(TTM_DELTOOL, 0, (LPARAM)pModuleThreadState->m_pLastInfo);

			pModuleThreadState->m_pLastHit = pHitWnd;
			pModuleThreadState->m_nLastHit = nHit;
			if (pModuleThreadState->m_pLastInfo == NULL)
			{
				pModuleThreadState->m_pLastInfo = new TOOLINFO;
				memset(pModuleThreadState->m_pLastInfo, 0, sizeof(TOOLINFO));
			}
			*pModuleThreadState->m_pLastInfo = tiHit;
		}
		else
		{
			if (m_nFlags & WF_TRACKINGTOOLTIPS)
			{
				POINT pt;

				::GetCursorPos(&pt);
				pToolTip->SendMessage(TTM_TRACKPOSITION, 0, MAKELPARAM(pt.x, pt.y));
			}
			else
			{
				// relay mouse events through the tooltip
				if (nHit != -1)
					_AfxRelayToolTipMessage(pToolTip, pMsg);
			}
		}

		if ((tiHit.lpszText != LPSTR_TEXTCALLBACK) && (tiHit.hinst == 0))
			free(tiHit.lpszText);
	}
	else if (m_nFlags & (WF_TOOLTIPS | WF_TRACKINGTOOLTIPS))
	{
		// make sure that tooltips are not already being handled
		CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
		while (pWnd != NULL && pWnd != this && !(pWnd->m_nFlags & (WF_TOOLTIPS | WF_TRACKINGTOOLTIPS)))
			pWnd = pWnd->GetParent();
		if (pWnd != this)
			return;

		BOOL bKeys = (message >= WM_KEYFIRST && message <= WM_KEYLAST) ||
						(message >= WM_SYSKEYFIRST && message <= WM_SYSKEYLAST);
		if ((m_nFlags & WF_TRACKINGTOOLTIPS) == 0 &&
			(bKeys ||
			(message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK) ||
			(message == WM_RBUTTONDOWN || message == WM_RBUTTONDBLCLK) ||
			(message == WM_MBUTTONDOWN || message == WM_MBUTTONDBLCLK) ||
			(message == WM_NCLBUTTONDOWN || message == WM_NCLBUTTONDBLCLK) ||
			(message == WM_NCRBUTTONDOWN || message == WM_NCRBUTTONDBLCLK) ||
			(message == WM_NCMBUTTONDOWN || message == WM_NCMBUTTONDBLCLK)))
		{
			CWnd::CancelToolTips(bKeys);
		}
	}
}

_INLINE BOOL CWnd::PreTranslateMessage(MSG* pMsg)
{
	// handle tooltip messages (some messages cancel, some may cause it to popup)
	AFX_MODULE_STATE* pModuleState = _AFX_CMDTARGET_GETSTATE();
	if (pModuleState->m_pfnFilterToolTipMessage != NULL)
		(*pModuleState->m_pfnFilterToolTipMessage)(pMsg, this);

	// no default processing
	return FALSE;
}

_INLINE int CWnd::RunModalLoop(DWORD dwFlags)
{
	ASSERT(::IsWindow(m_hWnd)); // window must be created
	ASSERT(!(m_nFlags & WF_MODALLOOP)); // window must not already be in modal state

	// for tracking the idle time state
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;
	BOOL bShowIdle = (dwFlags & MLF_SHOWONIDLE) && !(GetStyle() & WS_VISIBLE);
	HWND hWndParent = ::GetParent(m_hWnd);
	m_nFlags |= (WF_MODALLOOP | WF_CONTINUEMODAL);
	MSG *pMsg = (MSG*)AfxGetCurrentMessage();

	// acquire and dispatch messages until the modal state is done
	for (;;)
	{
		ASSERT(ContinueModal());

		// phase1: check to see if we can do idle work
		while (bIdle &&
			!::PeekMessage(pMsg, NULL, NULL, NULL, PM_NOREMOVE))
		{
			ASSERT(ContinueModal());

			// show the dialog when the message queue goes idle
			if (bShowIdle)
			{
				ShowWindow(SW_SHOWNORMAL);
				UpdateWindow();
				bShowIdle = FALSE;
			}

			// call OnIdle while in bIdle state
			if (!(dwFlags & MLF_NOIDLEMSG) && hWndParent != NULL && lIdleCount == 0)
			{
				// send WM_ENTERIDLE to the parent
				::SendMessage(hWndParent, WM_ENTERIDLE, MSGF_DIALOGBOX, (LPARAM)m_hWnd);
			}
			if ((dwFlags & MLF_NOKICKIDLE) ||
				!SendMessage(WM_KICKIDLE, MSGF_DIALOGBOX, lIdleCount++))
			{
				// stop idle processing next time
				bIdle = FALSE;
			}
		}

		// phase2: pump messages while available
		do
		{
			ASSERT(ContinueModal());

			// pump message, but quit on WM_QUIT
			if (!AfxGetApp()->PumpMessage())
			{
				AfxPostQuitMessage(0);
				return -1;
			}

			// show the window when certain special messages rec'd
			if (bShowIdle &&
				(pMsg->message == 0x118 || pMsg->message == WM_SYSKEYDOWN))
			{
				ShowWindow(SW_SHOWNORMAL);
				UpdateWindow();
				bShowIdle = FALSE;
			}

			if (!ContinueModal())
				goto ExitModal;

			// reset "no idle" state after pumping "normal" message
			if (AfxGetApp()->IsIdleMessage(pMsg))
			{
				bIdle = TRUE;
				lIdleCount = 0;
			}

		} while (::PeekMessage(pMsg, NULL, NULL, NULL, PM_NOREMOVE));
	}

ExitModal:
	m_nFlags &= ~(WF_MODALLOOP | WF_CONTINUEMODAL);
	return m_nModalResult;
}



