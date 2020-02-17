
// Image_lablingView.h: CImagelablingView 类的接口
//

#pragma once


class CImagelablingView : public CView
{
protected: // 仅从序列化创建
	CImagelablingView() noexcept;
	DECLARE_DYNCREATE(CImagelablingView)

// 特性
public:
	CImagelablingDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CImagelablingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRotateButton();
	void rotate_point(CPoint&p,double);
	void draw_rectangle();
	afx_msg void OnMoveleftButton();
	afx_msg void OnMoverightButton();
	afx_msg void OnSaveButton();
	afx_msg void OnUpButton();
	afx_msg void OnDownButton();
	afx_msg void OnReverseRotateButton();
};

#ifndef _DEBUG  // Image_lablingView.cpp 中的调试版本
inline CImagelablingDoc* CImagelablingView::GetDocument() const
   { return reinterpret_cast<CImagelablingDoc*>(m_pDocument); }
#endif

