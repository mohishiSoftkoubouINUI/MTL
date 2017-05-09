// SdiVC6Doc.cpp : CSdiVC6Doc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "SdiVC6.h"

#include "SdiVC6Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6Doc

IMPLEMENT_DYNCREATE(CSdiVC6Doc, CDocument)

BEGIN_MESSAGE_MAP(CSdiVC6Doc, CDocument)
	//{{AFX_MSG_MAP(CSdiVC6Doc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6Doc クラスの構築/消滅

CSdiVC6Doc::CSdiVC6Doc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CSdiVC6Doc::~CSdiVC6Doc()
{
}

BOOL CSdiVC6Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSdiVC6Doc シリアライゼーション

void CSdiVC6Doc::Serialize(CArchive& ar)
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
// CSdiVC6Doc クラスの診断

#ifdef _DEBUG
void CSdiVC6Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSdiVC6Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSdiVC6Doc コマンド
