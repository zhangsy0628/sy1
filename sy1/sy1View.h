// sy1View.h: Csy1View 类的接口
#pragma once

// 必须包含这两个头文件，否则 CWindmill 和 CTransform2 会报“未定义标识符”错误 [cite: 9, 12]
#include "Windmill.h"
#include "Transform2.h"

class Csy1View : public CView
{
protected: // 仅从序列化创建
	Csy1View() noexcept;
	DECLARE_DYNCREATE(Csy1View)

	// 特性
public:
	Csy1Doc* GetDocument() const;

	// 操作
public:
	// 定义实验所需的成员变量 [cite: 9, 12, 14]
	CWindmill windmill;     // 风车类对象 
	CTransform2 trans;      // 二维变换类对象 [cite: 12]
	bool bPlay;             // 动画播放状态开关 

	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图 [cite: 10]
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	virtual ~Csy1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	// 手动添加动画控制函数的声明，解决 E0135 报错 
	afx_msg void OnPlay();                   // 响应播放菜单
	afx_msg void OnStop();                   // 响应停止菜单
	afx_msg void OnTimer(UINT_PTR nIDEvent); // 响应定时器消息 

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // sy1View.cpp 中的调试版本
inline Csy1Doc* Csy1View::GetDocument() const
{
	return reinterpret_cast<Csy1Doc*>(m_pDocument);
}
#endif