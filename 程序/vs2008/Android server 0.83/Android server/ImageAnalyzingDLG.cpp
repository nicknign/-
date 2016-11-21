// ImageAnalyzingDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "Android server.h"
#include "ImageAnalyzingDLG.h"

#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>

IplImage *image = 0, *hsv = 0, *hue = 0, *mask = 0, *backproject = 0, *histimg = 0;
CvHistogram *hist = 0;

int backproject_mode = 0;
int select_object = 0;
int track_object = 0;
int show_hist = 1;  
CvPoint origin;
CvRect selection = {0};
CvRect track_window;
CvBox2D track_box;  // tracking 返回的区域 box，带角度
CvConnectedComp track_comp;
int hdims = 48;     // 划分HIST的个数，越高越精确
float hranges_arr[] = {0,180};
float* hranges = hranges_arr;
int vmin = 10, vmax = 256, smin = 30;



void on_mouse( int event, int x, int y, int flags, void* param)
{
    if( !image )
        return;

    if( image->origin )
        y = image->height - y;

    if( select_object )
    {
        selection.x = MIN(x,origin.x);
        selection.y = MIN(y,origin.y);
        selection.width = selection.x + CV_IABS(x - origin.x);
        selection.height = selection.y + CV_IABS(y - origin.y);
        
        selection.x = MAX( selection.x, 0 );
        selection.y = MAX( selection.y, 0 );
        selection.width = MIN( selection.width, image->width );
        selection.height = MIN( selection.height, image->height );
        selection.width -= selection.x;
        selection.height -= selection.y;

    }

    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        origin = cvPoint(x,y);
        selection = cvRect(x,y,0,0);
        select_object = 1;
        break;
    case CV_EVENT_LBUTTONUP:
        select_object = 0;
        if( selection.width > 0 && selection.height > 0 )
            track_object = -1;
#ifdef _DEBUG
    printf("\n # 鼠标的选择区域："); 
    printf("\n   X = %d, Y = %d, Width = %d, Height = %d",
        selection.x, selection.y, selection.width, selection.height);
#endif
        break;
    }
}

CvScalar hsv2rgb( float hue )
{
    int rgb[3], p, sector;
    static const int sector_data[][3]=
        {{0,2,1}, {1,2,0}, {1,0,2}, {2,0,1}, {2,1,0}, {0,1,2}};
    hue *= 0.033333333333333333333333333333333f;
    sector = cvFloor(hue);
    p = cvRound(255*(hue - sector));
    p ^= sector & 1 ? 255 : 0;

    rgb[sector_data[sector][0]] = 255;
    rgb[sector_data[sector][1]] = 0;
    rgb[sector_data[sector][2]] = p;

#ifdef _DEBUG
    printf("\n # Convert HSV to RGB："); 
    printf("\n   HUE = %f", hue);
    printf("\n   R = %d, G = %d, B = %d", rgb[0],rgb[1],rgb[2]);
#endif

    return cvScalar(rgb[2], rgb[1], rgb[0],0);
}








// ImageAnalyzingDLG 对话框

IMPLEMENT_DYNAMIC(ImageAnalyzingDLG, CDialog)

ImageAnalyzingDLG::ImageAnalyzingDLG(CWnd* pParent /*=NULL*/)
	: CDialog(ImageAnalyzingDLG::IDD, pParent)
	, TheImage(NULL)
{

}

ImageAnalyzingDLG::~ImageAnalyzingDLG()
{
}

void ImageAnalyzingDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ImageAnalyzingDLG, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_YUANTU, &ImageAnalyzingDLG::OnBnClickedYuantu)
	ON_BN_CLICKED(IDC_CANNY, &ImageAnalyzingDLG::OnBnClickedCanny)
	ON_BN_CLICKED(IDC_BUTTON11, &ImageAnalyzingDLG::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON1, &ImageAnalyzingDLG::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_close, &ImageAnalyzingDLG::OnBnClickedclose)
	ON_BN_CLICKED(IDC_SEARCH, &ImageAnalyzingDLG::OnBnClickedSearch)
	ON_BN_CLICKED(IDC_autochase, &ImageAnalyzingDLG::OnBnClickedautochase)
	ON_BN_CLICKED(IDC_GetBackGround, &ImageAnalyzingDLG::OnBnClickedGetbackground)
END_MESSAGE_MAP()


// ImageAnalyzingDLG 消息处理程序

BOOL ImageAnalyzingDLG::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CvSize ImgSize;
    ImgSize.height = IMAGE_HEIGHT;
    ImgSize.width = IMAGE_WIDTH;
    TheImage = cvCreateImage( ImgSize, IPL_DEPTH_8U, IMAGE_CHANNELS );
	
	 IplImage* ipl = cvLoadImage( "aa.jpg", 1 );    // 读取图片、缓存到一个局部变量 ipl 中
    if( !ipl )                                    // 判断是否成功载入图片
        return 0;
    if( TheImage )                                // 对上一幅显示的图片数据清零
        cvZero( TheImage );
    
    //ResizeImage( ipl );    // 对读入的图片进行缩放，使其宽或高最大值者刚好等于 256，再复制到 TheImage 中
	TheImage = ipl;
	TheTarget = 0;
	TheBack = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void ImageAnalyzingDLG::ShowImage( IplImage* img, UINT ID )    // ID 是Picture Control控件的ID号
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

void ImageAnalyzingDLG::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码    
	ShowImage( TheImage, IDC_PICTURE );            // 调用显示图片函数  
	ShowImage( TheTarget, IDC_TARGET );            // 调用显示图片函数
	ShowImage( TheBack, IDC_BACK );
   // cvReleaseImage( &TheImage );                        // 释放 ipl 占用的内存	

	// 不为绘图消息调用 CDialog::OnPaint()
}

void ImageAnalyzingDLG::OnBnClickedYuantu()
{
	// TODO: 在此添加控件通知处理程序代码
	cvReleaseImage( &TheImage );
	CvSize ImgSize;
    ImgSize.height = IMAGE_HEIGHT;
    ImgSize.width = IMAGE_WIDTH;
    TheImage = cvCreateImage( ImgSize, IPL_DEPTH_8U, IMAGE_CHANNELS );

	IplImage* ipl = cvLoadImage( "aa.jpg", 1 );    // 读取图片、缓存到一个局部变量 ipl 中
   
    if( TheImage )                                // 对上一幅显示的图片数据清零
        cvZero( TheImage );
	
    
   // ResizeImage( ipl );    // 对读入的图片进行缩放，使其宽或高最大值者刚好等于 256，再复制到 TheImage 中
	TheImage = ipl;
	ShowImage( TheImage, IDC_PICTURE );
}

void ImageAnalyzingDLG::OnBnClickedCanny()
{
	// TODO: 在此添加控件通知处理程序代码
	IplImage *gray = 0, *edge = 0;
	
    gray = cvCreateImage( cvSize(TheImage->width, TheImage->height), IPL_DEPTH_8U, 1 );
    edge = cvCreateImage( cvSize(TheImage->width, TheImage->height), IPL_DEPTH_8U, 1 );	
    cvCvtColor( TheImage, gray, CV_BGR2GRAY );
    cvCanny( gray, edge, 30, 100, 3 );       
    ShowImage( edge, IDC_PICTURE );            // 调用显示图片函数
    cvReleaseImage( &gray );
    cvReleaseImage( &edge );


}

void ImageAnalyzingDLG::ResizeImage(IplImage* img)
{
    // 读取图片的宽和高
    int w = img->width;
    int h = img->height;
    // 找出宽和高中的较大值者
    int max = (w > h)? w: h;
    // 计算将图片缩放到TheImage区域所需的比例因子
    float scale = (float) ( (float) max / 256.0f );
    
    // 缩放后图片的宽和高
    int nw = (int)( w/scale );
    int nh = (int)( h/scale );
    // 为了将缩放后的图片存入 TheImage 的正中部位，需计算图片在 TheImage 左上角的期望坐标值
    int tlx = (nw > nh)? 0: (int)(256-nw)/2;
    int tly = (nw > nh)? (int)(256-nh)/2: 0;
    // 设置 TheImage 的 ROI 区域，用来存入图片 img
    cvSetImageROI( TheImage, cvRect( tlx, tly, nw, nh) );
    // 对图片 img 进行缩放，并存入到 TheImage 中
    cvResize( img, TheImage );
    // 重置 TheImage 的 ROI 准备读入下一幅图片
    cvResetImageROI( TheImage );
}

void ImageAnalyzingDLG::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
    // TODO: 在此添加控件通知处理程序代码
	IplImage *gray = 0, *edge = 0;
	
	gray = cvCreateImage( cvSize(TheImage->width, TheImage->height), IPL_DEPTH_8U, 1 );    
    cvCvtColor( TheImage, gray, CV_BGR2GRAY );   
    //cvCvtColor( gray, TheImage, CV_GRAY2BGR );    
    ShowImage( gray, IDC_PICTURE );            // 调用显示图片函数
    cvReleaseImage( &gray );
  
}

void ImageAnalyzingDLG::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	IplImage* frame = 0;
	 int c;	
	 flag = 0;
	 cvNamedWindow( "CamShiftTarget", 1 );
    cvSetMouseCallback( "CamShiftTarget", on_mouse, NULL ); // on_mouse 自定义事件
	 frame = TheImage;
	 for(;;)
    {
	  if( !image )
        {
            /* allocate all the buffers */
            image = cvCreateImage( cvGetSize(frame), 8, 3 );
            image->origin = frame->origin;
            hsv = cvCreateImage( cvGetSize(frame), 8, 3 );
            hue = cvCreateImage( cvGetSize(frame), 8, 1 );
            mask = cvCreateImage( cvGetSize(frame), 8, 1 );
            backproject = cvCreateImage( cvGetSize(frame), 8, 1 );
            hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );  // 计算直方图
            histimg = cvCreateImage( cvSize(320,200), 8, 3 );
            cvZero( histimg );
        }
	  cvCopy( frame, image, 0 );
        cvCvtColor( image, hsv, CV_BGR2HSV );  // 彩色空间转换 BGR to HSV 

        if( track_object )
        {
            int _vmin = vmin, _vmax = vmax;

            cvInRangeS( hsv, cvScalar(0,smin,MIN(_vmin,_vmax),0),
                        cvScalar(180,256,MAX(_vmin,_vmax),0), mask );  // 得到二值的MASK
            cvSplit( hsv, hue, 0, 0, 0 );  // 只提取 HUE 分量

            if( track_object < 0 )
            {
                float max_val = 0.f;
                cvSetImageROI( hue, selection );  // 得到选择区域 for ROI
                cvSetImageROI( mask, selection ); // 得到选择区域 for mask
                cvCalcHist( &hue, hist, 0, mask ); // 计算直方图
                cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );  // 只找最大值
                cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 ); // 缩放 bin 到区间 [0,255] 
                cvResetImageROI( hue );  // remove ROI
                cvResetImageROI( mask );
                track_window = selection;
                track_object = 1;               
            }           
		}
		 if( select_object && selection.width > 0 && selection.height > 0 )
			{
            cvSetImageROI( image, selection );
            cvXorS( image, cvScalarAll(255), image, 0 );
            cvResetImageROI( image );
			}      
        cvShowImage( "CamShiftTarget", image ); 		
		 c = cvWaitKey(10);  
		 
		 if(selection.height != 0 && selection.width != 0)
		 {
			cvSetImageROI(image , selection);
			TheTarget = cvCreateImage(cvSize(selection.width,selection.height),8,3);
			cvCopy(image,TheTarget,0);
			cvResetImageROI( image );
			ShowImage( TheTarget, IDC_TARGET );
			
		 }
		 if(flag)
		 {
			Invalidate();			
			cvDestroyWindow("CamShiftTarget");
			break;
		 }
	 }
	
}

void ImageAnalyzingDLG::OnBnClickedclose()
{
	// TODO: 在此添加控件通知处理程序代码
	flag = 1;
	select = selection;
}

void ImageAnalyzingDLG::OnBnClickedSearch()
{
	// TODO: 在此添加控件通知处理程序代码   
    IplImage* frame = 0;    
        int i, bin_w;
        frame = TheImage;
        if( !frame )
           return;
	image = 0;
        if( !image )
        {
            /* allocate all the buffers */
            image = cvCreateImage( cvGetSize(frame), 8, 3 );
            image->origin = frame->origin;
            hsv = cvCreateImage( cvGetSize(frame), 8, 3 );
            hue = cvCreateImage( cvGetSize(frame), 8, 1 );
            mask = cvCreateImage( cvGetSize(frame), 8, 1 );
            backproject = cvCreateImage( cvGetSize(frame), 8, 1 );
            hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );  // 计算直方图
            histimg = cvCreateImage( cvSize(320,200), 8, 3 );
            cvZero( histimg );
        }

        cvCopy( frame, image, 0 );
        cvCvtColor( image, hsv, CV_BGR2HSV );  // 彩色空间转换 BGR to HSV 

        if( 1 )
        {
            int _vmin = vmin, _vmax = vmax;

            cvInRangeS( hsv, cvScalar(0,smin,MIN(_vmin,_vmax),0),
                        cvScalar(180,256,MAX(_vmin,_vmax),0), mask );  // 得到二值的MASK
            cvSplit( hsv, hue, 0, 0, 0 );  // 只提取 HUE 分量

            if( track_object < 0 )
            {
                float max_val = 0.f;
                cvSetImageROI( hue, selection );  // 得到选择区域 for ROI
                cvSetImageROI( mask, selection ); // 得到选择区域 for mask
                cvCalcHist( &hue, hist, 0, mask ); // 计算直方图
                cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );  // 只找最大值
                cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 ); // 缩放 bin 到区间 [0,255] 
                cvResetImageROI( hue );  // remove ROI
                cvResetImageROI( mask );
                track_window = selection;
                track_object = 1;

                cvZero( histimg );
                bin_w = histimg->width / hdims;  // hdims: 条的个数，则 bin_w 为条的宽度
                
                // 画直方图
                for( i = 0; i < hdims; i++ )
                {
                    int val = cvRound( cvGetReal1D(hist->bins,i)*histimg->height/255 );
                    CvScalar color = hsv2rgb(i*180.f/hdims);
                    cvRectangle( histimg, cvPoint(i*bin_w,histimg->height),
                                 cvPoint((i+1)*bin_w,histimg->height - val),
                                 color, -1, 8, 0 );
                }
            }

            cvCalcBackProject( &hue, backproject, hist );  // 使用 back project 方法
            cvAnd( backproject, mask, backproject, 0 );
            
            // calling CAMSHIFT 算法模块
            cvCamShift( backproject, track_window,
                        cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),
                        &track_comp, &track_box );
            track_window = track_comp.rect;
            
            if( backproject_mode )
                cvCvtColor( backproject, image, CV_GRAY2BGR ); // 使用backproject灰度图像
            if( image->origin )
                track_box.angle = -track_box.angle;
            cvEllipseBox( TheImage, track_box, CV_RGB(255,0,0), 3, CV_AA, 0 );
        }
			Invalidate();	
}

void ImageAnalyzingDLG::OnBnClickedautochase()
{
	// TODO: 在此添加控件通知处理程序代码
	long Rvalue,Gvalue,Bvalue;
	Rvalue = 0;
	Bvalue = 0;
	Gvalue = 0;
	int width = TheBack->width;
	int height = TheBack->height;
	int row,col;
	for( row=0;row<height;row++ )
		for( col=0;col<width;col++ )
		{
			Bvalue +=CV_IMAGE_ELEM(TheBack, unsigned char, row, col*3+0);
			Gvalue +=CV_IMAGE_ELEM(TheBack, unsigned char, row, col*3+1);
			Rvalue +=CV_IMAGE_ELEM(TheBack, unsigned char, row, col*3+2);
		}
	Rvalue = Rvalue/(width*height);
	Gvalue = Gvalue/(width*height);
	Bvalue = Bvalue/(width*height);//求取地面平均颜色

	int xstart,xend,ystart,yend,x,y;
	long Rpin=0,Gpin=0,Bpin=0;
	int i=0;
	int Hwidth = TheImage->width;
	int Hheight = TheImage->height;
	IplImage *ground = 0;
	ground = cvCreateImage( cvSize(TheImage->width, TheImage->height), IPL_DEPTH_8U, 3 );	

	for(xstart=0,xend=Hwidth/3;xend<=Hwidth;xstart+=Hwidth/3,xend+=Hwidth/3)
		for(ystart=0,yend=Hheight/3;yend<=Hheight;yend+=Hheight/3,ystart+=Hheight/3)
		{
			for(x=xstart;x<xend;x++)
				for(y=ystart;y<yend;y++)
				{
					Bpin +=CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+0);
					Gpin +=CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+1);
					Rpin +=CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+2);
				}
			Bpin = Bpin/((Hwidth/3)*(Hheight/3));
			Gpin = Gpin/((Hwidth/3)*(Hheight/3));
			Rpin = Rpin/((Hwidth/3)*(Hheight/3));

			if( abs(Bpin - Bvalue) < 30 && abs(Gpin - Gvalue) < 30 && abs(Rpin - Rvalue) < 30)
			{
				for(x=xstart;x<xend;x++)
				for(y=ystart;y<yend;y++)
				{
					CV_IMAGE_ELEM(ground, unsigned char, y, x*3+0)=255;
					CV_IMAGE_ELEM(ground, unsigned char, y, x*3+1)=0;
					CV_IMAGE_ELEM(ground, unsigned char, y, x*3+2)=0;

					CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+0)=(uchar)Bvalue;
					CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+1)=(uchar)Gvalue;
					CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+2)=(uchar)Rvalue;
				}
			}
			else
			{
				for(x=xstart;x<xend;x++)
				for(y=ystart;y<yend;y++)
				{
					CV_IMAGE_ELEM(ground, unsigned char, y, x*3+0)=0;
					CV_IMAGE_ELEM(ground, unsigned char, y, x*3+1)=0;
					CV_IMAGE_ELEM(ground, unsigned char, y, x*3+2)=255;
				}
			}
			i++;
		}
		//cvNamedWindow( "groundTest01", 1 );
		//cvShowImage( "groundTest01",ground );             
		cvReleaseImage( &ground);											//九方格预处理

	Rpin=0;
	Gpin=0;
	Bpin=0;
	i=0;
	IplImage *ground2 = 0;
	ground2 = cvCreateImage( cvSize(TheImage->width, TheImage->height), IPL_DEPTH_8U, 3 );	

	for(xstart=0,xend=Hwidth/9;xend<=Hwidth;xstart+=Hwidth/9,xend+=Hwidth/9)
		for(ystart=0,yend=Hheight/9;yend<=Hheight;yend+=Hheight/9,ystart+=Hheight/9)
		{
			for(x=xstart;x<xend;x++)
				for(y=ystart;y<yend;y++)
				{
					Bpin +=CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+0);
					Gpin +=CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+1);
					Rpin +=CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+2);
				}
			Bpin = Bpin/((Hwidth/9)*(Hheight/9));
			Gpin = Gpin/((Hwidth/9)*(Hheight/9));
			Rpin = Rpin/((Hwidth/9)*(Hheight/9));

			if( abs(Bpin - Bvalue) < 30 && abs(Gpin - Gvalue) < 30 && abs(Rpin - Rvalue) < 30)
			{
				for(x=xstart;x<xend;x++)
				for(y=ystart;y<yend;y++)
				{
					CV_IMAGE_ELEM(ground2, unsigned char, y, x*3+0)=255;
					CV_IMAGE_ELEM(ground2, unsigned char, y, x*3+1)=0;
					CV_IMAGE_ELEM(ground2, unsigned char, y, x*3+2)=0;

					CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+0)=(uchar)Bvalue;
					CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+1)=(uchar)Gvalue;
					CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+2)=(uchar)Rvalue;
				}
			}
			else
			{
				for(x=xstart;x<xend;x++)
				for(y=ystart;y<yend;y++)
				{
					CV_IMAGE_ELEM(ground2, unsigned char, y, x*3+0)=0;
					CV_IMAGE_ELEM(ground2, unsigned char, y, x*3+1)=0;
					CV_IMAGE_ELEM(ground2, unsigned char, y, x*3+2)=255;
				}
			}
			i++;
		}
		//cvNamedWindow( "groundTest02", 1 );
		//cvShowImage( "groundTest02",ground2 );             
		cvReleaseImage( &ground2);											//八十一方格预处理

	Rpin=0;
	Gpin=0;
	Bpin=0;
	i=0;
	IplImage *ground3 = 0;
	ground3 = cvCreateImage( cvSize(TheImage->width, TheImage->height), IPL_DEPTH_8U, 3 );	

	for(xstart=0,xend=Hwidth/27;xend<=Hwidth;xstart+=Hwidth/27,xend+=Hwidth/27)
		for(ystart=0,yend=Hheight/27;yend<=Hheight;yend+=Hheight/27,ystart+=Hheight/27)
		{
			for(x=xstart;x<xend;x++)
				for(y=ystart;y<yend;y++)
				{
					Bpin +=CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+0);
					Gpin +=CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+1);
					Rpin +=CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+2);
				}
			Bpin = Bpin/((Hwidth/27)*(Hheight/27));
			Gpin = Gpin/((Hwidth/27)*(Hheight/27));
			Rpin = Rpin/((Hwidth/27)*(Hheight/27));

			if( abs(Bpin - Bvalue) < 40 && abs(Gpin - Gvalue) < 40 && abs(Rpin - Rvalue) < 40)
			{
				for(x=xstart;x<xend;x++)
				for(y=ystart;y<yend;y++)
				{
					CV_IMAGE_ELEM(ground3, unsigned char, y, x*3+0)=255;
					CV_IMAGE_ELEM(ground3, unsigned char, y, x*3+1)=0;
					CV_IMAGE_ELEM(ground3, unsigned char, y, x*3+2)=0;

					CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+0)=(uchar)Bvalue;
					CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+1)=(uchar)Gvalue;
					CV_IMAGE_ELEM(TheImage, unsigned char, y, x*3+2)=(uchar)Rvalue;
				}
			}
			else
			{
				for(x=xstart;x<xend;x++)
				for(y=ystart;y<yend;y++)
				{
					CV_IMAGE_ELEM(ground3, unsigned char, y, x*3+0)=0;
					CV_IMAGE_ELEM(ground3, unsigned char, y, x*3+1)=0;
					CV_IMAGE_ELEM(ground3, unsigned char, y, x*3+2)=255;
				}
			}
			i++;
		}
		cvNamedWindow( "groundTest03", 1 );
		cvShowImage( "groundTest03",ground3 );             
		cvReleaseImage( &ground3);											//九成八十一方格预处理

		Invalidate();

}

void ImageAnalyzingDLG::OnBnClickedGetbackground()
{
	// TODO: 在此添加控件通知处理程序代码
	IplImage* frame = 0;
	 int c;	
	 flag = 0;
	 cvNamedWindow( "CamShiftTarget", 1 );
    cvSetMouseCallback( "CamShiftTarget", on_mouse, NULL ); // on_mouse 自定义事件
	 frame = TheImage;
	 for(;;)
    {
	  if( !image )
        {
            /* allocate all the buffers */
            image = cvCreateImage( cvGetSize(frame), 8, 3 );
            image->origin = frame->origin;
            hsv = cvCreateImage( cvGetSize(frame), 8, 3 );
            hue = cvCreateImage( cvGetSize(frame), 8, 1 );
            mask = cvCreateImage( cvGetSize(frame), 8, 1 );
            backproject = cvCreateImage( cvGetSize(frame), 8, 1 );
            hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );  // 计算直方图
            histimg = cvCreateImage( cvSize(320,200), 8, 3 );
            cvZero( histimg );
        }
	  cvCopy( frame, image, 0 );
        cvCvtColor( image, hsv, CV_BGR2HSV );  // 彩色空间转换 BGR to HSV 

        if( track_object )
        {
            int _vmin = vmin, _vmax = vmax;

            cvInRangeS( hsv, cvScalar(0,smin,MIN(_vmin,_vmax),0),
                        cvScalar(180,256,MAX(_vmin,_vmax),0), mask );  // 得到二值的MASK
            cvSplit( hsv, hue, 0, 0, 0 );  // 只提取 HUE 分量

            if( track_object < 0 )
            {
                float max_val = 0.f;
                cvSetImageROI( hue, selection );  // 得到选择区域 for ROI
                cvSetImageROI( mask, selection ); // 得到选择区域 for mask
                cvCalcHist( &hue, hist, 0, mask ); // 计算直方图
                cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );  // 只找最大值
                cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 ); // 缩放 bin 到区间 [0,255] 
                cvResetImageROI( hue );  // remove ROI
                cvResetImageROI( mask );
                track_window = selection;
                track_object = 1;             
               
            }            
		}
		 if( select_object && selection.width > 0 && selection.height > 0 )
			{
            cvSetImageROI( image, selection );
            cvXorS( image, cvScalarAll(255), image, 0 );
            cvResetImageROI( image );
			}      
        cvShowImage( "CamShiftTarget", image ); 		
		 c = cvWaitKey(10);  
		 
		 if(selection.height != 0 && selection.width != 0)
		 {
			cvSetImageROI(image , selection);
			TheBack = cvCreateImage(cvSize(selection.width,selection.height),8,3);
			cvCopy(image,TheBack,0);
			cvResetImageROI( image );
			ShowImage( TheBack, IDC_BACK );
			
		 }
		 if(flag)
		 {
			Invalidate();			
			cvDestroyWindow("CamShiftTarget");
			break;
		 }
	 }
}
