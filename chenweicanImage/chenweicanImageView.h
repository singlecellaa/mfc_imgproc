
// chenweicanImageView.h : interface of the CchenweicanImageView class
//

#pragma once


class CchenweicanImageView : public CScrollView
{
protected: // create from serialization only
	CchenweicanImageView() noexcept;
	DECLARE_DYNCREATE(CchenweicanImageView)

// Attributes
public:
	CchenweicanImageDoc* GetDocument() const;
	double m_dZoom;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	void DrawGraph(CDC* pDC);//����ֱ��ͼ����
	long m_lValue[256];//��ɫ�����Ҷ�ͳ��
	double m_dValue[256];// ��ɫ�����Ҷ�Ƶ��
	long m_lValueG[256]; //��ɫ�����Ҷ�ͳ��
	double m_dValueG[256]; // ��ɫ�����Ҷ�Ƶ��
	long m_lValueB[256]; //��ɫ�����Ҷ�ͳ��
	double m_dValueB[256]; //��ɫ�����Ҷ�Ƶ��
	double dMax, dMaxG, dMaxB;//�졢�̡���Ƶ�����ֵ
	BOOL m_bShow;//�Ƿ���ʾֱ��ͼ

	virtual ~CchenweicanImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewZoomIn();
	afx_msg void OnViewZoomOut();
	afx_msg void OnUpdateViewZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomOut(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnProcessLinetran();
	afx_msg void OnProcessZhifangtu();
	afx_msg void OnImageTxph();
	afx_msg void OnImageMedian();
	afx_msg void OnEnhaGradsharp();
	afx_msg void OnEnhaSharp();
	afx_msg void OnEdgeSobel();
	afx_msg void OnEdgePrewitt();
	afx_msg void OnInteequalize();
	afx_msg void OnImageRotate();
};

#ifndef _DEBUG  // debug version in chenweicanImageView.cpp
inline CchenweicanImageDoc* CchenweicanImageView::GetDocument() const
   { return reinterpret_cast<CchenweicanImageDoc*>(m_pDocument); }
#endif

