// Android serverDoc.cpp : CAndroidserverDoc ���ʵ��
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


// CAndroidserverDoc ����/����

CAndroidserverDoc::CAndroidserverDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CAndroidserverDoc::~CAndroidserverDoc()
{
}

BOOL CAndroidserverDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CAndroidserverDoc ���л�

void CAndroidserverDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CAndroidserverDoc ���

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


// CAndroidserverDoc ����
