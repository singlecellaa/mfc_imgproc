
// chenweicanImageDoc.cpp : implementation of the CchenweicanImageDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "chenweicanImage.h"
#endif

#include "chenweicanImageDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CchenweicanImageDoc

IMPLEMENT_DYNCREATE(CchenweicanImageDoc, CDocument)

BEGIN_MESSAGE_MAP(CchenweicanImageDoc, CDocument)
END_MESSAGE_MAP()


// CchenweicanImageDoc construction/destruction

CchenweicanImageDoc::CchenweicanImageDoc() noexcept
{
	// TODO: add one-time construction code here
	m_pBits = NULL;
	lpbmi = NULL;
	imageWidth = 768;
	imageHeight = 576;
	m_nOpenMode = 0;
	m_nColorBits = 8;

	image = NULL;
}

CchenweicanImageDoc::~CchenweicanImageDoc()
{
}

BOOL CchenweicanImageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CchenweicanImageDoc serialization

void CchenweicanImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CchenweicanImageDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CchenweicanImageDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CchenweicanImageDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CchenweicanImageDoc diagnostics

#ifdef _DEBUG
void CchenweicanImageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CchenweicanImageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CchenweicanImageDoc commands


BOOL CchenweicanImageDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	CString str = lpszPathName;
	str.MakeLower();
	if (str.Find(_TEXT(".bmp")) != -1)
	{
		m_nOpenMode = 1;
		if (!ReadBMP(lpszPathName)) return FALSE;
	}
	if (str.Find(_TEXT(".jpg")) != -1)
	{
		m_nOpenMode = 2;
		image = cvLoadImage((CStringA)lpszPathName);
		lpbmi = (LPBITMAPINFO)new char[sizeof(BITMAPINFO) + 4 * (1 << 8)];
		lpbmi->bmiHeader.biBitCount = image->depth * image->nChannels;
		lpbmi->bmiHeader.biClrUsed = 0;
		lpbmi->bmiHeader.biHeight = image->height;
		lpbmi->bmiHeader.biWidth = image->width;
		lpbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbmi->bmiHeader.biSizeImage = image->width * image -> height * image->nChannels;
		lpbmi->bmiHeader.biClrImportant = 0;
		lpbmi->bmiHeader.biCompression = 0;
		lpbmi->bmiHeader.biPlanes = 1;//image->nChannels;
		imageWidth = image->width;
		imageHeight = image->height;
		for (int i = 0; i < (1 << 8); i++)
		{
			RGBQUAD& o = lpbmi->bmiColors[i];
			o.rgbBlue = i;
			o.rgbGreen = i;
			o.rgbRed = i;
			o.rgbReserved = 0;
		}
	}

	return TRUE;
}

BOOL CchenweicanImageDoc::ReadBMP(LPCTSTR lpszPathName)
{
	long lTotal = 0;
	CFile file;
	file.Open(lpszPathName, CFile::modeRead);
	file.Read(&bmpFH, sizeof(BITMAPFILEHEADER));
	lpbmi = (LPBITMAPINFO)new char[sizeof(BITMAPINFO) + 4 * (1 << 8)];
	file.Read(lpbmi, sizeof(BITMAPINFOHEADER));
	m_nColorBits = lpbmi->bmiHeader.biBitCount;
	imageHeight = lpbmi->bmiHeader.biHeight;
	imageWidth = lpbmi->bmiHeader.biWidth;
	if (m_nColorBits == 8)
	{
		lTotal = imageWidth * imageHeight;
		file.Read(&(lpbmi->bmiColors[0]), 256 * 4);
	}
	else if (m_nColorBits == 24)
	{
		lTotal = imageWidth * imageHeight * 3;
	}
	else
	{
		file.Close();
		return FALSE;
	}
	m_pBits = new unsigned char[lTotal];
	file.Read(m_pBits, lTotal);
	file.Close();
	return TRUE;
}