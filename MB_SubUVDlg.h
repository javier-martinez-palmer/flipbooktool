
// MB_SubUVDlg.h : header file
//

#pragma once
#include <string>
#include "ximage.h"
#include <iostream> 
#include <fstream>
#include <cctype>
#include <time.h>
#include "afxwin.h"
#include "afxcmn.h"
using namespace std; 

#define MAX_LOADSTRING 100
#define MAX_LOADIMAGE 128



// CMB_SubUVDlg dialog
class CMB_SubUVDlg : public CDialog
{
// Construction
public:
	CMB_SubUVDlg(CWnd* pParent = NULL);	// standard constructor
	int m_numFrames;
	int m_numCols;
	int m_numRows;
	int m_sizeFrame;
	string m_fileDimensions;

	HBITMAP        g_hBMP;
	
	CxImage m_Image[MAX_LOADIMAGE];// = new CxImage();
	CxImage m_genImage;
	CxImage m_prevImage;

	enum Order
	{
		Normal,
		NormalInverted,
		Reverse,
		ReverseInverted,
		Random
	};

// Dialog Data
	enum { IDD = IDD_MB_SUBUV_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool OpenFileDialog( HWND hwnd, string & filename); 
	bool SaveFileDialog( HWND hwnd, string & filename);
	bool LoadTheBitmap(string & filename);
	bool SaveTheBitmap( CxImage & newImage, string & filename);
	bool ProcessImage(int & rows, int & columns, int & size);
	bool AddFileToList(string & filename);
	bool CleanList(void);
	bool RowsAdvice(void);
	bool UpdateFileSizeMessage(void);
	bool UpdateFileRowColSize(void);

	afx_msg void OnBnClickedCheck1();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CListBox fileFrames;
	BOOL m_OrderSelect;
	Order m_OrderType;

	afx_msg void OnCbnSelchangeComboboxex1();
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_numberOfRows;
	CComboBox m_numberOfColumns;
	BOOL m_alphaImage;
	afx_msg void OnCbnSelchangeCombo2();
	CComboBox m_CBFrameSize;
	BOOL m_fitPower2;
	BOOL m_framesPower2;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	CButton m_savePower2;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnCbnSelchangeCombo3();
	CListBox m_fileSize;
	CEdit m_EhighResample;
	CEdit m_EwidthResample;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	// //tutorial follow
	CStatic m_Picture;
};
