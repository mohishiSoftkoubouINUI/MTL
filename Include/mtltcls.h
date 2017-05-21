


template<class TYPE>
_INLINE void ConstructElements(TYPE* pElements, INT_PTR nCount)
{
	// first do bit-wise zero initialization
	memset((void*)pElements, 0, nCount * sizeof(TYPE));

	// then call the constructor(s)
	for (; nCount--; pElements++)
		::new((void*)pElements) TYPE;
}

template<class TYPE>
_INLINE void DestructElements(TYPE* pElements, INT_PTR nCount)
{
	// call the destructor(s)
	for (; nCount--; pElements++)
		pElements->~TYPE();
}


template<class TYPE, class ARG_TYPE = const TYPE&>
class CAlloc
{
	public:
		// Construction
		CAlloc()
		{
			m_pData = NULL;
			m_nSize = 0;
		}

		~CAlloc()
		{
			Free();
		}

	public:
		// Attributes
		TYPE* m_pData;   // the actual array of data
		INT_PTR m_nSize;     // # of elements (upperBound - 1)

	public:
		// overloaded operator helpers
		operator TYPE*() const
		{
			return m_pData;
		}

		// Allocate the vector
		bool Allocate(size_t nElements)
		{
			// create buffer big enough to hold number of requested elements or
			// m_nGrowBy elements, whichever is larger.
			size_t nAllocSize = nElements;
			m_pData = (TYPE*) new BYTE[(size_t)nElements * sizeof(TYPE)];
			ConstructElements<TYPE>(m_pData, nElements);
			m_nSize = nElements;

			return TRUE ;
		}

		// Operations
		// Clean up
		void Free()
		{
			// shrink to nothing
			if (m_pData != NULL)
			{
				DestructElements<TYPE>(m_pData, m_nSize);
				delete[](BYTE*)m_pData;
				m_pData = NULL;
			}
			m_nSize = 0;
		}

};

/*============================================================================*/
// CArray<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE = const TYPE&>
class CArray
{
	public:
		// Construction
		CArray()
		{
			m_pData = NULL;
			m_nSize = 0;
			m_nMaxSize = 0 ;
		}

		~CArray()
		{
			RemoveAll() ;
		}

	public:
		// Attributes
		TYPE* m_pData;   // the actual array of data
		INT_PTR m_nSize;     // # of elements (upperBound - 1)
		INT_PTR m_nMaxSize;  // max allocated

	public:
		// overloaded operator helpers
		const TYPE& operator[](INT_PTR nIndex) const
		{ 
			return m_pData[nIndex];
		}
		 
		TYPE& operator[](INT_PTR nIndex) 
		{
			return m_pData[nIndex];
		}

		INT_PTR Add(ARG_TYPE newElement)
		{
			INT_PTR nIndex = m_nSize;
			SetAtGrow(nIndex, newElement);
			return nIndex;
		}

		// Accessing elements
		const TYPE* GetData() const { return (const TYPE*)m_pData; }

		// Implementation
		INT_PTR GetCount() const { return m_nSize; }
		INT_PTR GetSize() const { return m_nSize; }
		INT_PTR GetUpperBound() const { return m_nSize - 1; }

		TYPE& GetAt(INT_PTR nIndex) const
		{
			return m_pData[nIndex];
		}

		void InsertAt(INT_PTR nIndex, ARG_TYPE newElement, INT_PTR nCount = 1 /*=1*/)
		{
			ASSERT_VALID(this);
			ASSERT(nIndex >= 0);    // will expand to meet need
			ASSERT(nCount > 0);     // zero or negative size not allowed


			if (nIndex >= m_nSize)
			{
				// adding after the end of the array
				SetSize(nIndex + nCount, -1);   // grow so nIndex is valid
			}
			else
			{
				// inserting in the middle of the array
				INT_PTR nOldSize = m_nSize;
				SetSize(m_nSize + nCount, -1);  // grow it to new size
				// destroy intial data before copying over it
				DestructElements<TYPE>(&m_pData[nOldSize], nCount);
				
				// shift old data up to fill gap
				memmove(m_pData + nIndex + nCount, m_pData + nIndex, (nOldSize - nIndex) * sizeof(TYPE));

				// re-init slots we copied from
				ConstructElements<TYPE>(&m_pData[nIndex], nCount);
			}

			// insert new value in the gap
			ASSERT(nIndex + nCount <= m_nSize);
			while (nCount--)
				m_pData[nIndex++] = newElement;
		}

		BOOL IsEmpty() const { return m_nSize == 0; }

		// Operations
		// Clean up
		void RemoveAll()
		{
			// shrink to nothing
			if (m_pData != NULL)
			{
				DestructElements<TYPE>(m_pData, m_nSize);
				delete[](BYTE*)m_pData;
				m_pData = NULL;
			}
			m_nSize = 0;
			m_nMaxSize = 0;
		}

		void RemoveAt(INT_PTR nIndex, INT_PTR nCount = 1)
		{
			ASSERT_VALID(this);
			ASSERT(nIndex >= 0);
			ASSERT(nCount >= 0);
			INT_PTR nUpperBound = nIndex + nCount;
			ASSERT(nUpperBound <= m_nSize && nUpperBound >= nIndex && nUpperBound >= nCount);

			// just remove a range
			INT_PTR nMoveCount = m_nSize - (nUpperBound);
			DestructElements<TYPE>(m_pData + nIndex, nCount);
			if (nMoveCount)
			{
				memmove(m_pData + nIndex, m_pData + nUpperBound, (size_t)nMoveCount * sizeof(TYPE));
			}
			m_nSize -= nCount;
		}

		void SetAtGrow(INT_PTR nIndex, ARG_TYPE newElement)
		{
			ASSERT_VALID(this);
			ASSERT(nIndex >= 0);

			if (nIndex >= m_nSize)
				SetSize(nIndex + 1, -1);
			m_pData[nIndex] = newElement;
		}

		void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1)
		{
			ASSERT_VALID(this);
			ASSERT(nNewSize >= 0);

			if (nNewSize == 0)
			{
				RemoveAll() ;
			}
			else if (m_pData == NULL)
			{
				// create buffer big enough to hold number of requested elements or
				// m_nGrowBy elements, whichever is larger.
				size_t nAllocSize = nNewSize ;
				m_pData = (TYPE*) new BYTE[(size_t)nAllocSize * sizeof(TYPE)];
				ConstructElements<TYPE>(m_pData, nNewSize);
				m_nSize = nNewSize;
				m_nMaxSize = nAllocSize;
			}
			else if (nNewSize <= m_nMaxSize)
			{
				// it fits
				if (nNewSize > m_nSize)
				{
					// initialize the new elements
					ConstructElements<TYPE>(&m_pData[m_nSize], nNewSize-m_nSize);
				}
				else if (m_nSize > nNewSize)
				{
					// destroy the old elements
					DestructElements<TYPE>(&m_pData[nNewSize], m_nSize-nNewSize);
				}
				m_nSize = nNewSize;
			}
			else
			{
				// otherwise, grow array
				// heuristically determine growth when nGrowBy == 0
				//  (this avoids heap fragmentation in many situations)
				nGrowBy = m_nSize / 8;
				nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);

				INT_PTR nNewMax;
				if (nNewSize < m_nMaxSize + nGrowBy)
					nNewMax = m_nMaxSize + nGrowBy;  // granularity
				else
					nNewMax = nNewSize;  // no slush

				TYPE* pNewData = (TYPE*) new BYTE[(size_t)nNewMax * sizeof(TYPE)];

				// copy new data from old
				memcpy(pNewData, m_pData, (size_t)m_nSize * sizeof(TYPE));

				// construct remaining elements
				ASSERT(nNewSize > m_nSize);
				ConstructElements<TYPE>(&pNewData[m_nSize], nNewSize-m_nSize);

				// get rid of old stuff (note: no destructors called)
				delete[](BYTE*)m_pData;
				m_pData = pNewData;
				m_nSize = nNewSize;
				m_nMaxSize = nNewMax;
			}
		}

		void SetAt(INT_PTR nIndex, ARG_TYPE newElement)
		{
			m_pData[nIndex] = newElement;
		}

		void Serialize(CArchive& ar) ;

};



template<class TYPE, class ARG_TYPE = const TYPE&>
class CList
{
	public:
		CList()
		{
			m_nCount = 0;
			m_pNodeHead = m_pNodeTail = NULL;
		}

		~CList()
		{
			CNode* pCNode = m_pNodeHead;

			while (pCNode != NULL)
			{
				CNode* pCNodeNext = pCNode->pNext;

				delete pCNode;

				pCNode = pCNodeNext;
			}
		}

	public:
		struct CNode
		{
			CNode* pPrev;
			CNode* pNext;
			TYPE data;

			CNode(CNode* pNewPrev = NULL, CNode* pNewNext = NULL) { pPrev = pNewPrev; pNext = pNewNext; data = 0 ;}

		};

		// Implementation
	public:
		CNode* m_pNodeHead;
		CNode* m_pNodeTail;

		INT_PTR m_nCount;

	public:
		operator TYPE() { return (TYPE)GetHead(); }

		CNode* NewNode(CNode* pPrev, CNode* pNext)
		{
			CNode* pNode = new CNode(pPrev, pNext);
			m_nCount++;
			ASSERT(m_nCount > 0);  // make sure we don't overflow

			return pNode;
		}

		void FreeNode(CNode* pNode)
		{
			delete pNode;

			m_nCount--;
			ASSERT(m_nCount >= 0);  // make sure we don't underflow
		}


		POSITION AddHead(ARG_TYPE newElement)
		{
			CNode* pNewNode = NewNode(NULL, m_pNodeHead);
			pNewNode->data = newElement;
			if (m_pNodeHead != NULL)
				m_pNodeHead->pPrev = pNewNode;

			else
				m_pNodeTail = pNewNode;
			m_pNodeHead = pNewNode;
			return (POSITION)pNewNode;
		}

		POSITION AddTail(ARG_TYPE newElement)
		{
			CNode* pNewNode = NewNode(m_pNodeTail, NULL);
			pNewNode->data = newElement;
			if (m_pNodeTail != NULL)
				m_pNodeTail->pNext = pNewNode;
			else
				m_pNodeHead = pNewNode;
			m_pNodeTail = pNewNode;
			return (POSITION)pNewNode;
		}

		POSITION Find(ARG_TYPE searchValue, POSITION startAfter = NULL) const
		{
			ASSERT_VALID(this);

			CNode* pNode = (CNode*)startAfter;
			if (pNode == NULL)
			{
				pNode = m_pNodeHead;  // start at head
			}
			else
			{
				pNode = pNode->pNext;  // start after the one specified
			}

			for (; pNode != NULL; pNode = pNode->pNext)
				if (pNode->data == searchValue)
					return (POSITION)pNode;
			return NULL;
		}

		POSITION GetHeadPosition() const { return (POSITION)m_pNodeHead; }
		POSITION GetTailPosition() const { return (POSITION)m_pNodeTail; }

		// peek at head or tail
		const TYPE& GetHead() const { return m_pNodeHead->data; }
		const TYPE& GetTail() const { return m_pNodeTail->data; }

		const TYPE& GetNext(POSITION& rPosition) const
		{
			CNode* pNode = (CNode*)rPosition;
			ASSERT(pNode != NULL) ;
			rPosition = (POSITION)pNode->pNext;
			return pNode->data;
		}

		const TYPE& GetPrev(POSITION& rPosition) const
		{
			CNode* pNode = (CNode*)rPosition;
			ASSERT(pNode != NULL) ;
			rPosition = (POSITION)pNode->pPrev;
			return pNode->data;
		}

		INT_PTR GetCount() const { return m_nCount; }
		BOOL IsEmpty() const { return m_nCount == 0; }

		void Remove(ARG_TYPE deletehValue)
		{
			POSITION pos = Find(deletehValue);
			if (pos != NULL)
				RemoveAt(pos) ;
		}

		void RemoveAt(POSITION position)
		{
			ASSERT_VALID(this);

			CNode* pOldNode = (CNode*)position;

			// remove pOldNode from list
			if (pOldNode == m_pNodeHead)
			{
				m_pNodeHead = pOldNode->pNext;
			}
			else
			{
				pOldNode->pPrev->pNext = pOldNode->pNext;
			}
			if (pOldNode == m_pNodeTail)
			{
				m_pNodeTail = pOldNode->pPrev;
			}
			else
			{
				pOldNode->pNext->pPrev = pOldNode->pPrev;
			}
			FreeNode(pOldNode);
		}

		TYPE RemoveHead()
		{
			ASSERT_VALID(this);
			ENSURE(m_pNodeHead != NULL);  // don't call on empty list !!!

			CNode* pOldNode = m_pNodeHead;
			TYPE returnValue = pOldNode->data;

			m_pNodeHead = pOldNode->pNext;
			if (m_pNodeHead != NULL)
				m_pNodeHead->pPrev = NULL;
			else
				m_pNodeTail = NULL;
			FreeNode(pOldNode);
			return returnValue;
		}

		TYPE RemoveTail()
		{
			ASSERT_VALID(this);
			ENSURE(m_pNodeTail != NULL);  // don't call on empty list !!!

			CNode* pOldNode = m_pNodeTail;
			TYPE returnValue = pOldNode->data;

			m_pNodeTail = pOldNode->pPrev;
			if (m_pNodeTail != NULL)
				m_pNodeTail->pNext = NULL;
			else
				m_pNodeHead = NULL;
			FreeNode(pOldNode);
			return returnValue;
		}

		void Serialize(CArchive& ar) ;


};


// CPair
template<class TKEY, class TYPE>
class CPair
{
	public:
		CPair() { key = NULL; value = NULL; bDelete = FALSE; }
		CPair(TKEY keyval, BOOL bdel = TRUE) { key = keyval; value = NULL; bDelete = bdel; }

	public:
		TKEY key;
		TYPE value;
		BOOL bDelete;

};

template<class TKEY, class TYPE, class TPAIR = CPair<TKEY, TYPE>, BOOL bDelete = FALSE>
class CMapList : public CList<TPAIR>
{
	typedef TPAIR CTypePair;

	public:
		CMapList() : CList<CTypePair>()
		{
		}

	public:
		typedef CList<CTypePair>::CNode CMapNode;

		void GetNextAssoc(POSITION& pos, TKEY key, TYPE value) const
		{
			key = ((CMapNode*)pos)->data.key ;
			value = ((CMapNode*)pos)->data.value;

			GetNext(pos);
		}

		POSITION GetStartPosition() const { return GetHeadPosition(); }

		POSITION Find(const TKEY key) const
		{
			POSITION pos = GetHeadPosition();

			while (pos != NULL)
			{
				if (((CMapNode*)pos)->data.key == key)
					return pos;

				GetNext(pos);
			}

			return NULL;
		}

		CTypePair& FindData(const TKEY key, const BOOL bDelete = FALSE)
		{
			POSITION pos = Find(key);
			if (pos != NULL)
				return ((CMapNode*)pos)->data;

			return ((CMapNode*)AddTail(CTypePair(key, bDelete)))->data;
		}

		const TYPE& operator [] (const TKEY key) const // “Ç‚ÝŽæ‚è—p
		{
			return FindData(key).value;
		}

		TYPE& operator [] (const TKEY key) // ‘‚«ž‚Ý—p
		{
			return FindData(key).value;
		}

		TYPE& GetAt(TKEY key) const
		{
			return (*this)[key] ;
		}

		TYPE GetValueAt(TKEY key)
		{
			POSITION pos = Find(key);
			if (pos != NULL)
				return ((CMapNode*)pos)->data.value ;

			return NULL ;
		}

		BOOL Lookup(const TKEY key, TYPE& rValue) const
		{
			POSITION pos = Find(key);
			if (pos == NULL)
				return FALSE ;

			rValue = ((CMapNode*)pos)->data.value ;
			return TRUE ;
		}

		BOOL RemoveKey(const TKEY key, const BOOL bDelete = FALSE)
		{
			POSITION pos = Find(key);
			if (pos != NULL)
			{
				if (bDelete || ((CMapNode*)pos)->data.bDelete)
					delete ((CMapNode*)pos)->data.value;

				RemoveAt(pos);
				return TRUE;
			}

			return FALSE;
		}

		void RemoveAll()
		{
			while (m_nCount > 0)
				RemoveTail() ;
		}

		void SetAt(const TKEY key, TYPE value)
		{
			(*this)[key] = value;
		}


};


#define HANDLE_ADDRESS_INVALID ((void*)-1)
//#define HANDLE_REMOVE_LIMIT (8)

typedef void* (*HANDLE_FUNC_NEW)(void*, HANDLE) ;

#if 0
// CHandlePair
template<class TKEY, class TYPE, class TOWNER>
class CHandlePair : public CPair<TKEY, TYPE>
{
	public:
		CHandlePair(TKEY keyval = NULL, BOOL bdel = FALSE) : CPair<TKEY, TYPE>(keyval, bdel) 
		{ 
			owner = NULL ; 
			pFunc = NULL ; 
		}

	public:
		TOWNER owner ;
		HANDLE_FUNC_NEW pFunc;

};


template<class H, class T, class O = HWND>
class CHandleList : public CMapList<H, T*, CHandlePair<H, T*, O> > // Blank required
{
	typedef CHandlePair<H, T*, O> CTypeHandlePair;

	public:

	public:
		typedef CMapList<H, T*, CTypeHandlePair>::CNode CHandleNode;

		T* FromHandle(const H h, T* t = NULL, const O o = NULL, const HANDLE_FUNC_NEW p = NULL)
		{
			POSITION pos = Find(h);
			if (pos != NULL)
				return ((CHandleNode*)pos)->data.value;

			T* pT = ((t != NULL) || (p == NULL)) ? t : (T*)p(h, NULL);

			if (pT != NULL)
			{
				CTypeHandlePair& rP = ((CHandleNode*)AddTail(CTypeHandlePair(h, FALSE)))->data;
				rP.value = pT ;
				rP.owner = o;
				rP.pFunc = (t != NULL) ? NULL : p ;
			}

			return pT;
		}

		H RemoveAt(POSITION position)
		{
			H key = ((CHandleNode*)position)->data.key;
			T* value = NULL ;
			HANDLE_FUNC_NEW pFunc = ((CHandleNode*)position)->data.pFunc ;
			if (pFunc != NULL)
			{	
				value = ((CHandleNode*)position)->data.value ;
			}

			CMapList<H, T*, CTypeHandlePair>::RemoveAt(position);

			if (pFunc != NULL)
				pFunc(key, value);

			return key ;
		}

		H RemoveHandle(const H h)
		{
			POSITION pos = Find(h);
			if (pos == NULL)
				return NULL;

			return RemoveAt(pos);
		}

		BOOL RemoveOwner(const O o)
		{
			POSITION pos = GetHeadPosition();

			while (pos != NULL)
			{
				//if (((void*)o == HANDLE_ADDRESS_INVALID) && (GetCount() <= HANDLE_REMOVE_LIMIT))
				//	return FALSE ;

				POSITION posFree = pos ;

				O owner = GetNext(pos).owner;

				//if ((owner == o) || (owner == HANDLE_ADDRESS_INVALID))
				if (owner == o)
				{
					RemoveAt(posFree) ;
				}
			}

			return TRUE ;
		}

};
#endif


#define HANDLE_HASH_SIZE (17)

// CHandlePair
template<class TKEY, class TYPE, class TOWNER>
class CHandlePair : public CPair<TKEY, TYPE>
{
	public:
		CHandlePair(TKEY keyval = NULL, BOOL bdel = FALSE) : CPair<TKEY, TYPE>(keyval, bdel)
		{
			owner = NULL;
			pFunc = NULL;
			pNext = NULL;
		}

	public:
		TOWNER owner;
		HANDLE_FUNC_NEW pFunc;
		CHandlePair* pNext;

};

template<class H, class T, class O = HWND>
class CHandleList
{
	typedef CHandlePair<H, T*, O> CTypeHandlePair;

	public:
		CHandleList()
		{
			m_pCTypeHandlePair = NULL;

			m_nCount = 0;
		}

	public:
		CTypeHandlePair* m_pCTypeHandlePair;

		INT_PTR m_nCount;

	public:
		INT_PTR GetCount() const { return m_nCount; }
		CTypeHandlePair* NewNode(H h) { m_nCount++; return new CTypeHandlePair(h, FALSE); }
		void FreeNode(CTypeHandlePair* pCTypeHandlePair) { m_nCount--; delete pCTypeHandlePair; }


		T* FromHandle(const H h, T* t = NULL, const O o = NULL, const HANDLE_FUNC_NEW p = NULL)
		{
			CTypeHandlePair* pCTypeHandlePair = m_pCTypeHandlePair;

			if (pCTypeHandlePair != NULL)
			{
				do
				{
					if (pCTypeHandlePair->key == h)
						return pCTypeHandlePair->value;

				} while ((pCTypeHandlePair = pCTypeHandlePair->pNext) != NULL);
			}

			T* pT = ((t != NULL) || (p == NULL)) ? t : (T*)p(h, NULL);

			if (pT != NULL)
			{
				CTypeHandlePair* pP = NewNode(h);
				pP->value = pT;
				pP->owner = o;
				pP->pFunc = (t != NULL) ? NULL : p;
				pP->pNext = m_pCTypeHandlePair;

				m_pCTypeHandlePair = pP;
			}

			return pT;
		}

		H RemoveHandle(const H h)
		{
			CTypeHandlePair* pCTypeHandlePair = m_pCTypeHandlePair;

			if (pCTypeHandlePair != NULL)
			{
				CTypeHandlePair** ppNext = &m_pCTypeHandlePair;

				do
				{
					if (pCTypeHandlePair->key == h)
					{
						*ppNext = pCTypeHandlePair->pNext;

						if (pCTypeHandlePair->pFunc != NULL)
							pCTypeHandlePair->pFunc(h, pCTypeHandlePair->value);

						FreeNode(pCTypeHandlePair);

						return h;
					}

					ppNext = &pCTypeHandlePair->pNext;

				} while ((pCTypeHandlePair = pCTypeHandlePair->pNext) != NULL);
			}

			return NULL;
		}

		BOOL RemoveOwner(const O o)
		{
			CTypeHandlePair* pCTypeHandlePair = m_pCTypeHandlePair;

			if (pCTypeHandlePair != NULL)
			{
				CTypeHandlePair** ppNext = &m_pCTypeHandlePair;

				CTypeHandlePair* pNext;

				do
				{
					pNext = pCTypeHandlePair->pNext;

					if (pCTypeHandlePair->owner == o)
					{
						*ppNext = pNext;

						if (pCTypeHandlePair->pFunc != NULL)
							pCTypeHandlePair->pFunc(pCTypeHandlePair->key, pCTypeHandlePair->value);

						FreeNode(pCTypeHandlePair);
					}
					else
					{
						ppNext = &pCTypeHandlePair->pNext;
					}

				} while ((pCTypeHandlePair = pNext) != NULL);
			}

			return TRUE;
		}

};


template<class H, class T, class O = HWND, INT S = HANDLE_HASH_SIZE>
class CHandleHashList
{
	typedef CHandlePair<H, T*, O> CTypeHandleHashPair;

	public:
		CHandleHashList()
		{
			m_paCTypeHandleHashPair = new CTypeHandleHashPair*[S * 2];
			memset(m_paCTypeHandleHashPair, 0, sizeof(CTypeHandleHashPair*) * S * 2);

			m_nCount = 0;
		}

		~CHandleHashList()
		{
			delete[] m_paCTypeHandleHashPair;
		}

	public:
		CTypeHandleHashPair** m_paCTypeHandleHashPair;

		INT_PTR m_nCount;

	public:
		INT_PTR GetCount() const { return m_nCount ; }
		const UINT Hash(H h) const { return (UINT)h % S; }
		CTypeHandleHashPair* NewNode(H h) { m_nCount++ ; return new CTypeHandleHashPair(h, FALSE); }
		void FreeNode(CTypeHandleHashPair* pCTypeHandleHashPair) { m_nCount-- ; delete pCTypeHandleHashPair; }


		T* FromHandle(const H h, T* t = NULL, const O o = NULL, const HANDLE_FUNC_NEW p = NULL)
		{
			const UINT iHash = Hash(h);

			CTypeHandleHashPair** ppCTypeHandleHashPair = m_paCTypeHandleHashPair;

			do
			{
				CTypeHandleHashPair* pCTypeHandleHashPair = ppCTypeHandleHashPair[iHash];

				if (pCTypeHandleHashPair != NULL)
				{
					do
					{
						if (pCTypeHandleHashPair->key == h)
							return pCTypeHandleHashPair->value;

					} while ((pCTypeHandleHashPair = pCTypeHandleHashPair->pNext) != NULL);
				}

			} while ((ppCTypeHandleHashPair += S) < (m_paCTypeHandleHashPair + S * 2));

			T* pT = ((t != NULL) || (p == NULL)) ? t : (T*)p(h, NULL);

			if (pT != NULL)
			{
				ppCTypeHandleHashPair = m_paCTypeHandleHashPair;
				if (o == HANDLE_ADDRESS_INVALID)
					ppCTypeHandleHashPair += S;

				CTypeHandleHashPair* pP = NewNode(h);
				pP->value = pT;
				pP->owner = o;
				pP->pFunc = (t != NULL) ? NULL : p;
				pP->pNext = ppCTypeHandleHashPair[iHash];

				ppCTypeHandleHashPair[iHash] = pP;
			}

			return pT;
		}

		H RemoveHandle(const H h)
		{
			const UINT iHash = Hash(h);

			CTypeHandleHashPair** ppCTypeHandleHashPair = m_paCTypeHandleHashPair;

			do
			{
				CTypeHandleHashPair* pCTypeHandleHashPair = ppCTypeHandleHashPair[iHash];

				if (pCTypeHandleHashPair != NULL)
				{
					CTypeHandleHashPair** ppNext = &ppCTypeHandleHashPair[iHash];

					do
					{
						if (pCTypeHandleHashPair->key == h)
						{
							*ppNext = pCTypeHandleHashPair->pNext;

							if (pCTypeHandleHashPair->pFunc != NULL)
								pCTypeHandleHashPair->pFunc(h, pCTypeHandleHashPair->value);

							FreeNode(pCTypeHandleHashPair);

							return h ;
						}

						ppNext = &pCTypeHandleHashPair->pNext;

					} while ((pCTypeHandleHashPair = pCTypeHandleHashPair->pNext) != NULL);
				}

			} while ((ppCTypeHandleHashPair += S) < (m_paCTypeHandleHashPair + S * 2));

			return NULL;
		}

		BOOL RemoveOwner(const O o)
		{
			CTypeHandleHashPair** ppCTypeHandleHashPair = m_paCTypeHandleHashPair;
			if (o == HANDLE_ADDRESS_INVALID)
				ppCTypeHandleHashPair += S;

			CTypeHandleHashPair** ppCTypeHandleHashPairEnd = ppCTypeHandleHashPair + S;

			do
			{
				CTypeHandleHashPair* pCTypeHandleHashPair = *ppCTypeHandleHashPair;

				if (pCTypeHandleHashPair != NULL)
				{
					CTypeHandleHashPair** ppNext = ppCTypeHandleHashPair;

					CTypeHandleHashPair* pNext ;

					do
					{
						pNext = pCTypeHandleHashPair->pNext ;

						if (pCTypeHandleHashPair->owner == o)
						{
							*ppNext = pNext;

							if (pCTypeHandleHashPair->pFunc != NULL)
								pCTypeHandleHashPair->pFunc(pCTypeHandleHashPair->key, pCTypeHandleHashPair->value);

							FreeNode(pCTypeHandleHashPair);
						}
						else
						{
							ppNext = &pCTypeHandleHashPair->pNext;
						}

					} while ((pCTypeHandleHashPair = pNext) != NULL);
				}

			} while (++ppCTypeHandleHashPair < ppCTypeHandleHashPairEnd);

			return TRUE;
		}

};


#if 1	// CMap

/*============================================================================*/
// CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>

template<class TYPE, class ARG_TYPE>
_INLINE BOOL CompareElements(const TYPE* pElement1, const ARG_TYPE* pElement2)
{
	ENSURE(pElement1 != NULL && pElement2 != NULL);

	return *pElement1 == *pElement2;
}
template<class ARG_KEY>
_INLINE UINT HashKey(ARG_KEY key)
{
	// (algorithm copied from STL hash in xfunctional)
#pragma warning(suppress: 4302) // 'type cast' : truncation
	ldiv_t HashVal = ldiv((long)(ARG_KEY)key, 127773);
	HashVal.rem = 16807 * HashVal.rem - 2836 * HashVal.quot;
	if (HashVal.rem < 0)
		HashVal.rem += 2147483647;
	return ((UINT)HashVal.rem);
}


struct CPlex     // warning variable length structure
{
	CPlex* pNext;
#ifndef _WIN64
#if (_AFX_PACKING >= 8)
	DWORD dwReserved[1];    // align on 8 byte boundary
#endif
#endif
	// BYTE data[maxNum*elementSize];

	void* data() { return this + 1; }

	static CPlex* PASCAL Create(CPlex*& head, UINT_PTR nMax, UINT_PTR cbElement);
	// like 'calloc' but no zero fill
	// may throw memory exceptions

	void FreeDataChain()       // free this one and links
	{
		CPlex* pPlex;

		pPlex = this;
		while (pPlex != NULL)
		{
			CPlex* pNext;

			pNext = pPlex->pNext;
			free(pPlex);
			pPlex = pNext;
		}
	}
};


template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class CMap
{
	public:
		// Construction
		CMap(INT_PTR nBlockSize = 10)
		{
			ASSERT(nBlockSize > 0);

			m_pHashTable = NULL;
			m_nHashTableSize = 17;  // default size
			m_nCount = 0;
			m_pFreeList = NULL;
			m_pBlocks = NULL;
			m_nBlockSize = nBlockSize;
		}

		~CMap()
		{
			RemoveAll();
			ASSERT(m_nCount == 0);
		}

		// CPair
		struct CPair
		{
			const KEY key;
			VALUE value;
		protected:
			CPair(ARG_KEY keyval) : key(keyval)	{}
		};

	public:
		// Association
		class CAssoc : public CPair
		{
			friend class CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>;
			CAssoc* pNext;
			UINT nHashValue;  // needed for efficient iteration
		public:
			CAssoc(ARG_KEY key) : CPair(key) {}
		};

	public:
		// Attributes

		CAssoc** m_pHashTable;
		UINT m_nHashTableSize;
		INT_PTR m_nCount;
		CAssoc* m_pFreeList;
		struct CPlex* m_pBlocks;
		INT_PTR m_nBlockSize;

	public:

		// number of elements
		INT_PTR GetCount() const;
		BOOL IsEmpty() const;

		// Lookup
		CAssoc* GetAssocAt(ARG_KEY key, UINT& nHashBucket, UINT& nHashValue) const // find association (or return NULL)
		{
			nHashValue = HashKey<ARG_KEY>(key);
			nHashBucket = nHashValue % m_nHashTableSize;

			if (m_pHashTable == NULL)
				return NULL;

			// see if it exists
			CAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHashBucket]; pAssoc != NULL; pAssoc = pAssoc->pNext)
			{
				if (pAssoc->nHashValue == nHashValue && CompareElements(&pAssoc->key, &key))
					return pAssoc;
			}
			return NULL;
		}

		void InitHashTable(UINT nHashSize, BOOL bAllocNow = TRUE)
		{
			//
			// Used to force allocation of a hash table or to override the default
			//   hash table size of (which is fairly small)
			ASSERT_VALID(this);
			ASSERT(m_nCount == 0);
			ASSERT(nHashSize > 0);

			if (m_pHashTable != NULL)
			{
				// free hash table
				delete[] m_pHashTable;
				m_pHashTable = NULL;
			}

			if (bAllocNow)
			{
				m_pHashTable = new CAssoc*[nHashSize];
				ENSURE(m_pHashTable != NULL);
				memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
			}
			m_nHashTableSize = nHashSize;
		}

		BOOL Lookup(ARG_KEY key, VALUE& rValue) const
		{
			ASSERT_VALID(this);

			UINT nHashBucket, nHashValue;
			CAssoc* pAssoc = GetAssocAt(key, nHashBucket, nHashValue);
			if (pAssoc == NULL)
				return FALSE;  // not in map

			rValue = pAssoc->value;
			return TRUE;
		}

		CAssoc* NewAssoc(ARG_KEY key)
		{
			CAssoc* pAssoc = new CAssoc(key);

			return pAssoc;
		}

		VALUE& operator[](ARG_KEY key)
		{
			ASSERT_VALID(this);

			UINT nHashBucket, nHashValue;
			CAssoc* pAssoc;
			if ((pAssoc = GetAssocAt(key, nHashBucket, nHashValue)) == NULL)
			{
				if (m_pHashTable == NULL)
					InitHashTable(m_nHashTableSize);

				ENSURE(m_pHashTable);
				// it doesn't exist, add a new Association
				pAssoc = NewAssoc(key);
				pAssoc->nHashValue = nHashValue;
				//'pAssoc->value' is a constructed object, nothing more

				// put into hash table
				pAssoc->pNext = m_pHashTable[nHashBucket];
				m_pHashTable[nHashBucket] = pAssoc;
			}
			return pAssoc->value;  // return new reference
		}

		// add a new (key, value) pair
		void SetAt(ARG_KEY key, ARG_VALUE newValue) { (*this)[key] = newValue; }

		// advanced features for derived classes
		UINT GetHashTableSize() const;


		void RemoveAll()
		{
			ASSERT_VALID(this);

			if (m_pHashTable != NULL)
			{
				// destroy elements (values and keys)
				for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
				{
					CAssoc* pAssoc;
					for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
					{
						pAssoc->CAssoc::~CAssoc();
					}
				}

				// free hash table
				delete[] m_pHashTable;
				m_pHashTable = NULL;
			}

			m_nCount = 0;
			m_pFreeList = NULL;
			m_pBlocks->FreeDataChain();
			m_pBlocks = NULL;
		}


};

#endif	// CMap



template<class BASE_CLASS, class TYPE>
class CTypedPtrList : public BASE_CLASS
{
	public:
		// peek at head or tail
		TYPE& GetHead() { return (TYPE&)BASE_CLASS::GetHead(); }
		TYPE& GetTail() { return (TYPE&)BASE_CLASS::GetTail(); }

		TYPE& GetNext(POSITION& rPosition) { return (TYPE&)BASE_CLASS::GetNext(rPosition); }
		TYPE& GetPrev(POSITION& rPosition) { return (TYPE&)BASE_CLASS::GetPrev(rPosition); }

		TYPE RemoveHead() { return (TYPE)BASE_CLASS::RemoveHead(); }
		TYPE RemoveTail() { return (TYPE)BASE_CLASS::RemoveTail(); }

#if 1
		void Serialize(CArchive& ar)
		{
			ASSERT_VALID(this);

			if (ar.IsStoring())
			{
				ar.WriteCount(m_nCount);
				for (CNode* pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
				{
					ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
					ar << pNode->data;
				}
			}
			else
			{
				DWORD_PTR nNewCount = ar.ReadCount();
				CObject* newData;
				while (nNewCount--)
				{
					ar >> newData;
					AddTail(newData);
				}
			}
		}
#endif
};



typedef CArray<BYTE> CByteArray;
typedef CArray<CString> CStringArray;
typedef CArray<void*> CPtrArray;

typedef CList<void*, void*> CPtrList;
typedef CList<CObject*> CObList;

typedef CMapList<LPCTSTR, void*> CMapStringToPtr;
typedef CMapList<void*, void*> CMapPtrToPtr ;



template<class TYPE>
void SerializeElements(CArchive& ar, TYPE* pElements, INT_PTR nCount)
{
	ENSURE(nCount == 0 || pElements != NULL);
	ASSERT(nCount == 0 || AfxIsValidAddress(pElements, (size_t)nCount * sizeof(TYPE)));

	// default is bit-wise read/write
	if (ar.IsStoring())
	{
		UINT_PTR nElementsLeft = nCount;
		TYPE* pData = pElements;
		while (nElementsLeft > 0)
		{
			UINT nElementsToWrite = UINT(__min(nElementsLeft, INT_MAX / sizeof(TYPE)));
			ar.Write(pData, nElementsToWrite * sizeof(TYPE));
			nElementsLeft -= nElementsToWrite;
			pData += nElementsToWrite;
		}
	}
	else
	{
		UINT_PTR nElementsLeft = nCount;
		TYPE* pData = pElements;
		while (nElementsLeft > 0)
		{
			UINT nElementsToRead = UINT(__min(nElementsLeft, INT_MAX / sizeof(TYPE)));
			ar.EnsureRead(pData, nElementsToRead * sizeof(TYPE));
			nElementsLeft -= nElementsToRead;
			pData += nElementsToRead;
		}
	}
}





class CModuleState
{
	public:
		CModuleState()
		{
			m_fRegisteredClasses = 0;

			m_nObjectCount = 0 ;

			m_afxContextIsDLL = FALSE;

			m_pfnFilterToolTipMessage = NULL;

			m_thread = NULL;
		}

	public:
		DWORD  m_fRegisteredClasses;

		CList<CRuntimeClass*> m_classList;

		long m_nObjectCount;

		BOOL m_afxContextIsDLL;

		CString m_strUnregisterList;

		// variables related to a given process in a module
		//  (used to be AFX_MODULE_PROCESS_STATE)
		void(*m_pfnFilterToolTipMessage)(MSG*, CWnd*);

		AFX_MODULE_THREAD_STATE* m_thread;

};

class CThreadState
{
	public:
		CThreadState()
		{
			m_hLockoutNotifyWindow = NULL;

			m_dwPropStyle = 0;
			m_dwPropExStyle = 0;

			m_hTrackingWindow = NULL;         // see CWnd::TrackPopupMenu
			m_hTrackingMenu = NULL;
		}

	public:
		HWND m_hLockoutNotifyWindow;    // see CWnd::OnCommand

		DWORD m_dwPropStyle;
		DWORD m_dwPropExStyle;

		// other CWnd modal data
		HWND m_hTrackingWindow;         // see CWnd::TrackPopupMenu
		HMENU m_hTrackingMenu;

		TCHAR m_szTempClassName[_AFX_TEMP_CLASS_NAME_SIZE];    // see AfxRegisterWndClass

};






