#pragma once
#include "objectManager.h"

//����ģ�ͽ���

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

//TODO:�����������ֹ���ģ�ͣ� BRDFģ���µ������� ��
//1.Lambertģ�ͣ�����������ģ�ͣ�����ͬ�ԣ� Idiffuse = Kd*Id*cos��

//1.����Shader
float calculateVertexLight(const point_t *v1, vector_t* normal);
//2.ƽ��Shader
float calculateGroudShader(const point_t *v1, const point_t *v2, const point_t *v3);

//3.����������
color_t Lambert(transform_t* mainTrans, vector_t *v_Obj, vector_t* normal, vector_t* lightPos, color_t diffuseColor, color_t ambientColor);

//4.������߹�������� Ҳ����Phong����ģ��
color_t Phong(transform_t* mainTrans, vector_t *posInObj, vector_t *normal, vector_t* lightPos, vector_t* cameraPos, color_t diffuseColor, color_t ambientColor, color_t specularColor);


//5. blinn shading