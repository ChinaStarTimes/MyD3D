#include "d3dUtility.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "winmm.lib")

IDirect3DDevice9*			Device = 0;		//D3D
const int					nWidth = 800;	//窗口宽
const int					nHeight = 600;	//窗口高度

ID3DXMesh* Teapot = 0;

bool Setup()
{
	//茶壶
	D3DXCreateTeapot(Device,&Teapot,0);

	//观察者位置
	D3DXVECTOR3 position(0.0f,0.0f,-5.0f);
	D3DXVECTOR3 target(0.0f,0.0f,0.0f);
	D3DXVECTOR3 up(0.0f,1.0f,0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V,&position,&target,&up);
	Device->SetTransform(D3DTS_VIEW,&V);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		MyD3D::PI * 0.5f,
		(float)nWidth/(float)nHeight,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION,&proj);

	//绘制模式
	Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);

	return true;
}

void Cleanup()
{
	MyD3D::Release<ID3DXMesh*>(Teapot);
}

bool Display(float timeDelta)
{
	if (Device)
	{
		Device->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0xffffffff,1.0f,0);

		Device->BeginScene();

		Teapot->DrawSubset(0);

		Device->EndScene();

		Device->Present(0,0,0,0);
	}

	return true;
}

LRESULT CALLBACK MyWinProc(
						 HWND hwnd,
						 UINT msg,
						 WPARAM wParam,
						 LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd,msg,wParam,lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE prevInstance,PSTR cmdLine,int showCmd)
{
	WNDCLASS wc;

	wc.style			= CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc		= MyWinProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hinstance;
	wc.hIcon			= LoadIcon(0,IDI_APPLICATION);
	wc.hCursor			= LoadIcon(0,IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= 0;
	wc.lpszClassName	="Client";

	if (!RegisterClass(&wc))
	{
		::MessageBox(0,"RegisterClass() - FAILED",0,0);
		return false;
	}

	HWND hwnd = 0;
	hwnd = ::CreateWindow("Client","Client",
		WS_EX_TOPMOST,
		100,100,nWidth,nHeight,
		0// parent hwnd
		,0// menu 
		,hinstance
		,0// extra
		);

	if (!hwnd)
	{
		::MessageBox(0,"CreateWindow() - FAILED",0,0);
		return false;
	}

	::ShowWindow(hwnd,SW_SHOW);
	::UpdateWindow(hwnd);

	//初始化d3d
	if (!MyD3D::InitD3D(hinstance,hwnd,nWidth,nHeight,true,D3DDEVTYPE_HAL,&Device))
	{
		::MessageBox(0,"InitD3D() - FAILED",0,0);
		return 0;
	}

	//分配资源
	if (!Setup())
	{
		::MessageBox(0,"Setup() - FAILED",0,0);
		return 0;
	}

	//消息循环
	MyD3D::EnterMsgLoop(Display);

	//释放资源
	Cleanup();

	//释放Device
	Device->Release();
	return 0;
}