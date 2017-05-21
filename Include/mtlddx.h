

/////////////////////////////////////////////////////////////////////////////
// Notes for implementing dialog data exchange and validation procs:
//  * always start with PrepareCtrl or PrepareEditCtrl
//  * always start with 'pDX->m_bSaveAndValidate' check
//  * pDX->Fail() will throw an exception - so be prepared
//  * avoid creating temporary HWNDs for dialog controls - i.e.
//      use HWNDs for child elements
//  * validation procs should only act if 'm_bSaveAndValidate'
//  * use the suffices:
//      DDX_ = exchange proc
//      DDV_ = validation proc
//
/////////////////////////////////////////////////////////////////////////////


// DDV parse errors
#define AFX_IDP_PARSE_INT               0xF110
#define AFX_IDP_PARSE_REAL              0xF111
#define AFX_IDP_PARSE_INT_RANGE         0xF112
#define AFX_IDP_PARSE_REAL_RANGE        0xF113
#define AFX_IDP_PARSE_STRING_SIZE       0xF114
#define AFX_IDP_PARSE_RADIO_BUTTON      0xF115
#define AFX_IDP_PARSE_BYTE              0xF116
#define AFX_IDP_PARSE_UINT              0xF117
#define AFX_IDP_PARSE_DATETIME          0xF118
#define AFX_IDP_PARSE_CURRENCY          0xF119
#define AFX_IDP_PARSE_GUID              0xF11A
#define AFX_IDP_PARSE_TIME              0xF11B
#define AFX_IDP_PARSE_DATE              0xF11C

/////////////////////////////////////////////////////////////////////////////
// Notes for implementing dialog data exchange and validation procs:
//  * always start with PrepareCtrl or PrepareEditCtrl
//  * always start with 'pDX->m_bSaveAndValidate' check
//  * pDX->Fail() will throw an exception - so be prepared
//  * avoid creating temporary HWNDs for dialog controls - i.e.
//      use HWNDs for child elements
//  * validation procs should only act if 'm_bSaveAndValidate'
//  * use the suffices:
//      DDX_ = exchange proc
//      DDV_ = validation proc
//
/////////////////////////////////////////////////////////////////////////////

_INLINE void _CDECL _AFX_DDX_TextWithFormat(CDataExchange* pDX, int nIDC, LPCTSTR lpszFormat, UINT nIDPrompt, ...)
	// only supports windows output formats - no floating point
{
	va_list pData;
	va_start(pData, nIDPrompt);

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	ASSERT(hWndCtrl != NULL);

	const int SZT_SIZE = 64;
	TCHAR szT[SZT_SIZE];
	if (pDX->m_bSaveAndValidate)
	{
		void* pResult;

		pResult = va_arg(pData, void*);
		// the following works for %d, %u, %ld, %lu
		::GetWindowText(hWndCtrl, szT, _countof(szT));
		if (_stscanf(szT, lpszFormat, pResult) != 1)
		{
#if 0
			AfxMessageBox(nIDPrompt);
			pDX->Fail();        // throws exception
#endif
		}
	}
	else
	{
		wvsprintf(szT, lpszFormat, pData);
		// does not support floating point numbers - see dlgfloat.cpp
		AfxSetWindowText(hWndCtrl, szT);
	}

	va_end(pData);
}



_INLINE void AfxTextFloatFormat(CDataExchange* pDX, int nIDC, void* pData, double value, int nSizeGcvt)
{
	ASSERT(pData != NULL);

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);

	const int TEXT_BUFFER_SIZE = 400;
	TCHAR szBuffer[TEXT_BUFFER_SIZE];
	if (pDX->m_bSaveAndValidate)
	{
		::GetWindowText(hWndCtrl, szBuffer, _countof(szBuffer));
		double d;
		if (_stscanf(szBuffer, _T("%lf"), &d) != 1)
		{
#if 0
			AfxMessageBox(AFX_IDP_PARSE_REAL);
			pDX->Fail();            // throws exception
#endif
		}
		if (nSizeGcvt == FLT_DIG)
			*((float*)pData) = (float)d;
		else
			*((double*)pData) = d;
	}
	else
	{
		_stprintf(szBuffer, _T("%.*g"), nSizeGcvt, value);
		AfxSetWindowText(hWndCtrl, szBuffer);
	}
}


/////////////////////////////////////////////////////////////////////////////
// Simple formatting to text item

_INLINE void DDX_Text(CDataExchange* pDX, int nIDC, BYTE& value)
{
	int n = (int)value;
	if (pDX->m_bSaveAndValidate)
	{
		_AFX_DDX_TextWithFormat(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_BYTE, &n);
#if 0
		if (n > 255)
		{
			AfxMessageBox(AFX_IDP_PARSE_BYTE);
			pDX->Fail();        // throws exception
		}
#endif
		value = (BYTE)n;
	}
	else
		_AFX_DDX_TextWithFormat(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_BYTE, n);
}

_INLINE void DDX_Text(CDataExchange* pDX, int nIDC, short& value)
{
	_AFX_DDX_TextWithFormat(pDX, nIDC, _T("%hd"), AFX_IDP_PARSE_INT, (pDX->m_bSaveAndValidate) ? (void*)&value : (void*)value);
}

_INLINE void DDX_Text(CDataExchange* pDX, int nIDC, int& value)
{
	_AFX_DDX_TextWithFormat(pDX, nIDC, _T("%d"), AFX_IDP_PARSE_INT, (pDX->m_bSaveAndValidate) ? (void*)&value : (void*)value);
}

_INLINE void DDX_Text(CDataExchange* pDX, int nIDC, UINT& value)
{
	_AFX_DDX_TextWithFormat(pDX, nIDC, _T("%u"), AFX_IDP_PARSE_UINT, (pDX->m_bSaveAndValidate) ? (void*)&value : (void*)value);
}

_INLINE void DDX_Text(CDataExchange* pDX, int nIDC, long& value)
{
	_AFX_DDX_TextWithFormat(pDX, nIDC, _T("%ld"), AFX_IDP_PARSE_INT, (pDX->m_bSaveAndValidate) ? (void*)&value : (void*)value);
}

_INLINE void DDX_Text(CDataExchange* pDX, int nIDC, DWORD& value)
{
	_AFX_DDX_TextWithFormat(pDX, nIDC, _T("%lu"), AFX_IDP_PARSE_UINT, (pDX->m_bSaveAndValidate) ? (void*)&value : (void*)value);
}

_INLINE void DDX_Text(CDataExchange* pDX, int nIDC, LONGLONG& value)
{
	_AFX_DDX_TextWithFormat(pDX, nIDC, _T("%I64d"), AFX_IDP_PARSE_INT, (pDX->m_bSaveAndValidate) ? (void*)&value : (void*)value);
}

_INLINE void DDX_Text(CDataExchange* pDX, int nIDC, ULONGLONG& value)
{
	_AFX_DDX_TextWithFormat(pDX, nIDC, _T("%I64u"), AFX_IDP_PARSE_UINT, (pDX->m_bSaveAndValidate) ? (void*)&value : (void*)value);
}

_INLINE void DDX_Text(CDataExchange* pDX, int nIDC, CString& value)
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		int nLen = ::GetWindowTextLength(hWndCtrl);
		::GetWindowText(hWndCtrl, value.GetBufferSetLength(nLen), nLen + 1);
		value.ReleaseBuffer();
	}
	else
	{
		AfxSetWindowText(hWndCtrl, value);
	}
}

_INLINE void DDX_Text(CDataExchange* pDX, int nIDC, LPTSTR value, int nMaxLen)
{
	ASSERT(nMaxLen != 0);

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		int nLen = ::GetWindowTextLength(hWndCtrl);
		int nRetrieved = ::GetWindowText(hWndCtrl, value, nMaxLen);
		if (nLen > nRetrieved)
			TRACE(traceAppMsg, 0, "Text in control ID %d is too long. Call DDV_MaxChars()!\n", nIDC);
	}
	else
	{
		AfxSetWindowText(hWndCtrl, value);
	}
}


_INLINE void DDX_Text(CDataExchange* pDX, int nIDC, float& value)
{
	AfxTextFloatFormat(pDX, nIDC, &value, value, FLT_DIG);
}

_INLINE void DDX_Text(CDataExchange* pDX, int nIDC, double& value)
{
	AfxTextFloatFormat(pDX, nIDC, &value, value, DBL_DIG);
}


/////////////////////////////////////////////////////////////////////////////
// Data exchange for special control

_INLINE void DDX_Check(CDataExchange* pDX, int nIDC, int& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		value = (int)::SendMessage(hWndCtrl, BM_GETCHECK, 0, 0L);
		ASSERT(value >= 0 && value <= 2);
	}
	else
	{
		if (value < 0 || value > 2)
		{
			TRACE(traceAppMsg, 0, "Warning: dialog data checkbox value (%d) out of range.\n", value);
			value = 0;  // default to off
		}
		::SendMessage(hWndCtrl, BM_SETCHECK, (WPARAM)value, 0L);
	}
}

_INLINE void DDX_Radio(CDataExchange* pDX, int nIDC, int& value)
// must be first in a group of auto radio buttons
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);

	ASSERT(::GetWindowLong(hWndCtrl, GWL_STYLE) & WS_GROUP);
	ASSERT(::SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0L) & DLGC_RADIOBUTTON);

	if (pDX->m_bSaveAndValidate)
		value = -1;     // value if none found

	// walk all children in group
	int iButton = 0;
	do
	{
		if (::SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0L) & DLGC_RADIOBUTTON)
		{
			// control in group is a radio button
			if (pDX->m_bSaveAndValidate)
			{
				if (::SendMessage(hWndCtrl, BM_GETCHECK, 0, 0L) != 0)
				{
					ASSERT(value == -1);    // only set once
					value = iButton;
				}
			}
			else
			{
				// select button
				::SendMessage(hWndCtrl, BM_SETCHECK, (iButton == value), 0L);
			}
			iButton++;
		}
		else
		{
			TRACE(traceAppMsg, 0, "Warning: skipping non-radio button in group.\n");
		}
		hWndCtrl = ::GetWindow(hWndCtrl, GW_HWNDNEXT);

	} while (hWndCtrl != NULL && !(GetWindowLong(hWndCtrl, GWL_STYLE) & WS_GROUP));
}

/////////////////////////////////////////////////////////////////////////////
// Listboxes, comboboxes

_INLINE void DDX_LBString(CDataExchange* pDX, int nIDC, CString& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		int nIndex = (int)::SendMessage(hWndCtrl, LB_GETCURSEL, 0, 0L);
		if (nIndex != -1)
		{
			int nLen = (int)::SendMessage(hWndCtrl, LB_GETTEXTLEN, nIndex, 0L);
			::SendMessage(hWndCtrl, LB_GETTEXT, nIndex,
				(LPARAM)(LPVOID)value.GetBufferSetLength(nLen));
		}
		else
		{
			// no selection
			value.Empty();
		}
		value.ReleaseBuffer();
	}
	else
	{
		// set current selection based on data string
		if (::SendMessage(hWndCtrl, LB_SELECTSTRING, (WPARAM)-1,
			(LPARAM)(LPCTSTR)value) == LB_ERR)
		{
			// no selection match
			TRACE(traceAppMsg, 0, "Warning: no listbox item selected.\n");
		}
	}
}

_INLINE void DDX_LBStringExact(CDataExchange* pDX, int nIDC, CString& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		DDX_LBString(pDX, nIDC, value);
	}
	else
	{
		// set current selection based on data string
		int i = (int)::SendMessage(hWndCtrl, LB_FINDSTRINGEXACT, (WPARAM)-1,
			(LPARAM)(LPCTSTR)value);
		if (i < 0)
		{
			// no selection match
			TRACE(traceAppMsg, 0, "Warning: no listbox item selected.\n");
		}
		else
		{
			// select it
			SendMessage(hWndCtrl, LB_SETCURSEL, i, 0L);
		}
	}
}

_INLINE void DDX_CBString(CDataExchange* pDX, int nIDC, CString& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);

	if ((::GetWindowLong(hWndCtrl, GWL_STYLE) & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST)
	{
		pDX->PrepareEditCtrl(nIDC);
	}

	if (pDX->m_bSaveAndValidate)
	{
		// just get current edit item text (or drop list static)
		int nLen = ::GetWindowTextLength(hWndCtrl);
		if (nLen > 0)
		{
			// get known length
			::GetWindowText(hWndCtrl, value.GetBufferSetLength(nLen), nLen + 1);
		}
		else
		{
			// for drop lists GetWindowTextLength does not work - assume
			//  max of 255 characters
			::GetWindowText(hWndCtrl, value.GetBuffer(255), 255 + 1);
		}
		value.ReleaseBuffer();
	}
	else
	{
		// set current selection based on model string
		if (::SendMessage(hWndCtrl, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)(LPCTSTR)value) == CB_ERR)
		{
			// just set the edit text (will be ignored if DROPDOWNLIST)
			AfxSetWindowText(hWndCtrl, value);
		}
	}
}

_INLINE void DDX_CBStringExact(CDataExchange* pDX, int nIDC, CString& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);

	if ((::GetWindowLong(hWndCtrl, GWL_STYLE) & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST)
	{
		pDX->PrepareEditCtrl(nIDC);
	}

	if (pDX->m_bSaveAndValidate)
	{
		DDX_CBString(pDX, nIDC, value);
	}
	else
	{
		// set current selection based on data string
		int i = (int)::SendMessage(hWndCtrl, CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)(LPCTSTR)value);
		if (i < 0)
		{
			// just set the edit text (will be ignored if DROPDOWNLIST)
			AfxSetWindowText(hWndCtrl, value);
		}
		else
		{
			// select it
			SendMessage(hWndCtrl, CB_SETCURSEL, i, 0L);
		}
	}
}

_INLINE void DDX_LBIndex(CDataExchange* pDX, int nIDC, int& index)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
		index = (int)::SendMessage(hWndCtrl, LB_GETCURSEL, 0, 0L);
	else
		::SendMessage(hWndCtrl, LB_SETCURSEL, (WPARAM)index, 0L);
}

_INLINE void DDX_CBIndex(CDataExchange* pDX, int nIDC, int& index)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
		index = (int)::SendMessage(hWndCtrl, CB_GETCURSEL, 0, 0L);
	else
		::SendMessage(hWndCtrl, CB_SETCURSEL, (WPARAM)index, 0L);
}

_INLINE void DDX_Scroll(CDataExchange* pDX, int nIDC, int& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
		value = GetScrollPos(hWndCtrl, SB_CTL);
	else
		SetScrollPos(hWndCtrl, SB_CTL, value, TRUE);
}

_INLINE void DDX_Slider(CDataExchange* pDX, int nIDC, int& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
		value = (int) ::SendMessage(hWndCtrl, TBM_GETPOS, 0, 0l);
	else
		::SendMessage(hWndCtrl, TBM_SETPOS, TRUE, value);
}

_INLINE void DDX_IPAddress(CDataExchange* pDX, int nIDC, DWORD& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
		::SendMessage(hWndCtrl, IPM_GETADDRESS, 0, (LPARAM)&value);
	else
		::SendMessage(hWndCtrl, IPM_SETADDRESS, 0, (LPARAM)value);
}




/////////////////////////////////////////////////////////////////////////////
// Range Dialog Data Validation


/////////////////////////////////////////////////////////////////////////////
// Validation procs

#define _DDV_VALUE_MIN_MAX(v, min, max) \
			ASSERT(min <= max); if (v < min) v = min; else if (v > max) v = max;

//NOTE: don't use overloaded function names to avoid type ambiguities
_INLINE void DDV_MinMaxByte(CDataExchange* pDX, BYTE& value, BYTE minVal, BYTE maxVal)
{
	_DDV_VALUE_MIN_MAX(value, minVal, maxVal) ;
}

_INLINE void DDV_MinMaxShort(CDataExchange* pDX, short& value, short minVal, short maxVal)
{
	_DDV_VALUE_MIN_MAX(value, minVal, maxVal);
}

_INLINE void DDV_MinMaxInt(CDataExchange* pDX, int& value, int minVal, int maxVal)
{
	_DDV_VALUE_MIN_MAX(value, minVal, maxVal);
}

_INLINE void DDV_MinMaxLong(CDataExchange* pDX, long& value, long minVal, long maxVal)
{
	_DDV_VALUE_MIN_MAX(value, minVal, maxVal);
}

_INLINE void DDV_MinMaxUInt(CDataExchange* pDX, UINT& value, UINT minVal, UINT maxVal)
{
	_DDV_VALUE_MIN_MAX(value, minVal, maxVal);
}

_INLINE void DDV_MinMaxDWord(CDataExchange* pDX, DWORD& value, DWORD minVal, DWORD maxVal)
{
	_DDV_VALUE_MIN_MAX(value, minVal, maxVal);
}

_INLINE void DDV_MinMaxLongLong(CDataExchange* pDX, LONGLONG& value, LONGLONG minVal, LONGLONG maxVal)
{
	_DDV_VALUE_MIN_MAX(value, minVal, maxVal);
}

_INLINE void DDV_MinMaxULongLong(CDataExchange* pDX, ULONGLONG& value, ULONGLONG minVal, ULONGLONG maxVal)
{
	_DDV_VALUE_MIN_MAX(value, minVal, maxVal);
}

_INLINE void DDV_MinMaxSlider(CDataExchange* pDX, DWORD& value, DWORD minVal, DWORD maxVal)
{
	ASSERT(minVal <= maxVal);

	if (!pDX->m_bSaveAndValidate)
	{
		if (value < minVal) value = minVal;
		else if (value > maxVal) value = maxVal;
	}

	HWND hWndLastControl;
	pDX->m_pDlgWnd->GetDlgItem(pDX->m_idLastControl, &hWndLastControl);
	::SendMessage(hWndLastControl, TBM_SETRANGEMIN, FALSE, (LPARAM)minVal);
	::SendMessage(hWndLastControl, TBM_SETRANGEMAX, TRUE, (LPARAM)maxVal);
}

_INLINE void DDV_MinMaxFloat(CDataExchange* pDX, float& value, float minVal, float maxVal)
{
	_DDV_VALUE_MIN_MAX(value, minVal, maxVal);
}

_INLINE void DDV_MinMaxDouble(CDataExchange* pDX, double& value, double minVal, double maxVal)
{
	_DDV_VALUE_MIN_MAX(value, minVal, maxVal);
}


/////////////////////////////////////////////////////////////////////////////
// Max Chars Dialog Data Validation

_INLINE void DDV_MaxChars(CDataExchange* pDX, CString& value, int nChars)
{
	ASSERT(nChars >= 1);        // allow them something
	if (pDX->m_bSaveAndValidate && value.GetLength() > nChars)
	{
		value.ReleaseBuffer(nChars);
	}
	else if (pDX->m_idLastControl != 0 && pDX->m_bEditLastControl)
	{
		HWND hWndLastControl;
		pDX->m_pDlgWnd->GetDlgItem(pDX->m_idLastControl, &hWndLastControl);
		// limit the control max-chars automatically
		// send messages for both an edit control and a combobox control--one will
		// be understood and one will be disregarded, but this is the only way to
		// ensure that the characters will be limited for both kinds of controls.
		::SendMessage(hWndLastControl, EM_SETLIMITTEXT, nChars, 0);
		::SendMessage(hWndLastControl, CB_LIMITTEXT, nChars, 0);
	}
}

