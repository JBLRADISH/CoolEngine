#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <windows.h>
#include <tchar.h>
#include <stdbool.h>
#include "geometry.h"



#define PI 3.141592653
#define angle_to_radian(X) ((X)/180*PI)  //�Ƕ�ת����
#define radian_to_angle(X) ((X)/PI*180)  //����ת�Ƕ�

typedef unsigned int IUINT32;
struct matrix_t {
	
	
	float m[4][4];

	matrix_t& operator* (float f) {
		int i, j;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++)
				this->m[i][j] = this->m[i][j] * f;
		}
		return *this;
	}


	matrix_t operator* (const matrix_t& right) //�����Ǹ���������ָ������Ҫ����紫��ָ��
	{
		matrix_t c;
		int i, j;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				c.m[i][j] =
					(this->m[i][0] * right.m[0][j]) +
					(this->m[i][1] * right.m[1][j]) +
					(this->m[i][2] * right.m[2][j]) +
					(this->m[i][3] * right.m[3][j]);
			}
		}
		return c;
	}

	matrix_t operator-(const matrix_t & b) {
		matrix_t c;
		int i, j;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++)
				c.m[i][j] = this->m[i][j] - b.m[i][j];
		}
		return c;
	}

	matrix_t operator+(const matrix_t & b) {
		matrix_t c;
		int i, j;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++)
				c.m[i][j] = this->m[i][j] + b.m[i][j];
		}
		return c;
	}


} ;




////������Ҳ�� ʹ�ýṹ�屾����ڴ���뿽��(�����ǰ�λ����)
//Vec4f& operator=(const Vec4f&  value)
//{
//	Vec4f ncolor_t;
//	this->x = value.x;
//	this->y = value.y;
//	this->z = value.z;
//	this->w = value.w;
//	return *this;
//}
//
//Vec4f operator*(float value)
//{
//	Vec4f ncolor_t;
//	ncolor_t.x = x * value;
//	ncolor_t.y = y * value;
//	ncolor_t.z = z * value;
//
//	return ncolor_t;
//}
//
//
//Vec4f operator+(const Vec4f& value)
//{
//	Vec4f ncolor_t;
//	ncolor_t.x = x + value.x;
//	ncolor_t.y = y + value.y;
//	ncolor_t.z = z + value.z;
//
//	return ncolor_t;
//}
//
//Vec4f operator-(const Vec4f& value)
//{
//	Vec4f ncolor_t;
//	ncolor_t.x = x - value.x;
//	ncolor_t.y = y - value.y;
//	ncolor_t.z = z - value.z;
//
//	return ncolor_t;
//}
//
//
//} ; //ʹ�õ�3D�������  �䳣������Ϊ {x/w, y/w, z/w; }
//typedef Vec4f point_t;



//�������������� 
float CMIDFloat(float x, float min, float max);

//��������������
int CMID(int x, int min, int max);


// �����ֵ��t Ϊ [0, 1] ֮�����ֵ lerp��ֵ
float interp(float x1, float x2, float t);

// ʸ��lerp��ֵ��tȡֵ [0, 1]
void vector_interp(Vec4f *z, const Vec4f *x1, const Vec4f *x2, float t);

//void matrix_mul(matrix_t *c, const matrix_t *a, const matrix_t *b);

// ����˷���   y = x * m   	//����/�㣬��תΪ�����, ���� �� ����.  ʵ������ת��
void matrix_apply(Vec4f	*y, const Vec4f *x, const matrix_t *m);

//��׼���� 4X4 
void matrix_set_identity(matrix_t *m);

//�����
void matrix_set_zero(matrix_t *m);

// ƽ�Ʊ任
void matrix_set_translate(matrix_t *m, float x, float y, float z);

// ���ű任
void matrix_set_scale(matrix_t *m, float x, float y, float z);

void matrix_Obj2World(matrix_t *m, Vec4f rot, Vec4f pos);

void matrix_World2Obj(matrix_t *m, Vec4f rot, Vec4f pos, float scale);

//����ϵ�ı仯 = �������λ��+����ϵ����ת   ����ϵ����ת �� ��������ϵ�ڲ���������ת��һ����
//��������ǻ���ָ��������ת����ת���� ��ָ������Ϊ (x,y,z)
void matrix_set_rotate(matrix_t *m, float x, float y, float z, float theta, float xOffset, float yOffest, float zOffset);






