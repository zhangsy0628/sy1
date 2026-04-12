// sy1View.cpp: Csy1View 类的实现
#include "pch.h"
#include "framework.h"

#ifndef SHARED_HANDLERS
#include "sy1.h"
#endif

#include "sy1Doc.h"
#include "sy1View.h"
#include "Resource.h"
// 如果 Resource.h 里没定义，这里才定义
#ifndef ID_PLAY
#define ID_PLAY 32771
#endif
#ifndef ID_STOP
#define ID_STOP 32772
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Csy1View
IMPLEMENT_DYNCREATE(Csy1View, CView)

BEGIN_MESSAGE_MAP(Csy1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Csy1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	// --- 手动添加的消息映射 ---
	ON_WM_TIMER()                    // 响应定时器消息 
	ON_COMMAND(ID_PLAY, &Csy1View::OnPlay)   // 响应播放菜单/按钮 
	ON_COMMAND(ID_STOP, &Csy1View::OnStop)   // 响应停止菜单/按钮 
END_MESSAGE_MAP()

// Csy1View 构造/析构
Csy1View::Csy1View() noexcept {
	bPlay = false; // 初始化播放状态为停止 
}

Csy1View::~Csy1View() {}

BOOL Csy1View::PreCreateWindow(CREATESTRUCT& cs) {
	return CView::PreCreateWindow(cs);
}

// Csy1View 绘图
void Csy1View::OnDraw(CDC* pDC) {
	// 1. 强制启动定时器
	static bool bInited = false;
	if (!bInited) {
		SetTimer(1, 50, NULL);
		bInited = true;
	}

	CRect rect;
	GetClientRect(&rect);

	// 2. 双缓冲准备 [cite: 13]
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	memDC.SelectObject(&memBitmap);

	// 涂背景色（确保不是透明的）
	memDC.FillSolidRect(rect, RGB(255, 255, 255));

	// 3. 设置坐标系：原点中心，y轴向上 [cite: 8]
	memDC.SetMapMode(MM_ANISOTROPIC);
	memDC.SetWindowExt(rect.Width(), rect.Height());
	memDC.SetViewportExt(rect.Width(), -rect.Height());
	memDC.SetViewportOrg(rect.Width() / 2, rect.Height() / 2);

	// 4. 重点：确保风车有半径数据（防止 R=0 导致画不出来）
	if (windmill.R == 0) {
		windmill.R = 200;
		windmill.r = 60;
		windmill.ReadPoint();
		windmill.ReadFace();
	}

	// 5. 绘制 [cite: 10]
	windmill.Draw(&memDC);

	// 6. 拷贝到屏幕 [cite: 13]
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, -rect.Width() / 2, rect.Height() / 2, SRCCOPY);
}

void Csy1View::OnPlay() {
	if (!bPlay) {
		bPlay = true;
		SetTimer(1, 50, NULL); // 设置定时器，每50毫秒旋转一次 
	}
}

void Csy1View::OnStop() {
	if (bPlay) {
		bPlay = false;
		KillTimer(1); // 停止定时器 
	}
}

void Csy1View::OnTimer(UINT_PTR nIDEvent) {
	if (nIDEvent == 1) {
		// 设计二维变换类进行旋转变换 [cite: 5, 12]
		trans.SetMatrix(windmill.P, 9); // 读取风车顶点 [cite: 9]
		trans.Rotate(5);                // 顺时针旋转5度
		Invalidate(FALSE);              // 触发重绘 
	}
	CView::OnTimer(nIDEvent);
}

// --- 其他标准函数保持不变 ---

void Csy1View::OnFilePrintPreview() {
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Csy1View::OnPreparePrinting(CPrintInfo* pInfo) {
	return DoPreparePrinting(pInfo);
}

void Csy1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {}

void Csy1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {}

void Csy1View::OnRButtonUp(UINT /* nFlags */, CPoint point) {
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Csy1View::OnContextMenu(CWnd* /* pWnd */, CPoint point) {
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

#ifdef _DEBUG
void Csy1View::AssertValid() const { CView::AssertValid(); }
void Csy1View::Dump(CDumpContext& dc) const { CView::Dump(dc); }
Csy1Doc* Csy1View::GetDocument() const {
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Csy1Doc)));
	return (Csy1Doc*)m_pDocument;
}
#endif