#pragma once

#include "Windmill.h"
#include "Transform2.h"

class Csy1View : public CView
{
protected: // 仅从序列化创建
	Csy1View() noexcept;
	DECLARE_DYNCREATE(Csy1View)

public:
	Csy1Doc* GetDocument() const;

public:
	CWindmill windmill;     // 风车类对象
	CTransform2 trans;      // 二维变换类对象
	bool bPlay;             // 动画播放状态开关

public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	virtual ~Csy1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); // 鼠标左键点击开关

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  
inline Csy1Doc* Csy1View::GetDocument() const
{
	return reinterpret_cast<Csy1Doc*>(m_pDocument);
}
#endif