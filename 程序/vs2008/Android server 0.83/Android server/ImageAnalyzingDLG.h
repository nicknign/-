#pragma once




// ImageAnalyzingDLG 对话框

class ImageAnalyzingDLG : public CDialog
{
	DECLARE_DYNAMIC(ImageAnalyzingDLG)

public:
	ImageAnalyzingDLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ImageAnalyzingDLG();

// 对话框数据
	enum { IDD = IDD_ImageAnalyzing };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

