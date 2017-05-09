// SdiVC6View.cpp : CSdiVC6View クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "SdiVC6.h"

#include "SdiVC6Doc.h"
#include "SdiVC6View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6View

IMPLEMENT_DYNCREATE(CSdiVC6View, CView)

BEGIN_MESSAGE_MAP(CSdiVC6View, CView)
	//{{AFX_MSG_MAP(CSdiVC6View)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//    この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6View クラスの構築/消滅

CSdiVC6View::CSdiVC6View()
{
	// TODO: この場所に構築用のコードを追加してください。

}

CSdiVC6View::~CSdiVC6View()
{
}

BOOL CSdiVC6View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6View クラスの描画

void CSdiVC6View::OnDraw(CDC* pDC)
{
	CSdiVC6Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6View クラスの診断

#ifdef _DEBUG
void CSdiVC6View::AssertValid() const
{
	CView::AssertValid();
}

void CSdiVC6View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSdiVC6Doc* CSdiVC6View::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSdiVC6Doc)));
	return (CSdiVC6Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6View クラスのメッセージ ハンドラ
