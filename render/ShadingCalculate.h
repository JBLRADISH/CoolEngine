#pragma once
#include "objectManager.h"
//������ɫ��ʽ

//Phong shading
//blinn shading

//Groud Shader

//VertexLight


//���㷨������
float ComputeNDotL(const point_t* vertex, const vector_t* normal, const point_t* lightPosition);

//���㶥�㵽�ƹ�ľ���,���ڲ�����Ӱ
//float DisVertexToLight(const point_t *Light, vector_t* VertertPosInWorld);

//����
float calculateVertexLight(const point_t *v1,  vector_t* normal);
//ƽ��
float calculateGroudShader(const point_t *v1, const point_t *v2, const point_t *v3);

float ComputeCameraToVertor(point_t* vertex, const vector_t* normal, const point_t* BBBCameraToVertor);

float CullCalcutate(const point_t *v1, const point_t *v2, const point_t *v3, const point_t *camera_pos);