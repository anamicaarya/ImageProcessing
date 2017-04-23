#include "stdafx.h"
#include "MyApp.h"
#include "MyAppDlg.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<icm.h>
#include<math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
BITMAPINFO *bitmapInfo;
HWND hWnd;
/////////////////////////////////////////////////////////////////////////////
#define	THRESHOLD_VAL 5  //for edge detection




/*-------STRUCTURES---------*/
typedef struct {int rows; int cols; unsigned char* data;} sImage;

/*-------PROTOTYPES---------*/
long getImageInfo(FILE*, long, int);
void copyImageInfo(FILE* inputFile, FILE* outputFile);
void copyColorTable(FILE* inputFile, FILE* outputFile, int nColors);


             
class CAboutDlg : public CDialog
{
public:
 CAboutDlg();
// Dialog Data
 //{{AFX_DATA(CAboutDlg)
 enum { IDD = IDD_ABOUTBOX };
 //}}AFX_DATA
 // ClassWizard generated virtual function overrides
 //{{AFX_VIRTUAL(CAboutDlg)
 protected:
 virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
 //}}AFX_VIRTUAL
// Implementation
protected:
 //{{AFX_MSG(CAboutDlg)
 //}}AFX_MSG
 DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
 //{{AFX_DATA_INIT(CAboutDlg)
 //}}AFX_DATA_INIT
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
 CDialog::DoDataExchange(pDX);
 //{{AFX_DATA_MAP(CAboutDlg)
 //}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
 //{{AFX_MSG_MAP(CAboutDlg)
  // No message handlers
 //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CMyAppDlg dialog
CMyAppDlg::CMyAppDlg(CWnd* pParent /*=NULL*/)
 : CDialog(CMyAppDlg::IDD, pParent)
{
 //{{AFX_DATA_INIT(CMyAppDlg)
	m_Brightness = 0;
	m_Contrast = 0;
	//}}AFX_DATA_INIT
 // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
 m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CMyAppDlg::DoDataExchange(CDataExchange* pDX)
{
 CDialog::DoDataExchange(pDX);
 //{{AFX_DATA_MAP(CMyAppDlg)
  //DDX_Check(pDX, IDC_BRIGHTNESS_TEXT, m_Brightness);
  //DDX_Check(pDX, IDC_CONTRAST_TEXT, m_Contrast);
  DDX_Text(pDX, IDC_BRIGHTNESS_TEXT, m_Brightness);
  DDX_Text(pDX, IDC_CONTRAST_TEXT, m_Contrast);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CMyAppDlg, CDialog)
 //{{AFX_MSG_MAP(CMyAppDlg)
 ON_WM_SYSCOMMAND()
 ON_WM_PAINT()
 ON_WM_QUERYDRAGICON()
 ON_BN_CLICKED(IDOK, OnOpen)
 ON_BN_CLICKED(OnChange_to_Grey, OntoGray)
 ON_BN_CLICKED(IDC_INVERT_IMAGE, OnInvert)
 ON_WM_HSCROLL()
 ON_WM_VSCROLL()
 ON_BN_CLICKED(IDC_EDGE_DETECTION, OnEdgeDetection)
 ON_BN_CLICKED(IDC_BRIGHTNESS, OnBrightness)
 ON_BN_CLICKED(IDC_HISTOGRAM, OnHistogram)
 ON_BN_CLICKED(IDC_RGBtoHSV, OnRGBtoHSV)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CMyAppDlg message handlers
BOOL CMyAppDlg::OnInitDialog()
{
 CDialog::OnInitDialog();
 // Add "About..." menu item to system menu.
 // IDM_ABOUTBOX must be in the system command range.
 ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
 ASSERT(IDM_ABOUTBOX < 0xF000);
 CMenu* pSysMenu = GetSystemMenu(FALSE);
 if (pSysMenu != NULL)
 {
  CString strAboutMenu;
  strAboutMenu.LoadString(IDS_ABOUTBOX);
  if (!strAboutMenu.IsEmpty())
  {
   pSysMenu->AppendMenu(MF_SEPARATOR);
   pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);

  }
 }

// Uses GetDlgItem to return a pointer to a user interface control(Brightness slider control ).

CWnd* dlgCtrl = GetDlgItem( IDC_BRIGHTNESS_SLIDER ); 

CSliderCtrl* pSlider = ( CSliderCtrl * ) dlgCtrl; 

pSlider->SetRange(-255,255,TRUE);

//
dlgCtrl = GetDlgItem( IDC_CONTRAST_SLIDER ); 

CSliderCtrl* contrastSlider = ( CSliderCtrl * ) dlgCtrl; 

contrastSlider->SetRange(-40,40,TRUE);

 // Set the icon for this dialog.  The framework does this automatically
 //  when the application's main window is not a dialog
 SetIcon(m_hIcon, TRUE);   // Set big icon
 SetIcon(m_hIcon, FALSE);  // Set small icon
 
 // TODO: Add extra initialization here
 
 return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyAppDlg::OnHScroll(UINT nSBCode, UINT nPos ,CScrollBar* pScrollBar) 
{
   // Get the minimum and maximum scroll-bar positions.
   int minpos;
   int maxpos;
   if (pScrollBar->GetDlgCtrlID() == IDC_BRIGHTNESS_SLIDER)
   {
	   pScrollBar->GetScrollRange(&minpos, &maxpos); 
	   maxpos = pScrollBar->GetScrollLimit();

		 // Get the current position of scroll box.
	   int curpos = pScrollBar->GetScrollPos();

	   // Determine the new position of scroll box.
	   switch (nSBCode)
	   {
	   case SB_LEFT:      // Scroll to far left.
		  curpos = minpos;
		  break;

	   case SB_RIGHT:      // Scroll to far right.
		  curpos = maxpos;
		  break;

	   case SB_ENDSCROLL:   // End scroll.
		  break;

	   case SB_LINELEFT:      // Scroll left.
		  if (curpos > minpos)
			 curpos--;
		  break;

	   case SB_LINERIGHT:   // Scroll right.
		  if (curpos < maxpos)
			 curpos++;
		  break;

	   case SB_PAGELEFT:    // Scroll one page left.
	   {
		  // Get the page size. 
		  SCROLLINFO   info;
		  pScrollBar->GetScrollInfo(&info, SIF_ALL);
   
		  if (curpos > minpos)
		  curpos = max(minpos, curpos - (int) info.nPage);
	   }
		  break;

	   case SB_PAGERIGHT:      // Scroll one page right.
	   {
		  // Get the page size. 
		  SCROLLINFO   info;
		  pScrollBar->GetScrollInfo(&info, SIF_ALL);

		  if (curpos < maxpos)
			 curpos = min(maxpos, curpos + (int) info.nPage);
	   }
		  break;

	   case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
          UpdateData(FALSE);
		  m_Brightness = nPos;
		  curpos = nPos;     
		  BrightnessChange(curpos);
		  break;

	   case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
          UpdateData(FALSE); // position that the scroll box has been dragged to.
		  m_Brightness = nPos; 
		  curpos = nPos;    
		  BrightnessChange(curpos);

		  break;
	   }

	   // Set the new position of the thumb (scroll box).
	   pScrollBar->SetScrollPos(curpos);

   //CMyAppDlg::OnHScroll(nSBCode, nPos, pScrollBar);
   }
   else if(pScrollBar->GetDlgCtrlID() == IDC_CONTRAST_SLIDER)
   {
	   m_Contrast = nPos;
	   pScrollBar->GetScrollRange(&minpos, &maxpos); 
	   maxpos = pScrollBar->GetScrollLimit();

		 // Get the current position of scroll box.
	   int curpos = pScrollBar->GetScrollPos();

	   // Determine the new position of scroll box.
	   switch (nSBCode)
	   {
	   case SB_LEFT:      // Scroll to far left.
		  curpos = minpos;
		  break;

	   case SB_RIGHT:      // Scroll to far right.
		  curpos = maxpos;
		  break;

	   case SB_ENDSCROLL:   // End scroll.
		  break;

	   case SB_LINELEFT:      // Scroll left.
		  if (curpos > minpos)
			 curpos--;
		  break;

	   case SB_LINERIGHT:   // Scroll right.
		  if (curpos < maxpos)
			 curpos++;
		  break;

	   case SB_PAGELEFT:    // Scroll one page left.
	   {
		  // Get the page size. 
		  SCROLLINFO   info;
		  pScrollBar->GetScrollInfo(&info, SIF_ALL);
   
		  if (curpos > minpos)
		  curpos = max(minpos, curpos - (int) info.nPage);
	   }
		  break;

	   case SB_PAGERIGHT:      // Scroll one page right.
	   {
		  // Get the page size. 
		  SCROLLINFO   info;
		  pScrollBar->GetScrollInfo(&info, SIF_ALL);

		  if (curpos < maxpos)
			 curpos = min(maxpos, curpos + (int) info.nPage);
	   }
		  break;

	   case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
							  // of the scroll box at the end of the drag operation.
          UpdateData(FALSE);
		  m_Contrast = nPos;
		  curpos = nPos;     
		  OnHistogram();
		  break;

	   case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
	                    	 // position that the scroll box has been dragged to.
          UpdateData(FALSE);
		  m_Contrast = nPos;
		   curpos = nPos;     
		  OnHistogram();

		  break;
	   }

	   // Set the new position of the thumb (scroll box).
	   pScrollBar->SetScrollPos(curpos);
   //CMyAppDlg::OnHScroll(nSBCode, nPos, pScrollBar);
   }

}


void CMyAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CMyAppDlg::OnPaint() 
{
 if (IsIconic())
 {
  CPaintDC dc(this); // device context for painting
  SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
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
}
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyAppDlg::OnQueryDragIcon()
{
 return (HCURSOR) m_hIcon;
}

void CMyAppDlg::OnOpen() 
{

        CString sFileName;  

		// File broswse dialog     
		static char BASED_CODE lpszccFilter[] = "BMP (*.bmp)|*.bmp|JPEG (*.jpg)|*.jpg|All files (*.*)|*.*||";
		TCHAR lpszExt[] = _T("bmp");
		CFileDialog FileDlg(TRUE, lpszExt,NULL,OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT,lpszccFilter,NULL );  
		int ret = FileDlg.DoModal();     
    
	
		if ( ret == IDOK) 
		{
		    sFileName=FileDlg.GetPathName();

			currentimage.SetFilename(sFileName);
			currentimage.LoadBMP();
			DrawBMP(currentimage.GetBuffer(),COLOUR);
	
		}

}

void CMyAppDlg::DrawBMP(BYTE *ColorBits ,filtertype type, int position)
{
	CDC *pDC = GetDC();
	if (ColorBits == NULL)		// if nothing in the buffer, get out of it
	return;

	if (pDC != NULL)
	{
		BYTE rgbColor[3];
		int rgbCol[3];
		COLORREF tempCol;
		int pos_V=350;

		for ( unsigned int row = 0; row < currentimage.GetHeight(); row++ ) 
		{

			for ( unsigned int col=0; col < currentimage.GetWidth(); col++ ) 
			{
				int pos_H=1000 + col;
			
				rgbColor[0]=ColorBits[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ];		// r
				rgbColor[1]=ColorBits[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 1 ];		// g
				rgbColor[2]=ColorBits[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ];		// b

				if (type == COLOUR)
				{

						tempCol = RGB( rgbColor[ 0 ], rgbColor[ 1 ], rgbColor[ 2 ] );
						pDC->SetPixel( col, pos_V, tempCol );
				}
				else if(type == GRAYSCALE)
				{
						BYTE nGray = ( BYTE )( 0.3 * rgbColor[0] + 0.59 * rgbColor[1] + 0.11 * rgbColor[2] );
						
						tempCol = RGB( nGray, nGray, nGray );
						pDC->SetPixel( pos_H, pos_V, tempCol );
						pos_H++;
				}
				else if (type == INVERT)
				{
						tempCol = RGB(255-rgbColor[ 0 ],255- rgbColor[ 1 ],255- rgbColor[ 2 ] );
						pDC->SetPixel( pos_H, pos_V, tempCol );
						pos_H++;

				}
				else if(type == BRIGHTNESS)
				{
					    
					    rgbCol[0] = ((int(rgbColor[ 0 ]) + position) > 255 ) ? 255 : (((int(rgbColor[ 0 ]) + position) < 0 ) ? 0 : ((int(rgbColor[ 0 ]) + position))) ;
					    rgbCol[1] = ((int(rgbColor[ 1 ]) + position) > 255 ) ? 255 : (((int(rgbColor[ 1 ]) + position) < 0 ) ? 0 : ((int(rgbColor[ 1 ]) + position))) ;
					    rgbCol[2] = ((int(rgbColor[ 2 ]) + position) > 255 ) ? 255 : (((int(rgbColor[ 2 ]) + position) < 0 ) ? 0 : ((int(rgbColor[ 2 ]) + position))) ;
					
						tempCol = RGB( rgbCol[0], rgbCol[1], rgbCol[2]);
						pDC->SetPixel( pos_H, pos_V, tempCol );
						pos_H++;

				}
				else if(type == EDGE_DETECTION)
				{
						tempCol = RGB( rgbColor[ 0 ], rgbColor[ 1 ], rgbColor[ 2 ] );
						pDC->SetPixel( pos_H, pos_V, tempCol );
						pos_H++;
				}

			}
			pos_V++;

		}
	
	}
}



void CMyAppDlg::OntoGray() 
{
	currentimage.LoadBMP();
	DrawBMP(currentimage.GetBuffer(),GRAYSCALE);
 
}


void CMyAppDlg::OnInvert()
{
	currentimage.LoadBMP();
	DrawBMP(currentimage.GetBuffer(),INVERT);
}


void CMyAppDlg::OnBrightness() 
{
	BrightnessChange(0);
}

void CMyAppDlg:: BrightnessChange(int position)
{
	currentimage.LoadBMP();
	DrawBMP(currentimage.GetBuffer(),BRIGHTNESS, position);

}

void CMyAppDlg::OnEdgeDetection() 
{
	EdgeDetect_Any("Diagonal");
}

void CMyAppDlg::EdgeDetect_Any(const char* Type_Of_Detect) 
{
	BYTE *buf;
	BYTE	nextRed ;
	BYTE	nextGreen;
	BYTE	nextBlue ;
	
	BYTE *tempBuf = new BYTE[currentimage.GetHeight() * currentimage.GetWidth() * 3];
	currentimage.LoadBMP();
	buf = currentimage.GetBuffer();
 for( unsigned int i = 0; i < ( currentimage.GetHeight() ); i++ )
 {
 
  for( unsigned int j = 0; j < ( currentimage.GetWidth() ); j++ )
  {
	//BOOL	edgeDetected = FALSE;
	BYTE	firstRed = 		buf[ ( i * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 0 ];
	BYTE	firstGreen = 	buf[ ( i * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 1 ];
	BYTE	firstBlue = 	buf[ ( i * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 2 ];

		if (strcmp("Horizontal",Type_Of_Detect) == 0 )
		{
			nextRed = 		buf[ ( i * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 3 ];
			nextGreen = 	buf[ ( i * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 4 ];
			nextBlue = 		buf[ ( i * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 5 ];
		}
		else if(strcmp("Vertical",Type_Of_Detect) == 0 )
		{
			nextRed = 		buf[ ( (i+1) * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 0 ];
			nextGreen = 	buf[ ( (i+1) * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 1 ];
			nextBlue = 		buf[ ( (i+1) * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 2 ];
		}
		else if(strcmp("Diagonal",Type_Of_Detect) == 0 )
		{
			nextRed = 		buf[ ( (i+1) * currentimage.GetWidth() * 3 ) + ( (j + 1) * 3 ) + 0 ];
			nextGreen = 	buf[ ( (i+1) * currentimage.GetWidth() * 3 ) + ( (j + 1) * 3 ) + 1 ];
			nextBlue = 		buf[ ( (i+1) * currentimage.GetWidth() * 3 ) + ( (j + 1) * 3 ) + 2 ];
		}

	if(		( nextRed - firstRed ) > THRESHOLD_VAL 
		||	( nextGreen - firstGreen ) > THRESHOLD_VAL 
		||	( nextBlue - firstBlue ) > THRESHOLD_VAL 
	)
	{
		tempBuf[ ( i * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 0 ] = 0;
		tempBuf[ ( i * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 1 ] = 0;
		tempBuf[ ( i * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 2 ] = 0;
	}
	else
	{
		tempBuf[ ( i * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 0 ] = 255;
		tempBuf[ ( i * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 1 ] = 255;
		tempBuf[ ( i * currentimage.GetWidth() * 3 ) + ( j * 3 ) + 2 ] = 255;
	}
  }
 }
	DrawBMP( tempBuf, EDGE_DETECTION );
	delete []tempBuf;

} 

void CMyAppDlg::PlotHistogram(BYTE *buf)  
{
 int MaxVal[3] ;
 int MinVal [3];
 BYTE ** rgb_Color = Calculate_Histogram( buf, MaxVal, MinVal );
 
 DrawBMPHistogram( rgb_Color );
 
 for (unsigned i =0 ;i<3 ;++i)
 {
  delete [] rgb_Color[i];
 }
 
}

void CMyAppDlg::StretchedHistogram(BYTE *buf,int Stretch_Percent)  
{
  BYTE rgbArray[3];
  unsigned int i;
  int color;
  int NewLeft[3] ;
  int NewRight[3];
  int oldRange[3];
  int StretchFactor[3];
  int NewRange[3];
  double ScaleFactor[3];
  int MaxVal[3];
  int MinVal[3];
  int newVal;
 // int NewRange = NewRight - NewLeft;
 
  BYTE **RGBColor = Calculate_Histogram( buf, MaxVal, MinVal );

  for (color = 0 ; color < 3 ; ++color )
  {
	  oldRange[color] = MaxVal[color] - MinVal[color];
	  StretchFactor[color]= (Stretch_Percent *  oldRange [color])/ 100;;
	  //NewLeft[color] = ((MinVal[color] - StretchFactor[color]) < 0 ? 0 :(MinVal[color] - StretchFactor[color]));
	  NewLeft[color] = (MinVal[color] - StretchFactor[color]);
	  NewRight[color] = ((MaxVal[color] + StretchFactor[color])> 255 ? 255 :(MaxVal[color] + StretchFactor[color]));
	  NewRange[color] = NewRight[color] - NewLeft[color];
	  ScaleFactor[color] = ( double )( (double)( NewRange[color] ) / ( double )( ( oldRange[color] == 0 ) ? 1 : oldRange[color] ) );
  }
  //DrawBMPHistogram( RGBColor );
 for (i =0 ;i<3 ;++i)
 {
  delete [] RGBColor[i];
 }
 
 
  for ( unsigned int row = 0; row < currentimage.GetHeight(); row++ )  {
    for (unsigned int col=0; col < currentimage.GetWidth(); col++ ) 
    {
  
  for( int i = 0; i < 3; ++i )
  {
   rgbArray[ i ] = (int)buf[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + i ];  // r g b
	  
     if (Stretch_Percent == 0)
      newVal = ( int )( rgbArray[ i ] * ScaleFactor[i]) ;
	 else if (Stretch_Percent > 0)
      newVal = ( int )( rgbArray[ i ] * ScaleFactor[i] - NewLeft[i] );
     else
      newVal = ( int )( rgbArray[ i ] * ScaleFactor[i] + NewLeft[i] );

   buf[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + i ] = ( newVal > 255 ) ? 255 :  newVal;
  }
  
    }
 }
   DrawBMP(buf,EDGE_DETECTION);
/*
   RGBColor = Calculate_Histogram(buf,MaxVal,MinVal);
 
   DrawBMPHistogram( RGBColor );

 for (i =0 ;i<3 ;++i)
 {
  delete [] RGBColor[i];
 }
*/
}
 
BYTE ** CMyAppDlg::Calculate_Histogram( BYTE * buf, int  MaxVal[3], int  MinVal[3] ) 
{  
	BYTE redColor;  
BYTE greenColor;  
BYTE blueColor;  
unsigned int i,row,col; 
// unsigned MaxVal = 0;  
//unsigned MinVal = 255;
 
 BYTE **rgb_Color = new BYTE *[ 3 ];
  for (i = 0 ; i < 3 ; ++i)
  {
   rgb_Color[ i ] = new BYTE[ 256 ];
   MaxVal[ i ] = 0;
   MinVal[ i ] = 255;
  }
 
  for (i = 0 ; i < 256 ; ++i)
  {
   rgb_Color[ 0 ][ i ] = 0;
   rgb_Color[ 1 ][ i ] = 0;
   rgb_Color[ 2 ][ i ] = 0;
  }
 
  for ( row = 0; row < currentimage.GetHeight(); row++ )
  {
     for ( col=0; col < currentimage.GetWidth(); col++ ) 
     {
   redColor = (int)buf[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ];  // r
   greenColor = (int)buf[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 1 ];  // g
   blueColor = (int)buf[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ];  // b
  /*
   int  maxOfThree = ( redColor > greenColor ) ? ( ( redColor > blueColor ) ? redColor : blueColor ) : ( ( greenColor > blueColor ) ?
greenColor : blueColor );
   int  minOfThree = ( redColor > greenColor ) ? ( ( redColor > blueColor ) ? blueColor : redColor ) : ( ( greenColor > blueColor ) ?
blueColor : greenColor );
 maxVal = maxOfThree > maxVal ? maxOfThree : maxVal ;  minVal = minOfThree < minVal ? minOfThree : minVal ;
   */
   		for(i = 0 ; i < 3 ; ++i)
		{
			MaxVal[ i ] = (int)buf[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + i ] > MaxVal[ i ] ? (int)buf[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + i ]  : MaxVal[ i ] ;
			MinVal[ i ]= (int)buf[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + i ]  < MinVal[ i ] ? (int)buf[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + i ]  : MinVal[ i ];
		}

   rgb_Color[ 0 ][ redColor ]++;
   rgb_Color[ 1 ][ greenColor ]++;
   rgb_Color[ 2 ][ blueColor ]++;
     }
  }
 
  return rgb_Color;
 
}



void CMyAppDlg::DrawBMPHistogram(BYTE **ColorBits )
{
	int i,j;
	CDC *pDC = GetDC();
	if (ColorBits == NULL)		// if nothing in the buffer, get out of it
	return;

	if (pDC != NULL)
	{
		BYTE rgbColor[3];
		COLORREF tempCol;
		int Pos_H = 450;
		//int Pos_V = 390;
		for(i = 0 ; i < 256 ; ++i)
		{
		    int Pos_V = 390;
			rgbColor[0] = ColorBits[0][i];
			rgbColor[1] = ColorBits[1][i];
			rgbColor[2] = ColorBits[2][i];
            tempCol = RGB(255,255,255);
            for(j=0 ; j< rgbColor[0] ;++j)
			{
			    Pos_V --;
				pDC->SetPixel(Pos_H,Pos_V,tempCol );
			}
			for(j=rgbColor[0] ; j< 300 ;++j)
			{
				pDC->SetPixel(Pos_H,Pos_V,RGB(0,0,0) );
				Pos_V--;
			}
	        
			
			Pos_H++;
		}

	}
}

BYTE * CMyAppDlg::RGBtoHSV(BYTE * RGBColor)
{

BYTE * HSVColor = new BYTE[currentimage.GetHeight() * currentimage.GetWidth() * 3];
BYTE rgbColor[3];
int delta;

	for ( unsigned int row = 0; row < currentimage.GetHeight(); row++ ) 
	{

		for ( unsigned int col=0; col < currentimage.GetWidth(); col++ ) 
		{
			rgbColor[0] = RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ];		// r
			rgbColor[1] = RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 1 ];		// g
			rgbColor[2] = RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ];		// b
            HSVColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ] =max((int)rgbColor[0],(max((int)rgbColor[1],(int)rgbColor[2])));// setting value of V in HSV
			delta = (int)max((int)rgbColor[0],(max((int)rgbColor[1],(int)rgbColor[2]))) - (int)min((int)rgbColor[0],(min((int)rgbColor[1],(int)rgbColor[2])));

//		    HSVColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 1 ] = delta / HSVColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ];
			if (HSVColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ] == 0 || delta == 0)
			{
				HSVColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ]= -1;
			}
	    	else if (rgbColor[0] == HSVColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ])
			{
				HSVColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ] = (60 * ((int)rgbColor[1] - (int)rgbColor[1]))/delta;
			}
	    	else if (rgbColor[1] == HSVColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ])
			{
				HSVColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ] = 120 + (60 * ((int)rgbColor[2] - (int)rgbColor[1]))/delta;
			}
	    	else if (rgbColor[2] == HSVColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ])
			{
				HSVColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ] = 240 + (60 * ((int)rgbColor[0] - (int)rgbColor[1]))/delta;
			}		
		}
	}


	return HSVColor;
}

void CMyAppDlg::OnRGBtoHSV() 
{
    //BYTE *tempHSV;
	currentimage.LoadBMP();
	//tempHSV = Contrast(currentimage.GetBuffer());
	//DrawBMP(tempHSV,BRIGHTNESS);
	StretchedHistogram(currentimage.GetBuffer());
//	delete []tempHSV;
}


void CMyAppDlg::OnHistogram() 
{
	//TODO: VC8 port, dummy variable, can cause grave errors
#pragma message("VC8 Port TODO lies here " )
	int position = 0;
	BOOL FLAG = TRUE;
	currentimage.LoadBMP();
    if (FLAG == TRUE)
		StretchedHistogram(currentimage.GetBuffer(),position);
	else
		StretchedHistogram(currentimage.GetBuffer());

	FLAG = FALSE ;
}


BYTE * CMyAppDlg::Contrast(BYTE * RGBColor)
{

BYTE * ContrastColor = new BYTE[currentimage.GetHeight() * currentimage.GetWidth() * 3];
int mymax=30;

	for ( unsigned int row = 0; row < currentimage.GetHeight(); row++ ) 
	{

		for ( unsigned int col=0; col < currentimage.GetWidth(); col++ ) 
		{
			
			if ((int)RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ] >= 128)
			{
				ContrastColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ] = (RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ] + mymax) > 255 ? 255 :(RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ] + mymax) ;
			}
			else
			{
				ContrastColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ] = (RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ] - mymax) < 0 ? 0 :(RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 0 ] + mymax) ;
			}


			if ((int)RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 1 ] >= 128)
			{
				ContrastColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 1 ] = (RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 1 ] + mymax) > 255 ? 255 :(RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 1 ] + mymax) ;
			}
			else
			{
				ContrastColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 1 ] = (RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 1 ] - mymax) < 0 ? 0 :(RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 1 ] + mymax) ;
			}
			


			if ((int)RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ] >= 128)
			{
				ContrastColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ] = (RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ] + mymax) > 255 ? 255 :(RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ] + mymax) ;
			}
			else
			{
				ContrastColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ] = (RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ] - mymax) < 0 ? 0 :(RGBColor[ ( row * ( currentimage.GetWidth() * 3 ) ) + ( col * 3 ) + 2 ] + mymax) ;
			}
			
		}
	}


	return ContrastColor;
}
