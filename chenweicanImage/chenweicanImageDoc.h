
// chenweicanImageDoc.h : interface of the CchenweicanImageDoc class
//
#include <opencv2/opencv.hpp> //OpenCV 3.0ͷ�ļ�
#include <opencv2/core/core.hpp> //OpenCV 3.0ͷ�ļ�
#include <opencv2/imgproc/imgproc.hpp> //OpenCV 3.0ͷ�ļ�
#include <opencv2/highgui/highgui.hpp> //OpenCV 3.0ͷ�ļ�
#include <opencv2/imgcodecs.hpp> //OpenCV 3.0ͷ�ļ�

#pragma once


class CchenweicanImageDoc : public CDocument
{
protected: // create from serialization only
	CchenweicanImageDoc() noexcept;
	DECLARE_DYNCREATE(CchenweicanImageDoc)

// Attributes
public:
	IplImage* image;//OpenCVͼ��ָ��
// Operations
public:
public:
	void ReadRAW(LPCTSTR lpszPathName, int nWidth, int nHeight);
	BOOL ReadBMP(LPCTSTR lpszPathName);
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	BITMAPINFO* lpbmi;
	unsigned char* m_pBits;
	BITMAPFILEHEADER bmpFH;
	int imageWidth;
	int imageHeight;
	int m_nColorBits;//8 24
	int m_nOpenMode;//=0 RAW =1 BMP =2 OpenCV

	virtual ~CchenweicanImageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
