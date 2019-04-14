#pragma once
#include "shaderStruct.h"
#include "ShadingCalculate.h"
#include "Camera.h"
//����Ⱦ�У�rhw ��������ռ�תCVV�ռ��ʱ��������ռ�����w�ĸ����ĵ�����
//����������ռ�תcvv��ʱ��һ���Ǵ�4ά���ŵ�3ά����һ����ͬʱǡ��ʵ���˽���ԶС��Ч����

//z - test  �� ��Z/w��ת����-1��1ȡֵ��Χ�ľ����壬Ȼ�� �� ��Z+1)/2 ת�� 0-1��Χ��   Ȼ�����õ� 1/Z�� �������Ե�����жϡ� ����ԽԶ��ԭ����ZԽ���õ������������ԽС��Խ�����

//����������������жϣ���Ϊ����ͬ�������� �ý������жϾ��ȸ���Զ���жϾ��ȸ�С�� ���⣬�� 0�ĵ����������壩����ʾ��Ұ������Զ��һ����



//��׶��(frustum, ��һ������)�任������۲���(Canonical View Volume ���¼��CVV)

//unity Ҳ����������ϵ

//��������ϵ, ��������
//Xx Yx Zx 0
//Xy Yy Zy 0
//Xz Yz Zz 0
//Dx Dy Dz 1

//X(x, y, z), Y(x, y, z), Z(x, y, z)�ֱ��ʾ����ϵ���������, ͨ��Ϊ��׼������, ���Ǳ�׼������, ��ʾ�ڸ���Ŵ������������׼���ı���, D(x, y, z)��ʾ�ڸ�����ϵ�е���ƫ�Ƶ�λ��

//��׼����
//1 0 0 0
//0 1 0 0
//0 0 1 0
//0 0 0 1
//��ʾ X(1, 0, 0), Y(0, 1, 0), Z(0, 0, 1) ƫ��(0, 0, 0), ��X�Ǳ�׼������ X(4, 0, 0)�����еĵ㽫�Ŵ�4��.


//TODO:�Ժ�ͬʱ��Ⱦ�������

//=====================================================================
// ��Ⱦ�豸   �����Ⱦ�豸 ����ô�����Windwos��gdi�����ϵģ�
//=====================================================================

#define MAX_NUM_CAMERA 10

typedef struct {

	camera cameras[MAX_NUM_CAMERA];
	camera camera_main;
	
	int width;                  // ���ڿ��
	int height;                 // ���ڸ߶�

								//����������Ļ������  800*600*32
	IUINT32 **framebuffer;      // ���ػ��棺framebuffer[x] �����x��      framebuffer[x][y] �����x�е�y������

								//ÿ�����ص���� 800*600*4   32λϵͳ�char 1λ  short 2λ   float int long ����4λ ֻ��double��8λ
	float **zbuffer;            // ��Ȼ��棺zbuffer[y] Ϊ�� y��ָ��      zbuffer[x][y] �����x�е�y������
	float *shadowbuffer;        // ��Ӱ����
	IUINT32 **texture;          // ����ͬ����ÿ������
	int tex_width;              // ������
	int tex_height;             // ����߶�
	float max_u;                // ��������ȣ�tex_width - 1
	float max_v;                // �������߶ȣ�tex_height - 1
	int render_state;           // ��Ⱦ״̬
	IUINT32 background;         // ������ɫ
	IUINT32 foreground;         // �߿���ɫ

	//ÿ����������ߵ�������Ⱦ���ߣ��������ڻ��������Ӱ����Ⱦ���ߵĲ���
	int cull;   // 0:���ü�;1:�ü�����;2:�ü�����
}
device_t;

//namespace {
//	device_t* static_device_t;
//}

int transform_check_cvv(const vector_t *v);

void transform_homogenize_reverse(const transform_t *ts, point_t *x, const point_t *y);

void transform_homogenize(const transform_t *ts, point_t *y, const point_t *x);

void vertex_rhw_init(vertex_t *v);

//�����Ƕ����ֵ
void vertex_interp(int render_state, vertex_t *y, const vertex_t *x1, const vertex_t *x2, float t);

//����ɨ���ߵ����ز�ֵ  ����ÿ�����ص�Ĺ��ɵĲ�ֵ  ��������x,y,z,w ��ɫrpg
void vertex_division(int render_state, vertex_t *y, const vertex_t *x1, const vertex_t *x2, float width);

// ���������β��Ϊ 0-2 �������Σ����ҷ��غϷ������ε�����
int trapezoid_init_triangle(trapezoid_t *trap, const vertex_t *p1, const vertex_t *p2, const vertex_t *p3);

//��ǰ���б�ʣ��Ż�
//����Y���������������������������Y�Ķ���
void trapezoid_edge_interp(device_t *device, trapezoid_t *trap, float y);

// �����������ߵĶ˵㣬��ʼ�������ɨ���ߵ����Ͳ���
void trapezoid_init_scan_line(const trapezoid_t *trap, scanline_t *scanline, int y);

// ���� 1.��դ��2D�㣨�����ڶ�ά�����ϻ��㣬�˽�ɫ�ʻ���ԭ�������Ӱ��������⣩
void device_pixel(device_t *device, int x, int y, IUINT32 color);

// x y������������������
// �����߶� 2.��դ��2Dֱ�ߣ�����ɭ�hķֱ�����㷨����С��ֱ���㷨�ȣ� ���-�յ㣬Ѱ���߶������άդ����������ص�
void device_draw_line(device_t *device, int x1, int y1, int x2, int y2, IUINT32 c);

// ���������ȡ����
IUINT32 device_texture_read(const device_t *device, float u, float v);

// ����ɨ����
void device_draw_scanline(device_t *device, scanline_t *scanline, float surfaceLight);

// ����Ⱦ����   ��Ⱦһ��������
void device_render_trap(device_t *device, trapezoid_t *trap, float surfaceLight);

// ���� render_state ����ԭʼ������   
void device_draw_primitive(device_t *device, vertex_t *v1, vertex_t *v2, vertex_t *v3);

// ���õ�ǰ����
void device_set_texture(device_t *device, void *bits, long pitch, int w, int h);

//void vert_shader(device_t *device, a2v *av, v2f *vf);

void frag_shader(device_t *device, v2f *vf, color_t *color);

void device_init(device_t *device, int width, int height, void *fb);

void device_destroy(device_t *device);

void device_clear(device_t *device);

// ��� framebuffer �� zbuffer
void device_clear(device_t *device, int mode);
