// Android serverDoc.cpp : CAndroidserverDoc 类的实现
//

#include "stdafx.h"
#include "Android server.h"

#include "Android serverDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAndroidserverDoc

IMPLEMENT_DYNCREATE(CAndroidserverDoc, CDocument)

BEGIN_MESSAGE_MAP(CAndroidserverDoc, CDocument)
END_MESSAGE_MAP()


// CAndroidserverDoc 构造/析构

CAndroidserverDoc::CAndroidserverDoc()
{
	// TODO: 在此添加一次性构造代码

}

CAndroidserverDoc::~CAndroidserverDoc()
{
}

BOOL CAndroidserverDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CAndroidserverDoc 序列化

void CAndroidserverDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CAndroidserverDoc 诊断

#ifdef _DEBUG
void CAndroidserverDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAndroidserverDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAndroidserverDoc 命令
