





/////////////////////////////////////////////////////////////////////////////
// COleVariant class - wraps VARIANT types

typedef const VARIANT* LPCVARIANT;

_INLINE void AfxCheckError(SCODE sc)
{
	if (FAILED(sc))
	{
		if (sc == E_OUTOFMEMORY)
			AfxThrowMemoryException();
		else
			AfxThrowOleException(sc);
	}
}

class COleVariant : public tagVARIANT
{
	// Constructors
	public:
		static void AfxVariantInit(LPVARIANT pVar) { memset(pVar, 0, sizeof(*pVar)); }

		COleVariant() { AfxVariantInit(this); }

		COleVariant(long lSrc, VARTYPE vtSrc = VT_I4)
		{
			ASSERT(vtSrc == VT_I4 || vtSrc == VT_ERROR || vtSrc == VT_BOOL
				|| vtSrc == VT_UINT || vtSrc == VT_INT || vtSrc == VT_UI4
				|| vtSrc == VT_HRESULT);

			if (vtSrc == VT_ERROR)
			{
				vt = VT_ERROR;
				scode = lSrc;
			}
			else if (vtSrc == VT_BOOL)
			{
				vt = VT_BOOL;
				if (!lSrc)
					V_BOOL(this) = AFX_OLE_FALSE;
				else
					V_BOOL(this) = AFX_OLE_TRUE;
			}
			else if (vtSrc == VT_INT)
			{
				vt = VT_INT;
				V_INT(this) = lSrc;
			}
			else if (vtSrc == VT_UINT)
			{
				vt = VT_UINT;
				V_UINT(this) = lSrc;
			}
			else if (vtSrc == VT_HRESULT)
			{
				vt = VT_HRESULT;
				V_ERROR(this) = lSrc;
			}
			else if (vtSrc == VT_UI4)
			{
				vt = VT_UI4;
				lVal = lSrc;
			}
			else
			{
				vt = VT_I4;
				lVal = lSrc;
			}
		}

		COleVariant(LPCTSTR lpszSrc, VARTYPE vtSrc) // used to set to ANSI string
		{
#if defined (UNICODE)
			ASSERT(vtSrc == VT_BSTR);
#else
			ASSERT(vtSrc == VT_BSTR || vtSrc == VT_BSTRT);
#endif
			UNUSED(vtSrc);

			vt = VT_BSTR;
			bstrVal = NULL;

			if (lpszSrc != NULL)
			{
#ifndef _UNICODE
				if (vtSrc == VT_BSTRT)
				{
					int nLen = static_cast<int>(_tcslen(lpszSrc));
					bstrVal = ::SysAllocStringByteLen(lpszSrc, nLen);

					if (bstrVal == NULL)
						AfxThrowMemoryException();
				}
				else
#endif
				{
					bstrVal = CStringW(lpszSrc).AllocSysString();
				}
			}
		}

		COleVariant(BYTE nSrc) { vt = VT_UI1; bVal = nSrc; }
		COleVariant(const CByteArray& arrSrc) { vt = VT_EMPTY; *this = arrSrc; }
		//COleVariant(const CLongBinary& lbSrc) { vt = VT_EMPTY; *this = lbSrc; }
		//COleVariant(const COleCurrency& curSrc) { vt = VT_CY; cyVal = curSrc.m_cur; }
		//COleVariant(const COleDateTime& dateSrc) { vt = VT_DATE; date = dateSrc; }
		COleVariant(CString& strSrc) { vt = VT_EMPTY; *this = strSrc; }
		COleVariant(double dblSrc) { vt = VT_R8; dblVal = dblSrc; }
		COleVariant(float fltSrc) { vt = VT_R4; fltVal = fltSrc; }
		COleVariant(LPCTSTR lpszSrc) { vt = VT_EMPTY; *this = lpszSrc; }
#ifndef _MTL_ATL3
		COleVariant(LONGLONG nSrc) { vt = VT_I8; llVal = nSrc; }
		COleVariant(ULONGLONG nSrc) { vt = VT_UI8; ullVal = nSrc; }
#endif // _MTL_ATL3

		~COleVariant() { VERIFY(::VariantClear(this) == NOERROR); }

	public:
	public:
		// Literal comparison. Types and values must match.
		BOOL operator==(const VARIANT& var) const
		{
			if (&var == this)
				return TRUE;

			// Variants not equal if types don't match
			if (var.vt != vt)
				return FALSE;

			// Check type specific values
			switch (vt)
			{
				case VT_EMPTY:
				case VT_NULL:
					return TRUE;

				case VT_BOOL:
					return V_BOOL(&var) == V_BOOL(this);

				case VT_I1:
					return var.cVal == cVal;

				case VT_UI1:
					return var.bVal == bVal;

				case VT_I2:
					return var.iVal == iVal;

				case VT_UI2:
					return var.uiVal == uiVal;

				case VT_I4:
					return var.lVal == lVal;

				case VT_UI4:
					return var.ulVal == ulVal;
#ifndef _MTL_ATL3
				case VT_I8:
					return var.llVal == llVal;

				case VT_UI8:
					return var.ullVal == ullVal;
#endif // _MTL_ATL3
				case VT_CY:
					return (var.cyVal.Hi == cyVal.Hi && var.cyVal.Lo == cyVal.Lo);

				case VT_R4:
					return var.fltVal == fltVal;

				case VT_R8:
					return var.dblVal == dblVal;

				case VT_DATE:
					return var.date == date;

				case VT_BSTR:
					return SysStringByteLen(var.bstrVal) == SysStringByteLen(bstrVal) &&
						memcmp(var.bstrVal, bstrVal, SysStringByteLen(bstrVal)) == 0;

				case VT_ERROR:
					return var.scode == scode;

				case VT_DISPATCH:
				case VT_UNKNOWN:
					return var.punkVal == punkVal;

				default:
					//if (vt & VT_ARRAY && !(vt & VT_BYREF))
					//	return _AfxCompareSafeArrays(var.parray, parray);
					//else
						ASSERT(FALSE);  // VT_BYREF not supported
					// fall through
			}

			return FALSE;
		}

		BOOL operator==(LPCVARIANT pSrc) const { return *this == *pSrc; }
		operator LPVARIANT() { return this; }
		operator LPCVARIANT() const { return this; }

		const COleVariant& operator=(const VARIANT& varSrc)
		{
			if ((LPVARIANT)(this) != &varSrc)
				AfxCheckError(::VariantCopy(this, (LPVARIANT)&varSrc));

			return *this;
		}

		const COleVariant& operator=(LPCVARIANT pSrc)
		{
			if ((LPCVARIANT)(this) != pSrc)
				AfxCheckError(::VariantCopy(this, (LPVARIANT)pSrc));

			return *this;
		}

		const COleVariant& operator=(const COleVariant& varSrc)
		{
			if (this != &varSrc)
			{
				AfxCheckError(::VariantCopy(this, (LPVARIANT)&varSrc));
			}

			return *this;
		}

		const COleVariant& operator=(const LPCTSTR lpszSrc)
		{
			// Free up previous VARIANT
			Clear();

			vt = VT_BSTR;
			if (lpszSrc == NULL)
				bstrVal = NULL;
			else
			{
				bstrVal = CStringW(lpszSrc).AllocSysString();
			}
			return *this;
		}

		const COleVariant& operator=(const CString& strSrc)
		{
			// Free up previous VARIANT
			Clear();

			vt = VT_BSTR;
			bstrVal = strSrc.AllocSysString();

			return *this;
		}

		const COleVariant& operator=(BYTE nSrc)
		{
			// Free up previous VARIANT if necessary
			if (vt != VT_UI1)
			{
				Clear();
				vt = VT_UI1;
			}

			bVal = nSrc;
			return *this;
		}

		const COleVariant& operator=(short nSrc)
		{
			if (vt == VT_I2)
				iVal = nSrc;
			else if (vt == VT_BOOL)
			{
				if (!nSrc)
					V_BOOL(this) = AFX_OLE_FALSE;
				else
					V_BOOL(this) = AFX_OLE_TRUE;
			}
			else
			{
				// Free up previous VARIANT
				Clear();
				vt = VT_I2;
				iVal = nSrc;
			}

			return *this;
		}

		const COleVariant& operator=(long lSrc)
		{
			if (vt == VT_I4)
				lVal = lSrc;
			else if (vt == VT_ERROR)
				scode = lSrc;
			else if (vt == VT_BOOL)
			{
				if (!lSrc)
					V_BOOL(this) = AFX_OLE_FALSE;
				else
					V_BOOL(this) = AFX_OLE_TRUE;
			}
			else
			{
				// Free up previous VARIANT
				Clear();
				vt = VT_I4;
				lVal = lSrc;
			}

			return *this;
		}

#ifndef _MTL_ATL3
		const COleVariant& operator=(LONGLONG nSrc)
		{
			if (vt != VT_I8)
			{
				Clear();
				vt = VT_I8;
			}

			llVal = nSrc;
			return *this;
		}

		const COleVariant& operator=(ULONGLONG nSrc)
		{
			if (vt != VT_UI8)
			{
				Clear();
				vt = VT_UI8;
			}

			ullVal = nSrc;
			return *this;
		}
#endif // _MTL_ATL3

#if 0
		const COleVariant& operator=(const COleCurrency& curSrc)
		{
			// Free up previous VARIANT if necessary
			if (vt != VT_CY)
			{
				Clear();
				vt = VT_CY;
			}

			cyVal = curSrc.m_cur;
			return *this;
		}
#endif
		const COleVariant& operator=(float fltSrc)
		{
			// Free up previous VARIANT if necessary
			if (vt != VT_R4)
			{
				Clear();
				vt = VT_R4;
			}

			fltVal = fltSrc;
			return *this;
		}

		const COleVariant& operator=(double dblSrc)
		{
			// Free up previous VARIANT if necessary
			if (vt != VT_R8)
			{
				Clear();
				vt = VT_R8;
			}

			dblVal = dblSrc;
			return *this;
		}
#if 0
		const COleVariant& operator=(const COleDateTime& dateSrc)
		{
			// Free up previous VARIANT if necessary
			if (vt != VT_DATE)
			{
				Clear();
				vt = VT_DATE;
			}

			date = dateSrc;
			return *this;
		}

		const COleVariant& operator=(const CByteArray& arrSrc)
		{
			INT_PTR nSize = arrSrc.GetSize();
			if (nSize > LONG_MAX)
			{
				AfxThrowMemoryException();
			}

			// Set the correct type and make sure SafeArray can hold data
			_AfxCreateOneDimArray(*this, (DWORD)nSize);

			// Copy the data into the SafeArray
			_AfxCopyBinaryData(parray, arrSrc.GetData(), (DWORD)nSize);

			return *this;
		}

		const COleVariant& operator=(const CLongBinary& lbSrc)
		{
			// Set the correct type and make sure SafeArray can hold data
			if (lbSrc.m_dwDataLength > LONG_MAX)
			{
				AfxThrowMemoryException();
			}
			_AfxCreateOneDimArray(*this, (ULONG)lbSrc.m_dwDataLength);

			// Copy the data into the SafeArray
			BYTE* pData = (BYTE*)::GlobalLock(lbSrc.m_hData);
			_AfxCopyBinaryData(parray, pData, (ULONG)lbSrc.m_dwDataLength);
			::GlobalUnlock(lbSrc.m_hData);

			return *this;
		}
#endif

		void ChangeType(VARTYPE vartype, LPVARIANT pSrc)
		{
			// If pSrc is NULL, convert type in place
			if (pSrc == NULL)
				pSrc = this;
			if (pSrc != this || vartype != vt)
				AfxCheckError(::VariantChangeType(this, pSrc, 0, vartype));
		}

		void Clear() { VERIFY(::VariantClear(this) == NOERROR); }
};

/////////////////////////////////////////////////////////////////////////////
// CSafeArray class

typedef const SAFEARRAY* LPCSAFEARRAY;

class COleSafeArray : public tagVARIANT
{
	//Constructors
	public:
		static void AfxSafeArrayInit(COleSafeArray* psa) 
		{
			if (psa != NULL)
				memset(psa, 0, sizeof(*psa));
		}

		COleSafeArray()
		{
			AfxSafeArrayInit(this);
			vt = VT_EMPTY;
		}

		COleSafeArray(LPCVARIANT pSrc)
		{
			AfxSafeArrayInit(this);
			*this = pSrc;
			m_dwDims = GetDim();
			m_dwElementSize = GetElemSize();
		}

		~COleSafeArray() { Clear(); }

	public:
		// Cache info to make element access (operator []) faster
		DWORD m_dwElementSize;
		DWORD m_dwDims;

	public:
		operator LPVARIANT() { return this; }
		operator LPCVARIANT() const { return this; }

		// Assignment operators
		COleSafeArray& operator=(const COleSafeArray& saSrc)
		{
			ASSERT(saSrc.vt & VT_ARRAY);

			if (!(saSrc.vt & VT_ARRAY))
				AfxThrowInvalidArgException();

			AfxCheckError(::VariantCopy(this, (LPVARIANT)&saSrc));
			return *this;
		}

		COleSafeArray& operator=(const VARIANT& varSrc)
		{
			ASSERT(varSrc.vt & VT_ARRAY);

			if (!(varSrc.vt & VT_ARRAY))
				AfxThrowInvalidArgException();

			AfxCheckError(::VariantCopy(this, (LPVARIANT)&varSrc));
			return *this;
		}

		COleSafeArray& operator=(LPCVARIANT pSrc)
		{
			ASSERT(pSrc->vt & VT_ARRAY);

			if (!(pSrc->vt & VT_ARRAY))
				AfxThrowInvalidArgException();

			AfxCheckError(::VariantCopy(this, (LPVARIANT)pSrc));
			return *this;
		}

		COleSafeArray& operator=(const COleVariant& varSrc)
		{
			ASSERT(varSrc.vt & VT_ARRAY);

			if (!(varSrc.vt & VT_ARRAY))
				AfxThrowInvalidArgException();

			AfxCheckError(::VariantCopy(this, (LPVARIANT)&varSrc));
			return *this;
		}

		// Comparison operators
		static BOOL _AfxCompareSafeArrays(SAFEARRAY* parray1, SAFEARRAY* parray2)
		{
			BOOL bCompare = FALSE;

			// If one is NULL they must both be NULL to compare
			if (parray1 == NULL || parray2 == NULL)
			{
				return parray1 == parray2;
			}

			// Dimension must match and if 0, then arrays compare
			DWORD dwDim1 = ::SafeArrayGetDim(parray1);
			DWORD dwDim2 = ::SafeArrayGetDim(parray2);
			if (dwDim1 != dwDim2)
				return FALSE;
			else if (dwDim1 == 0)
				return TRUE;

			// Element size must match
			DWORD dwSize1 = ::SafeArrayGetElemsize(parray1);
			DWORD dwSize2 = ::SafeArrayGetElemsize(parray2);
			if (dwSize1 != dwSize2)
				return FALSE;

			long* pLBound1 = NULL;
			long* pLBound2 = NULL;
			long* pUBound1 = NULL;
			long* pUBound2 = NULL;

			void* pData1 = NULL;
			void* pData2 = NULL;

			TRY
			{
				// Bounds must match
				pLBound1 = new long[dwDim1];
				pLBound2 = new long[dwDim2];
				pUBound1 = new long[dwDim1];
				pUBound2 = new long[dwDim2];

				size_t nTotalElements = 1;

				// Get and compare bounds
				for (DWORD dwIndex = 0; dwIndex < dwDim1; dwIndex++)
				{
					AfxCheckError(::SafeArrayGetLBound(parray1, dwIndex + 1, &pLBound1[dwIndex]));
					AfxCheckError(::SafeArrayGetLBound(parray2, dwIndex + 1, &pLBound2[dwIndex]));
					AfxCheckError(::SafeArrayGetUBound(parray1, dwIndex + 1, &pUBound1[dwIndex]));
					AfxCheckError(::SafeArrayGetUBound(parray2, dwIndex + 1, &pUBound2[dwIndex]));

					// Check the magnitude of each bound
					if (pUBound1[dwIndex] - pLBound1[dwIndex] !=
						pUBound2[dwIndex] - pLBound2[dwIndex])
					{
						delete[] pLBound1;
						delete[] pLBound2;
						delete[] pUBound1;
						delete[] pUBound2;

						return FALSE;
					}

					// Increment the element count
					nTotalElements *= pUBound1[dwIndex] - pLBound1[dwIndex] + 1;
				}

				// Access the data
				AfxCheckError(::SafeArrayAccessData(parray1, &pData1));
				AfxCheckError(::SafeArrayAccessData(parray2, &pData2));

				// Calculate the number of bytes of data and compare
				size_t nSize = nTotalElements * dwSize1;
				int nOffset = memcmp(pData1, pData2, nSize);
				bCompare = nOffset == 0;

				// Release the array locks
				AfxCheckError(::SafeArrayUnaccessData(parray1));
				AfxCheckError(::SafeArrayUnaccessData(parray2));
			}
			CATCH_ALL(e)
			{
				// Clean up bounds arrays
				delete[] pLBound1;
				delete[] pLBound2;
				delete[] pUBound1;
				delete[] pUBound2;

				// Release the array locks
				if (pData1 != NULL)
					AfxCheckError(::SafeArrayUnaccessData(parray1));
				if (pData2 != NULL)
					AfxCheckError(::SafeArrayUnaccessData(parray2));

				THROW_LAST();
			}
			END_CATCH_ALL

				// Clean up bounds arrays
			delete[] pLBound1;
			delete[] pLBound2;
			delete[] pUBound1;
			delete[] pUBound2;

			return bCompare;
		}

		BOOL operator==(const SAFEARRAY& saSrc) const
		{
			return _AfxCompareSafeArrays(parray, (LPSAFEARRAY)&saSrc);
		}

		BOOL operator==(LPCSAFEARRAY pSrc) const
		{
			return _AfxCompareSafeArrays(parray, (LPSAFEARRAY)pSrc);
		}

		BOOL operator==(const COleSafeArray& saSrc) const
		{
			if (vt != saSrc.vt)
				return FALSE;

			return _AfxCompareSafeArrays(parray, saSrc.parray);
		}

		BOOL operator==(const VARIANT& varSrc) const
		{
			if (vt != varSrc.vt)
				return FALSE;

			return _AfxCompareSafeArrays(parray, varSrc.parray);
		}

		BOOL operator==(LPCVARIANT pSrc) const
		{
			if (vt != pSrc->vt)
				return FALSE;

			return _AfxCompareSafeArrays(parray, pSrc->parray);
		}

		BOOL operator==(const COleVariant& varSrc) const
		{
			if (vt != varSrc.vt)
				return FALSE;

			return _AfxCompareSafeArrays(parray, varSrc.parray);
		}

		void AccessData(void** ppvData) { AfxCheckError(::SafeArrayAccessData(parray, ppvData)); }

		void Clear() { VERIFY(::VariantClear(this) == NOERROR); }

		void Create(VARTYPE vtSrc, DWORD dwDims, SAFEARRAYBOUND* rgsabound)
		{
			ASSERT(dwDims > 0);
			ASSERT(rgsabound != NULL);

			// Validate the VARTYPE for SafeArrayCreate call
			ASSERT(!(vtSrc & VT_ARRAY));
			ASSERT(!(vtSrc & VT_BYREF));
			ASSERT(!(vtSrc & VT_VECTOR));
			ASSERT(vtSrc != VT_EMPTY);
			ASSERT(vtSrc != VT_NULL);

			if (dwDims == 0 ||
				rgsabound == NULL ||
				(vtSrc & VT_ARRAY) ||
				(vtSrc & VT_BYREF) ||
				(vtSrc & VT_VECTOR) ||
				vtSrc == VT_EMPTY ||
				vtSrc == VT_NULL)
			{
				AfxThrowInvalidArgException();
			}

			// Free up old safe array if necessary
			Clear();

			parray = ::SafeArrayCreate(vtSrc, dwDims, rgsabound);

			if (parray == NULL)
				AfxThrowMemoryException();

			vt = unsigned short(vtSrc | VT_ARRAY);
			m_dwDims = dwDims;
			m_dwElementSize = GetElemSize();
		}

		void UnaccessData() { AfxCheckError(::SafeArrayUnaccessData(parray)); }

		void CreateOneDim(VARTYPE vtSrc, DWORD dwElements, const void* pvSrcData = NULL, long nLBound = 0)
		{
			ENSURE(dwElements > 0);

			if (!(dwElements > 0))
				AfxThrowInvalidArgException();

			// Setup the bounds and create the array
			SAFEARRAYBOUND rgsabound;
			rgsabound.cElements = dwElements;
			rgsabound.lLbound = nLBound;
			Create(vtSrc, 1, &rgsabound);

			// Copy over the data if neccessary
			if (pvSrcData != NULL)
			{
				void* pvDestData;
				AccessData(&pvDestData);


				unsigned __int64 tmp_64 = (unsigned __int64)(GetElemSize()) * (unsigned __int64)(dwElements);

				ENSURE(tmp_64 <= INT_MAX); //no overflow ENSURE  

				memcpy(pvDestData, pvSrcData, (size_t)(tmp_64));
				UnaccessData();
			}
		}

		DWORD GetDim() { return ::SafeArrayGetDim(parray); }
		DWORD GetElemSize() { return ::SafeArrayGetElemsize(parray); }
		void GetLBound(DWORD dwDim, long* pLbound) { AfxCheckError(::SafeArrayGetLBound(parray, dwDim, pLbound)); }

		DWORD GetOneDimSize()
		{
			ENSURE(GetDim() == 1);

			long nUBound, nLBound;

			GetUBound(1, &nUBound);
			GetLBound(1, &nLBound);

			return nUBound + 1 - nLBound;
		}

		void GetUBound(DWORD dwDim, long* pUbound) { AfxCheckError(::SafeArrayGetUBound(parray, dwDim, pUbound)); }


};

/////////////////////////////////////////////////////////////////////////////
// COleDispatchDriver implementation
class COleDispParams : public DISPPARAMS 
{
	public:
		COleDispParams()
		{
			rgvarg = NULL;
			rgdispidNamedArgs = NULL;
			cArgs = 0;
			cNamedArgs = 0;
		}
		~COleDispParams()
		{
			if (rgvarg)
			{
				delete[] rgvarg;
				rgvarg = NULL;
			}
		}
};

class CVariantBoolPair
{
	public:
		CVariantBoolPair() : m_pbool(NULL), m_pvarbool(NULL)
		{
		}
		CVariantBoolPair(BOOL* pbool, VARIANT_BOOL* pvarbool, BOOL bOwnBOOLMem = TRUE)
							: m_pbool(pbool), m_pvarbool(pvarbool), m_bOwnBOOLMem(bOwnBOOLMem)
		{
			ENSURE(m_pbool != NULL && m_pvarbool != NULL);
		}
		CVariantBoolPair(const CVariantBoolPair& rhs)
		{
			m_pbool = rhs.Detach();
			m_pvarbool = rhs.m_pvarbool;
			m_bOwnBOOLMem = rhs.m_bOwnBOOLMem;
		}


		~CVariantBoolPair()
		{
			if (m_bOwnBOOLMem)
			{
				delete m_pbool;
			}
		}

	public:
		mutable BOOL* m_pbool;
		VARIANT_BOOL* m_pvarbool;
		BOOL m_bOwnBOOLMem;

	public:
		CVariantBoolPair& operator =(const CVariantBoolPair& rhs)
		{
			if (this != &rhs)
			{
				m_pbool = rhs.Detach();
				m_pvarbool = rhs.m_pvarbool;
				m_bOwnBOOLMem = rhs.m_bOwnBOOLMem;
			}
			return *this;
		}

		BOOL* Detach() const
		{
			BOOL* pb = m_pbool;
			if (m_bOwnBOOLMem)
			{
				m_pbool = NULL;
			}
			return pb;
		}
};

class CVariantBoolConverter
{
	public:

	public:
		CArray<CVariantBoolPair> m_boolArgs;

	public:
		void AddPair(const CVariantBoolPair& newPair)
		{
			m_boolArgs.Add(newPair);
		}
		void CopyBOOLsIntoVarBools()
		{
			for (int i = 0; i < m_boolArgs.GetCount(); ++i)
			{
				ENSURE(m_boolArgs[i].m_pbool != NULL && m_boolArgs[i].m_pvarbool != NULL);
				*m_boolArgs[i].m_pvarbool = *m_boolArgs[i].m_pbool ? VARIANT_TRUE : VARIANT_FALSE;
			}
		}

		void CopyVarBoolsIntoBOOLs()
		{
			for (int i = 0; i < m_boolArgs.GetCount(); ++i)
			{
				ENSURE(m_boolArgs[i].m_pbool != NULL && m_boolArgs[i].m_pvarbool != NULL);
				*m_boolArgs[i].m_pbool = (*m_boolArgs[i].m_pvarbool == 0) ? FALSE : TRUE;
			}
		}
};


template <UINT nID, class T, const IID* pdiid>
class _NO_VTABLE IDispEventAxWndImpl : public IDispEventSimpleImpl<nID, T, pdiid>
{
	public:
	public:
		CWnd* m_pCWndParent;

	public:
		STDMETHOD(Invoke)(DISPID dispidMember,
							REFIID /*riid*/,
							LCID lcid,
							WORD /*wFlags*/,
							DISPPARAMS* pdispparams,
							VARIANT* pvarResult,
							EXCEPINFO* /*pexcepinfo*/,
							UINT* /*puArgErr*/)
		{
			//TRACE("Invoke: %d\n", dispidMember) ;

			AFX_EVENT event(AFX_EVENT::event, dispidMember);

			const AFX_EVENTSINKMAP_ENTRY* pEntry = m_pCWndParent->GetEventSinkEntry(nID, &event);

			if (pEntry == NULL)
				return S_OK;

			UINT nParams = lstrlenA(pEntry->dispEntry.lpszParams) ;

			ATLASSERT(nParams <= _ATL_MAX_VARTYPES);
			if (nParams > _ATL_MAX_VARTYPES)
			{
				return E_FAIL;
			}

			VARTYPE pVarTypes[_ATL_MAX_VARTYPES];
			VARIANTARG* rgVarArgs[_ATL_MAX_VARTYPES];
			VARIANTARG** pVarArgs = nParams ? rgVarArgs : 0;


			UINT nIndex = 0;

#ifndef _ATL_IGNORE_NAMED_ARGS
			for (nIndex; nIndex < pdispparams->cNamedArgs; nIndex++)
			{
				UINT uNamedArgs = pdispparams->rgdispidNamedArgs[nIndex] ;

				ATLASSERT((NULL != pVarArgs) && (uNamedArgs < _countof(rgVarArgs)));
				if ((NULL == pVarArgs) || (uNamedArgs >= _countof(rgVarArgs)))
				{
					return E_FAIL;
				}
				pVarArgs[uNamedArgs] = &pdispparams->rgvarg[nIndex];
				pVarTypes[uNamedArgs] = pdispparams->rgvarg[nIndex].vt ;
			}
#endif

			UINT uArgs = pdispparams->cArgs ;

			for (; nIndex < uArgs; nIndex++)
			{
				ATLASSERT(NULL != pVarArgs);
				if (NULL == pVarArgs)
				{
					return E_FAIL;
				}
				pVarArgs[uArgs - nIndex - 1] = &pdispparams->rgvarg[nIndex];
				pVarTypes[uArgs - nIndex - 1] = pdispparams->rgvarg[nIndex].vt;
			}

			CThisCallThunk<T> thunk;
			thunk.Init(*(CThisCallThunk<T>::TMFP*)&pEntry->dispEntry.pfn, m_pCWndParent);

			CComVariant tmpResult;
			if (pvarResult == NULL)
				pvarResult = &tmpResult;

			HRESULT hr = DispCallFunc(&thunk,
										0,
										CC_CDECL, // CC_STDCALL, 
										pEntry->dispEntry.vt,
										nParams,
										pVarTypes,
										pVarArgs,
										pvarResult);
			ATLASSERT(SUCCEEDED(hr));
			return hr;
		}

		static const _ATL_EVENT_ENTRY<T>* _GetSinkMap() { return NULL ; }

};

template <UINT nID, const IID* pdiid>
class CAxWnd : public CAxWindowT<CWnd>, public IDispEventAxWndImpl<nID, CAxWnd<nID, pdiid>, pdiid>
{
	public:
		CAxWnd()
		{
#ifdef _MTL_ATL3
			AtlAxWinInit();
#endif // _MTL_ATL3
		}

		~CAxWnd()
		{
			LPUNKNOWN pUnk = GetControlUnknown();
			if (pUnk != NULL)
			{
				DispEventUnadvise(pUnk) ;
			}

			m_hWnd = NULL ;
		}

	public:
	public:
		BOOL CreateControl(REFCLSID clsid, LPCTSTR lpszWindowName, DWORD dwStyle,
							const RECT& rect, CWnd* pParentWnd, UINT nID, CFile* pPersist = NULL,
							BOOL bStorage = FALSE, BSTR bstrLicKey = NULL)
		{
			ASSERT(pParentWnd != NULL);

			if (pParentWnd == NULL)
				return FALSE;

			HWND hWnd = CAxWindowT<CWnd>::Create(pParentWnd->GetSafeHwnd(), (RECT*)&rect, lpszWindowName, dwStyle);

			CString csCLSID = AfxStringFromCLSID(clsid) ;

			HRESULT hResult = CAxWindowT<CWnd>::CreateControl((LPCOLESTR)(LPCTSTR)csCLSID);

			if (SUCCEEDED(hResult))
			{
				LPUNKNOWN pUnk = GetControlUnknown();
				if (pUnk != NULL)
				{
					m_pCWndParent = pParentWnd ;

					DispEventAdvise(pUnk);
				}

				return TRUE;
			}

			return FALSE ;
		}

		LPUNKNOWN GetControlUnknown()
		{
			CComPtr<IUnknown> spUnk;
			HRESULT hResult = AtlAxGetControl(m_hWnd, &spUnk);

			if (SUCCEEDED(hResult))
				return spUnk ;

			return NULL ;
		}

		void AfxBSTR2CString(CString* pStr, BSTR bstr)
		{
			ASSERT(pStr != NULL);
			if (pStr == NULL)
			{
				return;
			}

			int nLen = SysStringLen(bstr);
#if defined(_UNICODE)
			LPTSTR lpsz = pStr->GetBufferSetLength(nLen);
			ASSERT(lpsz != NULL);
			memcpy(lpsz, bstr, nLen*sizeof(TCHAR));
			pStr->ReleaseBuffer(nLen);
#else
			int nBytes = WideCharToMultiByte(CP_ACP, 0, bstr, nLen, NULL, NULL, NULL, NULL);
			LPSTR lpsz = pStr->GetBufferSetLength(nBytes);
			ASSERT(lpsz != NULL);
			WideCharToMultiByte(CP_ACP, 0, bstr, nLen, lpsz, nBytes, NULL, NULL);
			pStr->ReleaseBuffer(nBytes);
#endif

		}

		void InvokeHelper(DISPID dwDispID, WORD wFlags, VARTYPE vtRet, void* pvRet, const BYTE* pbParamInfo, ...)
		{
			LPUNKNOWN pUnk = GetControlUnknown();
			if (pUnk == NULL)
				return ;

			// no dispatch pointer yet; find it now
			LPDISPATCH pDispatch;

			if (SUCCEEDED(pUnk->QueryInterface(IID_IDispatch, (LPVOID*)&pDispatch)) == FALSE)
			{
				ASSERT(pDispatch != NULL);
				return;
			}

			va_list argList;
			va_start(argList, pbParamInfo);

			COleDispParams dispparams;
			memset(&dispparams, 0, sizeof dispparams);

			// determine number of arguments
			if (pbParamInfo != NULL)
			{
				// Number of elements will never be biger than MAX_UINT thus casting
				dispparams.cArgs = (unsigned int)(lstrlen((LPCTSTR)pbParamInfo));
			}

			DISPID dispidNamed = DISPID_PROPERTYPUT;
			if (wFlags & (DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF))
			{
				ASSERT(dispparams.cArgs > 0);
				dispparams.cNamedArgs = 1;
				dispparams.rgdispidNamedArgs = &dispidNamed;
			}
			CVariantBoolConverter tempArgs; //Used to convert VARIANT_BOOL | VT_BYREF --> BOOL*.
			if (dispparams.cArgs != 0)
			{
				// allocate memory for all VARIANT parameters
				VARIANT* pArg = new VARIANT[dispparams.cArgs];
				ASSERT(pArg != NULL);   // should have thrown exception
				dispparams.rgvarg = pArg;
				memset(pArg, 0, sizeof(VARIANT) * dispparams.cArgs);

				// get ready to walk vararg list
				const BYTE* pb = pbParamInfo;
				pArg += dispparams.cArgs - 1;   // params go in opposite order

				while (*pb != 0)
				{
					ASSERT(pArg >= dispparams.rgvarg);

					pArg->vt = *pb; // set the variant type
					if (pArg->vt & VT_MFCBYREF)
					{
						pArg->vt &= ~VT_MFCBYREF;
						pArg->vt |= VT_BYREF;
					}
					switch (pArg->vt)
					{
						case VT_UI1:
							pArg->bVal = va_arg(argList, BYTE);
							break;
						case VT_UI2:
							pArg->uiVal = va_arg(argList, USHORT);
							break;
						case VT_UI4:
							pArg->ulVal = va_arg(argList, ULONG);
							break;
#ifndef _MTL_ATL3
						case VT_UI8:
							pArg->ullVal = va_arg(argList, ULONGLONG);
							break;
#endif // _MTL_ATL3
						case VT_I1:
							pArg->cVal = va_arg(argList, char);
							break;
						case VT_I2:
							pArg->iVal = va_arg(argList, short);
							break;
						case VT_I4:
							pArg->lVal = va_arg(argList, long);
							break;
#ifndef _MTL_ATL3
						case VT_I8:
							pArg->llVal = va_arg(argList, LONGLONG);
							break;
#endif // _MTL_ATL3
						case VT_R4:
							pArg->fltVal = (float)va_arg(argList, double);
							break;
						case VT_R8:
							pArg->dblVal = va_arg(argList, double);
							break;
						case VT_DATE:
							pArg->date = va_arg(argList, DATE);
							break;
						case VT_CY:
							//CY is always passed by ref
							pArg->cyVal = *va_arg(argList, CY*);
							break;
						case VT_BSTR:
						{
							LPCOLESTR lpsz = va_arg(argList, LPOLESTR);
							pArg->bstrVal = ::SysAllocString(lpsz);
							if (lpsz != NULL && pArg->bstrVal == NULL)
							{
								AfxThrowMemoryException();
							}
						}
						break;
#if !defined(_UNICODE)
						case VT_BSTRA:
						{
							LPCSTR lpsz = va_arg(argList, LPSTR);
							CStringW strMBToUnicode(lpsz);
							pArg->bstrVal = ::SysAllocString((LPCWSTR)((LPCTSTR)strMBToUnicode));
							if (lpsz != NULL && pArg->bstrVal == NULL)
								AfxThrowMemoryException();
							pArg->vt = VT_BSTR;
						}
						break;
#endif
						case VT_DISPATCH:
							pArg->pdispVal = va_arg(argList, LPDISPATCH);
							break;
						case VT_ERROR:
							pArg->scode = va_arg(argList, SCODE);
							break;
						case VT_BOOL:
							V_BOOL(pArg) = (VARIANT_BOOL)(va_arg(argList, BOOL) ? -1 : 0);
							break;
						case VT_VARIANT:
							//VARIANT is always passed by ref
							*pArg = *va_arg(argList, VARIANT*);
							break;
						case VT_UNKNOWN:
							pArg->punkVal = va_arg(argList, LPUNKNOWN);
							break;

						case VT_UI1 | VT_BYREF:
							pArg->pbVal = va_arg(argList, BYTE*);
							break;
						case VT_UI2 | VT_BYREF:
							pArg->puiVal = va_arg(argList, USHORT*);
							break;
						case VT_UI4 | VT_BYREF:
							pArg->pulVal = va_arg(argList, ULONG*);
							break;
#ifndef _MTL_ATL3
						case VT_UI8 | VT_BYREF:
							pArg->pullVal = va_arg(argList, ULONGLONG*);
							break;
#endif _MTL_ATL3
						case VT_I1 | VT_BYREF:
							pArg->pcVal = va_arg(argList, char*);
							break;
						case VT_I2 | VT_BYREF:
							pArg->piVal = va_arg(argList, short*);
							break;
						case VT_I4 | VT_BYREF:
							pArg->plVal = va_arg(argList, long*);
							break;
#ifndef _MTL_ATL3
						case VT_I8 | VT_BYREF:
							pArg->pllVal = va_arg(argList, LONGLONG*);
							break;
#endif // _MTL_ATL3
						case VT_R4 | VT_BYREF:
							pArg->pfltVal = va_arg(argList, float*);
							break;
						case VT_R8 | VT_BYREF:
							pArg->pdblVal = va_arg(argList, double*);
							break;
						case VT_DATE | VT_BYREF:
							pArg->pdate = va_arg(argList, DATE*);
							break;
						case VT_CY | VT_BYREF:
							pArg->pcyVal = va_arg(argList, CY*);
							break;
						case VT_BSTR | VT_BYREF:
							pArg->pbstrVal = va_arg(argList, BSTR*);
							break;
						case VT_DISPATCH | VT_BYREF:
							pArg->ppdispVal = va_arg(argList, LPDISPATCH*);
							break;
						case VT_ERROR | VT_BYREF:
							pArg->pscode = va_arg(argList, SCODE*);
							break;
						case VT_BOOL | VT_BYREF:
						{
							// coerce BOOL into VARIANT_BOOL
							BOOL* pboolVal = va_arg(argList, BOOL*);
							*pboolVal = *pboolVal ? MAKELONG(0xffff, 0) : 0;
							pArg->pboolVal = (VARIANT_BOOL*)pboolVal;
							tempArgs.AddPair(CVariantBoolPair(pboolVal, pArg->pboolVal, FALSE));
						}
						break;
						case VT_VARIANT | VT_BYREF:
							pArg->pvarVal = va_arg(argList, VARIANT*);
							break;
						case VT_UNKNOWN | VT_BYREF:
							pArg->ppunkVal = va_arg(argList, LPUNKNOWN*);
							break;

						default:
							ASSERT(FALSE);  // unknown type!
							break;
					}

					--pArg; // get ready to fill next argument
					++pb;
				}
			}

			// initialize return value
			VARIANT* pvarResult = NULL;
			VARIANT vaResult;
			COleVariant::AfxVariantInit(&vaResult);
			if (vtRet != VT_EMPTY)
				pvarResult = &vaResult;

			// initialize EXCEPINFO struct
			EXCEPINFO excepInfo;
			memset(&excepInfo, 0, sizeof excepInfo);

			UINT nArgErr = (UINT)-1;  // initialize to invalid arg

			// make the call
			SCODE sc = pDispatch->Invoke(dwDispID, IID_NULL, 0, wFlags, &dispparams, pvarResult, &excepInfo, &nArgErr);

			//When VT_BOOL | VT_BYREF is passed to com server, after Invoke returns - 
			//Convert VARIANT_TRUE[FALSE] --> BOOL TRUE[FALSE].	
			tempArgs.CopyVarBoolsIntoBOOLs();
			// cleanup any arguments that need cleanup
			if (dispparams.cArgs != 0)
			{
				VARIANT* pArg = dispparams.rgvarg + dispparams.cArgs - 1;
				const BYTE* pb = pbParamInfo;
				while (*pb != 0)
				{
					switch ((VARTYPE)*pb)
					{
#if !defined(_UNICODE)
						case VT_BSTRA:
#endif
						case VT_BSTR:
							VariantClear(pArg);
							break;
					}
					--pArg;
					++pb;
				}
			}
			delete[] dispparams.rgvarg;
			dispparams.rgvarg = NULL;

			// throw exception on failure
			if (FAILED(sc))
			{
				VariantClear(&vaResult);
				if (sc != DISP_E_EXCEPTION)
				{
					// non-exception error code
					AfxThrowOleException(sc);
				}

				// make sure excepInfo is filled in
				if (excepInfo.pfnDeferredFillIn != NULL)
					excepInfo.pfnDeferredFillIn(&excepInfo);

				// allocate new exception, and fill it
				COleDispatchException* pException = new COleDispatchException(NULL, 0, excepInfo.wCode);

				ASSERT(pException->m_wCode == excepInfo.wCode);

				if (::SysStringLen(excepInfo.bstrSource))
				{
					pException->m_strSource = excepInfo.bstrSource;
				}
				::SysFreeString(excepInfo.bstrSource);
				if (::SysStringLen(excepInfo.bstrDescription))
				{
					pException->m_strDescription = excepInfo.bstrDescription;
				}
				::SysFreeString(excepInfo.bstrDescription);
				if (::SysStringLen(excepInfo.bstrHelpFile))
				{
					pException->m_strHelpFile = excepInfo.bstrHelpFile;
				}
				::SysFreeString(excepInfo.bstrHelpFile);
				pException->m_dwHelpContext = excepInfo.dwHelpContext;
				pException->m_scError = excepInfo.scode;

				// then throw the exception
				THROW(pException);
			}

			if (vtRet != VT_EMPTY)
			{
				// convert return value
				if (vtRet != VT_VARIANT)
				{
					SCODE scChangeType = VariantChangeType(&vaResult, &vaResult, 0, vtRet);
					if (FAILED(scChangeType))
					{
						TRACE(traceOle, 0, "Warning: automation return value coercion failed.\n");
						VariantClear(&vaResult);
						AfxThrowOleException(scChangeType);
					}
					ASSERT(vtRet == vaResult.vt);
				}

				// copy return value into return spot!
				switch (vtRet)
				{
					case VT_UI1:
						*(BYTE*)pvRet = vaResult.bVal;
						break;
					case VT_UI2:
						*(USHORT*)pvRet = vaResult.uiVal;
						break;
					case VT_UI4:
						*(ULONG*)pvRet = vaResult.ulVal;
						break;
#ifndef _MTL_ATL3
					case VT_UI8:
						*(ULONGLONG*)pvRet = vaResult.ullVal;
						break;
#endif // _MTL_ATL3
					case VT_I1:
						*(char*)pvRet = vaResult.cVal;
						break;
					case VT_I2:
						*(short*)pvRet = vaResult.iVal;
						break;
					case VT_I4:
						*(long*)pvRet = vaResult.lVal;
						break;
#ifndef _MTL_ATL3
					case VT_I8:
						*(LONGLONG*)pvRet = vaResult.llVal;
						break;
#endif // _MTL_ATL3
					case VT_R4:
						*(float*)pvRet = vaResult.fltVal;
						break;
					case VT_R8:
						*(double*)pvRet = vaResult.dblVal;
						break;
					case VT_DATE:
						*(double*)pvRet = *(double*)&vaResult.date;
						break;
					case VT_CY:
						*(CY*)pvRet = vaResult.cyVal;
						break;
					case VT_BSTR:
						AfxBSTR2CString((CString*)pvRet, vaResult.bstrVal);
						SysFreeString(vaResult.bstrVal);
						break;
					case VT_DISPATCH:
						*(LPDISPATCH*)pvRet = vaResult.pdispVal;
						break;
					case VT_ERROR:
						*(SCODE*)pvRet = vaResult.scode;
						break;
					case VT_BOOL:
						*(BOOL*)pvRet = (V_BOOL(&vaResult) != 0);
						break;
					case VT_VARIANT:
						*(VARIANT*)pvRet = vaResult;
						break;
					case VT_UNKNOWN:
						*(LPUNKNOWN*)pvRet = vaResult.punkVal;
						break;

					default:
						ASSERT(FALSE);  // invalid return type specified
				}
			}

			va_end(argList);
		}


};


/*============================================================================*/
// CHtmlView

class CHtmlView : public CFormView
{
	DECLARE_DYNCREATE(CHtmlView)

	public:
		CHtmlView() : CFormView((LPCTSTR)NULL)
		{
		}

	public:
		DECLARE_MESSAGE_MAP()

		DECLARE_EVENTSINK_MAP()


		::ATL::CComPtr<IWebBrowser2> m_pBrowserApp;

		CAxWnd<AFX_IDW_PANE_FIRST, &DIID_DWebBrowserEvents2> m_wndBrowser;

	public:
		/////////////////////////////////////////////////////////////////////////////
		// CHtmlView Events

		virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
											LPCTSTR lpszTargetFrameName, CByteArray& baPostData,
											LPCTSTR lpszHeaders, BOOL* bCancel)
		{
			// default to continuing
			*bCancel = FALSE;

			// user will override to handle this notification
			UNUSED_ALWAYS(lpszURL);
			UNUSED_ALWAYS(nFlags);
			UNUSED_ALWAYS(lpszTargetFrameName);
			UNUSED_ALWAYS(baPostData);
			UNUSED_ALWAYS(lpszHeaders);
		}
		virtual void OnCommandStateChange(long lCommand, BOOL bEnable) {} // user will override to handle this notification
		virtual void OnDocumentComplete(LPCTSTR lpszURL) {} // user will override to handle this notification
		virtual void OnDownloadBegin() {} // user will override to handle this notification
		virtual void OnDownloadComplete() {} // user will override to handle this notification
		virtual void OnFullScreen(BOOL bFullScreen) {} // user will override to handle this notification
		virtual void OnMenuBar(BOOL bMenuBar) {} // user will override to handle this notification
		virtual void OnNavigateComplete2(LPCTSTR lpszURL) {} // user will override to handle this notification
#ifndef  _MTL_ATL3
		virtual void OnNavigateError(LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel)
		{
			// default to continuing
			*pbCancel = FALSE;

			// user will override to handle this notification
			UNUSED_ALWAYS(lpszURL);
			UNUSED_ALWAYS(lpszFrame);

			TRACE(traceHtml, 0, "OnNavigateError called with status scode = 0x%X\n", dwError);
		}
#endif //  _MTL_ATL3
		virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* bCancel)
		{
			// default to continuing
			*bCancel = FALSE;

			// user will override to handle this notification
			UNUSED_ALWAYS(ppDisp);
		}

		/////////////////////////////////////////////////////////////////////////////
		// CHtmlView printing

		void OnFilePrint()
		{
			// get the HTMLDocument

			if (m_pBrowserApp != NULL)
			{
				CComPtr<IDispatch> spDisp;
				m_pBrowserApp->get_Document(&spDisp);
				if (spDisp != NULL)
				{
					// the control will handle all printing UI

					CComQIPtr<IOleCommandTarget> spTarget = spDisp;
					if (spTarget != NULL)
						spTarget->Exec(NULL, OLECMDID_PRINT, 0, NULL, NULL);
				}
			}
		}

		virtual void OnProgressChange(long lProgress, long lProgressMax) {} // user will override to handle this notification
		virtual void OnPropertyChange(LPCTSTR lpszProperty) {} // user will override to handle this notification
		virtual void OnQuit() {} // user will override to handle this notification
		virtual void OnStatusTextChange(LPCTSTR pszText)
		{
			// try to set the status bar text via the frame

			CFrameWnd* pFrame = GetParentFrame();
			if (pFrame != NULL)
				pFrame->SetMessageText(pszText);
		}
		virtual void OnTheaterMode(BOOL bTheaterMode) {} // user will override to handle this notification
		virtual void OnTitleChange(LPCTSTR lpszText) {} // user will override to handle this notification
		virtual void OnToolBar(BOOL bToolBar) {} // user will override to handle this notification
		virtual void OnStatusBar(BOOL bStatusBar) {} // user will override to handle this notification
		virtual void OnVisible(BOOL bVisible) {} // user will override to handle this notification


		void OnPaint() { Default(); }

		void OnSize(UINT nType, int cx, int cy)
		{
			CFormView::OnSize(nType, cx, cy);

			if (::IsWindow(m_wndBrowser.m_hWnd))
			{
				// need to push non-client borders out of the client area
				CRect rect;
				GetClientRect(rect);
				::AdjustWindowRectEx(rect, m_wndBrowser.GetStyle(), FALSE, WS_EX_CLIENTEDGE);
				m_wndBrowser.SetWindowPos(NULL, rect.left, rect.top,
											rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
			}
		}



		virtual void BeforeNavigate2(LPDISPATCH /* pDisp */, VARIANT* URL,
									VARIANT* Flags, VARIANT* TargetFrameName,
									VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel)
		{
			ASSERT(V_VT(URL) == VT_BSTR);
			ASSERT(V_VT(TargetFrameName) == VT_BSTR);
			ASSERT(V_VT(PostData) == (VT_VARIANT | VT_BYREF));
			ASSERT(V_VT(Headers) == VT_BSTR);
			ASSERT(Cancel != NULL);

			VARIANT* vtPostedData = V_VARIANTREF(PostData);
			CByteArray array;
			if (V_VT(vtPostedData) & VT_ARRAY)
			{
				// must be a vector of bytes
				ASSERT(vtPostedData->parray->cDims == 1 && vtPostedData->parray->cbElements == 1);

				vtPostedData->vt |= VT_UI1;
				COleSafeArray safe(vtPostedData);

				DWORD dwSize = safe.GetOneDimSize();
				LPVOID pVoid;
				safe.AccessData(&pVoid);

				array.SetSize(dwSize);
				LPBYTE lpByte = (LPBYTE)array.GetData();

				memcpy(lpByte, pVoid, dwSize);
				safe.UnaccessData();
			}
			// make real parameters out of the notification

			CString strTargetFrameName(V_BSTR(TargetFrameName));
			CString strURL(V_BSTR(URL));
			CString strHeaders(V_BSTR(Headers));
			DWORD nFlags = V_I4(Flags);


			BOOL bCancel = FALSE;
			// notify the user's class
			OnBeforeNavigate2(strURL, nFlags, strTargetFrameName, array, strHeaders, &bCancel);

			if (bCancel)
				*Cancel = AFX_OLE_TRUE;
			else
				*Cancel = AFX_OLE_FALSE;
		}

		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
							DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
							UINT nID, CCreateContext* pContext)
		{
			CAfxModule* pCAfxModule = CAfx::AfxModule();

			// create the view window itself
			m_pCreateContext = pContext;
			if (!CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
			{
				return FALSE;
			}

			// assure that control containment is on
			//AfxEnableControlContainer();

			RECT rectClient;
			GetClientRect(&rectClient);

			// create the control window
			// AFX_IDW_PANE_FIRST is a safe but arbitrary ID
			if (!m_wndBrowser.CreateControl(CLSID_WebBrowser, lpszWindowName,
											WS_VISIBLE | WS_CHILD, rectClient, this, AFX_IDW_PANE_FIRST))
			{
				DestroyWindow();
				return FALSE;
			}

			// cache the dispinterface
			LPUNKNOWN lpUnk = m_wndBrowser.GetControlUnknown();
			HRESULT hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**)&m_pBrowserApp);
			if (!SUCCEEDED(hr))
			{
				m_pBrowserApp = NULL;
				m_wndBrowser.DestroyWindow();
				DestroyWindow();
				return FALSE;
			}

			return TRUE;
		}


		virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
		{
			UNUSED_ALWAYS(pDisp);
			ASSERT(V_VT(URL) == VT_BSTR);

			CString str(V_BSTR(URL));
			OnDocumentComplete(str);
		}

		void ExecWB(OLECMDID cmdID, OLECMDEXECOPT cmdexecopt, VARIANT* pvaIn, VARIANT* pvaOut)
		{
			ASSERT(m_pBrowserApp != NULL);

			m_pBrowserApp->ExecWB(cmdID, cmdexecopt, pvaIn, pvaOut);
		}

		BOOL GetProperty(LPCTSTR lpszProperty, CString& strValue)
		{
			ASSERT(m_pBrowserApp != NULL);

			CString strProperty(lpszProperty);
			BSTR bstrProperty = strProperty.AllocSysString();

			BOOL bResult = FALSE;
			VARIANT vReturn;
			vReturn.vt = VT_BSTR;
			vReturn.bstrVal = NULL;
			HRESULT hr = m_pBrowserApp->GetProperty(bstrProperty, &vReturn);

			if (SUCCEEDED(hr))
			{
				strValue = CString(vReturn.bstrVal);
				bResult = TRUE;
			}

			::SysFreeString(bstrProperty);
			return bResult;
		}

		COleVariant CGetProperty(LPCTSTR lpszProperty)
		{
			COleVariant result;

			static BYTE parms[] = VTS_BSTR;
			m_wndBrowser.InvokeHelper(0x12f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, lpszProperty);

			return result;
		}

		void GoBack() { ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->GoBack(); }
		void GoForward() { ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->GoForward(); }
		void GoHome() { ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->GoHome(); }
		void GoSearch() { ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->GoSearch(); }

		void Navigate2(LPCTSTR lpszURL, DWORD dwFlags = 0,
						LPCTSTR lpszTargetFrameName = NULL, LPCTSTR lpszHeaders = NULL,
						LPVOID lpvPostData = NULL, DWORD dwPostDataLen = 0)
		{
			ASSERT(m_pBrowserApp != NULL);

			COleSafeArray vPostData;
			if (lpvPostData != NULL)
			{
				if (dwPostDataLen == 0)
					dwPostDataLen = (DWORD)(lstrlen((LPCTSTR)lpvPostData));

				vPostData.CreateOneDim(VT_UI1, dwPostDataLen, lpvPostData);
			}

			COleVariant vURL(lpszURL, VT_BSTR);
			COleVariant vHeaders(lpszHeaders, VT_BSTR);
			COleVariant vTargetFrameName(lpszTargetFrameName, VT_BSTR);
			COleVariant vFlags((long)dwFlags, VT_I4);

			m_pBrowserApp->Navigate2(vURL, vFlags, vTargetFrameName, vPostData, vHeaders);
		}

#ifndef  _MTL_ATL3
		virtual void NavigateError(LPDISPATCH pDisp, VARIANT* pvURL, VARIANT* pvFrame, VARIANT* pvStatusCode, VARIANT_BOOL* pvbCancel)
		{
			UNUSED_ALWAYS(pDisp);
			ASSERT(pvURL != NULL);
			ASSERT(pvFrame != NULL);
			ASSERT(pvStatusCode != NULL);
			ASSERT(pvbCancel != NULL);
			ASSERT(V_VT(pvURL) == VT_BSTR);
			ASSERT(V_VT(pvFrame) == VT_BSTR);

			CString strURL(V_BSTR(pvURL));
			CString strFrame(V_BSTR(pvFrame));
			DWORD dwError = V_I4(pvStatusCode);

			BOOL bCancel = FALSE;
			// notify the user's class
			OnNavigateError(strURL, strFrame, dwError, &bCancel);

			if (pvbCancel)
				*pvbCancel = bCancel ? AFX_OLE_TRUE : AFX_OLE_FALSE;
		}
#endif //  _MTL_ATL3
		virtual void NavigateComplete2(LPDISPATCH /* pDisp */, VARIANT* URL)
		{
			ASSERT(V_VT(URL) == VT_BSTR);

			CString str(V_BSTR(URL));
			OnNavigateComplete2(str);
		}

		virtual BOOL PreCreateWindow(CREATESTRUCT& cs)
		{
			cs.style |= WS_CLIPCHILDREN;

			return CFormView::PreCreateWindow(cs);
		}

		virtual BOOL PreTranslateMessage(MSG* pMsg)
		{
			ASSERT(pMsg != NULL);
			ASSERT_VALID(this);
			ASSERT(m_hWnd != NULL);

			// allow tooltip messages to be filtered (skip CFormView)
			if (CView::PreTranslateMessage(pMsg))
				return TRUE;

			// don't translate dialog messages when in Shift+F1 help mode
			//CFrameWnd* pFrameWnd = GetTopLevelFrame();
			//if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
			//	return FALSE;

			// call all frame windows' PreTranslateMessage first
			CFrameWnd* pFrameWnd = GetParentFrame();   // start with first parent frame
			while (pFrameWnd != NULL)
			{
				// allow owner & frames to translate
				if (pFrameWnd->PreTranslateMessage(pMsg))
					return TRUE;

				// try parent frames until there are no parent frames
				pFrameWnd = pFrameWnd->GetParentFrame();
			}

			// check if the browser control wants to handle the message
			BOOL bRet = FALSE;
			if (m_pBrowserApp != NULL)
			{
				CComQIPtr<IOleInPlaceActiveObject> spInPlace = m_pBrowserApp;
				if (spInPlace)
					bRet = (spInPlace->TranslateAccelerator(pMsg) == S_OK) ? TRUE : FALSE;
			}

			return bRet;
		}


		void PutProperty(LPCTSTR lpszProperty, const VARIANT& vtValue)
		{
			ASSERT(m_pBrowserApp != NULL);

			CString strProp(lpszProperty);
			BSTR bstrProp = strProp.AllocSysString();
			m_pBrowserApp->PutProperty(bstrProp, vtValue);
			::SysFreeString(bstrProp);
		}

		void PutProperty(LPCTSTR lpszPropertyName, double dValue)
		{
			ASSERT(m_pBrowserApp != NULL); ASSERT(m_pBrowserApp != NULL); PutProperty(lpszPropertyName, COleVariant(dValue));
		}
		void PutProperty(LPCTSTR lpszPropertyName, long lValue)
		{
			ASSERT(m_pBrowserApp != NULL); ASSERT(m_pBrowserApp != NULL); PutProperty(lpszPropertyName, COleVariant(lValue, VT_UI4));
		}
		void PutProperty(LPCTSTR lpszPropertyName, LPCTSTR lpszValue)
		{
			ASSERT(m_pBrowserApp != NULL); ASSERT(m_pBrowserApp != NULL); PutProperty(lpszPropertyName, COleVariant(lpszValue, VT_BSTR));
		}
		void PutProperty(LPCTSTR lpszPropertyName, short nValue)
		{
			ASSERT(m_pBrowserApp != NULL); ASSERT(m_pBrowserApp != NULL); PutProperty(lpszPropertyName, COleVariant(nValue, VT_UI2));
		}

		void Refresh() { ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->Refresh(); }
		void Refresh2(int nLevel)
		{
			ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->Refresh2(COleVariant((long)nLevel, VT_I4));
		}

		void SetAddressBar(BOOL bNewValue)
		{
			ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_AddressBar((short)(bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
		}
		void SetFullScreen(BOOL bNewValue)
		{
			ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_FullScreen((short)(bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
		}
		void SetHeight(long nNewValue) { ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_Height(nNewValue); }
		void SetLeft(long nNewValue) { ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_Left(nNewValue); }
		void SetMenuBar(BOOL bNewValue)
		{
			ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_MenuBar((short)(bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
		}
		void SetOffline(BOOL bNewValue)
		{
			ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_Offline((short)(bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
		}
		void SetRegisterAsBrowser(BOOL bNewValue)
		{
			ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_RegisterAsBrowser((short)(bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
		}
		void SetRegisterAsDropTarget(BOOL bNewValue)
		{
			ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_RegisterAsDropTarget((short)(bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
		}
		void SetSilent(BOOL bNewValue)
		{
			ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_Silent((short)(bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
		}
		void SetStatusBar(BOOL bNewValue)
		{
			ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_StatusBar((short)(bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
		}
		void SetTheaterMode(BOOL bNewValue)
		{
			ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_TheaterMode((short)(bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
		}
		void SetToolBar(int nNewValue)
		{
			ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_ToolBar(nNewValue);
		}
		void SetTop(long nNewValue) { ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_Top(nNewValue); }
		void SetVisible(BOOL bNewValue)
		{
			ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_Visible((short)(bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
		}
		void SetWidth(long nNewValue) { ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_Width(nNewValue); }

		void Stop() { ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->Stop(); }



} ;

BEGIN_MESSAGE_MAP(CHtmlView, CFormView)
	ON_WM_PAINT()
	ON_WM_SIZE()

	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CHtmlView, CFormView)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_BEFORENAVIGATE2, BeforeNavigate2, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_COMMANDSTATECHANGE, OnCommandStateChange, VTS_I4 VTS_BOOL)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_DOCUMENTCOMPLETE, DocumentComplete, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_DOWNLOADBEGIN, OnDownloadBegin, VTS_NONE)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_DOWNLOADCOMPLETE, OnDownloadComplete, VTS_NONE)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_NAVIGATECOMPLETE2, NavigateComplete2, VTS_DISPATCH VTS_PVARIANT)
#ifndef _MTL_ATL3
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_NAVIGATEERROR, NavigateError, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
#endif //  _MTL_ATL3
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_NEWWINDOW2, OnNewWindow2, VTS_PDISPATCH VTS_PBOOL)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_ONFULLSCREEN, OnFullScreen, VTS_BOOL)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_ONMENUBAR, OnMenuBar, VTS_BOOL)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_ONQUIT, OnQuit, VTS_NONE)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_ONSTATUSBAR, OnStatusBar, VTS_BOOL)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_ONTHEATERMODE, OnTheaterMode, VTS_BOOL)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_ONTOOLBAR, OnToolBar, VTS_BOOL)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_ONVISIBLE, OnVisible, VTS_BOOL)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_PROGRESSCHANGE, OnProgressChange, VTS_I4 VTS_I4)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_PROPERTYCHANGE, OnPropertyChange, VTS_BSTR)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_STATUSTEXTCHANGE, OnStatusTextChange, VTS_BSTR)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_TITLECHANGE, OnTitleChange, VTS_BSTR)
END_EVENTSINK_MAP()
