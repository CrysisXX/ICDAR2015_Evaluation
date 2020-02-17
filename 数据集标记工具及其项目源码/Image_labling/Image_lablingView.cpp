
// Image_lablingView.cpp: CImagelablingView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Image_labling.h"
#endif

#include "Image_lablingDoc.h"
#include "Image_lablingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <vector>
#include <fstream>
#include <iostream>
std::vector<CString> files;
int id = 0;

bool l_button_down = false;
CPoint points[4];
std::ofstream fout;
// CImagelablingView

IMPLEMENT_DYNCREATE(CImagelablingView, CView)

BEGIN_MESSAGE_MAP(CImagelablingView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CImagelablingView::OnFileOpen)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_ROTATE_BUTTON, &CImagelablingView::OnRotateButton)
	ON_COMMAND(ID_MOVELEFT_BUTTON, &CImagelablingView::OnMoveleftButton)
	ON_COMMAND(ID_MOVERIGHT_BUTTON, &CImagelablingView::OnMoverightButton)
	ON_COMMAND(ID_SAVE_BUTTON, &CImagelablingView::OnSaveButton)
	ON_COMMAND(ID_UP_BUTTON, &CImagelablingView::OnUpButton)
	ON_COMMAND(ID_DOWN_BUTTON, &CImagelablingView::OnDownButton)
	ON_COMMAND(ID_REVERSE_ROTATE_BUTTON, &CImagelablingView::OnReverseRotateButton)
END_MESSAGE_MAP()

// CImagelablingView 构造/析构

CImagelablingView::CImagelablingView() noexcept
{
	// TODO: 在此处添加构造代码

}

CImagelablingView::~CImagelablingView()
{
}

BOOL CImagelablingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CImagelablingView 绘图

void CImagelablingView::OnDraw(CDC* pDC)
{
	CImagelablingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (id < files.size()) {
		fout.close();
		CImage image;
		image.Load(files[id]);
		image.Draw(pDC->GetSafeHdc(), CPoint(0, 0));
		CString name = files[id];
		name.Replace(_T("jpg"),_T("txt"));
		fout.open(name);
	}
	// TODO: 在此处为本机数据添加绘制代码
}


// CImagelablingView 打印

BOOL CImagelablingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImagelablingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImagelablingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CImagelablingView 诊断

#ifdef _DEBUG
void CImagelablingView::AssertValid() const
{
	CView::AssertValid();
}

void CImagelablingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImagelablingDoc* CImagelablingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImagelablingDoc)));
	return (CImagelablingDoc*)m_pDocument;
}
#endif //_DEBUG


// CImagelablingView 消息处理程序


void CImagelablingView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR serve_path[MAX_PATH];
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = serve_path;
	bi.lpszTitle = _T("请输入数据路径");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = NULL;
	bi.iImage = NULL;
	if ((pidl = SHBrowseForFolder(&bi)) != NULL) {
		SHGetPathFromIDList(pidl, serve_path);
	}
	CString path=serve_path;
	CFileFind finder;
	BOOL flag = finder.FindFile(path+"\\*");
	while (flag) {
		flag = finder.FindNextFile();
		if (!finder.IsDots()) {
			files.push_back(path+"\\"+finder.GetFileName());
		}
	}
	id = 0;
	Invalidate();
}


void CImagelablingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	l_button_down = true;
	points[0] = point;
	CView::OnLButtonDown(nFlags, point);
}


void CImagelablingView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (l_button_down) {
		points[2] = point;
		points[1].x = points[2].x;
		points[1].y = points[0].y;
		points[3].x = points[0].x;
		points[3].y = points[2].y;
		draw_rectangle();
	}
	CView::OnMouseMove(nFlags, point);
}


void CImagelablingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	l_button_down = false;
	CView::OnLButtonUp(nFlags, point);
}


void CImagelablingView::OnRotateButton()
{
	// TODO: 在此添加命令处理程序代码
	for (int i = 0; i < 4; ++i) {
		rotate_point(points[i], 0.5 / 3.14);
	}
	draw_rectangle();

}

void CImagelablingView::rotate_point(CPoint &p,double angle)
{
	double center_x = (points[0].x + points[1].x + points[2].x + points[3].x) / 4.0, center_y = (points[0].y + points[1].y + points[2].y + points[3].y) / 4.0;
	double cos_theta = cos(angle), sin_theta = sin(angle);
	double dx = p.x - center_x, dy = p.y - center_y;
	p.x = dx * cos_theta - dy * sin_theta + center_x;
	p.y = dx * sin_theta + dy * cos_theta + center_y;
}

void CImagelablingView::draw_rectangle()
{
	if (id < files.size()) {
		CImage image;
		image.Load(files[id]);
		image.Draw(this->GetDC()->GetSafeHdc(), CPoint(0, 0));
	}
	CClientDC dc(this);
	CPen pen, * oldpen;
	pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	dc.SelectStockObject(NULL_BRUSH);
	oldpen = dc.SelectObject(&pen);
	for (int i = 0; i < 4; ++i) {
		dc.MoveTo(points[i].x, points[i].y);
		dc.LineTo(points[(i + 1) % 4].x, points[(i + 1) % 4].y);
	}
	dc.SelectObject(oldpen);
}


void CImagelablingView::OnMoveleftButton()
{
	// TODO: 在此添加命令处理程序代码
	for (int i = 0; i < 4; ++i) {
		points[i].x -= 1;
		if (points[i].x < 0)
			points[i].x = 0;
	}
	draw_rectangle();
}


void CImagelablingView::OnMoverightButton()
{
	// TODO: 在此添加命令处理程序代码
	for (int i = 0; i < 4; ++i) {
		points[i].x += 1;
	}
	draw_rectangle();
}


void CImagelablingView::OnSaveButton()
{
	// TODO: 在此添加命令处理程序代码
	fout << points[0].x << "," << points[0].y << "," << points[1].x << "," << points[1].y << "," << points[2].x << "," << points[2].y << "," << points[3].x << "," << points[3].y << std::endl;
}


void CImagelablingView::OnUpButton()
{
	// TODO: 在此添加命令处理程序代码
	for (int i = 0; i < 4; ++i) {
		points[i].y -= 1;
		if (points[i].y < 0)
			points[i].y = 0;
	}
	draw_rectangle();
}


void CImagelablingView::OnDownButton()
{
	// TODO: 在此添加命令处理程序代码
	for (int i = 0; i < 4; ++i) {
		points[i].y += 1;
	}
	draw_rectangle();
}


void CImagelablingView::OnReverseRotateButton()
{
	// TODO: 在此添加命令处理程序代码
	for (int i = 0; i < 4; ++i) {
		rotate_point(points[i], -0.5 / 3.14);
	}
	draw_rectangle();
}
