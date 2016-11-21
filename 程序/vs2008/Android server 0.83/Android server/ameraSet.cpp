// ameraSet.cpp : 实现文件
//

#include "stdafx.h"
#include "Android server.h"
#include "ameraSet.h"


// CameraSet 对话框

IMPLEMENT_DYNAMIC(CameraSet, CDialog)

CameraSet::CameraSet(CWnd* pParent /*=NULL*/)
	: CDialog(CameraSet::IDD, pParent)
	, chuizhi_H(0)
	, chuizhi_B(0)
	, chuizhi_S(0)
	, chuizhi_K(0)
	, chuizhi_F(0)
	, m_H(15.3)
	, D1(22.3)
	, D2(57.3)	
{	
}

CameraSet::~CameraSet()
{
}

void CameraSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_chuizhi_H, chuizhi_H);
	DDX_Text(pDX, IDC_chuizhi_B, chuizhi_B);
	DDX_Text(pDX, IDC_chuizhi_s, chuizhi_S);
	DDX_Text(pDX, IDC_chuizhi_K, chuizhi_K);
	DDX_Text(pDX, IDC_chuizhi_F, chuizhi_F);
	DDX_Text(pDX, IDC_zhiding_H, m_H);
	DDX_Text(pDX, IDC_zhiding_D1, D1);
	DDX_Text(pDX, IDC_zhiding_D2, D2);	
}


BEGIN_MESSAGE_MAP(CameraSet, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_Auto_Deside, &CameraSet::OnBnClickedAutoDeside)
END_MESSAGE_MAP()


// CameraSet 消息处理程序

void CameraSet::ShowImage( IplImage* img, UINT ID )    // ID 是Picture Control控件的ID号
{
	if(!img)
		return;
    CDC* pDC = GetDlgItem( ID ) ->GetDC();        // 获得显示控件的 DC
    HDC hDC = pDC ->GetSafeHdc();                // 获取 HDC(设备句柄) 来进行绘图操作
    CRect rect;
    GetDlgItem(ID) ->GetClientRect( &rect );
    int rw = rect.right - rect.left;            // 求出图片控件的宽和高
    int rh = rect.bottom - rect.top;
    int iw = img->width;                        // 读取图片的宽和高
    int ih = img->height;
    int tx = (int)(rw - iw)/2;                    // 使图片的显示位置正好在控件的正中
    int ty = (int)(rh - ih)/2;
    SetRect( rect, tx, ty, tx+iw, ty+ih );
    CvvImage cimg;
    cimg.CopyOf( img );                            // 复制图片
    cimg.DrawToHDC( hDC, &rect );                // 将图片绘制到显示控件的指定区域内
    ReleaseDC( pDC );
}

void CameraSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	IplImage* ipl = cvLoadImage( "chuizhiyuanli.jpg", 1 );
	ShowImage( ipl,IDC_CHUIZHIYUANLI );   
	// 不为绘图消息调用 CDialog::OnPaint()
}

void CameraSet::OnBnClickedAutoDeside()
{
	// TODO: 在此添加控件通知处理程序代码
	double a1,I;
	int height;
	UpdateData(TRUE);	
	chuizhi_H = m_H;
	chuizhi_B = atan(D1/m_H);

	a1 = atan(D2/m_H) - chuizhi_B;
	chuizhi_S = chuizhi_F/cos(a1);
	
	I = 2*chuizhi_F*tan(a1);
	height = 384; //暂定
	chuizhi_K = I/height;
	UpdateData(FALSE);
}