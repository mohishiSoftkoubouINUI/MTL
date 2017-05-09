// MdiVC6Doc.cpp : CMdiVC6Doc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "MdiVC6.h"

#include "MdiVC6Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6Doc

IMPLEMENT_DYNCREATE(CMdiVC6Doc, CDocument)

BEGIN_MESSAGE_MAP(CMdiVC6Doc, CDocument)
	//{{AFX_MSG_MAP(CMdiVC6Doc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6Doc クラスの構築/消滅

CMdiVC6Doc::CMdiVC6Doc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CMdiVC6Doc::~CMdiVC6Doc()
{
}

BOOL CMdiVC6Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMdiVC6Doc シリアライゼーション

void CMdiVC6Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: この位置に保存用のコードを追加してください。
	}
	else
	{
		// TODO: この位置に読み込み用のコードを追加してください。
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6Doc クラスの診断

#ifdef _DEBUG
void CMdiVC6Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMdiVC6Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMdiVC6Doc コマンド
