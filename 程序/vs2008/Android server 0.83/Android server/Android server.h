// Android server.h : Android server Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "cv.h"
#include "highgui.h"
#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256
#define IMAGE_CHANNELS 3



// CAndroidserverApp:
// �йش����ʵ�֣������ Android server.cpp
//

class CAndroidserverApp : public CWinApp
{
public:
	CAndroidserverApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAndroidserverApp theApp;