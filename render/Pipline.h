#pragma once
#include "objectManager.h"
#include "ShadingCalculate.h"

//����Ⱦ�У�rhw ��������ռ�תCVV�ռ��ʱ��������ռ�����w�ĸ����ĵ�����
//����������ռ�תcvv��ʱ��һ���Ǵ�4ά���ŵ�3ά����һ����ͬʱǡ��ʵ���˽���ԶС��Ч����

//framebuffer zbuffer ���׸���ô�ã�
//z - test  �� ��Z/w��ת����-1��1ȡֵ��Χ�ľ����壬Ȼ�� �� ��Z+1)/2 ת�� 0-1��Χ��   Ȼ�����õ� 1/Z�� �������Ե�����жϡ� ����ԽԶ��ԭ����ZԽ���õ������������ԽС��Խ�����

//͸������ӳ�� /ɫ����� /�߿�ͼ

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

extern int render_state;

int transform_check_cvv(const vector_t *v);

void transform_homogenize(const transform_t *ts, point_t *y, const point_t *x);

void vertex_rhw_init(vertex_t *v);

//�����Ƕ����ֵ
void vertex_interp(vertex_t *y, const vertex_t *x1, const vertex_t *x2, float t);

//����ɨ���ߵ����ز�ֵ  ����ÿ�����ص�Ĺ��ɵĲ�ֵ  ��������x,y,z,w ��ɫrpg
void vertex_division(vertex_t *y, const vertex_t *x1, const vertex_t *x2, float width);

// ���������β��Ϊ 0-2 �������Σ����ҷ��غϷ������ε�����
int trapezoid_init_triangle(trapezoid_t *trap, const vertex_t *p1, const vertex_t *p2, const vertex_t *p3);

//��ǰ���б�ʣ��Ż�
//����Y���������������������������Y�Ķ���
void trapezoid_edge_interp(trapezoid_t *trap, float y);

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
void device_draw_scanline(device_t *device, scanline_t *scanline, float surfaceLight,  point_t* wolrdPoss);

// ����Ⱦ����   ��Ⱦһ��������
void device_render_trap(device_t *device, trapezoid_t *trap, float surfaceLight, point_t* wolrdPoss);

// ���� render_state ����ԭʼ������   
void device_draw_primitive(device_t *device, vertex_t *v1, vertex_t *v2, vertex_t *v3);