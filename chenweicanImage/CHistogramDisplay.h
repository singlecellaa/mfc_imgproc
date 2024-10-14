#pragma once
#include "afxdialogex.h"


// CHistogramDisplay dialog

class CHistogramDisplay : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDisplay)

public:
	CHistogramDisplay(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CHistogramDisplay();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HISTOGRAM };
#endif

public:
	double m_dValue[256]; //红分量直方图
	double m_dValueG[256];//绿分量直方图
	double m_dValueB[256]; //蓝分量直方图
	double dMax, dMaxG, dMaxB;//红、绿、蓝直方图中最大值，绘图时控制绘图范围
	int m_nColorBits;//区分黑白和真彩色图像

	void DrawGraph(CDC* pDC);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
