// Android serverDoc.h : CAndroidserverDoc ��Ľӿ�
//


#pragma once


class CAndroidserverDoc : public CDocument
{
protected: // �������л�����
	CAndroidserverDoc();
	DECLARE_DYNCREATE(CAndroidserverDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CAndroidserverDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


