#include <tchar.h>
#include <time.h>  
#include <Windows.h>
#include "Pipline.h"
#include "CMaterail.h"
#include "Camera.h"
#pragma comment( lib,"winmm.lib" )
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

int screen_init(int w, int h, const TCHAR *title);	// ��Ļ��ʼ��
int screen_close(void);								// �ر���Ļ
void screen_dispatch(void);							// ������Ϣ
void screen_update(LPCSTR);							// ��ʾFrameBuffer

													// win32 event handler
static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);

//����������Ⱦ����

Object_t box_obj;
Object_t box_obj_new;

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib") //gdi��ͼ
#pragma comment(lib, "user32.lib")
#endif

//һ������mesh����ĺ���
void GetMesh(vertex_t* meshPtr)
{
	//TODO:
	

}




// ��ʼ�����ڲ����ñ���
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

	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0); //����֡����ptr
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


//��������Ⱦ����
void screen_update(LPCSTR type) {
	//Device Contexts ���
	HDC hDC = GetDC(screen_handle); 
	//HDC�豸��������һ�ְ����й�ĳ���豸������ʾ�����ӡ�����Ļ���������Ϣ�� Windows ���ݽṹ�����л��Ƶ��ö�ͨ���豸�����Ķ�����У���Щ�����װ�����ڻ�����������״���ı��� Windows API��

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


void draw_Object(object_simple Cube, device_t *device, float theta)
{
	matrix_t m;

	matrix_set_rotate(&m, Cube.axis.x, Cube.axis.y, Cube.axis.z, theta, Cube.pos.x, Cube.pos.y, Cube.pos.z); //theta  �����屾���x,y,z����Ե���ת

	device->transform.model = m;
	transform_update(&device->transform);

	//TODO���������Ӱ
	device->transform_shadow.model = m;
	transform_update(&device->transform_shadow);

	//ֱ����Ⱦ������
	for (int i = 0; i < Cube.mesh_num; i += 3)
	{
		//�Ժ�ֻ�иı��ʱ�򣬲�ȥ��̬����һ��
		vertex_t v1 = (Cube.mesh)[i];
		vertex_t v2 = (Cube.mesh)[i+1];
		vertex_t v3 = (Cube.mesh)[i+2];
		vector_scale(&v1.pos, Cube.scale);
		vector_scale(&v2.pos, Cube.scale);
		vector_scale(&v3.pos, Cube.scale);

		device_draw_primitive(device, &v1, &v2, &v3);
	}
}


//���ݽ�����theta ��ʼֵΪ1, �����������ת�Ƕȣ�����ľֲ�����ϵ�������������ϵ����ת��
void draw_box(device_t *device, float theta)
{
	matrix_t m;
	//���ﴫ�ݵ��� (-1, -0.5, 1)���ָ������
	matrix_set_rotate(&m, -3, -0.5, 1, theta,-3,0,5); 
	matrix_set_rotate(&m, -3, -0.5, 1, theta,0,0,0); 

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

camera* shadowCamera;


int main(void)
{
	device_t device;
	//5��ģʽ�� �߿���䡢͸�Ӷ����ֵ��ɫ��͸���淨�ߡ�͸�Ӷ��㷨��  
	int states[] = { RENDER_STATE_WIREFRAME,RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_surfaceNormal_color ,RENDER_STATE_verterNormal_color };
	int indicator = 0; //����ģʽ����������������Խ��
	int kbhit = 0;//һ��boolֵ  ��֤ÿһִֻ֡��һ�μ�������
	float alpha = 1; //��ǰ�������ת���ȡ�  1 ���� 180��
	float pos = -3.5;

	TCHAR *title = _T("coolRender -- ")_T("Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

	if (screen_init(800, 600, title)) //��ʼ��WINDOWS���ڲ����ñ���
		return -1;

	device_init(&device, 800, 600, screen_fb); //�豸��ʼ

	//��ʼ���������
	camera_main.pos = { 0, 0, pos, 1 }; //(point_t) { pos, 0, 0, 1 };
	camera_main.front =  { 0, 0, 0, 1 };
	camera_main.worldup ={ 0, 1, 0, 1 };//{ 0, 0, 1, 1 };

	//��ʼ����Ӱ�����
	shadowCamera = &cameras[0];
	shadowCamera->pos = lightPosition;
	shadowCamera->front =  { 0, 0, 0, 1 };
	shadowCamera->worldup = { 0, 1, 0, 1 };




	init_texture(&device); //�����ʼ��

	device.render_state = RENDER_STATE_WIREFRAME;

	LPCSTR type = "Ĭ����Ⱦģʽ";

	//��ʼ��һ������
	object_simple Cube;
	Cube.pos =  { 0, 0, 0 ,1};
	Cube.axis = { -1, -0.5, 1, 0 };
	Cube.mesh = box_mesh;
	Cube.mesh_num = 34;
	Cube.scale = 1;

	
	//��ʼ��һ���ذ�
	object_simple ground;
	ground.pos =  { 0, -2, 3,1 };
	Cube.axis = { -1, -0.5, 1, 0 };
	ground.pos =  { -2, -2, 1,1 };
	Cube.axis =  { -1, -0.5, 1, 0 };
	ground.mesh = ground_mesh;
	ground.mesh_num = 4;
	ground.scale = 6;


	char *renderType = "�߿�";
	float fps = 0;
	char out[200];
	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0)
	{
		fps = Get_FPS();

		screen_dispatch(); //�ַ�msg
		device_clear(&device, 1); //��ջ��� Zbuffer frameBuffer

		camera_main.pos = { 0, 0, pos, 1 }; //{ pos, 0, 0, 1 };
		camera_update(&device, &camera_main); //��������ϸ��¾�����Ϊposһֱ�仯

		camera_updateShadow(&device, shadowCamera);

		if (screen_keys[VK_UP]) pos += 0.01f; //�����ǰ��  pos -= 0.01f;
		if (screen_keys[VK_DOWN]) pos -= 0.01f; //���������
		if (screen_keys[VK_LEFT]) alpha += 0.01f; //����������ת
		if (screen_keys[VK_RIGHT]) alpha -= 0.01f; //����������ת

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
			if (kbhit == 0)  //��֤ÿһִֻ֡��һ�μ�������
			{
				kbhit = 1;
				if (++indicator >= 5)
					indicator = 0;
				device.render_state = states[indicator];

				if (indicator==0)
				{
					renderType = "�߿�";
				}
				else if(indicator == 1)
				{
					renderType = "����";
				}
				else if (indicator == 2)
				{
					renderType = "��ɫ";
				}
				else if (indicator == 3)
				{
					renderType = "�淨��";
				}
				else if (indicator == 4)
				{
					renderType = "���㷨��";
				}
			}
			
		}
		else
		{
			kbhit = 0;
		}

		sprintf(out, "%3.1f", fps);
		//strcpy(out, renderType);//����һ���ַ���p������q��  
		strcat(out, renderType);//���ڶ����ַ���hƴ�ӵ�q֮��

		type = (LPCSTR)out;


		//��Ⱦһ�������� 
		draw_box(&device, alpha);

		draw_Object(Cube, &device, alpha);
		draw_Object(ground,&device, 0);

		//��������Ⱦ����
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
	//	TextOut(, 0, 16, "3D��Ⱦdemo", strlen("3D��Ⱦdemo"));
	}
}



