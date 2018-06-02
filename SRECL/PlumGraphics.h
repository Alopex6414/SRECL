/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumGraphics.h
* @brief	This Program is PlumGraphics DLL Project.
* @author	Alopex/Helium
* @version	v1.00a
* @date		2017-12-28	v1.00a	alopex	Create Project.
*/
#pragma once

#ifndef __PLUMGRAPHICS_H_
#define __PLUMGRAPHICS_H_

//Include Windows Header File
#include <Windows.h>

//Include C/C++ Run Header File
#include <stdio.h>

//Macro Definition
#ifdef	SRECL_EXPORTS
#define PLUMGRAPHICS_API	__declspec(dllexport)
#else
#define PLUMGRAPHICS_API	__declspec(dllimport)
#endif

//Macro Pointer
#define SAFE_DELETE(Pointer)	{if(Pointer){delete(Pointer);(Pointer) = NULL;}}
#define SAFE_DELETE_ARRAY(Pointer) {if(Pointer){delete[](Pointer);(Pointer) = NULL;}}

//Struct Definition
struct PlumPoint
{
	int X;
	int Y;
	PlumPoint(int x, int y): X(x), Y(y) {}
};

//Enum Definition
enum PlumPenType
{
	PlumPen_Solid = 0,		//ʵ��
	PlumPen_Dash = 1,		//����
	PlumPen_Dot = 2,		//����
	PlumPen_DashDot = 3,	//�㻮��
	PlumPen_DashDotDot = 4,	//��㻮��
};

//Class Definition
class PLUMGRAPHICS_API CPlumGraphics
{
private:
	HWND m_hWnd;			//PlumGraphics GDI Window Handle(GDI���ھ��)
	RECT m_sRect;			//PlumGraphics GDI Draw Area Size(GDI���ڻ��������С)

public:
	CPlumGraphics();					//PlumGraphics Construction Function(GDI���캯��)
	virtual ~CPlumGraphics();			//PlumGraphics Destruction Function(GDI��������)

	//����
	CPlumGraphics(HWND hWnd, RECT sRect);			//PlumGraphics Construction Function(GDI���캯��)

	//����
	virtual void PlumGraphicsSetHandle(HWND hWnd);	//PlumGraphics Set Handle(GDI���ô��ھ��)
	virtual void PlumGraphicsSetRect(RECT sRect);	//PlumGraphics Set Rect(GDI���ô�������)

	//����
	virtual HWND PlumGraphicsGetHandle(void) const;	//PlumGraphics Get Handle(GDI��ȡ���ھ��)
	virtual RECT PlumGraphicsGetRect(void) const;	//PlumGraphics Get Rect(GDI��ȡ��������)

	//����
	//ֱ��
	virtual void PlumGraphicsDrawLine(int nStartX, int nStartY, int nEndX, int nEndY);														//PlumGraphics Draw Line(GDI����ֱ��)
	virtual void PlumGraphicsDrawLine(PlumPoint sStart, PlumPoint sEnd);																	//PlumGraphics Draw Line(GDI����ֱ��)
	virtual void PlumGraphicsDrawLine(PlumPoint sStart, PlumPoint sEnd, PlumPenType ePenType, int nWidth, COLORREF crColor);				//PlumGraphics Draw Line(GDI����ֱ��)
	virtual void PlumGraphicsDrawLineOnOriginal(int nStartX, int nStartY, int nEndX, int nEndY);											//PlumGraphics Draw Line(GDI����ֱ��)
	virtual void PlumGraphicsDrawLineOnOriginal(PlumPoint sStart, PlumPoint sEnd);															//PlumGraphics Draw Line(GDI����ֱ��)
	virtual void PlumGraphicsDrawLineOnOriginal(PlumPoint sStart, PlumPoint sEnd, PlumPenType ePenType, int nPenWidth, COLORREF crColor);	//PlumGraphics Draw Line(GDI����ֱ��)

	//����
	virtual void PlumGraphicsDrawRectangle(int nX, int nY, int nWidth, int nHeight);														//PlumGraphics Draw Rectangle(GDI���ƾ���)
	virtual void PlumGraphicsDrawRectangle(RECT sRect);																						//PlumGraphics Draw Rectangle(GDI���ƾ���)
	virtual void PlumGraphicsDrawRectangle(RECT sRect, PlumPenType ePenType, int nWidth, COLORREF crColor);									//PlumGraphics Draw Rectangle(GDI���ƾ���)
	virtual void PlumGraphicsDrawRectangle(RECT sRect, PlumPenType ePenType, int nWidth, COLORREF crColorPen, COLORREF crColorBrush);		//PlumGraphics Draw Rectangle(GDI���ƾ���)
	virtual void PlumGraphicsDrawRectangleOnOriginal(int nX, int nY, int nWidth, int nHeight);												//PlumGraphics Draw Rectangle(GDI���ƾ���)
	virtual void PlumGraphicsDrawRectangleOnOriginal(RECT sRect);																			//PlumGraphics Draw Rectangle(GDI���ƾ���)
	virtual void PlumGraphicsDrawRectangleOnOriginal(RECT sRect, PlumPenType ePenType, int nPenWidth, COLORREF crPenColor);					//PlumGraphics Draw Rectangle(GDI���ƾ���)
	virtual void PlumGraphicsDrawRectangleOnOriginal(RECT sRect, PlumPenType ePenType, int nPenWidth, COLORREF crPenColor, COLORREF crBrushColor);	//PlumGraphics Draw Rectangle(GDI���ƾ���)

};


#endif