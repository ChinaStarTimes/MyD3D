#include "d3dUtility.h"
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")

//将D3DXCOLOR转换成D3DCOLORVALUE结构的数据
D3DCOLORVALUE MyD3D::xColorToValue(D3DXCOLOR a)
{
	D3DCOLORVALUE color;

	color.a = a.a;		//Alpha值
	color.b = a.b;		//蓝色值
	color.g = a.g;		//绿色值
	color.r = a.r;		//红色值

	return color;
}

//初始化DirectX3D
bool MyD3D::InitD3D(HINSTANCE hInstance,
					HWND hwnd,
					int width,int height,
					bool windowed, 
					D3DDEVTYPE deviceType, 
					IDirect3DDevice9** device)
{
	HRESULT hr = 0;

	//Step 1: 要初始化IDirect3D，首先必须获取指向接口IDrect3D9的指针。使得一个专门的Direct3D函数可以很容易做到  
	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		::MessageBox(0,"Direct3DCreate9() - FAILED",0,0);
		return false;
	}

	//Step 2: 检验图形卡是否支持该硬件顶点运算
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT,deviceType,&caps);
	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		//硬件定点运算
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		//软件顶点运算
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//Step 3: 填写 D3DPRESENT_PARAMETERS结构
	D3DPRESENT_PARAMETERS d3dpp;  
	d3dpp.BackBufferWidth            = width; //后台缓存中表面的宽度，单位为像素
	d3dpp.BackBufferHeight           = height;//后台缓存中表面的高度，单位为像素
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;//后台缓存的像素格式(如32位像素格式：D3DFMT_A8R8G8B8)  
	d3dpp.BackBufferCount            = 1;     //所需使用的后台缓存的个数，通常是1，需要一个后台缓存  
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE; //后台缓存使用的多重采样类型  
	d3dpp.MultiSampleQuality         = 0;     //多重采样的质量水平  
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; //枚举类型指定交换链中的缓存的页面设置方式。  
	d3dpp.hDeviceWindow              = hwnd;  //与设备相关的窗口句柄。指定了所有进行绘制的应用程序窗口  
	d3dpp.Windowed                   = windowed;//窗口模式  true为窗口 false为全屏
	d3dpp.EnableAutoDepthStencil     = true; //自动创建并维护深度缓存或模板缓存  
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;//深度缓存或模板缓存的像素格式  
	d3dpp.Flags                      = 0;//附加特性 0表示无标记  
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//刷新频率 当前表示默认频率
	/*D3DPRESENT集合的一个成员 合法时间间隔列表
	 D3DPRESENT_INTERVAL_IMMEDIATE 立即提交
	D3DPRESENT_INTERVAL_DEFAULT 由Direct3D来选择提交频率 */
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
	
	// Step 4: 利用已经初始化的D3DPRESENT_PARAMETERS结构创建IDrect3Device9(一个C++对象，代表我们用来显示3D图形的物理硬件设备)
	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter默认显卡  
		deviceType,         // device type  
		hwnd,               // window associated with device  
		vp,                 // vertex processing  
		&d3dpp,             // present parameters  
		device);            // return created device  

	if( FAILED(hr) )  
	{  
		// try again using a 16-bit depth buffer  
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;  

		hr = d3d9->CreateDevice(  
			D3DADAPTER_DEFAULT,  
			deviceType,  
			hwnd,  
			vp,  
			&d3dpp,  
			device);//返回创建的设备  

		if( FAILED(hr) )  
		{  
			d3d9->Release(); // done with d3d9 object  
			::MessageBox(0, "CreateDevice() - FAILED", 0, 0);  
			return false;  
		}  
	}  

	d3d9->Release(); // done with d3d9 object  

	return true;  
}

//消息循环  
int MyD3D::EnterMsgLoop( bool (*ptr_display)(float timeDelta) )  
{  
	MSG msg;  
	::ZeroMemory(&msg, sizeof(MSG));  

	static float lastTime = (float)timeGetTime();   

	while(msg.message != WM_QUIT)  
	{  
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))  
		{  
			::TranslateMessage(&msg);  
			::DispatchMessage(&msg);  
		}  
		else  
		{     
			float currTime  = (float)timeGetTime();  
			float timeDelta = (currTime - lastTime)*0.001f;  
			//计算相邻两次调用ptr_display的时间间隔  
			ptr_display(timeDelta);//调用ptr_display()函数  

			lastTime = currTime;  
		}  
	}  
	return msg.wParam;  
}

//初始化材质
D3DMATERIAL9 MyD3D::InitMtrl(D3DXCOLOR a,D3DXCOLOR d,D3DXCOLOR s,D3DXCOLOR e,float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = xColorToValue(a);
	mtrl.Diffuse = xColorToValue(d);
	mtrl.Specular = xColorToValue(s);
	mtrl.Emissive = xColorToValue(e);
	mtrl.Power = p;

	return mtrl;
}

//初始化方向光
D3DLIGHT9 MyD3D::InitDirectjonalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light,sizeof(light));

	light.Type			= D3DLIGHT_DIRECTIONAL;				//类型
	light.Ambient		= xColorToValue(*color * 0.4f);		//光源发出的环境光颜色
	light.Diffuse		= xColorToValue(*color);			//光源发出的漫射光颜色
	light.Specular		= xColorToValue(*color * 0.6f);		//光源发出的镜面光颜色
	light.Direction		= *direction;						//描述光在世界坐标系中传播方向的向量

	return light;
}

//初始化点光源
D3DLIGHT9 MyD3D::InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light,sizeof(light));

	light.Type			= D3DLIGHT_POINT;					//类型
	light.Position		= *position;						//描述光源在世界坐标系中的位置的向量
	light.Ambient		= xColorToValue(*color * 0.4f);		//光源发出的环境光颜色
	light.Diffuse		= xColorToValue(*color);			//光源发出的漫射光颜色
	light.Specular		= xColorToValue(*color * 0.6f);		//光源发出的镜面光颜色
	light.Range			= 100.0f;							//光线“消亡”前，所能达到的最大光程

	return light;
}

//聚光灯
D3DLIGHT9 MyD3D::InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light,sizeof(light));

	light.Type			= D3DLIGHT_SPOT;					//类型
	light.Position		= *position;						//描述光源在世界坐标系中的位置的向量
	light.Direction		= *direction;						//描述光在世界坐标系中传播方向的向量
	light.Ambient		= xColorToValue(*color * 0.4f);		//光源发出的环境光颜色
	light.Diffuse		= xColorToValue(*color);			//光源发出的漫射光颜色
	light.Specular		= xColorToValue(*color * 0.6f);		//光源发出的镜面光颜色
	light.Range			= 100.0f;							//光线“消亡”前，所能达到的最大光程
	light.Falloff		= 1.0f;								//光强从内锥形到外锥形的衰减方式
	light.Theta			= MyD3D::PI/3.0f;						//内锥形角度
	light.Phi			= MyD3D::PI/2.0f;						//外锥形角度

	return light;
}