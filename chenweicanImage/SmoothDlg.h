#pragma once
#include "afxdialogex.h"


// SmoothDlg dialog

class SmoothDlg : public CDialog
{
	DECLARE_DYNAMIC(SmoothDlg)

public:
	SmoothDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SmoothDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SMOOTH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nSmooth1 = 1;
	int m_nSmooth2 = 1;
	int m_nSmooth3 = 1;
	int m_nSmooth4 = 1;
	int m_nSmooth5 = 0;
	int m_nSmooth6 = 1;
	int m_nSmooth7 = 1;
	int m_nSmooth8 = 1;
	int m_nSmooth9 = 1;
	double m_nSmooth10 = 0.125;
};
