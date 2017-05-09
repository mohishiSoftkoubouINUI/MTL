// MdiVC6View.cpp : CMdiVC6View クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "MdiVC6.h"

#include "MdiVC6Doc.h"
#include "MdiVC6View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6View

IMPLEMENT_DYNCREATE(CMdiVC6View, CView)

BEGIN_MESSAGE_MAP(CMdiVC6View, CView)
	//{{AFX_MSG_MAP(CMdiVC6View)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//    この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6View クラスの構築/消滅

CMdiVC6View::CMdiVC6View()
{
	// TODO: この場所に構築用のコードを追加してください。

}

CMdiVC6View::~CMdiVC6View()
{
}

BOOL CMdiVC6View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6View クラスの描画

void CMdiVC6View::OnDraw(CDC* pDC)
{
	CMdiVC6Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6View クラスの印刷

BOOL CMdiVC6View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの印刷準備
	return DoPreparePrinting(pInfo);
}

void CMdiVC6View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CMdiVC6View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6View クラスの診断

#ifdef _DEBUG
void CMdiVC6View::AssertValid() const
{
	CView::AssertValid();
}

void CMdiVC6View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMdiVC6Doc* CMdiVC6View::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMdiVC6Doc)));
	return (CMdiVC6Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6View クラスのメッセージ ハンドラ
