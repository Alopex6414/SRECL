/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		DirectGraphics.cpp
* @brief	This Program is DirectGraphics DLL Project.
* @author	Alopex/Helium
* @version	v1.29a
* @date		2017-11-2	v1.00a	alopex	Create Project.
* @date		2017-12-2	v1.01a	alopex	Add D3DXFont.
* @date		2017-12-8	v1.11a	alopex	Code Do Not Rely On MSVCR Library.
* @date		2018-01-10	v1.20a	alopex	Code Add dxerr & d3dcompiler Library and Modify Verify.
* @date		2018-01-10	v1.21a	alopex	Add Thread Safe File & Variable(DirectThreadSafe).
* @date		2018-02-11	v1.22a	alopex	Add D3D9 Lost Device Function.
* @date		2018-04-12	v1.23a	alopex	Add Macro Call Mode.
* @date		2018-06-16	v1.24a	alopex	Add StretchRect Function.
* @date		2018-06-17	v1.25a	alopex	Modify Reset Function.
* @date		2018-06-18	v1.26a	alopex	Modify D3D9 Clear Function(Background Color).
* @date		2018-06-21	v1.27a	alopex	Update Function D3DXFont Abort.
* @date		2018-06-23	v1.28a	alopex	Repair Bugs.
* @date		2018-06-23	v1.29a	alopex	Add Draw Function.
*/
#include "DirectCommon.h"
#include "DirectGraphics.h"
#include "DirectThreadSafe.h"

//------------------------------------------------------------------
// @Function:	 DirectGraphics()
// @Purpose: DirectGraphics构造函数
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
DirectGraphics::DirectGraphics()
{
	m_bThreadSafe = true;									//线程安全
	if (m_bThreadSafe) InitializeCriticalSection(&m_cs);	//初始化临界区

	m_nWidth = 0;			//IDirect3D9窗口宽度
	m_nHeight = 0;			//IDirect3D9窗口高度

	m_pD3D9 = NULL;			//IDirect3D9接口指针初始化(NULL)
	m_pD3D9Device = NULL;	//IDirect3DDevice9接口指针初始化(NULL)
	m_pD3DXFont = NULL;		//ID3DXFont接口指针初始化(NULL)
	ZeroMemory(&m_D3D9Caps, sizeof(m_D3D9Caps));	//清空m_D3D9Caps内存区域
	ZeroMemory(&m_D3D9pp, sizeof(m_D3D9pp));		//清空m_D3D9pp内存区域
	ZeroMemory(m_wcD3D9AdapterType, sizeof(wchar_t)*ADAPTERTYPESIZE);	//清空m_wcD3D9AdapterType内存区域
	ZeroMemory(m_wcD3D9BackFormat, sizeof(wchar_t)*D3D9FORMATSIZE);		//清空m_wcD3D9BackFormat内存区域
	ZeroMemory(m_wcD3D9AutoDepthStencilFormat, sizeof(wchar_t)*D3D9FORMATSIZE);	//清空m_wcD3D9AutoDepthStencilFormat内存区域
	ZeroMemory(m_wcD3D9ScreenInfo, sizeof(wchar_t)*D3D9FORMATSIZE);	//清空m_wcD3D9ScreenInfo内存区域
}

//------------------------------------------------------------------
// @Function:	 ~DirectGraphics()
// @Purpose: DirectGraphics析构函数
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
DirectGraphics::~DirectGraphics()
{
	SAFE_RELEASE(m_pD3DXFont);		//ID3DXFont接口释放
	SAFE_RELEASE(m_pD3D9Device);	//IDirect3D9接口指针释放
	SAFE_RELEASE(m_pD3D9);			//IDirect3DDevice9接口指针释放

	if (m_bThreadSafe) DeleteCriticalSection(&m_cs);	//删除临界区
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetObject(void) const
// @Purpose: DirectGraphics读取D3D9对象
// @Since: v1.00a
// @Para: None
// @Return: IDirect3D9*(D3D9对象指针)
//------------------------------------------------------------------
IDirect3D9* DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetObject(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_pD3D9;
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetDevice(void) const
// @Purpose: DirectGraphics读取D3D9设备对象
// @Since: v1.00a
// @Para: None
// @Return: IDirect3DDevice9*(D3D9设备对象指针)
//------------------------------------------------------------------
IDirect3DDevice9* DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetDevice(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_pD3D9Device;
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetCaps(void) const
// @Purpose: DirectGraphics读取D3D9设备型号
// @Since: v1.00a
// @Para: None
// @Return: D3DCAPS9*(D3D9设备型号指针)
//------------------------------------------------------------------
const D3DCAPS9* DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetCaps(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return (&m_D3D9Caps);
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetPresentParameters(void) const
// @Purpose: DirectGraphics读取D3D9设备参数
// @Since: v1.00a
// @Para: None
// @Return: D3DPRESENT_PARAMETERS*(D3D9设备参数指针)
//------------------------------------------------------------------
const D3DPRESENT_PARAMETERS* DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetPresentParameters(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return (&m_D3D9pp);
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetAdapterType(void) const
// @Purpose: DirectGraphics读取D3D9 GPU型号
// @Since: v1.00a
// @Para: None
// @Return: wchar_t*(宽字符数组地址)
//------------------------------------------------------------------
const wchar_t* DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetAdapterType(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_wcD3D9AdapterType;
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetBackBufferFormat(void) const
// @Purpose: DirectGraphics读取D3D9 后台缓冲格式
// @Since: v1.00a
// @Para: None
// @Return: wchar_t*(宽字符数组地址)
//------------------------------------------------------------------
const wchar_t* DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetBackBufferFormat(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_wcD3D9BackFormat;
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetAutoDepthStencilFormat(void) const
// @Purpose: DirectGraphics读取D3D9 深度模板缓冲格式
// @Since: v1.00a
// @Para: None
// @Return: wchar_t*(宽字符数组地址)
//------------------------------------------------------------------
const wchar_t* DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetAutoDepthStencilFormat(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_wcD3D9AutoDepthStencilFormat;
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetFont(void) const
// @Purpose: DirectGraphics读取D3D9 GPU字体
// @Since: v1.00a
// @Para: None
// @Return: ID3DXFont*(ID3DXFont类型指针)
//------------------------------------------------------------------
const ID3DXFont* DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetFont(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_pD3DXFont;
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetSufaceWidth(void) const
// @Purpose: DirectGraphics读取D3D9 设备表面宽度
// @Since: v1.00a
// @Para: None
// @Return: UINT
//------------------------------------------------------------------
const UINT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetSufaceWidth(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_nWidth;
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetSufaceHeight(void) const
// @Purpose: DirectGraphics读取D3D9 设备表面高度
// @Since: v1.00a
// @Para: None
// @Return: UINT
//------------------------------------------------------------------
const UINT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetSufaceHeight(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_nHeight;
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetD3D9AdapterType(void) const
// @Purpose: DirectGraphics读取D3D9 显卡型号字体
// @Since: v1.00a
// @Para: None
// @Return: UINT
//------------------------------------------------------------------
const wchar_t* DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetD3D9AdapterType(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_wcD3D9AdapterType;
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetD3D9BackFormat(void) const
// @Purpose: DirectGraphics读取D3D9 后台缓存类型格式
// @Since: v1.00a
// @Para: None
// @Return: UINT
//------------------------------------------------------------------
const wchar_t* DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetD3D9BackFormat(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_wcD3D9BackFormat;
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetD3D9AutoDepthStencilFormat(void) const
// @Purpose: DirectGraphics读取D3D9 深度模板缓存类型格式
// @Since: v1.00a
// @Para: None
// @Return: UINT
//------------------------------------------------------------------
const wchar_t* DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetD3D9AutoDepthStencilFormat(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_wcD3D9AutoDepthStencilFormat;
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetD3D9ScreenInfo(void) const
// @Purpose: DirectGraphics读取D3D9 屏幕分辨率信息
// @Since: v1.00a
// @Para: None
// @Return: UINT
//------------------------------------------------------------------
const wchar_t* DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetD3D9ScreenInfo(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_wcD3D9ScreenInfo;
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsTestCooperativeLevel(void) const
// @Purpose: DirectGraphics读取D3D9 当前状态
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(当前状态:正常:S_OK,错误:E_FAIL)
//			D3DERR_DEVICELOST:设备丢失(无法Reset)
//			D3DERR_DEVICENOTRESET:设备未Reset(可以Reset)
//------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsTestCooperativeLevel(void) const
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return (m_pD3D9Device->TestCooperativeLevel());
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsResetDevice(void) const
// @Purpose: DirectGraphics重置D3D9 初始设置
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(当前状态:正常:S_OK,错误:E_FAIL)
//------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsResetDevice(void)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return (m_pD3D9Device->Reset(&m_D3D9pp));
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsGetBackBuffer(IDirect3DSurface9* pD3D9BackBuffer)
// @Purpose: DirectGraphics获取D3D9 表面
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(当前状态:正常:S_OK,错误:E_FAIL)
//------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetBackBuffer(IDirect3DSurface9** ppD3D9BackBuffer)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return (m_pD3D9Device->GetBackBuffer(NULL, NULL, D3DBACKBUFFER_TYPE_MONO, ppD3D9BackBuffer));
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsResetFont(void)
// @Purpose: DirectGraphics 重置
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(重置状态:成功:S_OK,失败:E_FAIL)
//------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsResetFont(void)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return (m_pD3DXFont ? m_pD3DXFont->OnLostDevice() : S_OK);
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsReset(void)
// @Purpose: DirectGraphics 重置D3D9 DirectGraphics类
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(重置状态:成功:S_OK,失败:E_FAIL)
//------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsReset(void)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	IDirect3DSurface9* pD3D9BackBuffer = NULL;
	if (m_pD3DXFont) VERIFY(m_pD3DXFont->OnLostDevice());
	VERIFY(m_pD3D9Device->GetBackBuffer(NULL, NULL, D3DBACKBUFFER_TYPE_MONO, &pD3D9BackBuffer));
	SAFE_RELEASE(pD3D9BackBuffer);
	VERIFY(m_pD3D9Device->Reset(&m_D3D9pp));
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsCreateOffscreenPlainSurface(UINT nWidth, UINT nHeight, D3DFORMAT D3DFormat, D3DPOOL D3DPool, IDirect3DSurface9**& ppD3D9Surface)
// @Purpose: DirectGraphics 创建离屏表面
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(状态:成功:S_OK,失败:E_FAIL)
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsCreateOffscreenPlainSurface(UINT nWidth, UINT nHeight, D3DFORMAT D3DFormat, D3DPOOL D3DPool, IDirect3DSurface9** ppD3D9Surface)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_pD3D9Device->CreateOffscreenPlainSurface(nWidth, nHeight, D3DFormat, D3DPool, ppD3D9Surface, NULL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsCreateOffscreenPlainSurface(UINT nWidth, UINT nHeight, IDirect3DSurface9**& ppD3D9Surface)
// @Purpose: DirectGraphics 创建离屏表面
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(状态:成功:S_OK,失败:E_FAIL)
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsCreateOffscreenPlainSurface(UINT nWidth, UINT nHeight, IDirect3DSurface9 ** ppD3D9Surface)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_pD3D9Device->CreateOffscreenPlainSurface(nWidth, nHeight, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, ppD3D9Surface, NULL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsCreateOffscreenPlainSurface(UINT nWidth, UINT nHeight, IDirect3DSurface9**& ppD3D9Surface)
// @Purpose: DirectGraphics 创建离屏表面
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(状态:成功:S_OK,失败:E_FAIL)
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsCreateOffscreenPlainSurface(IDirect3DSurface9 ** ppD3D9Surface)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_pD3D9Device->CreateOffscreenPlainSurface(USER_SCREENWIDTH, USER_SCREENHEIGHT, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, ppD3D9Surface, NULL);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsStretchRect(IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestSurface, const RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
// @Purpose: DirectGraphics 绘制表面
// @Since: v1.00a
// @Para: IDirect3DSurface9* pSourceSurface(源表面地址)
// @Para: const RECT* pSourceRect(源表面区域)
// @Para: IDirect3DSurface9* pDestSurface(目标表面地址)
// @Para: const RECT* pDestRect(目标表面区域)
// @Para: D3DTEXTUREFILTERTYPE Filter(D3D纹理滤波类型)
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsStretchRect(IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestSurface, const RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_pD3D9Device->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsStretchRect(IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestSurface, const RECT* pDestRect)
// @Purpose: DirectGraphics 绘制表面
// @Since: v1.00a
// @Para: IDirect3DSurface9* pSourceSurface(源表面地址)
// @Para: const RECT* pSourceRect(源表面区域)
// @Para: IDirect3DSurface9* pDestSurface(目标表面地址)
// @Para: const RECT* pDestRect(目标表面区域)
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsStretchRect(IDirect3DSurface9 * pSourceSurface, const RECT * pSourceRect, IDirect3DSurface9 * pDestSurface, const RECT * pDestRect)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_pD3D9Device->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, D3DTEXF_NONE);
}

//------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsStretchRect(IDirect3DSurface9* pSourceSurface, IDirect3DSurface9* pDestSurface)
// @Purpose: DirectGraphics 绘制表面
// @Since: v1.00a
// @Para: IDirect3DSurface9* pSourceSurface(源表面地址)
// @Para: IDirect3DSurface9* pDestSurface(目标表面地址)
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//------------------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsStretchRect(IDirect3DSurface9 * pSourceSurface, IDirect3DSurface9 * pDestSurface)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	return m_pD3D9Device->StretchRect(pSourceSurface, NULL, pDestSurface, NULL, D3DTEXF_NONE);
}

//----------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsStretchRect(IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect)
// @Purpose: DirectGraphics 绘制表面
// @Since: v1.00a
// @Para: IDirect3DSurface9* pSourceSurface(源表面地址)
// @Para: const RECT* pSourceRect(源表面区域)
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsStretchRect(IDirect3DSurface9 * pSourceSurface, const RECT * pSourceRect)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	IDirect3DSurface9* pD3D9BackBuffer = NULL;
	HRESULT hr;

	m_pD3D9Device->GetBackBuffer(NULL, NULL, D3DBACKBUFFER_TYPE_MONO, &pD3D9BackBuffer);
	hr = m_pD3D9Device->StretchRect(pSourceSurface, pSourceRect, pD3D9BackBuffer, NULL, D3DTEXF_NONE);
	SAFE_RELEASE(pD3D9BackBuffer);

	return hr;
}

//---------------------------------------------------------------------------
// @Function:	 DirectGraphicsStretchRect(IDirect3DSurface9* pSourceSurface)
// @Purpose: DirectGraphics 绘制表面
// @Since: v1.00a
// @Para: IDirect3DSurface9* pSourceSurface(源表面地址)
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsStretchRect(IDirect3DSurface9 * pSourceSurface)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	IDirect3DSurface9* pD3D9BackBuffer = NULL;
	HRESULT hr;

	m_pD3D9Device->GetBackBuffer(NULL, NULL, D3DBACKBUFFER_TYPE_MONO, &pD3D9BackBuffer);
	hr = m_pD3D9Device->StretchRect(pSourceSurface, NULL, pD3D9BackBuffer, NULL, D3DTEXF_NONE);
	SAFE_RELEASE(pD3D9BackBuffer);

	return hr;
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsInit(HWND hWnd)
// @Purpose: DirectGraphics 初始化
// @Since: v1.00a
// @Para: HWND hWnd(窗口句柄)
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsInit(HWND hWnd)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	int nSize;
	int Vp;
	D3DADAPTER_IDENTIFIER9 D3D9Adapter;

	//读取Direct3D9接口对象指针
	m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);//创建Direct3D9接口对象

	//读取D3DCAPS9校验硬件顶点运算
	VERIFY(m_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3D9Caps));//校验硬件顶点运算能力

	if (m_D3D9Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		Vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;//支持硬件顶点运算(硬件顶点运算)
	}
	else
	{
		Vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//不支持硬件顶点运算(软件顶点运算)
	}

	//填充D3DPRESENT_PARAMETERS结构
	m_D3D9pp.BackBufferWidth = USER_SCREENWIDTH;//后台缓冲表面宽度(Pixel)
	m_D3D9pp.BackBufferHeight = USER_SCREENHEIGHT;//后台缓冲表面高度(Pixel)
	m_D3D9pp.BackBufferFormat = D3DFMT_A8R8G8B8;//后台缓冲像素格式
	m_D3D9pp.BackBufferCount = 1;//后台缓冲数量(1)
	m_D3D9pp.MultiSampleType = D3DMULTISAMPLE_NONE;//后台缓冲多重采样类型
	m_D3D9pp.MultiSampleQuality = 0;//后台缓冲多重采样质量
	m_D3D9pp.SwapEffect = D3DSWAPEFFECT_DISCARD;//交换链页面置换方式
	m_D3D9pp.hDeviceWindow = hWnd;//设备相关窗口句柄
	m_D3D9pp.Windowed = true;//窗口模式:true/全屏模式:false
	m_D3D9pp.EnableAutoDepthStencil = true;//Direct3D自动创建维护深度缓冲和模板缓冲
	m_D3D9pp.AutoDepthStencilFormat = D3DFMT_D24S8;//深度缓冲和模板缓冲像素格式
	m_D3D9pp.Flags = 0;//无标记格式
	m_D3D9pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//Direct3D默认刷新频率
	m_D3D9pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//Direct3D提交频率(默认提交)

	//创建IDirect3DDevice9接口对象指针
	VERIFY(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, Vp, &m_D3D9pp, &m_pD3D9Device));//创建IDirect3DDevice9接口对象指针

	//读取窗口尺寸
	m_nWidth = m_D3D9pp.BackBufferWidth;
	m_nHeight = m_D3D9pp.BackBufferHeight;

	//读取GPU型号
	m_pD3D9->GetAdapterIdentifier(0, 0, &D3D9Adapter);	//读取GPU信息
	nSize = MultiByteToWideChar(CP_ACP, 0, D3D9Adapter.Description, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, D3D9Adapter.Description, -1, m_wcD3D9AdapterType, nSize);

	//读取后台缓冲像素格式
	DirectGraphicsGetD3D9Format(m_D3D9pp.BackBufferFormat, m_wcD3D9BackFormat);

	//读取深度缓存和模板缓冲像素格式
	DirectGraphicsGetD3D9Format(m_D3D9pp.AutoDepthStencilFormat, m_wcD3D9AutoDepthStencilFormat);

	//读取屏幕分辨率信息
	DirectGraphicsGetD3D9Screen(m_nWidth, m_nHeight, m_wcD3D9ScreenInfo);

	return S_OK;//OK
}

//------------------------------------------------------------------
// @Function:	 DirectGraphicsInit(HWND hWnd, bool bIsWindowed)
// @Purpose: DirectGraphics 初始化
// @Since: v1.00a
// @Para: HWND hWnd(窗口句柄)
// @Para: bool bIsWindowed(是否以窗口模式运行)
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsInit(HWND hWnd, bool bIsWindowed)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	int nSize;
	int Vp;
	D3DADAPTER_IDENTIFIER9 D3D9Adapter;

	//读取Direct3D9接口对象指针
	m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);//创建Direct3D9接口对象

	//读取D3DCAPS9校验硬件顶点运算
	VERIFY(m_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3D9Caps));//校验硬件顶点运算能力

	if (m_D3D9Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		Vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;//支持硬件顶点运算(硬件顶点运算)
	}
	else
	{
		Vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//不支持硬件顶点运算(软件顶点运算)
	}

	//填充D3DPRESENT_PARAMETERS结构
	m_D3D9pp.BackBufferWidth = USER_SCREENWIDTH;//后台缓冲表面宽度(Pixel)
	m_D3D9pp.BackBufferHeight = USER_SCREENHEIGHT;//后台缓冲表面高度(Pixel)
	m_D3D9pp.BackBufferFormat = D3DFMT_A8R8G8B8;//后台缓冲像素格式
	m_D3D9pp.BackBufferCount = 1;//后台缓冲数量(1)
	m_D3D9pp.MultiSampleType = D3DMULTISAMPLE_NONE;//后台缓冲多重采样类型
	m_D3D9pp.MultiSampleQuality = 0;//后台缓冲多重采样质量
	m_D3D9pp.SwapEffect = D3DSWAPEFFECT_DISCARD;//交换链页面置换方式
	m_D3D9pp.hDeviceWindow = hWnd;//设备相关窗口句柄
	m_D3D9pp.Windowed = bIsWindowed;//窗口模式:true/全屏模式:false
	m_D3D9pp.EnableAutoDepthStencil = true;//Direct3D自动创建维护深度缓冲和模板缓冲
	m_D3D9pp.AutoDepthStencilFormat = D3DFMT_D24S8;//深度缓冲和模板缓冲像素格式
	m_D3D9pp.Flags = 0;//无标记格式
	m_D3D9pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//Direct3D默认刷新频率
	m_D3D9pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//Direct3D提交频率(默认提交)

	//创建IDirect3DDevice9接口对象指针
	VERIFY(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, Vp, &m_D3D9pp, &m_pD3D9Device));//创建IDirect3DDevice9接口对象指针

	//读取窗口尺寸
	m_nWidth = m_D3D9pp.BackBufferWidth;
	m_nHeight = m_D3D9pp.BackBufferHeight;

	//读取GPU型号
	m_pD3D9->GetAdapterIdentifier(0, 0, &D3D9Adapter);	//读取GPU信息
	nSize = MultiByteToWideChar(CP_ACP, 0, D3D9Adapter.Description, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, D3D9Adapter.Description, -1, m_wcD3D9AdapterType, nSize);

	//读取后台缓冲像素格式
	DirectGraphicsGetD3D9Format(m_D3D9pp.BackBufferFormat, m_wcD3D9BackFormat);

	//读取深度缓存和模板缓冲像素格式
	DirectGraphicsGetD3D9Format(m_D3D9pp.AutoDepthStencilFormat, m_wcD3D9AutoDepthStencilFormat);

	//读取屏幕分辨率信息
	DirectGraphicsGetD3D9Screen(m_nWidth, m_nHeight, m_wcD3D9ScreenInfo);

	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsInit(HWND hWnd, bool bIsWindowed, int nScreenWidth, int nScreenHeight)
// @Purpose: DirectGraphics 初始化
// @Since: v1.00a
// @Para: HWND hWnd(窗口句柄)
// @Para: bool bIsWindowed(是否以窗口模式运行)
// @Para: int nScreenWidth(屏幕宽度)
// @Para: int nScreenHeight(屏幕高度)
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsInit(HWND hWnd, bool bIsWindowed, int nScreenWidth, int nScreenHeight)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	int nSize;
	int Vp;
	D3DADAPTER_IDENTIFIER9 D3D9Adapter;

	//读取Direct3D9接口对象指针
	m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);//创建Direct3D9接口对象

	//读取D3DCAPS9校验硬件顶点运算
	VERIFY(m_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3D9Caps));//校验硬件顶点运算能力

	if (m_D3D9Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		Vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;//支持硬件顶点运算(硬件顶点运算)
	}
	else
	{
		Vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//不支持硬件顶点运算(软件顶点运算)
	}

	//填充D3DPRESENT_PARAMETERS结构
	m_D3D9pp.BackBufferWidth = nScreenWidth;//后台缓冲表面宽度(Pixel)
	m_D3D9pp.BackBufferHeight = nScreenHeight;//后台缓冲表面高度(Pixel)
	m_D3D9pp.BackBufferFormat = D3DFMT_A8R8G8B8;//后台缓冲像素格式
	m_D3D9pp.BackBufferCount = 1;//后台缓冲数量(1)
	m_D3D9pp.MultiSampleType = D3DMULTISAMPLE_NONE;//后台缓冲多重采样类型
	m_D3D9pp.MultiSampleQuality = 0;//后台缓冲多重采样质量
	m_D3D9pp.SwapEffect = D3DSWAPEFFECT_DISCARD;//交换链页面置换方式
	m_D3D9pp.hDeviceWindow = hWnd;//设备相关窗口句柄
	m_D3D9pp.Windowed = bIsWindowed;//窗口模式:true/全屏模式:false
	m_D3D9pp.EnableAutoDepthStencil = true;//Direct3D自动创建维护深度缓冲和模板缓冲
	m_D3D9pp.AutoDepthStencilFormat = D3DFMT_D24S8;//深度缓冲和模板缓冲像素格式
	m_D3D9pp.Flags = 0;//无标记格式
	m_D3D9pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//Direct3D默认刷新频率
	m_D3D9pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//Direct3D提交频率(默认提交)

	//创建IDirect3DDevice9接口对象指针
	VERIFY(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, Vp, &m_D3D9pp, &m_pD3D9Device));//创建IDirect3DDevice9接口对象指针

	//读取窗口尺寸
	m_nWidth = m_D3D9pp.BackBufferWidth;
	m_nHeight = m_D3D9pp.BackBufferHeight;

	//读取GPU型号
	m_pD3D9->GetAdapterIdentifier(0, 0, &D3D9Adapter);	//读取GPU信息
	nSize = MultiByteToWideChar(CP_ACP, 0, D3D9Adapter.Description, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, D3D9Adapter.Description, -1, m_wcD3D9AdapterType, nSize);

	//读取后台缓冲像素格式
	DirectGraphicsGetD3D9Format(m_D3D9pp.BackBufferFormat, m_wcD3D9BackFormat);

	//读取深度缓存和模板缓冲像素格式
	DirectGraphicsGetD3D9Format(m_D3D9pp.AutoDepthStencilFormat, m_wcD3D9AutoDepthStencilFormat);

	//读取屏幕分辨率信息
	DirectGraphicsGetD3D9Screen(m_nWidth, m_nHeight, m_wcD3D9ScreenInfo);

	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsInit(D3DPRESENT_PARAMETERS D3D9pp)
// @Purpose: DirectGraphics 初始化
// @Since: v1.00a
// @Para: D3DPRESENT_PARAMETERS D3D9pp(D3D9参数)
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsInit(D3DPRESENT_PARAMETERS D3D9pp)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	int nSize;
	int Vp;
	D3DADAPTER_IDENTIFIER9 D3D9Adapter;

	//读取Direct3D9接口对象指针
	m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);//创建Direct3D9接口对象

	//读取D3DCAPS9校验硬件顶点运算
	VERIFY(m_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3D9Caps));//校验硬件顶点运算能力

	if (m_D3D9Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		Vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;//支持硬件顶点运算(硬件顶点运算)
	}
	else
	{
		Vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//不支持硬件顶点运算(软件顶点运算)
	}

	//填充D3DPRESENT_PARAMETERS结构
	m_D3D9pp.BackBufferWidth = D3D9pp.BackBufferWidth;//后台缓冲表面宽度(Pixel)
	m_D3D9pp.BackBufferHeight = D3D9pp.BackBufferHeight;//后台缓冲表面高度(Pixel)
	m_D3D9pp.BackBufferFormat = D3D9pp.BackBufferFormat;//后台缓冲像素格式
	m_D3D9pp.BackBufferCount = D3D9pp.BackBufferCount;//后台缓冲数量(1)
	m_D3D9pp.MultiSampleType = D3D9pp.MultiSampleType;//后台缓冲多重采样类型
	m_D3D9pp.MultiSampleQuality = D3D9pp.MultiSampleQuality;//后台缓冲多重采样质量
	m_D3D9pp.SwapEffect = D3D9pp.SwapEffect;//交换链页面置换方式
	m_D3D9pp.hDeviceWindow = D3D9pp.hDeviceWindow;//设备相关窗口句柄
	m_D3D9pp.Windowed = D3D9pp.Windowed;//窗口模式:true/全屏模式:false
	m_D3D9pp.EnableAutoDepthStencil = D3D9pp.EnableAutoDepthStencil;//Direct3D自动创建维护深度缓冲和模板缓冲
	m_D3D9pp.AutoDepthStencilFormat = D3D9pp.AutoDepthStencilFormat;//深度缓冲和模板缓冲像素格式
	m_D3D9pp.Flags = D3D9pp.Flags;//无标记格式
	m_D3D9pp.FullScreen_RefreshRateInHz = D3D9pp.FullScreen_RefreshRateInHz;//Direct3D默认刷新频率
	m_D3D9pp.PresentationInterval = D3D9pp.PresentationInterval;//Direct3D提交频率(默认提交)

	//创建IDirect3DDevice9接口对象指针
	VERIFY(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3D9pp.hDeviceWindow, Vp, &m_D3D9pp, &m_pD3D9Device));//创建IDirect3DDevice9接口对象指针

	//读取窗口尺寸
	m_nWidth = m_D3D9pp.BackBufferWidth;
	m_nHeight = m_D3D9pp.BackBufferHeight;

	//读取GPU型号
	m_pD3D9->GetAdapterIdentifier(0, 0, &D3D9Adapter);	//读取GPU信息
	nSize = MultiByteToWideChar(CP_ACP, 0, D3D9Adapter.Description, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, D3D9Adapter.Description, -1, m_wcD3D9AdapterType, nSize);

	//读取后台缓冲像素格式
	DirectGraphicsGetD3D9Format(m_D3D9pp.BackBufferFormat, m_wcD3D9BackFormat);

	//读取深度缓存和模板缓冲像素格式
	DirectGraphicsGetD3D9Format(m_D3D9pp.AutoDepthStencilFormat, m_wcD3D9AutoDepthStencilFormat);

	//读取屏幕分辨率信息
	DirectGraphicsGetD3D9Screen(m_nWidth, m_nHeight, m_wcD3D9ScreenInfo);

	return S_OK;//OK
}
//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsBeginScene(void)
// @Purpose: DirectGraphics 开始渲染
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsBeginScene(void)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	VERIFY(m_pD3D9Device->BeginScene());	//开始渲染
	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsEndScene(void)
// @Purpose: DirectGraphics 结束渲染
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsEndScene(void)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	VERIFY(m_pD3D9Device->EndScene());		//结束渲染
	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsBegin(void)
// @Purpose: DirectGraphics 开始渲染
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsBegin(void)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	VERIFY(m_pD3D9Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));		//清空图像
	VERIFY(m_pD3D9Device->BeginScene());	//开始渲染
	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsBegin(void)
// @Purpose: DirectGraphics 开始渲染
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsBegin(D3DCOLOR Color)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	VERIFY(m_pD3D9Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Color, 1.0f, 0));		//清空图像
	VERIFY(m_pD3D9Device->BeginScene());	//开始渲染
	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsEnd(void)
// @Purpose: DirectGraphics 结束渲染
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsEnd(void)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	VERIFY(m_pD3D9Device->EndScene());		//结束渲染
	VERIFY(m_pD3D9Device->Present(NULL, NULL, NULL, NULL));		//提交渲染(显示)
	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsPresent(void)
// @Purpose: DirectGraphics 提交渲染(显示)
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsPresent(void)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	VERIFY(m_pD3D9Device->Present(NULL,NULL,NULL,NULL));		//提交渲染(显示)
	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsClear(void)
// @Purpose: DirectGraphics 清空图像
// @Since: v1.00a
// @Para: None
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsClear(void)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	VERIFY(m_pD3D9Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));		//清空图像
	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsClear(DWORD dwColor)
// @Purpose: DirectGraphics 清空图像
// @Since: v1.00a
// @Para: D3DCOLOR Color(背景颜色)
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsClear(D3DCOLOR Color)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	VERIFY(m_pD3D9Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Color, 1.0f, 0));		//清空图像
	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsFontInit(void)
// @Purpose: DirectGraphics 字体初始化
// @Since: v1.01a
// @Para: None
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsFontInit(void)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	VERIFY(D3DXCreateFont(m_pD3D9Device, 20, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Consolas"), &m_pD3DXFont));
	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsFontInit(int nFontSize)
// @Purpose: DirectGraphics 字体初始化
// @Since: v1.01a
// @Para: int nFontSize		//字体大小
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsFontInit(int Height)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	VERIFY(D3DXCreateFont(m_pD3D9Device, Height, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Consolas"), &m_pD3DXFont));
	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsFontInit(int nFontSize, LPWSTR lpszFontType)
// @Purpose: DirectGraphics 字体初始化
// @Since: v1.01a
// @Para: int nFontSize			//字体大小
// @Para: LPWSTR lpszFontType	//字体类型
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//---------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsFontInit(int Height, LPWSTR lpszFontType)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	VERIFY(D3DXCreateFont(m_pD3D9Device, Height, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, lpszFontType, &m_pD3DXFont));
	return S_OK;//OK
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsFontInit(INT Height, UINT Width, UINT Weight, UINT MipLevels, BOOL Italic, DWORD CharSet, DWORD OutputPrecision, DWORD Quality, DWORD PitchAndFamily, LPCWSTR pFaceName)
// @Purpose: DirectGraphics 字体初始化
// @Since: v1.01a
// @Para: INT Height
// @Para: UINT Width
// @Para: UINT Weight
// @Para: UINT MipLevels
// @Para: BOOL Italic
// @Para: DWORD CharSet
// @Para: DWORD OutputPrecision
// @Para: DWORD Quality
// @Para: DWORD PitchAndFamily
// @Para: LPCWSTR pFaceName
// @Return: HRESULT(初始化状态:成功:S_OK,失败:E_FAIL)
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsFontInit(INT Height, UINT Width, UINT Weight, UINT MipLevels, BOOL Italic, DWORD CharSet, DWORD OutputPrecision, DWORD Quality, DWORD PitchAndFamily, LPCWSTR pFaceName)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	VERIFY(D3DXCreateFont(m_pD3D9Device, Height, Width, Weight, MipLevels, Italic, CharSet, OutputPrecision, Quality, PitchAndFamily, pFaceName, &m_pD3DXFont));
	return S_OK;//OK
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsFontDrawText(HWND hWnd)
// @Purpose: DirectGraphics 绘制HAL信息
// @Since: v1.01a
// @Para: HWND hWnd		//窗口句柄
// @Return: None
//---------------------------------------------------------------------------------------------------
void DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsFontDrawText(HWND hWnd)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	RECT Rect;

	GetClientRect(hWnd, &Rect);
	m_pD3DXFont->DrawText(NULL, m_wcD3D9AdapterType, -1, &Rect, DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));
}

//---------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsFontDrawText(D3DXCOLOR dwColor)
// @Purpose: DirectGraphics 绘制HAL信息
// @Since: v1.01a
// @Para: HWND hWnd				//窗口句柄
// @Para: D3DXCOLOR dwColor		//字体颜色
// @Return: None
//---------------------------------------------------------------------------------------------------
void DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsFontDrawText(HWND hWnd, D3DXCOLOR dwColor)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	RECT Rect;

	GetClientRect(hWnd, &Rect);
	m_pD3DXFont->DrawText(NULL, m_wcD3D9AdapterType, -1, &Rect, DT_TOP | DT_LEFT, dwColor);
}

//-----------------------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsFontDrawTextW(LPCWSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color)
// @Purpose: DirectGraphics 绘制HAL信息
// @Since: v1.01a
// @Para: LPCWSTR pString		//字符数组(Unicode)
// @Para: INT Count				//数组长度(-1)
// @Para: LPRECT pRect			//绘制区域
// @Para: DWORD Format			//绘制格式
// @Para: D3DCOLOR Color		//绘制颜色
// @Return: None
//-----------------------------------------------------------------------------------------------------------------------------
void DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsFontDrawTextW(LPCWSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	m_pD3DXFont->DrawTextW(NULL, pString, Count, pRect, Format, Color);
}

//-----------------------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsFontDrawTextA(LPCSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color)
// @Purpose: DirectGraphics 绘制HAL信息
// @Since: v1.01a
// @Para: LPCSTR pString		//字符数组(ASCII)
// @Para: INT Count				//数组长度(-1)
// @Para: LPRECT pRect			//绘制区域
// @Para: DWORD Format			//绘制格式
// @Para: D3DCOLOR Color		//绘制颜色
// @Return: None
//-----------------------------------------------------------------------------------------------------------------------------
void DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsFontDrawTextA(LPCSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	m_pD3DXFont->DrawTextA(NULL, pString, Count, pRect, Format, Color);
}

//-----------------------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsFontDrawTextA(LPCSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color)
// @Purpose: DirectGraphics 绘制HAL信息
// @Since: v1.01a
// @Para: LPCSTR pString		//字符数组(ASCII)
// @Para: INT Count				//数组长度(-1)
// @Para: LPRECT pRect			//绘制区域
// @Para: DWORD Format			//绘制格式
// @Para: D3DCOLOR Color		//绘制颜色
// @Return: None
//-----------------------------------------------------------------------------------------------------------------------------
void DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsFontDrawTextAdapterType(LPRECT pRect, DWORD Format, D3DCOLOR Color)
{
	DirectGraphicsFontDrawTextW(m_wcD3D9AdapterType, -1, pRect, Format, Color);
}

//-----------------------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsFontDrawTextA(LPCSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color)
// @Purpose: DirectGraphics 绘制HAL信息
// @Since: v1.01a
// @Para: LPCSTR pString		//字符数组(ASCII)
// @Para: INT Count				//数组长度(-1)
// @Para: LPRECT pRect			//绘制区域
// @Para: DWORD Format			//绘制格式
// @Para: D3DCOLOR Color		//绘制颜色
// @Return: None
//-----------------------------------------------------------------------------------------------------------------------------
void DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsFontDrawTextFormat(LPRECT pRect, DWORD Format, D3DCOLOR Color)
{
	wchar_t wcFormatArray[MAX_PATH] = { 0 };

	swprintf_s(wcFormatArray, L"%s (%s)", m_wcD3D9BackFormat, m_wcD3D9AutoDepthStencilFormat);
	DirectGraphicsFontDrawTextW(wcFormatArray, -1, pRect, Format, Color);
}

//-----------------------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsFontDrawTextA(LPCSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color)
// @Purpose: DirectGraphics 绘制HAL信息
// @Since: v1.01a
// @Para: LPCSTR pString		//字符数组(ASCII)
// @Para: INT Count				//数组长度(-1)
// @Para: LPRECT pRect			//绘制区域
// @Para: DWORD Format			//绘制格式
// @Para: D3DCOLOR Color		//绘制颜色
// @Return: None
//-----------------------------------------------------------------------------------------------------------------------------
void DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsFontDrawTextScreen(LPRECT pRect, DWORD Format, D3DCOLOR Color)
{
	DirectGraphicsFontDrawTextW(m_wcD3D9ScreenInfo, -1, pRect, Format, Color);
}

//-----------------------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsGetD3D9Screen(UINT nWidth, UINT nHeight, LPWSTR pString)
// @Purpose: DirectGraphics 获取后台缓冲型号
// @Since: v1.01a
// @Para: UINT nWidth			//屏幕宽度
// @Para: UINT nHeight			//屏幕高度
// @Para: LPCSTR pString		//字符数组(Uincode)
// @Return: None
//-----------------------------------------------------------------------------------------------------------------------------
void DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetD3D9Screen(UINT nWidth, UINT nHeight, LPWSTR pString)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	CHAR chString[MAX_PATH] = { 0 };
	INT nSize = 0;

	sprintf_s(chString, "D3D9 Vsync on (%dx%d)", nWidth, nHeight);

	nSize = MultiByteToWideChar(CP_ACP, 0, chString, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, chString, -1, pString, nSize);
}

//-----------------------------------------------------------------------------------------------------------------------------
// @Function:	 DirectGraphicsGetD3D9Format(LPCWSTR pString, UINT nSize)
// @Purpose: DirectGraphics 获取后台缓冲型号
// @Since: v1.01a
// @Para: D3DFORMAT Format		//D3D9格式
// @Para: LPCSTR pString		//字符数组(Uincode)
// @Return: None
//-----------------------------------------------------------------------------------------------------------------------------
void DIRECTGRAPHICS_CALLMODE DirectGraphics::DirectGraphicsGetD3D9Format(D3DFORMAT Format, LPWSTR pString)
{
	DirectThreadSafe ThreadSafe(&m_cs, m_bThreadSafe);
	CHAR chString[MAX_PATH] = { 0 };
	INT nSize = 0;

	switch (Format)
	{
	case D3DFMT_UNKNOWN:
		strcpy_s(chString, "D3DFMT_UNKNOWN");
		break;
	case D3DFMT_R8G8B8:
		strcpy_s(chString, "D3DFMT_R8G8B8");
		break;
	case D3DFMT_A8R8G8B8:
		strcpy_s(chString, "D3DFMT_A8R8G8B8");
		break;
	case D3DFMT_X8R8G8B8:
		strcpy_s(chString, "D3DFMT_A8R8G8B8");
		break;
	case D3DFMT_R5G6B5:
		strcpy_s(chString, "D3DFMT_R5G6B5");
		break;
	case D3DFMT_X1R5G5B5:
		strcpy_s(chString, "D3DFMT_X1R5G5B5");
		break;
	case D3DFMT_A1R5G5B5:
		strcpy_s(chString, "D3DFMT_A1R5G5B5");
		break;
	case D3DFMT_A4R4G4B4:
		strcpy_s(chString, "D3DFMT_A4R4G4B4");
		break;
	case D3DFMT_R3G3B2:
		strcpy_s(chString, "D3DFMT_R3G3B2");
		break;
	case D3DFMT_A8:
		strcpy_s(chString, "D3DFMT_A8");
		break;
	case D3DFMT_A8R3G3B2:
		strcpy_s(chString, "D3DFMT_A8R3G3B2");
		break;
	case D3DFMT_X4R4G4B4:
		strcpy_s(chString, "D3DFMT_X4R4G4B4");
		break;
	case D3DFMT_A2B10G10R10:
		strcpy_s(chString, "D3DFMT_A2B10G10R10");
		break;
	case D3DFMT_A8B8G8R8:
		strcpy_s(chString, "D3DFMT_A8B8G8R8");
		break;
	case D3DFMT_X8B8G8R8:
		strcpy_s(chString, "D3DFMT_X8B8G8R8");
		break;
	case D3DFMT_G16R16:
		strcpy_s(chString, "D3DFMT_G16R16");
		break;
	case D3DFMT_A2R10G10B10:
		strcpy_s(chString, "D3DFMT_A2R10G10B10");
		break;
	case D3DFMT_A16B16G16R16:
		strcpy_s(chString, "D3DFMT_A16B16G16R16");
		break;
	case D3DFMT_A8P8:
		strcpy_s(chString, "D3DFMT_A8P8");
		break;
	case D3DFMT_P8:
		strcpy_s(chString, "D3DFMT_P8");
		break;
	case D3DFMT_L8:
		strcpy_s(chString, "D3DFMT_L8");
		break;
	case D3DFMT_A8L8:
		strcpy_s(chString, "D3DFMT_A8L8");
		break;
	case D3DFMT_A4L4:
		strcpy_s(chString, "D3DFMT_A4L4");
		break;
	case D3DFMT_V8U8:
		strcpy_s(chString, "D3DFMT_V8U8");
		break;
	case D3DFMT_L6V5U5:
		strcpy_s(chString, "D3DFMT_L6V5U5");
		break;
	case D3DFMT_X8L8V8U8:
		strcpy_s(chString, "D3DFMT_X8L8V8U8");
		break;
	case D3DFMT_Q8W8V8U8:
		strcpy_s(chString, "D3DFMT_Q8W8V8U8");
		break;
	case D3DFMT_V16U16:
		strcpy_s(chString, "D3DFMT_V16U16");
		break;
	case D3DFMT_A2W10V10U10:
		strcpy_s(chString, "D3DFMT_A2W10V10U10");
		break;
	case D3DFMT_UYVY:
		strcpy_s(chString, "D3DFMT_UYVY");
		break;
	case D3DFMT_R8G8_B8G8:
		strcpy_s(chString, "D3DFMT_R8G8_B8G8");
		break;
	case D3DFMT_YUY2:
		strcpy_s(chString, "D3DFMT_YUY2");
		break;
	case D3DFMT_G8R8_G8B8:
		strcpy_s(chString, "D3DFMT_G8R8_G8B8");
		break;
	case D3DFMT_DXT1:
		strcpy_s(chString, "D3DFMT_DXT1");
		break;
	case D3DFMT_DXT2:
		strcpy_s(chString, "D3DFMT_DXT2");
		break;
	case D3DFMT_DXT3:
		strcpy_s(chString, "D3DFMT_DXT3");
		break;
	case D3DFMT_DXT4:
		strcpy_s(chString, "D3DFMT_DXT4");
		break;
	case D3DFMT_DXT5:
		strcpy_s(chString, "D3DFMT_DXT5");
		break;
	case D3DFMT_D16_LOCKABLE:
		strcpy_s(chString, "D3DFMT_D16_LOCKABLE");
		break;
	case D3DFMT_D32:
		strcpy_s(chString, "D3DFMT_D32");
		break;
	case D3DFMT_D15S1:
		strcpy_s(chString, "D3DFMT_D15S1");
		break;
	case D3DFMT_D24S8:
		strcpy_s(chString, "D3DFMT_D24S8");
		break;
	case D3DFMT_D24X8:
		strcpy_s(chString, "D3DFMT_D24X8");
		break;
	case D3DFMT_D24X4S4:
		strcpy_s(chString, "D3DFMT_D24X4S4");
		break;
	case D3DFMT_D16:
		strcpy_s(chString, "D3DFMT_D16");
		break;
	case D3DFMT_D32F_LOCKABLE:
		strcpy_s(chString, "D3DFMT_D32F_LOCKABLE");
		break;
	case D3DFMT_D24FS8:
		strcpy_s(chString, "D3DFMT_D24FS8");
		break;
/* D3D9Ex only -- */
#if !defined(D3D_DISABLE_9EX)
	case D3DFMT_D32_LOCKABLE:
		strcpy_s(chString, "D3DFMT_D32_LOCKABLE");
		break;
	case D3DFMT_S8_LOCKABLE:
		strcpy_s(chString, "D3DFMT_S8_LOCKABLE");
		break;
#endif // !D3D_DISABLE_9EX
	case D3DFMT_L16:
		strcpy_s(chString, "D3DFMT_L16");
		break;
	case D3DFMT_VERTEXDATA:
		strcpy_s(chString, "D3DFMT_VERTEXDATA");
		break;
	case D3DFMT_INDEX16:
		strcpy_s(chString, "D3DFMT_INDEX16");
		break;
	case D3DFMT_INDEX32:
		strcpy_s(chString, "D3DFMT_INDEX32");
		break;
	case D3DFMT_Q16W16V16U16:
		strcpy_s(chString, "D3DFMT_Q16W16V16U16");
		break;
	case D3DFMT_MULTI2_ARGB8:
		strcpy_s(chString, "D3DFMT_MULTI2_ARGB8");
		break;
	case D3DFMT_R16F:
		strcpy_s(chString, "D3DFMT_R16F");
		break;
	case D3DFMT_G16R16F:
		strcpy_s(chString, "D3DFMT_G16R16F");
		break;
	case D3DFMT_A16B16G16R16F:
		strcpy_s(chString, "D3DFMT_A16B16G16R16F");
		break;
	case D3DFMT_R32F:
		strcpy_s(chString, "D3DFMT_R32F");
		break;
	case D3DFMT_G32R32F:
		strcpy_s(chString, "D3DFMT_G32R32F");
		break;
	case D3DFMT_A32B32G32R32F:
		strcpy_s(chString, "D3DFMT_A32B32G32R32F");
		break;
	case D3DFMT_CxV8U8:
		strcpy_s(chString, "D3DFMT_CxV8U8");
		break;
		/* D3D9Ex only -- */
#if !defined(D3D_DISABLE_9EX)
	case D3DFMT_A1:
		strcpy_s(chString, "D3DFMT_A1");
		break;
	case D3DFMT_A2B10G10R10_XR_BIAS:
		strcpy_s(chString, "D3DFMT_A2B10G10R10_XR_BIAS");
		break;
	case D3DFMT_BINARYBUFFER:
		strcpy_s(chString, "D3DFMT_BINARYBUFFER");
		break;
#endif // !D3D_DISABLE_9EX

	case D3DFMT_FORCE_DWORD:
		strcpy_s(chString, "D3DFMT_FORCE_DWORD");
		break;
	default:
		strcpy_s(chString, "D3DFMT_UNKNOWN");
		break;
	}

	nSize = MultiByteToWideChar(CP_ACP, 0, chString, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, chString, -1, pString, nSize);
}