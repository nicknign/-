#pragma once




// ImageAnalyzingDLG �Ի���

class ImageAnalyzingDLG : public CDialog
{
	DECLARE_DYNAMIC(ImageAnalyzingDLG)

public:
	ImageAnalyzingDLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ImageAnalyzingDLG();

// �Ի�������
	enum { IDD = IDD_ImageAnalyzing };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowImage( IplImage* img, UINT ID );
	void ResizeImage(IplImage* img);
	afx_msg void OnPaint();
	IplImage* TheImage;
	IplImage* TheTarget;
	IplImage* TheBack;
	 bool flag;
	 CvRect select;


	afx_msg void OnBnClickedYuantu();
	afx_msg void OnBnClickedCanny();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedclose();
	afx_msg void OnBnClickedSearch();
	afx_msg void OnBnClickedautochase();
	afx_msg void OnBnClickedGetbackground();
};

