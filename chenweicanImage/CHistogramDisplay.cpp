// CHistogramDisplay.cpp : implementation file
//

#include "pch.h"
#include "chenweicanImage.h"
#include "afxdialogex.h"
#include "CHistogramDisplay.h"


// CHistogramDisplay dialog

IMPLEMENT_DYNAMIC(CHistogramDisplay, CDialogEx)

CHistogramDisplay::CHistogramDisplay(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_HISTOGRAM, pParent)
{

}

CHistogramDisplay::~CHistogramDisplay()
{
}

void CHistogramDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistogramDisplay, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistogramDisplay message handlers

void CHistogramDisplay::DrawGraph(CDC* pDC)
{
	int X0 = 0 + 250;
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
	if (m_nColorBits == 24)
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
void CHistogramDisplay::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	DrawGraph(&dc);
}
