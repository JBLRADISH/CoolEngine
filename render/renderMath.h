#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <windows.h>
#include <tchar.h>
#include <stdbool.h>




#define PI 3.141592653
#define angle_to_radian(X) ((X)/180*PI)  //�Ƕ�ת����
#define radian_to_angle(X) ((X)/PI*180)  //����ת�Ƕ�

typedef unsigned int IUINT32;
typedef struct { float m[4][4]; } matrix_t;
struct vector_t  { float x, y, z, w;

vector_t()
{
	x = 0;
		y = 0;
		z = 0;
		w = 1;
}


vector_t(float ix, float iy, float iz, float iw)
{
	x = ix;
	y = iy;
	z = iz;
	w = iw;
}

vector_t operator*(float value)
{
	vector_t ncolor_t;
	ncolor_t.x = x * value;
	ncolor_t.y = y * value;
	ncolor_t.z = z * value;

	return ncolor_t;
}


vector_t operator+(const vector_t* value)
{
	vector_t ncolor_t;
	ncolor_t.x = x + value->x;
	ncolor_t.y = y + value->y;
	ncolor_t.z = z + value->z;

	return ncolor_t;
}

vector_t operator-(const vector_t* value)
{
	vector_t ncolor_t;
	ncolor_t.x = x - value->x;
	ncolor_t.y = y - value->y;
	ncolor_t.z = z - value->z;

	return ncolor_t;
}


} ; //ʹ�õ�3D�������  �䳣������Ϊ {x/w, y/w, z/w; }
typedef vector_t point_t;



//�������������� 
float CMIDFloat(float x, float min, float max);

//��������������
int CMID(int x, int min, int max);

void vector_scale(vector_t *z, float scale);

// �����ֵ��t Ϊ [0, 1] ֮�����ֵ lerp��ֵ
float interp(float x1, float x2, float t);

// | v |
float vector_length(const vector_t *v);


//�������ĵڶ�����Ч����������������.  wΪ1��Ϊ��.  wΪ0��Ϊ������  ��Ϊw������λ�ƣ�����λ��ûʲô���塣
// z = x + y  wһֱΪ1
void vector_add(vector_t *z, const vector_t *x, const vector_t *y);


// z = x - y
void vector_sub(vector_t *z, const vector_t *x, const vector_t *y);

// ʸ�����
float vector_dotproduct(const vector_t *x, const vector_t *y);


// ʸ�����  ��� x y ��׼������������Ĳ��ZҲ�Ǳ�׼����xyƽ��ķ��ߡ� ����Ļ���Ҫ��׼��һ��Z
void vector_crossproduct(vector_t *z, const vector_t *x, const vector_t *y);

// ʸ��lerp��ֵ��tȡֵ [0, 1]
void vector_interp(vector_t *z, const vector_t *x1, const vector_t *x2, float t);

// ʸ����һ��  ע���һ�������漰W�� ֻ����ά����άͶӰ��ʱ��W��һ��������
void vector_normalize(vector_t *v);

// c = a + b
void matrix_add(matrix_t *c, const matrix_t *a, const matrix_t *b);

// c = a - b
void matrix_sub(matrix_t *c, const matrix_t *a, const matrix_t *b);

//����˷���   z.m[j][i] = (a->m[j][0] * b->m[0][i]) +(a->m[j][1] * b->m[1][i]) +(a->m[j][2] * b->m[2][i]) +(a->m[j][3] * b->m[3][i]);
// c = a * b
void matrix_mul(matrix_t *c, const matrix_t *a, const matrix_t *b);

// c = a * f
void matrix_scale(matrix_t *c, const matrix_t *a, float f);

// ����˷���   y = x * m   	//����/�㣬��תΪ�����, ���� �� ����.  ʵ������ת��
void matrix_apply(vector_t *y, const vector_t *x, const matrix_t *m);

//��׼���� 4X4 
void matrix_set_identity(matrix_t *m);

//�����
void matrix_set_zero(matrix_t *m);

// ƽ�Ʊ任
void matrix_set_translate(matrix_t *m, float x, float y, float z);

// ���ű任
void matrix_set_scale(matrix_t *m, float x, float y, float z);

void matrix_Obj2World(matrix_t *m, vector_t rot, vector_t pos,float scale);

void matrix_World2Obj(matrix_t *m, vector_t rot, vector_t pos, float scale);

//����ϵ�ı仯 = �������λ��+����ϵ����ת   ����ϵ����ת �� ��������ϵ�ڲ���������ת��һ����
//��������ǻ���ָ��������ת����ת���� ��ָ������Ϊ (x,y,z)
void matrix_set_rotate(matrix_t *m, float x, float y, float z, float theta, float xOffset, float yOffest, float zOffset);






