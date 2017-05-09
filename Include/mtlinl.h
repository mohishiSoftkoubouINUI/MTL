


class CObject;
class CCmdTarget;
class CWMHnd;
class CWnd;

#if 1
#define CAnimateCtrl  CAnimateCtrlMTL
#define CButton  CButtonMTL
#define CComboBox  CComboBoxMTL
#define CComboBoxEx  CComboBoxExMTL
#define CDateTimePickerCtrl  CDateTimePickerCtrlMTL
#define CDragListBox  CDragListBoxMTL
#define CEdit  CEditMTL
#define CFlatScrollBar  CFlatScrollBarMTL
#define CHeaderCtrl  CHeaderCtrlMTL
#define CHotKeyCtrl  CHotKeyCtrlMTL
#define CIPAddressCtrl  CIPAddressCtrlMTL
#define CLinkCtrl  CLinkCtrlMTL
#define CListBox  CListBoxMTL
#define CListCtrl  CListCtrlMTL
#define CMonthCalendarCtrl  CMonthCalendarCtrlMTL
#define CPagerCtrl  CPagerCtrlMTL
#define CProgressCtrl  CProgressCtrlMTL
#define CProgressBarCtrl  CProgressBarCtrlMTL
#define CReBarCtrl  CReBarCtrlMTL
#define CRichEditCtrl  CRichEditCtrlMTL
#define CScrollBar  CScrollBarMTL
#define CSliderCtrl  CSliderCtrlMTL
#define CStatic  CStaticMTL
#define CStatusBarCtrl  CStatusBarCtrlMTL
#define CTabCtrl  CTabCtrlMTL
#define CToolBarCtrl  CToolBarCtrlMTL
#define CToolTipCtrl  CToolTipCtrlMTL
#define CTrackBarCtrl  CTrackBarCtrlMTL
#define CTreeItem  CTreeItemMTL
#define CTreeCtrl  CTreeCtrlMTL
#define CTreeCtrlEx  CTreeCtrlExMTL
#define CUpDownCtrl  CUpDownCtrlMTL
#define CSpinButtonCtrl  CSpinButtonCtrlMTL

#define CImageList CImageListMTL

class CAnimateCtrl;
class CButton;
class CComboBox;
class CComboBoxEx;
class CDateTimePickerCtrl;
class CDragListBox;
class CEdit;
class CFlatScrollBar;
class CHeaderCtrl;
class CHotKeyCtrl;
class CIPAddressCtrl;
class CLinkCtrl;
class CListBox;
class CListCtrl;
class CMonthCalendarCtrl;
class CPagerCtrl;
class CProgressBarCtrl;
class CReBarCtrl;
class CRichEditCtrl;
class CScrollBar;
class CSLiderCtrl;
class CStatic;
class CStatusBarCtrl;
class CTabCtrl;
class CToolBarCtrl;
class CToolTipCtrl;
class CTrackBarCtrl;
class CTreeItem;
class CTreeCtrl;
class CTreeCtrlEx;
class CUpDownCtrl;
class CSpinButtonCtrl;

class CImageList ;
#endif



class CFrameWnd ;
class CSplitterWnd ;

class CDialog;
class CPreviewDialog;

class CView;
class CScrollView;
class CPreviewView;
struct CPrintPreviewState;

class CDocument;
class CDocTemplate;

class CCmdUI;

class CWinApp;

class CAfx ;

struct CRuntimeClass;

class CException ;
class CFleException;

class CArchive ;
class CDumpContext ;





#define DECLARE_DYNAMIC(class_name) \
public: \
	static const CRuntimeClass class##class_name; \
	virtual CRuntimeClass* GetRuntimeClass() const ;

// not serializable, but dynamically constructable
#define DECLARE_DYNCREATE(class_name) \
	DECLARE_DYNAMIC(class_name) \
	static CObject* CreateObject();


#define DECLARE_SERIAL(class_name) \
	DECLARE_DYNCREATE(class_name) \
	friend CArchive& operator>>(CArchive& ar, class_name* &pOb);


#define IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew, class_init) \
	AFX_COMDAT const CRuntimeClass class_name::class##class_name = { \
		#class_name, sizeof(class class_name), wSchema, pfnNew, \
			RUNTIME_CLASS(base_class_name), NULL, class_init }; \
	CRuntimeClass* class_name::GetRuntimeClass() const { return RUNTIME_CLASS(class_name); }

#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
	IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, NULL, NULL)

#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
	CObject* class_name::CreateObject() { return new class_name; } \
	IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, class_name::CreateObject, NULL)


#define IMPLEMENT_SERIAL(class_name, base_class_name, wSchema) \
	CObject* class_name::CreateObject() { return new class_name; } \
	extern AFX_CLASSINIT _init_##class_name; \
	IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, class_name::CreateObject, &_init_##class_name) \
	AFX_CLASSINIT _init_##class_name(RUNTIME_CLASS(class_name)); \
	CArchive& operator>>(CArchive& ar, class_name* &pOb) \
		{ pOb = (class_name*) ar.ReadObject(RUNTIME_CLASS(class_name)); return ar; }




class CThreadState;
class CModuleThreadState;
class CModuleState;

#define _AFX_THREAD_STATE CThreadState
#define AFX_MODULE_THREAD_STATE CModuleThreadState
#define AFX_MODULE_STATE CModuleState


#define AFX_MANAGE_STATE(p)


CWinApp* AfxGetApp();
LPCTSTR AfxGetAppName();
MSG* AfxGetCurrentMessage();
HINSTANCE AfxGetInstanceHandle();
CWnd* AfxGetMainWnd();
AFX_MODULE_STATE* AfxGetModuleState();
AFX_MODULE_THREAD_STATE* AfxGetModuleThreadState();
HINSTANCE AfxGetResourceHandle();
_AFX_THREAD_STATE* AfxGetThreadState();
BOOL AfxPumpMessage();

void AfxClassInit(CRuntimeClass* pNewClass) ;


BOOL AfxHelpEnabled();
int AfxMessageBox(LPCTSTR lpszText, UINT nType = 0, UINT nIDHelp = 0);
int AfxMessageBox(UINT nIDPrompt = 0, UINT nType = 0, UINT nIDHelp = 0);


#define _AfxSetDlgCtrlID(hWnd, nID)     ::SetWindowLong(hWnd, GWL_ID, nID)
#define _AfxGetDlgCtrlID(hWnd)          ((UINT)::GetDlgCtrlID(hWnd))


// Low level sanity checks for memory blocks
_INLINE BOOL AfxIsValidAddress(const void* lp, UINT_PTR nBytes, BOOL bReadWrite = TRUE) { return TRUE ;}
_INLINE BOOL AfxIsValidString(LPCWSTR lpsz, int nLength = -1) { return TRUE; }
_INLINE BOOL AfxIsValidString(LPCSTR lpsz, int nLength = -1) { return TRUE; }

// Sanity checks for ATOMs
_INLINE BOOL AfxIsValidAtom(ATOM nAtom)  { return TRUE; }
_INLINE BOOL AfxIsValidAtom(LPCTSTR psz)  { return TRUE; }



#if defined(_DEBUG)
class CDumpContext
{
	public:

	public:

};
#endif



class CObject
{
	DECLARE_DYNAMIC(CObject)

	public:

		// Object model (types, destruction, allocation)

		// Disable the copy constructor and assignment by default so you will get
		//   compiler errors instead of unexpected behaviour if you pass objects
		//   by value or assign objects.

		// Implementation
	public:
		BOOL IsKindOf(const CRuntimeClass* pClass) const ;

		virtual void Serialize(CArchive& ar) {}

#if defined(_DEBUG)
		// Diagnostic Support
		virtual void AssertValid() const {}
		virtual void Dump(CDumpContext& dc) const {}
#endif

};




class CException //: public CObject
{
	//DECLARE_DYNAMIC(CException)

	public:
		CException() { m_bAutoDelete = TRUE ; }
		CException(BOOL bAutoDelete) { m_bAutoDelete = bAutoDelete ; }

	public:
		BOOL m_bAutoDelete;

	public:
		void CException::Delete()
		{
			// delete exception if it is auto-deleting
			if (m_bAutoDelete)
			{
				delete this;
			}
		}

};

class CSimpleException : public CException
{
	//DECLARE_DYNAMIC(CSimpleException)

	public:
		CSimpleException() { m_nResourceID = 0 ;}

		CSimpleException(BOOL bAutoDelete, UINT nResourceID)
		{
			m_bAutoDelete = bAutoDelete;
			m_nResourceID = nResourceID;
		}

	public:
		UINT m_nResourceID;

	public:

};

class CArchiveException : public CSimpleException
{
	//DECLARE_DYNAMIC(CArchiveException)

	public:
		CArchiveException(BOOL bAutoDelete, UINT nResourceID) : CSimpleException(bAutoDelete, nResourceID) {}
		CArchiveException(int cause = CArchiveException::none, LPCTSTR lpszArchiveName = NULL)
		{
#ifndef MTL_REMOVE_CEXCEPTION_DETAIL
			m_cause = cause ;
			m_strFileName = lpszArchiveName ;
#endif
		}
	public:
		enum {
			none,
			genericException,
			readOnly,
			endOfFile,
			writeOnly,
			badIndex,
			badClass,
			badSchema,
			bufferFull
		};

#ifndef MTL_REMOVE_CEXCEPTION_DETAIL
		int m_cause;
		CString m_strFileName;
#endif
	public:

};

class CFileException : public CSimpleException
{
	//DECLARE_DYNAMIC(CFileException)

	public:
		CFileException(BOOL bAutoDelete, UINT nResourceID) : CSimpleException(bAutoDelete, nResourceID) {}
		CFileException(int cause = CFileException::none, LONG lOsError = -1, LPCTSTR lpszArchiveName = NULL)
		{
#ifndef MTL_REMOVE_CEXCEPTION_DETAIL
			m_cause = cause;
			m_lOsError = lOsError ;
			m_strFileName = lpszArchiveName ;
#endif //  MTL_REMOVE_CEXCEPTION_DETAIL
		}

	public:
		enum {
			none,
			genericException,
			fileNotFound,
			badPath,
			tooManyOpenFiles,
			accessDenied,
			invalidFile,
			removeCurrentDir,
			directoryFull,
			badSeek,
			hardIO,
			sharingViolation,
			lockViolation,
			diskFull,
			endOfFile
		};

//#ifndef MTL_REMOVE_CEXCEPTION_DETAIL
		int m_cause;
		LONG    m_lOsError;
		CString m_strFileName;
//#endif //  MTL_REMOVE_CEXCEPTION_DETAIL
	public:
		static int OsErrorToException(LONG lOsErr)
		{
#ifndef MTL_REMOVE_CEXCEPTION_DETAIL
			// NT Error codes
			switch ((UINT)lOsErr)
			{
				case NO_ERROR:
					return CFileException::none;
				case ERROR_FILE_NOT_FOUND:
					return CFileException::fileNotFound;
				case ERROR_PATH_NOT_FOUND:
					return CFileException::badPath;
				case ERROR_TOO_MANY_OPEN_FILES:
					return CFileException::tooManyOpenFiles;
				case ERROR_ACCESS_DENIED:
					return CFileException::accessDenied;
				case ERROR_INVALID_HANDLE:
					return CFileException::fileNotFound;
				case ERROR_BAD_FORMAT:
					return CFileException::invalidFile;
				case ERROR_INVALID_ACCESS:
					return CFileException::accessDenied;
				case ERROR_INVALID_DRIVE:
					return CFileException::badPath;
				case ERROR_CURRENT_DIRECTORY:
					return CFileException::removeCurrentDir;
				case ERROR_NOT_SAME_DEVICE:
					return CFileException::badPath;
				case ERROR_NO_MORE_FILES:
					return CFileException::fileNotFound;
				case ERROR_WRITE_PROTECT:
					return CFileException::accessDenied;
				case ERROR_BAD_UNIT:
					return CFileException::hardIO;
				case ERROR_NOT_READY:
					return CFileException::hardIO;
				case ERROR_BAD_COMMAND:
					return CFileException::hardIO;
				case ERROR_CRC:
					return CFileException::hardIO;
				case ERROR_BAD_LENGTH:
					return CFileException::badSeek;
				case ERROR_SEEK:
					return CFileException::badSeek;
				case ERROR_NOT_DOS_DISK:
					return CFileException::invalidFile;
				case ERROR_SECTOR_NOT_FOUND:
					return CFileException::badSeek;
				case ERROR_WRITE_FAULT:
					return CFileException::accessDenied;
				case ERROR_READ_FAULT:
					return CFileException::badSeek;
				case ERROR_SHARING_VIOLATION:
					return CFileException::sharingViolation;
				case ERROR_LOCK_VIOLATION:
					return CFileException::lockViolation;
				case ERROR_WRONG_DISK:
					return CFileException::badPath;
				case ERROR_SHARING_BUFFER_EXCEEDED:
					return CFileException::tooManyOpenFiles;
				case ERROR_HANDLE_EOF:
					return CFileException::endOfFile;
				case ERROR_HANDLE_DISK_FULL:
					return CFileException::diskFull;
				case ERROR_DUP_NAME:
					return CFileException::badPath;
				case ERROR_BAD_NETPATH:
					return CFileException::badPath;
				case ERROR_NETWORK_BUSY:
					return CFileException::accessDenied;
				case ERROR_DEV_NOT_EXIST:
					return CFileException::badPath;
				case ERROR_ADAP_HDW_ERR:
					return CFileException::hardIO;
				case ERROR_BAD_NET_RESP:
					return CFileException::accessDenied;
				case ERROR_UNEXP_NET_ERR:
					return CFileException::hardIO;
				case ERROR_BAD_REM_ADAP:
					return CFileException::invalidFile;
				case ERROR_NO_SPOOL_SPACE:
					return CFileException::directoryFull;
				case ERROR_NETNAME_DELETED:
					return CFileException::accessDenied;
				case ERROR_NETWORK_ACCESS_DENIED:
					return CFileException::accessDenied;
				case ERROR_BAD_DEV_TYPE:
					return CFileException::invalidFile;
				case ERROR_BAD_NET_NAME:
					return CFileException::badPath;
				case ERROR_TOO_MANY_NAMES:
					return CFileException::tooManyOpenFiles;
				case ERROR_SHARING_PAUSED:
					return CFileException::badPath;
				case ERROR_REQ_NOT_ACCEP:
					return CFileException::accessDenied;
				case ERROR_FILE_EXISTS:
					return CFileException::accessDenied;
				case ERROR_CANNOT_MAKE:
					return CFileException::accessDenied;
				case ERROR_ALREADY_ASSIGNED:
					return CFileException::badPath;
				case ERROR_INVALID_PASSWORD:
					return CFileException::accessDenied;
				case ERROR_NET_WRITE_FAULT:
					return CFileException::hardIO;
				case ERROR_DISK_CHANGE:
					return CFileException::fileNotFound;
				case ERROR_DRIVE_LOCKED:
					return CFileException::lockViolation;
				case ERROR_BUFFER_OVERFLOW:
					return CFileException::badPath;
				case ERROR_DISK_FULL:
					return CFileException::diskFull;
				case ERROR_NO_MORE_SEARCH_HANDLES:
					return CFileException::tooManyOpenFiles;
				case ERROR_INVALID_TARGET_HANDLE:
					return CFileException::invalidFile;
				case ERROR_INVALID_CATEGORY:
					return CFileException::hardIO;
				case ERROR_INVALID_NAME:
					return CFileException::badPath;
				case ERROR_INVALID_LEVEL:
					return CFileException::badPath;
				case ERROR_NO_VOLUME_LABEL:
					return CFileException::badPath;
				case ERROR_NEGATIVE_SEEK:
					return CFileException::badSeek;
				case ERROR_SEEK_ON_DEVICE:
					return CFileException::badSeek;
				case ERROR_DIR_NOT_ROOT:
					return CFileException::badPath;
				case ERROR_DIR_NOT_EMPTY:
					return CFileException::removeCurrentDir;
				case ERROR_LABEL_TOO_LONG:
					return CFileException::badPath;
				case ERROR_BAD_PATHNAME:
					return CFileException::badPath;
				case ERROR_LOCK_FAILED:
					return CFileException::lockViolation;
				case ERROR_BUSY:
					return CFileException::accessDenied;
				case ERROR_INVALID_ORDINAL:
					return CFileException::invalidFile;
				case ERROR_ALREADY_EXISTS:
					return CFileException::accessDenied;
				case ERROR_INVALID_EXE_SIGNATURE:
					return CFileException::invalidFile;
				case ERROR_BAD_EXE_FORMAT:
					return CFileException::invalidFile;
				case ERROR_FILENAME_EXCED_RANGE:
					return CFileException::badPath;
				case ERROR_META_EXPANSION_TOO_LONG:
					return CFileException::badPath;
				case ERROR_DIRECTORY:
					return CFileException::badPath;
				case ERROR_OPERATION_ABORTED:
					return CFileException::hardIO;
				case ERROR_IO_INCOMPLETE:
					return CFileException::hardIO;
				case ERROR_IO_PENDING:
					return CFileException::hardIO;
				case ERROR_SWAPERROR:
					return CFileException::accessDenied;
				default:
					return CFileException::genericException;
			}
#endif //  MTL_REMOVE_CEXCEPTION_DETAIL
			return CFileException::genericException;
		}

		static void ThrowOsError(LONG lOsError, LPCTSTR lpszFileName = NULL)
		{
			if (lOsError != 0)
			{
				//AfxThrowFileException(OsErrorToException(lOsError), lOsError, lpszFileName);
				THROW(new CFileException(OsErrorToException(lOsError), lOsError, lpszFileName));
			}
		}


};

class CInvalidArgException : public CSimpleException
{
	//DECLARE_DYNAMIC(CInvalidArgException)

	public:
		CInvalidArgException(BOOL bAutoDelete, UINT nResourceID) : CSimpleException(bAutoDelete, nResourceID) {}
	public:
	public:

};

class CMemoryException : public CSimpleException
{
	//DECLARE_DYNAMIC(CMemoryException)

	public:
		CMemoryException(BOOL bAutoDelete, UINT nResourceID) : CSimpleException(bAutoDelete, nResourceID) {}
	public:
	public:

};

class CNotSupportedException : public CSimpleException
{
	//DECLARE_DYNAMIC(CNotSupportedException)

	public:
		CNotSupportedException(BOOL bAutoDelete, UINT nResourceID) : CSimpleException(bAutoDelete, nResourceID) {}
	public:
	public:

};

class CResourceException : public CSimpleException
{
	//DECLARE_DYNAMIC(CResourceException)

	public:
		CResourceException(BOOL bAutoDelete, UINT nResourceID) : CSimpleException(bAutoDelete, nResourceID) {}
	public:
	public:

};


