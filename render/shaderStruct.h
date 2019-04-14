#ifndef shaderBase_h
#define shaderBase_h
#include <stdbool.h>
#include<vector>
using namespace std;
#include "renderMath.h"

// ǰ����ģʽ��û����ɫ���̣�Ҳ����Ҫ��ɫ��


#define RENDER_STATE_WIREFRAME_TEXT     "��Ⱦ�߿�"
#define RENDER_STATE_TEXTURE_TEXT        "��Ⱦ����"
#define RENDER_STATE_COLOR_TEXT          "������ɫ" //�����Ǹ��ݶ������(��Z������)����   ʹ�õ��Ǹ��ݶ�����������ż�������ɫ��Ȼ�󶥵�������ֵ��

#define RENDER_STATE_surfaceNormal_color_TEXT   "�淨�ߵ���ɫ��ʾ"   //Ҳ������ν��ƽ����ɫ��ÿ��ͼԪ�����ض���һ�µĸ����ͼԪ�ĵ�һ���������ɫ��
#define RENDER_STATE_verterNormal_color_TEXT     "���㷨�ߵ���ɫ��ʾ"   //Ҳ������ν�Ķ�����ɫ��������ɫ��������Gouraud��ɫ��ͼԪ�еĸ����ص���ɫֵ�ɶ������ɫ�����Բ�ֵ�õ���

enum Render_State
{
	RENDER_STATE_WIREFRAME = 1,
	RENDER_STATE_TEXTURE = 2,
	RENDER_STATE_COLOR = 4,
	RENDER_STATE_surfaceNormal_color = 8,
	RENDER_STATE_verterNormal_color = 16
};


 struct color_t { float r, g, b, a;

 color_t operator*(float value)
 {
	 color_t ncolor_t;
	 ncolor_t.r = r*value;
	 ncolor_t.g = g*value;
	 ncolor_t.b = b*value;
	 ncolor_t.a = a*value; //TODO:͸��ֵ���������Ժ󲻼���
	 return ncolor_t;
 }


color_t operator+(color_t value)
{
	color_t ncolor_t;
	ncolor_t.r = r + value.r;
	ncolor_t.g = g + value.g;
	ncolor_t.b = b + value.b;
	ncolor_t.a = a + value.a;
	return ncolor_t;
}

//color_t operator-(color_t* value)
//{
//	color_t ncolor_t;
//	ncolor_t.r = r - value->r;
//	ncolor_t.g = g - value->g;
//	ncolor_t.b = b - value->b;
//	ncolor_t.a = a - value->a;
//	return ncolor_t;
//}
} ; //��ɫ

//typedef struct {
//	vector_t pos;
//	color_t color;
//	vector_t normal;
//	vector_t tangent;
//	vector_t binormal;
//	texcoord_t texcoord;
//} a2v;
//
//
//typedef struct {
//	vector_t pos;
//	texcoord_t texcoord;
//	color_t color;
//	vector_t normal;
//	vector_t storage0;
//	vector_t storage1;
//	vector_t storage2;
//} v2f;


typedef struct { float u, v; } texcoord_t; //����


typedef struct {
	vector_t pos; //λ��
	color_t color; //��ɫ
	vector_t normal; //����
	vector_t tangent; //����
	vector_t binormal; //������
	texcoord_t texcoord; //uv����
} a2v;

typedef struct {
	vector_t pos;
	texcoord_t texcoord;
	color_t color;
	vector_t normal;
	vector_t storage0;
	vector_t storage1;
	vector_t storage2;
} v2f;

//=====================================================================
// ����任
//=====================================================================
typedef struct {

	float screen_width, screen_height;             // ��Ļ��С

	//ò��û��ģ�;���

	matrix_t model;         // ����-�������ÿ�����嶼���Լ����������TODO: �Ժ���뵽ÿ���������
	matrix_t view;          // ��Ӱ������任 ���絽���������任����  TODO:�Ժ���뵽ÿ�����������
	matrix_t projection;    // ͶӰ�任 �����Ǳ任���������Զƽ��-��ƽ��ռ���  TODO:�Ժ���뵽ÿ�����������
	//����������ľ��루z��Ҳ����ˣ����ھ�������x��y������������㣬�������z����Ķ��㽫����Ļ�����ĸ����λ����Ļ�����ġ�     
	// ���⣬�����͸����ͼ��W�������������С����ģ���Ӿ���Զ���������С



	matrix_t mvp;     // transform = world * view * projection
	matrix_t vp;           // view * projection
	matrix_t mv;           // model * view
}
transform_t;


// �������
void transform_update(transform_t *ts);
//void transform_update(transform_t *ts, object_simple Obj);

void transform_apply(const transform_t *ts, vector_t *posInCVV, const vector_t *posInObj);


void transform_applyVP(const transform_t *ts, vector_t *posInCVV, const vector_t *posInWorld);

#endif