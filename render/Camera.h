#pragma once
#include <stdbool.h>
#include "shaderStruct.h"


typedef enum {
	perspective, //͸��
	orthographic //����
} PROJECTION;

typedef struct  camera_temp
{
	//x�������⣬z���ϣ�y����

	// ����� (x,y,z) �൱��DX ��� (z,x,y)

	// public
	point_t pos; //λ��
	vector_t front; //��ǰ
	vector_t worldup;  //����
	matrix_t view_matrix;
	matrix_t projection_matrix;
	matrix_t view_matrix_r;

	//���
	int width;
	int height;

	float fovy;//
	float zn; //������
	float zf; //Զ����

			  //��������
	float left;
	float right;
	float bottom;
	float top;

	bool dirty; //�Ƿ����
	PROJECTION projection;  //�����ȡ������
	bool main;  //�Ƿ����������

				// private
	float aspect; //�ݺ�ȣ���Ļ�߿��
}camera;

extern float Forwardoffset;
extern camera camera_main;

#define MAX_NUM_CAMERA 10
extern camera cameras[];

void CameraInit();

void matrix_set_lookat(matrix_t *m, const vector_t *eye, const vector_t *at, const vector_t *up);

void matrix_set_perspective(matrix_t *m, float fovy, float aspect, float zn, float zf);

// ��ʼ����������Ļ����
void transform_init(transform_t *ts, int width, int height);

void camera_update(device_t *device, camera * caneraMain);
void camera_updateShadow(device_t *device, camera * caneraMain);

void Forward();

void device_init(device_t *device, int width, int height, void *fb);

void device_destroy(device_t *device);

// ���õ�ǰ����
void device_set_texture(device_t *device, void *bits, long pitch, int w, int h);

// ��� framebuffer �� zbuffer
void device_clear(device_t *device, int mode);