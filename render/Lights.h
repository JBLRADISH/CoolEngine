#pragma once
#include <windows.h>
#include "renderMath.h"

//���Դ
struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	vector_t ambient;
	vector_t diffuse;
	vector_t specular;

	vector_t position;//��Դλ��

	vector_t att;     //˥��ϵ��

	float range;      //���շ�Χ
};

//rgb   �� ǿ�� ���ڻ�����
typedef struct {
	float r, g, b, intension;
	bool shadow;   //�Ƿ������Ӱ
} Light_t;


//����� λ������ν
typedef struct {
	vector_t dir;
	vector_t ambi;
	//color_t ambi;
	//color_t diff;
	//color_t spec;
	bool shadow;
} dirlight_t;
