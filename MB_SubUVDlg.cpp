
// MB_SubUVDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MB_SubUV.h"
#include "MB_SubUVDlg.h"
#include <stdio.h>
#include <vfw.h>              // show bmp's

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



bool CMB_SubUVDlg::RowsAdvice()
{
	int auxFrames = m_numFrames;
    char defSelect[2] = "1";
	//char *defSelectCol = new char(4);
	char *str = new char(10);


	str = itoa(m_numFrames,str,10);
	//clean previous values
	while(m_numberOfRows.DeleteString(0));
	while(m_numberOfColumns.DeleteString(0));

	m_numberOfRows.AddString(defSelect);
	m_numberOfRows.SelectString(1,defSelect);
	
	m_numberOfColumns.AddString(str);
	m_numberOfColumns.SelectString(1,str);


	for(int i=2,aux=0; i<=auxFrames; i++)
	{
		if((auxFrames%i)==0)
		{
			str = itoa(i,str,10);
			m_numberOfRows.AddString(str);
			aux = auxFrames/i;
			str = itoa(aux,str,10);
			m_numberOfColumns.AddString(str);
		}
	}

	UpdateFileRowColSize();
	UpdateFileSizeMessage();

	return TRUE;
}
bool CMB_SubUVDlg::UpdateFileRowColSize()
{
	int aux;
	char *ptr = new char(10);

	//ROWS & COLUMNS
	aux = m_numberOfRows.GetCurSel();
	m_numberOfRows.GetLBText(aux,ptr);
	m_numRows = atoi(ptr);
	m_numCols = m_numFrames/m_numRows;

	//IMAGE SIZE
	aux = m_CBFrameSize.GetCurSel();
	m_CBFrameSize.GetLBText(aux,ptr);
	m_sizeFrame = atoi(ptr);

	return TRUE;
}

bool CMB_SubUVDlg::UpdateFileSizeMessage()
{
	char sentence[100];

	sprintf(sentence,"File Dimensions %dp x %dp",m_numRows*m_sizeFrame,m_numCols*m_sizeFrame);
	m_fileSize.DeleteString(0);
	m_fileSize.AddString(sentence);

	return TRUE;
}
bool CMB_SubUVDlg::OpenFileDialog( HWND hwnd, string & filename) 
{
	//variables
	bool ret;
	OPENFILENAME ofname;
	void * namePool;

	//init
	namePool = calloc(10000,1);
	memset( &ofname, 0, sizeof(OPENFILENAME) ); 

	//RUN
	ofname.lStructSize = sizeof(OPENFILENAME);
	ofname.hwndOwner = hwnd;
	ofname.nFilterIndex = 0; 
	ofname.lpstrFile = (LPSTR)namePool;
	ofname.nMaxFile = 10000;
	ofname.lpstrInitialDir = NULL; 
	ofname.lpstrTitle = "Open picture...";
	ofname.hInstance = theApp.m_hInstance; 
	ofname.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING; 

	ret = FALSE; 
	if ( GetOpenFileName( &ofname ) ) 
	{
		filename.assign((char *)namePool); // conversion from windows to STL
		ret = TRUE;
	}

	//clean & leave
	free(namePool);
	return ret; 

}


bool CMB_SubUVDlg::SaveFileDialog( HWND hwnd, string & filename)
{
	//variables	
	OPENFILENAME ofname;
 	void * namePool;

	char extFile[7]="*.TGA";
	extFile[6]='\0'; // the second null required for the signal "last string in the list"

	bool ret;
	
	//init
	namePool = calloc(10000,1);
	memset( &ofname, 0, sizeof(OPENFILENAME) ); 

	//RUN
    ofname.lStructSize = sizeof(OPENFILENAME);
    ofname.hwndOwner = hwnd;
    ofname.lpstrFilter = extFile; 
	ofname.lpstrDefExt = "tga";
    ofname.nFilterIndex = 0; 
    ofname.lpstrFile = (LPSTR)namePool;
    ofname.nMaxFile = 10000;
    ofname.lpstrInitialDir = NULL; 
    ofname.lpstrTitle = "Save picture...";
	ofname.hInstance = theApp.m_hInstance; 
    ofname.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT; 

	ret = FALSE; 
    if ( GetSaveFileName( &ofname ) ) 
    {
		filename.assign((char *)namePool); // conversion from windows to STL
		ret = TRUE;
    }
	
	//clean & leave
	free(namePool);  
    return ret; 

}


bool CMB_SubUVDlg::AddFileToList(string & filename)
{
	//variables
	int pos;

	//init
	pos = filename.find_last_of('\\');
	const char * ptr = filename.c_str();

	//RUN
	ptr = &ptr[pos+1];
	fileFrames.AddString(ptr);

	//clean & save
	return TRUE;
}


bool CMB_SubUVDlg::CleanList()
{
	//RUN
	/*
	for (int i=0;i<=m_numFrames;i++)
	{
		m_Image[i].Destroy();
		fileFrames.DeleteString(0);
	}
	m_numFrames = 0;
	*/
	//make sure we delete all
	int i=0;
	while(fileFrames.DeleteString(0))
		m_Image[i++].Destroy();
	m_numFrames = 0;
	//clean & save
	return TRUE;
}


bool CMB_SubUVDlg::LoadTheBitmap(string & filename)
{	
	fstream file;
	char u,d,c;
	int index = m_numFrames;
	const char * ptr = filename.c_str();
	bool reading;
	
	reading = TRUE;

	do{
		//file.clear();
		file.open(  ptr, fstream::in | fstream::binary );//if exists 
		reading = !file.fail();
		if(file)  //WE READ ALL
		{
			reading = TRUE;
			file.close();
			if (m_numFrames < MAX_LOADIMAGE) // THAT IS AN ERROR
			{
				m_numFrames++;

				m_Image[index++].Load(ptr,CXIMAGE_FORMAT_TGA);
				AddFileToList(filename);
				

				u = filename[filename.length()-5]; //take unit
				d = filename[filename.length()-6]; //take dec
				c = filename[filename.length()-7]; //take cent
				if ( isdigit(u) )
				{
					if( u != '9')
						filename[filename.length()-5] = u+1; //increment
					else{
						filename[filename.length()-5] = '0'; //increment
						if( isdigit(d) )
							if( d != '9')
								filename[filename.length()-6] = d+1; //increment
							else
								filename[filename.length()-6] = '0';
								if ( isdigit(c))
									if ( c != '9')
										filename[filename.length()-7] = c+1; //increment
									else
										filename[filename.length()-7] = '0'; //this should never happen...

					}
				}
				else
					reading = FALSE;
			}
			else // COME FROM = IF WE READ MORE THAN MAX_LOADIMAGE
			{	
				reading = FALSE;
				MessageBox("Reached maximum number of frames!");
			}
		}
	}while(reading);
	--m_numFrames;
	return TRUE;

	/*
	//variables . init
	const char * ptr = filename.c_str();
	int index = m_numFrames;
	fstream file;
	char c,d;

	//RUN
	if(m_numFrames == MAX_LOADIMAGE)
	{
		MessageBox("Reached maximum number of frames!");
		return FALSE;
	}
	else
	{
		m_Image[index++].Load(ptr,CXIMAGE_FORMAT_TGA);
		AddFileToList(filename);
		m_numFrames++;
	

		c = filename[filename.length()-5]; //take the number
		d = filename[filename.length()-6]; //take the number
		if ( isdigit(c) && isdigit(d))
		{
			if( c != '9')
				filename[filename.length()-5] = c+1; //increment
			else{
				filename[filename.length()-5] = '0'; //increment
				filename[filename.length()-6] = d+1; //increment
			}
		}
		file.open( ptr, fstream::in | fstream::out | fstream::app ); //if exists

		while(!file.fail())
		{
			file.close();
			if(m_numFrames == MAX_LOADIMAGE)
			{
				MessageBox("Reached maximum number of frames!");
				return FALSE;
			}
			else
			{
				m_Image[index++].Load(ptr,CXIMAGE_FORMAT_TGA); //open it
				AddFileToList(filename);
				m_numFrames++;

				c = filename[filename.length()-5]; //take the number
				d = filename[filename.length()-6]; //take the number
				if( c != '9')	
					filename[filename.length()-5] = c+1; //look for next
				else{
					filename[filename.length()-5] = '0'; //increment
					filename[filename.length()-6] = d+1; //increment
				}
				file.open( ptr, fstream::in | fstream::out | fstream::binary ); //if exists
			}
		}
	
	}
	//clean & leave
	if (m_Image[--m_numFrames].IsValid())
		return TRUE;
	else
		return FALSE;
		*/
}


bool CMB_SubUVDlg::SaveTheBitmap( CxImage & m_Image, string & filename)
{
	//variables . init
	const char * ptr = filename.c_str();

	//RUN . clean & leave
	if (m_genImage.IsValid())
	{
		if (m_alphaImage==false)
		{
			int R,G,B;
			R= 0;
			G= 0;
			B= 0;
			RGBQUAD rgb={(BYTE)B,(BYTE)G,(BYTE)R,0};
			m_genImage.SetTransColor(rgb);
			m_genImage.AlphaStrip();
		}

		m_genImage.Save(ptr,CXIMAGE_FORMAT_TGA);
		CleanList();
		return TRUE;
	}
	else 
		return FALSE;
}



bool CMB_SubUVDlg::ProcessImage(int & columns, int & rows, int & size)
{
	//variables
	int sx = columns * size;
	int sy = rows * size;
	int ptr = 0;
	int ptrVec[MAX_LOADIMAGE]; // used for random

	CxImage::ImageOpType op;
	op = CxImage::OpDstCopy;

	//init
	m_genImage.Destroy();
	m_genImage.Create(sx,sy,32,CXIMAGE_FORMAT_TGA);
	m_genImage.AlphaCreate();
	m_genImage.AlphaInvert();	
/*
	for (int i=0; i< m_numFrames; i++)
		m_Image[i].Resample(size,size,2);
*/
	//RUN
	switch(m_OrderType)
	{
	case Normal:

/*	// RIGHT UP - LEFT DOWN*/
	// 1 2 3
	// 4 5 6
	// 7 8 9
	for (int i=rows-1; i>=0; i--)
		for (int j=0; j<columns; j++)
		{
			m_prevImage = m_Image[ptr++];
			m_prevImage.Resample(size,size,2);
			m_genImage.Mix(m_prevImage,op,(-1)*j*size,(-1)*i*size,TRUE);
		}
	break;
	case NormalInverted:
/*  // LEFT UP - DOWN RIGHT*/
	// 1 4 7
	// 2 5 8
	// 3 6 9
	for (int j=0; j<columns; j++)
		for (int i=rows-1; i>=0; i--)
		{
			m_prevImage = m_Image[ptr++];
			m_prevImage.Resample(size,size,2);
			m_genImage.Mix(m_prevImage,op,(-1)*j*size,(-1)*i*size,TRUE);
		}
	break;
	case Reverse:
/*	// LEFT DOWN - RIGHT UP*/
	// 7 8 9
	// 4 5 6
	// 1 2 3
	for (int j=0; j<rows; j++)
		for (int i=0; i<columns; i++)
		{
			m_prevImage = m_Image[ptr++];
			m_prevImage.Resample(size,size,2);
			m_genImage.Mix(m_prevImage,op,(-1)*i*size,(-1)*j*size,TRUE);
		}
	break;
	case ReverseInverted:
/*  // LEFT DOWN - UP RIGHT*/
	// 3 6 9
	// 2 5 8
	// 1 4 7
	for (int i=0; i<columns; i++)
		for (int j=0; j<rows; j++)
		{
			m_prevImage = m_Image[ptr++];
			m_prevImage.Resample(size,size,2);
			m_genImage.Mix(m_prevImage,op,(-1)*i*size,(-1)*j*size,TRUE);
		}
	break;
	case Random:
  // RANDOM
	// 3 1 5
	// 4 7 6
	// 8 2 9
	for (int i=0; i< m_numFrames; i++)
		ptrVec[i] = 0;

	bool placed = false;
	srand ( time(NULL) );
	for (int j=0; j<rows; j++)
		for (int i=0; i<columns; i++)
		{
			ptr = rand() % m_numFrames;
			placed = false;
			
			while(!placed)
			{
				if (! ptrVec[ptr] )
				{	
					m_prevImage = m_Image[ptr];
					m_prevImage.Resample(size,size,2);
					m_genImage.Mix(m_prevImage,op,(-1)*i*size,(-1)*j*size,TRUE);
					ptrVec[ptr] = 1;
					placed = true;
				}
				else
				{
					if(ptr < m_numFrames)
						ptr++;
					else
						ptr = 0;
				}
			}
		}
	}
/**/
	return TRUE;
}



///////////////////////////////////////////////////////////

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMB_SubUVDlg dialog




CMB_SubUVDlg::CMB_SubUVDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMB_SubUVDlg::IDD, pParent)
	, m_OrderSelect(FALSE)
	, m_alphaImage(TRUE)
	, m_fitPower2(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMB_SubUVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, fileFrames);
	DDX_Radio(pDX, IDC_RADIO1, m_OrderSelect);
	DDX_Control(pDX, IDC_COMBO1, m_numberOfRows);
	DDX_Check(pDX, IDC_CHECK1, m_alphaImage);
	DDX_Control(pDX, IDC_COMBO2, m_CBFrameSize);
	DDX_Check(pDX, IDC_CHECK2, m_fitPower2);
	DDX_Control(pDX, IDC_BUTTON6, m_savePower2);
	DDX_Control(pDX, IDC_COMBO3, m_numberOfColumns);
	DDX_Control(pDX, IDC_LIST2, m_fileSize);
	DDX_Control(pDX, IDC_EDIT1, m_EhighResample);
	DDX_Control(pDX, IDC_EDIT2, m_EwidthResample);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}

BEGIN_MESSAGE_MAP(CMB_SubUVDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK1, &CMB_SubUVDlg::OnBnClickedCheck1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMB_SubUVDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDOK, &CMB_SubUVDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMB_SubUVDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON4, &CMB_SubUVDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CMB_SubUVDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CMB_SubUVDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMB_SubUVDlg::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBOBOXEX1, &CMB_SubUVDlg::OnCbnSelchangeComboboxex1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMB_SubUVDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CMB_SubUVDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_CHECK2, &CMB_SubUVDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON5, &CMB_SubUVDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMB_SubUVDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_RADIO1, &CMB_SubUVDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMB_SubUVDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CMB_SubUVDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CMB_SubUVDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CMB_SubUVDlg::OnBnClickedRadio5)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CMB_SubUVDlg::OnCbnSelchangeCombo3)
	ON_EN_CHANGE(IDC_EDIT1, &CMB_SubUVDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CMB_SubUVDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CMB_SubUVDlg message handlers

BOOL CMB_SubUVDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	m_numFrames = 0;
	m_OrderSelect = 0;
	m_alphaImage = 1;
	m_fitPower2 = 0;
	BOOL m_framesPower2 = 1;
	m_savePower2.EnableWindow(TRUE);
	m_OrderType = Normal;


	m_numCols = 1;
	m_numRows = 1;
	m_sizeFrame = 1;


	m_fileDimensions = "File Dimensions: 0p x 0p";
	const char * str = m_fileDimensions.c_str();
	m_fileSize.AddString(str);
	
	
	string numOfRowsCalc = "1";
	const char * ptr = numOfRowsCalc.c_str();
	m_numberOfRows.AddString(ptr);
	m_numberOfRows.SelectString(1,ptr);
	m_numberOfColumns.AddString(ptr);
	m_numberOfColumns.SelectString(1,ptr);

	string singleImageSize = "32";
	ptr = singleImageSize.c_str();
	m_CBFrameSize.SelectString(1,ptr);

/*
   // load bmp logo
g_hBMP = (HBITMAP)LoadImage(NULL, "C:/Users/matheboy/Desktop/icon.bmp",
                               IMAGE_BITMAP,0,0,
                               LR_LOADFROMFILE |
                               LR_CREATEDIBSECTION);
*/
	CWnd* temp = theApp.m_pMainWnd;

	CDC* cdc = temp->GetDC();

	HDC hdc = cdc->GetSafeHdc();

	m_genImage.Destroy();
	m_genImage.Create(512,512,32,CXIMAGE_FORMAT_TGA);
	m_prevImage = m_genImage;
	m_prevImage.Resample(357,252,2);
	g_hBMP = m_prevImage.MakeBitmap(hdc);



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMB_SubUVDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMB_SubUVDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	
	CRect rctPicture;
	m_Picture.GetWindowRect(&rctPicture);
	ScreenToClient(&rctPicture);

	//we need the image g_hBMP
	/*
	CWnd* temp = theApp.m_pMainWnd;
	
    CDC* cdc = temp->GetDC();

	HDC hdc = cdc->GetSafeHdc();
	
	m_prevImage = m_genImage;

	m_prevImage.Resample(rctPicture.Width(), rctPicture.Height(),2);
	
	g_hBMP = m_prevImage.MakeBitmap(hdc);
	*/
	//we need the image g_hBMP
	

	CBitmap bmpPicture;
	CDC mdcPicture;
	CBitmap *bmpFromHandle = bmpPicture.FromHandle(g_hBMP);

	mdcPicture.CreateCompatibleDC(&dc);
	CBitmap * bmpPrevious = mdcPicture.SelectObject(bmpFromHandle);

	

	dc.BitBlt(rctPicture.left, rctPicture.top,
		  rctPicture.Width(), rctPicture.Height(),
		  &mdcPicture, 0, 0, SRCCOPY);

	dc.SelectObject(bmpPrevious);


}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMB_SubUVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMB_SubUVDlg::OnBnClickedCheck1()
{
	if(m_alphaImage == true)
		m_alphaImage = false;
	else
		m_alphaImage = true;
	// TODO: Add your control notification handler code here
}

void CMB_SubUVDlg::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
}

void CMB_SubUVDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}


void CMB_SubUVDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

//Load an image sequence
void CMB_SubUVDlg::OnBnClickedButton4()
{
	string filename;

	CWnd* temp = theApp.m_pMainWnd;
	//hwnd = temp.GetSafeHwnd();
	if ( OpenFileDialog(temp->GetSafeHwnd() ,filename))
		LoadTheBitmap(filename );


	RowsAdvice();
}

// Save the resulting image.
void CMB_SubUVDlg::OnBnClickedButton3()
{
	int aux,rows,columns=0;
	int size=0;

	char *ptr = new char(10);

	CWnd* temp = theApp.m_pMainWnd;
	string filename;
	
	UpdateFileRowColSize();
	
	if(m_numFrames > 2)
	{
		ProcessImage(m_numCols, m_numRows, m_sizeFrame);
		if( m_fitPower2 )
		{
			int high,width;
			char *ptr = new char(10);
			m_EhighResample.GetWindowTextA(ptr,10);
			high = atoi(ptr);
			width = high;
			m_genImage.Resample(high,width,2);
		}
		if ( SaveFileDialog( temp->GetSafeHwnd(), filename ) )
			SaveTheBitmap( m_Image[0], filename ); 
	}
	else
	{
		// show window and say: "hey! I need a file!"
	}
    CDC* cdc = temp->GetDC();

	HDC hdc = cdc->GetSafeHdc();

	m_genImage.Destroy();
	m_genImage.Create(512,512,32,CXIMAGE_FORMAT_TGA);
	m_prevImage = m_genImage;
	m_prevImage.Resample(357,252,2);
	g_hBMP = m_prevImage.MakeBitmap(hdc);
	Invalidate();

	m_fileDimensions = "File Dimensions: 0p x 0p";
	const char * str = m_fileDimensions.c_str();
	m_fileSize.DeleteString(0);
	m_fileSize.AddString(str);
	
	RowsAdvice();
}

// pick a single image and add it to the frames list.
void CMB_SubUVDlg::OnBnClickedButton1()
{
	string filename;
	int index=0;

	CWnd* temp = theApp.m_pMainWnd;
	//hwnd = temp.GetSafeHwnd();
	if ( OpenFileDialog(temp->GetSafeHwnd() ,filename))
	{
		const char * ptr = filename.c_str();
		m_Image[m_numFrames++].Load(ptr,CXIMAGE_FORMAT_TGA);
		AddFileToList(filename);
	}


	RowsAdvice();
	// TODO: Add your control notification handler code here
}

//clean the animation list.
void CMB_SubUVDlg::OnBnClickedButton2()
{
	CleanList();
	RowsAdvice();
	// TODO: Add your control notification handler code here
}

void CMB_SubUVDlg::OnCbnSelchangeComboboxex1()
{
	// TODO: Add your control notification handler code here
}

//change rows value
void CMB_SubUVDlg::OnCbnSelchangeCombo1()
{
	
	int rows,columns,aux,check;
	char *col = new char(10);
	char *colCheck = new char(10);
	//ROWS & COLUMNS
	rows = m_numberOfRows.GetCurSel();
	m_numberOfRows.GetLBText(rows,col);
	rows = atoi(col);
	columns = m_numFrames/rows;
	col = itoa(columns,col,10);
	m_numberOfColumns.SelectString(1,col);

	//have we selected the correct one?
	check = m_numberOfColumns.GetCurSel();
	m_numberOfColumns.GetLBText(check,colCheck);
	aux = atoi(colCheck);
	if( columns != aux)
		m_numberOfColumns.SelectString(check+1,col);

	
	//
	UpdateFileRowColSize();
	UpdateFileSizeMessage();
	
	// TODO: Add your control notification handler code here
}

void CMB_SubUVDlg::OnCbnSelchangeCombo2()
{
	UpdateFileRowColSize();
	UpdateFileSizeMessage();
	// TODO: Add your control notification handler code here
}

void CMB_SubUVDlg::OnBnClickedCheck2()
{
	
	if(m_fitPower2 == true)
	{
		//m_savePower2.EnableWindow(TRUE);
		m_fitPower2 = false;
	}
	else
	{
		//m_savePower2.EnableWindow(FALSE);
		m_fitPower2 = true;
	}
	
	//RowsAdvice();
	/**/
	// TODO: Add your control notification handler code here
}


// the preview button
void CMB_SubUVDlg::OnBnClickedButton5()
{
	HBITMAP m_bitmap = NULL;
	int aux,rows,columns,size;
	char *ptr = new char(10);

	DIBSECTION dibSection;
    

	CWnd* temp = theApp.m_pMainWnd;
	
    CDC* cdc = temp->GetDC();

	HDC hdc = cdc->GetSafeHdc();

	UpdateFileRowColSize();

	if(m_Image[1].IsValid())
		ProcessImage(m_numCols,m_numRows,m_sizeFrame);
	else
		m_genImage.Create(512,512,32,CXIMAGE_FORMAT_TGA);
	
	m_prevImage = m_genImage;
	m_prevImage.Resample(357,252,2);
	g_hBMP = m_prevImage.MakeBitmap(hdc);

	Invalidate(); //invalidate the picture rectangle and calls the onpaint() function.

/*
	GetObject(g_hBMP, sizeof(DIBSECTION), &dibSection);

	HDRAWDIB hdd = DrawDibOpen();
    DrawDibDraw(hdd, hdc, 337, 50, 335, 220, 
                &dibSection.dsBmih,
                dibSection.dsBm.bmBits, 0, 0, 
                dibSection.dsBmih.biWidth, 
                dibSection.dsBmih.biHeight, 0);
    DrawDibClose(hdd); 
	*/
	temp->ReleaseDC(cdc);//	ReleaseDC(hDlg, hdc);

}

void CMB_SubUVDlg::OnBnClickedButton6()
{
	if (m_genImage.GetHeight()>0)
	{
		if(m_genImage.GetHeight() > 0)
			m_genImage.Resample(25,25);
	}
	// TODO: Add your control notification handler code here
}

void CMB_SubUVDlg::OnBnClickedRadio1()
{
	m_OrderType = Normal;
	// TODO: Add your control notification handler code here
}

void CMB_SubUVDlg::OnBnClickedRadio2()
{
	m_OrderType = NormalInverted;
	// TODO: Add your control notification handler code here
}

void CMB_SubUVDlg::OnBnClickedRadio3()
{
	m_OrderType = Reverse;
	// TODO: Add your control notification handler code here
}

void CMB_SubUVDlg::OnBnClickedRadio4()
{
	m_OrderType = ReverseInverted;
	// TODO: Add your control notification handler code here
}

void CMB_SubUVDlg::OnBnClickedRadio5()
{
	m_OrderType = Random;
	// TODO: Add your control notification handler code here
}

//change columns value
void CMB_SubUVDlg::OnCbnSelchangeCombo3()
{
	int rows,columns,aux,check,columnsCheck;
	char *row = new char(10);
	char *rowCheck = new char(10);
	//ROWS & COLUMNS
	columns = m_numberOfColumns.GetCurSel();
	m_numberOfColumns.GetLBText(columns,row);
	columns = atoi(row);
	rows = m_numFrames/columns;
	row = itoa(rows,row,10);
	m_numberOfRows.SelectString(1,row);

//have we selected the correct one?
	check = m_numberOfRows.GetCurSel();
	m_numberOfRows.GetLBText(check,rowCheck);
	aux = atoi(rowCheck);
	if( rows != aux)
		m_numberOfRows.SelectString(check+1,row);
	// TODO: Add your control notification handler code here

	UpdateFileRowColSize();
	UpdateFileSizeMessage();
}

void CMB_SubUVDlg::OnEnChangeEdit1()
{
	int i=0;

	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CMB_SubUVDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
