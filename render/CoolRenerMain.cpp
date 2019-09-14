#include <tchar.h>
#include <time.h>  
#include <Windows.h>
#include<list>
#include "Pipline.h"
#include "CMaterail.h"
#include "UObject.h"
#include "Camera.h"

//#include "../raytracer1/common.h"
#include <string>

#include <stdlib.h>
#include "../raytracer1/common.h"
#include "../raytracer1/raytracer.h"
#include "../raytracer1/scene.h"
#include "../raytracer1/surface.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "tgaimage.h"
#include <iostream>
using namespace std;

//#include "../raytracer1/testapp.h"
#pragma comment( lib,"winmm.lib" )
#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib") //gdi绘图
#pragma comment(lib, "user32.lib")
#endif

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
float beta =  1;

int screen_init(int w, int h, const TCHAR *title);	// 屏幕初始化
int screen_close(void);								// 关闭屏幕
void screen_dispatch(void);							// 处理消息
void screen_update(LPCSTR);							// 显示FrameBuffer

													// win32 event handler
static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);

#define SCRWIDTH	800
#define SCRHEIGHT	600

//一个解析mesh网格的函数
void GetMesh(vertex_t* meshPtr)
{
	//TODO:

}

//extern Pixel* buffer;
LPVOID ptr;

Raytracer::Surface* surface = 0;
Pixel* buffer = 0;
Raytracer::Engine* tracer = 0;

void Set2RayTracer()
{
	ptr = buffer;
}

//int RaytracerIT()
//{
//	int cc;
//	// prepare output canvas
//	surface = new Raytracer::Surface(SCRWIDTH, SCRHEIGHT);
//	buffer = surface->GetBuffer();
//	surface->Clear(0);
//	surface->InitCharset();
//	surface->Print("timings:", 2, 2, 0xffffffff);
//	// prepare renderer
//	tracer = new Raytracer::Engine();
//	tracer->GetScene()->InitScene();
//	tracer->SetTarget(surface->GetBuffer(), SCRWIDTH, SCRHEIGHT);
//	int tpos = 60;
//	// go
//	while (1)
//	{
//		int fstart = GetTickCount();
//		tracer->InitRender();
//		// while (!tracer->RenderTiles()) DrawWindow();
//		while (!tracer->Render())
//		{
//
//		}
//		//DrawWindow();
//		int ftime = GetTickCount() - fstart;
//		char t[] = "00:00.000";
//		t[6] = (ftime / 100) % 10 + '0';
//		t[7] = (ftime / 10) % 10 + '0';
//		t[8] = (ftime % 10) + '0';
//		int secs = (ftime / 1000) % 60;
//		int mins = (ftime / 60000) % 100;
//		t[3] = ((secs / 10) % 10) + '0';
//		t[4] = (secs % 10) + '0';
//		t[1] = (mins % 10) + '0';
//		t[0] = ((mins / 10) % 10) + '0';
//		surface->Print(t, tpos, 2, 0xffffffff);
//		tpos += 100;
//	}
//	return 1;
//}



// 初始化窗口并设置标题
int screen_init(int w, int h, const TCHAR *title)
{
	//__stdcall* WNDPROC 
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)screen_events, 0, 0, 0, NULL, NULL, NULL, NULL, _T("SCREEN3.1415926") };
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB, w * h * 4, 0, 0, 0, 0 } };

	RECT rect = { 0, 0, w, h };
	int wx, wy, sx, sy;

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

	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0); //创建帧缓冲Bitmap ptr
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
	case WM_KEYDOWN: screen_keys[wParam & 511] = 1; 
		break;
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

	string str="aaaaaaaaaa";
	string c = str + "1111";
	const char* p = str.data();
	//strlen(*abc);


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


void draw_Object_Shadow(Object_t Cube, device_t *device)
{
	//阴影
	device->transform.model = Cube.model;
	transform_update(&device->transform);

	//shadowCamera.transform.mvp = (&device->transform)->mvp;

	//直接渲染三角形
	for (int i = 0; i < Cube.mesh_num; i += 3)
	{
		vertex_t v1 = (Cube.mesh)[i];
		vertex_t v2 = (Cube.mesh)[i + 1];
		vertex_t v3 = (Cube.mesh)[i + 2];

		device_draw_primitive_shadow(device, &v1, &v2, &v3);
	}
}


void draw_Object(const Object_t& Cube, device_t *device)
{

	device->transform.model = Cube.model;
	transform_update(&device->transform);

	//直接渲染三角形
	for (int i = 0; i < Cube.mesh_num; i += 3)
	{
		//以后只有改变的时候，才去动态缩放一次
		vertex_t v1 = (Cube.mesh)[i];
		vertex_t v2 = (Cube.mesh)[i+1];
		vertex_t v3 = (Cube.mesh)[i+2];

		device_draw_primitive(device, &v1, &v2, &v3);
	}
}


//传递进来的theta 初始值为1, 代表物体的旋转角度（物体的局部坐标系相对于世界坐标系的旋转）
void draw_box(device_t *device, float theta)
{
	matrix_t m;
	matrix_Obj2World(&m,  vector_t(0, theta, beta,1), vector_t(2, 4, 5,1));
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

void load_texture(std::string filename, const char* suffix, TGAImage& img) {
	//std::string texfile(filename);
	//size_t dot = texfile.find_last_of(".");
	//if (dot != std::string::npos) {
	//	texfile = texfile.substr(0, dot) + std::string(suffix);

	//	//load tga
	//	bool ifload = img.read_tga_file(texfile.c_str());

		bool ifload = img.read_tga_file(filename.c_str());
		std::cerr << "texture file " << filename << " loading " << (ifload ? "ok" : "failed") << std::endl;
		img.flip_vertically();
	//}
}

void Load_Tga()
{
	TGAImage diffusemap_;
	TGAImage normalmap_;
	TGAImage specularmap_;
	string filename = "obj/floor_diffuse.tga";
	/*load_texture(filename, "_diffuse.tga", diffusemap_);
	load_texture(filename, "_nm_tangent.tga", normalmap_);
	load_texture(filename, "_spec.tga", specularmap_);*/

	load_texture("obj/floor_diffuse.tga", "_diffuse.tga", diffusemap_);
	load_texture("obj/floor_nm_tangent.tga", "_nm_tangent.tga", normalmap_);
	load_texture("obj/floor_spec.tga", "_spec.tga", specularmap_);
}


void Load_Model()
{
	char** argv;
	int argc = 2;
	string obj = "obj/floor.obj";
	char* filename = (char*)obj.c_str();

	std::ifstream in = ifstream();
	in.open(filename, std::ifstream::in);

	if (in.fail())
	{
		cout << "no file-------" << endl;
		return;
	}

	//for (int m = 1; m < argc; m++) {
	//	model = new Model(argv[m]);
	//	Shader shader;
	//	for (int i = 0; i < model->nfaces(); i++) {
	//		for (int j = 0; j < 3; j++) {
	//			shader.vertex(i, j);
	//		}
	//		triangle(shader.varying_tri, shader, frame, zbuffer);
	//	}
	//	delete model;
	//}
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


void Create_Obj()
{
	//初始化一个地板
	Object_t ground;

	ground.pos = { -2, -3, 0,1 };
	ground.axis = { 0, 0, 0, 0 };
	ground.mesh = ground_mesh;
	ground.mesh_num = 4;
	ground.scaleX = 9;
	ground.scaleY = 1;
	ground.scaleZ = 4;
	//ground.theta = 0;

	//初始化一个物体
	Object_t Cube; 
	Cube.pos = { 1, 2, 1 ,1 };
	Cube.axis = { 0, 0, 0, 0 };
	Cube.mesh = box_mesh;
	Cube.mesh_num = 34;
	Cube.scaleX = 3;
	Cube.scaleY = 3;
	Cube.scaleZ = 3;
	//Cube.theta = 1;  //当前物体的旋转弧度。  1 就是 180度

	Scene_render_Objs.push_back(Cube);
	Scene_render_Objs.push_back(ground);
}

void Init_Obj(Object_t& Cube)
{	//matrix_set_rotate(&m, Cube.axis.x, Cube.axis.y, Cube.axis.z, 0, Cube.pos.x, Cube.pos.y, Cube.pos.z); //theta  是物体本身的x,y,z轴相对的旋转

	matrix_t s, r, t;
	//TODO: 计算 旋转 和位移 需要的矩阵

//输入 当前物体原点在世界坐标系中的位置和旋转，  来反推世界矩阵
//axis.x 绕X轴的旋转角度

//物体转世界坐标系  	//平移-> 旋转-》缩放 
	matrix_set_scale(&s, Cube.scaleX, Cube.scaleY, Cube.scaleZ);
	matrix_Obj2World(&r, Cube.axis, Cube.pos);
	matrix_set_identity(&t);
	t.m[3][0] = Cube.pos.x;
	t.m[3][1] = Cube.pos.y;
	t.m[3][2] = Cube.pos.z;

	Cube.model = s * r * t;
}

void CreateCamera(device_t* device,int width, int height)
{
	//初始化主摄像机
	camera_main.eye = { 0, 3, -10, 1 };

	camera_main.eyeTarget = { 0, 0, 0, 1};
	camera_main.worldup = { 0, 1, 0, 1 };

	camera_main.fov = PI * 0.5f;
	camera_main.zn = 1;
	camera_main.zf = 500;
	camera_main.aspect = (float)width / ((float)height);
	//camera_main.rotation = { -0.15, 0, 0, 1 };

}


void Init_ShadowCamera(device_t* device)
{
	if (dirLight.shadow == true)
	{
		//初始化阴影摄像机 采用深度测试，然后将设定的阴影颜色跟当前像素颜色混合。在像素阶段执行
		camera* shadowCamera = &cameras[0];
		shadowCamera->eye = vector_t{0,10,10,1};
		shadowCamera->eyeTarget = vector_t{ 0,0,0,1 };
		shadowCamera->worldup = { 0, 1, 0, 1 };
	}
}
void InitDevice(device_t* device,camera& curCamera)
{
	device->cull = curCamera.cull;

	//这里把屏幕制空了
	device->transform.projection = curCamera.projection_trans;
	device->transform.view = curCamera.view;
	device->curCamera = curCamera;

}
void TestReference();
void TestReset();
void TestReset2();


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
int imageCount = 0;
bool TestimageCount(TGAImage& image, int x, int y, const TGAColor& color)
{
	//cout << x << "----" << y<<endl;
	imageCount++;
	return image.set(x, y, color);
}

//错误：只能用于x1 》x0 y1》y0
//void  lineSimple(int x0, int y0,int x1,int y1,TGAImage&image,TGAColor color){
//	 for(int x = x0; x <= x1; x ++){
//		 float t = (x - x0) / (float)(x1 - x0);
//		int y = y0  + (y1-y0) * t;
//		cout << x << "----" << y <<"--t---"<<t<< endl;
//		TestimageCount(image, x, y, color);
//	}
//}
//
//void lineSimple2(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
//	bool steep = false;
//
//	//设想极端情况,X0 到X1 只相差1，但Y0-Y1相差100，以X++来循环，Y无法插值。所以要以最长的轴来++循环
//	//由于高度大于宽度会造成空洞，需要保持不陡
//	if (std::abs(x0 - x1) < std::abs(y0 - y1)) { // if the line is steep, we transpose the image 
//		std::swap(x0, y0);
//		std::swap(x1, y1);
//		steep = true;
//	}
//	if (x0 > x1) { // make it left−to−right 
//		std::swap(x0, x1);
//		std::swap(y0, y1);
//	}
//	int dx = x1 - x0;
//	int dy = y1 - y0;
//	float derror = std::abs(dy / float(dx)); //斜率，X轴上每移动一格，Y轴增加的误差
//	float error = 0;
//	int y = y0;
//
//
//	for (int x = x0; x <= x1; x++) {
//		//float t = (x - x0) / (float)(x1 - x0);
//		//int y = y0 * (1. - t) + y1 * t;
//		if (steep) {
//			TestimageCount(image, y, x, color); // if transposed, de−transpose 
//		}
//		else {
//			TestimageCount(image, x, y, color);
//		}
//		error += derror;
//		if (error > .5) {
//			y += (y1 > y0 ? 1 : -1);
//			error -= 1.;
//		}
//	}
//}
//void  line(int x0,int y0,int x1,int y1,TGAImage &image,TGAColor color){
//	for (float t = 0.; t < 1.; t += .1) {
//		 int x = x0 + (x1 - x0) * t;
//		int y = y0 + (y1 - y0) * t;
//		TestimageCount(image,x,y,color);
//	}
//}
//
//// x y都必须是整数。。。
//// 绘制线段 2.光栅化2D直线（布雷森漢姆直線演算法、吴小林直线算法等） 起点-终点，寻找线段上离二维栅格最近的像素点
//void device_draw_line(int x1, int y1, int x2, int y2,  TGAImage& image, TGAColor color)
//{
//	//rem用于修正
//	int x, y, rem = 0;
//
//	//这个没有寻找最近的栅格...   只是计算了下距离。
//	if (x1 == x2 && y1 == y2)  //同一点，绘制一个像素点
//	{
//		TestimageCount(image, x1, y1, color);
//		//device_pixel(device, x1, y1, c);
//	}
//	else if (x1 == x2)  //说明是垂直线
//	{
//		int inc = (y1 <= y2) ? 1 : -1;
//		for (y = y1; y != y2; y += inc) //朝上或者朝下 
//			TestimageCount(image, x1, y, color);
//			//device_pixel(device, x1, y, c);
//		TestimageCount(image, x2, y2, color);
//		//device_pixel(device, x2, y2, c);
//	}
//	else if (y1 == y2) //说明是平行线
//	{
//		int inc = (x1 <= x2) ? 1 : -1;
//		for (x = x1; x != x2; x += inc)
//			TestimageCount(image, x, y1, color);
//			//device_pixel(device, x, y1, c);
//		TestimageCount(image, x2, y2, color);
//		//device_pixel(device, x2, y2, c);
//	}
//	else
//	{
//		int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
//		int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
//
//		if (dx >= dy) //说明跟X轴夹角小于45
//		{
//			if (x2 < x1)
//				x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
//			for (x = x1, y = y1; x <= x2; x++)
//			{
//				
//				//device_pixel(device, x, y, c);
//				rem += dy;
//				if (rem >= dx)
//				{
//					rem -= dx;
//					y += (y2 >= y1) ? 1 : -1;
//					TestimageCount(image, x, y, color);
//					//device_pixel(device, x, y, c);
//				}
//				else
//					TestimageCount(image, x, y, color);
//			}
//			//TestimageCount(image, x2, y2, color);
//			//device_pixel(device, x2, y2, c);
//		}
//		else
//		{
//			if (y2 < y1)
//				x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
//			for (x = x1, y = y1; y <= y2; y++)
//			{
//				
//				//device_pixel(device, x, y, c);
//				rem += dx;
//				if (rem >= dy)
//				{
//					rem -= dy;
//					x += (x2 >= x1) ? 1 : -1;
//					TestimageCount(image, x, y, color);
//					//device_pixel(device, x, y, c);
//				}
//				else
//					TestimageCount(image, x, y1, color);
//			}
//			//TestimageCount(image, x2, y2, color);
//			//device_pixel(device, x2, y2, c);
//		}
//	}
//}

//int main(int argc, char** argv) {
//	TGAImage image(100, 100, TGAImage::RGB);
//	//device_draw_line(3, 3, 67, 83,image, red);
//	//line(67, 83, 3, 3, image, red);
//	//lineSimple(3, 3, 8, 16, image, red);
//	lineSimple2(3, 3, 67, 83, image, red);
//	//lineSimple(20, 13, 40, 80, image, red);
//	//lineSimple(80, 40, 13, 20, image, red);
//	//line(3, 3, 67, 83, image, red);
//	//image.set(52, 41, red);
//	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
//	image.write_tga_file("output.tga");
//	cout << imageCount << "----" << endl;
//	return 0;
//}

int main(void)
{
	//TestReference();
	//TestReset2();
	device_t device;
	//5种模式： 线框、填充、透视顶点插值颜色、透视面法线、透视顶点法线  
	int states[] = { RENDER_STATE_WIREFRAME,RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_surfaceNormal_color ,RENDER_STATE_verterNormal_color };

	char * states_text[] = { RENDER_STATE_WIREFRAME_TEXT,RENDER_STATE_TEXTURE_TEXT, RENDER_STATE_COLOR_TEXT, RENDER_STATE_surfaceNormal_color_TEXT ,RENDER_STATE_verterNormal_color_TEXT };

	int indicator = 0; //模式的数组索引，避免越界
	int kbhit = 0;//一个bool值  保证每一帧只执行一次键盘输入
	float alpha = 1; //当前物体的旋转弧度。  1 就是 180度

	TCHAR *title = _T("coolRender -- ")_T("Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

	int width = 800;
	int height = 600;

	if (screen_init(width, height, title)) //初始化WINDOWS窗口并设置标题
		return -1;

	Load_Tga();
	device_init(&device, width, height, screen_fb); //设备初始

	CreateCamera(&device, width, height);//初始化主摄像机

	Init_ShadowCamera(&device);//阴影摄像机

	Create_Obj();//初始化场景里的物体

	init_texture(&device); //纹理初始化

	device.render_state = RENDER_STATE_WIREFRAME;

	LPCSTR type = "默认渲染模式";

	char *renderType = RENDER_STATE_WIREFRAME_TEXT;
	float fps = 0;
	char out[200];
	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0)
	{
		fps = Get_FPS();

		screen_dispatch(); //分发msg
		device_clear(&device, 1); //清空缓冲
		//device_clear(&device); // Zbuffer frameBuffer

		if (screen_keys[VK_UP]) camera_main.eye.z += 0.01f; //摄像机前进  pos -= 0.01f;
		if (screen_keys[VK_DOWN]) camera_main.eye.z -= 0.01f; //摄像机后退
		if (screen_keys[VK_LEFT]) alpha += 0.01f; //物体向左旋转
		if (screen_keys[VK_RIGHT]) alpha -= 0.01f; //物体向右旋转


		if (screen_keys[0x41]) beta -= 0.01f; //欧拉角
		if (screen_keys[0x42]) beta += 0.01f; //欧拉角

		camera_update(&camera_main); //摄像机不断更新矩阵
		//动态灯光 阴影
		camera_updateShadow(&cameras[0], &camera_main);
		cameras[0].width = width;
		cameras[0].height = height;

		if (screen_keys[VK_F1])
		{
			camera_main.cull = 0;
		}
		else if (screen_keys[VK_F2])
		{
			camera_main.cull = 1;
		}
		else if (screen_keys[VK_F3])
		{
			camera_main.cull = 2;
		}
		
		if (screen_keys[VK_SPACE])
		{
			if (kbhit == 0)//保证每次按下只执行一次键盘输入
			{
				kbhit = 1;
				if (++indicator >= 5)
					indicator = 0;
				device.render_state = states[indicator];
				renderType = states_text[indicator];
			}		
		}
		else
		{
			kbhit = 0;
		}


		//渲染一个立方体 
	//	draw_box(&device, alpha);

		// 渲染物体,更新设备
		(&device.transform)->screen_width = (float)width;
		(&device.transform)->screen_height = (float)height;


		//1.初始化物体的矩阵跟属性
		for (int i = 0; i < Scene_render_Objs.size(); i++)
		{
			Scene_render_Objs[i].axis.y = alpha;
			Init_Obj(Scene_render_Objs[i]);
		}


		////2.先重置阴影的深度缓冲
		InitDevice(&device, cameras[0]);

		if ((&device)->shadowbuffer != NULL) {
			for (int y = 0; y < (&device)->height; y++)
				for (int x = 0; x < (&device)->width; x++)
					(&device)->shadowbuffer[y * (&device)->width + x] = 0;
		}

		//渲染阴影的深度缓冲
		for (int i = 0; i < Scene_render_Objs.size(); i++)
		{
			draw_Object_Shadow(Scene_render_Objs[i], &device); 
		}

		int y,x;
		for (y = 0; y < (&device)->height; y++)
		{
			float *dst = (&device)->zbuffer[y];
			for (x = (&device)->width; x > 0; dst++, x--)
				dst[0] = 0.0f;
		}

		//3.渲染物体
		InitDevice(&device, camera_main);

		for (int i = 0; i < Scene_render_Objs.size(); i++)
		{
			draw_Object(Scene_render_Objs[i], &device);
		}

		sprintf(out, "%3.1f", fps);
		//strcpy(out, renderType);//将第一个字符串p拷贝到q中  
		strcat(out, renderType);//将第二个字符串h拼接到q之后

		//string outText = out;

		char c[8];
		itoa(device.cull, c, 16);
		strcat(out, c);
		type = (LPCSTR)out;
		Set2RayTracer();
		//RaytracerIT();
		//真正的渲染函数
		screen_update(type);

		Sleep(1);
	}

	device_destroy(&device);
	//static_device_t = nullptr;
	return 0;
}



void Control(WPARAM wParam, LPARAM lParam)
{
	int mouse_x = (int)LOWORD(lParam);
	int mouse_y = (int)HIWORD(lParam);
	int button = (int)wParam;
	HDC hDC = GetDC(screen_handle);
	if (button&MK_LBUTTON)
	{
		TextOut(hDC, 0, 16, "3D渲染demo", strlen("3D渲染demo"));
	}
}