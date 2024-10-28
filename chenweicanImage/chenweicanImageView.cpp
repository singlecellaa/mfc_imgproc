
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

#include "CHistogramDisplay.h"
#include "SmoothDlg.h"

#include <stdlib.h>

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
	ON_COMMAND(ID_PROCESS_LINETRAN, &CchenweicanImageView::OnProcessLinetran)
	ON_COMMAND(ID_PROCESS_ZHIFANGTU, &CchenweicanImageView::OnProcessZhifangtu)
	ON_COMMAND(ID_IMAGE_TXPH, &CchenweicanImageView::OnImageTxph)
	ON_COMMAND(ID_IMAGE_MEDIAN, &CchenweicanImageView::OnImageMedian)
	ON_COMMAND(ID_ENHA_GRADSHARP, &CchenweicanImageView::OnEnhaGradsharp)
	ON_COMMAND(ID_ENHA_SHARP, &CchenweicanImageView::OnEnhaSharp)
	ON_COMMAND(ID_EDGE_SOBEL, &CchenweicanImageView::OnEdgeSobel)
	ON_COMMAND(ID_EDGE_PREWITT, &CchenweicanImageView::OnEdgePrewitt)
END_MESSAGE_MAP()

// CchenweicanImageView construction/destruction

CchenweicanImageView::CchenweicanImageView() noexcept
{
	// TODO: add construction code here
	m_dZoom = 1.0;
	m_bShow = FALSE;
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
	/*if (m_bShow)
		DrawGraph(pDC);*/
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

		int nX = int(0.5 + (scrPoint.x + point.x) / m_dZoom);
		int nY = int(0.5 + (scrPoint.y + point.y) / m_dZoom);
		int nR, nG, nB;
		if (nX >= 0 && nX < pDoc->imageWidth && nY >= 0 && nY < pDoc->imageHeight) {
			CString strPrompt;
			if (pDoc->m_nOpenMode == 1)  // BMP
			{
				if (pDoc->m_nColorBits == 8) // black-white
				{
					nB = *(pDoc->m_pBits + (pDoc->imageHeight-nY-1)*pDoc->imageWidth + nX);
					strPrompt.Format(CString("X: % 4d - Y : % 4d Gray : % 3d"), nX, nY, nB);
				}
				if (pDoc->m_nColorBits == 24) // colored
				{
					nB = *(pDoc->m_pBits + (pDoc->imageHeight-nY-1)*pDoc->imageWidth*3 + nX*3 + 0);
					nG = *(pDoc->m_pBits + (pDoc->imageHeight-nY-1)*pDoc->imageWidth*3 + nX*3 + 1);
					nR = *(pDoc->m_pBits + (pDoc->imageHeight-nY-1)*pDoc->imageWidth*3 + nX*3 + 2);
					strPrompt.Format(CString("X:%4d-Y:%4d R:%3d B:%3d"), nX, nY, nR, nG, nB);
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
					nB = *(pDoc->m_pBits + nY*pDoc->imageWidth + nX*3 + 0);
					nG = *(pDoc->m_pBits + nY*pDoc->imageWidth + nX*3 + 1);
					nR = *(pDoc->m_pBits + nY*pDoc->imageWidth + nX*3 + 2);
										strPrompt.Format(CString("X:%4d-Y:%4d R:%3d B:%3d"), nX, nY, nR, nG, nB);

				}
			}
			((CMainFrame*)AfxGetMainWnd())->SetStatusBarText(1, strPrompt);
		}
	}
	CScrollView::OnMouseMove(nFlags, point);
}

void CchenweicanImageView::OnProcessLinetran()
{
	// TODO: Add your command handler code here
	CchenweicanImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	unsigned char* pBits = pDoc->m_pBits;
	int nWidth = pDoc->imageWidth;
	int nHeight = pDoc->imageHeight;
	int nValue = 0;
	double dValue = 0.0;
	long lTotal = 0;
	long lTotalD = 0;
	long lTotalL = 0;
	int ab = 255, bb = 0, a = 0, b = 255;
	if (pBits == NULL) return;
	for (int i = 0; i < nHeight; i++)
	{
		lTotalL = nWidth * i;
		for (int j = 0; j < nWidth; j++)
		{
			lTotalD = lTotalL + j;
			nValue = *(pBits + lTotalD);
			dValue = ab + 1.0 * (bb - ab) / (b - a) * (nValue - a);
			pBits[lTotalD] = int(dValue);
		}
	}
	Invalidate();
}

void CchenweicanImageView::OnProcessZhifangtu()
{
	// TODO: Add your command handler code here
	m_bShow = TRUE;
	CchenweicanImageDoc* pDoc = GetDocument();// CWJ_ImageDoc* pDoc 在各人程序中不一样
	ASSERT_VALID(pDoc);
	unsigned char* pBits = pDoc->m_pBits;
	int nWidth = pDoc->imageWidth;
	int nHeight = pDoc->imageHeight;
	int nValue = 0;
	long lTotal = 0;
	long lTotalD = 0;
	long lTotalL = 0;
	dMax = 0;
	dMaxG = 0;
	dMaxB = 0;
	if (pBits == NULL) return;

	if (pDoc->m_nColorBits == 8)
	{
		for (int k = 0; k < 256; k++)
		{
			m_lValue[k] = 0;
			m_dValue[k] = 0.0;
		}
		for (int i = 0; i < nHeight; i++)
		{
			lTotalL = nWidth * i;
			for (int j = 0; j < nWidth; j++)
			{
				lTotalD = lTotalL + j;
				nValue = *(pBits + lTotalD);
				m_lValue[nValue] = m_lValue[nValue] + 1;
			}
		}
		lTotal = nWidth * nHeight;
		for (int k = 0; k < 256; k++)
		{
			m_dValue[k] = 1.0 * m_lValue[k] / lTotal;
			if (dMax < m_dValue[k]) dMax = m_dValue[k];
		}
	}
	long nValueG = 0;
	long nValueB = 0;
	if (pDoc->m_nColorBits == 24)
	{
		for (int k = 0; k < 256; k++)
		{
			m_lValue[k] = 0;
			m_dValue[k] = 0.0;
			m_lValueG[k] = 0;
			m_dValueG[k] = 0.0;
			m_lValueB[k] = 0;
			m_dValueB[k] = 0.0;
		}
		for (int i = 0; i < nHeight; i++)
		{
			lTotalL = nWidth * i;
			for (int j = 0; j < nWidth; j++)
			{
				lTotalD = (lTotalL + j) * 3;
				nValue = *(pBits + lTotalD);
				m_lValue[nValue] = m_lValue[nValue] + 1;
				nValueG = *(pBits + lTotalD + 1);
				m_lValueG[nValueG] = m_lValueG[nValueG] + 1;
				nValueB = *(pBits + lTotalD + 2);
				m_lValueB[nValueB] = m_lValueB[nValueB] + 1;
			}
		}
		lTotal = nWidth * nHeight;
		for (int k = 0; k < 256; k++)
		{
			m_dValue[k] = 1.0 * m_lValue[k] / lTotal;
			if (dMax < m_dValue[k]) dMax = m_dValue[k];
			m_dValueG[k] = 1.0 * m_lValueG[k] / lTotal;
			if (dMaxG < m_dValueG[k]) dMaxG = m_dValueG[k];
			m_dValueB[k] = 1.0 * m_lValueB[k] / lTotal;
			if (dMaxB < m_dValueB[k]) dMaxB = m_dValueB[k];
		}
	}
	Invalidate();

	CHistogramDisplay hisDlg;
	hisDlg.m_nColorBits = pDoc->m_nColorBits;
	hisDlg.dMax = dMax;
	hisDlg.dMaxG = dMaxG;
	hisDlg.dMaxB = dMaxB;

	memcpy(hisDlg.m_dValue, m_dValue, 256 * sizeof(double));
	memcpy(hisDlg.m_dValueG, m_dValueG, 256 * sizeof(double));
	memcpy(hisDlg.m_dValueB, m_dValueB, 256 * sizeof(double));
	hisDlg.DoModal();
}

void CchenweicanImageView::DrawGraph(CDC* pDC)
{
	CchenweicanImageDoc* pDoc = GetDocument(); // CCCJImageDoc* pDoc 在各人程序中不一样
	ASSERT_VALID(pDoc);
	unsigned char* pBits = pDoc->m_pBits;
	int X0 = 0 + 5;
	int Y0 = 510;
	int WX = 768 + 3, HY = 500;
	int H = 375;
	int W = 256;
	CPen cyPen(PS_SOLID, 2, RGB(255, 255, 0));
	CPen* oldPen = pDC->SelectObject(&cyPen);
	pDC->MoveTo(X0, Y0);
	pDC->LineTo(X0 + WX, Y0);
	pDC->MoveTo(X0, Y0);
	pDC->LineTo(X0, Y0 - HY);
	pDC->MoveTo(X0, Y0 - HY);
	pDC->LineTo(X0 + WX, Y0 - HY);
	pDC->MoveTo(X0 + WX, Y0);
	pDC->LineTo(X0 + WX, Y0 - HY);
	pDC->SelectObject(&oldPen);
	{
		CPen redPen(PS_SOLID, 1, RGB(255, 0, 0));
		oldPen = pDC->SelectObject(&redPen);
		for (int i = 0; i < 256; i++)
		{
			int x0, y0, x1, y1;
			double dy;
			x0 = X0 + i * 3;
			y0 = Y0;
			x1 = X0 + i * 3;
			dy = Y0 - 1.0 * H * m_dValue[i] / dMax;
			y1 = int(dy + 0.5);
			pDC->MoveTo(x0, y0);
			pDC->LineTo(x1, y1);
		}
		pDC->SelectObject(oldPen);
	}
	if (pDoc->m_nColorBits == 24)
	{
		{
			CPen grnPen(PS_SOLID, 1, RGB(0, 255, 0));
			oldPen = pDC->SelectObject(&grnPen);
			for (int i = 0; i < 256; i++)
			{
				int x0, y0, x1, y1;
				double dy;
				x0 = X0 + i * 3 + 1;
				y0 = Y0;
				x1 = X0 + i * 3 + 1;
				dy = Y0 - 1.0 * H * m_dValueG[i] / dMaxG;
				y1 = int(dy + 0.5);
				pDC->MoveTo(x0, y0);
				pDC->LineTo(x1, y1);
			}
			pDC->SelectObject(oldPen);
		}
		{
			CPen grnPen(PS_SOLID, 1, RGB(0, 0, 255));
			oldPen = pDC->SelectObject(&grnPen);
			for (int i = 0; i < 256; i++)
			{
				int x0, y0, x1, y1;
				double dy;
				x0 = X0 + i * 3 + 2;
				y0 = Y0;
				x1 = X0 + i * 3 + 2;
				dy = Y0 - 1.0 * H * m_dValueB[i] / dMaxB;
				y1 = int(dy + 0.5);
				pDC->MoveTo(x0, y0);
				pDC->LineTo(x1, y1);
			}
			pDC->SelectObject(oldPen);
		}
	}
}

void CchenweicanImageView::OnImageTxph()
{
	// TODO: Add your command handler code here
	SmoothDlg sdlg;
	if (sdlg.DoModal())
	{
		double H[3][3];//申明模板变量
		double K = sdlg.m_nSmooth10;//H 与 K组合为8-领域平均法
		H[0][0] = sdlg.m_nSmooth1;// 申明模板变量赋值
		H[0][1] = sdlg.m_nSmooth2;
		H[0][2] = sdlg.m_nSmooth3;
		H[1][0] = sdlg.m_nSmooth4;
		H[1][1] = sdlg.m_nSmooth5;
		H[1][2] = sdlg.m_nSmooth6;
		H[2][0] = sdlg.m_nSmooth7;
		H[2][1] = sdlg.m_nSmooth8;
		H[2][2] = sdlg.m_nSmooth9;
		CchenweicanImageDoc* pDoc = GetDocument(); //根据自己的命名进行相应更改
		ASSERT_VALID(pDoc);
		unsigned char* pBits = pDoc->m_pBits;
		if (!pBits)
			return;
		int nWidth = pDoc->imageWidth;
		int nHeight = pDoc->imageHeight;
		long lTotalR, lTotal;
		int m, n;
		double dValue;
		double dThres = 100;
		double Value[3][3];
		unsigned char* pOldBits = NULL;
		if (pDoc->m_nColorBits == 8)
		{
			pOldBits = new unsigned char[nWidth * nHeight];//分配内存
			CopyMemory(pOldBits, pBits, nWidth * nHeight);//把图像复制到pOldBit
			for (int i = 0; i < nHeight; i++)
			{
				lTotalR = i * nWidth;
				for (int j = 0; j < nWidth; j++)
				{
					lTotal = lTotalR + j;
					if (i == 0 || i == nHeight - 1 || j == 0 || j == nWidth - 1)//边缘点不作处理，直接拷贝数据
						pBits[lTotal] = pOldBits[lTotal];
					else//内部点作平滑处理，
					{
						Value[0][0] = pOldBits[lTotal - nWidth - 1];
						Value[0][1] = pOldBits[lTotal - nWidth - 0];
						Value[0][2] = pOldBits[lTotal - nWidth + 1];
						Value[1][0] = pOldBits[lTotal - 0 - 1];
						Value[1][1] = pOldBits[lTotal - 0 - 0];
						Value[1][2] = pOldBits[lTotal - 0 + 1];
						Value[2][0] = pOldBits[lTotal + nWidth - 1];
						Value[2][1] = pOldBits[lTotal + nWidth - 0];
						Value[2][2] = pOldBits[lTotal + nWidth + 1];
						dValue = 0.0;
						for (m = 0; m < 3; m++)
							for (n = 0; n < 3; n++)
								dValue = dValue + H[m][n] * Value[m][n];
						dValue = dValue * K;

						if (dValue < dThres)
							pBits[lTotal] = int(dValue * 0.5);
						else
							pBits[lTotal] = int(dValue);

						//pBits[lTotal] = int(dValue + 0.5);
					}
				}
			}
		}
		
		if (pDoc->m_nColorBits == 24)
		{
			pOldBits = new unsigned char[3 * nWidth * nHeight];//分配内存
			CopyMemory(pOldBits, pBits, 3 * nWidth * nHeight);//把图像复制到pOldBit
			for (int i = 0; i < nHeight; i++)
			{
				lTotalR = i * nWidth;
				for (int j = 0; j < nWidth; j++)
				{
					lTotal = (lTotalR + j) * 3;
					if (i == 0 || i == nHeight - 1 || j == 0 || j == nWidth - 1)//边缘点不作处理，直接拷贝数据
					{
						pBits[lTotal + 0] = pOldBits[lTotal + 0];
						pBits[lTotal + 1] = pOldBits[lTotal + 1];
						pBits[lTotal + 2] = pOldBits[lTotal + 2];
					}
					else//内部点作平滑处理，
					{
						Value[0][0] = pOldBits[lTotal - 3 * nWidth - 3 * 1 + 0];
						Value[0][1] = pOldBits[lTotal - 3 * nWidth - 3 * 0 + 0];
						Value[0][2] = pOldBits[lTotal - 3 * nWidth + 3 * 1 + 0];
						Value[1][0] = pOldBits[lTotal - 3 * 0 - 3 * 1 + 0];
						Value[1][1] = pOldBits[lTotal - 3 * 0 - 3 * 0 + 0];
						Value[1][2] = pOldBits[lTotal - 3 * 0 + 3 * 1 + 0];
						Value[2][0] = pOldBits[lTotal + 3 * nWidth - 3 * 1 + 0];
						Value[2][1] = pOldBits[lTotal + 3 * nWidth - 3 * 0 + 0];
						Value[2][2] = pOldBits[lTotal + 3 * nWidth + 3 * 1 + 0];
						dValue = 0.0;
						for (m = 0; m < 3; m++)
							for (n = 0; n < 3; n++)
								dValue = dValue + H[m][n] * Value[m][n];
						dValue = dValue * K;
						//pBits[lTotal + 0] = int(dValue + 0.5);
						if (dValue < dThres)
							pBits[lTotal + 0] = int(dValue * 0.5);
						else
							pBits[lTotal + 0] = int(dValue);

						Value[0][0] = pOldBits[lTotal - 3 * nWidth - 3 * 1 + 1];
						Value[0][1] = pOldBits[lTotal - 3 * nWidth - 3 * 0 + 1];
						Value[0][2] = pOldBits[lTotal - 3 * nWidth + 3 * 1 + 1];
						Value[1][0] = pOldBits[lTotal - 3 * 0 - 3 * 1 + 1];
						Value[1][1] = pOldBits[lTotal - 3 * 0 - 3 * 0 + 1];
						Value[1][2] = pOldBits[lTotal - 3 * 0 + 3 * 1 + 1];
						Value[2][0] = pOldBits[lTotal + 3 * nWidth - 3 * 1 + 1];
						Value[2][1] = pOldBits[lTotal + 3 * nWidth - 3 * 0 + 1];
						Value[2][2] = pOldBits[lTotal + 3 * nWidth + 3 * 1 + 1];
						dValue = 0.0;
						for (m = 0; m < 3; m++)
							for (n = 0; n < 3; n++)
								dValue = dValue + H[m][n] * Value[m][n];
						dValue = dValue * K;
						//pBits[lTotal + 1] = int(dValue + 0.5);
						if (dValue < dThres)
							pBits[lTotal + 1] = int(dValue * 0.5);
						else
							pBits[lTotal + 1] = int(dValue);

						Value[0][0] = pOldBits[lTotal - 3 * nWidth - 3 * 1 + 2];
						Value[0][1] = pOldBits[lTotal - 3 * nWidth - 3 * 0 + 2];
						Value[0][2] = pOldBits[lTotal - 3 * nWidth + 3 * 1 + 2];
						Value[1][0] = pOldBits[lTotal - 3 * 0 - 3 * 1 + 2];
						Value[1][1] = pOldBits[lTotal - 3 * 0 - 3 * 0 + 2];
						Value[1][2] = pOldBits[lTotal - 3 * 0 + 3 * 1 + 2];
						Value[2][0] = pOldBits[lTotal + 3 * nWidth - 3 * 1 + 2];
						Value[2][1] = pOldBits[lTotal + 3 * nWidth - 3 * 0 + 2];
						Value[2][2] = pOldBits[lTotal + 3 * nWidth + 3 * 1 + 2];
						dValue = 0.0;
						for (m = 0; m < 3; m++)
							for (n = 0; n < 3; n++)
								dValue = dValue + H[m][n] * Value[m][n];
						dValue = dValue * K;
						if (dValue < dThres)
							pBits[lTotal + 2] = int(dValue * 0.5);
						else
							pBits[lTotal + 2] = int(dValue);
					}
				}
			}
		}
		Invalidate();
		if (pOldBits != NULL)
			delete pOldBits; //删除临时分配内存
	}
}

int compare(const void* a, const void* b) {
	return (*(unsigned char*)a - *(unsigned char*)b);
}
void CchenweicanImageView::OnImageMedian()
{
	// TODO: Add your command handler code here
	CchenweicanImageDoc* pDoc = GetDocument(); //根据自己的命名进行相应更改
	ASSERT_VALID(pDoc);
	unsigned char* pBits = pDoc->m_pBits;
	if (!pBits)
		return;
	int nWidth = pDoc->imageWidth;
	int nHeight = pDoc->imageHeight;
	long lTotalR, lTotal;
	int m, n;
	double dValue;
	
	unsigned char* pOldBits = NULL;
	if (pDoc->m_nColorBits == 8)
	{
		pOldBits = new unsigned char[nWidth * nHeight];//分配内存
		CopyMemory(pOldBits, pBits, nWidth * nHeight);//把图像复制到pOldBit
		for (int i = 0; i < nHeight; i++)
		{
			lTotalR = i * nWidth;
			for (int j = 0; j < nWidth; j++)
			{
				lTotal = lTotalR + j;
				if (i == 0 || i == nHeight - 1 || j == 0 || j == nWidth - 1)//边缘点不作处理，直接拷贝数据
					pBits[lTotal] = pOldBits[lTotal];
				else//内部点作平滑处理，
				{
					unsigned char neighborhood[9] = {
						pOldBits[lTotal - nWidth - 1], 
						pOldBits[lTotal - nWidth + 0],     
						pOldBits[lTotal - nWidth + 1], 
						pOldBits[lTotal - 1],          
						pOldBits[lTotal + 0],              
						pOldBits[lTotal + 1],          
						pOldBits[lTotal + nWidth - 1], 
						pOldBits[lTotal + nWidth + 0],     
						pOldBits[lTotal + nWidth + 1]  
					};
					qsort(neighborhood, 9, sizeof(unsigned char), compare);
					dValue = neighborhood[4];

					pBits[lTotal] = int(dValue + 0.5);
				}
			}
		}
	}

	if (pDoc->m_nColorBits == 24)
	{
		pOldBits = new unsigned char[3 * nWidth * nHeight];//分配内存
		CopyMemory(pOldBits, pBits, 3 * nWidth * nHeight);//把图像复制到pOldBit
		for (int i = 0; i < nHeight; i++)
		{
			lTotalR = i * nWidth;
			for (int j = 0; j < nWidth; j++)
			{
				lTotal = (lTotalR + j) * 3;
				if (i == 0 || i == nHeight - 1 || j == 0 || j == nWidth - 1)//边缘点不作处理，直接拷贝数据
				{
					pBits[lTotal + 0] = pOldBits[lTotal + 0];
					pBits[lTotal + 1] = pOldBits[lTotal + 1];
					pBits[lTotal + 2] = pOldBits[lTotal + 2];
				}
				else//内部点作平滑处理，
				{
					unsigned char neighborhood1[9] = {
						pOldBits[lTotal - 3 * nWidth - 3 * 1 + 0],
						pOldBits[lTotal - 3 * nWidth - 3 * 0 + 0],
						pOldBits[lTotal - 3 * nWidth + 3 * 1 + 0],
						pOldBits[lTotal - 3 * 0 - 3 * 1 + 0],
						pOldBits[lTotal - 3 * 0 - 3 * 0 + 0],
						pOldBits[lTotal - 3 * 0 + 3 * 1 + 0],
						pOldBits[lTotal + 3 * nWidth - 3 * 1 + 0],
						pOldBits[lTotal + 3 * nWidth - 3 * 0 + 0],
						pOldBits[lTotal + 3 * nWidth + 3 * 1 + 0]
					};
					dValue = 0.0;
					qsort(neighborhood1, 9, sizeof(unsigned char), compare);
					dValue = neighborhood1[4];
					pBits[lTotal + 0] = int(dValue + 0.5);


					unsigned char neighborhood2[9] = {
						pOldBits[lTotal - 3 * nWidth - 3 * 1 + 1],
						pOldBits[lTotal - 3 * nWidth - 3 * 0 + 1],
						pOldBits[lTotal - 3 * nWidth + 3 * 1 + 1],
						pOldBits[lTotal - 3 * 0 - 3 * 1 + 1],
						pOldBits[lTotal - 3 * 0 - 3 * 0 + 1],
						pOldBits[lTotal - 3 * 0 + 3 * 1 + 1],
						pOldBits[lTotal + 3 * nWidth - 3 * 1 + 1],
						pOldBits[lTotal + 3 * nWidth - 3 * 0 + 1],
						pOldBits[lTotal + 3 * nWidth + 3 * 1 + 1],
					};
					dValue = 0.0;
					qsort(neighborhood2, 9, sizeof(unsigned char), compare);
					dValue = neighborhood2[4];
					pBits[lTotal + 1] = int(dValue + 0.5);

					unsigned char neighborhood3[9] = {
						pOldBits[lTotal - 3 * nWidth - 3 * 1 + 2],
						pOldBits[lTotal - 3 * nWidth - 3 * 0 + 2],
						pOldBits[lTotal - 3 * nWidth + 3 * 1 + 2],
						pOldBits[lTotal - 3 * 0 - 3 * 1 + 2],
						pOldBits[lTotal - 3 * 0 - 3 * 0 + 2],
						pOldBits[lTotal - 3 * 0 + 3 * 1 + 2],
						pOldBits[lTotal + 3 * nWidth - 3 * 1 + 2],
						pOldBits[lTotal + 3 * nWidth - 3 * 0 + 2],
						pOldBits[lTotal + 3 * nWidth + 3 * 1 + 2],
					};
					qsort(neighborhood3, 9, sizeof(unsigned char), compare);
					dValue = neighborhood3[4];
					pBits[lTotal + 2] = int(dValue + 0.5);
				}
			}
		}
	}
	Invalidate();
	if (pOldBits != NULL)
		delete pOldBits; //删除临时分配内存
}

BOOL WINAPI GradSharp(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, BYTE bThre)
{
	// 指向源图像的指针
	unsigned char* lpSrc;
	unsigned char* lpSrc1;
	unsigned char* lpSrc2;
	// 图像每行的字节数
	LONG lLineBytes;
	// 中间变量
	BYTE bTemp;
	// 计算图像每行的字节数
	lLineBytes = (((lWidth * 8) + 31) / 32 * 4); // 每行 字节
	for (LONG i = 0; i < lHeight-1; i++){ // 不处理下边缘
		// 每列
		for (LONG j = 0; j < lWidth-1; j++){ // 不处理右边缘
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			// 指向DIB第i+1行，第j个象素的指针
			lpSrc1 = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 2 - i) + j;
			// 指向DIB第i行，第j+1个象素的指针
			lpSrc2 = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j + 1;
			bTemp = abs((*lpSrc) - (*lpSrc1)) + abs((*lpSrc) - (*lpSrc2));
			// 判断是否小于阈值
			if (bTemp < 255){
				if (bTemp >= bThre) // 判断是否大于阈值，对于小于情况，灰度值不变。
					*lpSrc = bTemp;// 直接赋值为bTemp
			}
			else  
				*lpSrc = 255; // 直接赋值为255
		}
	}
	return TRUE;
}

void CchenweicanImageView::OnEnhaGradsharp()
{
	// TODO: Add your command handler code here
	CchenweicanImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	unsigned char* pBits = pDoc->m_pBits;
	int nWidth = pDoc->imageWidth;
	int nHeight = pDoc->imageHeight;
	int nColorBits = pDoc->m_nColorBits;
	if (nColorBits != 8)
	{
		// 提示用户
		MessageBox(_T("目前只支持256级灰度图像的锐化！"), _T("系统提示"),
			MB_ICONINFORMATION | MB_OK);
		// 返回
		return;
	}
	// 阈值
	BYTE bThre = 10;
	// 更改光标形状
	BeginWaitCursor();
	// 调用GradSharp()函数进行梯度板锐化
	if (::GradSharp(pBits, nWidth, nHeight, bThre))
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox(_T("分配内存失败！"), _T("系统提示"), MB_ICONINFORMATION | MB_OK);
	}
	// 恢复光标
	EndWaitCursor();
}

BOOL WINAPI Template(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight,int iTempH, int iTempW,int iTempMX, int iTempMY,FLOAT* fpArray, FLOAT fCoef)
{
	// 指向复制图像的指针
	unsigned char* lpNewDIBBits;
	// 指向源图像的指针
	unsigned char* lpSrc;
	// 指向要复制区域的指针
	unsigned char* lpDst;
	// 计算结果
	FLOAT fResult;
	// 图像每行的字节数
	LONG lLineBytes = (((lWidth * 8) + 31) / 32 * 4);
	// 暂时分配内存，以保存新图像
	lpNewDIBBits = new unsigned char[lLineBytes * lHeight];
	// 判断是否内存分配失败
	if (lpNewDIBBits == NULL)
		return FALSE; // 分配内存失败
	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);
	// 行(除去边缘几行)
	for (LONG i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++){
		// 列(除去边缘几列)
		for (LONG j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++){
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			fResult = 0;
			// 计算
			for (LONG k = 0; k < iTempH; k++){
				for (LONG l = 0; l < iTempW; l++){
					// 指向DIB第i - iTempMY + k行，第j - iTempMX + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k) + j - iTempMX + l;
					// 保存象素值
					fResult += (*lpSrc) * fpArray[k * iTempW + l];
				}
			}
			// 乘上系数
			fResult *= fCoef;
			// 取绝对值
			fResult = (FLOAT)fabs(fResult);
			// 判断是否超过255
			if (fResult > 255)
				*lpDst = 255; // 直接赋值为255
			else
				*lpDst = (unsigned char)(fResult + 0.5);
		}
	}
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	delete lpNewDIBBits;
	return TRUE;
}

void CchenweicanImageView::OnEnhaSharp()
{
	// TODO: Add your command handler code here
	CchenweicanImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	unsigned char* pBits = pDoc->m_pBits;
	int nWidth = pDoc->imageWidth;
	int nHeight = pDoc->imageHeight;
	int nColorBits = pDoc->m_nColorBits;
	if (nColorBits != 8)
	{
		// 提示用户
		MessageBox(_T("目前只支持256级灰度图像的锐化！"), _T("系统提示"),
			MB_ICONINFORMATION | MB_OK);
		// 返回
		return;
	}
	
	int iTempH; // 模板高度
	int iTempW; // 模板宽度
	// 模板系数
	FLOAT fTempC;
	int iTempMX; // 模板中心元素X坐标
	int iTempMY; // 模板中心元素Y坐标
	FLOAT aValue[9]; // 模板元素数组
	// 更改光标形状
	BeginWaitCursor();
	// 设置拉普拉斯模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aValue[0] = -1.0;
	aValue[1] = -1.0;
	aValue[2] = -1.0;
	aValue[3] = -1.0;
	aValue[4] = 9.0;
	aValue[5] = -1.0;
	aValue[6] = -1.0;
	aValue[7] = -1.0;
	aValue[8] = -1.0;
	// 调用Template()函数用拉普拉斯模板锐化DIB
	if (::Template(pBits, nWidth, nHeight,iTempH, iTempW, iTempMX, iTempMY, aValue, fTempC))
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox(_T("分配内存失败！"), _T("系统提示"), MB_ICONINFORMATION | MB_OK);
	}
	// 恢复光标
	EndWaitCursor();
}

BOOL WINAPI SobelDIB(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向缓存图像的指针
	unsigned char* lpDst1;
	unsigned char* lpDst2;
	lWidth = (((lWidth * 8) + 31) / 32 * 4);
	// 指向缓存DIB图像的指针
	unsigned char* lpNewDIBBits1;
	unsigned char* lpNewDIBBits2;
	int iTempH;
	int iTempW;
	FLOAT fTempC; //模版系数
	int iTempMX;
	int iTempMY;
	FLOAT aTemplate[9];  //模版数组
	lpNewDIBBits1 = new unsigned char[lWidth * lHeight]; // 暂时分配内存，以保存新图像
	if (lpNewDIBBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	lpNewDIBBits2 = new unsigned char[lWidth * lHeight]; // 暂时分配内存，以保存新图像
	if (lpNewDIBBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	// 拷贝源图像到缓存图像中
	lpDst1 = lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);
	// 设置Sobel模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = -1.0;
	aTemplate[1] = -2.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 2.0;
	aTemplate[8] = 1.0;
	// 调用Template()函数
	if (!Template(lpNewDIBBits1, lWidth, lHeight,iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}
	// 设置Sobel模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;
	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight,iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}
	//求两幅缓存图像的最大值
	for (long j = 0; j < lHeight; j++)
	{
		for (long i = 0; i < lWidth - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针
			lpDst1 = lpNewDIBBits1 + lWidth * j + i;
			// 指向缓存图像2倒数第j行，第i个象素的指针
			lpDst2 = lpNewDIBBits2 + lWidth * j + i;
			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		}
	}
	// 复制经过模板运算后的图像到源图像
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);
	delete lpNewDIBBits2;
	delete lpNewDIBBits1;
	// 返回
	return TRUE;
}

void CchenweicanImageView::OnEdgeSobel()
{
	// TODO: Add your command handler code here
	CchenweicanImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	unsigned char* pBits = pDoc->m_pBits;
	int nWidth = pDoc->imageWidth;
	int nHeight = pDoc->imageHeight;
	int nColorBits = pDoc->m_nColorBits;
	if (nColorBits != 8)
	{
		// 提示用户
		MessageBox(_T("目前只支持256级灰度图像的锐化！"), _T("系统提示"),MB_ICONINFORMATION | MB_OK);
		return;
	}
	//Sobel边缘检测运算
	// 调用SobelDIB()函数对DIB进行边缘检测
	if (SobelDIB(pBits, nWidth, nHeight))
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox(_T("分配内存失败！"), _T("系统提示"), MB_ICONINFORMATION | MB_OK);
	}
	// 恢复光标
	EndWaitCursor();
}

BOOL WINAPI PrewittDIB(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向缓存图像的指针
	unsigned char* lpDst1;
	unsigned char* lpDst2;

	unsigned char* lpNewDIBBits1;
	unsigned char* lpNewDIBBits2;
	int iTempH;
	int iTempW;
	// 模板系数
	FLOAT fTempC;
	int iTempMX;
	int iTempMY;
	FLOAT aTemplate[9]; //模板数组
	lpNewDIBBits1 = new unsigned char[lWidth * lHeight]; // 暂时分配内存，以保存新图像
	if (lpNewDIBBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	lpNewDIBBits2 = new unsigned char[lWidth * lHeight]; // 暂时分配内存，以保存新图像
	if (lpNewDIBBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	// 拷贝源图像到缓存图像中
	lpDst1 = lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);
	// 设置Prewitt模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 1.0;
	// 调用Template()函数
	if (!Template(lpNewDIBBits1, lWidth, lHeight,iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}
	// 设置Prewitt模板参数
	aTemplate[0] = 1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = -1.0;
	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight,iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}
	//求两幅缓存图像的最大值
	for (long j = 0; j < lHeight; j++)
	{
		for (long i = 0; i < lWidth - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针
			lpDst1 = lpNewDIBBits1 + lWidth * j + i;
			// 指向缓存图像2倒数第j行，第i个象素的指针
			lpDst2 = lpNewDIBBits2 + lWidth * j + i;
			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		}
	}
	// 复制经过模板运算后的图像到源图像
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);
	// 释放内存
	delete lpNewDIBBits2;
	delete lpNewDIBBits1;
	return TRUE;
}

void CchenweicanImageView::OnEdgePrewitt(){
	// TODO: Add your command handler code here
	CchenweicanImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	unsigned char* pBits = pDoc->m_pBits;
	int nWidth = pDoc->imageWidth;
	int nHeight = pDoc->imageHeight;
	int nColorBits = pDoc->m_nColorBits;
	if (nColorBits != 8)
	{
		MessageBox(_T("目前只支持256色位图的运算！"), _T("系统提示"), MB_ICONINFORMATION | MB_OK);
		return;
	}
	// 调用PrewittDIB()函数对DIB进行边缘检测
	if (PrewittDIB(pBits,nWidth, nHeight))
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox(_T("分配内存失败！"), _T("系统提示"), MB_ICONINFORMATION | MB_OK);
	}
	EndWaitCursor();
}
