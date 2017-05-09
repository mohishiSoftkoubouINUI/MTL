


class ATL_NO_VTABLE CDocument : public CCmdTarget
{
	DECLARE_DYNAMIC(CDocument)

	public:
		CDocument()
		{
			m_pDocTemplate = NULL;
			m_bModified = FALSE;
			m_bAutoDelete = TRUE;       // default to auto delete document

			ASSERT(m_viewList.IsEmpty());

		}

		~CDocument() ;

	public:
		// メッセージマップ
		DECLARE_MESSAGE_MAP()

	public:
		// Document event notifications
		enum DocumentEvent
		{
			onAfterNewDocument = 0,
			onAfterOpenDocument = 1,
			onAfterSaveDocument = 2,
			onAfterCloseDocument = 3
		};

		// default implementation
		CString m_strTitle;
		CString m_strPathName;
		CDocTemplate* m_pDocTemplate;
		CPtrList m_viewList;                // list of views
		BOOL m_bModified;                   // changed since last saved

		BOOL m_bAutoDelete;         // TRUE => delete document when no more views

	public:

		void OnChangedViewList()
		{
			// if no more views on the document, delete ourself
			// not called if directly closing the document or terminating the app
			if (m_viewList.IsEmpty() && m_bAutoDelete)
			{
				OnCloseDocument();
				return;
			}

			// update the frame counts as needed
			UpdateFrameCounts();
		}

		void OnCloseDocument()
			// must close all views now (no prompting) - usually destroys this
		{
			// destroy all frames viewing this document
			// the last destroy may destroy us
			BOOL bAutoDelete = m_bAutoDelete;
			m_bAutoDelete = FALSE;  // don't destroy document while closing views
			while (!m_viewList.IsEmpty())
			{
				// get frame attached to the view
				CView* pView = (CView*)m_viewList.GetHead();
				ASSERT_VALID(pView);
				CFrameWnd* pFrame = pView->EnsureParentFrame();

				// and close it
				PreCloseFrame(pFrame);
				pFrame->DestroyWindow();
				// will destroy the view as well
			}
			m_bAutoDelete = bAutoDelete;
			OnDocumentEvent(onAfterCloseDocument);

			// clean up contents of document before destroying the document itself
			DeleteContents();

			// delete the document if necessary
			if (m_bAutoDelete)
				delete this;
		}

		virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

		virtual void OnDocumentEvent(DocumentEvent deEvent)
		{
			// check if recovery handling is enabled...
		}

		/////////////////////////////////////////////////////////////////////////////
		// Standard file menu commands

		void OnFileClose()
		{
			if (!SaveModified())
				return;

			// shut it down
			OnCloseDocument();
			// this should destroy the document
		}

		void OnFileSave()
		{
			DoFileSave();
		}

		void OnFileSaveAs()
		{
			if (!DoSave(NULL))
				TRACE(traceAppMsg, 0, "Warning: File save-as failed.\n");
		}

#if 0
		virtual void OnIdle() {}
#endif

		virtual BOOL OnNewDocument()
		{
			DeleteContents();
			m_strPathName.Empty();      // no path name yet
			SetModifiedFlag(FALSE);     // make clean
			OnDocumentEvent(onAfterNewDocument);

			return TRUE;
		}

		virtual BOOL OnOpenDocument(LPCTSTR lpszPathName) ;

		virtual BOOL OnSaveDocument(LPCTSTR lpszPathName) ;


		// Operations
		void AddView(CView* pView)
		{
			ASSERT_VALID(pView);
			ASSERT(pView->m_pDocument == NULL); // must not be already attached
			ASSERT(m_viewList.Find(pView, NULL) == NULL);   // must not be in list

			m_viewList.AddTail(pView);
			ASSERT(pView->m_pDocument == NULL); // must be un-attached
			pView->m_pDocument = this;

			OnChangedViewList();    // must be the last thing done to the document
		}

		BOOL CanCloseFrame(CFrameWnd* pFrameArg)
			// permission to close all views using this frame
			//  (at least one of our views must be in this frame)
		{
			ASSERT_VALID(pFrameArg);
			UNUSED(pFrameArg);   // unused in release builds

			POSITION pos = GetFirstViewPosition();
			while (pos != NULL)
			{
				CView* pView = GetNextView(pos);
				ASSERT_VALID(pView);
				CFrameWnd* pFrame = pView->GetParentFrame();
				// assume frameless views are ok to close
				if (pFrame != NULL)
				{
					// assumes 1 document per frame
					ASSERT_VALID(pFrame);
					if (pFrame->m_nWindow > 0)
						return TRUE;        // more than one frame refering to us
				}
			}

			// otherwise only one frame that we know about
			return SaveModified();
		}


		virtual void DeleteContents() {}

		void DisconnectViews()
		{
			while (!m_viewList.IsEmpty())
			{
				CView* pView = (CView*)m_viewList.RemoveHead();
				ASSERT_VALID(pView);
				ASSERT_KINDOF(CView, pView);
				pView->m_pDocument = NULL;
			}
		}

		BOOL DoFileSave()
		{
			DWORD dwAttrib = GetFileAttributes(m_strPathName);
			if (dwAttrib & FILE_ATTRIBUTE_READONLY)
			{
				// we do not have read-write access or the file does not (now) exist
				if (!DoSave(NULL))
				{
					TRACE(traceAppMsg, 0, "Warning: File save with new name failed.\n");
					return FALSE;
				}
			}
			else
			{
				if (!DoSave(m_strPathName))
				{
					TRACE(traceAppMsg, 0, "Warning: File save failed.\n");
					return FALSE;
				}
			}
			return TRUE;
		}

		virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);

		CDocTemplate* GetDocTemplate() const { ASSERT(this != NULL); return m_pDocTemplate; }

		CFile* GetFile(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError)
		{
			CFile* pFile = new CFile;
			ASSERT(pFile != NULL);
			if (!pFile->Open(lpszFileName, nOpenFlags, pError))
			{
				delete pFile;
				pFile = NULL;
			}
			return pFile;
		}

		HMENU GetDefaultMenu()
		{
			return NULL;    // just use original default
		}

		virtual POSITION GetFirstViewPosition() const { return m_viewList.GetHeadPosition(); }

		virtual HACCEL CDocument::GetDefaultAccelerator() { return NULL; }    // just use original default

		virtual CView* GetNextView(POSITION& rPosition) const
		{
			ASSERT(rPosition != BEFORE_START_POSITION);
			// use CDocument::GetFirstViewPosition instead !
			if (rPosition == NULL)
				return NULL;    // nothing left
			CView* pView = (CView*)m_viewList.GetNext(rPosition);
			return pView;
		}

		const CString& GetPathName() const { ASSERT(this != NULL); return m_strPathName; }

		const CString& GetTitle() const { return m_strTitle; }

		BOOL IsModified() { ASSERT(this != NULL); return m_bModified; }

		virtual void PreCloseFrame(CFrameWnd* /*pFrameArg*/)
		{
			// default does nothing
		}

		void RemoveView(CView* pView)
		{
			ASSERT_VALID(pView);
			ASSERT(pView->m_pDocument == this); // must be attached to us

			m_viewList.RemoveAt(m_viewList.Find(pView));
			pView->m_pDocument = NULL;

			OnChangedViewList();    // must be the last thing done to the document
		}

		virtual void ReleaseFile(CFile* pFile, BOOL bAbort)
		{
			if (bAbort)
				pFile->Abort(); // will not throw an exception
			else
				pFile->Close();
			delete pFile;
		}

		virtual BOOL SaveModified() ; // return TRUE if ok to continue

		virtual void SetModifiedFlag(BOOL bModified = TRUE) { ASSERT(this != NULL); m_bModified = bModified; }

		virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);

		virtual void SetTitle(LPCTSTR lpszTitle)
		{
			m_strTitle = lpszTitle;
			UpdateFrameCounts();        // will cause name change in views
		}

		void UpdateAllViews(CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL)
			// walk through all views
		{
			ASSERT(pSender == NULL || !m_viewList.IsEmpty());
			// must have views if sent by one of them

			POSITION pos = GetFirstViewPosition();
			while (pos != NULL)
			{
				CView* pView = GetNextView(pos);
				ASSERT_VALID(pView);
				if (pView != pSender)
					pView->OnUpdate(pSender, lHint, pHint);
			}
		}

		void UpdateFrameCounts()// assumes 1 doc per frame
		{
			// walk all frames of views (mark and sweep approach)
			POSITION pos = GetFirstViewPosition();
			while (pos != NULL)
			{
				CView* pView = GetNextView(pos);
				ASSERT_VALID(pView);
				ASSERT(::IsWindow(pView->m_hWnd));
				if (pView->IsWindowVisible())   // Do not count invisible windows.
				{
					CFrameWnd* pFrame = pView->GetParentFrame();
					if (pFrame != NULL)
						pFrame->m_nWindow = -1;     // unknown
				}
			}

			// now do it again counting the unique ones
			int nFrames = 0;
			pos = GetFirstViewPosition();
			while (pos != NULL)
			{
				CView* pView = GetNextView(pos);
				ASSERT_VALID(pView);
				ASSERT(::IsWindow(pView->m_hWnd));
				if (pView->IsWindowVisible())   // Do not count invisible windows.
				{
					CFrameWnd* pFrame = pView->GetParentFrame();
					if (pFrame != NULL && pFrame->m_nWindow == -1)
					{
						ASSERT_VALID(pFrame);
						// not yet counted (give it a 1 based number)
						pFrame->m_nWindow = ++nFrames;
					}
				}
			}

			// lastly walk the frames and update titles (assume same order)
			// go through frames updating the appropriate one
			int iFrame = 1;
			pos = GetFirstViewPosition();
			while (pos != NULL)
			{
				CView* pView = GetNextView(pos);
				ASSERT_VALID(pView);
				ASSERT(::IsWindow(pView->m_hWnd));
				if (pView->IsWindowVisible())   // Do not count invisible windows.
				{
					CFrameWnd* pFrame = pView->GetParentFrame();
					if (pFrame != NULL && pFrame->m_nWindow == iFrame)
					{
						ASSERT_VALID(pFrame);
						if (nFrames == 1)
							pFrame->m_nWindow = 0;      // the only one of its kind
						pFrame->OnUpdateFrameTitle(TRUE);
						iFrame++;
					}
				}
			}
			ASSERT(iFrame == nFrames + 1);
		}


};

BEGIN_MESSAGE_MAP(CDocument, CCmdTarget)
	ON_COMMAND(ID_FILE_CLOSE, &CDocument::OnFileClose)
	ON_COMMAND(ID_FILE_SAVE, &CDocument::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CDocument::OnFileSaveAs)
END_MESSAGE_MAP()






_INLINE CDocument* CFrameWnd::GetActiveDocument()
{
	ASSERT_VALID(this);
#ifndef MTL_REMOVE_CFRAMEWND_VIEW
	CView* pView = GetActiveView();
	if (pView != NULL)
		return pView->GetDocument();
#endif	// MTL_REMOVE_CFRAMEWND_VIEW
	return NULL;
}

_INLINE HACCEL CFrameWnd::GetDefaultAccelerator()
{
	// use document specific accelerator table over m_hAccelTable
	HACCEL hAccelTable = m_hAccelTable;
	HACCEL hAccel;
	CDocument* pDoc = GetActiveDocument();
	if (pDoc != NULL && (hAccel = pDoc->GetDefaultAccelerator()) != NULL)
		hAccelTable = hAccel;

	return hAccelTable;
}

_INLINE void CFrameWnd::OnUpdateFrameMenu(HMENU hMenuAlt)
{
	if (hMenuAlt == NULL)
	{
		// attempt to get default menu from document
		CDocument* pDoc = GetActiveDocument();
		if (pDoc != NULL)
			hMenuAlt = pDoc->GetDefaultMenu();
		// use default menu stored in frame if none from document
		if (hMenuAlt == NULL)
			hMenuAlt = m_hMenuDefault;
	}
	// finally, set the menu
	if (m_dwMenuBarState == AFX_MBS_VISIBLE)
	{
		::SetMenu(m_hWnd, hMenuAlt);
	}
	else if (m_dwMenuBarState == AFX_MBS_HIDDEN)
	{
		m_hMenu = hMenuAlt;
	}
}

_INLINE void CFrameWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave it alone!

	CDocument* pDocument = GetActiveDocument();
	if (bAddToTitle && pDocument != NULL)
		UpdateFrameTitleForDocument(pDocument->GetTitle());
	else
		UpdateFrameTitleForDocument(NULL);
}


_INLINE CView::~CView()
{
	if (m_pDocument != NULL)
		m_pDocument->RemoveView(this);
}

/////////////////////////////////////////////////////////////////////////////
// Command routing

_INLINE BOOL CView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// first pump through pane
	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// then pump through document
	if (m_pDocument != NULL)
	{
		return m_pDocument->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}

	return FALSE;
}


_INLINE int CView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CWnd::OnCreate(lpcs) == -1)
		return -1;

	// if ok, wire in the current document
	ASSERT(m_pDocument == NULL);
	CCreateContext* pContext = (CCreateContext*)lpcs->lpCreateParams;

	// A view should be created in a given context!
	if (pContext != NULL && pContext->m_pCurrentDoc != NULL)
	{
		pContext->m_pCurrentDoc->AddView(this);
		ASSERT(m_pDocument != NULL);
	}
	else
	{
		TRACE(traceAppMsg, 0, "Warning: Creating a pane with no CDocument.\n");
	}

	return 0;   // ok
}




_INLINE BOOL CEditView::OnEditChange()
{
	ASSERT_VALID(this);
	GetDocument()->SetModifiedFlag();
	ASSERT_VALID(this);

	return FALSE;   // continue routing
}




class ATL_NO_VTABLE CDocTemplate : public CCmdTarget
{
	DECLARE_DYNAMIC(CDocTemplate)

	public:
		CDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
		{
			m_nIDResource = nIDResource;
			m_nIDContainerResource = NULL;
			m_nIDPreviewResource = NULL;

			m_pDocClass = pDocClass;
			m_pFrameClass = pFrameClass;
			m_pViewClass = pViewClass;


			m_pPreviewFrameClass = NULL;
			m_pPreviewViewClass = NULL;

			m_bAutoDelete = TRUE;   // usually allocated on the heap
			LoadTemplate();
		}

	public:
		enum DocStringIndex
		{
			windowTitle,        // default window title
			docName,            // user visible name for default document
			fileNewName,        // user visible name for FileNew
			// for file based documents:
			filterName,         // user visible name for FileOpen
			filterExt,          // user visible extension for FileOpen
			// for file based documents with Shell open support:
			regFileTypeId,      // REGEDIT visible registered file type identifier
			regFileTypeName,    // Shell visible registered file type name
		};

		enum Confidence
		{
			noAttempt,
			maybeAttemptForeign,
			maybeAttemptNative,
			yesAttemptForeign,
			yesAttemptNative,
			yesAlreadyOpen
		};

		BOOL m_bAutoDelete;     // enables 'delete this' after thread termination

		UINT m_nIDResource;						// IDR_ for frame/menu/accel as well
		UINT m_nIDContainerResource;			// IDR_ for container frame/menu/accel
		UINT m_nIDPreviewResource;				// IDR_ for preview frame. Do not load menu/accel

		CRuntimeClass* m_pDocClass;				// class for creating new documents
		CRuntimeClass* m_pFrameClass;			// class for creating new frames
		CRuntimeClass* m_pViewClass;			// class for creating new views
		CRuntimeClass* m_pPreviewFrameClass;	// class for creating in-place preview frame
		CRuntimeClass* m_pPreviewViewClass;		// class for creating in-place preview view

		CString m_strDocStrings;    // '\n' separated names
		// The document names sub-strings are represented as _one_ string:
		// windowTitle\ndocName\n ... (see DocStringIndex enum)

	public:
#if 0
		virtual void OnIdle()
		{
			POSITION pos = GetFirstDocPosition();
			while (pos != NULL)
			{
				CDocument* pDoc = GetNextDoc(pos);
				ASSERT_VALID(pDoc);
				ASSERT_KINDOF(CDocument, pDoc);
				pDoc->OnIdle();
			}
		}
#endif

		// iterating over open documents
		virtual void AddDocument(CDocument* pDoc)      // must override
		{
			ASSERT_VALID(pDoc);
			ASSERT(pDoc->m_pDocTemplate == NULL);   // no template attached yet
			pDoc->m_pDocTemplate = this;
		}

		void CloseAllDocuments(BOOL)
		{
			POSITION pos = GetFirstDocPosition();
			while (pos != NULL)
			{
				CDocument* pDoc = GetNextDoc(pos);
				pDoc->OnCloseDocument();
			}
		}

		virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
		{
			BOOL bReturn = CCmdTarget::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

			return bReturn;
		}

		virtual CDocument* CreateNewDocument()
		{
			// default implementation constructs one from CRuntimeClass
			if (m_pDocClass == NULL)
			{
				TRACE(traceAppMsg, 0, "Error: you must override CDocTemplate::CreateNewDocument.\n");
				ASSERT(FALSE);
				return NULL;
			}
			CDocument* pDocument = (CDocument*)m_pDocClass->CreateObject();
			if (pDocument == NULL)
			{
				TRACE(traceAppMsg, 0, "Warning: Dynamic create of document type %hs failed.\n", m_pDocClass->m_lpszClassName);
				return NULL;
			}
			AddDocument(pDocument);
			return pDocument;
		}

		virtual CFrameWnd* CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther)
		{
			// create a frame wired to the specified document

			ASSERT(m_nIDResource != 0); // must have a resource ID to load from
			CCreateContext context;
			context.m_pCurrentFrame = pOther;
			context.m_pCurrentDoc = pDoc;
			context.m_pNewViewClass = m_pViewClass;
			context.m_pNewDocTemplate = this;

			if (m_pFrameClass == NULL)
			{
				TRACE(traceAppMsg, 0, "Error: you must override CDocTemplate::CreateNewFrame.\n");
				ASSERT(FALSE);
				return NULL;
			}
			CFrameWnd* pFrame = (CFrameWnd*)m_pFrameClass->CreateObject();
			if (pFrame == NULL)
			{
				TRACE(traceAppMsg, 0, "Warning: Dynamic create of frame %hs failed.\n", m_pFrameClass->m_lpszClassName);
				return NULL;
			}
			ASSERT_KINDOF(CFrameWnd, pFrame);

			if (context.m_pNewViewClass == NULL)
				TRACE(traceAppMsg, 0, "Warning: creating frame with no default view.\n");

			// create new from resource
			if (!pFrame->LoadFrame(m_nIDResource, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, &context))   // default frame styles
			{
				TRACE(traceAppMsg, 0, "Warning: CDocTemplate couldn't create a frame.\n");
				// frame will be deleted in PostNcDestroy cleanup
				return NULL;
			}

			// it worked !
			return pFrame;
		}

		virtual POSITION GetFirstDocPosition() const = 0;
		virtual CDocument* GetNextDoc(POSITION& rPos) const = 0;

		virtual BOOL GetDocString(CString& rString, enum DocStringIndex i) const
		{
			return AfxExtractSubString(rString, m_strDocStrings, (int)i);
		}

		virtual void InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc, BOOL bMakeVisible = TRUE)
		{
			// just delagate to implementation in CFrameWnd
			pFrame->InitialUpdateFrame(pDoc, bMakeVisible);
		}

		virtual void LoadTemplate()
		{
			if (m_strDocStrings.IsEmpty() && !m_strDocStrings.LoadString(m_nIDResource))
			{
				TRACE(traceAppMsg, 0, "Warning: no document names in string for template #%d.\n", m_nIDResource);
			}
		}

		virtual Confidence MatchDocType(LPCTSTR lpszPathName, CDocument*& rpDocMatch)
		{
			ASSERT(lpszPathName != NULL);
			rpDocMatch = NULL;

			// go through all documents
			POSITION pos = GetFirstDocPosition();
			while (pos != NULL)
			{
				CDocument* pDoc = GetNextDoc(pos);
				if (AfxComparePath(pDoc->GetPathName(), lpszPathName))
				{
					// already open
					rpDocMatch = pDoc;
					return yesAlreadyOpen;
				}
			}

			// see if it matches our default suffix
			CString strFilterExt;
			if (GetDocString(strFilterExt, CDocTemplate::filterExt) && !strFilterExt.IsEmpty())
			{
				// see if extension matches
				ASSERT(strFilterExt[0] == '.');
				LPCTSTR lpszDot = ::PathFindExtension(lpszPathName);
				if (lpszDot != NULL)
				{
					if (::AfxComparePath(lpszDot, static_cast<const TCHAR *>(strFilterExt)))
					{
						return yesAttemptNative; // extension matches, looks like ours
					}
				}
			}

			// otherwise we will guess it may work
			return yesAttemptForeign;
		}


		virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE) = 0;
		virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible) = 0;

		virtual void RemoveDocument(CDocument* pDoc)   // must override
		{
			ASSERT_VALID(pDoc);
			ASSERT(pDoc->m_pDocTemplate == this);   // must be attached to us
			pDoc->m_pDocTemplate = NULL;
		}

		BOOL SaveAllModified()
		{
			POSITION pos = GetFirstDocPosition();
			while (pos != NULL)
			{
				CDocument* pDoc = GetNextDoc(pos);
				if (!pDoc->SaveModified())
					return FALSE;
			}
			return TRUE;
		}

};

_INLINE CDocument::~CDocument()
{
	// do not call DeleteContents here !

	// there should be no views left!
	DisconnectViews();
	ASSERT(m_viewList.IsEmpty());

	if (m_pDocTemplate != NULL)
		m_pDocTemplate->RemoveDocument(this);
	ASSERT(m_pDocTemplate == NULL);     // must be detached
}

/////////////////////////////////////////////////////////////////////////////
// command routing

_INLINE BOOL CDocument::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (CCmdTarget::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise check template
	if (m_pDocTemplate != NULL &&
		m_pDocTemplate->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return FALSE;
}





// SDI support (1 document only)
class CSingleDocTemplate : public CDocTemplate
{
		DECLARE_DYNAMIC(CSingleDocTemplate)

		// Constructors
	public:
		CSingleDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
			: CDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
		{
			m_pOnlyDoc = NULL;
		}

		virtual ~CSingleDocTemplate()
		{

		}

	public:  // standard implementation
		CDocument* m_pOnlyDoc;

	// Implementation
	public:
		virtual void AddDocument(CDocument* pDoc)
		{
			ASSERT(m_pOnlyDoc == NULL);     // one at a time please
			ASSERT_VALID(pDoc);

			CDocTemplate::AddDocument(pDoc);
			m_pOnlyDoc = pDoc;
		}

		virtual POSITION GetFirstDocPosition() const { return (m_pOnlyDoc == NULL) ? NULL : BEFORE_START_POSITION; }

		virtual CDocument* GetNextDoc(POSITION& rPos) const
		{
			CDocument* pDoc = NULL;
			if (rPos == BEFORE_START_POSITION)
			{
				// first time through, return a real document
				ASSERT(m_pOnlyDoc != NULL);
				pDoc = m_pOnlyDoc;
			}
			rPos = NULL;        // no more
			return pDoc;
		}

		virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE) { return OpenDocumentFile(lpszPathName, TRUE, bMakeVisible); }

		virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible) ;

		virtual void RemoveDocument(CDocument* pDoc)
		{
			ASSERT(m_pOnlyDoc == pDoc);     // must be this one
			ASSERT_VALID(pDoc);

			CDocTemplate::RemoveDocument(pDoc);
			m_pOnlyDoc = NULL;
		}

		virtual void SetDefaultTitle(CDocument* pDocument)
		{
			CString strDocName;
			if (!GetDocString(strDocName, CDocTemplate::docName) || strDocName.IsEmpty())
			{
				// use generic 'untitled'
				ENSURE(strDocName.LoadString(AFX_IDS_UNTITLED));
			}
			pDocument->SetTitle(strDocName);
		}

};



_INLINE void _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn, CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) && !strFilterExt.IsEmpty() &&
		pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) && !strFilterName.IsEmpty())
	{
		if (pstrDefaultExt != NULL)
			pstrDefaultExt->Empty();

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please

		int iStart = 0;
		do
		{
			CString strExtension = Tokenize(strFilterExt, _T(";"), iStart);

			if (iStart != -1)
			{
				// a file based document template - add to filter list

				// If you hit the following ASSERT, your document template 
				// string is formatted incorrectly.  The section of your 
				// document template string that specifies the allowable file
				// extensions should be formatted as follows:
				//    .<ext1>;.<ext2>;.<ext3>
				// Extensions may contain wildcards (e.g. '?', '*'), but must
				// begin with a '.' and be separated from one another by a ';'.
				// Example:
				//    .jpg;.jpeg
				ASSERT(strExtension[0] == '.');
				if ((pstrDefaultExt != NULL) && pstrDefaultExt->IsEmpty())
				{
					// set the default extension
					*pstrDefaultExt = strExtension.Mid(1);  // skip the '.'
					ofn.lpstrDefExt = ( LPTSTR )((LPCTSTR)(*pstrDefaultExt));
					ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
				}

				filter += (TCHAR)'*';
				filter += strExtension;
				filter += (TCHAR)';';  // Always append a ';'.  The last ';' will get replaced with a '\0' later.
			}
		} while (iStart != -1);

		filter.SetAt(filter.GetLength() - 1, '\0');;  // Replace the last ';' with a '\0'
		ofn.nMaxCustFilter++;
	}
}


// Get the best document template for the named file

class CNewTypeDlg : public CDialog
{
	public:
		CNewTypeDlg(CPtrList* pList) : CDialog(CNewTypeDlg::IDD)
		{
			m_pList = pList;
			m_pSelectedTemplate = NULL;
		}

	public:
		DECLARE_MESSAGE_MAP()

		enum { IDD = AFX_IDD_NEWTYPEDLG };

		CPtrList*   m_pList;        // actually a list of doc templates

		CDocTemplate*   m_pSelectedTemplate;

	public:
		BOOL OnInitDialog()
		{
			CListBox* pListBox = (CListBox*)GetDlgItem(AFX_IDC_LISTBOX);
			ENSURE(pListBox != NULL);

			// fill with document templates in list
			pListBox->ResetContent();

			POSITION pos = m_pList->GetHeadPosition();
			// add all the CDocTemplates in the list by name
			while (pos != NULL)
			{
				CDocTemplate* pTemplate = (CDocTemplate*)m_pList->GetNext(pos);
				ASSERT_KINDOF(CDocTemplate, pTemplate);

				CString strTypeName;
				if (pTemplate->GetDocString(strTypeName, CDocTemplate::fileNewName) &&
					!strTypeName.IsEmpty())
				{
					// add it to the listbox
					int nIndex = pListBox->AddString(strTypeName);
					if (nIndex == -1)
					{
						EndDialog(-1);
						return FALSE;
					}
					pListBox->SetItemDataPtr(nIndex, pTemplate);
				}
			}

			int nTemplates = pListBox->GetCount();
			if (nTemplates == 0)
			{
				TRACE(traceAppMsg, 0, "Error: no document templates to select from!\n");
				EndDialog(-1); // abort
			}
			else if (nTemplates == 1)
			{
				// get the first/only item
				m_pSelectedTemplate = (CDocTemplate*)pListBox->GetItemDataPtr(0);
				ASSERT_VALID(m_pSelectedTemplate);
				ASSERT_KINDOF(CDocTemplate, m_pSelectedTemplate);
				EndDialog(IDOK);    // done
			}
			else
			{
				// set selection to the first one (NOT SORTED)
				pListBox->SetCurSel(0);
			}

			return CDialog::OnInitDialog();
		}

		void OnOK()
		{
			CListBox* pListBox = (CListBox*)GetDlgItem(AFX_IDC_LISTBOX);
			ENSURE(pListBox != NULL);
			// if listbox has selection, set the selected template
			int nIndex;
			if ((nIndex = pListBox->GetCurSel()) == -1)
			{
				// no selection
				m_pSelectedTemplate = NULL;
			}
			else
			{
				m_pSelectedTemplate = (CDocTemplate*)pListBox->GetItemDataPtr(nIndex);
				ASSERT_VALID(m_pSelectedTemplate);
				ASSERT_KINDOF(CDocTemplate, m_pSelectedTemplate);
			}
			CDialog::OnOK();
		}
};

BEGIN_MESSAGE_MAP(CNewTypeDlg, CDialog)
	ON_LBN_DBLCLK(AFX_IDC_LISTBOX, &CNewTypeDlg::OnOK)
END_MESSAGE_MAP()


class CDocManager
{
	public:
		CDocManager::CDocManager() {}

	public:
		CPtrList m_templateList;

	public:
		virtual void OnFileNew()
		{
			if (m_templateList.IsEmpty())
				return ;

			CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetHead();
#ifndef MTL_REMOVE_CDOCMANAGER_MULTITEMPLATE
			if (m_templateList.GetCount() > 1)
			{
				// more than one document template to choose from
				// bring up dialog prompting user
				CNewTypeDlg dlg(&m_templateList);
				INT_PTR nID = dlg.DoModal();
				if (nID != IDOK)
					return;     // none - cancel operation
				pTemplate = dlg.m_pSelectedTemplate;
			}
#endif // MTL_REMOVE_CDOCMANAGER_MULTITEMPLATE
			pTemplate->OpenDocumentFile(NULL);
		}

		virtual void OnFileOpen() ;

		void AddDocTemplate(CDocTemplate* pTemplate)
		{
			ASSERT(pTemplate != NULL) ;

			ASSERT_VALID(pTemplate);
			pTemplate->LoadTemplate();
			m_templateList.AddTail(pTemplate);
		}

		void CloseAllDocuments(BOOL bEndSession)
		{
			POSITION pos = m_templateList.GetHeadPosition();
			while (pos != NULL)
			{
				CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
				ASSERT_KINDOF(CDocTemplate, pTemplate);
				pTemplate->CloseAllDocuments(bEndSession);
			}
		}

		BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
		{
			CFileDialog dlgFile(bOpenFileDialog, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);

			CString title;

			dlgFile.m_ofn.Flags |= lFlags;

			CString strFilter;
			CString strDefault;
			if (pTemplate != NULL)
			{
				_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
			}
			else
			{
				// do for all doc template
				POSITION pos = m_templateList.GetHeadPosition();
				BOOL bFirst = TRUE;
				while (pos != NULL)
				{
					pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
					_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, bFirst ? &strDefault : NULL);
					bFirst = FALSE;
				}
			}

			// append the "*.*" all files filter
			CString allFilter;
			VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
			strFilter += allFilter;
			strFilter += (TCHAR)'\0';   // next string please
			strFilter += _T("*.*");
			strFilter += (TCHAR)'\0';   // last string
			dlgFile.m_ofn.nMaxCustFilter++;

			dlgFile.m_ofn.lpstrFilter = strFilter;
			dlgFile.m_ofn.lpstrTitle = title;
			dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

			INT_PTR nResult = dlgFile.DoModal();
			fileName.ReleaseBuffer();
			return nResult == IDOK;
		}

		virtual POSITION GetFirstDocTemplatePosition() const { return m_templateList.GetHeadPosition(); }
		virtual CDocTemplate* GetNextDocTemplate(POSITION& pos) const { return (CDocTemplate*)m_templateList.GetNext(pos); }

		CDocument* OpenDocumentFile(LPCTSTR lpszFileName, BOOL bAddToMRU = TRUE) ;

		virtual void RegisterShellFileTypes(BOOL bCompat) {}
		void UnregisterShellFileTypes() {}

		BOOL SaveAllModified()
		{
			POSITION pos = m_templateList.GetHeadPosition();
			while (pos != NULL)
			{
				CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
				ASSERT_KINDOF(CDocTemplate, pTemplate);
				if (!pTemplate->SaveAllModified())
					return FALSE;
			}
			return TRUE;
		}

};


