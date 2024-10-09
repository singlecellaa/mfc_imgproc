
// chenweicanImageView.cpp : implementation of the CchenweicanImageView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "chenweicanImage.h"
#endif

#include "chenweicanImageDoc.h"
#include "chenweicanImageView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CchenweicanImageView

IMPLEMENT_DYNCREATE(CchenweicanImageView, CScrollView)

BEGIN_MESSAGE_MAP(CchenweicanImageView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CchenweicanImageView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(AFX_ID_PREVIEW_ZOOMIN, &CchenweicanImageView::OnViewZoomIn)
	ON_COMMAND(AFX_ID_PREVIEW_ZOOMOUT, &CchenweicanImageView::OnViewZoomOut)
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_ZOOMIN, &CchenweicanImageView::OnUpdateViewZoomIn)
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_ZOOMOUT, &CchenweicanImageView::OnUpdateViewZoomOut)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CchenweicanImageView construction/destruction

CchenweicanImageView::CchenweicanImageView() noexcept
{
	// TODO: add construction code here
	m_dZoom = 1.0;
}

CchenweicanImageView::~CchenweicanImageView()
{
}

BOOL CchenweicanImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CchenweicanImageView drawing

void CchenweicanImageView::OnDraw(CDC* pDC)
{
	CchenweicanImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CSize sizeTotal;
	sizeTotal.cx = int(m_dZoom * pDoc->imageWidth + 0.5);
	sizeTotal.cy = int(m_dZoom * pDoc->imageHeight + 0.5);
	SetScrollSizes(MM_TEXT, sizeTotal);

	// TODO: add draw code for native data here
	if (pDoc->m_nOpenMode == 1)//BMP
	{
		StretchDIBits(
			pDC->m_hDC,
			0, 0,
			int(m_dZoom * pDoc->imageWidth + 0.5), int(m_dZoom * pDoc->imageHeight + 0.5),
			0, 0, 
			pDoc->imageWidth, pDoc->imageHeight,
			pDoc->m_pBits, pDoc->lpbmi, DIB_RGB_COLORS, SRCCOPY);
	}
	if (pDoc->m_nOpenMode == 2)//JPG
	{
		StretchDIBits(
			pDC->m_hDC,
			0, 0, 
			int(m_dZoom * pDoc->image->width + 0.5), int(m_dZoom * pDoc->image->height + 0.5),
			0,                   pDoc->image->height, 
			pDoc->image->width, -pDoc->image->height, 
			pDoc->image->imageData, 
			pDoc->lpbmi, 
			DIB_RGB_COLORS, 
			SRCCOPY
		);
	}
}

void CchenweicanImageView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CchenweicanImageView printing


void CchenweicanImageView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CchenweicanImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CchenweicanImageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CchenweicanImageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CchenweicanImageView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CchenweicanImageView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CchenweicanImageView diagnostics

#ifdef _DEBUG
void CchenweicanImageView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CchenweicanImageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CchenweicanImageDoc* CchenweicanImageView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CchenweicanImageDoc)));
	return (CchenweicanImageDoc*)m_pDocument;
}
#endif //_DEBUG


// CchenweicanImageView message handlers


void CchenweicanImageView::OnViewZoomIn()
{
	// TODO: Add your command handler code here
	m_dZoom = m_dZoom * 1.1;
	Invalidate();

}

void CchenweicanImageView::OnViewZoomOut()
{
	// TODO: Add your command handler code here
	m_dZoom = m_dZoom / 1.1;
	Invalidate();
}

void CchenweicanImageView::OnUpdateViewZoomIn(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE); // Enable the zoom in menu item
}

void CchenweicanImageView::OnUpdateViewZoomOut(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE); // Enable the zoom out menu item
}

BOOL CchenweicanImageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta > 0) {
		OnViewZoomIn();
	}
	else if (zDelta < 0) {
		OnViewZoomOut();
	}
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CchenweicanImageView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CchenweicanImageDoc* pDoc = GetDocument();

	if (pDoc->m_pBits != NULL || pDoc->image != NULL) {
		CPoint scrPoint = GetScrollPosition();

		int nX = int(0.5 + (scrPoint.x * point.x) / m_dZoom);
		int nY = int(0.5 + (scrPoint.y * point.y) / m_dZoom);
		int nR, nG, nB;
		if (nX >= 0 && nX < pDoc->imageWidth && nY >= 0 && nY < pDoc->imageHeight) {
			CString strPrompt;
			if (pDoc->m_nOpenMode == 1)  // BMP
			{
				if (pDoc->m_nColorBits == 8) // black-white
				{
					nB =*(pDoc->m_pBits+ (pDoc->imageHeight - nY - 1)*pDoc->imageWidth + nX);
					strPrompt.Format(CString("X: % 4d - Y : % 4d Gray : % 3d"), nX, nY, nB);
				}
				if (pDoc->m_nColorBits == 24) // colored
				{
					nB =*(pDoc->m_pBits + (pDoc->imageHeight-nY-1)*pDoc->imageWidth*3 + nX * 3 + 0);
					nG =*(pDoc->m_pBits + (pDoc->imageHeight-nY-1)*pDoc->imageWidth*3 + nX * 3 + 1);
					nR =*(pDoc->m_pBits + (pDoc->imageHeight-nY-1)*pDoc->imageWidth*3 + nX * 3 + 2);

				}
			}
			if (pDoc->m_nOpenMode == 2)  // JPG
			{
				pDoc->m_pBits = (unsigned char*)pDoc->image->imageData;
				if (pDoc->image->nChannels == 1) // black-white
				{
					nB = *(pDoc->m_pBits + nY * pDoc->imageWidth + nX);
					strPrompt.Format(CString("X: % 4d - Y : % 4d Gray : % 3d"), nX, nY, nB);
				}
				if (pDoc->image->nChannels == 3) // colored
				{
					nB = *(pDoc->m_pBits + nY * pDoc->imageWidth + nX*3 + 0);
					nG = *(pDoc->m_pBits + nY * pDoc->imageWidth + nX*3 + 1);
					nR = *(pDoc->m_pBits + nY * pDoc->imageWidth + nX*3 + 2);
				}
			}
			((CMainFrame*)AfxGetMainWnd())->SetStatusBarText(1, strPrompt);
		}
	}
	CScrollView::OnMouseMove(nFlags, point);
}
