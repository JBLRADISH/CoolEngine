#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <windows.h>
#include <tchar.h>
#include <stdbool.h>
#include "geometry.h"
#include"geometry.h"
#define PI 3.141592653
#define angle_to_radian(X) ((X)/180*PI)  //�Ƕ�ת����
#define radian_to_angle(X) ((X)/PI*180)  //����ת�Ƕ�

typedef unsigned int IUINT32;

void matrix_Obj2World(matrix_t<4,4,float> *m, Vec4f rot, Vec4f pos);

void matrix_World2Obj(matrix_t<4, 4, float>*m, Vec4f rot, Vec4f pos, float scale);

//����ϵ�ı仯 = �������λ��+����ϵ����ת   ����ϵ����ת �� ��������ϵ�ڲ���������ת��һ����
//��������ǻ���ָ��������ת����ת���� ��ָ������Ϊ (x,y,z)
void matrix_set_rotate(matrix_t<4, 4, float>*m, float x, float y, float z, float theta, float xOffset, float yOffest, float zOffset);






 