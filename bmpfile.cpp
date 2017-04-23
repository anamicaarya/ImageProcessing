//	bmpops.cpp : implementation of the BMPFile class
//	
//	This handles the reading and writing of BMP files.
//
//

#include "stdafx.h"
#include "bmpfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BMPFile::BMPFile()
{
	m_errorText="OK";
}

////////////////////////////////////////////////////////////////////////////
//	load a .BMP file - 1,4,8,24 bit
//
//	allocates and returns an RGB buffer containing the image.
//	modifies width and height accordingly - NULL, 0, 0 on error

BYTE * BMPFile::LoadBMP(CString fileName, 
						UINT *width, 
						UINT *height)
{
	BITMAP inBM;

	BYTE m1,m2;
    long filesize;
    short res1,res2;
    long pixoff;
    long bmisize;                    
    long compression;
    unsigned long sizeimage;
    long xscale, yscale;
    long colors;
    long impcol;
    

	BYTE *outBuf=NULL;
	
	// for safety
	*width=0; *height=0;

	// init
	m_errorText="OK";
	m_bytesRead=0;

	FILE *fp;
	
	fp=fopen(fileName,"rb");
	if (fp==NULL) {
		CString msg;                    
		msg="Can't open file for reading :\n"+fileName;
		m_errorText=msg;
		return NULL;
	} else {
	    long rc;
		rc=fread((BYTE  *)&(m1),1,1,fp); m_bytesRead+=1;
		if (rc==-1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((BYTE  *)&(m2),1,1,fp); m_bytesRead+=1;
		if (rc==-1) m_errorText="Read Error!";
		if ((m1!='B') || (m2!='M')) {
			m_errorText="Not a valid BMP File";
			fclose(fp);
			return NULL;
        }
        
		////////////////////////////////////////////////////////////////////////////
		//
		//	read a ton of header stuff

		rc=fread((long  *)&(filesize),4,1,fp); m_bytesRead+=4;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((int  *)&(res1),2,1,fp); m_bytesRead+=2;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((int  *)&(res2),2,1,fp); m_bytesRead+=2;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((long  *)&(pixoff),4,1,fp); m_bytesRead+=4;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((long  *)&(bmisize),4,1,fp); m_bytesRead+=4;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((long  *)&(inBM.bmWidth),4,1,fp);	 m_bytesRead+=4;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((long  *)&(inBM.bmHeight),4,1,fp); m_bytesRead+=4;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((int  *)&(inBM.bmPlanes),2,1,fp); m_bytesRead+=2;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((int  *)&(inBM.bmBitsPixel),2,1,fp); m_bytesRead+=2;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((long  *)&(compression),4,1,fp); m_bytesRead+=4;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((long  *)&(sizeimage),4,1,fp); m_bytesRead+=4;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((long  *)&(xscale),4,1,fp); m_bytesRead+=4;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((long  *)&(yscale),4,1,fp); m_bytesRead+=4;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((long  *)&(colors),4,1,fp); m_bytesRead+=4;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		rc=fread((long  *)&(impcol),4,1,fp); m_bytesRead+=4;
		if (rc!=1) {m_errorText="Read Error!"; fclose(fp); return NULL;}

		////////////////////////////////////////////////////////////////////////////
		//	i don't do RLE files

		if (compression!=BI_RGB) {
	    	m_errorText="This is a compressed file.";
	    	fclose(fp);
	    	return NULL;
	    }

		if (colors == 0) {
			colors = 1 << inBM.bmBitsPixel;
		}


		////////////////////////////////////////////////////////////////////////////
		// read colormap

		RGBQUAD *colormap = NULL;

		switch (inBM.bmBitsPixel) {
		case 24:
			break;
			// read pallete 
		case 1:
		case 4:
		case 8:
			colormap = new RGBQUAD[colors];
			if (colormap==NULL) {
				fclose(fp);
				m_errorText="Out of memory";
				return NULL;
			}

			int i;
			for (i=0;i<colors;i++) {
				BYTE r,g,b, dummy;

				rc=fread((BYTE *)&(b),1,1,fp);
				m_bytesRead++;
				if (rc!=1) {
					m_errorText="Read Error!";	
					delete [] colormap;
					fclose(fp);
					return NULL;
				}

				rc=fread((BYTE  *)&(g),1,1,fp); 
				m_bytesRead++;
				if (rc!=1) {
					m_errorText="Read Error!";	
					delete [] colormap;
					fclose(fp);
					return NULL;
				}

				rc=fread((BYTE  *)&(r),1,1,fp); 
				m_bytesRead++;
				if (rc!=1) {
					m_errorText="Read Error!";	
					delete [] colormap;
					fclose(fp);
					return NULL;
				}


				rc=fread((BYTE  *)&(dummy),1,1,fp); 
				m_bytesRead++;
				if (rc!=1) {
					m_errorText="Read Error!";	
					delete [] colormap;
					fclose(fp);
					return NULL;
				}

				colormap[i].rgbRed=r;
				colormap[i].rgbGreen=g;
				colormap[i].rgbBlue=b;
			}
			break;
		}


		if ((long)m_bytesRead>pixoff) {
			fclose(fp);
			m_errorText="Corrupt palette";
			delete [] colormap;
			fclose(fp);
			return NULL;
		}

		while ((long)m_bytesRead<pixoff) {
			char dummy;
			fread(&dummy,1,1,fp);
			m_bytesRead++;
		}

		int w=inBM.bmWidth;
		int h=inBM.bmHeight;

		// set the output params
		*width=w;
		*height=h;

		long row_size = w * 3;

		long bufsize = (long)w * 3 * (long)h;

		////////////////////////////////////////////////////////////////////////////
		// alloc our buffer

		outBuf=(BYTE *) new BYTE [bufsize];
		if (outBuf==NULL) {
			m_errorText="Memory alloc Failed";
		} else {

			////////////////////////////////////////////////////////////////////////////
			//	read it

			long row=0;
			long rowOffset=0;

			// read rows in reverse order
			for (row=inBM.bmHeight-1;row>=0;row--) {

				// which row are we working on?
				rowOffset=(long unsigned)row*row_size;						      

				if (inBM.bmBitsPixel==24) {

					for (int col=0;col<w;col++) {
						long offset = col * 3;
						char pixel[3];

						if (fread((void  *)(pixel),1,3,fp)==3) {
							// we swap red and blue here
							*(outBuf + rowOffset + offset + 0)=pixel[2];		// r
							*(outBuf + rowOffset + offset + 1)=pixel[1];		// g
							*(outBuf + rowOffset + offset + 2)=pixel[0];		// b
						}

					}

					m_bytesRead+=row_size;
					
					// read DWORD padding
					while ((m_bytesRead-pixoff)&3) {
						char dummy;
						if (fread(&dummy,1,1,fp)!=1) {
							m_errorText="Read Error";
							delete [] outBuf;
							fclose(fp);
							return NULL;
						}

						m_bytesRead++;
					}
 
					
				} else {	// 1, 4, or 8 bit image

					////////////////////////////////////////////////////////////////
					// pixels are packed as 1 , 4 or 8 bit vals. need to unpack them

					int bit_count = 0;
					UINT mask = (1 << inBM.bmBitsPixel) - 1;

					BYTE inbyte=0;

					for (int col=0;col<w;col++) {
						
						int pix=0;

						// if we need another byte
						if (bit_count <= 0) {
							bit_count = 8;
							if (fread(&inbyte,1,1,fp)!=1) {
								m_errorText="Read Error";
								delete [] outBuf;
								delete [] colormap;
								fclose(fp);
								return NULL;
							}
							m_bytesRead++;
						}

						// keep track of where we are in the bytes
						bit_count -= inBM.bmBitsPixel;
						pix = ( inbyte >> bit_count) & mask;

						// lookup the color from the colormap - stuff it in our buffer
						// swap red and blue
						*(outBuf + rowOffset + col * 3 + 2) = colormap[pix].rgbBlue;
						*(outBuf + rowOffset + col * 3 + 1) = colormap[pix].rgbGreen;
						*(outBuf + rowOffset + col * 3 + 0) = colormap[pix].rgbRed;
					}

					// read DWORD padding
					while ((m_bytesRead-pixoff)&3) {
						char dummy;
						if (fread(&dummy,1,1,fp)!=1) {
							m_errorText="Read Error";
							delete [] outBuf;
							if (colormap)
								delete [] colormap;
							fclose(fp);
							return NULL;
						}
						m_bytesRead++;
					}
				}
			}
		
		}

		if (colormap) {
			delete [] colormap;
		}

		fclose(fp);

    }

	return outBuf;
}

