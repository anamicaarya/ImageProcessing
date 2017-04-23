// MyAppDlg.h : header file
//
#if !defined(AFX_MYAPPDLG_H__BDD41B61_98E4_44D3_9C82_679EC969AB46__INCLUDED_)
#define AFX_MYAPPDLG_H__BDD41B61_98E4_44D3_9C82_679EC969AB46__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxwin.h>
#include "filter.h"

/////////////////////////////////////////////////////////////////////////////
// CMyAppDlg dialog
class CMyAppDlg : public CDialog
{
// Construction
public:
 CMyAppDlg(CWnd* pParent = NULL); // standard constructor
// Dialog Data
 //{{AFX_DATA(CMyAppDlg)
	enum { IDD = IDD_MYAPP_DIALOG };
	int		m_Brightness;
	int		m_Contrast;
	//}}AFX_DATA
 // ClassWizard generated virtual function overrides
 //{{AFX_VIRTUAL(CMyAppDlg)
 protected:
 virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
 //}}AFX_VIRTUAL

// Implementation
	MyImage currentimage;				// current image displayed
	void BrightnessChange(int pos);
	void EdgeDetect_Vertical();
	void EdgeDetect_Diagonal();
	void EdgeDetect_Any(const char* Type_Of_Detect);
	void DrawBMP(BYTE * Buf,filtertype type,int position=0);
	void DrawBMPHistogram(BYTE ** Buf);
	BYTE * RGBtoHSV(BYTE * RGBColor);
	BYTE * Contrast(BYTE * RGBColor);
	void PlotHistogram(BYTE *buf);
	void StretchedHistogram(BYTE *buf,int Stretch_Percent=0);
	BYTE ** Calculate_Histogram( BYTE * Buf, int maxVal[], int minVal[] );
 protected:
 HICON m_hIcon;
 // Generated message map functions
 //{{AFX_MSG(CMyAppDlg)
 virtual BOOL OnInitDialog();
 afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
 afx_msg void OnPaint();
 afx_msg HCURSOR OnQueryDragIcon();
 afx_msg void OnOpen();
 afx_msg void OntoGray();
 afx_msg void OnInvert();
 afx_msg void OnHScroll(UINT nSBCode,UINT nPos,CScrollBar* pScrollBar);
 afx_msg void OnEdgeDetection();
 afx_msg void OnBrightness();
 afx_msg void OnHistogram();
 afx_msg void OnRGBtoHSV();
	//}}AFX_MSG
 DECLARE_MESSAGE_MAP()
private:
 //CString m_FileName; 
 //File to be read

};




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_MYAPPDLG_H__BDD41B61_98E4_44D3_9C82_679EC969AB46__INCLUDED_)
// MyAppDlg.cpp : implementation file
