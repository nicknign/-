// ameraSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Android server.h"
#include "ameraSet.h"


// CameraSet �Ի���

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


// CameraSet ��Ϣ�������

void CameraSet::ShowImage( IplImage* img, UINT ID )    // ID ��Picture Control�ؼ���ID��
{
	if(!img)
		return;
    CDC* pDC = GetDlgItem( ID ) ->GetDC();        // �����ʾ�ؼ��� DC
    HDC hDC = pDC ->GetSafeHdc();                // ��ȡ HDC(�豸���) �����л�ͼ����
    CRect rect;
    GetDlgItem(ID) ->GetClientRect( &rect );
    int rw = rect.right - rect.left;            // ���ͼƬ�ؼ��Ŀ�͸�
    int rh = rect.bottom - rect.top;
    int iw = img->width;                        // ��ȡͼƬ�Ŀ�͸�
    int ih = img->height;
    int tx = (int)(rw - iw)/2;                    // ʹͼƬ����ʾλ�������ڿؼ�������
    int ty = (int)(rh - ih)/2;
    SetRect( rect, tx, ty, tx+iw, ty+ih );
    CvvImage cimg;
    cimg.CopyOf( img );                            // ����ͼƬ
    cimg.DrawToHDC( hDC, &rect );                // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
    ReleaseDC( pDC );
}

void CameraSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	IplImage* ipl = cvLoadImage( "chuizhiyuanli.jpg", 1 );
	ShowImage( ipl,IDC_CHUIZHIYUANLI );   
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CameraSet::OnBnClickedAutoDeside()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	double a1,I;
	int height;
	UpdateData(TRUE);	
	chuizhi_H = m_H;
	chuizhi_B = atan(D1/m_H);

	a1 = atan(D2/m_H) - chuizhi_B;
	chuizhi_S = chuizhi_F/cos(a1);
	
	I = 2*chuizhi_F*tan(a1);
	height = 384; //�ݶ�
	chuizhi_K = I/height;
	UpdateData(FALSE);
}