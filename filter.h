

#define UNDEFINED -1


// Image Class Definition 
//////////////////////////////////////////////////////////////////////
enum filtertype
{
	COLOUR,
	GRAYSCALE,			//   convert to grayscale by intensity
	INVERT,			//   convert negative of image
	BRIGHTNESS,			//   changing brightness of image
	EDGE_DETECTION
};

class MyImage  
{

//friend class MyFilter; //everything in filter can access private here

public:
	
	//default constructor
	MyImage();

	//inline functions
	void SetFilename(CString sFilename) { m_filename = sFilename; };
	
	bool OK()							{ return (m_buf != NULL); };
	UINT GetWidth()						{ return m_width;         };
	UINT GetHeight()                    { return m_height;		  };
	CString GetFilename()				{ return m_filename;	  };
    BYTE* GetBuffer()                   { return m_buf;           };
	// member functions

	//void LoadJPG();
	void LoadBMP();
	//void DrawBMP(CDC* pDC, BYTE * ColorBits);

    //destructor
	virtual ~MyImage();

private:
	// private member variables

	BYTE*   m_buf;
	BYTE*   m_undobuf;
	UINT    m_width;
	UINT    m_height;
	UINT    m_widthDW;
	CString m_filename;
};


