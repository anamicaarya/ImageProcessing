

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#define BMP_HEADERSIZE (3 * 2 + 4 * 12)
#define BMP_BYTESPERLINE (width, bits) ((((width) * (bits) + 31) / 32) * 4)
#define BMP_PIXELSIZE(width, height, bits) (((((width) * (bits) + 31) / 32) * 4) * height)


class BMPFile
{
public:
	// parameters
	CString m_errorText;
	DWORD m_bytesRead;

public:

	// operations

	BMPFile();

	BYTE * LoadBMP(CString fileName, UINT *width, UINT *height);


};