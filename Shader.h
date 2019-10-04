#pragma once
#include "render/geometry.h"
#include "render/Camera.h"
#include "render/Model.h"

//class IShader {
//	virtual ~IShader();
//	virtual Vec4f vertex(int iface, int nthvert) = 0;
//	virtual bool fragment(Vec3f bar, TGAColor& color) = 0;
//};


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