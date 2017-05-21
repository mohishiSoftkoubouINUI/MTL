




class CTime
{
	public:
		CTime() : m_time(0) {}
		CTime(__time64_t time) : m_time(time) {}
		CTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST = -1)
		{
			ENSURE(nYear >= 1900);
			ENSURE(nMonth >= 1 && nMonth <= 12);
			ENSURE(nDay >= 1 && nDay <= 31);
			ENSURE(nHour >= 0 && nHour <= 23);
			ENSURE(nMin >= 0 && nMin <= 59);
			ENSURE(nSec >= 0 && nSec <= 59);

			struct tm atm;

			atm.tm_sec = nSec;
			atm.tm_min = nMin;
			atm.tm_hour = nHour;
			atm.tm_mday = nDay;
			atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
			atm.tm_year = nYear - 1900;     // tm_year is 1900 based
			atm.tm_isdst = nDST;

			m_time = _mktime64(&atm);
			ASSERT(m_time != -1);       // indicates an illegal input time
			if (m_time == -1)
			{
				AfxThrowInvalidArgException();
			}
		}

		CTime(const SYSTEMTIME& sysTime, int nDST = -1)
		{
			if (sysTime.wYear < 1900)
			{
				__time64_t time0 = 0L;
				CTime timeT(time0);
				*this = timeT;
			}
			else
			{
				CTime timeT((int)sysTime.wYear, (int)sysTime.wMonth, (int)sysTime.wDay,
							(int)sysTime.wHour, (int)sysTime.wMinute, (int)sysTime.wSecond, nDST);
				*this = timeT;
			}
		}

		CTime(const FILETIME& fileTime, int nDST = -1)
		{
			// first convert file time (UTC time) to local time
			FILETIME localTime;
			if (!FileTimeToLocalFileTime(&fileTime, &localTime))
			{
				m_time = 0;
				AfxThrowInvalidArgException();
				return;
			}

			// then convert that time to system time
			SYSTEMTIME sysTime;
			if (!FileTimeToSystemTime(&localTime, &sysTime))
			{
				m_time = 0;
				AfxThrowInvalidArgException();
				return;
			}

			// then convert the system time to a time_t (C-runtime local time)
			CTime timeT(sysTime, nDST);
			*this = timeT;
		}

	public:
		__time64_t m_time;

	public:
		static CTime GetCurrentTime() { return(CTime(::_time64(NULL))) ; }

		static BOOL IsValidFILETIME(const FILETIME& fileTime)
		{
			FILETIME localTime;
			if (!FileTimeToLocalFileTime(&fileTime, &localTime))
			{
				return FALSE;
			}

			// then convert that time to system time
			SYSTEMTIME sysTime;
			if (!FileTimeToSystemTime(&localTime, &sysTime))
			{
				return FALSE;
			}

			return TRUE;
		}

		struct tm* CTime::GetLocalTm(struct tm* ptm) const
		{
			// Ensure ptm is valid
			ENSURE(ptm != NULL);

			if (ptm != NULL)
			{
				struct tm ptmTemp;
				errno_t err = _localtime64_s(&ptmTemp, &m_time);

				if (err != 0)
				{
					return NULL;    // indicates that m_time was not initialized!
				}

				*ptm = ptmTemp;
				return ptm;
			}

			return NULL;
		}

		__time64_t GetTime() const { return(m_time); }

		int GetYear() const
		{
			struct tm ttm;

			struct tm* ptm = GetLocalTm(&ttm);
			return ptm ? (ptm->tm_year) + 1900 : 0;
		}

		int GetMonth() const
		{
			struct tm ttm;

			struct tm* ptm = GetLocalTm(&ttm);
			return ptm ? ptm->tm_mon + 1 : 0;
		}

		int GetDay() const
		{
			struct tm ttm;

			struct tm* ptm = GetLocalTm(&ttm);
			return ptm ? ptm->tm_mday : 0;
		}

		int GetHour() const
		{
			struct tm ttm;

			struct tm* ptm = GetLocalTm(&ttm);
			return ptm ? ptm->tm_hour : -1;
		}

		int GetMinute() const
		{
			struct tm ttm;

			struct tm* ptm = GetLocalTm(&ttm);
			return ptm ? ptm->tm_min : -1;
		}

		int GetSecond() const
		{
			struct tm ttm;

			struct tm* ptm = GetLocalTm(&ttm);
			return ptm ? ptm->tm_sec : -1;
		}

		int GetDayOfWeek() const
		{
			struct tm ttm;

			struct tm* ptm = GetLocalTm(&ttm);
			return ptm ? ptm->tm_wday + 1 : 0;
		}

};

/*============================================================================*/
// File status

struct CFileStatus
{
	CTime m_ctime;          // creation date/time of file
	CTime m_mtime;          // last modification date/time of file
	CTime m_atime;          // last access date/time of file
	ULONGLONG m_size;       // logical size of file in bytes
	DWORD m_attribute;      // logical OR of CFile::Attribute enum values
	TCHAR m_szFullName[_MAX_PATH]; // absolute path name
};


class CFile
{
	public:
		CFile()
		{
			CommonBaseInit(INVALID_HANDLE_VALUE, NULL);
		}

		~CFile()
		{
			if (m_hFile != INVALID_HANDLE_VALUE && m_bCloseOnDelete)
			{
				Close();
			}
		}


	public:
		// Flag values
		enum OpenFlags {
			modeRead = (int)0x00000,
			modeWrite = (int)0x00001,
			modeReadWrite = (int)0x00002,
			shareCompat = (int)0x00000,
			shareExclusive = (int)0x00010,
			shareDenyWrite = (int)0x00020,
			shareDenyRead = (int)0x00030,
			shareDenyNone = (int)0x00040,
			modeNoInherit = (int)0x00080,
#ifdef _UNICODE
			typeUnicode = (int)0x00400, // used in derived classes (e.g. CStdioFile) only
#endif
			modeCreate = (int)0x01000,
			modeNoTruncate = (int)0x02000,
			typeText = (int)0x04000, // used in derived classes (e.g. CStdioFile) only
			typeBinary = (int)0x08000, // used in derived classes (e.g. CStdioFile) only
			osNoBuffer = (int)0x10000,
			osWriteThrough = (int)0x20000,
			osRandomAccess = (int)0x40000,
			osSequentialScan = (int)0x80000,
		};

		enum Attribute {
			normal = 0x00,                // note: not same as FILE_ATTRIBUTE_NORMAL
			readOnly = FILE_ATTRIBUTE_READONLY,
			hidden = FILE_ATTRIBUTE_HIDDEN,
			system = FILE_ATTRIBUTE_SYSTEM,
			volume = 0x08,
			directory = FILE_ATTRIBUTE_DIRECTORY,
			archive = FILE_ATTRIBUTE_ARCHIVE,
			//device = FILE_ATTRIBUTE_DEVICE,
			temporary = FILE_ATTRIBUTE_TEMPORARY,
			sparse = FILE_ATTRIBUTE_SPARSE_FILE,
			reparsePt = FILE_ATTRIBUTE_REPARSE_POINT,
			compressed = FILE_ATTRIBUTE_COMPRESSED,
			offline = FILE_ATTRIBUTE_OFFLINE,
			notIndexed = FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
			encrypted = FILE_ATTRIBUTE_ENCRYPTED
		};

		enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

		HANDLE m_hFile;
		BOOL m_bCloseOnDelete;
		CString m_strFileName;

		//CAtlTransactionManager* m_pTM;

	public:
		static void AfxThrowFileException(int cause, LONG lOsError, LPCTSTR lpszFileName = NULL)
		{
			THROW(new CFileException(cause, lOsError, lpszFileName));
		}

		void ThrowOsError()
		{
#ifndef MTL_REMOVE_CEXCEPTION_DETAIL
			CFileException::ThrowOsError((LONG)::GetLastError(), m_strFileName);
#else //  MTL_REMOVE_CEXCEPTION_DETAIL
			THROW(new CFileException(CFileException::genericException));
#endif //  MTL_REMOVE_CEXCEPTION_DETAIL
		}


		virtual void Abort()
		{
			ASSERT_VALID(this);
			if (m_hFile != INVALID_HANDLE_VALUE)
			{
				// close but ignore errors
				::CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;
			}
			m_strFileName.Empty();
		}

		void CommonBaseInit(HANDLE hFile, CAtlTransactionManager* pTM)
		{
			m_hFile = hFile;
			m_bCloseOnDelete = FALSE;
			//m_pTM = pTM;
		}

		virtual void Close()
		{
			ASSERT_VALID(this);
			ASSERT(m_hFile != INVALID_HANDLE_VALUE);
			BOOL bError = FALSE;
			if (m_hFile != INVALID_HANDLE_VALUE)
				bError = !::CloseHandle(m_hFile);

			m_hFile = INVALID_HANDLE_VALUE;
			m_bCloseOnDelete = FALSE;
			m_strFileName.Empty();

			if (bError)
				CFileException::ThrowOsError((LONG)::GetLastError(), m_strFileName);
		}

		virtual CString GetFilePath() const
		{
			ASSERT_VALID(this);

			CFileStatus status;
			GetStatus(status);
			return status.m_szFullName;
		}

		virtual ULONGLONG GetLength() const
		{
			ASSERT_VALID(this);

			ULARGE_INTEGER liSize;
			liSize.LowPart = ::GetFileSize(m_hFile, &liSize.HighPart);
			if (liSize.LowPart == INVALID_FILE_SIZE)
				if (::GetLastError() != NO_ERROR)
					CFileException::ThrowOsError((LONG)::GetLastError(), m_strFileName);

			return liSize.QuadPart;
		}

		BOOL GetStatus(CFileStatus& rStatus) const
		{
			ASSERT_VALID(this);

			memset(&rStatus, 0, sizeof(CFileStatus));

			// copy file name from cached m_strFileName
			lstrcpyn(rStatus.m_szFullName, m_strFileName, _countof(rStatus.m_szFullName));

			if (m_hFile != NULL)
			{
				// get time current file size
				FILETIME ftCreate, ftAccess, ftModify;
				if (!::GetFileTime(m_hFile, &ftCreate, &ftAccess, &ftModify))
					return FALSE;


				LARGE_INTEGER li;

				if (::GetFileSizeEx(m_hFile, &li) == 0)
					return FALSE;

				rStatus.m_size = li.QuadPart;


				if (m_strFileName.IsEmpty())
				{
					rStatus.m_attribute = 0;
				}
				else
				{
					DWORD dwAttribute = ::GetFileAttributes(m_strFileName);

					// don't return an error for this because previous versions of MFC didn't
					if (dwAttribute == 0xFFFFFFFF)
					{
						rStatus.m_attribute = 0;
					}
					else
					{
						rStatus.m_attribute = dwAttribute;
					}
				}

				// convert times as appropriate
				// some file systems may not record file creation time, file access time etc
				if (CTime::IsValidFILETIME(ftCreate))
				{
					rStatus.m_ctime = CTime(ftCreate);
				}
				else
				{
					rStatus.m_ctime = CTime();
				}

				if (CTime::IsValidFILETIME(ftAccess))
				{
					rStatus.m_atime = CTime(ftAccess);
				}
				else
				{
					rStatus.m_atime = CTime();
				}

				if (CTime::IsValidFILETIME(ftModify))
				{
					rStatus.m_mtime = CTime(ftModify);
				}
				else
				{
					rStatus.m_mtime = CTime();
				}

				if (rStatus.m_ctime.GetTime() == 0)
					rStatus.m_ctime = rStatus.m_mtime;

				if (rStatus.m_atime.GetTime() == 0)
					rStatus.m_atime = rStatus.m_mtime;
			}
			return TRUE;
		}

		virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pException = NULL)
		{
			ASSERT_VALID(this);

			ASSERT((nOpenFlags & typeText) == 0);   // text mode not supported

			// shouldn't open an already open file (it will leak)
			ASSERT(m_hFile == INVALID_HANDLE_VALUE);

			// CFile objects are always binary and CreateFile does not need flag
			nOpenFlags &= ~(UINT)typeBinary;

			m_bCloseOnDelete = FALSE;

			m_hFile = INVALID_HANDLE_VALUE;
			m_strFileName.Empty();

			TCHAR szTemp[_MAX_PATH];
			if (lpszFileName != NULL)
			{
				if (_AfxFullPath2(szTemp, lpszFileName, pException) == FALSE)
					return FALSE;
			}
			else
			{
				// user passed in a buffer greater then _MAX_PATH
#ifndef MTL_REMOVE_CEXCEPTION_DETAIL
				if (pException != NULL)
				{
					pException->m_cause = CFileException::badPath;
					pException->m_strFileName = lpszFileName;
				}
#endif //  MTL_REMOVE_CEXCEPTION_DETAIL
				return FALSE; // path is too long
			}

			m_strFileName = szTemp;
			ASSERT(shareCompat == 0);

			// map read/write mode
			ASSERT((modeRead | modeWrite | modeReadWrite) == 3);
			DWORD dwAccess = 0;
			switch (nOpenFlags & 3)
			{
				case modeRead:
					dwAccess = GENERIC_READ;
					break;
				case modeWrite:
					dwAccess = GENERIC_WRITE;
					break;
				case modeReadWrite:
					dwAccess = GENERIC_READ | GENERIC_WRITE;
					break;
				default:
					ASSERT(FALSE);  // invalid share mode
			}

			// map share mode
			DWORD dwShareMode = 0;
			switch (nOpenFlags & 0x70)    // map compatibility mode to exclusive
			{
				default:
					ASSERT(FALSE);  // invalid share mode?
				case shareCompat:
				case shareExclusive:
					dwShareMode = 0;
					break;
				case shareDenyWrite:
					dwShareMode = FILE_SHARE_READ;
					break;
				case shareDenyRead:
					dwShareMode = FILE_SHARE_WRITE;
					break;
				case shareDenyNone:
					dwShareMode = FILE_SHARE_WRITE | FILE_SHARE_READ;
					break;
			}

			// Note: typeText and typeBinary are used in derived classes only.

			// map modeNoInherit flag
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(sa);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = (nOpenFlags & modeNoInherit) == 0;

			// map creation flags
			DWORD dwCreateFlag;
			if (nOpenFlags & modeCreate)
			{
				if (nOpenFlags & modeNoTruncate)
					dwCreateFlag = OPEN_ALWAYS;
				else
					dwCreateFlag = CREATE_ALWAYS;
			}
			else
				dwCreateFlag = OPEN_EXISTING;

			// special system-level access flags

			// Random access and sequential scan should be mutually exclusive
			ASSERT((nOpenFlags&(osRandomAccess | osSequentialScan)) != (osRandomAccess |
				osSequentialScan));

			DWORD dwFlags = FILE_ATTRIBUTE_NORMAL;
			if (nOpenFlags & osNoBuffer)
				dwFlags |= FILE_FLAG_NO_BUFFERING;
			if (nOpenFlags & osWriteThrough)
				dwFlags |= FILE_FLAG_WRITE_THROUGH;
			if (nOpenFlags & osRandomAccess)
				dwFlags |= FILE_FLAG_RANDOM_ACCESS;
			if (nOpenFlags & osSequentialScan)
				dwFlags |= FILE_FLAG_SEQUENTIAL_SCAN;

			// attempt file creation
			HANDLE hFile = ::CreateFile(lpszFileName, dwAccess, dwShareMode, &sa, dwCreateFlag, dwFlags, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				return FALSE;
			}
			m_hFile = hFile;
			m_bCloseOnDelete = TRUE;

			return TRUE;
		}

		static void Remove(LPCTSTR lpszFileName, CAtlTransactionManager* pTM = NULL)
		{
			BOOL bRes = ::DeleteFile((LPTSTR)lpszFileName);
		}

		virtual UINT Read(void* lpBuf, UINT nCount)
		{
			ASSERT_VALID(this);
			ASSERT(m_hFile != INVALID_HANDLE_VALUE);

			if (nCount == 0)
				return 0;   // avoid Win32 "null-read"

			ASSERT(lpBuf != NULL);

			DWORD dwRead;
			if (!::ReadFile(m_hFile, lpBuf, nCount, &dwRead, NULL))
				CFileException::ThrowOsError((LONG)::GetLastError(), m_strFileName);

			return (UINT)dwRead;
		}

		virtual void Write(const void* lpBuf, UINT nCount)
		{
			ASSERT_VALID(this);
			ASSERT(m_hFile != INVALID_HANDLE_VALUE);

			if (nCount == 0)
				return;     // avoid Win32 "null-write" option

			ASSERT(lpBuf != NULL);

			DWORD nWritten;
			if (!::WriteFile(m_hFile, lpBuf, nCount, &nWritten, NULL))
				CFileException::ThrowOsError((LONG)::GetLastError(), m_strFileName);

			if (nWritten != nCount)
				AfxThrowFileException(CFileException::diskFull, -1, m_strFileName);
		}

		virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom)
		{
			ASSERT_VALID(this);
			ASSERT(m_hFile != INVALID_HANDLE_VALUE);
			ASSERT(nFrom == begin || nFrom == end || nFrom == current);
			ASSERT(begin == FILE_BEGIN && end == FILE_END && current == FILE_CURRENT);

			LARGE_INTEGER liOff;

			liOff.QuadPart = lOff;
			liOff.LowPart = ::SetFilePointer(m_hFile, liOff.LowPart, &liOff.HighPart,
				(DWORD)nFrom);
			if (liOff.LowPart == (DWORD)-1)
				if (::GetLastError() != NO_ERROR)
					CFileException::ThrowOsError((LONG)::GetLastError(), m_strFileName);

			return liOff.QuadPart;
		}

} ;

/*============================================================================*/
// CRT functions

inline errno_t AfxCrtErrorCheck(errno_t error)
{
	switch (error)
	{
		case ENOMEM:
			AfxThrowMemoryException();
			break;
		case EINVAL:
		case ERANGE:
			AfxThrowInvalidArgException();
			break;
		case STRUNCATE:
		case 0:
			break;
		default:
			AfxThrowInvalidArgException();
			break;
	}
	return error;
}

#define AFX_CRT_ERRORCHECK(expr) AfxCrtErrorCheck(expr)

inline void __cdecl Afx_clearerr_s(FILE *stream)
{
	AFX_CRT_ERRORCHECK(::clearerr_s(stream));
}



/*============================================================================*/
// STDIO file implementation

class CStdioFile : public CFile
{
	//DECLARE_DYNAMIC(CStdioFile)

	public:
		// Constructors
		CStdioFile()
		{
			CommonBaseInit(NULL, NULL);
		}

		CStdioFile(FILE* pOpenStream)
		{
			ASSERT(pOpenStream != NULL);
			if (pOpenStream == NULL)
			{
				AfxThrowInvalidArgException();
			}

			CommonBaseInit(pOpenStream, NULL);
		}

		CStdioFile(LPCTSTR lpszFileName, UINT nOpenFlags)
		{
			CommonInit(lpszFileName, nOpenFlags, NULL);
		}

		virtual ~CStdioFile()
		{
			ASSERT_VALID(this);

			if (m_pStream != NULL && m_bCloseOnDelete)
			{
				Close();
			}
		}

	public:
		// Attributes
		FILE* m_pStream;    // stdio FILE
		// m_hFile from base class is _fileno(m_pStream)

	public:
		virtual void Abort()
		{
			ASSERT_VALID(this);

			if (m_pStream != NULL && m_bCloseOnDelete)
				fclose(m_pStream);  // close but ignore errors
			m_hFile = NULL;
			m_pStream = NULL;
			m_bCloseOnDelete = FALSE;
		}

		virtual void Close()
		{
			ASSERT_VALID(this);
			ASSERT(m_pStream != NULL);

			int nErr = 0;

			if (m_pStream != NULL)
				nErr = fclose(m_pStream);

			m_hFile = NULL;
			m_bCloseOnDelete = FALSE;
			m_pStream = NULL;

			if (nErr != 0)
				AfxThrowFileException(CFileException::diskFull, _doserrno,
				m_strFileName);
		}

		void CommonBaseInit(FILE* pOpenStream, CAtlTransactionManager* pTM)
		{
			m_pStream = pOpenStream;
			//m_pTM = pTM;

			if (pOpenStream != NULL)
			{
				m_hFile = (HANDLE)_get_osfhandle(_fileno(pOpenStream));
				ASSERT(!m_bCloseOnDelete);
			}
		}

		void CommonInit(LPCTSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM)
		{
			ASSERT(lpszFileName != NULL);
			ASSERT(AfxIsValidString(lpszFileName));
			if (lpszFileName == NULL)
			{
				AfxThrowInvalidArgException();
			}

			CommonBaseInit(NULL, pTM);

			CFileException e;
			if (!Open(lpszFileName, nOpenFlags, &e))
			{
				AfxThrowFileException(e.m_cause, e.m_lOsError, e.m_strFileName);
			}
		}

		virtual void Flush()
		{
			ASSERT_VALID(this);

			if (m_pStream != NULL && fflush(m_pStream) != 0)
				AfxThrowFileException(CFileException::diskFull, _doserrno, m_strFileName);
		}

		virtual ULONGLONG GetPosition() const
		{
			ASSERT_VALID(this);
			ASSERT(m_pStream != NULL);

			long pos = ftell(m_pStream);
			if (pos == -1)
				AfxThrowFileException(CFileException::invalidFile, _doserrno, m_strFileName);
			return pos;
		}

		virtual ULONGLONG GetLength() const
		{
			ASSERT_VALID(this);

			LONG nCurrent = ftell(m_pStream);
			if (nCurrent == -1)
				AfxThrowFileException(CFileException::invalidFile, _doserrno, m_strFileName);

			LONG nResult = fseek(m_pStream, 0, SEEK_END);
			if (nResult != 0)
				AfxThrowFileException(CFileException::badSeek, _doserrno, m_strFileName);

			LONG nLength = ftell(m_pStream);
			if (nLength == -1)
				AfxThrowFileException(CFileException::invalidFile, _doserrno, m_strFileName);
			nResult = fseek(m_pStream, nCurrent, SEEK_SET);
			if (nResult != 0)
				AfxThrowFileException(CFileException::badSeek, _doserrno, m_strFileName);

			return nLength;
		}

		// Implementation
		virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pException = NULL)
		{
			ASSERT(pException == NULL || AfxIsValidAddress(pException, sizeof(CFileException)));
			ASSERT(lpszFileName != NULL);
			ASSERT(AfxIsValidString(lpszFileName));

			if (lpszFileName == NULL)
			{
				return FALSE;
			}

			m_pStream = NULL;
			if (!CFile::Open(lpszFileName, (nOpenFlags & ~typeText), pException))
				return FALSE;

			ASSERT(m_hFile != NULL);
			ASSERT(m_bCloseOnDelete);

			char szMode[4]; // C-runtime open string
			int nMode = 0;

			// determine read/write mode depending on CFile mode
			if (nOpenFlags & modeCreate)
			{
				if (nOpenFlags & modeNoTruncate)
					szMode[nMode++] = 'a';
				else
					szMode[nMode++] = 'w';
			}
			else if (nOpenFlags & modeWrite)
				szMode[nMode++] = 'a';
			else
				szMode[nMode++] = 'r';

			// add '+' if necessary (when read/write modes mismatched)
			if (szMode[0] == 'r' && (nOpenFlags & modeReadWrite) ||
				szMode[0] != 'r' && !(nOpenFlags & modeWrite))
			{
				// current szMode mismatched, need to add '+' to fix
				szMode[nMode++] = '+';
			}

			// will be inverted if not necessary
			int nFlags = _O_RDONLY | _O_TEXT;
			if (nOpenFlags & (modeWrite | modeReadWrite))
				nFlags ^= _O_RDONLY;

			if (nOpenFlags & typeBinary)
				szMode[nMode++] = 'b', nFlags ^= _O_TEXT;
			else
				szMode[nMode++] = 't';
			szMode[nMode++] = '\0';

	#ifdef _UNICODE
			if (nOpenFlags & typeUnicode)
			{
				nFlags ^= _O_TEXT;
				nFlags |= _O_WTEXT;
			}
	#endif

			// open a C-runtime low-level file handle
			int nHandle = _open_osfhandle((UINT_PTR)m_hFile, nFlags);

			// open a C-runtime stream from that handle
			if (nHandle != -1)
				m_pStream = _fdopen(nHandle, szMode);

			if (m_pStream == NULL)
			{
				// an error somewhere along the way...
				if (pException != NULL)
				{
					pException->m_lOsError = _doserrno;
					pException->m_cause = CFileException::OsErrorToException(_doserrno);
				}

				CFile::Abort(); // close m_hFile
				return FALSE;
			}

			return TRUE;
		}

		// Operations
		// reading and writing strings
		virtual UINT Read(void* lpBuf, UINT nCount)
		{
			ASSERT_VALID(this);
			ASSERT(m_pStream != NULL);

			if (nCount == 0)
				return 0;   // avoid Win32 "null-read"

			ASSERT(AfxIsValidAddress(lpBuf, nCount));

			if (lpBuf == NULL)
			{
				AfxThrowInvalidArgException();
			}

			UINT nRead = 0;

			if ((nRead = (UINT)fread(lpBuf, sizeof(BYTE), nCount, m_pStream)) == 0 && !feof(m_pStream))
				AfxThrowFileException(CFileException::genericException, _doserrno, m_strFileName);
			if (ferror(m_pStream))
			{
				Afx_clearerr_s(m_pStream);
				AfxThrowFileException(CFileException::genericException, _doserrno, m_strFileName);
			}
			return nRead;
		}

		virtual LPTSTR ReadString(LPTSTR lpsz, UINT nMax)
		{
			ASSERT(lpsz != NULL);
			ASSERT(AfxIsValidAddress(lpsz, nMax));
			ASSERT(m_pStream != NULL);

			if (lpsz == NULL)
			{
				AfxThrowInvalidArgException();
			}

			LPTSTR lpszResult = _fgetts(lpsz, nMax, m_pStream);
			if (lpszResult == NULL && !feof(m_pStream))
			{
				Afx_clearerr_s(m_pStream);
				AfxThrowFileException(CFileException::genericException, _doserrno, m_strFileName);
			}
			return lpszResult;
		}

		virtual BOOL ReadString(CString& rString)
		{
			ASSERT_VALID(this);

			rString = _T("");    // empty string without deallocating
			const int nMaxSize = 128;
			LPTSTR lpsz = rString.GetBuffer(nMaxSize);
			LPTSTR lpszResult;
			int nLen = 0;
			for (;;)
			{
				lpszResult = _fgetts(lpsz, nMaxSize + 1, m_pStream);
				rString.ReleaseBuffer();

				// handle error/eof case
				if (lpszResult == NULL && !feof(m_pStream))
				{
					Afx_clearerr_s(m_pStream);
					AfxThrowFileException(CFileException::genericException, _doserrno, m_strFileName);
				}

				// if string is read completely or EOF
				if (lpszResult == NULL ||
					(nLen = lstrlen(lpsz)) < nMaxSize || lpsz[nLen - 1] == '\n')
					break;

				nLen = rString.GetLength();
				lpsz = rString.GetBuffer(nMaxSize + nLen) + nLen;
			}

			// remove '\n' from end of string if present
			lpsz = rString.GetBuffer(0);
			nLen = rString.GetLength();
			if (nLen != 0 && lpsz[nLen - 1] == '\n')
				rString.GetBufferSetLength(nLen - 1);

			return nLen != 0;
		}

		virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom)
		{
			ASSERT_VALID(this);
			ASSERT(nFrom == begin || nFrom == end || nFrom == current);
			ASSERT(m_pStream != NULL);

			LONG lOff32;

			if ((lOff < LONG_MIN) || (lOff > LONG_MAX))
			{
				AfxThrowFileException(CFileException::badSeek, -1, m_strFileName);
			}

			lOff32 = (LONG)lOff;
			if (fseek(m_pStream, lOff32, nFrom) != 0)
				AfxThrowFileException(CFileException::badSeek, _doserrno, m_strFileName);

			long pos = ftell(m_pStream);
			return pos;
		}

		virtual void Write(const void* lpBuf, UINT nCount)
		{
			ASSERT_VALID(this);
			ASSERT(m_pStream != NULL);
			ASSERT(AfxIsValidAddress(lpBuf, nCount, FALSE));

			if (lpBuf == NULL)
			{
				AfxThrowInvalidArgException();
			}

			if (fwrite(lpBuf, sizeof(BYTE), nCount, m_pStream) != nCount)
				AfxThrowFileException(CFileException::genericException, _doserrno, m_strFileName);
		}

		virtual void WriteString(LPCTSTR lpsz)
		{
			ASSERT(lpsz != NULL);
			ASSERT(m_pStream != NULL);

			if (lpsz == NULL)
			{
				AfxThrowInvalidArgException();
			}

			if (_fputts(lpsz, m_pStream) == _TEOF)
				AfxThrowFileException(CFileException::diskFull, _doserrno, m_strFileName);
		}


};



class CFileFind : public CObject
{
	//DECLARE_DYNAMIC(CFileFind)

	public:
		CFileFind()
		{
			m_pFoundInfo = NULL;
			m_pNextInfo = NULL;
			m_hContext = NULL;
			m_chDirSeparator = '\\';
		}

		virtual ~CFileFind() { Close(); }

		// Attributes
	public:
		void* m_pFoundInfo;
		void* m_pNextInfo;
		HANDLE m_hContext;
		CString m_strRoot;
		TCHAR m_chDirSeparator;     // not '\\' for Internet classes

	public:
		void Close()
		{
			if (m_pFoundInfo != NULL)
			{
				delete m_pFoundInfo;
				m_pFoundInfo = NULL;
			}

			if (m_pNextInfo != NULL)
			{
				delete m_pNextInfo;
				m_pNextInfo = NULL;
			}

			if (m_hContext != NULL)
			{
				if (m_hContext != INVALID_HANDLE_VALUE)
					CloseContext();
				m_hContext = NULL;
			}
		}

		virtual void CloseContext() { ::FindClose(m_hContext); }

		// Operations
		virtual BOOL FindFile(LPCTSTR pstrName = NULL, DWORD dwUnused = 0)
		{
			UNUSED_ALWAYS(dwUnused);
			Close();

			if (pstrName == NULL)
				pstrName = _T("*.*");
			else if (lstrlen(pstrName) >= (_countof(((WIN32_FIND_DATA*)m_pNextInfo)->cFileName)))
			{
				::SetLastError(ERROR_BAD_ARGUMENTS);
				return FALSE;
			}

			m_pNextInfo = new WIN32_FIND_DATA;

			WIN32_FIND_DATA *pFindData = (WIN32_FIND_DATA *)m_pNextInfo;

			lstrcpy(pFindData->cFileName, pstrName);

			m_hContext = ::FindFirstFile(pstrName, (WIN32_FIND_DATA*)m_pNextInfo);

			if (m_hContext == INVALID_HANDLE_VALUE)
			{
				DWORD dwTemp = ::GetLastError();
				Close();
				::SetLastError(dwTemp);
				return FALSE;
			}

			LPTSTR pstrRoot = m_strRoot.GetBufferSetLength(_MAX_PATH);
			LPCTSTR pstr = _tfullpath(pstrRoot, pstrName, _MAX_PATH);

			// passed name isn't a valid path but was found by the API
			ASSERT(pstr != NULL);
			if (pstr == NULL)
			{
				m_strRoot.ReleaseBuffer(0);
				Close();
				::SetLastError(ERROR_INVALID_NAME);
				return FALSE;
			}
			else
			{
				TCHAR strDrive[_MAX_DRIVE], strDir[_MAX_DIR];
				_tsplitpath(pstrRoot, strDrive, strDir, NULL, NULL);
				_tmakepath(pstrRoot, strDrive, strDir, NULL, NULL);
				m_strRoot.ReleaseBuffer(-1);
			}
			return TRUE;
		}

		virtual BOOL FindNextFile()
		{
			ASSERT(m_hContext != NULL);

			if (m_hContext == NULL)
				return FALSE;
			if (m_pFoundInfo == NULL)
				m_pFoundInfo = new WIN32_FIND_DATA;

			ASSERT_VALID(this);

			void* pTemp = m_pFoundInfo;
			m_pFoundInfo = m_pNextInfo;
			m_pNextInfo = pTemp;

			return ::FindNextFile(m_hContext, (LPWIN32_FIND_DATA)m_pNextInfo);
		}

		virtual BOOL GetCreationTime(FILETIME* pTimeStamp) const
		{
			ASSERT(m_hContext != NULL);
			ASSERT_VALID(this);

			if (m_pFoundInfo != NULL && pTimeStamp != NULL)
			{
				*pTimeStamp = ((LPWIN32_FIND_DATA)m_pFoundInfo)->ftCreationTime;
				return TRUE;
			}
			else
				return FALSE;
		}

		virtual BOOL GetCreationTime(CTime& refTime) const
		{
			ASSERT(m_hContext != NULL);
			ASSERT_VALID(this);

			if (m_pFoundInfo != NULL)
			{
				if (CTime::IsValidFILETIME(((LPWIN32_FIND_DATA)m_pFoundInfo)->ftCreationTime))
				{
					refTime = CTime(((LPWIN32_FIND_DATA)m_pFoundInfo)->ftCreationTime);
				}
				else
				{
					refTime = CTime();
				}
				return TRUE;
			}
			else
				return FALSE;
		}

		ULONGLONG GetLength() const
		{
			ASSERT(m_hContext != NULL);
			ASSERT_VALID(this);

			ULARGE_INTEGER nFileSize;

			if (m_pFoundInfo != NULL)
			{
				nFileSize.LowPart = ((LPWIN32_FIND_DATA)m_pFoundInfo)->nFileSizeLow;
				nFileSize.HighPart = ((LPWIN32_FIND_DATA)m_pFoundInfo)->nFileSizeHigh;
			}
			else
			{
				nFileSize.QuadPart = 0;
			}

			return nFileSize.QuadPart;
		}

		virtual CString GetFileName() const
		{
			ASSERT(m_hContext != NULL);
			ASSERT_VALID(this);

			CString ret;

			if (m_pFoundInfo != NULL)
				ret = ((LPWIN32_FIND_DATA)m_pFoundInfo)->cFileName;
			return ret;
		}


		virtual CString GetFilePath() const
		{
			ASSERT(m_hContext != NULL);
			ASSERT_VALID(this);

			CString strResult = m_strRoot;
			LPCTSTR pszResult;
			LPCTSTR pchLast;
			pszResult = strResult;
			pchLast = _tcsdec(pszResult, pszResult + strResult.GetLength());
			ENSURE(pchLast != NULL);
			if ((*pchLast != _T('\\')) && (*pchLast != _T('/')))
				strResult += m_chDirSeparator;
			strResult += GetFileName();
			return strResult;
		}

		virtual CString GetFileTitle() const
		{
			ASSERT(m_hContext != NULL);
			ASSERT_VALID(this);

			CString strFullName = GetFileName();
			CString strResult;

			_tsplitpath(strFullName, NULL, NULL, strResult.GetBuffer(_MAX_FNAME), NULL);

			strResult.ReleaseBuffer();
			return strResult;
		}

		virtual CString GetFileURL() const
		{
			ASSERT(m_hContext != NULL);
			ASSERT_VALID(this);

			CString strResult("file://");
			strResult += GetFilePath();
			return strResult;
		}

		virtual BOOL GetLastAccessTime(FILETIME* pTimeStamp) const
		{
			ASSERT(m_hContext != NULL);
			ASSERT(pTimeStamp != NULL);
			ASSERT_VALID(this);

			if (m_pFoundInfo != NULL && pTimeStamp != NULL)
			{
				*pTimeStamp = ((LPWIN32_FIND_DATA)m_pFoundInfo)->ftLastAccessTime;
				return TRUE;
			}
			else
				return FALSE;
		}

		virtual BOOL GetLastAccessTime(CTime& refTime) const
		{
			ASSERT(m_hContext != NULL);
			ASSERT_VALID(this);

			if (m_pFoundInfo != NULL)
			{
				if (CTime::IsValidFILETIME(((LPWIN32_FIND_DATA)m_pFoundInfo)->ftLastAccessTime))
				{
					refTime = CTime(((LPWIN32_FIND_DATA)m_pFoundInfo)->ftLastAccessTime);
				}
				else
				{
					refTime = CTime();
				}
				return TRUE;
			}
			else
				return FALSE;
		}


		virtual BOOL GetLastWriteTime(FILETIME* pTimeStamp) const
		{
			ASSERT(m_hContext != NULL);
			ASSERT(pTimeStamp != NULL);
			ASSERT_VALID(this);

			if (m_pFoundInfo != NULL && pTimeStamp != NULL)
			{
				*pTimeStamp = ((LPWIN32_FIND_DATA)m_pFoundInfo)->ftLastWriteTime;
				return TRUE;
			}
			else
				return FALSE;
		}

		virtual BOOL GetLastWriteTime(CTime& refTime) const
		{
			ASSERT(m_hContext != NULL);
			ASSERT_VALID(this);

			if (m_pFoundInfo != NULL)
			{
				if (CTime::IsValidFILETIME(((LPWIN32_FIND_DATA)m_pFoundInfo)->ftLastWriteTime))
				{
					refTime = CTime(((LPWIN32_FIND_DATA)m_pFoundInfo)->ftLastWriteTime);
				}
				else
				{
					refTime = CTime();
				}
				return TRUE;
			}
			else
				return FALSE;
		}

		virtual CString GetRoot() const
		{
			ASSERT(m_hContext != NULL);
			ASSERT_VALID(this);

			return m_strRoot;
		}

		BOOL IsArchived() const{ return MatchesMask(FILE_ATTRIBUTE_ARCHIVE); }
		BOOL IsCompressed() const { return MatchesMask(FILE_ATTRIBUTE_COMPRESSED); }
		BOOL IsDirectory() const { return MatchesMask(FILE_ATTRIBUTE_DIRECTORY); }

		virtual BOOL IsDots() const
		{
			ASSERT(m_hContext != NULL);
			ASSERT_VALID(this);

			// return TRUE if the file name is "." or ".." and
			// the file is a directory

			BOOL bResult = FALSE;
			if (m_pFoundInfo != NULL && IsDirectory())
			{
				LPWIN32_FIND_DATA pFindData = (LPWIN32_FIND_DATA)m_pFoundInfo;
				if (pFindData->cFileName[0] == '.')
				{
					if (pFindData->cFileName[1] == '\0' ||
						(pFindData->cFileName[1] == '.' &&
						pFindData->cFileName[2] == '\0'))
					{
						bResult = TRUE;
					}
				}
			}

			return bResult;
		}

		BOOL IsHidden() const { return MatchesMask(FILE_ATTRIBUTE_HIDDEN); }
		BOOL IsNormal() const { return MatchesMask(FILE_ATTRIBUTE_NORMAL); }
		BOOL IsReadOnly() const { return MatchesMask(FILE_ATTRIBUTE_READONLY); }
		BOOL IsSystem() const { return MatchesMask(FILE_ATTRIBUTE_SYSTEM); }
		BOOL IsTemporary() const { return MatchesMask(FILE_ATTRIBUTE_TEMPORARY); }

		virtual BOOL MatchesMask(DWORD dwMask) const
		{
			ASSERT(m_hContext != NULL);
			ASSERT_VALID(this);

			if (m_pFoundInfo != NULL)
				return (!!(((LPWIN32_FIND_DATA)m_pFoundInfo)->dwFileAttributes & dwMask));
			else
				return FALSE;
		}

};






// minimum buffer size
enum { nBufSizeMin = 128 };

// default amount to grow m_pLoadArray upon insert
enum { nGrowSize = 64 };
// default size of hash table in m_pStoreMap when storing
enum { nHashSize = 137 };
// default size to grow collision blocks when storing
enum { nBlockSize = 16 };

// Pointer mapping constants
#define wNullTag        ((WORD)0)           // special tag indicating NULL ptrs
#define wNewClassTag    ((WORD)0xFFFF)      // special tag indicating new CRuntimeClass
#define wClassTag       ((WORD)0x8000)      // 0x8000 indicates class tag (OR'd)
#define dwBigClassTag   ((DWORD)0x80000000) // 0x8000000 indicates big class tag (OR'd)
#define wBigObjectTag   ((WORD)0x7FFF)      // 0x7FFF indicates DWORD object tag
#define nMaxMapCount    ((DWORD)0x3FFFFFFE) // 0x3FFFFFFE last valid mapCount

// optional bit for schema number that enables object versioning
#define VERSIONABLE_SCHEMA  (0x80000000)

class CArchive
{
	public:
		CArchive(CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL)
		{
			m_strFileName = pFile->GetFilePath();

			m_nObjectSchema = -1 ;

			m_nMode = nMode;
			m_pFile = pFile;

			m_nBufSize = nBufSize;

			// initialize the buffer.  minimum size is 128
			m_lpBufStart = (BYTE*)lpBuf;

			m_pDocument = NULL ;

			if (nBufSize < nBufSizeMin)
			{
				// force use of private buffer of minimum size
				m_nBufSize = nBufSizeMin;
				m_lpBufStart = NULL;
			}
			else
				m_nBufSize = nBufSize;

			nBufSize = m_nBufSize;

			if (m_lpBufStart == NULL)
			{
				// no support for direct buffering, allocate new buffer
				m_lpBufStart = new BYTE[m_nBufSize];
			}

			m_lpBufMax = m_lpBufStart + nBufSize;
			m_lpBufCur = (IsLoading()) ? m_lpBufMax : m_lpBufStart;

			m_nMapCount = 0 ;
			m_pLoadArray = NULL ;

			m_pSchemaMap = NULL ;
		}

	public:
		enum SchemaMapReservedRefs { objTypeArrayRef = 1 };
		enum LoadArrayObjType{ typeUndefined = 0, typeCRuntimeClass = 1, typeCObject = 2 };

		// Flag values
		enum Mode { store = 0, load = 1, bNoFlushOnDelete = 2, bNoByteSwap = 4 };

		// public for advanced use
		UINT m_nObjectSchema;
		CString m_strFileName;

		CFile* m_pFile;
		BOOL m_nMode;
		int m_nBufSize;
		BYTE* m_lpBufCur;
		BYTE* m_lpBufMax;
		BYTE* m_lpBufStart;
		CDocument* m_pDocument;

		// array/map for CObject* and CRuntimeClass* load/store
		UINT m_nMapCount;
		union
		{
			CPtrArray* m_pLoadArray;
			CMapPtrToPtr* m_pStoreMap;
		};
		// map to keep track of mismatched schemas
		CMapPtrToPtr* m_pSchemaMap;

	public:
		CArchive& operator<<(int i) { return operator<<((LONG)i); }
		CArchive& operator<<(unsigned u) { return operator<<((LONG)u); }
		CArchive& operator<<(short w) { return operator<<((WORD)w); }
		CArchive& operator<<(char ch) { return operator<<((BYTE)ch); }
#ifndef _MTL_ATL3
		CArchive& operator<<(wchar_t ch) { return operator<<((WORD)ch); }
#endif
		CArchive& operator<<(bool b) { return operator <<((BYTE)(b ? 1 : 0)); }
		CArchive& operator<<(BYTE by)
		{
			if (m_lpBufCur + sizeof(BYTE) > m_lpBufMax) Flush();
			*(UNALIGNED BYTE*)m_lpBufCur = by; m_lpBufCur += sizeof(BYTE); return *this;
		}

		CArchive& operator<<(LONGLONG dwdw)
		{
			if (m_lpBufCur + sizeof(LONGLONG) > m_lpBufMax) Flush();
			*(UNALIGNED LONGLONG*)m_lpBufCur = dwdw; m_lpBufCur += sizeof(LONGLONG); return *this;
		}
		CArchive& operator<<(ULONGLONG dwdw)
		{
			if (m_lpBufCur + sizeof(ULONGLONG) > m_lpBufMax) Flush();
			*(UNALIGNED ULONGLONG*)m_lpBufCur = dwdw; m_lpBufCur += sizeof(ULONGLONG); return *this;
		}
		CArchive& operator<<(WORD w)
		{
			if (m_lpBufCur + sizeof(WORD) > m_lpBufMax) Flush();
			*(UNALIGNED WORD*)m_lpBufCur = w; m_lpBufCur += sizeof(WORD); return *this;
		}
		CArchive& operator<<(LONG l)
		{
			if (m_lpBufCur + sizeof(LONG) > m_lpBufMax) Flush();
			*(UNALIGNED LONG*)m_lpBufCur = l; m_lpBufCur += sizeof(LONG); return *this;
		}
		CArchive& operator<<(DWORD dw)
		{
			if (m_lpBufCur + sizeof(DWORD) > m_lpBufMax) Flush();
			*(UNALIGNED DWORD*)m_lpBufCur = dw; m_lpBufCur += sizeof(DWORD); return *this;
		}
		CArchive& operator<<(float f)
		{
			if (m_lpBufCur + sizeof(float) > m_lpBufMax) Flush();
			*(UNALIGNED float*)m_lpBufCur = f; m_lpBufCur += sizeof(float); return *this;
		}
		CArchive& operator<<(double d)
		{
			if (m_lpBufCur + sizeof(double) > m_lpBufMax) Flush();
			*(UNALIGNED double*)m_lpBufCur = d; m_lpBufCur += sizeof(double); return *this;
		}

		CArchive& operator>>(int& i) { return operator>>((LONG&)i); }
		CArchive& operator>>(unsigned& u) { return operator>>((LONG&)u); }
		CArchive& operator>>(short& w) { return operator>>((WORD&)w); }
		CArchive& operator>>(char& ch) { return operator>>((BYTE&)ch); }
#ifndef _MTL_ATL3
		CArchive& operator>>(wchar_t& ch) { return operator>>((WORD&)ch); }
#endif
		CArchive& operator>>(bool& b) { BYTE by; CArchive& ar = operator>>(by); b = (by ? true : false); return ar; }
		CArchive& operator>>(BYTE& by)
		{
			if (m_lpBufCur + sizeof(BYTE) > m_lpBufMax)
				FillBuffer(UINT(sizeof(BYTE) - (m_lpBufMax - m_lpBufCur)));
			by = *(UNALIGNED BYTE*)m_lpBufCur; m_lpBufCur += sizeof(BYTE); return *this;
		}

		CArchive& operator>>(LONGLONG& dwdw)
		{
			if (m_lpBufCur + sizeof(LONGLONG) > m_lpBufMax)
				FillBuffer(sizeof(LONGLONG) - (UINT)(m_lpBufMax - m_lpBufCur));
			dwdw = *(UNALIGNED LONGLONG*)m_lpBufCur; m_lpBufCur += sizeof(LONGLONG); return *this;
		}
		CArchive& operator>>(ULONGLONG& dwdw)
		{
			if (m_lpBufCur + sizeof(ULONGLONG) > m_lpBufMax)
				FillBuffer(sizeof(ULONGLONG) - (UINT)(m_lpBufMax - m_lpBufCur));
			dwdw = *(UNALIGNED ULONGLONG*)m_lpBufCur; m_lpBufCur += sizeof(ULONGLONG); return *this;
		}
		CArchive& operator>>(WORD& w)
		{
			if (m_lpBufCur + sizeof(WORD) > m_lpBufMax)
				FillBuffer(UINT(sizeof(WORD) - (m_lpBufMax - m_lpBufCur)));
			w = *(UNALIGNED WORD*)m_lpBufCur; m_lpBufCur += sizeof(WORD); return *this;
		}
		CArchive& operator>>(DWORD& dw)
		{
			if (m_lpBufCur + sizeof(DWORD) > m_lpBufMax)
				FillBuffer(UINT(sizeof(DWORD) - (m_lpBufMax - m_lpBufCur)));
			dw = *(UNALIGNED DWORD*)m_lpBufCur; m_lpBufCur += sizeof(DWORD); return *this;
		}
		CArchive& operator>>(float& f)
		{
			if (m_lpBufCur + sizeof(float) > m_lpBufMax)
				FillBuffer(UINT(sizeof(float) - (m_lpBufMax - m_lpBufCur)));
			f = *(UNALIGNED float*)m_lpBufCur; m_lpBufCur += sizeof(float); return *this;
		}
		CArchive& operator>>(double& d)
		{
			if (m_lpBufCur + sizeof(double) > m_lpBufMax)
				FillBuffer(UINT(sizeof(double) - (m_lpBufMax - m_lpBufCur)));
			d = *(UNALIGNED double*)m_lpBufCur; m_lpBufCur += sizeof(double); return *this;
		}
		CArchive& operator>>(LONG& l)
		{
			if (m_lpBufCur + sizeof(LONG) > m_lpBufMax)
				FillBuffer(UINT(sizeof(LONG) - (m_lpBufMax - m_lpBufCur)));
			l = *(UNALIGNED LONG*)m_lpBufCur; m_lpBufCur += sizeof(LONG); return *this;
		}


		// CArchive output helpers
		CArchive& operator>>(SIZE& size) { EnsureRead(&size, sizeof(SIZE)); return *this; }
		CArchive& operator>>(POINT& point) { EnsureRead(&point, sizeof(POINT)); return *this; }
		CArchive& operator>>(RECT& rect) { EnsureRead(&rect, sizeof(RECT)); return *this; }
		CArchive& operator<<(SIZE size) { Write(&size, sizeof(SIZE)); return *this; }
		CArchive& operator<<(POINT point) { Write(&point, sizeof(POINT)); return *this; }
		CArchive& operator<<(const RECT& rect) { Write(&rect, sizeof(RECT)); return *this; }

		CArchive& operator<<(const CObject* pOb) { WriteObject(pOb); return *this; }
		CArchive& operator>>(CObject*& pOb) { pOb = ReadObject(NULL); return *this; }
		CArchive& operator>>(const CObject*& pOb) { pOb = ReadObject(NULL); return *this; }




		static void AfxThrowArchiveException(int cause, LPCTSTR lpszArchiveName = NULL)
		{
			THROW(new CArchiveException(cause, lpszArchiveName));
		}


		void CheckCount()
		{
			if (m_nMapCount >= nMaxMapCount)
				AfxThrowArchiveException(CArchiveException::badIndex, m_strFileName);
		}

		void Close()
		{
			ASSERT_VALID(m_pFile);

			Flush();
			m_pFile = NULL;
		}

		void EnsureRead(void *lpBuf, UINT nCount)
		{
			UINT nRead = Read(lpBuf, nCount);
			if (nRead != nCount)
			{
				AfxThrowArchiveException(CArchiveException::endOfFile);
			}
		}

		void EnsureSchemaMapExists(CArray<LoadArrayObjType, const LoadArrayObjType&>** ppObjTypeArray = NULL)
		{
			CMapPtrToPtr* pSchemaMap = m_pSchemaMap;
			CArray<LoadArrayObjType>* pObjTypeArray = NULL;
			void* pTemp = NULL;

			// Ensure schema map exists
			if (NULL == pSchemaMap)
			{
				pSchemaMap = new CMapPtrToPtr;
			}

			// Ensure schema map is initialized
			if (pSchemaMap->Lookup((void*)((DWORD_PTR)(objTypeArrayRef)), pTemp))
			{
				pObjTypeArray = static_cast<CArray<LoadArrayObjType>*>(pTemp);
			}
			else
			{
				// initialize the object type array
				pObjTypeArray = new CArray<LoadArrayObjType>;

				// When memory is exhausted SetSize will raise exception.
				TRY
				{
					//pObjTypeArray->SetSize(1, m_nGrowSize);
					pObjTypeArray->SetSize(1);
					pSchemaMap->SetAt((void*)((DWORD_PTR)(objTypeArrayRef)), (void*)(pObjTypeArray));
				}
				CATCH_ALL(e)
				{
					delete pObjTypeArray;
					pObjTypeArray = NULL;
				}
				END_CATCH_ALL
			}

			// Set the target variables
			m_pSchemaMap = pSchemaMap;
			if (NULL != ppObjTypeArray)
			{
				*ppObjTypeArray = pObjTypeArray;
			}
		}



		void FillBuffer(UINT nAdditionalBytesNeeded)
		{
			ASSERT_VALID(m_pFile);

			ASSERT(IsLoading());
			if (!IsLoading())
			{
				return ;
			}

			ASSERT(nAdditionalBytesNeeded > 0);


			UINT nPreviouslyFilled = UINT(m_lpBufMax - m_lpBufCur);
			ULONG nTotalSizeWanted = ((ULONG)nAdditionalBytesNeeded) + nPreviouslyFilled;

			// fill up the current buffer from file

			ASSERT(m_lpBufCur != NULL);
			ASSERT(m_lpBufStart != NULL);
			ASSERT(m_lpBufMax != NULL);

			if (m_lpBufCur > m_lpBufStart)
			{
				// copy previously filled bytes to the start of the buffer
				if ((int)nPreviouslyFilled > 0)
				{
					memmove(m_lpBufStart, m_lpBufCur, nPreviouslyFilled);
					m_lpBufCur = m_lpBufStart;
					m_lpBufMax = m_lpBufStart + nPreviouslyFilled;
				}

				// read to satisfy nAdditionalBytesNeeded if possible, else fill the buffer and throw bufferFull exception
				UINT nTotalInBuffer = nPreviouslyFilled;
				UINT nLeftToRead;
				UINT nBytesRead = 0;

				// Only read what we have to, to avoid blocking waiting on data we don't immediately need
				nLeftToRead = (UINT)m_nBufSize - nPreviouslyFilled;

				BYTE* lpTemp = m_lpBufStart + nPreviouslyFilled;
				do
				{
					nBytesRead = m_pFile->Read(lpTemp, nLeftToRead);
					lpTemp = lpTemp + nBytesRead;
					nTotalInBuffer += nBytesRead;
					nLeftToRead -= nBytesRead;
				} while (nBytesRead > 0 && nLeftToRead > 0 && nTotalInBuffer < nTotalSizeWanted);

				m_lpBufCur = m_lpBufStart;
				m_lpBufMax = m_lpBufStart + nTotalInBuffer;
			}

		}

		void Flush()
		{
			ASSERT_VALID(m_pFile);

			if (IsLoading())
			{
				// unget the characters in the buffer, seek back unused amount
				if (m_lpBufMax != m_lpBufCur)
					m_pFile->Seek(-(int(m_lpBufMax - m_lpBufCur)), CFile::current);
				m_lpBufCur = m_lpBufMax;    // empty
			}
			else
			{
				// write out the current buffer to file
				if (m_lpBufCur != m_lpBufStart)
					m_pFile->Write(m_lpBufStart, ULONG(m_lpBufCur - m_lpBufStart));

				m_lpBufCur = m_lpBufStart;
			}
		}

		CFile* GetFile() const { return m_pFile; }

		BOOL IsBufferEmpty() const { return m_lpBufCur == m_lpBufMax; }
		BOOL IsByteSwapping() const { return FALSE; }
		BOOL IsLoading() const { return (m_nMode & CArchive::load) != 0; }
		BOOL IsStoring() const { return (m_nMode & CArchive::load) == 0; }

		// advanced object mapping (used for forced references)
		void MapObject(const CObject* pOb)
		{
			if (IsStoring())
			{
				if (m_pStoreMap == NULL)
				{
					// initialize the storage map
					//  (use CMapPtrToPtr because it is used for HANDLE maps too)
					//m_pStoreMap = new CMapPtrToPtr(m_nGrowSize);
					//m_pStoreMap->InitHashTable(m_nHashSize);
					m_pStoreMap = new CMapPtrToPtr();
					m_pStoreMap->SetAt(NULL, (void*)(DWORD_PTR)wNullTag);
					m_nMapCount = 1;
				}
				if (pOb != NULL)
				{
					CheckCount();
					(*m_pStoreMap)[(void*)pOb] = (void*)(DWORD_PTR)m_nMapCount++;
				}
			}
			else
			{
				if (m_pLoadArray == NULL)
				{
					// initialize the loaded object pointer array and set special values
					m_pLoadArray = new CPtrArray;
					//m_pLoadArray->SetSize(1, m_nGrowSize);
					m_pLoadArray->SetSize(1);
					ASSERT(wNullTag == 0);
					m_pLoadArray->SetAt(wNullTag, NULL);
					m_nMapCount = 1;
				}

				if (NULL == m_pSchemaMap)
				{
					CArray<LoadArrayObjType>* pObjTypeArray = NULL;
					EnsureSchemaMapExists(&pObjTypeArray);
					pObjTypeArray->InsertAt(wNullTag, typeUndefined);
				}

				if (pOb != NULL)
				{
					CheckCount();
					m_pLoadArray->InsertAt(m_nMapCount, (void*)pOb);

					void* pTemp = NULL;
					m_pSchemaMap->Lookup((void*)(DWORD_PTR)(objTypeArrayRef), pTemp);
					CArray<LoadArrayObjType>* pObjTypeArray = static_cast<CArray<LoadArrayObjType>*>(pTemp);
					ENSURE(pObjTypeArray);
					pObjTypeArray->InsertAt(m_nMapCount, typeCObject);

					m_nMapCount++;
				}
			}
		}

		UINT Read(void* lpBuf, UINT nMax)
		{
			ASSERT_VALID(m_pFile);

			if (nMax == 0)
				return 0;

			ASSERT(lpBuf != NULL);

			if (lpBuf == NULL)
				return 0;

			ASSERT(IsLoading());

			if (!IsLoading())
				return 0;


			// try to fill from buffer first
			UINT nMaxTemp = nMax;
			UINT nTemp = min(nMaxTemp, UINT(m_lpBufMax - m_lpBufCur));
			memcpy(lpBuf, m_lpBufCur, nTemp);
			m_lpBufCur += nTemp;
			lpBuf = (BYTE*)lpBuf + nTemp;
			nMaxTemp -= nTemp;

			if (nMaxTemp != 0)
			{
				ASSERT(m_lpBufCur == m_lpBufMax);

				// read rest in buffer size chunks
				nTemp = nMaxTemp - (nMaxTemp % m_nBufSize);
				UINT nRead = 0;

				UINT nLeft = nTemp;
				UINT nBytes;
				do
				{
					nBytes = m_pFile->Read(lpBuf, nLeft);
					lpBuf = (BYTE*)lpBuf + nBytes;
					nRead += nBytes;
					nLeft -= nBytes;
				} while ((nBytes > 0) && (nLeft > 0));

				nMaxTemp -= nRead;

				if (nMaxTemp > 0)
				{
					// read last chunk into buffer then copy
					if (nRead == nTemp)
					{
						ASSERT(m_lpBufCur == m_lpBufMax);
						ASSERT(nMaxTemp < (UINT)m_nBufSize);

						// fill buffer (similar to CArchive::FillBuffer, but no exception)

						UINT nLastLeft;
						UINT nLastBytes;

						nLastLeft = max(nMaxTemp, (UINT)m_nBufSize);

						BYTE* lpTemp = m_lpBufStart;
						nRead = 0;
						do
						{
							nLastBytes = m_pFile->Read(lpTemp, nLastLeft);
							lpTemp = lpTemp + nLastBytes;
							nRead += nLastBytes;
							nLastLeft -= nLastBytes;
						} while ((nLastBytes > 0) && (nLastLeft > 0) && nRead < nMaxTemp);

						m_lpBufCur = m_lpBufStart;
						m_lpBufMax = m_lpBufStart + nRead;

						// use first part for rest of read
						nTemp = min(nMaxTemp, UINT(m_lpBufMax - m_lpBufCur));
						memcpy(lpBuf, m_lpBufCur, nTemp);
						m_lpBufCur += nTemp;
						nMaxTemp -= nTemp;
					}
				}
			}
			return nMax - nMaxTemp;
		}

		// advanced versioning support

		CRuntimeClass* ReadClass(const CRuntimeClass* pClassRefRequested = NULL, UINT* pSchema = NULL, DWORD* pObTag = NULL)
		{
			ASSERT(IsLoading());    // proper direction

			if (!IsLoading())
			{
				AfxThrowArchiveException(CArchiveException::genericException, m_strFileName);
			}

			if (pClassRefRequested != NULL && pClassRefRequested->m_wSchema == 0xFFFF)
			{
				TRACE(traceAppMsg, 0, "Warning: Cannot call ReadClass/ReadObject for %hs.\n",
					pClassRefRequested->m_lpszClassName);
				AfxThrowNotSupportedException();
			}

			// make sure m_pLoadArray is initialized
			MapObject(NULL);

			// read object tag - if prefixed by wBigObjectTag then DWORD tag follows
			DWORD obTag;
			WORD wTag;
			*this >> wTag;
			if (wTag == wBigObjectTag)
				*this >> obTag;
			else
				obTag = ((wTag & wClassTag) << 16) | (wTag & ~wClassTag);

			// check for object tag (throw exception if expecting class tag)
			if (!(obTag & dwBigClassTag))
			{
				if (pObTag == NULL)
					AfxThrowArchiveException(CArchiveException::badIndex, m_strFileName);

				*pObTag = obTag;
				return NULL;
			}

			CRuntimeClass* pClassRef;
			UINT nSchema;
			if (wTag == wNewClassTag)
			{
				// new object follows a new class id
				if ((pClassRef = CRuntimeClass::Load(*this, &nSchema)) == NULL)
					AfxThrowArchiveException(CArchiveException::badClass, m_strFileName);

				// check nSchema against the expected schema
				if ((pClassRef->m_wSchema & ~VERSIONABLE_SCHEMA) != nSchema)
				{
					if (!(pClassRef->m_wSchema & VERSIONABLE_SCHEMA))
					{
						// schema doesn't match and not marked as VERSIONABLE_SCHEMA
						AfxThrowArchiveException(CArchiveException::badSchema,
							m_strFileName);
					}
					else
					{
						// they differ -- store the schema for later retrieval
						EnsureSchemaMapExists();
						ASSERT_VALID(m_pSchemaMap);
						m_pSchemaMap->SetAt(pClassRef, (void*)(DWORD_PTR)nSchema);
					}
				}
				CheckCount();
				m_pLoadArray->InsertAt(m_nMapCount, pClassRef);

				void* pTemp = NULL;
				m_pSchemaMap->Lookup((void*)(DWORD_PTR)(objTypeArrayRef), pTemp);
				CArray<LoadArrayObjType>* pObjTypeArray = (CArray<LoadArrayObjType>*)(pTemp);
				ENSURE(pObjTypeArray);

				pObjTypeArray->InsertAt(m_nMapCount, typeCRuntimeClass);
				m_nMapCount++;
			}
			else
			{
				// existing class index in obTag followed by new object
				DWORD nClassIndex = (obTag & ~dwBigClassTag);
				if (nClassIndex == 0 || nClassIndex > (DWORD)m_pLoadArray->GetUpperBound())
					AfxThrowArchiveException(CArchiveException::badIndex, m_strFileName);

				void* pTemp = NULL;
				m_pSchemaMap->Lookup((void*)(DWORD_PTR)(objTypeArrayRef), pTemp);
				CArray<LoadArrayObjType>* pObjTypeArray = (CArray<LoadArrayObjType>*)(pTemp);
				ENSURE(pObjTypeArray);

				// typeUndefined is acceptable here since the user might have added 
				// objects to m_pLoadArray without updating the object type array.
				if (typeCObject == (LoadArrayObjType)pObjTypeArray->GetAt(nClassIndex))
				{
					AfxThrowArchiveException(CArchiveException::badIndex,
						m_strFileName);
				}

				pClassRef = (CRuntimeClass*)m_pLoadArray->GetAt(nClassIndex);
				ASSERT(pClassRef != NULL);

				// determine schema stored against objects of this type
				BOOL bFound = FALSE;
				nSchema = 0;
				if (m_pSchemaMap != NULL)
				{
					bFound = m_pSchemaMap->Lookup(pClassRef, pTemp);
					if (bFound)
						nSchema = (UINT)(UINT_PTR)pTemp;
				}
				if (!bFound)
					nSchema = pClassRef->m_wSchema & ~VERSIONABLE_SCHEMA;
			}

			// check for correct derivation
			if (pClassRefRequested != NULL &&
				!pClassRef->IsDerivedFrom(pClassRefRequested))
			{
				AfxThrowArchiveException(CArchiveException::badClass, m_strFileName);
			}

			// store nSchema for later examination
			if (pSchema != NULL)
				*pSchema = nSchema;
			else
				m_nObjectSchema = nSchema;

			// store obTag for later examination
			if (pObTag != NULL)
				*pObTag = obTag;

			// return the resulting CRuntimeClass*
			return pClassRef;
		}

		DWORD_PTR ReadCount()
		{
			WORD wCount;
			*this >> wCount;
			if (wCount != 0xFFFF)
				return wCount;

			DWORD dwCount;
			*this >> dwCount;
#ifndef _WIN64
			return dwCount;
#else  // _WIN64
			if (dwCount != 0xFFFFFFFF)
				return dwCount;

			DWORD_PTR qwCount;
			*this >> qwCount;
			return qwCount;
#endif  // _WIN64
		}

		// object read/write
		CObject* ReadObject(const CRuntimeClass* pClassRefRequested)
		{
			ASSERT(IsLoading());    // proper direction

			if (!IsLoading())
			{
				AfxThrowArchiveException(CArchiveException::writeOnly, m_strFileName);
			}

			// attempt to load next stream as CRuntimeClass
			UINT nSchema;
			DWORD obTag;
			CRuntimeClass* pClassRef = ReadClass(pClassRefRequested, &nSchema, &obTag);

			// check to see if tag to already loaded object
			CObject* pOb = NULL;
			if (pClassRef == NULL)
			{
				if (obTag > (DWORD)m_pLoadArray->GetUpperBound())
				{
					// tag is too large for the number of objects read so far
					AfxThrowArchiveException(CArchiveException::badIndex, m_strFileName);
				}

				LoadArrayObjType eType = typeUndefined;
				void* pTemp = NULL;

				if (m_pSchemaMap->Lookup((void*)((DWORD_PTR)(objTypeArrayRef)), pTemp))
				{
					CArray<LoadArrayObjType>* pTypeArray = static_cast<CArray<LoadArrayObjType>*>(pTemp);
					ENSURE(pTypeArray);
					if (obTag <= (DWORD)pTypeArray->GetUpperBound())
					{
						eType = pTypeArray->GetAt(obTag);
					}
				}

				// typeUndefined is acceptable since the user might have added objects
				// to m_pLoadArray without updating the object type array.
				if (eType == typeCRuntimeClass)
				{
					AfxThrowArchiveException(CArchiveException::badIndex, m_strFileName);
				}

				pOb = (CObject*)m_pLoadArray->GetAt(obTag);
				if (pOb != NULL && pClassRefRequested != NULL &&
					!pOb->IsKindOf(pClassRefRequested))
				{
					// loaded an object but of the wrong class
					AfxThrowArchiveException(CArchiveException::badClass, m_strFileName);
				}
			}
			else
			{
				TRY
				{
					// allocate a new object based on the class just acquired
					pOb = pClassRef->CreateObject();
					if (pOb == NULL)
						AfxThrowMemoryException();

					// Add to mapping array BEFORE de-serializing
					CheckCount();

					m_pLoadArray->InsertAt(m_nMapCount, pOb);

					void* pTemp = NULL;
					m_pSchemaMap->Lookup((void*)(DWORD_PTR)(objTypeArrayRef), pTemp);
					CArray<LoadArrayObjType>* pObjTypeArray = (CArray<LoadArrayObjType>*)(pTemp);
					ENSURE(pObjTypeArray);

					pObjTypeArray->InsertAt(m_nMapCount, typeCObject);
					m_nMapCount++;

					// Serialize the object with the schema number set in the archive
					UINT nSchemaSave = m_nObjectSchema;
					m_nObjectSchema = nSchema;
					pOb->Serialize(*this);
					m_nObjectSchema = nSchemaSave;
					ASSERT_VALID(pOb);
				}
					CATCH_ALL(e)
				{
					if (pOb != NULL)
					{
						delete pOb;
						pOb = NULL;
					}
					THROW_LAST();
				}
				END_CATCH_ALL
			}

			return pOb;
		}

		void SerializeClass(const CRuntimeClass* pClassRef)
		{
			if (IsStoring())
				WriteClass(pClassRef);
			else
				ReadClass(pClassRef);
		}

		void SetObjectSchema(UINT nSchema) { m_nObjectSchema = nSchema; }

		void Write(const void* lpBuf, UINT nMax)
		{
			ASSERT_VALID(m_pFile);

			if (nMax == 0)
				return;

			ASSERT(lpBuf != NULL);

			if (lpBuf == NULL)
				return;

			ASSERT(IsStoring());

			if (!IsStoring())
				return;

			// copy to buffer if possible
			UINT nTemp = min(nMax, (UINT)(m_lpBufMax - m_lpBufCur));
			memcpy(m_lpBufCur, lpBuf, nTemp);
			m_lpBufCur += nTemp;
			lpBuf = (BYTE*)lpBuf + nTemp;
			nMax -= nTemp;

			if (nMax > 0)
			{
				Flush();    // flush the full buffer

				// write rest of buffer size chunks
				nTemp = nMax - (nMax % m_nBufSize);
				m_pFile->Write(lpBuf, nTemp);
				lpBuf = (BYTE*)lpBuf + nTemp;
				nMax -= nTemp;

				// copy remaining to active buffer
				ENSURE(nMax < (UINT)m_nBufSize);
				ENSURE(m_lpBufCur == m_lpBufStart);
				memcpy(m_lpBufCur, lpBuf, nMax);
				m_lpBufCur += nMax;
			}
		}

		void WriteCount(DWORD_PTR dwCount)
		{
			if (dwCount < 0xFFFF)
				*this << (WORD)dwCount;  // 16-bit count
			else
			{
				*this << (WORD)0xFFFF;
#ifndef _WIN64
				*this << (DWORD)dwCount;  // 32-bit count
#else  // _WIN64
				if (dwCount < 0xFFFFFFFF)
					*this << (DWORD)dwCount;  // 32-bit count
				else
				{
					*this << (DWORD)0xFFFFFFFF;
					*this << dwCount;
				}
#endif  // _WIN64
			}
		}

		void WriteClass(const CRuntimeClass* pClassRef)
		{
			ASSERT(pClassRef != NULL);
			ASSERT(IsStoring());    // proper direction

			if (pClassRef == NULL)
			{
				AfxThrowArchiveException(CArchiveException::badClass, m_strFileName);
			}

			if (!IsStoring())
			{
				AfxThrowArchiveException(CArchiveException::genericException, m_strFileName);
			}

			if (pClassRef->m_wSchema == 0xFFFF)
			{
				TRACE(traceAppMsg, 0, "Warning: Cannot call WriteClass/WriteObject for %hs.\n",
					pClassRef->m_lpszClassName);
				AfxThrowNotSupportedException();
			}

			// make sure m_pStoreMap is initialized
			MapObject(NULL);

			// write out class id of pOb, with high bit set to indicate
			// new object follows

			// ASSUME: initialized to 0 map
			DWORD nClassIndex;
			if ((nClassIndex = (DWORD)(DWORD_PTR)(*m_pStoreMap)[(void*)pClassRef]) != 0)
			{
				// previously seen class, write out the index tagged by high bit
				if (nClassIndex < wBigObjectTag)
					*this << (WORD)(wClassTag | nClassIndex);
				else
				{
					*this << wBigObjectTag;
					*this << (dwBigClassTag | nClassIndex);
				}
			}
			else
			{
				// store new class
				*this << wNewClassTag;
				pClassRef->Store(*this);

				// store new class reference in map, checking for overflow
				CheckCount();
				(*m_pStoreMap)[(void*)pClassRef] = (void*)(DWORD_PTR)m_nMapCount++;
			}
		}

		void WriteObject(const CObject* pOb)
		{
			// object can be NULL

			ASSERT(IsStoring());    // proper direction

			if (!IsStoring())
			{
				AfxThrowArchiveException(CArchiveException::readOnly, m_strFileName);
			}

			DWORD nObIndex;
			ASSERT(sizeof(nObIndex) == 4);
			ASSERT(sizeof(wNullTag) == 2);
			ASSERT(sizeof(wBigObjectTag) == 2);
			ASSERT(sizeof(wNewClassTag) == 2);

			// make sure m_pStoreMap is initialized
			MapObject(NULL);

			if (pOb == NULL)
			{
				// save out null tag to represent NULL pointer
				*this << wNullTag;
			}
			else if ((nObIndex = (DWORD)(DWORD_PTR)(*m_pStoreMap)[(void*)pOb]) != 0)
				// assumes initialized to 0 map
			{
				// save out index of already stored object
				if (nObIndex < wBigObjectTag)
					*this << (WORD)nObIndex;
				else
				{
					*this << wBigObjectTag;
					*this << nObIndex;
				}
			}
			else
			{
				// write class of object first
				CRuntimeClass* pClassRef = pOb->GetRuntimeClass();
				WriteClass(pClassRef);

				// enter in stored object table, checking for overflow
				CheckCount();
				(*m_pStoreMap)[(void*)pOb] = (void*)(DWORD_PTR)m_nMapCount++;

				// cause the object to serialize itself
				((CObject*)pOb)->Serialize(*this);
			}
		}








};



template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	//CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar.WriteCount(m_nSize);
	}
	else
	{
		DWORD_PTR nOldSize = ar.ReadCount();
		SetSize(nOldSize, -1);
	}
	SerializeElements<TYPE>(ar, m_pData, m_nSize);
}


template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	//CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar.WriteCount(m_nCount);
		for (CNode* pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
		{
			ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
			TYPE* pData;
			/*
			* in some cases the & operator might be overloaded, and we cannot use it to obtain
			* the address of a given object.  We then use the following trick to get the address
			*/
			pData = (TYPE*)(&(int&)((TYPE&)(pNode->data)));
			SerializeElements<TYPE>(ar, pData, 1);
		}
	}
	else
	{
		DWORD_PTR nNewCount = ar.ReadCount();
		while (nNewCount--)
		{
			TYPE newData[1];
			SerializeElements<TYPE>(ar, newData, 1);
			AddTail(newData[0]);
		}
	}
}






_INLINE CRuntimeClass* CRuntimeClass::FromName(LPCSTR lpszClassName)
{
	CRuntimeClass* pClass = NULL;

	ENSURE(lpszClassName);

	// search app specific classes
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();

	for (pClass = pModuleState->m_classList; pClass != NULL; pClass = pClass->m_pNextClass)
	{
		if (lstrcmpA(lpszClassName, pClass->m_lpszClassName) == 0)
		{
			return pClass;
		}
	}

	return NULL; // not found
}

_INLINE CRuntimeClass* CRuntimeClass::Load(CArchive& ar, UINT* pwSchemaNum)
// loads a runtime class description
{
	if (pwSchemaNum == NULL)
	{
		return NULL;
	}
	WORD nLen;
	char szClassName[64];

	WORD wTemp;
	ar >> wTemp; *pwSchemaNum = wTemp;
	ar >> nLen;

	// load the class name
	if (nLen >= _countof(szClassName) ||
		ar.Read(szClassName, nLen*sizeof(char)) != nLen*sizeof(char))
	{
		return NULL;
	}
	szClassName[nLen] = '\0';

	// match the string against an actual CRuntimeClass
	CRuntimeClass* pClass = FromName(szClassName);
	if (pClass == NULL)
	{
		// not found, trace a warning for diagnostic purposes
		TRACE(traceAppMsg, 0, "Warning: Cannot load %hs from archive.  Class not defined.\n", szClassName);
	}

	return pClass;
}

_INLINE void CRuntimeClass::Store(CArchive& ar) const
// stores a runtime class description
{
	WORD nLen = (WORD)lstrlenA(m_lpszClassName);
	ar << (WORD)m_wSchema << nLen;
	ar.Write(m_lpszClassName, nLen*sizeof(char));
}


