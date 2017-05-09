

CWinApp* CWinApp::m_pCWinApp;


// CWnds for setting z-order with SetWindowPos's pWndInsertAfter parameter
const CWMHnd CWMHnd::wndTop(HWND_TOP);
const CWMHnd CWMHnd::wndBottom(HWND_BOTTOM);
const CWMHnd CWMHnd::wndTopMost(HWND_TOPMOST);
const CWMHnd CWMHnd::wndNoTopMost(HWND_NOTOPMOST);


const SIZE CScrollView::sizeDefault = {0,0};
const CRect CFrameWnd::rectDefault(CW_USEDEFAULT, CW_USEDEFAULT, 0 /* 2*CW_USEDEFAULT */, 0 /* 2*CW_USEDEFAULT */);



//IMPLEMENT_DYNAMIC(CObject, CObject)
// special runtime-class structure for CObject (no base class)
const struct CRuntimeClass CObject::classCObject = { "CObject", sizeof(CObject), 0xffff, NULL, NULL, NULL };
CRuntimeClass* CObject::GetRuntimeClass() const { return RUNTIME_CLASS(CObject); }



IMPLEMENT_DYNAMIC(CCmdTarget, CObject)

IMPLEMENT_DYNCREATE(CWnd, CCmdTarget)
IMPLEMENT_DYNAMIC(CDialog, CWnd)

IMPLEMENT_DYNCREATE(CFrameWnd, CWnd)
IMPLEMENT_DYNAMIC(CView, CWnd)
IMPLEMENT_DYNAMIC(CSplitterWnd, CWnd)

#ifndef MTL_REMOVE_CWINAPP_DOCMANAGER
IMPLEMENT_DYNAMIC(CDocument, CCmdTarget)

IMPLEMENT_DYNAMIC(CDocTemplate, CCmdTarget)
IMPLEMENT_DYNAMIC(CSingleDocTemplate, CDocTemplate)

IMPLEMENT_DYNAMIC(CMultiDocTemplate, CDocTemplate)
#endif // MTL_REMOVE_CWINAPP_DOCMANAGER

IMPLEMENT_DYNAMIC(CControlBar, CWnd)
IMPLEMENT_DYNAMIC(CDockBar, CControlBar)

IMPLEMENT_DYNAMIC(CToolBar, CControlBar)


IMPLEMENT_DYNCREATE(CMiniFrameWnd, CFrameWnd)
IMPLEMENT_DYNCREATE(CMiniDockFrameWnd, CMiniFrameWnd)

IMPLEMENT_DYNCREATE(CMDIFrameWnd, CFrameWnd)
IMPLEMENT_DYNCREATE(CMDIChildWnd, CFrameWnd)

IMPLEMENT_DYNAMIC(CScrollView, CView)
IMPLEMENT_DYNAMIC(CFormView, CScrollView)
#ifndef MTL_REMOVE_CFRAMEWND_VIEW
IMPLEMENT_DYNCREATE(CPreviewView, CScrollView)
#endif // MTL_REMOVE_CFRAMEWND_VIEW

IMPLEMENT_DYNAMIC(CCtrlView, CView)
IMPLEMENT_DYNAMIC(CEditView, CCtrlView)
IMPLEMENT_DYNAMIC(CListView, CCtrlView)
IMPLEMENT_DYNAMIC(CTreeView, CCtrlView)

IMPLEMENT_DYNAMIC(CPropertyPageMTL, CDialog)
IMPLEMENT_DYNAMIC(CPropertySheetMTL, CWnd)

IMPLEMENT_DYNAMIC(CFileDialogMTL, CDialog)

IMPLEMENT_DYNAMIC(CTabCtrlMTL, CWnd)
IMPLEMENT_DYNAMIC(CToolTipCtrlMTL, CWnd)



#ifndef _MTL_ATL3

IMPLEMENT_DYNAMIC(CMFCVisualManagerWindows, CObject)

IMPLEMENT_DYNAMIC(CDialogEx, CDialog)

IMPLEMENT_DYNAMIC(CFrameWndEx, CFrameWnd)
IMPLEMENT_DYNAMIC(CMFCPropertyPage, CPropertyPage)
IMPLEMENT_DYNAMIC(CMFCToolTipCtrl, CToolTipCtrl)
//IMPLEMENT_DYNAMIC(CDialogEx, CDialog)
//IMPLEMENT_DYNAMIC(CDialogEx, CDialog)

#endif // _MTL_ATL3



#ifdef _MTL_ATL3

CComModule _AtlWinModule ;

#endif // _MTL_ATL3



#ifndef _USRDLL

extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
#ifdef _MTL_ATL3
	_AtlWinModule.Init(NULL, hInstance) ;
#endif // _MTL_ATL3


	int nReturnCode = -1;
	CWinApp* pApp = AfxGetApp();

	// Windows specific initialization (not done if no CWinApp)
	pApp->m_hInstance = hInstance;
	hPrevInstance; // Obsolete.
	pApp->m_lpCmdLine = lpCmdLine;
	pApp->m_nCmdShow = nCmdShow;
	pApp->SetCurrentHandles();

	AfxGetModuleState()->m_afxContextIsDLL = FALSE;


	// Perform specific initializations
	if (!pApp->InitInstance())
	{
		if (pApp->m_pMainWnd != NULL)
		{
			pApp->m_pMainWnd->DestroyWindow();
		}
		nReturnCode = pApp->ExitInstance();
		goto InitFailure;
	}
	nReturnCode = pApp->Run();

InitFailure:

#ifdef _MTL_ATL3
	_AtlWinModule.Term() ;
#endif // _MTL_ATL3

	return nReturnCode;
}

#else

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
#ifdef _MTL_ATL3
		_AtlWinModule.Init(NULL, hInstance) ;
#endif // _MTL_ATL3
		
		// Initialize DLL's instance(/module) not the app's
		// initialize the single instance DLL
		CWinApp* pApp = AfxGetApp();
		pApp->m_hInstance = hInstance;
		pApp->SetCurrentHandles();

		AfxGetModuleState()->m_afxContextIsDLL = TRUE;

		if (pApp != NULL && !pApp->InitInstance())
		{
			pApp->ExitInstance();
			return FALSE ;       // Init Failed
		}

		return TRUE;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		CWinApp* pApp = AfxGetApp();
		if (pApp != NULL)
			pApp->ExitInstance();

#ifdef _MTL_ATL3
		_AtlWinModule.Term() ;
#endif // _MTL_ATL3
	}

	return TRUE;
}

#endif

