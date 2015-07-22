#ifndef D3DUTILITY_H
#define D3DUTILITY_H

#ifdef _D3DUTILITY
#define _MYD3D_EXPORT __declspec( dllexport )
#else
#define _MYD3D_EXPORT __declspec( dllimport )
#endif

//Include the main Direct3D header file. This will include
//the other Direct3D header files we need.
#include <d3d9.h>
#include <D3DX10math.h>
#include <d3dx9shape.h>


namespace MyD3D{
	
	/*****************************************************************
	*函数名：InitD3D
	*参  数：[in]	(HINSTANCE)hInstance	使用设备的程序实例
	*		[in]	(HWND)hwnd	使用设备的窗口句柄
	*		[in]	(int)width	后台缓存规模	窗口宽度
	*		[in]	(int)height	后台缓存规模	窗口高度
	*		[in]	(bool)windowed	窗口模式(true)或全屏模式(false)
	*		[in]	(D3DDEVTYPE)deviceType	HAL 或 REF
	*		[out]	(IDirect3DDevice9**)device	返回的设备
	*返  回：true 或 false
	*应  用：初始化DirectX3D
	*******************************************************************/
	bool _MYD3D_EXPORT InitD3D(
		HINSTANCE hInstance,
		HWND hwnd,
		int width,int height,
		bool windowed,
		D3DDEVTYPE deviceType,
		IDirect3DDevice9** device);
	
	/*****************************************************************
	*函数名：InitMtrl
	*参  数：[in]	(D3DXCOLOR)a	材质对环境光的反射率
	*		[in]	(D3DXCOLOR)d	材质对漫射光的反射率
	*		[in]	(D3DXCOLOR)s	材质对镜面光的反射率
	*		[in]	(D3DXCOLOR)e	增强物体的亮度，使之看起来好像可以自己发光
	*		[in]	(float)p		镜面高光点的锐度，值越大，锐度越大
	*返  回：D3DMATERIAL9 一个材质
	*应  用：按参数要求初始化一个材质
	*******************************************************************/
	D3DMATERIAL9 _MYD3D_EXPORT InitMtrl(D3DXCOLOR a,D3DXCOLOR d,D3DXCOLOR s,D3DXCOLOR e,float p);

	/*****************************************************************
	*函数名：InitDirectjonalLight
	*参  数：[in]	(D3DXVECTOR3*)direction		方向光方向
	*		[in]	(D3DXCOLOR*)color			光的颜色
	*返  回：D3DLIGHT9 一个方向光
	*应  用：按参数要求初始化一个方向光
	*******************************************************************/
	D3DLIGHT9 _MYD3D_EXPORT InitDirectjonalLight(
									D3DXVECTOR3* direction,
									D3DXCOLOR* color
									);

	/*****************************************************************
	*函数名：InitPointLight
	*参  数：[in]	(D3DXVECTOR3*)position	点光源的空间位置
	*		[in]	(D3DXCOLOR*)color		光的颜色
	*返  回：D3DLIGHT9 一个点光源
	*应  用：按参数要求初始化一个点光源
	*******************************************************************/
	D3DLIGHT9 _MYD3D_EXPORT InitPointLight(
									D3DXVECTOR3* position,
									D3DXCOLOR* color
									);

	/*****************************************************************
	*函数名：InitSpotLight
	*参  数：[in]	(D3DXVECTOR3*)position	聚光灯的空间位置
	*		[in]	(D3DXVECTOR3*)direction	聚光灯方向
	*		[in]	(D3DXCOLOR*)color		光的颜色
	*返  回：D3DLIGHT9 一个聚光灯
	*应  用：按参数要求初始化一个聚光灯
	*******************************************************************/
	D3DLIGHT9 _MYD3D_EXPORT InitSpotLight(
									D3DXVECTOR3* position,
									D3DXVECTOR3* direction,
									D3DXCOLOR* color
									);

	/*****************************************************************
	*函数名：EnterMsgLoop
	*参  数：[in]	一个带[in](float)timeDelta参数的 函数
	*返  回：msg.wParam
	*应  用：消息循环
	*******************************************************************/
	int _MYD3D_EXPORT EnterMsgLoop(bool (*ptr_display)(float timeDelta));
	
	/*****************************************************************
	*函数名：Release
	*参  数：[in] (T)t	模板参数
	*返  回：无
	*应  用：释放模板T
	*******************************************************************/
	template<class T> void Release(T t)
	{
		if ( t )
		{
			t->Release();
			t = 0;
		}
	}
	
	/*****************************************************************
	*函数名：Delete
	*参  数：[in] (T)t	模板参数
	*返  回：无
	*应  用：删除模板T
	*******************************************************************/
	template<class T> void Delete(T t)
	{
		if ( t )
		{
			delete t;
			t = 0;
		}
	}
	
	/*****************************************************************
	*函数名：xColorToValue
	*参  数：[in] (D3DXCOLOR)a	需要转换成D3DCOLORVALUE的数据
	*返  回：D3DCOLORVALUE 结构的数据
	*应  用：将D3DXCOLOR转换成D3DCOLORVALUE结构的数据
	*******************************************************************/
	D3DCOLORVALUE xColorToValue(D3DXCOLOR a);
	
	//赋值PI
	const float PI = 3.14f;

	//颜色
	const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255,255,255));	//白色
	const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0,0,0));		//黑色
	const D3DXCOLOR RED(D3DCOLOR_XRGB(255,0,0));		//红色
	const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0,255,0));		//绿色
	const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0,0,255));		//蓝色
	const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255,255,0));	//黄色
	const D3DXCOLOR CYAN(D3DCOLOR_XRGB(0,255,255));		//蓝绿色
	const D3DXCOLOR MAGENTA(D3DCOLOR_XRGB(255,0,255));	//洋红

	//常用材质
	const D3DMATERIAL9 WHITE_MTRL		= InitMtrl(WHITE,WHITE,WHITE,BLACK,8.0f);		//白色材质
	const D3DMATERIAL9 RED_MTRL			= InitMtrl(RED,RED,RED,BLACK,8.0f);				//红色材质
	const D3DMATERIAL9 GREEN_MTRL		= InitMtrl(GREEN,GREEN,GREEN,BLACK,8.0f);		//绿色材质
	const D3DMATERIAL9 BLUE_MTRL		= InitMtrl(BLUE,BLUE,BLUE,BLACK,8.0f);			//蓝色材质
	const D3DMATERIAL9 YELLOW_MTRL		= InitMtrl(YELLOW,YELLOW,YELLOW,BLACK,8.0f);	//黄色材质
}
#endif //D3DUTILITY_H
