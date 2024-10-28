
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
	CchenweicanImageDoc* pDoc = GetDocument();// CWJ_ImageDoc* pDoc �ڸ��˳����в�һ��
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
	CchenweicanImageDoc* pDoc = GetDocument(); // CCCJImageDoc* pDoc �ڸ��˳����в�һ��
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
		double H[3][3];//����ģ�����
		double K = sdlg.m_nSmooth10;//H �� K���Ϊ8-����ƽ����
		H[0][0] = sdlg.m_nSmooth1;// ����ģ�������ֵ
		H[0][1] = sdlg.m_nSmooth2;
		H[0][2] = sdlg.m_nSmooth3;
		H[1][0] = sdlg.m_nSmooth4;
		H[1][1] = sdlg.m_nSmooth5;
		H[1][2] = sdlg.m_nSmooth6;
		H[2][0] = sdlg.m_nSmooth7;
		H[2][1] = sdlg.m_nSmooth8;
		H[2][2] = sdlg.m_nSmooth9;
		CchenweicanImageDoc* pDoc = GetDocument(); //�����Լ�������������Ӧ����
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
			pOldBits = new unsigned char[nWidth * nHeight];//�����ڴ�
			CopyMemory(pOldBits, pBits, nWidth * nHeight);//��ͼ���Ƶ�pOldBit
			for (int i = 0; i < nHeight; i++)
			{
				lTotalR = i * nWidth;
				for (int j = 0; j < nWidth; j++)
				{
					lTotal = lTotalR + j;
					if (i == 0 || i == nHeight - 1 || j == 0 || j == nWidth - 1)//��Ե�㲻������ֱ�ӿ�������
						pBits[lTotal] = pOldBits[lTotal];
					else//�ڲ�����ƽ������
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
			pOldBits = new unsigned char[3 * nWidth * nHeight];//�����ڴ�
			CopyMemory(pOldBits, pBits, 3 * nWidth * nHeight);//��ͼ���Ƶ�pOldBit
			for (int i = 0; i < nHeight; i++)
			{
				lTotalR = i * nWidth;
				for (int j = 0; j < nWidth; j++)
				{
					lTotal = (lTotalR + j) * 3;
					if (i == 0 || i == nHeight - 1 || j == 0 || j == nWidth - 1)//��Ե�㲻������ֱ�ӿ�������
					{
						pBits[lTotal + 0] = pOldBits[lTotal + 0];
						pBits[lTotal + 1] = pOldBits[lTotal + 1];
						pBits[lTotal + 2] = pOldBits[lTotal + 2];
					}
					else//�ڲ�����ƽ������
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
			delete pOldBits; //ɾ����ʱ�����ڴ�
	}
}

int compare(const void* a, const void* b) {
	return (*(unsigned char*)a - *(unsigned char*)b);
}
void CchenweicanImageView::OnImageMedian()
{
	// TODO: Add your command handler code here
	CchenweicanImageDoc* pDoc = GetDocument(); //�����Լ�������������Ӧ����
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
		pOldBits = new unsigned char[nWidth * nHeight];//�����ڴ�
		CopyMemory(pOldBits, pBits, nWidth * nHeight);//��ͼ���Ƶ�pOldBit
		for (int i = 0; i < nHeight; i++)
		{
			lTotalR = i * nWidth;
			for (int j = 0; j < nWidth; j++)
			{
				lTotal = lTotalR + j;
				if (i == 0 || i == nHeight - 1 || j == 0 || j == nWidth - 1)//��Ե�㲻������ֱ�ӿ�������
					pBits[lTotal] = pOldBits[lTotal];
				else//�ڲ�����ƽ������
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
		pOldBits = new unsigned char[3 * nWidth * nHeight];//�����ڴ�
		CopyMemory(pOldBits, pBits, 3 * nWidth * nHeight);//��ͼ���Ƶ�pOldBit
		for (int i = 0; i < nHeight; i++)
		{
			lTotalR = i * nWidth;
			for (int j = 0; j < nWidth; j++)
			{
				lTotal = (lTotalR + j) * 3;
				if (i == 0 || i == nHeight - 1 || j == 0 || j == nWidth - 1)//��Ե�㲻������ֱ�ӿ�������
				{
					pBits[lTotal + 0] = pOldBits[lTotal + 0];
					pBits[lTotal + 1] = pOldBits[lTotal + 1];
					pBits[lTotal + 2] = pOldBits[lTotal + 2];
				}
				else//�ڲ�����ƽ������
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
		delete pOldBits; //ɾ����ʱ�����ڴ�
}

BOOL WINAPI GradSharp(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, BYTE bThre)
{
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	unsigned char* lpSrc1;
	unsigned char* lpSrc2;
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	// �м����
	BYTE bTemp;
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = (((lWidth * 8) + 31) / 32 * 4); // ÿ�� �ֽ�
	for (LONG i = 0; i < lHeight-1; i++){ // �������±�Ե
		// ÿ��
		for (LONG j = 0; j < lWidth-1; j++){ // �������ұ�Ե
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			// ָ��DIB��i+1�У���j�����ص�ָ��
			lpSrc1 = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 2 - i) + j;
			// ָ��DIB��i�У���j+1�����ص�ָ��
			lpSrc2 = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j + 1;
			bTemp = abs((*lpSrc) - (*lpSrc1)) + abs((*lpSrc) - (*lpSrc2));
			// �ж��Ƿ�С����ֵ
			if (bTemp < 255){
				if (bTemp >= bThre) // �ж��Ƿ������ֵ������С��������Ҷ�ֵ���䡣
					*lpSrc = bTemp;// ֱ�Ӹ�ֵΪbTemp
			}
			else  
				*lpSrc = 255; // ֱ�Ӹ�ֵΪ255
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
		// ��ʾ�û�
		MessageBox(_T("Ŀǰֻ֧��256���Ҷ�ͼ����񻯣�"), _T("ϵͳ��ʾ"),
			MB_ICONINFORMATION | MB_OK);
		// ����
		return;
	}
	// ��ֵ
	BYTE bThre = 10;
	// ���Ĺ����״
	BeginWaitCursor();
	// ����GradSharp()���������ݶȰ���
	if (::GradSharp(pBits, nWidth, nHeight, bThre))
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox(_T("�����ڴ�ʧ�ܣ�"), _T("ϵͳ��ʾ"), MB_ICONINFORMATION | MB_OK);
	}
	// �ָ����
	EndWaitCursor();
}

BOOL WINAPI Template(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight,int iTempH, int iTempW,int iTempMX, int iTempMY,FLOAT* fpArray, FLOAT fCoef)
{
	// ָ����ͼ���ָ��
	unsigned char* lpNewDIBBits;
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	// ָ��Ҫ���������ָ��
	unsigned char* lpDst;
	// ������
	FLOAT fResult;
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes = (((lWidth * 8) + 31) / 32 * 4);
	// ��ʱ�����ڴ棬�Ա�����ͼ��
	lpNewDIBBits = new unsigned char[lLineBytes * lHeight];
	// �ж��Ƿ��ڴ����ʧ��
	if (lpNewDIBBits == NULL)
		return FALSE; // �����ڴ�ʧ��
	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);
	// ��(��ȥ��Ե����)
	for (LONG i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++){
		// ��(��ȥ��Ե����)
		for (LONG j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++){
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			fResult = 0;
			// ����
			for (LONG k = 0; k < iTempH; k++){
				for (LONG l = 0; l < iTempW; l++){
					// ָ��DIB��i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k) + j - iTempMX + l;
					// ��������ֵ
					fResult += (*lpSrc) * fpArray[k * iTempW + l];
				}
			}
			// ����ϵ��
			fResult *= fCoef;
			// ȡ����ֵ
			fResult = (FLOAT)fabs(fResult);
			// �ж��Ƿ񳬹�255
			if (fResult > 255)
				*lpDst = 255; // ֱ�Ӹ�ֵΪ255
			else
				*lpDst = (unsigned char)(fResult + 0.5);
		}
	}
	// ���Ʊ任���ͼ��
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
		// ��ʾ�û�
		MessageBox(_T("Ŀǰֻ֧��256���Ҷ�ͼ����񻯣�"), _T("ϵͳ��ʾ"),
			MB_ICONINFORMATION | MB_OK);
		// ����
		return;
	}
	
	int iTempH; // ģ��߶�
	int iTempW; // ģ����
	// ģ��ϵ��
	FLOAT fTempC;
	int iTempMX; // ģ������Ԫ��X����
	int iTempMY; // ģ������Ԫ��Y����
	FLOAT aValue[9]; // ģ��Ԫ������
	// ���Ĺ����״
	BeginWaitCursor();
	// ����������˹ģ�����
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
	// ����Template()������������˹ģ����DIB
	if (::Template(pBits, nWidth, nHeight,iTempH, iTempW, iTempMX, iTempMY, aValue, fTempC))
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox(_T("�����ڴ�ʧ�ܣ�"), _T("ϵͳ��ʾ"), MB_ICONINFORMATION | MB_OK);
	}
	// �ָ����
	EndWaitCursor();
}

BOOL WINAPI SobelDIB(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight)
{
	// ָ�򻺴�ͼ���ָ��
	unsigned char* lpDst1;
	unsigned char* lpDst2;
	lWidth = (((lWidth * 8) + 31) / 32 * 4);
	// ָ�򻺴�DIBͼ���ָ��
	unsigned char* lpNewDIBBits1;
	unsigned char* lpNewDIBBits2;
	int iTempH;
	int iTempW;
	FLOAT fTempC; //ģ��ϵ��
	int iTempMX;
	int iTempMY;
	FLOAT aTemplate[9];  //ģ������
	lpNewDIBBits1 = new unsigned char[lWidth * lHeight]; // ��ʱ�����ڴ棬�Ա�����ͼ��
	if (lpNewDIBBits1 == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	lpNewDIBBits2 = new unsigned char[lWidth * lHeight]; // ��ʱ�����ڴ棬�Ա�����ͼ��
	if (lpNewDIBBits2 == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	// ����Դͼ�񵽻���ͼ����
	lpDst1 = lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);
	// ����Sobelģ�����
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
	// ����Template()����
	if (!Template(lpNewDIBBits1, lWidth, lHeight,iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}
	// ����Sobelģ�����
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;
	// ����Template()����
	if (!Template(lpNewDIBBits2, lWidth, lHeight,iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}
	//����������ͼ������ֵ
	for (long j = 0; j < lHeight; j++)
	{
		for (long i = 0; i < lWidth - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��
			lpDst1 = lpNewDIBBits1 + lWidth * j + i;
			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��
			lpDst2 = lpNewDIBBits2 + lWidth * j + i;
			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		}
	}
	// ���ƾ���ģ��������ͼ��Դͼ��
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);
	delete lpNewDIBBits2;
	delete lpNewDIBBits1;
	// ����
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
		// ��ʾ�û�
		MessageBox(_T("Ŀǰֻ֧��256���Ҷ�ͼ����񻯣�"), _T("ϵͳ��ʾ"),MB_ICONINFORMATION | MB_OK);
		return;
	}
	//Sobel��Ե�������
	// ����SobelDIB()������DIB���б�Ե���
	if (SobelDIB(pBits, nWidth, nHeight))
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox(_T("�����ڴ�ʧ�ܣ�"), _T("ϵͳ��ʾ"), MB_ICONINFORMATION | MB_OK);
	}
	// �ָ����
	EndWaitCursor();
}

BOOL WINAPI PrewittDIB(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight)
{
	// ָ�򻺴�ͼ���ָ��
	unsigned char* lpDst1;
	unsigned char* lpDst2;

	unsigned char* lpNewDIBBits1;
	unsigned char* lpNewDIBBits2;
	int iTempH;
	int iTempW;
	// ģ��ϵ��
	FLOAT fTempC;
	int iTempMX;
	int iTempMY;
	FLOAT aTemplate[9]; //ģ������
	lpNewDIBBits1 = new unsigned char[lWidth * lHeight]; // ��ʱ�����ڴ棬�Ա�����ͼ��
	if (lpNewDIBBits1 == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	lpNewDIBBits2 = new unsigned char[lWidth * lHeight]; // ��ʱ�����ڴ棬�Ա�����ͼ��
	if (lpNewDIBBits2 == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	// ����Դͼ�񵽻���ͼ����
	lpDst1 = lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);
	// ����Prewittģ�����
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
	// ����Template()����
	if (!Template(lpNewDIBBits1, lWidth, lHeight,iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}
	// ����Prewittģ�����
	aTemplate[0] = 1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = -1.0;
	// ����Template()����
	if (!Template(lpNewDIBBits2, lWidth, lHeight,iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}
	//����������ͼ������ֵ
	for (long j = 0; j < lHeight; j++)
	{
		for (long i = 0; i < lWidth - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��
			lpDst1 = lpNewDIBBits1 + lWidth * j + i;
			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��
			lpDst2 = lpNewDIBBits2 + lWidth * j + i;
			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;
		}
	}
	// ���ƾ���ģ��������ͼ��Դͼ��
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);
	// �ͷ��ڴ�
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
		MessageBox(_T("Ŀǰֻ֧��256ɫλͼ�����㣡"), _T("ϵͳ��ʾ"), MB_ICONINFORMATION | MB_OK);
		return;
	}
	// ����PrewittDIB()������DIB���б�Ե���
	if (PrewittDIB(pBits,nWidth, nHeight))
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox(_T("�����ڴ�ʧ�ܣ�"), _T("ϵͳ��ʾ"), MB_ICONINFORMATION | MB_OK);
	}
	EndWaitCursor();
}
