// Android server.h : Android server 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "cv.h"
#include "highgui.h"
#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256
#define IMAGE_CHANNELS 3



// CAndroidserverApp:
// 有关此类的实现，请参阅 Android server.cpp
//

class CAndroidserverApp : public CWinApp
{
public:
	CAndroidserverApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAndroidserverApp theApp;