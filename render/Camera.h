#pragma once
#include <stdbool.h>
#include "shaderStruct.h"
#include "renderMath.h"

extern void matrix_Obj2World(matrix_t *m, vector_t rot, vector_t pos, float scale);

typedef enum {
	perspective, //͸��
	orthographic //����
} PROJECTION;

typedef struct  camera_temp
{
	vector_t eye; //��ǰ����
	vector_t eyeTarget; //����Ľ���
	int cull;   // 0:���ü�;1:�ü�����;2:�ü�����

	transform_t transform;

	//��Unity������ʹ��Transform�����Rotation
	vector_t rotation;


	vector_t front; //��ǰ����
	vector_t worldup;  //����


	matrix_t view_matrix;
	matrix_t projection_matrix;
	matrix_t view_matrix_r;

	//���
	int width;
	int height;

	//Maya Camera��Angle of View��Horizontal FOV�ĽǶ�
	//Unity Camera��FOV��Vertical FOV�ĽǶ�
	float fov;  //fov����ʱ���ֱ������ţ�����������

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
	float aspect; //�ݺ�ȣ������������ͷ����ȫ�� = ��Ļ�߿��
}camera;


void CameraInit();

void matrix_set_lookat(matrix_t *m, const vector_t *eye, const vector_t *at, const vector_t *up);

void matrix_set_perspective(matrix_t *m, float fovy, float aspect, float zn, float zf);

void camera_update(camera * caneraMain);
void camera_updateShadow(camera * caneraMain);