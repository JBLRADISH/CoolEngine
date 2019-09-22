#pragma once
#include "render/geometry.h"
#include "render/Camera.h"
#include "render/Model.h"

class Shader
{
public:
	//��ǰ�ĸ��������Ⱦ��
	camera curCamera;

	Model *mModel; //��ʱ����һ������

	//��Ļ�ֱ���

	int tex_width;              // ������
	int tex_height;             // ����߶�
	IUINT32** texture; //[256][256]; //����

	Vec2f _uv;  //UV  // ��������ȣ�tex_width - 1  // �������߶ȣ�tex_height - 1
	Vec3f _verter; //����

	int cull;   // 0:���ü�;1:�ü�����;2:�ü�����

};


//��ÿ������ʹ��ÿ��shader��Ⱦ ��д��buffer�����ã��У����ﴦ����Ļ�ֱ��ʣ�
//����devece �����bufferд����Ļ�����ﴦ��