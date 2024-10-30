// CRotateDlg.cpp : implementation file
//

#include "pch.h"
#include "chenweicanImage.h"
#include "afxdialogex.h"
#include "CRotateDlg.h"


// CRotateDlg dialog

IMPLEMENT_DYNAMIC(CRotateDlg, CDialogEx)

CRotateDlg::CRotateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CROTANGLE, pParent)
	, m_fRotateAngle(0)
{

}

CRotateDlg::~CRotateDlg()
{
}

void CRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fRotateAngle);
}


BEGIN_MESSAGE_MAP(CRotateDlg, CDialogEx)
END_MESSAGE_MAP()


// CRotateDlg message handlers
