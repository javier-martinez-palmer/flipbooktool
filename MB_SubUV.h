
// MB_SubUV.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMB_SubUVApp:
// See MB_SubUV.cpp for the implementation of this class
//

class CMB_SubUVApp : public CWinAppEx
{
public:
	CMB_SubUVApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMB_SubUVApp theApp;