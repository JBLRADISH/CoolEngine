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

<<<<<<< HEAD
#include <iostream>
=======
>>>>>>> f3b6847aa1d9bb27560ac96ba6a85fee937425ea
using namespace std;

//#include "../raytracer1/testapp.h"
#pragma comment( lib,"winmm.lib" )
#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib") //gdi��ͼ
#pragma comment(lib, "user32.lib")
#endif

//=====================================================================
// Win32 ���ڼ�ͼ�λ��ƣ�Ϊ device �ṩһ�� DibSection �� FB
//=====================================================================
int screen_w, screen_h, screen_exit = 0;
int screen_mx = 0, screen_my = 0, screen_mb = 0;
int screen_keys[512];	// ��ǰ���̰���״̬
static HWND screen_handle = NULL;		// ������ HWND
static HDC screen_dc = NULL;			// ���׵� HDC
static HBITMAP screen_hb = NULL;		// DIB
static HBITMAP screen_ob = NULL;		// �ϵ� BITMAP  λͼ

//�Զ����deviese ��windows ֮��Ĺ��� ��������� 
unsigned char *screen_fb = NULL;		// frame buffer  ֡����
long screen_pitch = 0;
float beta =  1;

int screen_init(int w, int h, const TCHAR *title);	// ��Ļ��ʼ��
int screen_close(void);								// �ر���Ļ
void screen_dispatch(void);							// ������Ϣ
void screen_update(LPCSTR);							// ��ʾFrameBuffer

													// win32 event handler
static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);

#define SCRWIDTH	800
#define SCRHEIGHT	600

//һ������mesh����ĺ���
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



// ��ʼ�����ڲ����ñ���
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

	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0); //����֡����Bitmap ptr
	if (screen_hb == NULL)
		return -3;

	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);

	screen_fb = (unsigned char*)ptr; //��֡����ָ�븳ֵ������
	screen_w = w;
	screen_h = h;
	screen_pitch = w * 4;

	AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
	wx = rect.right - rect.left; //��Ļ��  X���յ�
	wy = rect.bottom - rect.top;//��Ļ��  Y���յ�
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2; //��ĻX���    Ϊ������Ⱦ������С�  ��������ұ߶���һ����Ϊ���/�յ�
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;//��ĻY���
	if (sy < 0)
		sy = 0;
	SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(screen_handle);

	ShowWindow(screen_handle, SW_NORMAL);
	screen_dispatch();

	memset(screen_keys, 0, sizeof(int) * 512); //ʹ�� 0 ��д sizeof(int) * 512λ�� byte�ֽڡ�
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


//��������Ⱦ����
void screen_update(LPCSTR type) {
	//Device Contexts ���
	HDC hDC = GetDC(screen_handle); 
	//HDC�豸��������һ�ְ����й�ĳ���豸������ʾ�����ӡ�����Ļ���������Ϣ�� Windows ���ݽṹ�����л��Ƶ��ö�ͨ���豸�����Ķ�����У���Щ�����װ�����ڻ�����������״���ı��� Windows API��

	string str="aaaaaaaaaa";
	string c = str + "1111";
	const char* p = str.data();
	//strlen(*abc);


	//�豸�����Ļ�ͼ�кܶ��ַ���  SetPixel�ǳ��ǳ��� �Լ�ά��һ��Color����Ȼ����BitBlt�ȽϿ�
	BitBlt(hDC, 0, 0, screen_w, screen_h, screen_dc, 0, 0, SRCCOPY);
	TextOut(hDC, 0, 20, type, strlen(type));
	ReleaseDC(screen_handle, hDC);


}


//=====================================================================
// ������
//=====================================================================

//��Ⱦһ���Ľ���  ����� U3D�� ģ��ѡ�� �Լ����㷨�ߣ��������뷨�ߵ����
void draw_plane(device_t *device, int a, int b, int c, int d) {

	//��ʼ���ĸ�����
	vertex_t p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];

	//��ʼ��UV   
	p1.tc.u = 0, p1.tc.v = 0,
		p2.tc.u = 0, p2.tc.v = 1;
	p3.tc.u = 1, p3.tc.v = 1,
		p4.tc.u = 1, p4.tc.v = 0;

	//���Ľ����Ϊ����������
	device_draw_primitive(device, &p1, &p2, &p3);
	device_draw_primitive(device, &p3, &p4, &p1);
}


void draw_Object_Shadow(Object_t Cube, device_t *device)
{
	//��Ӱ
	device->transform.model = Cube.model;
	transform_update(&device->transform);

	//shadowCamera.transform.mvp = (&device->transform)->mvp;

	//ֱ����Ⱦ������
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

	//ֱ����Ⱦ������
	for (int i = 0; i < Cube.mesh_num; i += 3)
	{
		//�Ժ�ֻ�иı��ʱ�򣬲�ȥ��̬����һ��
		vertex_t v1 = (Cube.mesh)[i];
		vertex_t v2 = (Cube.mesh)[i+1];
		vertex_t v3 = (Cube.mesh)[i+2];

		device_draw_primitive(device, &v1, &v2, &v3);
	}
}


//���ݽ�����theta ��ʼֵΪ1, �����������ת�Ƕȣ�����ľֲ�����ϵ�������������ϵ����ת��
void draw_box(device_t *device, float theta)
{
	matrix_t m;
	matrix_Obj2World(&m,  vector_t(0, theta, beta,1), vector_t(2, 4, 5,1));
	//matrix_set_rotate(&m, -3, -0.5, 1, theta,0,0,0); 

	device->transform.model = m;
	transform_update(&device->transform);


	//�ĸ���
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

//��ʼ��һ������
void init_texture(device_t *device) {
	static IUINT32 texture[256][256];
	int i, j;

	//��ʼ��һ��256X256������
	for (j = 0; j < 256; j++)
	{
		for (i = 0; i < 256; i++)
		{
			int x = i / 32,
				y = j / 32;
			texture[j][i] = ((x + y) & 1) ? 0xffffff : 0x3fbcef; // &1���������ж��Ƿ���ż���� ����ʹ��������ɫ
		}
	}
	device_set_texture(device, texture, 1024, 256, 256);
}

float Get_FPS()
{
	static float  fps = 0; //������Ҫ�����FPSֵ
	static int    frameCount = 0;//֡��
	static float  currentTime = 0.0f;//��ǰʱ��
	static float  lastTime = 0.0f;//����ʱ��

	frameCount++;
	currentTime = timeGetTime()*0.001f;//��ȡϵͳʱ�䣬����timeGetTime�������ص����Ժ���Ϊ��λ��ϵͳʱ��


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
	//��ʼ��һ���ذ�
	Object_t ground;

	ground.pos = { -2, -3, 0,1 };
	ground.axis = { 0, 0, 0, 0 };
	ground.mesh = ground_mesh;
	ground.mesh_num = 4;
	ground.scaleX = 9;
	ground.scaleY = 1;
	ground.scaleZ = 4;
	//ground.theta = 0;

	//��ʼ��һ������
	Object_t Cube; 
	Cube.pos = { 1, 2, 1 ,1 };
	Cube.axis = { 0, 0, 0, 0 };
	Cube.mesh = box_mesh;
	Cube.mesh_num = 34;
	Cube.scaleX = 3;
	Cube.scaleY = 3;
	Cube.scaleZ = 3;
	//Cube.theta = 1;  //��ǰ�������ת���ȡ�  1 ���� 180��

	Scene_render_Objs.push_back(Cube);
	Scene_render_Objs.push_back(ground);
}

void Init_Obj(Object_t& Cube)
{	//matrix_set_rotate(&m, Cube.axis.x, Cube.axis.y, Cube.axis.z, 0, Cube.pos.x, Cube.pos.y, Cube.pos.z); //theta  �����屾���x,y,z����Ե���ת

	matrix_t s, r, t;
	//TODO: ���� ��ת ��λ�� ��Ҫ�ľ���

//���� ��ǰ����ԭ������������ϵ�е�λ�ú���ת��  �������������
//axis.x ��X�����ת�Ƕ�

//����ת��������ϵ  	//ƽ��-> ��ת-������ 
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
	//��ʼ���������
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
		//��ʼ����Ӱ����� ������Ȳ��ԣ�Ȼ���趨����Ӱ��ɫ����ǰ������ɫ��ϡ������ؽ׶�ִ��
		camera* shadowCamera = &cameras[0];
		shadowCamera->eye = vector_t{0,10,10,1};
		shadowCamera->eyeTarget = vector_t{ 0,0,0,1 };
		shadowCamera->worldup = { 0, 1, 0, 1 };
	}
}
void InitDevice(device_t* device,camera& curCamera)
{
	device->cull = curCamera.cull;

	//�������Ļ�ƿ���
	device->transform.projection = curCamera.projection_trans;
	device->transform.view = curCamera.view;
	device->curCamera = curCamera;

}
void TestReference();
void TestReset();
void TestReset2();

int main(void)
{
	//TestReference();
	TestReset2();
	device_t device;
	//5��ģʽ�� �߿���䡢͸�Ӷ����ֵ��ɫ��͸���淨�ߡ�͸�Ӷ��㷨��  
	int states[] = { RENDER_STATE_WIREFRAME,RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_surfaceNormal_color ,RENDER_STATE_verterNormal_color };

	char * states_text[] = { RENDER_STATE_WIREFRAME_TEXT,RENDER_STATE_TEXTURE_TEXT, RENDER_STATE_COLOR_TEXT, RENDER_STATE_surfaceNormal_color_TEXT ,RENDER_STATE_verterNormal_color_TEXT };

	int indicator = 0; //ģʽ����������������Խ��
	int kbhit = 0;//һ��boolֵ  ��֤ÿһִֻ֡��һ�μ�������
	float alpha = 1; //��ǰ�������ת���ȡ�  1 ���� 180��

	TCHAR *title = _T("coolRender -- ")_T("Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

	int width = 800;
	int height = 600;

	if (screen_init(width, height, title)) //��ʼ��WINDOWS���ڲ����ñ���
		return -1;

	Load_Tga();
	device_init(&device, width, height, screen_fb); //�豸��ʼ

	CreateCamera(&device, width, height);//��ʼ���������

	Init_ShadowCamera(&device);//��Ӱ�����

	Create_Obj();//��ʼ�������������

	init_texture(&device); //�����ʼ��

	device.render_state = RENDER_STATE_WIREFRAME;

	LPCSTR type = "Ĭ����Ⱦģʽ";

	char *renderType = RENDER_STATE_WIREFRAME_TEXT;
	float fps = 0;
	char out[200];
	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0)
	{
		fps = Get_FPS();

		screen_dispatch(); //�ַ�msg
		device_clear(&device, 1); //��ջ���
		//device_clear(&device); // Zbuffer frameBuffer

		if (screen_keys[VK_UP]) camera_main.eye.z += 0.01f; //�����ǰ��  pos -= 0.01f;
		if (screen_keys[VK_DOWN]) camera_main.eye.z -= 0.01f; //���������
		if (screen_keys[VK_LEFT]) alpha += 0.01f; //����������ת
		if (screen_keys[VK_RIGHT]) alpha -= 0.01f; //����������ת


		if (screen_keys[0x41]) beta -= 0.01f; //ŷ����
		if (screen_keys[0x42]) beta += 0.01f; //ŷ����

		camera_update(&camera_main); //��������ϸ��¾���
		//��̬�ƹ� ��Ӱ
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
			if (kbhit == 0)//��֤ÿ�ΰ���ִֻ��һ�μ�������
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


		//��Ⱦһ�������� 
	//	draw_box(&device, alpha);

		// ��Ⱦ����,�����豸
		(&device.transform)->screen_width = (float)width;
		(&device.transform)->screen_height = (float)height;


		//1.��ʼ������ľ��������
		for (int i = 0; i < Scene_render_Objs.size(); i++)
		{
			Scene_render_Objs[i].axis.y = alpha;
			Init_Obj(Scene_render_Objs[i]);
		}


		////2.��������Ӱ����Ȼ���
		InitDevice(&device, cameras[0]);

		if ((&device)->shadowbuffer != NULL) {
			for (int y = 0; y < (&device)->height; y++)
				for (int x = 0; x < (&device)->width; x++)
					(&device)->shadowbuffer[y * (&device)->width + x] = 0;
		}

		//��Ⱦ��Ӱ����Ȼ���
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

		//3.��Ⱦ����
		InitDevice(&device, camera_main);

		for (int i = 0; i < Scene_render_Objs.size(); i++)
		{
			draw_Object(Scene_render_Objs[i], &device);
		}

		sprintf(out, "%3.1f", fps);
		//strcpy(out, renderType);//����һ���ַ���p������q��  
		strcat(out, renderType);//���ڶ����ַ���hƴ�ӵ�q֮��

		//string outText = out;

		char c[8];
		itoa(device.cull, c, 16);
		strcat(out, c);
		type = (LPCSTR)out;
		Set2RayTracer();
		//RaytracerIT();
		//��������Ⱦ����
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
		TextOut(hDC, 0, 16, "3D��Ⱦdemo", strlen("3D��Ⱦdemo"));
	}
}