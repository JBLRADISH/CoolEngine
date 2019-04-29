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

	matrix_t view_matrix_r;

	matrix_t view;          // ��Ӱ������任 ���絽���������任����  TODO:�Ժ���뵽ÿ�����������
	matrix_t projection_trans;    // ͶӰ�任 �����Ǳ任���������Զƽ��-��ƽ��ռ���  TODO:�Ժ���뵽ÿ�����������
	//����������ľ��루z��Ҳ����ˣ����ھ�������x��y������������㣬�������z����Ķ��㽫����Ļ�����ĸ����λ����Ļ�����ġ�     
	// ���⣬�����͸����ͼ��W�������������С����ģ���Ӿ���Զ���������С


	//��Unity������ʹ��Transform�����Rotation
	vector_t rotation;


	vector_t front; //��ǰ����
	vector_t worldup;  //����



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
void camera_updateShadow(camera * caneraShadow,camera* caneraMain);