/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumGraphics.cpp
* @brief	This Program is PlumGraphics DLL Project.
* @author	Alopex/Helium
* @version	v1.01a
* @date		2017-12-28	v1.00a	alopex	Create Project.
* @date		2018-07-25	v1.01a	alopex	Add Call Mode.
*/
#include "PlumGraphics.h"

//This Project is Use For GDI Graphics(GDI相关绘图)

//------------------------------------------------------------------
// @Function:	 CPlumGraphics()
// @Purpose: CPlumGraphics构造函数
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CPlumGraphics::CPlumGraphics()
{
}

//------------------------------------------------------------------
// @Function:	 ~CPlumGraphics()
// @Purpose: CPlumGraphics析构函数
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CPlumGraphics::~CPlumGraphics()
{
}

//------------------------------------------------------------------
// @Function:	 CPlumGraphics(HWND hWnd)
// @Purpose: CPlumGraphics构造函数
// @Since: v1.00a
// @Para: HWND hWnd
// @Return: None
//------------------------------------------------------------------
CPlumGraphics::CPlumGraphics(HWND hWnd, RECT sRect)
{
	m_hWnd = hWnd;
	m_sRect = sRect;
}

//------------------------------------------------------------------
// @Function:	 PlumGraphicsSetHandle(HWND hWnd)
// @Purpose: PlumGraphics设置窗口句柄
// @Since: v1.00a
// @Para: HWND hWnd			//窗口句柄
// @Return: None
//------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsSetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

//------------------------------------------------------------------
// @Function:	 PlumGraphicsSetRect(RECT sRect)
// @Purpose: PlumGraphics设置窗口区域
// @Since: v1.00a
// @Para: RECT sRect		//窗口区域
// @Return: None
//------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsSetRect(RECT sRect)
{
	m_sRect = sRect;
}

//------------------------------------------------------------------
// @Function:	 PlumGraphicsGetHandle(void) const
// @Purpose: PlumGraphics设置窗口句柄
// @Since: v1.00a
// @Para: None
// @Return: HWND (窗口句柄)
//------------------------------------------------------------------
HWND PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsGetHandle(void) const
{
	return m_hWnd;
}

//------------------------------------------------------------------
// @Function:	 PlumGraphicsGetRect(void)
// @Purpose: PlumGraphics获取窗口区域
// @Since: v1.00a
// @Para: RECT sRect		//窗口区域
// @Return: None
//------------------------------------------------------------------
RECT PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsGetRect(void) const
{
	return m_sRect;
}

//-----------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawLine(int nStartX, int nStartY, int nEndX, int nEndY)
// @Purpose: PlumGraphics绘制直线
// @Since: v1.00a
// @Para: int nStartX		//开始点X坐标
// @Para: int nStartY		//开始点Y坐标
// @Para: int nEndX			//结束点X坐标
// @Para: int nEndY			//结束点Y坐标
// @Return: None
//-----------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawLine(int nStartX, int nStartY, int nEndX, int nEndY)
{
	HDC hDC;
	HPEN hPen;

	hDC = GetDC(m_hWnd);
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	SelectObject(hDC, hPen);
	MoveToEx(hDC, nStartX, nStartY, NULL);
	LineTo(hDC, nEndX, nEndY);

	DeleteObject(hPen);
	ReleaseDC(m_hWnd, hDC);
}

//-----------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawLine(PlumPoint sStart, PlumPoint sEnd)
// @Purpose: PlumGraphics绘制直线
// @Since: v1.00a
// @Para: PlumPoint sStart		//开始坐标点
// @Para: PlumPoint sEnd		//结束坐标点
// @Return: None
//-----------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawLine(PlumPoint sStart, PlumPoint sEnd)
{
	HDC hDC;
	HPEN hPen;

	hDC = GetDC(m_hWnd);
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	SelectObject(hDC, hPen);
	MoveToEx(hDC, sStart.X, sStart.Y, NULL);
	LineTo(hDC, sEnd.X, sEnd.Y);

	DeleteObject(hPen);
	ReleaseDC(m_hWnd, hDC);
}

//-------------------------------------------------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawLine(PlumPoint sStart, PlumPoint sEnd, PlumPenType ePenType, int nWidth, COLORREF crColor)
// @Purpose: PlumGraphics绘制直线
// @Since: v1.00a
// @Para: PlumPoint sStart		//开始坐标点
// @Para: PlumPoint sEnd		//结束坐标点
// @Para: PlumPenType ePenType	//画笔样式
// @Para: int nWidth			//画笔宽度
// @Para: COLORREF crColor		//画笔颜色
// @Return: None
//-------------------------------------------------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawLine(PlumPoint sStart, PlumPoint sEnd, PlumPenType ePenType, int nWidth, COLORREF crColor)
{
	HDC hDC;
	HPEN hPen;

	hDC = GetDC(m_hWnd);

	switch (ePenType)
	{
	case PlumPen_Solid:
		hPen = CreatePen(PS_SOLID, nWidth, crColor);
		break;
	case PlumPen_Dash:
		hPen = CreatePen(PS_DASH, 1, crColor);
		break;
	case PlumPen_Dot:
		hPen = CreatePen(PS_DOT , 1, crColor);
		break;
	case PlumPen_DashDot:
		hPen = CreatePen(PS_DASHDOT, 1, crColor);
		break;
	case PlumPen_DashDotDot:
		hPen = CreatePen(PS_DASHDOTDOT, 1, crColor);
		break;
	default:
		return;
		break;
	}

	SelectObject(hDC, hPen);
	MoveToEx(hDC, sStart.X, sStart.Y, NULL);
	LineTo(hDC, sEnd.X, sEnd.Y);

	DeleteObject(hPen);
	ReleaseDC(m_hWnd, hDC);
}

//---------------------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawLineOnOriginal(int nStartX, int nStartY, int nEndX, int nEndY)
// @Purpose: PlumGraphics绘制直线
// @Since: v1.00a
// @Para: int nStartX		//开始点X坐标
// @Para: int nStartY		//开始点Y坐标
// @Para: int nEndX			//结束点X坐标
// @Para: int nEndY			//结束点Y坐标
// @Return: None
//---------------------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawLineOnOriginal(int nStartX, int nStartY, int nEndX, int nEndY)
{
	HDC hDC;
	HDC hMemDC;
	HBITMAP hBitmap;
	HPEN hPen;
	int nWidth;
	int nHeight;

	nWidth = m_sRect.right - m_sRect.left;
	nHeight = m_sRect.bottom - m_sRect.top;

	hDC = GetDC(m_hWnd);
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	SelectObject(hMemDC, hBitmap);
	SelectObject(hMemDC, hPen);
	MoveToEx(hMemDC, nStartX, nStartY, NULL);
	LineTo(hMemDC, nEndX, nEndY);

	BitBlt(hDC, m_sRect.left, m_sRect.top, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);

	DeleteObject(hPen);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	ReleaseDC(m_hWnd, hDC);
}

//-------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawLineOnOriginal(PlumPoint sStart, PlumPoint sEnd)
// @Purpose: PlumGraphics绘制直线
// @Since: v1.00a
// @Para: PlumPoint sStart		//开始坐标点
// @Para: PlumPoint sEnd		//结束坐标点
// @Return: None
//-------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawLineOnOriginal(PlumPoint sStart, PlumPoint sEnd)
{
	HDC hDC;
	HDC hMemDC;
	HBITMAP hBitmap;
	HPEN hPen;
	int nWidth;
	int nHeight;

	nWidth = m_sRect.right - m_sRect.left;
	nHeight = m_sRect.bottom - m_sRect.top;

	hDC = GetDC(m_hWnd);
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	SelectObject(hMemDC, hBitmap);
	SelectObject(hMemDC, hPen);
	MoveToEx(hMemDC, sStart.X, sStart.Y, NULL);
	LineTo(hMemDC, sEnd.X, sEnd.Y);

	BitBlt(hDC, m_sRect.left, m_sRect.top, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);

	DeleteObject(hPen);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	ReleaseDC(m_hWnd, hDC);
}

//-----------------------------------------------------------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawLineOnOriginal(PlumPoint sStart, PlumPoint sEnd, PlumPenType ePenType, int nWidth, COLORREF crColor)
// @Purpose: PlumGraphics绘制直线
// @Since: v1.00a
// @Para: PlumPoint sStart		//开始坐标点
// @Para: PlumPoint sEnd		//结束坐标点
// @Para: PlumPenType ePenType	//画笔样式
// @Para: int nWidth			//画笔宽度
// @Para: COLORREF crColor		//画笔颜色
// @Return: None
//-----------------------------------------------------------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawLineOnOriginal(PlumPoint sStart, PlumPoint sEnd, PlumPenType ePenType, int nPenWidth, COLORREF crColor)
{
	HDC hDC;
	HDC hMemDC;
	HBITMAP hBitmap;
	HPEN hPen;
	int nWidth;
	int nHeight;

	nWidth = m_sRect.right - m_sRect.left;
	nHeight = m_sRect.bottom - m_sRect.top;

	hDC = GetDC(m_hWnd);
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	
	switch (ePenType)
	{
	case PlumPen_Solid:
		hPen = CreatePen(PS_SOLID, nPenWidth, crColor);
		break;
	case PlumPen_Dash:
		hPen = CreatePen(PS_DASH, 1, crColor);
		break;
	case PlumPen_Dot:
		hPen = CreatePen(PS_DOT, 1, crColor);
		break;
	case PlumPen_DashDot:
		hPen = CreatePen(PS_DASHDOT, 1, crColor);
		break;
	case PlumPen_DashDotDot:
		hPen = CreatePen(PS_DASHDOTDOT, 1, crColor);
		break;
	default:
		return;
		break;
	}

	SelectObject(hMemDC, hBitmap);
	SelectObject(hMemDC, hPen);
	MoveToEx(hMemDC, sStart.X, sStart.Y, NULL);
	LineTo(hMemDC, sEnd.X, sEnd.Y);

	BitBlt(hDC, m_sRect.left, m_sRect.top, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);

	DeleteObject(hPen);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	ReleaseDC(m_hWnd, hDC);
}

//--------------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawRectangle(int nX, int nY, int nWidth, int nHeight)
// @Purpose: PlumGraphics绘制矩形
// @Since: v1.00a
// @Para: int nX				//绘制矩形X坐标
// @Para: int nY				//绘制矩形Y坐标
// @Para: int nWidth			//绘制矩形宽度
// @Para: int nHeight			//绘制矩形高度
// @Return: None
//--------------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawRectangle(int nX, int nY, int nWidth, int nHeight)
{
	HDC hDC;
	HPEN hPen;

	hDC = GetDC(m_hWnd);
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	SelectObject(hDC, hPen);
	Rectangle(hDC, nX, nY, (nX + nWidth), (nY + nHeight));

	DeleteObject(hPen);
	ReleaseDC(m_hWnd, hDC);
}

//--------------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawRectangle(RECT sRect)
// @Purpose: PlumGraphics绘制矩形
// @Since: v1.00a
// @Para: int nX				//绘制矩形X坐标
// @Para: int nY				//绘制矩形Y坐标
// @Para: int nWidth			//绘制矩形宽度
// @Para: int nHeight			//绘制矩形高度
// @Return: None
//--------------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawRectangle(RECT sRect)
{
	HDC hDC;
	HPEN hPen;

	hDC = GetDC(m_hWnd);
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	SelectObject(hDC, hPen);
	Rectangle(hDC, sRect.left, sRect.top, sRect.right, sRect.bottom);

	DeleteObject(hPen);
	ReleaseDC(m_hWnd, hDC);
}

//--------------------------------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawRectangle(RECT sRect, PlumPenType ePenType, int nWidth, COLORREF crColor)
// @Purpose: PlumGraphics绘制矩形
// @Since: v1.00a
// @Para: RECT sRect			//绘制矩形区域
// @Para: PlumPenType ePenType	//绘制画笔枚举
// @Para: int nWidth			//绘制画笔宽度
// @Para: COLORREF crColor		//绘制画笔颜色
// @Return: None
//--------------------------------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawRectangle(RECT sRect, PlumPenType ePenType, int nWidth, COLORREF crColor)
{
	HDC hDC;
	HPEN hPen;

	hDC = GetDC(m_hWnd);

	switch (ePenType)
	{
	case PlumPen_Solid:
		hPen = CreatePen(PS_SOLID, nWidth, crColor);
		break;
	case PlumPen_Dash:
		hPen = CreatePen(PS_DASH, 1, crColor);
		break;
	case PlumPen_Dot:
		hPen = CreatePen(PS_DOT, 1, crColor);
		break;
	case PlumPen_DashDot:
		hPen = CreatePen(PS_DASHDOT, 1, crColor);
		break;
	case PlumPen_DashDotDot:
		hPen = CreatePen(PS_DASHDOTDOT, 1, crColor);
		break;
	default:
		return;
		break;
	}

	SelectObject(hDC, hPen);
	Rectangle(hDC, sRect.left, sRect.top, sRect.right, sRect.bottom);

	DeleteObject(hPen);
	ReleaseDC(m_hWnd, hDC);
}

//----------------------------------------------------------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawRectangle(RECT sRect, PlumPenType ePenType, int nWidth, COLORREF crColorPen, COLORREF crColorBrush)
// @Purpose: PlumGraphics绘制矩形
// @Since: v1.00a
// @Para: RECT sRect			//绘制矩形区域
// @Para: PlumPenType ePenType	//绘制画笔枚举
// @Para: int nWidth			//绘制画笔宽度
// @Para: COLORREF crColorPen	//绘制画笔颜色
// @Para: COLORREF crColorBrush	//绘制画刷颜色
// @Return: None
//----------------------------------------------------------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawRectangle(RECT sRect, PlumPenType ePenType, int nWidth, COLORREF crColorPen, COLORREF crColorBrush)
{
	HDC hDC;
	HPEN hPen;
	HBRUSH hBrush;

	hDC = GetDC(m_hWnd);

	switch (ePenType)
	{
	case PlumPen_Solid:
		hPen = CreatePen(PS_SOLID, nWidth, crColorPen);
		break;
	case PlumPen_Dash:
		hPen = CreatePen(PS_DASH, 1, crColorPen);
		break;
	case PlumPen_Dot:
		hPen = CreatePen(PS_DOT, 1, crColorPen);
		break;
	case PlumPen_DashDot:
		hPen = CreatePen(PS_DASHDOT, 1, crColorPen);
		break;
	case PlumPen_DashDotDot:
		hPen = CreatePen(PS_DASHDOTDOT, 1, crColorPen);
		break;
	default:
		return;
		break;
	}

	hBrush = CreateSolidBrush(crColorBrush);

	SelectObject(hDC, hPen);
	SelectObject(hDC, hBrush);
	Rectangle(hDC, sRect.left, sRect.top, sRect.right, sRect.bottom);

	DeleteObject(hPen);
	DeleteObject(hBrush);
	ReleaseDC(m_hWnd, hDC);
}

//-------------------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawRectangleOnOriginal(int nX, int nY, int nWidth, int nHeight)
// @Purpose: PlumGraphics绘制矩形
// @Since: v1.00a
// @Para: int nX				//绘制矩形X坐标
// @Para: int nY				//绘制矩形Y坐标
// @Para: int nWidth			//绘制矩形宽度
// @Para: int nHeight			//绘制矩形高度
// @Return: None
//-------------------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawRectangleOnOriginal(int nX, int nY, int nWidth, int nHeight)
{
	HDC hDC;
	HDC hMemDC;
	HBITMAP hBitmap;
	HPEN hPen;

	hDC = GetDC(m_hWnd);
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	SelectObject(hMemDC, hBitmap);
	SelectObject(hMemDC, hPen);
	Rectangle(hMemDC, nX, nY, (nX + nWidth), (nY + nHeight));

	BitBlt(hDC, m_sRect.left, m_sRect.top, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);

	DeleteObject(hPen);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	ReleaseDC(m_hWnd, hDC);
}

//--------------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawRectangle(RECT sRect)
// @Purpose: PlumGraphics绘制矩形
// @Since: v1.00a
// @Para: int nX				//绘制矩形X坐标
// @Para: int nY				//绘制矩形Y坐标
// @Para: int nWidth			//绘制矩形宽度
// @Para: int nHeight			//绘制矩形高度
// @Return: None
//--------------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawRectangleOnOriginal(RECT sRect)
{
	HDC hDC;
	HDC hMemDC;
	HBITMAP hBitmap;
	HPEN hPen;
	int nWidth;
	int nHeight;

	nWidth = sRect.right - sRect.left;
	nHeight = sRect.bottom - sRect.top;

	hDC = GetDC(m_hWnd);
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	SelectObject(hMemDC, hBitmap);
	SelectObject(hMemDC, hPen);
	Rectangle(hMemDC, sRect.left, sRect.top, sRect.right, sRect.bottom);

	BitBlt(hDC, m_sRect.left, m_sRect.top, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);

	DeleteObject(hPen);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	ReleaseDC(m_hWnd, hDC);
}

//--------------------------------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawRectangle(RECT sRect, PlumPenType ePenType, int nWidth, COLORREF crColor)
// @Purpose: PlumGraphics绘制矩形
// @Since: v1.00a
// @Para: RECT sRect			//绘制矩形区域
// @Para: PlumPenType ePenType	//绘制画笔枚举
// @Para: int nWidth			//绘制画笔宽度
// @Para: COLORREF crColor		//绘制画笔颜色
// @Return: None
//--------------------------------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawRectangleOnOriginal(RECT sRect, PlumPenType ePenType, int nPenWidth, COLORREF crPenColor)
{
	HDC hDC;
	HDC hMemDC;
	HBITMAP hBitmap;
	HPEN hPen;
	int nWidth;
	int nHeight;

	nWidth = sRect.right - sRect.left;
	nHeight = sRect.bottom - sRect.top;

	hDC = GetDC(m_hWnd);
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	
	switch (ePenType)
	{
	case PlumPen_Solid:
		hPen = CreatePen(PS_SOLID, nPenWidth, crPenColor);
		break;
	case PlumPen_Dash:
		hPen = CreatePen(PS_DASH, 1, crPenColor);
		break;
	case PlumPen_Dot:
		hPen = CreatePen(PS_DOT, 1, crPenColor);
		break;
	case PlumPen_DashDot:
		hPen = CreatePen(PS_DASHDOT, 1, crPenColor);
		break;
	case PlumPen_DashDotDot:
		hPen = CreatePen(PS_DASHDOTDOT, 1, crPenColor);
		break;
	default:
		return;
		break;
	}

	SelectObject(hMemDC, hBitmap);
	SelectObject(hMemDC, hPen);
	Rectangle(hMemDC, sRect.left, sRect.top, sRect.right, sRect.bottom);

	BitBlt(hDC, m_sRect.left, m_sRect.top, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);

	DeleteObject(hPen);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	ReleaseDC(m_hWnd, hDC);
}

//----------------------------------------------------------------------------------------------------------------------------------
// @Function:	 PlumGraphicsDrawRectangle(RECT sRect, PlumPenType ePenType, int nWidth, COLORREF crColorPen, COLORREF crColorBrush)
// @Purpose: PlumGraphics绘制矩形
// @Since: v1.00a
// @Para: RECT sRect			//绘制矩形区域
// @Para: PlumPenType ePenType	//绘制画笔枚举
// @Para: int nWidth			//绘制画笔宽度
// @Para: COLORREF crColorPen	//绘制画笔颜色
// @Para: COLORREF crColorBrush	//绘制画刷颜色
// @Return: None
//----------------------------------------------------------------------------------------------------------------------------------
void PLUMGRAPHICS_CALLMODE CPlumGraphics::PlumGraphicsDrawRectangleOnOriginal(RECT sRect, PlumPenType ePenType, int nPenWidth, COLORREF crPenColor, COLORREF crBrushColor)
{
	HDC hDC;
	HDC hMemDC;
	HBITMAP hBitmap;
	HPEN hPen;
	HBRUSH hBrush;
	int nWidth;
	int nHeight;

	nWidth = sRect.right - sRect.left;
	nHeight = sRect.bottom - sRect.top;

	hDC = GetDC(m_hWnd);
	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);

	switch (ePenType)
	{
	case PlumPen_Solid:
		hPen = CreatePen(PS_SOLID, nPenWidth, crPenColor);
		break;
	case PlumPen_Dash:
		hPen = CreatePen(PS_DASH, 1, crPenColor);
		break;
	case PlumPen_Dot:
		hPen = CreatePen(PS_DOT, 1, crPenColor);
		break;
	case PlumPen_DashDot:
		hPen = CreatePen(PS_DASHDOT, 1, crPenColor);
		break;
	case PlumPen_DashDotDot:
		hPen = CreatePen(PS_DASHDOTDOT, 1, crPenColor);
		break;
	default:
		return;
		break;
	}

	hBrush = CreateSolidBrush(crBrushColor);

	SelectObject(hMemDC, hBitmap);
	SelectObject(hMemDC, hBrush);
	SelectObject(hMemDC, hPen);
	Rectangle(hMemDC, sRect.left, sRect.top, sRect.right, sRect.bottom);

	BitBlt(hDC, m_sRect.left, m_sRect.top, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);

	DeleteObject(hPen);
	DeleteObject(hBrush);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
	ReleaseDC(m_hWnd, hDC);
}