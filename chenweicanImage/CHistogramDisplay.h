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
	double m_dValue[256]; //�����ֱ��ͼ
	double m_dValueG[256];//�̷���ֱ��ͼ
	double m_dValueB[256]; //������ֱ��ͼ
	double dMax, dMaxG, dMaxB;//�졢�̡���ֱ��ͼ�����ֵ����ͼʱ���ƻ�ͼ��Χ
	int m_nColorBits;//���ֺڰ׺����ɫͼ��

	void DrawGraph(CDC* pDC);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
