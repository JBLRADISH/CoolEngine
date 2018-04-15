#include <tchar.h>
#include <time.h>  
#include <Windows.h>
#include "Pipline.h"
#include "CMaterail.h"
#include "Camera.h"
#pragma comment( lib,"winmm.lib" )
//=====================================================================
// Win32 窗口及图形绘制：为 device 提供一个 DibSection 的 FB
//=====================================================================
int screen_w, screen_h, screen_exit = 0;
int screen_mx = 0, screen_my = 0, screen_mb = 0;
int screen_keys[512];	// 当前键盘按下状态
static HWND screen_handle = NULL;		// 主窗口 HWND
static HDC screen_dc = NULL;			// 配套的 HDC
static HBITMAP screen_hb = NULL;		// DIB
static HBITMAP screen_ob = NULL;		// 老的 BITMAP  位图

//自定义的deviese 和windows 之间的关联 就在于这个 
unsigned char *screen_fb = NULL;		// frame buffer  帧缓冲
long screen_pitch = 0;

int screen_init(int w, int h, const TCHAR *title);	// 屏幕初始化
int screen_close(void);								// 关闭屏幕
void screen_dispatch(void);							// 处理消息
void screen_update(LPCSTR);							// 显示FrameBuffer

													// win32 event handler
static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);

//储存所有渲染物体

Object_t box_obj;
Object_t box_obj_new;

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib") //gdi绘图
#pragma comment(lib, "user32.lib")
#endif

//一个解析mesh网格的函数
void GetMesh(vertex_t* meshPtr)
{
	//TODO:
	

}




// 初始化窗口并设置标题
int screen_init(int w, int h, const TCHAR *title)
{
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)screen_events, 0, 0, 0, NULL, NULL, NULL, NULL, _T("SCREEN3.1415926") };
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB, w * h * 4, 0, 0, 0, 0 } };

	RECT rect = { 0, 0, w, h };
	int wx, wy, sx, sy;
	LPVOID ptr;
	HDC hDC;

	screen_close();

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc))
		return -1;

	screen_handle = CreateWindow(_T("SCREEN3.1415926"), title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (screen_handle == NULL)
		return -2;

	screen_exit = 0;
	hDC = GetDC(screen_handle);
	screen_dc = CreateCompatibleDC(hDC);
	ReleaseDC(screen_handle, hDC);

	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0); //创建帧缓冲ptr
	if (screen_hb == NULL)
		return -3;

	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);

	screen_fb = (unsigned char*)ptr; //把帧缓冲指针赋值给外面
	screen_w = w;
	screen_h = h;
	screen_pitch = w * 4;

	AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
	wx = rect.right - rect.left; //屏幕宽  X轴终点
	wy = rect.bottom - rect.top;//屏幕高  Y轴终点
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2; //屏幕X起点    为了让渲染界面居中。  所以左边右边都留一半作为起点/终点
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;//屏幕Y起点
	if (sy < 0)
		sy = 0;
	SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(screen_handle);

	ShowWindow(screen_handle, SW_NORMAL);
	screen_dispatch();

	memset(screen_keys, 0, sizeof(int) * 512); //使用 0 重写 sizeof(int) * 512位的 byte字节。
	memset(screen_fb, 0, w * h * 4);

	return 0;
}

int screen_close(void) {
	if (screen_dc) {
		if (screen_ob) {
			SelectObject(screen_dc, screen_ob);
			screen_ob = NULL;
		}
		DeleteDC(screen_dc);
		screen_dc = NULL;
	}
	if (screen_hb) {
		DeleteObject(screen_hb);
		screen_hb = NULL;
	}
	if (screen_handle) {
		CloseWindow(screen_handle);
		screen_handle = NULL;
	}
	return 0;
}

void Control(WPARAM wParam, LPARAM lParam);

static LRESULT screen_events(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: screen_exit = 1; break;
	case WM_KEYDOWN: screen_keys[wParam & 511] = 1; break;
	case WM_KEYUP: screen_keys[wParam & 511] = 0; break;
	case WM_MOUSEMOVE:
			Control(wParam,lParam); break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void screen_dispatch(void) {
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			break;
		if (!GetMessage(&msg, NULL, 0, 0))
			break;
		DispatchMessage(&msg);
	}
}


//真正的渲染函数
void screen_update(LPCSTR type) {
	//Device Contexts 句柄
	HDC hDC = GetDC(screen_handle); 
	//HDC设备上下文是一种包含有关某个设备（如显示器或打印机）的绘制属性信息的 Windows 数据结构。所有绘制调用都通过设备上下文对象进行，这些对象封装了用于绘制线条、形状和文本的 Windows API。

	//设备上下文绘图有很多种方法  SetPixel非常非常慢 自己维护一个Color数组然后用BitBlt比较快
	BitBlt(hDC, 0, 0, screen_w, screen_h, screen_dc, 0, 0, SRCCOPY);
	TextOut(hDC, 0, 20, type, strlen(type));
	ReleaseDC(screen_handle, hDC);
}


//=====================================================================
// 主程序
//=====================================================================

//渲染一个四角面  这个是 U3D里 模型选择 自己计算法线，而不导入法线的情况
void draw_plane(device_t *device, int a, int b, int c, int d) {

	//初始化四个顶点
	vertex_t p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];

	//初始化UV   
	p1.tc.u = 0, p1.tc.v = 0,
		p2.tc.u = 0, p2.tc.v = 1;
	p3.tc.u = 1, p3.tc.v = 1,
		p4.tc.u = 1, p4.tc.v = 0;

	//将四角面拆为两个三角形
	device_draw_primitive(device, &p1, &p2, &p3);
	device_draw_primitive(device, &p3, &p4, &p1);
}


void draw_Object(object_simple Cube, device_t *device, float theta)
{
	matrix_t m;

	matrix_set_rotate(&m, Cube.axis.x, Cube.axis.y, Cube.axis.z, theta, Cube.pos.x, Cube.pos.y, Cube.pos.z); //theta  是物体本身的x,y,z轴相对的旋转

	device->transform.model = m;
	transform_update(&device->transform);

	//TODO：如果打开阴影
	device->transform_shadow.model = m;
	transform_update(&device->transform_shadow);

	//直接渲染三角形
	for (int i = 0; i < Cube.mesh_num; i += 3)
	{
		//以后只有改变的时候，才去动态缩放一次
		vertex_t v1 = (Cube.mesh)[i];
		vertex_t v2 = (Cube.mesh)[i+1];
		vertex_t v3 = (Cube.mesh)[i+2];
		vector_scale(&v1.pos, Cube.scale);
		vector_scale(&v2.pos, Cube.scale);
		vector_scale(&v3.pos, Cube.scale);

		device_draw_primitive(device, &v1, &v2, &v3);
	}
}


//传递进来的theta 初始值为1, 代表物体的旋转角度（物体的局部坐标系相对于世界坐标系的旋转）
void draw_box(device_t *device, float theta)
{
	matrix_t m;
	//这里传递的是 (-1, -0.5, 1)这个指定向量
	matrix_set_rotate(&m, -3, -0.5, 1, theta,-3,0,5); 
	//matrix_set_rotate(&m, -3, -0.5, 1, theta,0,0,0); 

	device->transform.model = m;
	transform_update(&device->transform);


	//四个面
	draw_plane(device, 0, 1, 2, 3);
	draw_plane(device, 4, 5, 6, 7);
	draw_plane(device, 0, 4, 5, 1);
	draw_plane(device, 1, 5, 6, 2);
	draw_plane(device, 2, 6, 7, 3);
	draw_plane(device, 3, 7, 4, 0);
}


//初始化一个纹理
void init_texture(device_t *device) {
	static IUINT32 texture[256][256];
	int i, j;

	//初始化一个256X256的纹理
	for (j = 0; j < 256; j++)
	{
		for (i = 0; i < 256; i++)
		{
			int x = i / 32,
				y = j / 32;
			texture[j][i] = ((x + y) & 1) ? 0xffffff : 0x3fbcef; // &1用来快速判断是否是偶数， 交叉使用两种颜色
		}
	}
	device_set_texture(device, texture, 1024, 256, 256);
}

float Get_FPS()
{
	static float  fps = 0; //我们需要计算的FPS值
	static int    frameCount = 0;//帧数
	static float  currentTime = 0.0f;//当前时间
	static float  lastTime = 0.0f;//持续时间

	frameCount++;
	currentTime = timeGetTime()*0.001f;//获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间


	if (currentTime - lastTime > 1.0f) 
	{
		fps = (float)frameCount / (currentTime - lastTime);
		lastTime = currentTime; 
		frameCount = 0;
	}

	return fps;
}


int main(void)
{
	device_t device;
	//5种模式： 线框、填充、透视顶点插值颜色、透视面法线、透视顶点法线  
	int states[] = { RENDER_STATE_WIREFRAME,RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_surfaceNormal_color ,RENDER_STATE_verterNormal_color };
	int indicator = 0; //三种模式的数组索引，避免越界
	int kbhit = 0;//一个bool值  保证每一帧只执行一次键盘输入
	float alpha = 1; //当前物体的旋转弧度。  1 就是 180度
	float pos = -3.5;

	TCHAR *title = _T("coolRender -- ")_T("Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

	if (screen_init(800, 600, title)) //初始化WINDOWS窗口并设置标题
		return -1;

	device_init(&device, 800, 600, screen_fb); //设备初始

	//初始化主摄像机
	camera_main.pos = { 0, 0, pos, 1 }; //(point_t) { pos, 0, 0, 1 };
	camera_main.front =  { 0, 0, 0, 1 };
	camera_main.worldup ={ 0, 1, 0, 1 };//{ 0, 0, 1, 1 };

	if(dirLight.shadow == true)
	{
		//初始化阴影摄像机 采用深度测试，然后将设定的阴影颜色跟当前像素颜色混合。在像素阶段执行
		camera* shadowCamera = &cameras[0];
		shadowCamera->pos = { 0.0f, 3.0f, -3.0f, 1.0f }; //设定到产生阴影的那个灯光位置
		shadowCamera->front = dirLight.dir;
		shadowCamera->worldup = { 0, 1, 0, 1 };
    }

	init_texture(&device); //纹理初始化

	device.render_state = RENDER_STATE_WIREFRAME;

	LPCSTR type = "默认渲染模式";

	//初始化一个物体
	object_simple Cube;
	Cube.pos =  { 0, 0, 0 ,1};
	Cube.axis = { -1, -0.5, 1, 0 };
	Cube.mesh = box_mesh;
	Cube.mesh_num = 34;
	Cube.scale = 1;

	
	//初始化一个地板
	object_simple ground;

	ground.pos =  { 0, -2, 3,1 };
	Cube.axis = { -1, -0.5, 1, 0 };

	//ground.pos =  { -2, -2, 1,1 };
	//Cube.axis =  { -1, -0.5, 1, 0 };

	ground.mesh = ground_mesh;
	ground.mesh_num = 4;
	ground.scale = 6;


	char *renderType = "线框";
	float fps = 0;
	char out[200];
	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0)
	{

		//device_clear(&device); //合并设备清理

		fps = Get_FPS();

		screen_dispatch(); //分发msg
		device_clear(&device, 1); //清空缓存 Zbuffer frameBuffer

		camera_main.pos = { 0, 0, pos, 1 }; //{ pos, 0, 0, 1 };
		camera_update(&device, &camera_main); //摄像机不断更新矩阵，因为pos一直变化

		//动态灯光 阴影
		camera_updateShadow(&device, &cameras[0]);

		//camera_updateShadow(&device, &shadowCamera);

		if (screen_keys[VK_UP]) pos += 0.01f; //摄像机前进  pos -= 0.01f;
		if (screen_keys[VK_DOWN]) pos -= 0.01f; //摄像机后退
		if (screen_keys[VK_LEFT]) alpha += 0.01f; //物体向左旋转
		if (screen_keys[VK_RIGHT]) alpha -= 0.01f; //物体向右旋转

		if (screen_keys[VK_F1])
		{
			device.cull = 0;
		}
		else if (screen_keys[VK_F2])
		{
			device.cull = 1;
		}
		else if (screen_keys[VK_F3])
		{
			device.cull = 2;
		}

		if (screen_keys[VK_SPACE])
		{
			if (kbhit == 0)  //保证每一帧只执行一次键盘输入
			{
				kbhit = 1;
				if (++indicator >= 5)
					indicator = 0;
				device.render_state = states[indicator];

				if (indicator==0)
				{
					renderType = "线框";
				}
				else if(indicator == 1)
				{
					renderType = "纹理";
				}
				else if (indicator == 2)
				{
					renderType = "颜色";
				}
				else if (indicator == 3)
				{
					renderType = "面法线";
				}
				else if (indicator == 4)
				{
					renderType = "顶点法线";
				}
			}
			
		}
		else
		{
			kbhit = 0;
		}

		sprintf(out, "%3.1f", fps);
		//strcpy(out, renderType);//将第一个字符串p拷贝到q中  
		strcat(out, renderType);//将第二个字符串h拼接到q之后

		type = (LPCSTR)out;


		//渲染一个立方体 
		draw_box(&device, alpha);

		draw_Object(Cube, &device, alpha);
		draw_Object(ground,&device, 0);

		//真正的渲染函数
		screen_update(type);

		Sleep(1);
	}
	device_destroy(&device);
	return 0;
}


void Control(WPARAM wParam, LPARAM lParam)
{
	int mouse_x = (int)LOWORD(lParam);
	int mouse_y = (int)HIWORD(lParam);
	int button = (int)wParam;

	if (button&MK_LBUTTON)
	{
	//	TextOut(, 0, 16, "3D渲染demo", strlen("3D渲染demo"));
	}
}



