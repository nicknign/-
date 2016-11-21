// Android serverView.h : CAndroidserverView ��Ľӿ�
//


#pragma once

#define WM_LINK     WM_USER+1
#define WM_INPUT2    WM_USER+2
#define WM_OUTPUT   WM_USER+3
#define WM_CAMERA	WM_USER+4

#include "ImageAnalyzingDLG.h"
#include "ameraSet.h"
#include <math.h>
#include "afxwin.h"



struct RECVPARAM1  
{	
	CSocket* psock;
	CSocket* psocksend;
	HWND hwnd;
};

struct PP//�������������ݽṹ��
{
	double x;
	double y;
};

class CAndroidserverView : public CFormView
{
protected: // �������л�����
	CAndroidserverView();
	DECLARE_DYNCREATE(CAndroidserverView)

public:
	enum{ IDD = IDD_ANDROIDSERVER_FORM };

// ����
public:
	CAndroidserverDoc* GetDocument() const;

// ����
public:	
		

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CAndroidserverView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HICON m_hIcon;
	DWORD ThreadID;
	DWORD ThreadID1;
	HANDLE hThread;
	HANDLE hThread1;
	CSocket sockTemp;
	CSocket sockSend;
	CSocket* sockConn;
	CvBox2D track_box;  // tracking ���ص����� box�����Ƕ�

	int speed;
	BOOL Flag;


// ���ɵ���Ϣӳ�亯��
protected:	
	afx_msg LRESULT OnLink(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT SetInput(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT SetOutput(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnCamera(WPARAM wParam,LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
public:
	//static UINT ReceiveDataThread(LPVOID lpParam);	
	void ShowImage( IplImage* img, UINT ID );
	void ResizeImage(IplImage* img);
	double CanculateDistance(int l_pixcel);
	double CanculateWidth(int w_pixcel,int l_pixcel);
	int CanculateWpixel(double W,int l_pixcel);
	void MeanShiftSmooth(const IplImage* src, IplImage* dst, int hs, int hr, int maxIter);
	int dis(CPoint *a,CPoint *b);

	void DrawBox(CvBox2D box,IplImage* img);

	bool Rundirect();//ֱ����������
	void Gototarget();//��·������

	void DrawMap();
	
	afx_msg void OnstartServer();
	
	static DWORD WINAPI fun2proc(LPVOID lpParameter1);
	static DWORD WINAPI fun1proc(LPVOID lpParameter);
	
	

	
	afx_msg void OnBnClickedButton5();

	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void Oncloseserver();
	CString m_speed;
	BOOL m_cameracheck;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedCheck2();
	//afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool InputOrOutput;
	ImageAnalyzingDLG *pTD;
	CameraSet *pTD1;
	IplImage* TheImage;	
	IplImage* TheTarget;
	IplImage* TheBack;
	IplImage* theshow;
	IplImage* TheImage1;
	IplImage* TheImage2;
	int ZHUANWAN;  //0 ֱ�У�1 ��ת��2 ��ת
	int direction; //���� ��map��0 �£�1 �ң�2 �ϣ�3 ��
	double D1;

	bool flag;
	bool biaoding;
	bool m_advanceinline;
	bool m_gototarget;
	CvRect selectrect;
	int track_object;
	CvHistogram* hist;
	IplImage* frame;
	IplImage* target;
	IplImage* image;
	IplImage* image2;
	IplImage* hsv;
	IplImage* hsv2;
	IplImage* hue;
	IplImage* hue2;
	IplImage* mask;
	IplImage* mask2;
	IplImage* backproject;
	IplImage* histimg;	
	CBrush m_brush;
	CPoint p_Car;
	CPoint p_Target;
	PP f_Target;
	CPoint p_obstacle[400];
	CPoint *road;
	CPoint p_road[400];	
	int countOB;
	//int CarAngle;//��ʱ,�Ե�ͼΪ׼ 0 �£�1 ��2 �ϣ�3 ��
	PP* Shade;

	afx_msg void OnImageAnalyzing();	
	afx_msg void OnBnClickedFind();
	BOOL m_trackshow;
	afx_msg void OnCameraSet();
	double m_distance;
	double Focus;
	afx_msg void OnBnClickedCanculateDis();
	double m_width;
	afx_msg void OnMenushishijianshi();
	afx_msg void OnBnClickedPictureprocess();
	afx_msg void OnBnClickedadvanceinline();
	afx_msg void OnBnClickedgototarget();
	afx_msg void OnBnClickedQianjin();
	afx_msg void OnBnClickedHoutui();
	afx_msg void OnBnClickedYouzhuan();
	afx_msg void OnBnClickedZuozhuan();
	afx_msg void OnBnClickedTingzhi();
	CStatic m_mapcontrol;
	void shading(void);
	void shadingchange(double award,double right,double left);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedRuntarget();
	afx_msg void OnBnClickedRoadcreat();
};

#ifndef _DEBUG  // Android serverView.cpp �еĵ��԰汾
inline CAndroidserverDoc* CAndroidserverView::GetDocument() const
   { return reinterpret_cast<CAndroidserverDoc*>(m_pDocument); }
#endif

