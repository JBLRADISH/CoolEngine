#pragma once
#include "geometry.h"
#include "Camera.h"
#include "Model.h"

//class IShader {
//	virtual ~IShader();
//	virtual Vec4f vertex(int iface, int nthvert) = 0;
//	virtual bool fragment(Vec3f bar, TGAColor& color) = 0;
//};

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

typedef struct {
	Vec4f pos; //λ��
	color_t color; //��ɫ
	Vec4f normal; //����
	Vec4f tangent; //����
	Vec4f binormal; //������
	Vec2f texcoord; //uv����
} a2v;

typedef struct {
	Vec4f pos;
	Vec2f texcoord;
	color_t color;
	Vec4f normal;
	Vec4f storage0;
	Vec4f storage1;
	Vec4f storage2;
} v2f;


//Ϊ�˲���������Ҫ����Ϊ�����ͼԪ���������ֱ𲢷�
class Shader//:public IShader
{
public:
	//��ǰ�ĸ��������Ⱦ��
	camera curCamera;

	Model *mModel; //��ʱ����һ������

	transform_t transform;

	//Vec3f varying_tri;
	matrix_t<3, 3, float> varying_tri; //ͼԪ(������)���� triangle coordinates (clip coordinates), written by VS, read by FS

	int render_state;

	//��Ļ�ֱ���

	int tex_width;              // ������
	int tex_height;             // ����߶�
	IUINT32** texture; //[256][256]; //����

	Vec2f _uv;  //UV  // ��������ȣ�tex_width - 1  // �������߶ȣ�tex_height - 1
	Vec3f _verter; //����

		//ÿ����������ߵ�������Ⱦ���ߣ��������ڻ��������Ӱ����Ⱦ���ߵĲ���
	int cull;   // 0:���ü�;1:�ü�����;2:�ü�����

	virtual Vec3f vertex(Vec3f iface, int nthvert);

	//���㴦��������ö����ÿ�����һ��ͼԪ(�����õ������Σ���󲿷��������������)���򴫵ݵ�ͼԪ��Ⱦ����Ⱦ
	virtual bool fragment(Vec3f bar, TGAColor& color);
};
//��.obj�ļ��У����Ǿ����ԡ� vt u v����ͷ���У����Ǹ�����������������
//�����ߡ� fx / x / xx / x / xx / x / x�����м䣨б��֮�䣩�������Ǵ������ζ������������
//��������������ڣ���������ͼ��Ŀ�� - �߶ȣ��������Ҫ������Ⱦ�е���ɫ

//��ÿ������ʹ��ÿ��shader��Ⱦ ��д��buffer�����ã��У����ﴦ����Ļ�ֱ��ʣ�
//����devece �����bufferд����Ļ�����ﴦ��