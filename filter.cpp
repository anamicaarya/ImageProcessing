#include "stdafx.h"
#include <math.h>
#include "filter.h"
#include "bmpfile.h"


// MYIMAGE Construction/Destruction

MyImage::MyImage()
{
	m_buf     = NULL;
	m_width   = 0;
	m_height  = 0;
	m_widthDW = 0;
	m_filename.Empty();
}

MyImage::~MyImage()
{
	if (m_buf != NULL)
	{
		delete[] m_buf;
		m_buf = NULL;
	}
}

void MyImage::LoadBMP()
//
// Desc:   Loads a BMP image from a file into the buffer
// Pre:    m_filename is the name of the file containting the BMP image
// Post:   m_buf contains the file; m_width and m_height are set;
//         m_undobuf has been allocated enough space to hold a copy of an
//         image the size of m_buf.
// Source: [SMALL00]
//
{
	if (m_buf != NULL)
	{
		delete[] m_buf;
		m_buf = NULL;
	}

	BMPFile theBmpFile;
	if (m_filename.IsEmpty())
	{
		int message = AfxMessageBox("Please Open the file", MB_YESNO|MB_ICONSTOP);
		if (message == IDYES)
		{
		// File broswse dialog     
		static char BASED_CODE lpszccFilter[] = "BMP (*.bmp)|*.bmp|JPEG (*.jpg)|*.jpg|All files (*.*)|*.*||";
		TCHAR lpszExt[] = _T("bmp");
		CFileDialog FileDlg(TRUE, lpszExt,NULL,OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT,lpszccFilter,NULL );  
		int ret = FileDlg.DoModal();  
		m_filename = FileDlg.GetFileName();
		m_buf = theBmpFile.LoadBMP(m_filename, &m_width, &m_height);

		}
	}
	else
	{
		m_buf = theBmpFile.LoadBMP(m_filename, &m_width, &m_height);
	}
	if ((m_buf == NULL) || (theBmpFile.m_errorText != "OK"))
	{
		AfxMessageBox(theBmpFile.m_errorText, MB_ICONSTOP);
		m_buf = NULL;
		return;
	}

}

