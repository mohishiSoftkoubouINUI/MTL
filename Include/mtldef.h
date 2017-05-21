

#define _CDECL __cdecl
#define _STDCALL __stdcall
#define _THISCALL __thiscall
#define _INLINE __inline
#define _FORCEINLINE __forceinline
#define _SELECTANY __declspec(selectany)





// afxwin.h

#define afx_msg




#ifdef _DEBUG
#define DEBUG_NEW new

#define DEBUG_ONLY(f)      _ASSERTE(f)
#define VERIFY(f)			ASSERT(f)

#define TRACE					CAfx::_Trace2
#define TRACE0(sz)              TRACE("%s", sz)
#define TRACE1(sz, p1)          TRACE(sz, p1)
#define TRACE2(sz, p1, p2)      TRACE(sz, p1, p2)
#define TRACE3(sz, p1, p2, p3)  TRACE(sz, p1, p2, p3)

#define DECLARE_AFX_TRACE_CATEGORY( name ) extern ATL::CTraceCategory name;
#else // _DEBUG
#define DEBUG_NEW new

#define DEBUG_ONLY(f)      ((void)0)
#define VERIFY(f)	(f)

#define TRACE       (1) ? (void)(0) : CAfx::_TraceNop
#define TRACE0(sz)
#define TRACE1(sz, p1)
#define TRACE2(sz, p1, p2)
#define TRACE3(sz, p1, p2, p3)

#define DECLARE_AFX_TRACE_CATEGORY( name ) const DWORD_PTR name = 0;

#endif // _DEBUG

#define ASSERT(f)          DEBUG_ONLY((f))
#define ASSERT_KINDOF(class_name, object) ASSERT((object)->IsKindOf(RUNTIME_CLASS(class_name)))
#define ASSERT_VALID(pOb)
#define ENSURE_ASSERT(cond)	do { int __afx_condVal=!!(cond); ASSERT(__afx_condVal); } while (false)
#define ENSURE(cond) ENSURE_ASSERT(cond)
#define ENSURE_ARG(cond) ENSURE_ASSERT(cond)
#define ENSURE_VALID(pOb)
#define ENSURE_THROW(cond, exception)	\
	do { int __afx_condVal=!!(cond); ASSERT(__afx_condVal); if (!(__afx_condVal)){exception;} } while (false)
#define UNUSED(x)
#define UNUSED_ALWAYS(x)


// BASED_XXXX macros are provided for backward compatibility
#ifndef BASED_CODE
#define BASED_CODE
#endif

#ifndef BASED_DEBUG
#define BASED_DEBUG
#endif

#ifndef BASED_STACK
#define BASED_STACK
#endif

//
// Generic test for success on any status value (non-negative numbers
// indicate success).
//
#ifndef SUCCEEDED
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#endif
//
// and the inverse
//
#ifndef FAILED
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#endif



/*============================================================================*/
// Exception macros using try, catch and throw
//  (for backward compatibility to previous versions of MFC)

#define TRY { try {

#define CATCH(class, e) } catch (class* e) \
			{ (e) ;

#define AND_CATCH(class, e) } catch (class* e) \
			{ (e) ;

#define END_CATCH } }

#define THROW(e) throw e
#define THROW_LAST() (throw)

// Advanced macros for smaller code
#define CATCH_ALL(e) } catch (CException* e) \
			{ { (e) ;

#define AND_CATCH_ALL(e) } catch (CException* e) \
			{ { (e) ;

#define END_CATCH_ALL } } }

#define END_TRY } catch (CException* e) \
			{ (e) ; } }

#define DELETE_EXCEPTION(e) { if (e) { e->Delete(); } }



// Helper macros
#define RUNTIME_CLASS(class_name) ((CRuntimeClass*)(&class_name::class##class_name))


/////////////////////////////////////////////////////////////////////////////
// Global ID ranges (see Technical note TN020 for more details)

// 8000 -> FFFF command IDs (used for menu items, accelerators and controls)
#define IS_COMMAND_ID(nID)  ((nID) & 0x8000)

// 8000 -> DFFF : user commands
// E000 -> EFFF : AFX commands and other things
// F000 -> FFFF : standard windows commands and other things etc
// E000 -> E7FF standard commands
// E800 -> E8FF control bars (first 32 are special)
// E900 -> EEFF standard window controls/components
// EF00 -> EFFF SC_ menu help
// F000 -> FFFF standard strings
#define ID_COMMAND_FROM_SC(sc)  (((sc - 0xF000) >> 4) + AFX_IDS_SCFIRST)

// 0000 -> 7FFF IDR range
// 0000 -> 6FFF : user resources
// 7000 -> 7FFF : AFX (and standard windows) resources
// IDR ranges (NOTE: IDR_ values must be <32768)
#define ASSERT_VALID_IDR(nIDR) ASSERT((nIDR) != 0 && (nIDR) < 0x8000)





#define AFX_ABBREV_FILENAME_LEN 30

// Restart Manager support flags
#define AFX_RESTART_MANAGER_SUPPORT_RESTART				0x01  // restart support, means application is registered via RegisterApplicationRestart

#define ATL_MAKEINTRESOURCEA(i) ((LPSTR)((ULONG_PTR)((WORD)(i))))
#define ATL_MAKEINTRESOURCEW(i) ((LPWSTR)((ULONG_PTR)((WORD)(i))))
#ifdef UNICODE
#define ATL_MAKEINTRESOURCE  ATL_MAKEINTRESOURCEW
#else
#define ATL_MAKEINTRESOURCE  ATL_MAKEINTRESOURCEA
#endif // !UNICODE

#define AfxFindResourceHandle(lpszResource, lpszType) AfxGetResourceHandle()


#define AFX_CX_BORDER   1
#define AFX_CY_BORDER   1

#define AFX_CX_GRIPPER  3
#define AFX_CY_GRIPPER  3
#define AFX_CX_BORDER_GRIPPER 2
#define AFX_CY_BORDER_GRIPPER 2


// Frame window menu bar visibility states
#define AFX_MBS_VISIBLE 0x01L // visible
#define AFX_MBS_HIDDEN  0x02L // hidden

// Frame window menu bar visibility styles
#define AFX_MBV_KEEPVISIBLE    0x01L // always visible
#define AFX_MBV_DISPLAYONFOCUS 0x02L // toggle state on ALT
#define AFX_MBV_DISPLAYONF10   0x04L // display on F10


#define _AFX_MRU_COUNT   4      // default support for 4 entries in file MRU
#define _AFX_MRU_MAX_COUNT 16   // currently allocated id range supports 16


#define _AFX_TEMP_CLASS_NAME_SIZE 96


#define AFX_WM_MOUSELAST 0x0209

#define AFX_WM_UPDATETOOLTIPS (WM_USER + 202)

// common style for form views
#define AFX_WS_DEFAULT_VIEW             (WS_CHILD | WS_VISIBLE | WS_BORDER)



/////////////////////////////////////////////////////////////////////////////
// Window class names and other window creation support

#define AFX_WND_REG                     0x00001
#define AFX_WNDCONTROLBAR_REG           0x00002
#define AFX_WNDMDIFRAME_REG             0x00004
#define AFX_WNDFRAMEORVIEW_REG          0x00008
#define AFX_WNDCOMMCTLS_REG             0x00010 // means all original Win95
#define AFX_WNDOLECONTROL_REG           0x00020
#define AFX_WNDCOMMCTL_UPDOWN_REG       0x00040 // these are original Win95
#define AFX_WNDCOMMCTL_TREEVIEW_REG     0x00080
#define AFX_WNDCOMMCTL_TAB_REG          0x00100
#define AFX_WNDCOMMCTL_PROGRESS_REG     0x00200
#define AFX_WNDCOMMCTL_LISTVIEW_REG     0x00400
#define AFX_WNDCOMMCTL_HOTKEY_REG       0x00800
#define AFX_WNDCOMMCTL_BAR_REG          0x01000
#define AFX_WNDCOMMCTL_ANIMATE_REG      0x02000
#define AFX_WNDCOMMCTL_INTERNET_REG     0x04000 // these are new in IE4
#define AFX_WNDCOMMCTL_COOL_REG         0x08000
#define AFX_WNDCOMMCTL_USEREX_REG       0x10000
#define AFX_WNDCOMMCTL_DATE_REG         0x20000
#define AFX_WNDCOMMCTL_LINK_REG         0x40000 // new in IE6 (Unicode only control)
#define AFX_WNDCOMMCTL_PAGER_REG        0x80000 // new in IE5? (Unicode only control)

#define AFX_WIN95CTLS_MASK              0x03FC0 // UPDOWN -> ANIMATE
#ifndef _UNICODE
#define AFX_WNDCOMMCTLSALL_REG          0x3C010 // COMMCTLS|INTERNET|COOL|USEREX|DATE
#define AFX_WNDCOMMCTLSNEW_REG          0x3C000 // INTERNET|COOL|USEREX|DATE
#else
#define AFX_WNDCOMMCTLSALL_REG          0xFC010 // COMMCTLS|INTERNET|COOL|USEREX|DATE|LINK|PAGER
#define AFX_WNDCOMMCTLSNEW_REG          0xFC000 // INTERNET|COOL|USEREX|DATE|LINK|PAGER
#endif


/*============================================================================*/
// Other implementation helpers

#define BEFORE_START_POSITION ((POSITION)-1L)


/////////////////////////////////////////////////////////////////////////////
// General style bits etc

// ControlBar styles
#define CBRS_ALIGN_LEFT     0x1000L
#define CBRS_ALIGN_TOP      0x2000L
#define CBRS_ALIGN_RIGHT    0x4000L
#define CBRS_ALIGN_BOTTOM   0x8000L
#define CBRS_ALIGN_ANY      0xF000L

#define CBRS_BORDER_LEFT    0x0100L
#define CBRS_BORDER_TOP     0x0200L
#define CBRS_BORDER_RIGHT   0x0400L
#define CBRS_BORDER_BOTTOM  0x0800L
#define CBRS_BORDER_ANY     0x0F00L

#define CBRS_TOOLTIPS       0x0010L
#define CBRS_FLYBY          0x0020L
#define CBRS_FLOAT_MULTI    0x0040L
#define CBRS_BORDER_3D      0x0080L
#define CBRS_HIDE_INPLACE   0x0008L
#define CBRS_SIZE_DYNAMIC   0x0004L
#define CBRS_SIZE_FIXED     0x0002L
#define CBRS_FLOATING       0x0001L

#define CBRS_GRIPPER        0x00400000L

#define CBRS_ORIENT_HORZ    (CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM)
#define CBRS_ORIENT_VERT    (CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT)
#define CBRS_ORIENT_ANY     (CBRS_ORIENT_HORZ|CBRS_ORIENT_VERT)

#define CBRS_ALL            0x0040FFFFL

// the CBRS_ style is made up of an alignment style and a draw border style
//  the alignment styles are mutually exclusive
//  the draw border styles may be combined
#define CBRS_NOALIGN        0x00000000L
#define CBRS_LEFT           (CBRS_ALIGN_LEFT|CBRS_BORDER_RIGHT)
#define CBRS_TOP            (CBRS_ALIGN_TOP|CBRS_BORDER_BOTTOM)
#define CBRS_RIGHT          (CBRS_ALIGN_RIGHT|CBRS_BORDER_LEFT)
#define CBRS_BOTTOM         (CBRS_ALIGN_BOTTOM|CBRS_BORDER_TOP)


/*============================================================================*/
// CFrameWnd - base class for SDI and other frame windows

// Frame window styles
#define FWS_ADDTOTITLE  0x00008000L // modify title based on content
#define FWS_PREFIXTITLE 0x00004000L // show document name before app name
#define FWS_SNAPTOBARS  0x00002000L // snap size to size of contained bars



/////////////////////////////////////////////////////////////////////////////
// Context sensitive help support (see Technical note TN028 for more details)

// Help ID bases
#define HID_BASE_COMMAND    0x00010000UL        // ID and IDM
#define HID_BASE_RESOURCE   0x00020000UL        // IDR and IDD
#define HID_BASE_PROMPT     0x00030000UL        // IDP
#define HID_BASE_NCAREAS    0x00040000UL
#define HID_BASE_CONTROL    0x00050000UL        // IDC
#define HID_BASE_DISPATCH   0x00060000UL        // IDispatch help codes



/////////////////////////////////////////////////////////////////////////////
// Standard window components

/////////////////////////////////////////////////////////////////////////////
// Standard Commands

// File commands
#define ID_FILE_NEW                     0xE100
#define ID_FILE_OPEN                    0xE101
#define ID_FILE_CLOSE                   0xE102
#define ID_FILE_SAVE                    0xE103
#define ID_FILE_SAVE_AS                 0xE104
#define ID_FILE_PAGE_SETUP              0xE105
#define ID_FILE_PRINT_SETUP             0xE106
#define ID_FILE_PRINT                   0xE107
#define ID_FILE_PRINT_DIRECT            0xE108
#define ID_FILE_PRINT_PREVIEW           0xE109
#define ID_FILE_UPDATE                  0xE10A
#define ID_FILE_SAVE_COPY_AS            0xE10B
#define ID_FILE_SEND_MAIL               0xE10C
#define ID_FILE_NEW_FRAME               0xE10D

#define ID_FILE_MRU_FIRST               0xE110
#define ID_FILE_MRU_FILE1               0xE110          // range - 16 max
#define ID_FILE_MRU_FILE2               0xE111
#define ID_FILE_MRU_FILE3               0xE112
#define ID_FILE_MRU_FILE4               0xE113
#define ID_FILE_MRU_FILE5               0xE114
#define ID_FILE_MRU_FILE6               0xE115
#define ID_FILE_MRU_FILE7               0xE116
#define ID_FILE_MRU_FILE8               0xE117
#define ID_FILE_MRU_FILE9               0xE118
#define ID_FILE_MRU_FILE10              0xE119
#define ID_FILE_MRU_FILE11              0xE11A
#define ID_FILE_MRU_FILE12              0xE11B
#define ID_FILE_MRU_FILE13              0xE11C
#define ID_FILE_MRU_FILE14              0xE11D
#define ID_FILE_MRU_FILE15              0xE11E
#define ID_FILE_MRU_FILE16              0xE11F
#define ID_FILE_MRU_LAST                0xE11F

// Edit commands
#define ID_EDIT_CLEAR                   0xE120
#define ID_EDIT_CLEAR_ALL               0xE121
#define ID_EDIT_COPY                    0xE122
#define ID_EDIT_CUT                     0xE123
#define ID_EDIT_FIND                    0xE124
#define ID_EDIT_PASTE                   0xE125
#define ID_EDIT_PASTE_LINK              0xE126
#define ID_EDIT_PASTE_SPECIAL           0xE127
#define ID_EDIT_REPEAT                  0xE128
#define ID_EDIT_REPLACE                 0xE129
#define ID_EDIT_SELECT_ALL              0xE12A
#define ID_EDIT_UNDO                    0xE12B
#define ID_EDIT_REDO                    0xE12C


// Mode indicators in status bar - these are routed like commands
#define ID_INDICATOR_EXT                0xE700  // extended selection indicator
#define ID_INDICATOR_CAPS               0xE701  // cap lock indicator
#define ID_INDICATOR_NUM                0xE702  // num lock indicator
#define ID_INDICATOR_SCRL               0xE703  // scroll lock indicator
#define ID_INDICATOR_OVR                0xE704  // overtype mode indicator
#define ID_INDICATOR_REC                0xE705  // record mode indicator
#define ID_INDICATOR_KANA               0xE706  // kana lock indicator

#define ID_SEPARATOR                    0   // special separator value


#define LAYOUT_LTR								 0x00000000	


// Layout Modes for CalcDynamicLayout
#define LM_STRETCH  0x01    // same meaning as bStretch in CalcFixedLayout.  If set, ignores nLength
// and returns dimensions based on LM_HORZ state, otherwise LM_HORZ is used
// to determine if nLength is the desired horizontal or vertical length
// and dimensions are returned based on nLength
#define LM_HORZ     0x02    // same as bHorz in CalcFixedLayout
#define LM_MRUWIDTH 0x04    // Most Recently Used Dynamic Width
#define LM_HORZDOCK 0x08    // Horizontal Docked Dimensions
#define LM_VERTDOCK 0x10    // Vertical Docked Dimensions
#define LM_LENGTHY  0x20    // Set if nLength is a Height instead of a Width
#define LM_COMMIT   0x40    // Remember MRUWidth


/*============================================================================*/
// CMiniFrameWnd

// MiniFrame window styles
#define MFS_SYNCACTIVE      0x00000100L // syncronize activation w/ parent
#define MFS_4THICKFRAME     0x00000200L // thick frame all around (no tiles)
#define MFS_THICKFRAME      0x00000400L // use instead of WS_THICKFRAME
#define MFS_MOVEFRAME       0x00000800L // no sizing, just moving
#define MFS_BLOCKSYSMENU    0x00001000L // block hit testing on system menu


// flags for CWnd::RunModalLoop
#define MLF_NOIDLEMSG       0x0001  // don't send WM_ENTERIDLE messages
#define MLF_NOKICKIDLE      0x0002  // don't send WM_KICKIDLE messages
#define MLF_SHOWONIDLE      0x0004  // show window if not visible at idle time


// Styles for toolbar buttons
#define TBBS_BUTTON     MAKELONG(TBSTYLE_BUTTON, 0) // this entry is button
#define TBBS_SEPARATOR  MAKELONG(TBSTYLE_SEP, 0)    // this entry is a separator
#define TBBS_CHECKBOX   MAKELONG(TBSTYLE_CHECK, 0)  // this is an auto check button
#define TBBS_GROUP      MAKELONG(TBSTYLE_GROUP, 0)  // marks the start of a group
#define TBBS_CHECKGROUP (TBBS_GROUP|TBBS_CHECKBOX)  // normal use of TBBS_GROUP
#define TBBS_DROPDOWN	MAKELONG(TBSTYLE_DROPDOWN, 0) // drop down style
#define TBBS_AUTOSIZE	MAKELONG(TBSTYLE_AUTOSIZE, 0) // autocalc button width
#define TBBS_NOPREFIX	MAKELONG(TBSTYLE_NOPREFIX, 0) // no accel prefix for this button

#define TBBS_BREAK      0x20000000

// styles for display states
#define TBBS_CHECKED    MAKELONG(0, TBSTATE_CHECKED)    // button is checked/down
#define TBBS_PRESSED    MAKELONG(0, TBSTATE_PRESSED)    // button is being depressed
#define TBBS_DISABLED   MAKELONG(0, TBSTATE_ENABLED)    // button is disabled
#define TBBS_INDETERMINATE  MAKELONG(0, TBSTATE_INDETERMINATE)  // third state
#define TBBS_HIDDEN     MAKELONG(0, TBSTATE_HIDDEN) // button is hidden
#define TBBS_WRAPPED    MAKELONG(0, TBSTATE_WRAP)   // button is wrapped at this point
#define TBBS_ELLIPSES	MAKELONG(0, TBSTATE_ELIPSES) 
#define TBBS_MARKED		MAKELONG(0, TBSTATE_MARKED)


// extra MFC defined TTF_ flags for TOOLINFO::uFlags
#define TTF_NOTBUTTON       0x80000000L // no status help on buttondown
#define TTF_ALWAYSTIP       0x40000000L // always show the tip even if not active


// CWnd::m_nFlags (generic to CWnd)
#define WF_TOOLTIPS         0x0001  // window is enabled for tooltips
#define WF_TEMPHIDE         0x0002  // window is temporarily hidden
#define WF_STAYDISABLED     0x0004  // window should stay disabled
#define WF_MODALLOOP        0x0008  // currently in modal loop
#define WF_CONTINUEMODAL    0x0010  // modal loop should continue running
#define WF_OLECTLCONTAINER  0x0100  // some descendant is an OLE control
#define WF_TRACKINGTOOLTIPS 0x0400  // window is enabled for tracking tooltips

// CWnd::m_nFlags (specific to CFrameWnd)
#define WF_STAYACTIVE       0x0020  // look active even though not active
#define WF_NOPOPMSG         0x0040  // ignore WM_POPMESSAGESTRING calls
#define WF_MODALDISABLE     0x0080  // window is disabled
#define WF_KEEPMINIACTIVE   0x0200  // stay activate even though you are deactivated



/*============================================================================*/
// Basic types
// abstract iteration position
struct __POSITION {};
typedef __POSITION* POSITION;

#define WM_CTLCOLOR			0x0019	// WM_CTLCOLOR for 16 bit API compatability

#define WM_SIZEPARENT       0x0361  // lParam = &AFX_SIZEPARENTPARAMS
#define WM_SETMESSAGESTRING 0x0362  // wParam = nIDS (or 0),
#define WM_IDLEUPDATECMDUI  0x0363  // wParam == bDisableIfNoHandler

#define WM_INITIALUPDATE    0x0364  // (params unused) - sent to children
#define WM_COMMANDHELP      0x0365  // lResult = TRUE/FALSE,
#define WM_KICKIDLE         0x036A  // (params unused) causes idles to kick in
#define WM_QUERYCENTERWND   0x036B  // lParam = HWND to use as centering parent
#define WM_FLOATSTATUS      0x036D  // wParam combination of FS_* flags below


// WM_SOCKET_NOTIFY and WM_SOCKET_DEAD are used internally by MFC's
// Windows sockets implementation.  For more information, see sockcore.cpp
#define WM_SOCKET_NOTIFY    0x0373
#define WM_SOCKET_DEAD      0x0374


// same as WM_SETMESSAGESTRING except not popped if IsTracking()
#define WM_POPMESSAGESTRING 0x0375



/////////////////////////////////////////////////////////////////////////////
// useful message ranges

#define WM_NCMOUSEFIRST WM_NCMOUSEMOVE
#define WM_NCMOUSELAST  WM_NCMBUTTONDBLCLK

#define WM_SYSKEYFIRST  WM_SYSKEYDOWN
#define WM_SYSKEYLAST   WM_SYSDEADCHAR


#define WM_RECALCPARENT     0x0368  // force RecalcLayout on frame window
//  (only for inplace frame windows)




/////////////////////////////////////////////////////////////////////////////
// Standard app configurable strings

// for print preview dialog bar
#define AFX_ID_PREVIEW_CLOSE            0xE300
#define AFX_ID_PREVIEW_NUMPAGE          0xE301      // One/Two Page button
#define AFX_ID_PREVIEW_NEXT             0xE302
#define AFX_ID_PREVIEW_PREV             0xE303
#define AFX_ID_PREVIEW_PRINT            0xE304
#define AFX_ID_PREVIEW_ZOOMIN           0xE305
#define AFX_ID_PREVIEW_ZOOMOUT          0xE306

// Parts of dialogs
#define AFX_IDC_LISTBOX                 100

// for print dialog
#define AFX_IDC_PRINT_DOCNAME           201
#define AFX_IDC_PRINT_PRINTERNAME       202
#define AFX_IDC_PRINT_PORTNAME          203
#define AFX_IDC_PRINT_PAGENUM           204

// Property Sheet control id's (determined with Spy++)
#define AFX_IDC_TAB_CONTROL             0x3020


// Standard cursors (0x7901->)
// AFX_IDC = Cursor resources
#define AFX_IDC_CONTEXTHELP             30977       // context sensitive help
#define AFX_IDC_MAGNIFY                 30978       // print preview zoom
#define AFX_IDC_SMALLARROWS             30979       // splitter
#define AFX_IDC_HSPLITBAR               30980       // splitter
#define AFX_IDC_VSPLITBAR               30981       // splitter
#define AFX_IDC_NODROPCRSR              30982       // No Drop Cursor
#define AFX_IDC_TRACKNWSE               30983       // tracker
#define AFX_IDC_TRACKNESW               30984       // tracker
#define AFX_IDC_TRACKNS                 30985       // tracker
#define AFX_IDC_TRACKWE                 30986       // tracker
#define AFX_IDC_TRACK4WAY               30987       // tracker
#define AFX_IDC_MOVE4WAY                30988       // resize bar (server only)

// Wheel mouse cursors
// NOTE: values must be in this order!  See CScrollView::OnTimer()
#define AFX_IDC_MOUSE_PAN_NW            30998       // pan east
#define AFX_IDC_MOUSE_PAN_N             30999       // pan northeast
#define AFX_IDC_MOUSE_PAN_NE            31000       // pan north
#define AFX_IDC_MOUSE_PAN_W             31001       // pan northwest
#define AFX_IDC_MOUSE_PAN_HV            31002       // pan both axis
#define AFX_IDC_MOUSE_PAN_E             31003       // pan west
#define AFX_IDC_MOUSE_PAN_SW            31004       // pan south-west
#define AFX_IDC_MOUSE_PAN_S             31005       // pan south
#define AFX_IDC_MOUSE_PAN_SE            31006       // pan south-east
#define AFX_IDC_MOUSE_PAN_HORZ          31007       // pan X-axis
#define AFX_IDC_MOUSE_PAN_VERT          31008       // pan Y-axis

// Wheel mouse bitmaps
#define AFX_IDC_MOUSE_ORG_HORZ          31009       // anchor for horz only
#define AFX_IDC_MOUSE_ORG_VERT          31010       // anchor for vert only
#define AFX_IDC_MOUSE_ORG_HV            31011       // anchor for horz/vert
#define AFX_IDC_MOUSE_MASK              31012


// These are really COMMDLG dialogs, so there usually isn't a resource
// for them, but these IDs are used as help IDs.
#define AFX_IDD_FILEOPEN                28676
#define AFX_IDD_FILESAVE                28677
#define AFX_IDD_FONT                    28678
#define AFX_IDD_COLOR                   28679
#define AFX_IDD_PRINT                   28680
#define AFX_IDD_PRINTSETUP              28681
#define AFX_IDD_FIND                    28682
#define AFX_IDD_REPLACE                 28683

// Standard dialogs app should leave alone (0x7801->)
#define AFX_IDD_NEWTYPEDLG              30721
#define AFX_IDD_PRINTDLG                30722
#define AFX_IDD_PREVIEW_TOOLBAR         30723



// AFX standard ICON IDs (for MFC V1 apps) (0x7A01->)
#define AFX_IDI_STD_MDIFRAME            31233
#define AFX_IDI_STD_FRAME               31234

#define AFX_IDM_WINDOW_FIRST            0xE130
#define AFX_IDM_WINDOW_LAST             0xE13F
#define AFX_IDM_FIRST_MDICHILD          0xFF00  // window list starts here

#define AFX_IDS_SCFIRST                 0xEF00
#define AFX_IDS_SCCLOSE                 0xEF06
#define AFX_IDS_MDICHILD                0xEF1F



// General error / prompt strings
#define AFX_IDP_INVALID_FILENAME        0xF100
#define AFX_IDP_FAILED_TO_OPEN_DOC      0xF101
#define AFX_IDP_FAILED_TO_SAVE_DOC      0xF102
#define AFX_IDP_ASK_TO_SAVE             0xF103
#define AFX_IDP_FAILED_TO_CREATE_DOC    0xF104
#define AFX_IDP_FILE_TOO_LARGE          0xF105
#define AFX_IDP_FAILED_TO_START_PRINT   0xF106
#define AFX_IDP_FAILED_TO_LAUNCH_HELP   0xF107
#define AFX_IDP_INTERNAL_FAILURE        0xF108      // general failure
#define AFX_IDP_COMMAND_FAILURE         0xF109      // command failure
#define AFX_IDP_FAILED_MEMORY_ALLOC     0xF10A
#define AFX_IDP_UNREG_DONE              0xF10B
#define AFX_IDP_UNREG_FAILURE           0xF10C
#define AFX_IDP_DLL_LOAD_FAILED         0xF10D
#define AFX_IDP_DLL_BAD_VERSION         0xF10E


// AFX standard accelerator resources
#define AFX_IDR_PREVIEW_ACCEL           30997


// for application title (defaults to EXE name or name in constructor)
#define AFX_IDS_APP_TITLE               0xE000 // idle message bar line
#define AFX_IDS_IDLEMESSAGE             0xE001 // message bar line when in shift-F1 help mode
#define AFX_IDS_HELPMODEMESSAGE         0xE002 // document title when editing OLE embedding
#define AFX_IDS_APP_TITLE_EMBEDDING     0xE003 // company name
#define AFX_IDS_COMPANY_NAME            0xE004 // object name when server is inplace
#define AFX_IDS_OBJ_TITLE_INPLACE       0xE005 // Application User Model ID
#define AFX_IDS_APP_ID                  0xE006

// General strings
#define AFX_IDS_OPENFILE                0xF000
#define AFX_IDS_SAVEFILE                0xF001
#define AFX_IDS_ALLFILTER               0xF002
#define AFX_IDS_UNTITLED                0xF003
#define AFX_IDS_SAVEFILECOPY            0xF004
#define AFX_IDS_PREVIEW_CLOSE           0xF005
#define AFX_IDS_UNNAMED_FILE            0xF006
#define AFX_IDS_HIDE                    0xF011


// MFC Standard Exception Error messages
#define AFX_IDP_NO_ERROR_AVAILABLE      0xF020
#define AFX_IDS_NOT_SUPPORTED_EXCEPTION 0xF021
#define AFX_IDS_RESOURCE_EXCEPTION      0xF022
#define AFX_IDS_MEMORY_EXCEPTION        0xF023
#define AFX_IDS_USER_EXCEPTION          0xF024
#define AFX_IDS_INVALID_ARG_EXCEPTION   0xF025


// Printing and print preview strings
#define AFX_IDS_PRINTONPORT             0xF040
#define AFX_IDS_ONEPAGE                 0xF041
#define AFX_IDS_TWOPAGE                 0xF042
#define AFX_IDS_PRINTPAGENUM            0xF043
#define AFX_IDS_PREVIEWPAGEDESC         0xF044
#define AFX_IDS_PRINTDEFAULTEXT         0xF045
#define AFX_IDS_PRINTDEFAULT            0xF046
#define AFX_IDS_PRINTFILTER             0xF047
#define AFX_IDS_PRINTCAPTION            0xF048
#define AFX_IDS_PRINTTOFILE             0xF049



// Standard control bars (IDW = window ID)
#define AFX_IDW_CONTROLBAR_FIRST        0xE800
#define AFX_IDW_CONTROLBAR_LAST         0xE8FF

#define AFX_IDW_TOOLBAR                 0xE800  // main Toolbar for window
#define AFX_IDW_STATUS_BAR              0xE801  // Status bar window
#define AFX_IDW_PREVIEW_BAR             0xE802  // PrintPreview Dialog Bar
#define AFX_IDW_RESIZE_BAR              0xE803  // OLE in-place resize bar
#define AFX_IDW_REBAR                   0xE804  // COMCTL32 "rebar" Bar
#define AFX_IDW_DIALOGBAR               0xE805  // CDialogBar
#define AFX_IDW_MENUBAR                 0xE806  // CMFCMenuBar

// Note: If your application supports docking toolbars, you should
//  not use the following IDs for your own toolbars.  The IDs chosen
//  are at the top of the first 32 such that the bars will be hidden
//  while in print preview mode, and are not likely to conflict with
//  IDs your application may have used succesfully in the past.

#define AFX_IDW_DOCKBAR_TOP             0xE81B
#define AFX_IDW_DOCKBAR_LEFT            0xE81C
#define AFX_IDW_DOCKBAR_RIGHT           0xE81D
#define AFX_IDW_DOCKBAR_BOTTOM          0xE81E
#define AFX_IDW_DOCKBAR_FLOAT           0xE81F

// Macro for mapping standard control bars to bitmask (limit of 32)
#define AFX_CONTROLBAR_MASK(nIDC)   (1L << (nIDC - AFX_IDW_CONTROLBAR_FIRST))

// parts of Main Frame
#define AFX_IDW_PANE_FIRST              0xE900  // first pane (256 max)
#define AFX_IDW_PANE_LAST               0xE9ff
#define AFX_IDW_HSCROLL_FIRST           0xEA00  // first Horz scrollbar (16 max)
#define AFX_IDW_VSCROLL_FIRST           0xEA10  // first Vert scrollbar (16 max)

#define AFX_IDW_SIZE_BOX                0xEA20  // size box for splitters
#define AFX_IDW_PANE_SAVE               0xEA21  // to shift AFX_IDW_PANE_FIRST




// MFC timer IDs
#define AFX_TIMER_ID_FIRST                      0xEC00
#define AFX_TIMER_ID_DISPLAY_AHWND_EVENT        0xEC00
#define AFX_TIMER_ID_AUTO_HIDE_SLIDE_IN_EVENT   0xEC01
#define AFX_TIMER_ID_AUTO_HIDE_SLIDE_OUT_EVENT  0xEC02
#define AFX_TIMER_ID_CHECK_AUTO_HIDE_CONDITION  0xEC03
#define AFX_TIMER_ID_DOCK_EVENT                 0xEC04
#define AFX_TIMER_ID_CHECK_ROLL_STATE           0xEC05
#define AFX_TIMER_ID_VISIBILITY_TIMER           0xEC06
#define AFX_TIMER_ID_ACCELERATOR_NOTIFY_EVENT   0xEC07
#define AFX_TIMER_ID_TIMER_TRACKING             0xEC08
#define AFX_TIMER_ID_TIMER_WAIT                 0xEC09  // timer while waiting to show status
#define AFX_TIMER_ID_TIMER_CHECK                0xEC0A  // timer to check for removal of status
#define AFX_TIMER_ID_TASK_ANIMATION             0xEC0B
#define AFX_TIMER_ID_TASK_SCROLL                0xEC0C
#define AFX_TIMER_ID_AUTOCOMMAND                0xEC0D
#define AFX_TIMER_ID_ALERT_CLOSE_POPUP          0xEC0E
#define AFX_TIMER_ID_ALERT_ANIMATION            0xEC0F
#define AFX_TIMER_ID_ALERT_CHECK_ACTIVITY       0xEC10
#define AFX_TIMER_ID_TB_SHOW_DROPDOWN           0xEC11
#define AFX_TIMER_ID_MENU_SHOW_ALL_ITEMS        0xEC12
#define AFX_TIMER_ID_OUTLOOK_BAR_SCRL_UP        0xEC13
#define AFX_TIMER_ID_OUTLOOK_BAR_SCRL_DN        0xEC14
#define AFX_TIMER_ID_MENU_POPUP_ANIMATION       0xEC15
#define AFX_TIMER_ID_MENU_POPUP_SCROLL          0xEC16
#define AFX_TIMER_ID_MENUBAR_POPUP              0xEC17
#define AFX_TIMER_ID_MENUBAR_REMOVE             0xEC18
#define AFX_TIMER_ID_RIBBONBAR_AUTO_COMMAND     0xEC19
#define AFX_TIMER_ID_RIBBONBAR_SHOW_KEYTIPS     0xEC1A
#define AFX_TIMER_ID_RIBBONPANEL_POPUP          0xEC1B
#define AFX_TIMER_ID_RIBBONPANEL_REMOVE         0xEC1C
#define AFX_TIMER_ID_RIBBONPANEL_AUTO_COMMAND   0xEC1D
#define AFX_TIMER_ID_LAST                       0xEC1F


#define AFX_TOOLTIP_TYPE_ALL        0xFFFF

#define AFX_TOOLTIP_TYPE_DEFAULT    0x0001
#define AFX_TOOLTIP_TYPE_TOOLBAR    0x0002
#define AFX_TOOLTIP_TYPE_TAB        0x0004
#define AFX_TOOLTIP_TYPE_MINIFRAME  0x0008
#define AFX_TOOLTIP_TYPE_DOCKBAR    0x0010
#define AFX_TOOLTIP_TYPE_EDIT       0x0020
#define	AFX_TOOLTIP_TYPE_BUTTON     0x0040
#define	AFX_TOOLTIP_TYPE_TOOLBOX    0x0080
#define AFX_TOOLTIP_TYPE_PLANNER    0x0100
#define AFX_TOOLTIP_TYPE_RIBBON     0x0200
#define	AFX_TOOLTIP_TYPE_CAPTIONBAR 0x0400

#define AFX_TOOLTIP_TYPES           11

/*============================================================================*/
// Auxiliary System/Screen metrics

typedef enum AFX_DOCK_TYPE
{
	DT_UNDEFINED = 0,    // inherit from application
	DT_IMMEDIATE = 1,    // control bar torn off immediately and follows the mouse
	DT_STANDARD = 2,    // user drags a frame
	DT_SMART = 0x80  // smart docking style
};

// Smart docking theme:
typedef enum AFX_SMARTDOCK_THEME
{
	AFX_SDT_DEFAULT = 0,	// Visual Manager-specific theme
	AFX_SDT_VS2005 = 1,		// VS 2005-style
	AFX_SDT_VS2008 = 2		// VS 2008-style
};

struct AFX_CTLCOLOR
{
	HWND hWnd;
	HDC hDC;
	UINT nCtlType;
};

enum AFX_HELP_TYPE
{
	afxWinHelp = 0,
	afxHTMLHelp = 1
};

struct AFX_NOTIFY
{
	LRESULT* pResult;
	NMHDR* pNMHDR;
};


// MFC has its own version of the TOOLINFO structure containing the
// the Win2K base version of the structure. Since MFC targets Win2K base,
// we need this structure so calls into that old library don't fail.

typedef struct tagAFX_OLDTOOLINFO {
	UINT cbSize;
	UINT uFlags;
	HWND hwnd;
	UINT uId;
	RECT rect;
	HINSTANCE hinst;
	LPTSTR lpszText;
	LPARAM lParam;
} AFX_OLDTOOLINFO;

// special struct for WM_SIZEPARENT
struct AFX_SIZEPARENTPARAMS
{
	HDWP hDWP;       // handle for DeferWindowPos
	RECT rect;       // parent client rectangle (trim as appropriate)
	SIZE sizeTotal;  // total size on each side as layout proceeds
	BOOL bStretch;   // should stretch to fill all space
};


// flags for wParam in the WM_FLOATSTATUS message
enum {
	FS_SHOW = 0x01, FS_HIDE = 0x02,
	FS_ACTIVATE = 0x04, FS_DEACTIVATE = 0x08,
	FS_ENABLE = 0x10, FS_DISABLE = 0x20,
	FS_SYNCACTIVE = 0x40
};


typedef UINT(_CDECL *AFX_THREADPROC)(LPVOID);

///////////////////////////////////////////////////////////////////////////////
// Message map macro for cracked handlers


#define MEMBER_FXN_COMPATIBLE(f) (f)
#define MEMBER_FXN_STANDARD(f) (this->*(f))

#define MEMBER_FXN(f) MEMBER_FXN_STANDARD(f)


#define DECLARE_MESSAGE_MAP() \
public: \
	virtual BOOL _ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult) ; \


//#define BEGIN_MSG_MAP_EX(theClass)
#define BEGIN_MESSAGE_MAP(theClass, baseClass) \
	/* "handled" management for cracked handlers */ \
	_INLINE BOOL theClass::_ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult) \
	{ \
		typedef theClass ThisClass;						   \
		typedef baseClass TheBaseClass;					   \

//#define END_MSG_MAP()
#define END_MESSAGE_MAP() \
		return TheBaseClass::_ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult); \
	} 


#define ON_HANDLED_VALUE TRUE

/////////////////////////////////////////////////////////////////////////////
// Command notifications for CCmdTarget notifications

struct AFX_CMDHANDLERINFO ;

struct MTL_ONCMDMSG
{
	void* pExtra ;
	AFX_CMDHANDLERINFO* pHandlerInfo ;
} ;

#define CN_COMMAND              0               // void ()
#define CN_UPDATE_COMMAND_UI    ((UINT)(-1))    // void (CCmdUI*)
#define CN_EVENT                ((UINT)(-2))    // OLE event
#define CN_OLECOMMAND           ((UINT)(-3))    // OLE document command
#define CN_OLE_UNREGISTER       ((UINT)(-4))    // OLE unregister
// > 0 are control notifications
// < 0 are for MFC's use

#define IS_MFCNOTIFYCODE(w) \
	((WORD)(-4) <= HIWORD(w)) \

#define IS_NOTIFYCODE(w, n) \
	((WORD)(n) == HIWORD(w)) \

#define IS_NOTIFYCODE_ID(w, n, id) \
	((WORD)(n) == HIWORD(w) && (id) == LOWORD(w)) \

#define IS_NOTIFYCODE_ID_RANGE(w, n, id, idLast) \
	((WORD)(n) == HIWORD(w) && (id) <= LOWORD(w) && (idLast) >= LOWORD(w)) \


#define ON_COMMAND(id, memberFxn) \
	if (uMsg == WM_COMMAND && IS_NOTIFYCODE_ID(CMDHAS_HANDLER_WPARAM_MASK(wParam), CN_COMMAND, id)) \
	{ \
		if (IS_CMDHAS_HANDLER_WPARAM(wParam)) return TRUE ; \
		MEMBER_FXN(memberFxn)() ; \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}
// ON_COMMAND(id, OnBar) is the same as
//   ON_CONTROL(0, id, OnBar) or ON_BN_CLICKED(0, id, OnBar)

#define ON_COMMAND_RANGE(id, idLast, memberFxn) \
	if (uMsg == WM_COMMAND && IS_NOTIFYCODE_ID_RANGE(CMDHAS_HANDLER_WPARAM_MASK(wParam), CN_COMMAND, id, idLast)) \
	{ \
		if (IS_CMDHAS_HANDLER_WPARAM(wParam)) return TRUE ; \
		MEMBER_FXN(memberFxn)((UINT)LOWORD(wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}
// ON_COMMAND_RANGE(id, idLast, OnBar) is the same as
//   ON_CONTROL_RANGE(0, id, idLast, OnBar)

#define ON_COMMAND_EX(id, memberFxn) \
	if (uMsg == WM_COMMAND && IS_NOTIFYCODE_ID(CMDHAS_HANDLER_WPARAM_MASK(wParam), CN_COMMAND, id)) \
	{ \
		if (IS_CMDHAS_HANDLER_WPARAM(wParam)) return TRUE ; \
		if (MEMBER_FXN(memberFxn)((UINT)LOWORD(wParam))) { \
		lResult = 0; \
		return ON_HANDLED_VALUE ; } \
	}

#define ON_COMMAND_EX_RANGE(id, idLast, memberFxn) \
	if (uMsg == WM_COMMAND && IS_NOTIFYCODE_ID_RANGE(CMDHAS_HANDLER_WPARAM_MASK(wParam), CN_COMMAND, id, idLast)) \
	{ \
		if (IS_CMDHAS_HANDLER_WPARAM(wParam)) return TRUE ; \
		if (MEMBER_FXN(memberFxn)((UINT)LOWORD(wParam))) { \
		lResult = 0; \
		return ON_HANDLED_VALUE ; } \
	}

// update ui's are listed as WM_COMMAND's so they get routed like commands
#define ON_UPDATE_COMMAND_UI(id, memberFxn) \
	if (uMsg == WM_COMMAND && IS_NOTIFYCODE_ID(wParam, CN_UPDATE_COMMAND_UI, id)) \
	{ \
		CCmdUI* pCmdUI = (CCmdUI*)((struct MTL_ONCMDMSG*)lParam)->pExtra; \
		MEMBER_FXN(memberFxn)(pCmdUI); \
		lResult = 0; \
		if (!pCmdUI->m_bContinueRouting) return ON_HANDLED_VALUE ; \
		pCmdUI->m_bContinueRouting = FALSE ; \
	}

#define ON_UPDATE_COMMAND_UI_RANGE(id, idLast, memberFxn) \
	if (uMsg == WM_COMMAND && IS_NOTIFYCODE_ID_RANGE(wParam, CN_UPDATE_COMMAND_UI, id, idLast)) \
	{ \
		CCmdUI* pCmdUI = (CCmdUI*)((struct MTL_ONCMDMSG*)lParam)->pExtra; \
		MEMBER_FXN(memberFxn)(pCmdUI); \
		lResult = 0; \
		if (!pCmdUI->m_bContinueRouting) return ON_HANDLED_VALUE ; \
		pCmdUI->m_bContinueRouting = FALSE ; \
	}

#define ON_NOTIFY(wNotifyCode, id, memberFxn) \
	if (uMsg == WM_NOTIFY && IS_NOTIFYCODE_ID(wParam, wNotifyCode, id)) \
	{ \
		AFX_NOTIFY* pNotify = (AFX_NOTIFY*)((struct MTL_ONCMDMSG*)lParam)->pExtra ; \
		MEMBER_FXN(memberFxn)(pNotify->pNMHDR, &lResult); \
		*pNotify->pResult = lResult ; \
		return ON_HANDLED_VALUE ; \
	}

#define ON_NOTIFY_RANGE(wNotifyCode, id, idLast, memberFxn) \
	if (uMsg == WM_NOTIFY && IS_NOTIFYCODE_ID_RANGE(wParam, wNotifyCode, id, idLast)) \
	{ \
		AFX_NOTIFY* pNotify = (AFX_NOTIFY*)((struct MTL_ONCMDMSG*)lParam)->pExtra ; \
		MEMBER_FXN(memberFxn)(pNotify->pNMHDR, &lResult); \
		*pNotify->pResult = lResult ; \
		return ON_HANDLED_VALUE ; \
	}

#define ON_NOTIFY_EX(wNotifyCode, id, memberFxn) \
	if (uMsg == WM_NOTIFY && IS_NOTIFYCODE_ID(wParam, wNotifyCode, id)) \
	{ \
		AFX_NOTIFY* pNotify = (AFX_NOTIFY*)((struct MTL_ONCMDMSG*)lParam)->pExtra ; \
		if (MEMBER_FXN(memberFxn)((UINT)LOWORD(wParam), pNotify->pNMHDR, &lResult)) { \
		*pNotify->pResult = lResult ; \
		return ON_HANDLED_VALUE ; } \
	}

#define ON_NOTIFY_EX_RANGE(wNotifyCode, id, idLast, memberFxn) \
	if (uMsg == WM_NOTIFY && IS_NOTIFYCODE_ID_RANGE(wParam, wNotifyCode, id, idLast)) \
	{ \
		AFX_NOTIFY* pNotify = (AFX_NOTIFY*)((struct MTL_ONCMDMSG*)lParam)->pExtra ; \
		if (MEMBER_FXN(memberFxn)((UINT)LOWORD(wParam), pNotify->pNMHDR, &lResult)) { \
		*pNotify->pResult = lResult ; \
		return ON_HANDLED_VALUE ; } \
	}

// for general controls
#define ON_CONTROL(wNotifyCode, id, memberFxn) \
	if (uMsg == WM_COMMAND && IS_NOTIFYCODE_ID(wParam, wNotifyCode, id)) \
	{ \
		MEMBER_FXN(memberFxn)(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

#define ON_CONTROL_RANGE(wNotifyCode, id, idLast, memberFxn) \
	if (uMsg == WM_COMMAND && IS_NOTIFYCODE_ID_RANGE(wParam, wNotifyCode, id, idLast)) \
	{ \
		MEMBER_FXN(memberFxn)((UINT)LOWORD(wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

#define WM_REFLECT_BASE 0xBC00

// for control notification reflection
#define ON_CONTROL_REFLECT(wNotifyCode, memberFxn) \
	if (uMsg == WM_COMMAND+WM_REFLECT_BASE && IS_NOTIFYCODE(wParam, wNotifyCode)) \
	{ \
		MEMBER_FXN(memberFxn)(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

#define ON_CONTROL_REFLECT_EX(wNotifyCode, memberFxn) \
	if (uMsg == WM_COMMAND+WM_REFLECT_BASE && IS_NOTIFYCODE(wParam, wNotifyCode)) \
	{ \
		if (MEMBER_FXN(memberFxn)()) { \
		lResult = 0 ; \
		return ON_HANDLED_VALUE ; } \
	}

#define ON_NOTIFY_REFLECT(wNotifyCode, memberFxn) \
	if (uMsg == WM_NOTIFY+WM_REFLECT_BASE && IS_NOTIFYCODE(wParam, wNotifyCode)) \
	{ \
		AFX_NOTIFY* pNotify = (AFX_NOTIFY*)((struct MTL_ONCMDMSG*)lParam)->pExtra ; \
		MEMBER_FXN(memberFxn)(pNotify->pNMHDR, &lResult); \
		*pNotify->pResult = lResult ; \
		return ON_HANDLED_VALUE ; \
	}

#define ON_NOTIFY_REFLECT_EX(wNotifyCode, memberFxn) \
	if (uMsg == WM_NOTIFY+WM_REFLECT_BASE && IS_NOTIFYCODE(wParam, wNotifyCode)) \
	{ \
		AFX_NOTIFY* pNotify = (AFX_NOTIFY*)((struct MTL_ONCMDMSG*)lParam)->pExtra ; \
		if (MEMBER_FXN(memberFxn)(pNotify->pNMHDR, &lResult)) { \
		*pNotify->pResult = lResult ; \
		return ON_HANDLED_VALUE ; } \
	}

#define ON_UPDATE_COMMAND_UI_REFLECT(memberFxn) \
	if (uMsg == WM_COMMAND+WM_REFLECT_BASE && IS_NOTIFYCODE(wParam, CN_UPDATE_COMMAND_UI)) \
	{ \
		CCmdUI* pCmdUI = (CCmdUI*)((struct MTL_ONCMDMSG*)lParam)->pExtra; \
		MEMBER_FXN(memberFxn)pCmdUI); \
		lResult = 0; \
		if (!pCmdUI->m_bContinueRouting) return ON_HANDLED_VALUE ; \
		pCmdUI->m_bContinueRouting = FALSE ; \
	}

///////////////////////////////////////////////////////////////////////////////
// Standard Windows message macros

// int OnCreate(LPCREATESTRUCT lpCreateStruct)
#define ON_WM_CREATE() \
	if (uMsg == WM_CREATE) \
	{ \
		lResult = (LRESULT)ThisClass :: OnCreate((LPCREATESTRUCT)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
#define ON_WM_INITDIALOG() \
	if (uMsg == WM_INITDIALOG) \
	{ \
		lResult = (LRESULT)ThisClass :: OnInitDialog(); \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnCopyData(CWindow wnd, PCOPYDATASTRUCT pCopyDataStruct)
#define ON_WM_COPYDATA() \
	if (uMsg == WM_COPYDATA) \
	{ \
		lResult = (LRESULT)ThisClass :: OnCopyData(FromHandle((HWND)wParam), (PCOPYDATASTRUCT)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// void OnDestroy()
#define ON_WM_DESTROY() \
	if (uMsg == WM_DESTROY) \
	{ \
		ThisClass :: OnDestroy(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnMove(CPoint ptPos)
#define ON_WM_MOVE() \
	if (uMsg == WM_MOVE) \
	{ \
		ThisClass :: OnMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnSize(UINT nType, CSize size)
#define ON_WM_SIZE() \
	if (uMsg == WM_SIZE) \
	{ \
		ThisClass :: OnSize((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther)
#define ON_WM_ACTIVATE() \
	if (uMsg == WM_ACTIVATE) \
	{ \
		ThisClass :: OnActivate((UINT)LOWORD(wParam), FromHandle((HWND)lParam), (BOOL)HIWORD(wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnSetFocus(CWindow wndOld)
#define ON_WM_SETFOCUS() \
	if (uMsg == WM_SETFOCUS) \
	{ \
		ThisClass :: OnSetFocus(FromHandle((HWND)wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnKillFocus(CWindow wndFocus)
#define ON_WM_KILLFOCUS() \
	if (uMsg == WM_KILLFOCUS) \
	{ \
		ThisClass :: OnKillFocus(FromHandle((HWND)wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnEnable(BOOL bEnable)
#define ON_WM_ENABLE() \
	if (uMsg == WM_ENABLE) \
	{ \
		ThisClass :: OnEnable((BOOL)wParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnPaint(CDCHandle dc)
#define ON_WM_PAINT() \
	if (uMsg == WM_PAINT) \
	{ \
		ThisClass :: OnPaint(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnClose()
#define ON_WM_CLOSE() \
	if (uMsg == WM_CLOSE) \
	{ \
		ThisClass :: OnClose(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnQueryEndSession(UINT nSource, UINT uLogOff)
#define ON_WM_QUERYENDSESSION() \
	if (uMsg == WM_QUERYENDSESSION) \
	{ \
		lResult = (LRESULT)ThisClass :: OnQueryEndSession(); \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnQueryOpen()
#define ON_WM_QUERYOPEN() \
	if (uMsg == WM_QUERYOPEN) \
	{ \
		lResult = (LRESULT)ThisClass :: OnQueryOpen(); \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnEraseBkgnd(CDCHandle dc)
#define ON_WM_ERASEBKGND() \
	if (uMsg == WM_ERASEBKGND) \
	{ \
		lResult = (LRESULT)ThisClass :: OnEraseBkgnd(&CDC((HDC)wParam)); \
		return ON_HANDLED_VALUE ; \
	}

#define ON_WM_PRINTCLIENT() \
	if (uMsg == WM_PRINTCLIENT) \
	{ \
		lResult = ThisClass :: OnPrintClient(&CDC((HDC)wParam), (UINT)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// void OnSysColorChange()
#define ON_WM_SYSCOLORCHANGE() \
	if (uMsg == WM_SYSCOLORCHANGE) \
	{ \
		ThisClass :: OnSysColorChange(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnEndSession(BOOL bEnding, UINT uLogOff)
#define ON_WM_ENDSESSION() \
	if (uMsg == WM_ENDSESSION) \
	{ \
		ThisClass :: OnEndSession((BOOL)wParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// int OnSetText(LPCTSTR lpstrText)
#define ON_WM_SETTEXT() \
	if (uMsg == WM_SETTEXT) \
	{ \
		lResult = (LRESULT)ThisClass :: OnSetText((LPCTSTR)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// int OnGetText(int cchTextMax, LPTSTR lpszText)
#define ON_WM_GETTEXT() \
	if (uMsg == WM_GETTEXT) \
	{ \
		lResult = (LRESULT)ThisClass :: OnGetText((int)wParam, (LPTSTR)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// int OnGetTextLength()
#define ON_WM_GETTEXTLENGTH() \
	if (uMsg == WM_GETTEXTLENGTH) \
	{ \
		lResult = (LRESULT)ThisClass :: OnGetTextLength(); \
		return ON_HANDLED_VALUE ; \
	}

// void OnSetFont(CFontHandle font, BOOL bRedraw)
#define ON_WM_SETFONT() \
	if (uMsg == WM_SETFONT) \
	{ \
		ThisClass :: OnSetFont(&CFont((HFONT)wParam), (BOOL)LOWORD(lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// HFONT OnGetFont()
#define ON_WM_GETFONT() \
	if (uMsg == WM_GETFONT) \
	{ \
		lResult = (LRESULT)ThisClass :: OnGetFont(); \
		return ON_HANDLED_VALUE ; \
	}

// HICON OnSetIcon(UINT uType, HICON hIcon)
#define ON_WM_SETICON() \
	if (uMsg == WM_SETICON) \
	{ \
		lResult = (LRESULT)ThisClass :: OnSetIcon((BOOL)wParam, (HICON)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// void OnCut()
#define ON_WM_CUT() \
	if (uMsg == WM_CUT) \
	{ \
		ThisClass :: OnCut(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnCopy()
#define ON_WM_COPY() \
	if (uMsg == WM_COPY) \
	{ \
		ThisClass :: OnCopy(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnPaste()
#define ON_WM_PASTE() \
	if (uMsg == WM_PASTE) \
	{ \
		ThisClass :: OnPaste(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnClear()
#define ON_WM_CLEAR() \
	if (uMsg == WM_CLEAR) \
	{ \
		ThisClass :: OnClear(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnDisplayChange(UINT uBitsPerPixel, CSize sizeScreen)
#define ON_WM_DISPLAYCHANGE() \
	if (uMsg == WM_DISPLAYCHANGE) \
	{ \
		ThisClass :: OnDisplayChange((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnWTSSessionChange(WPARAM nStatusCode, PWTSSESSION_NOTIFICATION nSessionID)
#define ON_WM_WTSSESSION_CHANGE() \
	if (uMsg == WM_WTSSESSION_CHANGE) \
	{ \
		ThisClass :: OnSessionChange(wParam, (PWTSSESSION_NOTIFICATION)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnShowWindow(BOOL bShow, UINT nStatus)
#define ON_WM_SHOWWINDOW() \
	if (uMsg == WM_SHOWWINDOW) \
	{ \
		ThisClass :: OnShowWindow((BOOL)wParam, (UINT)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

#if 0
// HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit)
#define ON_WM_CTLCOLOREDIT() \
	if (uMsg == WM_CTLCOLOREDIT) \
	{ \
		lResult = (LRESULT)ThisClass :: func(FromHandle((HDC)wParam), FromHandle((HWND)lParam)); \
		return ON_HANDLED_VALUE ; \
	}

// HBRUSH OnCtlColorListBox(CDCHandle dc, CListBox listBox)
#define ON_WM_CTLCOLORLISTBOX() \
	if (uMsg == WM_CTLCOLORLISTBOX) \
	{ \
		lResult = (LRESULT)ThisClass :: func(FromHandle((HDC)wParam), FromHandle((HWND)lParam)); \
		return ON_HANDLED_VALUE ; \
	}

// HBRUSH OnCtlColorBtn(CDCHandle dc, CButton button)
#define ON_WM_CTLCOLORBTN() \
	if (uMsg == WM_CTLCOLORBTN) \
	{ \
		lResult = (LRESULT)ThisClass :: func(FromHandle((HDC)wParam), FromHandle((HWND)lParam)); \
		return ON_HANDLED_VALUE ; \
	}

// HBRUSH OnCtlColorDlg(CDCHandle dc, CWindow wnd)
#define ON_WM_CTLCOLORDLG() \
	if (uMsg == WM_CTLCOLORDLG) \
	{ \
		lResult = (LRESULT)ThisClass :: func(FromHandle((HDC)wParam), FromHandle((HWND)lParam)); \
		return ON_HANDLED_VALUE ; \
	}

// HBRUSH OnCtlColorScrollBar(CDCHandle dc, CScrollBar scrollBar)
#define ON_WM_CTLCOLORSCROLLBAR() \
	if (uMsg == WM_CTLCOLORSCROLLBAR) \
	{ \
		lResult = (LRESULT)ThisClass :: func(FromHandle((HDC)wParam), FromHandle((HWND)lParam)); \
		return ON_HANDLED_VALUE ; \
	}

// HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic)
#define ON_WM_CTLCOLORSTATIC() \
	if (uMsg == WM_CTLCOLORSTATIC) \
	{ \
		lResult = (LRESULT)ThisClass :: func(FromHandle((HDC)wParam), FromHandle((HWND)lParam)); \
		return ON_HANDLED_VALUE ; \
	}
#endif

#define ON_WM_CTLCOLOR() \
	if ((uMsg == WM_CTLCOLOR) || ((WM_CTLCOLORMSGBOX <= uMsg) && (uMsg <= WM_CTLCOLORSTATIC))) \
	{ \
		UINT nCtlType = (uMsg == WM_CTLCOLOR) ? (UINT)-1 : uMsg - WM_CTLCOLORMSGBOX ; \
		lResult = (LRESULT)ThisClass :: OnCtlColor(&CDC((HDC)wParam), CWnd::FromHandle((HWND)lParam, NULL, m_hWnd), nCtlType); \
		return ON_HANDLED_VALUE ; \
	}

#define ON_WM_CTLCOLOR_REFLECT() \
	if ((uMsg == WM_CTLCOLOR+WM_REFLECT_BASE) || ((WM_CTLCOLORMSGBOX+WM_REFLECT_BASE <= uMsg) && (uMsg <= WM_CTLCOLORSTATIC+WM_REFLECT_BASE))) \
	{ \
		HDC hDC = (uMsg == WM_CTLCOLOR+WM_REFLECT_BASE) ? ((AFX_CTLCOLOR*)lParam)->hDC : (HDC)wParam ; \
		UINT nCtlType = (uMsg == WM_CTLCOLOR + WM_REFLECT_BASE) ? ((AFX_CTLCOLOR*)lParam)->nCtlType : uMsg - (WM_CTLCOLORMSGBOX + WM_REFLECT_BASE); \
		lResult = (LRESULT)ThisClass :: CtlColor(&CDC(hDC), nCtlType); \
		return ON_HANDLED_VALUE ; \
	}

// void OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
#define ON_WM_SETTINGCHANGE() \
	if (uMsg == WM_SETTINGCHANGE) \
	{ \
		ThisClass :: OnSettingChange((UINT)wParam, (LPCTSTR)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnDevModeChange(LPCTSTR lpDeviceName)
#define ON_WM_DEVMODECHANGE() \
	if (uMsg == WM_DEVMODECHANGE) \
	{ \
		ThisClass :: OnDevModeChange((LPCTSTR)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnActivateApp(BOOL bActive, DWORD dwThreadID)
#define ON_WM_ACTIVATEAPP() \
	if (uMsg == WM_ACTIVATEAPP) \
	{ \
		ThisClass :: OnActivateApp((BOOL)wParam, (DWORD)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnFontChange()
#define ON_WM_FONTCHANGE() \
	if (uMsg == WM_FONTCHANGE) \
	{ \
		ThisClass :: OnFontChange(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnTimeChange()
#define ON_WM_TIMECHANGE() \
	if (uMsg == WM_TIMECHANGE) \
	{ \
		ThisClass :: OnTimeChange(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnCancelMode()
#define ON_WM_CANCELMODE() \
	if (uMsg == WM_CANCELMODE) \
	{ \
		ThisClass :: OnCancelMode(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
#define ON_WM_SETCURSOR() \
	if (uMsg == WM_SETCURSOR) \
	{ \
		lResult = (LRESULT)ThisClass :: OnSetCursor(FromHandle((HWND)wParam, NULL, m_hWnd), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
		return ON_HANDLED_VALUE ; \
	}

// int OnMouseActivate(CWindow wndTopLevel, UINT nHitTest, UINT message)
#define ON_WM_MOUSEACTIVATE() \
	if (uMsg == WM_MOUSEACTIVATE) \
	{ \
		lResult = (LRESULT)ThisClass :: OnMouseActivate(FromHandle((HWND)wParam, NULL, m_hWnd), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
		return ON_HANDLED_VALUE ; \
	}

// void OnChildActivate()
#define ON_WM_CHILDACTIVATE() \
	if (uMsg == WM_CHILDACTIVATE) \
	{ \
		ThisClass :: OnChildActivate(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnGetMinMaxInfo(LPMINMAXINFO lpMMI)
#define ON_WM_GETMINMAXINFO() \
	if (uMsg == WM_GETMINMAXINFO) \
	{ \
		ThisClass :: OnGetMinMaxInfo((LPMINMAXINFO)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnIconEraseBkgnd(CDCHandle dc)
#define ON_WM_ICONERASEBKGND() \
	if (uMsg == WM_ICONERASEBKGND) \
	{ \
		ThisClass :: OnIconEraseBkgnd(&CDC((HDC)wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnSpoolerStatus(UINT nStatus, UINT nJobs)
#define ON_WM_SPOOLERSTATUS() \
	if (uMsg == WM_SPOOLERSTATUS) \
	{ \
		ThisClass :: OnSpoolerStatus((UINT)wParam, (UINT)LOWORD(lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
#define ON_WM_DRAWITEM() \
	if (uMsg == WM_DRAWITEM) \
	{ \
		ThisClass :: OnDrawItem((INT)wParam, (LPDRAWITEMSTRUCT)lParam); \
		lResult = TRUE; \
		return ON_HANDLED_VALUE ; \
	}

#define ON_WM_DRAWITEM_REFLECT() \
	if (uMsg == WM_DRAWITEM+WM_REFLECT_BASE) \
	{ \
		ThisClass :: DrawItem((LPDRAWITEMSTRUCT)lParam); \
		lResult = TRUE; \
		return ON_HANDLED_VALUE ; \
	}

// void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
#define ON_WM_MEASUREITEM() \
	if (uMsg == WM_MEASUREITEM) \
	{ \
		ThisClass :: OnMeasureItem((INT)wParam, (LPMEASUREITEMSTRUCT)lParam); \
		lResult = TRUE; \
		return ON_HANDLED_VALUE ; \
	}

#define ON_WM_MEASUREITEM_REFLECT() \
	if (uMsg == WM_MEASUREITEM+WM_REFLECT_BASE) \
	{ \
		ThisClass :: MeasureItem((LPMEASUREITEMSTRUCT)lParam); \
		lResult = TRUE; \
		return ON_HANDLED_VALUE ; \
	}

// void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct)
#define ON_WM_DELETEITEM() \
	if (uMsg == WM_DELETEITEM) \
	{ \
		ThisClass :: OnDeleteItem((INT)wParam, (LPDELETEITEMSTRUCT)lParam); \
		lResult = TRUE; \
		return ON_HANDLED_VALUE ; \
	}

#define ON_WM_DELETEITEM_REFLECT() \
	if (uMsg == WM_DELETEITEM+WM_REFLECT_BASE) \
	{ \
		ThisClass :: DeleteItem((LPDELETEITEMSTRUCT)lParam); \
		lResult = TRUE; \
		return ON_HANDLED_VALUE ; \
	}

//int OnCharToItem(UINT nChar, UINT nIndex, CListBox listBox)
#define ON_WM_CHARTOITEM() \
	if (uMsg == WM_CHARTOITEM) \
	{ \
		lResult = (LRESULT)ThisClass :: OnCharToItem((UINT)LOWORD(wParam), (CListBox*)FromHandle((HWND)lParam, NULL, m_hWnd), (UINT)HIWORD(wParam)); \
		return ON_HANDLED_VALUE ; \
	}

#define ON_WM_CHARTOITEM_REFLECT() \
	if (uMsg == WM_CHARTOITEM+WM_REFLECT_BASE) \
	{ \
		lResult = (LRESULT)ThisClass :: CharToItem((UINT)LOWORD(wParam), (UINT)HIWORD(wParam)); \
		return ON_HANDLED_VALUE ; \
	}

// int OnVKeyToItem(UINT nKey, UINT nIndex, CListBox listBox)
#define ON_WM_VKEYTOITEM() \
	if (uMsg == WM_VKEYTOITEM) \
	{ \
		lResult = (LRESULT)ThisClass :: OnVKeyToItem((UINT)LOWORD(wParam), (CListBox*)FromHandle((HWND)lParam, NULL, m_hWnd), (UINT)HIWORD(wParam)); \
		return ON_HANDLED_VALUE ; \
	}

#define ON_WM_VKEYTOITEM_REFLECT() \
	if (uMsg == WM_VKEYTOITEM+WM_REFLECT_BASE) \
	{ \
		lResult = (LRESULT)ThisClass :: VKeyToItem((UINT)LOWORD(wParam), (UINT)HIWORD(wParam)); \
		return ON_HANDLED_VALUE ; \
	}

// HCURSOR OnQueryDragIcon()
#define ON_WM_QUERYDRAGICON() \
	if (uMsg == WM_QUERYDRAGICON) \
	{ \
		lResult = (LRESULT)ThisClass :: OnQueryDragIcon(); \
		return ON_HANDLED_VALUE ; \
	}

// int OnCompareItem(int nIDCtl, LPCOMPAREITEMSTRUCT lpCompareItemStruct)
#define ON_WM_COMPAREITEM() \
	if (uMsg == WM_COMPAREITEM) \
	{ \
		lResult = (LRESULT)ThisClass :: OnCompareItem((INT)wParam, (LPCOMPAREITEMSTRUCT)lParam); \
		return ON_HANDLED_VALUE ; \
	}

#define ON_WM_COMPAREITEM_REFLECT() \
	if (uMsg == WM_COMPAREITEM+WM_REFLECT_BASE) \
	{ \
		lResult = (LRESULT)ThisClass :: CompareItem((LPCOMPAREITEMSTRUCT)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// void OnCompacting(UINT nCpuTime)
#define ON_WM_COMPACTING() \
	if (uMsg == WM_COMPACTING) \
	{ \
		ThisClass :: OnCompacting((UINT)wParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct)
#define ON_WM_NCCREATE() \
	if (uMsg == WM_NCCREATE) \
	{ \
		lResult = (LRESULT)ThisClass :: OnNcCreate((LPCREATESTRUCT)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// void OnNcDestroy()
#define ON_WM_NCDESTROY() \
	if (uMsg == WM_NCDESTROY) \
	{ \
		ThisClass :: OnNcDestroy(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
#define ON_WM_NCCALCSIZE() \
	if (uMsg == WM_NCCALCSIZE) \
	{ \
		ThisClass :: OnNcCalcSize((BOOL)wParam, (NCCALCSIZE_PARAMS*)lParam); \
		lResult = 0 ; \
		return ON_HANDLED_VALUE ; \
	}

// UINT OnNcHitTest(CPoint point)
#define ON_WM_NCHITTEST() \
	if (uMsg == WM_NCHITTEST) \
	{ \
		lResult = (LRESULT)ThisClass :: OnNcHitTest(CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		return ON_HANDLED_VALUE ; \
	}

// void OnNcPaint(CRgnHandle rgn)
#define ON_WM_NCPAINT() \
	if (uMsg == WM_NCPAINT) \
	{ \
		ThisClass :: OnNcPaint(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnNcActivate(BOOL bActive)
#define ON_WM_NCACTIVATE() \
	if (uMsg == WM_NCACTIVATE) \
	{ \
		lResult = (LRESULT)ThisClass :: OnNcActivate((BOOL)wParam); \
		return ON_HANDLED_VALUE ; \
	}

// UINT OnGetDlgCode(LPMSG lpMsg)
#define ON_WM_GETDLGCODE() \
	if (uMsg == WM_GETDLGCODE) \
	{ \
		lResult = (LRESULT)ThisClass :: OnGetDlgCode(); \
		return ON_HANDLED_VALUE ; \
	}

// void OnNcMouseMove(UINT nHitTest, CPoint point)
#define ON_WM_NCMOUSEMOVE() \
	if (uMsg == WM_NCMOUSEMOVE) \
	{ \
		ThisClass :: OnNcMouseMove((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnNcLButtonDown(UINT nHitTest, CPoint point)
#define ON_WM_NCLBUTTONDOWN() \
	if (uMsg == WM_NCLBUTTONDOWN) \
	{ \
		ThisClass :: OnNcLButtonDown((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnNcLButtonUp(UINT nHitTest, CPoint point)
#define ON_WM_NCLBUTTONUP() \
	if (uMsg == WM_NCLBUTTONUP) \
	{ \
		ThisClass :: OnNcLButtonUp((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnNcLButtonDblClk(UINT nHitTest, CPoint point)
#define ON_WM_NCLBUTTONDBLCLK() \
	if (uMsg == WM_NCLBUTTONDBLCLK) \
	{ \
		ThisClass :: OnNcLButtonDblClk((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnNcRButtonDown(UINT nHitTest, CPoint point)
#define ON_WM_NCRBUTTONDOWN() \
	if (uMsg == WM_NCRBUTTONDOWN) \
	{ \
		ThisClass :: OnNcRButtonDown((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnNcRButtonUp(UINT nHitTest, CPoint point)
#define ON_WM_NCRBUTTONUP() \
	if (uMsg == WM_NCRBUTTONUP) \
	{ \
		ThisClass :: OnNcRButtonUp((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnNcRButtonDblClk(UINT nHitTest, CPoint point)
#define ON_WM_NCRBUTTONDBLCLK() \
	if (uMsg == WM_NCRBUTTONDBLCLK) \
	{ \
		ThisClass :: OnNcRButtonDblClk((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnNcMButtonDown(UINT nHitTest, CPoint point)
#define ON_WM_NCMBUTTONDOWN() \
	if (uMsg == WM_NCMBUTTONDOWN) \
	{ \
		ThisClass :: OnNcMButtonDown((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnNcMButtonUp(UINT nHitTest, CPoint point)
#define ON_WM_NCMBUTTONUP() \
	if (uMsg == WM_NCMBUTTONUP) \
	{ \
		ThisClass :: OnNcMButtonUp((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnNcMButtonDblClk(UINT nHitTest, CPoint point)
#define ON_WM_NCMBUTTONDBLCLK() \
	if (uMsg == WM_NCMBUTTONDBLCLK) \
	{ \
		ThisClass :: OnNcMButtonDblClk((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnNCXButtonDown(int fwButton, short nHittest, CPoint ptPos)
#define ON_WM_NCXBUTTONDOWN() \
	if (uMsg == WM_NCXBUTTONDOWN) \
	{ \
		ThisClass :: OnNcXButtonDown(GET_XBUTTON_WPARAM(wParam), GET_NCHITTEST_WPARAM(wParam), CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnNCXButtonUp(int fwButton, short nHittest, CPoint ptPos)
#define ON_WM_NCXBUTTONUP() \
	if (uMsg == WM_NCXBUTTONUP) \
	{ \
		ThisClass :: OnNcXButtonUp(GET_XBUTTON_WPARAM(wParam), GET_NCHITTEST_WPARAM(wParam), CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnNCXButtonDblClk(int fwButton, short nHittest, CPoint ptPos)
#define ON_WM_NCXBUTTONDBLCLK() \
	if (uMsg == WM_NCXBUTTONDBLCLK) \
	{ \
		ThisClass :: OnNcXButtonDblClk(GET_XBUTTON_WPARAM(wParam), GET_NCHITTEST_WPARAM(wParam), CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
#define ON_WM_KEYDOWN() \
	if (uMsg == WM_KEYDOWN) \
	{ \
		ThisClass :: OnKeyDown((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
#define ON_WM_KEYUP() \
	if (uMsg == WM_KEYUP) \
	{ \
		ThisClass :: OnKeyUp((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnHotKey(int nHotKeyID, UINT uModifiers, UINT uVirtKey)
#define ON_WM_HOTKEY() \
	if (uMsg == WM_HOTKEY) \
	{ \
		ThisClass :: OnHotKey((UINT)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define ON_WM_CHAR() \
	if (uMsg == WM_CHAR) \
	{ \
		ThisClass :: OnChar((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnUniChar(TCHAR nChar, UINT nRepCnt, UINT nFlags)
#define ON_WM_UNICHAR() \
	if (uMsg == WM_UNICHAR) \
	{ \
		ThisClass :: OnUniChar((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		return ON_HANDLED_VALUE ; \
		{ \
			lResult = (wParam == UNICODE_NOCHAR) ? TRUE : FALSE; \
		} \
	}

// void OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define ON_WM_DEADCHAR() \
	if (uMsg == WM_DEADCHAR) \
	{ \
		ThisClass :: OnDeadChar((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
#define ON_WM_SYSKEYDOWN() \
	if (uMsg == WM_SYSKEYDOWN) \
	{ \
		ThisClass :: OnSysKeyDown((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
#define ON_WM_SYSKEYUP() \
	if (uMsg == WM_SYSKEYUP) \
	{ \
		ThisClass :: OnSysKeyUp((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define ON_WM_SYSCHAR() \
	if (uMsg == WM_SYSCHAR) \
	{ \
		ThisClass :: OnSysChar((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags)
#define ON_WM_SYSDEADCHAR() \
	if (uMsg == WM_SYSDEADCHAR) \
	{ \
		ThisClass :: OnSysDeadChar((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnSysCommand(UINT nID, CPoint point)
#define ON_WM_SYSCOMMAND() \
	if (uMsg == WM_SYSCOMMAND) \
	{ \
		ThisClass :: OnSysCommand((UINT)wParam, lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnInputLangChange(DWORD dwCharSet, HKL hKbdLayout)
#define ON_WM_INPUTLANGCHANGE() \
	if (uMsg == WM_INPUTLANGCHANGE) \
	{ \
		ThisClass :: OnInputLangChange((UINT)wParam, (UINT)lParam); \
		lResult = TRUE; \
		return ON_HANDLED_VALUE ; \
	}

// void OnInputLangChangeRequest(BOOL bSysCharSet, HKL hKbdLayout)
#define ON_WM_INPUTLANGCHANGEREQUEST() \
	if (uMsg == WM_INPUTLANGCHANGEREQUEST) \
	{ \
		ThisClass :: OnInputLangChangeRequest((UINT)wParam, (UINT)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnAppCommand(CWindow wndFocus, short cmd, WORD uDevice, int dwKeys)
#define ON_WM_APPCOMMAND() \
	if (uMsg == WM_APPCOMMAND) \
	{ \
		lResult = (LRESULT)ThisClass :: OnAppCommand(FromHandle((HWND)wParam, NULL, m_hWnd), (UINT)GET_APPCOMMAND_LPARAM(lParam), (UINT)GET_DEVICE_LPARAM(lParam), (UINT)GET_KEYSTATE_LPARAM(lParam)); \
		return ON_HANDLED_VALUE ; \
	}

// void OnInput(WPARAM RawInputCode, HRAWINPUT hRawInput)
#define ON_WM_INPUT() \
	if (uMsg == WM_INPUT) \
	{ \
		ThisClass :: OnRawInput((UINT)GET_RAWINPUT_CODE_WPARAM(wParam), (HRAWINPUT)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnTCard(UINT idAction, DWORD dwActionData)
#define ON_WM_TCARD() \
	if (uMsg == WM_TCARD) \
	{ \
		ThisClass :: OnTCard((UINT)wParam, (DWORD)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnTimer(UINT_PTR nIDEvent)
#define ON_WM_TIMER() \
	if (uMsg == WM_TIMER) \
	{ \
		ThisClass :: OnTimer((UINT_PTR)wParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
#define ON_WM_HSCROLL() \
	if (uMsg == WM_HSCROLL) \
	{ \
		ThisClass :: OnHScroll((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (CScrollBar*)FromHandle((HWND)lParam, NULL, m_hWnd)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

#define ON_WM_HSCROLL_REFLECT() \
	if (uMsg == WM_HSCROLL+WM_REFLECT_BASE) \
	{ \
		ThisClass :: HScroll((UINT)LOWORD(wParam), (UINT)HIWORD(wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
#define ON_WM_VSCROLL() \
	if (uMsg == WM_VSCROLL) \
	{ \
		ThisClass :: OnVScroll((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (CScrollBar*)FromHandle((HWND)lParam, NULL, m_hWnd)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

#define ON_WM_VSCROLL_REFLECT() \
	if (uMsg == WM_VSCROLL+WM_REFLECT_BASE) \
	{ \
		ThisClass :: VScroll((UINT)LOWORD(wParam), (UINT)HIWORD(wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnInitMenu(CMenuHandle menu)
#define ON_WM_INITMENU() \
	if (uMsg == WM_INITMENU) \
	{ \
		ThisClass :: OnInitMenu(&CMenu((HMENU)wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnInitMenuPopup(CMenuHandle menuPopup, UINT nIndex, BOOL bSysMenu)
#define ON_WM_INITMENUPOPUP() \
	if (uMsg == WM_INITMENUPOPUP) \
	{ \
		ThisClass :: OnInitMenuPopup(&CMenu((HMENU)wParam), (UINT)LOWORD(lParam), (BOOL)HIWORD(lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnMenuSelect(UINT nItemID, UINT nFlags, CMenuHandle menu)
#define ON_WM_MENUSELECT() \
	if (uMsg == WM_MENUSELECT) \
	{ \
		ThisClass :: OnMenuSelect((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HMENU)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenuHandle menu)
#define ON_WM_MENUCHAR() \
	if (uMsg == WM_MENUCHAR) \
	{ \
		lResult = ThisClass :: OnMenuChar((TCHAR)LOWORD(wParam), (UINT)HIWORD(wParam), &CMenu((HMENU)wParam)); \
		return ON_HANDLED_VALUE ; \
	}

// void OnMenuRButtonUp(WPARAM wParam, CMenuHandle menu)
#define ON_WM_MENURBUTTONUP() \
	if (uMsg == WM_MENURBUTTONUP) \
	{ \
		ThisClass :: OnMenuRButtonUp((UINT)wParam, &CMenu((HMENU)wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// LRESULT OnMenuDrag(WPARAM wParam, CMenuHandle menu)
#define ON_WM_MENUDRAG() \
	if (uMsg == WM_MENUDRAG) \
	{ \
		lResult = ThisClass :: OnMenuDrag((UINT)wParam, &CMenu((HMENU)wParam)); \
		return ON_HANDLED_VALUE ; \
	}

// LRESULT OnMenuGetObject(PMENUGETOBJECTINFO info)
#define ON_WM_MENUGETOBJECT() \
	if (uMsg == WM_MENUGETOBJECT) \
	{ \
		lResult = ThisClass :: OnMenuGetObject((PMENUGETOBJECTINFO)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// void OnUnInitMenuPopup(UINT nID, CMenuHandle menu)
#define ON_WM_UNINITMENUPOPUP() \
	if (uMsg == WM_UNINITMENUPOPUP) \
	{ \
		ThisClass :: OnUnInitMenuPopup(&CMenu((HMENU)wParam), (UINT)HIWORD(lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnNextMenu(int nVirtKey, LPMDINEXTMENU lpMdiNextMenu)
#define ON_WM_NEXTMENU() \
	if (uMsg == WM_NEXTMENU) \
	{ \
		ThisClass :: OnNextMenu((UINT)wParam, (LPMDINEXTMENU)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnEnterIdle(UINT nWhy, CWindow wndWho)
#define ON_WM_ENTERIDLE() \
	if (uMsg == WM_ENTERIDLE) \
	{ \
		ThisClass :: OnEnterIdle((UINT)wParam, FromHandle((HWND)lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnMouseMove(UINT nFlags, CPoint point)
#define ON_WM_MOUSEMOVE() \
	if (uMsg == WM_MOUSEMOVE) \
	{ \
		ThisClass :: OnMouseMove((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnMouseHover(WPARAM wParam, CPoint ptPos)
#define ON_WM_MOUSEHOVER() \
	if (uMsg == WM_MOUSEHOVER) \
	{ \
		ThisClass :: OnMouseHover((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnMouseLeave()
#define ON_WM_MOUSELEAVE() \
	if (uMsg == WM_MOUSELEAVE) \
	{ \
		ThisClass :: OnMouseLeave(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
#define ON_WM_MOUSEWHEEL() \
	if (uMsg == WM_MOUSEWHEEL) \
	{ \
		lResult = (LRESULT)ThisClass :: OnMouseWheel((UINT)LOWORD(wParam), (short)HIWORD(wParam), CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
#define ON_WM_MOUSEHWHEEL() \
	if (uMsg == WM_MOUSEHWHEEL) \
	{ \
		lResult = (LRESULT)ThisClass :: OnMouseHWheel((UINT)LOWORD(wParam), (short)HIWORD(wParam), CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		return ON_HANDLED_VALUE ; \
	}

// void OnLButtonDown(UINT nFlags, CPoint point)
#define ON_WM_LBUTTONDOWN() \
	if (uMsg == WM_LBUTTONDOWN) \
	{ \
		ThisClass :: OnLButtonDown((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnLButtonUp(UINT nFlags, CPoint point)
#define ON_WM_LBUTTONUP() \
	if (uMsg == WM_LBUTTONUP) \
	{ \
		ThisClass :: OnLButtonUp((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnLButtonDblClk(UINT nFlags, CPoint point)
#define ON_WM_LBUTTONDBLCLK() \
	if (uMsg == WM_LBUTTONDBLCLK) \
	{ \
		ThisClass :: OnLButtonDblClk((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnRButtonDown(UINT nFlags, CPoint point)
#define ON_WM_RBUTTONDOWN() \
	if (uMsg == WM_RBUTTONDOWN) \
	{ \
		ThisClass :: OnRButtonDown((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnRButtonUp(UINT nFlags, CPoint point)
#define ON_WM_RBUTTONUP() \
	if (uMsg == WM_RBUTTONUP) \
	{ \
		ThisClass :: OnRButtonUp((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnRButtonDblClk(UINT nFlags, CPoint point)
#define ON_WM_RBUTTONDBLCLK() \
	if (uMsg == WM_RBUTTONDBLCLK) \
	{ \
		ThisClass :: OnRButtonDblClk((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnMButtonDown(UINT nFlags, CPoint point)
#define ON_WM_MBUTTONDOWN() \
	if (uMsg == WM_MBUTTONDOWN) \
	{ \
		ThisClass :: OnMButtonDown((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnMButtonUp(UINT nFlags, CPoint point)
#define ON_WM_MBUTTONUP() \
	if (uMsg == WM_MBUTTONUP) \
	{ \
		ThisClass :: OnMButtonUp((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnMButtonDblClk(UINT nFlags, CPoint point)
#define ON_WM_MBUTTONDBLCLK() \
	if (uMsg == WM_MBUTTONDBLCLK) \
	{ \
		ThisClass :: OnMButtonDblClk((UINT)wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnXButtonDown(int fwButton, int dwKeys, CPoint ptPos)
#define ON_WM_XBUTTONDOWN() \
	if (uMsg == WM_XBUTTONDOWN) \
	{ \
		ThisClass :: OnXButtonDown(GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam), CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnXButtonUp(int fwButton, int dwKeys, CPoint ptPos)
#define ON_WM_XBUTTONUP() \
	if (uMsg == WM_XBUTTONUP) \
	{ \
		ThisClass :: OnXButtonUp(GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam), CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnXButtonDblClk(int fwButton, int dwKeys, CPoint ptPos)
#define ON_WM_XBUTTONDBLCLK() \
	if (uMsg == WM_XBUTTONDBLCLK) \
	{ \
		ThisClass :: OnXButtonDblClk(GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam), CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnParentNotify(UINT message, UINT nChildID, LPARAM lParam)
#define ON_WM_PARENTNOTIFY() \
	if (uMsg == WM_PARENTNOTIFY) \
	{ \
		ThisClass :: OnParentNotify((UINT)LOWORD(wParam), (LPARAM)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

#define ON_WM_PARENTNOTIFY_REFLECT() \
	if (uMsg == WM_PARENTNOTIFY+WM_REFLECT_BASE) \
	{ \
		ThisClass :: ParentNotify((UINT)LOWORD(wParam), (LPARAM)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// int OnNotifyFormat(CWindow wndFrom, int nCommand)
#define ON_WM_NOTIFYFORMAT() \
	if (uMsg == WM_NOTIFYFORMAT) \
	{ \
		lResult = (LRESULT)ThisClass :: OnNotifyFormat(FromHandle((HWND)wParam), (UINT)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// void OnMDIActivate(CWindow wndActivate, CWindow wndDeactivate)
#define ON_WM_MDIACTIVATE() \
	if (uMsg == WM_MDIACTIVATE) \
	{ \
		ThisClass :: OnMDIActivate((hWnd == (HWND)lParam), FromHandle((HWND)lParam), FromHandle((HWND)wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnRenderFormat(UINT nFormat)
#define ON_WM_RENDERFORMAT() \
	if (uMsg == WM_RENDERFORMAT) \
	{ \
		ThisClass :: OnRenderFormat((UINT)wParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnRenderAllFormats()
#define ON_WM_RENDERALLFORMATS() \
	if (uMsg == WM_RENDERALLFORMATS) \
	{ \
		ThisClass :: OnRenderAllFormats(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnDestroyClipboard()
#define ON_WM_DESTROYCLIPBOARD() \
	if (uMsg == WM_DESTROYCLIPBOARD) \
	{ \
		ThisClass :: OnDestroyClipboard(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnDrawClipboard()
#define ON_WM_DRAWCLIPBOARD() \
	if (uMsg == WM_DRAWCLIPBOARD) \
	{ \
		ThisClass :: OnDrawClipboard(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnPaintClipboard(CWindow wndViewer, const LPPAINTSTRUCT lpPaintStruct)
#define ON_WM_PAINTCLIPBOARD() \
	if (uMsg == WM_PAINTCLIPBOARD) \
	{ \
		ThisClass :: OnPaintClipboard(FromHandle((HWND)wParam), (HGLOBAL)lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnVScrollClipboard(CWindow wndViewer, UINT nSBCode, UINT nPos)
#define ON_WM_VSCROLLCLIPBOARD() \
	if (uMsg == WM_VSCROLLCLIPBOARD) \
	{ \
		ThisClass :: OnVScrollClipboard(FromHandle((HWND)wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnContextMenu(CWindow wnd, CPoint point)
#define ON_WM_CONTEXTMENU() \
	if (uMsg == WM_CONTEXTMENU) \
	{ \
		ThisClass :: OnContextMenu(FromHandle((HWND)wParam, NULL, m_hWnd), CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnSizeClipboard(CWindow wndViewer, const LPRECT lpRect)
#define ON_WM_SIZECLIPBOARD() \
	if (uMsg == WM_SIZECLIPBOARD) \
	{ \
		ThisClass :: OnSizeClipboard(FromHandle((HWND)wParam), (HGLOBAL)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnAskCbFormatName(UINT nMaxCount, LPTSTR lpszString)
#define ON_WM_ASKCBFORMATNAME() \
	if (uMsg == WM_ASKCBFORMATNAME) \
	{ \
		ThisClass :: OnAskCbFormatName((UINT)wParam, (LPTSTR)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnChangeCbChain(CWindow wndRemove, CWindow wndAfter)
#define ON_WM_CHANGECBCHAIN() \
	if (uMsg == WM_CHANGECBCHAIN) \
	{ \
		ThisClass :: OnChangeCbChain(FromHandle((HWND)wParam), FromHandle((HWND)lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnHScrollClipboard(CWindow wndViewer, UINT nSBCode, UINT nPos)
#define ON_WM_HSCROLLCLIPBOARD() \
	if (uMsg == WM_HSCROLLCLIPBOARD) \
	{ \
		ThisClass :: OnHScrollClipboard(FromHandle((HWND)wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnQueryNewPalette()
#define ON_WM_QUERYNEWPALETTE() \
	if (uMsg == WM_QUERYNEWPALETTE) \
	{ \
		lResult = (LRESULT)ThisClass :: OnQueryNewPalette(); \
		return ON_HANDLED_VALUE ; \
	}

// void OnPaletteChanged(CWindow wndFocus)
#define ON_WM_PALETTECHANGED() \
	if (uMsg == WM_PALETTECHANGED) \
	{ \
		ThisClass :: OnPaletteChanged(FromHandle((HWND)wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnPaletteIsChanging(CWindow wndPalChg)
#define ON_WM_PALETTEISCHANGING() \
	if (uMsg == WM_PALETTEISCHANGING) \
	{ \
		ThisClass :: OnPaletteIsChanging(FromHandle((HWND)wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnDropFiles(HDROP hDropInfo)
#define ON_WM_DROPFILES() \
	if (uMsg == WM_DROPFILES) \
	{ \
		ThisClass :: OnDropFiles((HDROP)wParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnWindowPosChanging(LPWINDOWPOS lpWndPos)
#define ON_WM_WINDOWPOSCHANGING() \
	if (uMsg == WM_WINDOWPOSCHANGING) \
	{ \
		ThisClass :: OnWindowPosChanging((LPWINDOWPOS)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
#define ON_WM_WINDOWPOSCHANGED() \
	if (uMsg == WM_WINDOWPOSCHANGED) \
	{ \
		ThisClass :: OnWindowPosChanged((LPWINDOWPOS)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnExitMenuLoop(BOOL fIsTrackPopupMenu)
#define ON_WM_EXITMENULOOP() \
	if (uMsg == WM_EXITMENULOOP) \
	{ \
		ThisClass :: OnExitMenuLoop((BOOL)wParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnEnterMenuLoop(BOOL fIsTrackPopupMenu)
#define ON_WM_ENTERMENULOOP() \
	if (uMsg == WM_ENTERMENULOOP) \
	{ \
		ThisClass :: OnEnterMenuLoop((BOOL)wParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
#define ON_WM_STYLECHANGED() \
	if (uMsg == WM_STYLECHANGED) \
	{ \
		ThisClass :: OnStyleChanged((INT)wParam, (LPSTYLESTRUCT)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnStyleChanging(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
#define ON_WM_STYLECHANGING() \
	if (uMsg == WM_STYLECHANGING) \
	{ \
		ThisClass :: OnStyleChanging((INT)wParam, (LPSTYLESTRUCT)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnSizing(UINT fwSide, LPRECT pRect)
#define ON_WM_SIZING() \
	if (uMsg == WM_SIZING) \
	{ \
		ThisClass :: OnSizing((UINT)wParam, (LPRECT)lParam); \
		lResult = TRUE; \
		return ON_HANDLED_VALUE ; \
	}

// void OnMoving(UINT fwSide, LPRECT pRect)
#define ON_WM_MOVING() \
	if (uMsg == WM_MOVING) \
	{ \
		ThisClass :: OnMoving((UINT)wParam, (LPRECT)lParam); \
		lResult = TRUE; \
		return ON_HANDLED_VALUE ; \
	}

// void OnEnterSizeMove()
#define ON_WM_ENTERSIZEMOVE() \
	if (uMsg == WM_ENTERSIZEMOVE) \
	{ \
		ThisClass :: OnEnterSizeMove(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnExitSizeMove()
#define ON_WM_EXITSIZEMOVE() \
	if (uMsg == WM_EXITSIZEMOVE) \
	{ \
		ThisClass :: OnExitSizeMove(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnCaptureChanged(CWindow wnd)
#define ON_WM_CAPTURECHANGED() \
	if (uMsg == WM_CAPTURECHANGED) \
	{ \
		ThisClass :: OnCaptureChanged(FromHandle((HWND)lParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
#define ON_WM_DEVICECHANGE() \
	if (uMsg == WM_DEVICECHANGE) \
	{ \
		lResult = (LRESULT)ThisClass :: OnDeviceChange((UINT)wParam, (DWORD_PTR)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// BOOL OnPowerBroadcast(DWORD dwPowerEvent, DWORD_PTR dwData)
#define ON_WM_POWERBROADCAST() \
	if (uMsg == WM_POWERBROADCAST) \
	{ \
		lResult = (LRESULT)ThisClass :: OnPowerBroadcast((UINT)wParam, (LPARAM)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// void OnUserChanged()
#define ON_WM_USERCHANGED() \
	if (uMsg == WM_USERCHANGED) \
	{ \
		ThisClass :: OnUserChanged(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnChangeUIState(WORD nAction, WORD nState)
#define ON_WM_CHANGEUISTATE() \
	if (uMsg == WM_CHANGEUISTATE) \
	{ \
		ThisClass :: OnChangeUIState((UINT)LOWORD(wParam), (UINT)HIWORD(wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnUpdateUIState(WORD nAction, WORD nState)
#define ON_WM_UPDATEUISTATE() \
	if (uMsg == WM_UPDATEUISTATE) \
	{ \
		ThisClass :: OnUpdateUIState((UINT)LOWORD(wParam), (UINT)HIWORD(wParam)); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// LRESULT OnQueryUIState()
#define ON_WM_QUERYUISTATE() \
	if (uMsg == WM_QUERYUISTATE) \
	{ \
		lResult = ThisClass :: OnQueryUIState(); \
		return ON_HANDLED_VALUE ; \
	}

// void OnThemeChanged()
#define ON_WM_THEMECHANGED() \
	if (uMsg == WM_THEMECHANGED) \
	{ \
		ThisClass :: OnThemeChanged(); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

// void OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
#define ON_WM_SETTINGCHANGE() \
	if (uMsg == WM_SETTINGCHANGE) \
	{ \
		ThisClass :: OnSettingChange((UINT)wParam, (LPCTSTR)lParam); \
		lResult = 0; \
		return ON_HANDLED_VALUE ; \
	}

/////////////////////////////////////////////////////////////////////////////
// Message map tables for Control Notification messages

// Static control notification codes
#define ON_STN_CLICKED(id, memberFxn) \
	ON_CONTROL(STN_CLICKED, id, memberFxn)
#define ON_STN_DBLCLK(id, memberFxn) \
	ON_CONTROL(STN_DBLCLK, id, memberFxn)
#define ON_STN_ENABLE(id, memberFxn) \
	ON_CONTROL(STN_ENABLE, id, memberFxn)
#define ON_STN_DISABLE(id, memberFxn) \
	ON_CONTROL(STN_DISABLE, id, memberFxn)


// Edit Control Notification Codes
#define ON_EN_SETFOCUS(id, memberFxn) \
	ON_CONTROL(EN_SETFOCUS, id, memberFxn)
#define ON_EN_KILLFOCUS(id, memberFxn) \
	ON_CONTROL(EN_KILLFOCUS, id, memberFxn)
#define ON_EN_CHANGE(id, memberFxn) \
	ON_CONTROL(EN_CHANGE, id, memberFxn)
#define ON_EN_UPDATE(id, memberFxn) \
	ON_CONTROL(EN_UPDATE, id, memberFxn)
#define ON_EN_ERRSPACE(id, memberFxn) \
	ON_CONTROL(EN_ERRSPACE, id, memberFxn)
#define ON_EN_MAXTEXT(id, memberFxn) \
	ON_CONTROL(EN_MAXTEXT, id, memberFxn)
#define ON_EN_HSCROLL(id, memberFxn) \
	ON_CONTROL(EN_HSCROLL, id, memberFxn)
#define ON_EN_VSCROLL(id, memberFxn) \
	ON_CONTROL(EN_VSCROLL, id, memberFxn)

#define ON_EN_ALIGN_LTR_EC(id, memberFxn) \
	ON_CONTROL(EN_ALIGN_LTR_EC, id, memberFxn)
#define ON_EN_ALIGN_RTL_EC(id, memberFxn) \
	ON_CONTROL(EN_ALIGN_RTL_EC, id, memberFxn)

// Richedit Control Notification Codes
#define ON_EN_IMECHANGE(id, memberFxn) \
	ON_CONTROL(EN_IMECHANGE, id, memberFxn)
#define ON_EN_ALIGNLTR(id, memberFxn) \
	ON_CONTROL(EN_ALIGNLTR, id, memberFxn)
#define ON_EN_ALIGNRTL(id, memberFxn) \
	ON_CONTROL(EN_ALIGNRTL, id, memberFxn)

// Animation Control Notification Codes
#define ON_ACN_START(id, memberFxn) \
	ON_CONTROL(ACN_START, id, memberFxn)
#define ON_ACN_STOP(id, memberFxn) \
	ON_CONTROL(ACN_STOP, id, memberFxn)

// User Button Notification Codes
#define ON_BN_CLICKED(id, memberFxn) \
	ON_CONTROL(BN_CLICKED, id, memberFxn)
#define ON_BN_DOUBLECLICKED(id, memberFxn) \
	ON_CONTROL(BN_DOUBLECLICKED, id, memberFxn)
#define ON_BN_SETFOCUS(id, memberFxn) \
	ON_CONTROL(BN_SETFOCUS, id, memberFxn)
#define ON_BN_KILLFOCUS(id, memberFxn) \
	ON_CONTROL(BN_KILLFOCUS, id, memberFxn)

// old BS_USERBUTTON button notifications - obsolete in Win31
#define ON_BN_PAINT(id, memberFxn) \
	ON_CONTROL(BN_PAINT, id, memberFxn)
#define ON_BN_HILITE(id, memberFxn) \
	ON_CONTROL(BN_HILITE, id, memberFxn)
#define ON_BN_UNHILITE(id, memberFxn) \
	ON_CONTROL(BN_UNHILITE, id, memberFxn)
#define ON_BN_DISABLE(id, memberFxn) \
	ON_CONTROL(BN_DISABLE, id, memberFxn)

// Listbox Notification Codes
#define ON_LBN_ERRSPACE(id, memberFxn) \
	ON_CONTROL(LBN_ERRSPACE, id, memberFxn)
#define ON_LBN_SELCHANGE(id, memberFxn) \
	ON_CONTROL(LBN_SELCHANGE, id, memberFxn)
#define ON_LBN_DBLCLK(id, memberFxn) \
	ON_CONTROL(LBN_DBLCLK, id, memberFxn)
#define ON_LBN_SELCANCEL(id, memberFxn) \
	ON_CONTROL(LBN_SELCANCEL, id, memberFxn)
#define ON_LBN_SETFOCUS(id, memberFxn) \
	ON_CONTROL(LBN_SETFOCUS, id, memberFxn)
#define ON_LBN_KILLFOCUS(id, memberFxn) \
	ON_CONTROL(LBN_KILLFOCUS, id, memberFxn)

// Check Listbox Notification codes
#define CLBN_CHKCHANGE (40)
#define ON_CLBN_CHKCHANGE(id, memberFxn) \
	ON_CONTROL(CLBN_CHKCHANGE, id, memberFxn)

// Combo Box Notification Codes
#define ON_CBN_ERRSPACE(id, memberFxn) \
	ON_CONTROL(CBN_ERRSPACE, id, memberFxn)
#define ON_CBN_SELCHANGE(id, memberFxn) \
	ON_CONTROL(CBN_SELCHANGE, id, memberFxn)
#define ON_CBN_DBLCLK(id, memberFxn) \
	ON_CONTROL(CBN_DBLCLK, id, memberFxn)
#define ON_CBN_SETFOCUS(id, memberFxn) \
	ON_CONTROL(CBN_SETFOCUS, id, memberFxn)
#define ON_CBN_KILLFOCUS(id, memberFxn) \
	ON_CONTROL(CBN_KILLFOCUS, id, memberFxn)
#define ON_CBN_EDITCHANGE(id, memberFxn) \
	ON_CONTROL(CBN_EDITCHANGE, id, memberFxn)
#define ON_CBN_EDITUPDATE(id, memberFxn) \
	ON_CONTROL(CBN_EDITUPDATE, id, memberFxn)
#define ON_CBN_DROPDOWN(id, memberFxn) \
	ON_CONTROL(CBN_DROPDOWN, id, memberFxn)
#define ON_CBN_CLOSEUP(id, memberFxn)  \
	ON_CONTROL(CBN_CLOSEUP, id, memberFxn)
#define ON_CBN_SELENDOK(id, memberFxn)  \
	ON_CONTROL(CBN_SELENDOK, id, memberFxn)
#define ON_CBN_SELENDCANCEL(id, memberFxn)  \
	ON_CONTROL(CBN_SELENDCANCEL, id, memberFxn)

/////////////////////////////////////////////////////////////////////////////
// User extensions for message map entries

// for Windows messages
#define ON_MESSAGE(message, memberFxn) \
	if (uMsg == message) \
	{ \
		lResult = (LRESULT)MEMBER_FXN(memberFxn)((WPARAM)wParam, (LPARAM)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// like ON_MESSAGE but no return value
#define ON_MESSAGE_VOID(message, memberFxn) \
	if (uMsg == message) \
	{ \
		MEMBER_FXN(memberFxn)(); \
		lResult = 0 ; \
		return ON_HANDLED_VALUE ; \
	}

// for Registered Windows messages
#define ON_REGISTERED_MESSAGE(nMessageVariable, memberFxn) \
	if (uMsg == 0xC000) \
	{ \
		lResult = (LRESULT)MEMBER_FXN(memberFxn)((WPARAM)wParam, (LPARAM)lParam); \
		return ON_HANDLED_VALUE ; \
	}

// for Thread messages
#define ON_THREAD_MESSAGE(message, memberFxn) \
	if (uMsg == message) \
	{ \
		MEMBER_FXN(memberFxn)((WPARAM)wParam, (LPARAM)lParam); \
		lResult = 0 ;\
		return ON_HANDLED_VALUE ; \
	}

// for Registered Windows messages
#define ON_REGISTERED_THREAD_MESSAGE(nMessageVariable, memberFxn) \
	if (uMsg == 0xC000) \
	{ \
		MEMBER_FXN(memberFxn)((WPARAM)wParam, (LPARAM)lParam); \
		lResult = 0 ;\
		return ON_HANDLED_VALUE ; \
	}

