#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "sy1.h"
#endif
#include "sy1Doc.h"
#include "sy1View.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef ID_PLAY
#define ID_PLAY 32771
#endif
#ifndef ID_STOP
#define ID_STOP 32772
#endif

IMPLEMENT_DYNCREATE(Csy1View, CView)

BEGIN_MESSAGE_MAP(Csy1View, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Csy1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_COMMAND(ID_PLAY, &Csy1View::OnPlay)
	ON_COMMAND(ID_STOP, &Csy1View::OnStop)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

Csy1View::Csy1View() noexcept {
	bPlay = true;
}

Csy1View::~Csy1View() {}

BOOL Csy1View::PreCreateWindow(CREATESTRUCT& cs) {
	return CView::PreCreateWindow(cs);
}

void Csy1View::OnDraw(CDC* pDC) {
	// 1. 强制开启定时器
	static bool bTimerStarted = false;
	if (!bTimerStarted) {
		SetTimer(1, 50, NULL);
		bTimerStarted = true;
	}
	CRect rect;
	GetClientRect(&rect);
	if (windmill.R < 10) {
		windmill.R = 200;
		windmill.r = 60;
		windmill.ReadPoint();
		windmill.ReadFace();
	}
	// 2. 双缓冲准备
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&memBitmap);
	// 3. 刷白背景
	memDC.FillSolidRect(rect, RGB(255, 255, 255));
	// 4. 设置自定义坐标系（原点中心，Y轴向上） [cite: 8]
	memDC.SetMapMode(MM_ANISOTROPIC);
	memDC.SetWindowExt(rect.Width(), rect.Height());
	memDC.SetViewportExt(rect.Width(), -rect.Height());
	memDC.SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	// 红色十字架辅助线，确认中心点
	CPen redPen(PS_DOT, 1, RGB(255, 0, 0));
	CPen* pOldPen = memDC.SelectObject(&redPen);
	memDC.MoveTo(-1000, 0); memDC.LineTo(1000, 0);
	memDC.MoveTo(0, -1000); memDC.LineTo(0, 1000);
	// 创建黑色实线画笔绘制风车
	CPen blackPen(PS_SOLID, 2, RGB(0, 0, 0));
	memDC.SelectObject(&blackPen);
	windmill.Draw(&memDC); // [cite: 10]
	memDC.SetMapMode(MM_TEXT);
	memDC.SetViewportOrg(0, 0);
	memDC.SetWindowExt(1, 1);
	memDC.SetViewportExt(1, 1);
	// 6. 1:1 拷贝到屏幕 [cite: 13]
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldPen);
	memDC.SelectObject(pOldBmp);
}

void Csy1View::OnTimer(UINT_PTR nIDEvent) {
	if (nIDEvent == 1) {
		trans.SetMatrix(windmill.P, 9); // [cite: 9]
		trans.Rotate(5);                // 旋转 [cite: 12]
		Invalidate(FALSE);              // 刷新重绘
	}
	CView::OnTimer(nIDEvent);
}

void Csy1View::OnPlay() { SetTimer(1, 50, NULL); bPlay = true; }
void Csy1View::OnStop() { KillTimer(1); bPlay = false; }

void Csy1View::OnLButtonDown(UINT nFlags, CPoint point) {
	// 点击屏幕任何地方都可以暂停/继续
	if (bPlay) OnStop();
	else OnPlay();
	CView::OnLButtonDown(nFlags, point);
}

// === 以下都是不用管的自带函数 ===
void Csy1View::OnRButtonUp(UINT /* nFlags */, CPoint point) {
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Csy1View::OnContextMenu(CWnd* /* pWnd */, CPoint point) {
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void Csy1View::OnFilePrintPreview() {
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Csy1View::OnPreparePrinting(CPrintInfo* pInfo) { return DoPreparePrinting(pInfo); }
void Csy1View::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {}
void Csy1View::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {}

#ifdef _DEBUG
void Csy1View::AssertValid() const { CView::AssertValid(); }
void Csy1View::Dump(CDumpContext& dc) const { CView::Dump(dc); }
Csy1Doc* Csy1View::GetDocument() const {
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Csy1Doc)));
	return (Csy1Doc*)m_pDocument;
}
#endif