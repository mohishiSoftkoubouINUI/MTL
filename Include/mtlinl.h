


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


class CDialogBar;
class CReBar;
class CStatusBar;
class CToolBar;


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



// for global data that should be in COMDATs (packaged data)
#ifndef AFX_COMDAT
#define AFX_COMDAT _SELECTANY
#endif

#ifndef PTM_WARNING_DISABLE
#define PTM_WARNING_DISABLE
#endif
#ifndef PTM_WARNING_RESTORE
#define PTM_WARNING_RESTORE
#endif



#define _VOLATILE_STATIC_FUNC_T_V(retType, pfnHandler) \
	volatile retType (*pFunc)() = (volatile retType (*)())&pfnHandler; \
	return (retType)(*pFunc)(); \

#define _VOLATILE_STATIC_FUNC_T_T_1(retType, pfnHandler, argType, tArg) \
	volatile retType (*pFunc)(argType) = (volatile retType (*)(argType))&pfnHandler; \
	return (retType)(*pFunc)(tArg); \

#define _VOLATILE_CLASS_FUNC_V_V(theClass, pfnHandler) \
	volatile void (theClass::*pFunc)() = (volatile void (theClass::*)())&theClass::pfnHandler; \
	(this->*pFunc)(); \




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

class COleException : public CException
{
	//DECLARE_DYNAMIC(COleException)

	public:
		COleException() { m_sc = S_OK; }

	public:
		SCODE m_sc;

	public:

};

/*============================================================================*/
// IDispatch specific exception

class COleDispatchException : public CException
{
	//DECLARE_DYNAMIC(COleDispatchException)

	public:
		COleDispatchException(LPCTSTR lpszDescription, UINT nHelpID, WORD wCode)
		{
			m_dwHelpContext = nHelpID != 0 ? HID_BASE_DISPATCH + nHelpID : 0;
			m_wCode = wCode;
			if (lpszDescription != NULL)
				m_strDescription = lpszDescription;
			m_scError = wCode != 0 ? NOERROR : E_UNEXPECTED;
		}

	public:
		// Attributes
		WORD m_wCode;   // error code (specific to IDispatch implementation)
		CString m_strDescription;   // human readable description of the error
		DWORD m_dwHelpContext;      // help context for error
		SCODE m_scError;            // SCODE describing the error

		// usually empty in application which creates it (eg. servers)
		CString m_strHelpFile;      // help file to use with m_dwHelpContext
		CString m_strSource;        // source of the error (name of server)

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






#if 0

#define _SHELLDIALOG

/*============================================================================*/
// OLE interface map handling

struct AFX_INTERFACEMAP_ENTRY
{
	const void* piid;       // the interface id (IID) (NULL for aggregate)
	size_t nOffset;         // offset of the interface vtable from m_unknown
};

struct AFX_INTERFACEMAP
{
	const AFX_INTERFACEMAP* pBaseMap;
	const AFX_INTERFACEMAP_ENTRY* pEntry; // map for this class
};


#define DECLARE_INTERFACE_MAP() \
	private: \
		static const AFX_INTERFACEMAP_ENTRY _interfaceEntries[]; \
	protected: \
		static const AFX_INTERFACEMAP interfaceMap; \
		virtual const AFX_INTERFACEMAP* GetInterfaceMap() const; \

/*============================================================================*/
// OLE COM (Component Object Model) implementation infrastructure
//      - data driven QueryInterface
//      - standard implementation of aggregate AddRef and Release

#define BEGIN_INTERFACE_PART(localClass, baseClass) \
	class X##localClass : public baseClass \
	{ \
	public: \
		STDMETHOD_(ULONG, AddRef)(); \
		STDMETHOD_(ULONG, Release)(); \
		STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppvObj); \

// Note: Inserts the rest of OLE functionality between these two macros,
//  depending upon the interface that is being implemented.  It is not
//  necessary to include AddRef, Release, and QueryInterface since those
//  member functions are declared by the macro.

#define END_INTERFACE_PART(localClass) \
	} m_x##localClass; \
	friend class X##localClass; \

struct CInterfacePlaceHolder
{
	DWORD_PTR m_vtbl;   // filled in with USE_INTERFACE_PART
	CInterfacePlaceHolder() { m_vtbl = 0; }
};

#define END_INTERFACE_PART_OPTIONAL(localClass) \
		}; \
	CInterfacePlaceHolder m_x##localClass; \
	friend class X##localClass; \



#endif


/*============================================================================*/
// OLE event sink map handling 

// parameter types: by value VTs
#define VTS_I2              "\x02"      // a 'short'
#define VTS_I4              "\x03"      // a 'long'
#define VTS_R4              "\x04"      // a 'float'
#define VTS_R8              "\x05"      // a 'double'
#define VTS_CY              "\x06"      // a 'CY' or 'CY*'
#define VTS_DATE            "\x07"      // a 'DATE'
#define VTS_WBSTR           "\x08"      // an 'LPCOLESTR'
#define VTS_DISPATCH        "\x09"      // an 'IDispatch*'
#define VTS_SCODE           "\x0A"      // an 'SCODE'
#define VTS_BOOL            "\x0B"      // a 'VARIANT_BOOL'
#define VTS_VARIANT         "\x0C"      // a 'const VARIANT&' or 'VARIANT*'
#define VTS_UNKNOWN         "\x0D"      // an 'IUnknown*'
#if defined(_UNICODE)
#define VTS_BSTR            VTS_WBSTR// an 'LPCOLESTR'
#define VT_BSTRT            VT_BSTR
#else
#define VTS_BSTR            "\x0E"  // an 'LPCSTR'
#define VT_BSTRA            14
#define VT_BSTRT            VT_BSTRA
#endif
#define VTS_I1              "\x10"      // a 'signed char'
#define VTS_UI1             "\x11"      // a 'BYTE'
#define VTS_UI2             "\x12"      // a 'WORD'
#define VTS_UI4             "\x13"      // a 'DWORD'
#define VTS_I8              "\x14"      // a 'LONGLONG'
#define VTS_UI8             "\x15"      // a 'ULONGLONG'

// parameter types: by reference VTs
#define VTS_PI2             "\x42"      // a 'short*'
#define VTS_PI4             "\x43"      // a 'long*'
#define VTS_PR4             "\x44"      // a 'float*'
#define VTS_PR8             "\x45"      // a 'double*'
#define VTS_PCY             "\x46"      // a 'CY*'
#define VTS_PDATE           "\x47"      // a 'DATE*'
#define VTS_PBSTR           "\x48"      // a 'BSTR*'
#define VTS_PDISPATCH       "\x49"      // an 'IDispatch**'
#define VTS_PSCODE          "\x4A"      // an 'SCODE*'
#define VTS_PBOOL           "\x4B"      // a 'VARIANT_BOOL*'
#define VTS_PVARIANT        "\x4C"      // a 'VARIANT*'
#define VTS_PUNKNOWN        "\x4D"      // an 'IUnknown**'
#define VTS_PI1             "\x50"      // a 'signed char*'
#define VTS_PUI1            "\x51"      // a 'BYTE*'
#define VTS_PUI2            "\x52"      // a 'WORD*'
#define VTS_PUI4            "\x53"      // a 'DWORD*'
#define VTS_PI8             "\x54"      // a 'LONGLONG*'
#define VTS_PUI8            "\x55"      // a 'ULONGLONG*'

// special VT_ and VTS_ values
#define VTS_NONE            NULL        // used for members with 0 params
#define VT_MFCVALUE         0xFFF       // special value for DISPID_VALUE
#define VT_MFCBYREF         0x40        // indicates VT_BYREF type
#define VT_MFCMARKER        0xFF        // delimits named parameters (INTERNAL USE)

// variant handling (use V_BSTRT when you have ANSI BSTRs, as in DAO)
#ifndef _UNICODE
#define V_BSTRT(b)  (LPSTR)V_BSTR(b)
#else
#define V_BSTRT(b)  V_BSTR(b)
#endif

/////////////////////////////////////////////////////////////////////////////
// OLE control parameter types

#define VTS_COLOR           VTS_I4      // OLE_COLOR
#define VTS_XPOS_PIXELS     VTS_I4      // OLE_XPOS_PIXELS
#define VTS_YPOS_PIXELS     VTS_I4      // OLE_YPOS_PIXELS
#define VTS_XSIZE_PIXELS    VTS_I4      // OLE_XSIZE_PIXELS
#define VTS_YSIZE_PIXELS    VTS_I4      // OLE_YSIZE_PIXELS
#define VTS_XPOS_HIMETRIC   VTS_I4      // OLE_XPOS_HIMETRIC
#define VTS_YPOS_HIMETRIC   VTS_I4      // OLE_YPOS_HIMETRIC
#define VTS_XSIZE_HIMETRIC  VTS_I4      // OLE_XSIZE_HIMETRIC
#define VTS_YSIZE_HIMETRIC  VTS_I4      // OLE_YSIZE_HIMETRIC
#define VTS_TRISTATE        VTS_I2      // OLE_TRISTATE
#define VTS_OPTEXCLUSIVE    VTS_BOOL    // OLE_OPTEXCLUSIVE

#define VTS_PCOLOR          VTS_PI4     // OLE_COLOR*
#define VTS_PXPOS_PIXELS    VTS_PI4     // OLE_XPOS_PIXELS*
#define VTS_PYPOS_PIXELS    VTS_PI4     // OLE_YPOS_PIXELS*
#define VTS_PXSIZE_PIXELS   VTS_PI4     // OLE_XSIZE_PIXELS*
#define VTS_PYSIZE_PIXELS   VTS_PI4     // OLE_YSIZE_PIXELS*
#define VTS_PXPOS_HIMETRIC  VTS_PI4     // OLE_XPOS_HIMETRIC*
#define VTS_PYPOS_HIMETRIC  VTS_PI4     // OLE_YPOS_HIMETRIC*
#define VTS_PXSIZE_HIMETRIC VTS_PI4     // OLE_XSIZE_HIMETRIC*
#define VTS_PYSIZE_HIMETRIC VTS_PI4     // OLE_YSIZE_HIMETRIC*
#define VTS_PTRISTATE       VTS_PI2     // OLE_TRISTATE*
#define VTS_POPTEXCLUSIVE   VTS_PBOOL   // OLE_OPTEXCLUSIVE*

#define VTS_FONT            VTS_DISPATCH    // IFontDispatch*
#define VTS_PICTURE         VTS_DISPATCH    // IPictureDispatch*

#define VTS_HANDLE          VTS_I4      // OLE_HANDLE
#define VTS_PHANDLE         VTS_PI4     // OLE_HANDLE*

/////////////////////////////////////////////////////////////////////////////
// COleVariant class helpers

#define AFX_OLE_TRUE (-1)
#define AFX_OLE_FALSE 0

/*============================================================================*/
// Implementation of event sink handling

// DSC Sink state/reason codes passed to MFC user event handlers
enum DSCSTATE
{
	dscNoState = 0,
	dscOKToDo,
	dscCancelled,
	dscSyncBefore,
	dscAboutToDo,
	dscFailedToDo,
	dscSyncAfter,
	dscDidEvent
};

enum DSCREASON
{
	dscNoReason = 0,
	dscClose,
	dscCommit,
	dscDelete,
	dscEdit,
	dscInsert,
	dscModify,
	dscMove
};

struct AFX_EVENT
{
	enum
	{
		event,
		propRequest, propChanged,
		propDSCNotify
	};

	AFX_EVENT(int eventKind)
	{
		m_eventKind = eventKind;
		m_dispid = DISPID_UNKNOWN;
		m_pDispParams = NULL;
		m_pExcepInfo = NULL;
		m_puArgError = NULL;
		m_hResult = NOERROR;
		m_nDSCState = dscNoState;
		m_nDSCReason = dscNoReason;
	}

	AFX_EVENT(int eventKind, DISPID dispid, DISPPARAMS* pDispParams = NULL, EXCEPINFO* pExcepInfo = NULL, UINT* puArgError = NULL)
	{
		m_eventKind = eventKind;
		m_dispid = dispid;
		m_pDispParams = pDispParams;
		m_pExcepInfo = pExcepInfo;
		m_puArgError = puArgError;
		m_hResult = NOERROR;
		m_nDSCState = dscNoState;
		m_nDSCReason = dscNoReason;
	}

	int m_eventKind;
	DISPID m_dispid;
	DISPPARAMS* m_pDispParams;
	EXCEPINFO* m_pExcepInfo;
	UINT* m_puArgError;
	BOOL m_bPropChanged;
	HRESULT m_hResult;
	DSCSTATE m_nDSCState;
	DSCREASON m_nDSCReason;
};


typedef void (CCmdTarget::*AFX_PMSG)(void);

enum AFX_DISPMAP_FLAGS
{
	afxDispCustom = 0,
	afxDispStock = 1
};

struct AFX_DISPMAP_ENTRY
{
	LPCTSTR lpszName;       // member/property name
	long lDispID;           // DISPID (may be DISPID_UNKNOWN)
	LPCSTR lpszParams;      // member parameter description
	WORD vt;                // return value type / or type of property
	AFX_PMSG pfn;           // normal member On<membercall> or, OnGet<property>
	AFX_PMSG pfnSet;        // special member for OnSet<property>
	size_t nPropOffset;     // property offset
	AFX_DISPMAP_FLAGS flags;// flags (e.g. stock/custom)
};

struct AFX_EVENTSINKMAP_ENTRY
{
	AFX_DISPMAP_ENTRY dispEntry;
	UINT nCtrlIDFirst;
	UINT nCtrlIDLast;
};

struct AFX_EVENTSINKMAP
{
	const AFX_EVENTSINKMAP* pBaseMap;
	const AFX_EVENTSINKMAP_ENTRY* lpEntries;
	UINT* lpEntryCount;
};


/////////////////////////////////////////////////////////////////////////////
// EventSink Maps

#define DECLARE_EVENTSINK_MAP() \
public: \
	static const AFX_EVENTSINKMAP* _GetEventSinkMap() ; \
	virtual const AFX_EVENTSINKMAP* GetEventSinkMap() const { return _GetEventSinkMap(); } \

#define BEGIN_EVENTSINK_MAP(theClass, baseClass) \
	PTM_WARNING_DISABLE \
	_INLINE const AFX_EVENTSINKMAP* theClass::_GetEventSinkMap() \
	{ \
		typedef theClass ThisClass;						   \
		typedef baseClass TheBaseClass;					   \
		static const AFX_EVENTSINKMAP_ENTRY _eventsinkEntries[] = \
		{\

#define END_EVENTSINK_MAP() \
			{ VTS_NONE, DISPID_UNKNOWN, VTS_NONE, VT_VOID, \
				(AFX_PMSG)NULL, (AFX_PMSG)NULL, (size_t)-1, afxDispCustom, (UINT)-1, 0 } \
		}; \
		static UINT _eventsinkEntryCount = (UINT)-1; \
		static const AFX_EVENTSINKMAP eventsinkMap = \
			{ TheBaseClass::_GetEventSinkMap(), &_eventsinkEntries[0], &_eventsinkEntryCount }; \
		return &eventsinkMap; \
	} \
	PTM_WARNING_RESTORE \



#define ON_EVENT(theClass, id, dispid, pfnHandler, vtsParams) \
	{ _T(""), dispid, vtsParams, VT_BOOL, \
		(AFX_PMSG)(void (theClass::*)(void))&pfnHandler, (AFX_PMSG)0, 0, \
		afxDispCustom, id, (UINT)-1 }, \

#define ON_EVENT_RANGE(theClass, idFirst, idLast, dispid, pfnHandler, vtsParams) \
	{ _T(""), dispid, vtsParams, VT_BOOL, \
		(AFX_PMSG)(void (theClass::*)(void))&pfnHandler, (AFX_PMSG)0, 0, \
		afxDispCustom, idFirst, idLast }, \

#define ON_PROPNOTIFY(theClass, id, dispid, pfnRequest, pfnChanged) \
	{ _T(""), dispid, VTS_NONE, VT_VOID, \
		(AFX_PMSG)(BOOL (CCmdTarget::*)(BOOL*))&pfnRequest, \
		(AFX_PMSG)(BOOL (CCmdTarget::*)(void))&pfnChanged, \
		1, afxDispCustom, id, (UINT)-1 }, \

#define ON_PROPNOTIFY_RANGE(theClass, idFirst, idLast, dispid, pfnRequest, pfnChanged) \
	{ _T(""), dispid, VTS_NONE, VT_VOID, \
		(AFX_PMSG)(BOOL (CCmdTarget::*)(UINT, BOOL*))&pfnRequest, \
		(AFX_PMSG)(BOOL (CCmdTarget::*)(UINT))&pfnChanged, \
		1, afxDispCustom, idFirst, idLast }, \

#define ON_DSCNOTIFY(theClass, id, pfnNotify) \
	{ _T(""), DISPID_UNKNOWN, VTS_NONE, VT_VOID, \
		(AFX_PMSG)(BOOL (CCmdTarget::*)(DSCSTATE, DSCREASON, BOOL*))&pfnNotify, (AFX_PMSG)0, \
		1, afxDispCustom, id, (UINT)-1 }, \

#define ON_DSCNOTIFY_RANGE(theClass, idFirst, idLast, pfnNotify) \
	{ _T(""), DISPID_UNKNOWN, VTS_NONE, VT_VOID, \
		(AFX_PMSG)(BOOL (CCmdTarget::*)(UINT, DSCSTATE, DSCREASON, BOOL*))&pfnNotify, (AFX_PMSG)0, \
		1, afxDispCustom, idFirst, idLast }, \

#define ON_EVENT_REFLECT(theClass, dispid, pfnHandler, vtsParams) \
	{ _T(""), dispid, vtsParams, VT_BOOL, \
		(AFX_PMSG)(void (theClass::*)(void))&pfnHandler, (AFX_PMSG)0, 0, \
		afxDispCustom, (UINT)-1, (UINT)-1 }, \

#define ON_PROPNOTIFY_REFLECT(theClass, dispid, pfnRequest, pfnChanged) \
	{ _T(""), dispid, VTS_NONE, VT_VOID, \
		(AFX_PMSG)(BOOL (CCmdTarget::*)(BOOL*))&pfnRequest, \
		(AFX_PMSG)(BOOL (CCmdTarget::*)(void))&pfnChanged, \
		1, afxDispCustom, (UINT)-1, (UINT)-1 }, \



#if defined ( _M_IX86 )

#ifndef _MTL_ATL3
PVOID __stdcall __AllocStdCallThunk(VOID);
VOID  __stdcall __FreeStdCallThunk(PVOID);
/*
--- f:\dd\vctools\vc7libs\ship\atlmfc\src\atl\atls\atlfuncs.cpp ----------------
0144272C 8B 44 24 04          mov         eax,dword ptr [esp+4]  	// get pThunk
01442730 8B 50 04             mov         edx,dword ptr [eax+4]  	// get the pThunk->pThis
01442733 89 54 24 04          mov         dword ptr [esp+4],edx  	// replace pThunk with pThis
01442737 8B 40 08             mov         eax,dword ptr [eax+8]		// get pThunk->pfn
0144273A FF E0                jmp         eax						// jump pfn
*/
#endif //  _MTL_ATL3

#pragma pack(push,1)
template <class T>
class CThisCallThunk	// le 16 bytes
{
	struct _Thunk {
		DWORD   m_movp;        // 59             pop ecx
		BYTE    m_movi;        // 89 0c 24       mov DWORD PTR [esp],ecx
		DWORD   m_this;         // b9 xx xx xx xx mov ecx, pThis
		BYTE    m_jmp;          // e9 rr rr rr rr jmp func
		DWORD   m_relproc;      // relative jmp
	};

	public:
		CThisCallThunk()
		{
	#ifndef _MTL_ATL3
			m_pFunc = ATL::__AllocStdCallThunk();
	#else // _MTL_ATL3
			m_pFunc = &m_Thunk;
	#endif // _MTL_ATL3
		}
	#ifndef _MTL_ATL3
		~CThisCallThunk()
		{
			ATL::__FreeStdCallThunk(m_pFunc);
		}
	#endif // _MTL_ATL3

	public:
		typedef void(_CDECL T::*TMFP)();

		void* m_pVtable;
		void* m_pFunc;

	#ifdef _MTL_ATL3
		struct _Thunk m_Thunk;
	#endif // _MTL_ATL3

	public:
		void Init(TMFP dw, void* pThis)
		{
			union {
				DWORD dwFunc;
				TMFP pfn;
			} pfn;
			pfn.pfn = dw;

			m_pVtable = &m_pFunc;

			struct _Thunk* pThunk = (struct _Thunk*)m_pFunc;

			pThunk->m_movp = 0x240c8959;
			pThunk->m_movi = 0xb9;
			pThunk->m_this = (DWORD)pThis;
			pThunk->m_jmp = 0xe9;
			pThunk->m_relproc = (int)pfn.dwFunc - ((int)pThunk + sizeof(struct _Thunk));

			FlushInstructionCache(GetCurrentProcess(), pThunk, sizeof(struct _Thunk));
		}
};
#pragma pack(pop)
#else
template <class T>
class CThisCallThunk : public CComStdCallThunk<T>
{
};
#endif

