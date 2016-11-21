#pragma once

#include <math.h>


// CameraSet �Ի���

class CameraSet : public CDialog
{
	DECLARE_DYNAMIC(CameraSet)

public:
	CameraSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CameraSet();
	void ShowImage( IplImage* img, UINT ID );

// �Ի�������
	enum { IDD = IDD_CameraSet };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	double chuizhi_H;
	double chuizhi_B;
	double chuizhi_S;
	double chuizhi_K;
	double chuizhi_F;
	afx_msg void OnBnClickedAutoDeside();
	double m_H;
	double D1;
	double D2;	
};
