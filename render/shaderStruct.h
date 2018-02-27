#ifndef shaderBase_h
#define shaderBase_h

#include "renderMath.h"
//#include "objectManager.h"
// ǰ����ģʽ��û����ɫ���̣�Ҳ����Ҫ��ɫ��


#define RENDER_STATE_WIREFRAME      1		// ��Ⱦ�߿�
#define RENDER_STATE_TEXTURE        2		// ��Ⱦ����
#define RENDER_STATE_COLOR          4		// ��Ⱦ��ɫ �������Ǹ��ݶ������(��Z������)����   ʹ�õ��Ǹ��ݶ�����������ż�������ɫ��Ȼ�󶥵�������ֵ��

#define RENDER_STATE_surfaceNormal_color         8		//�淨�ߵ���ɫ��ʾ   Ҳ������ν��ƽ����ɫ��ÿ��ͼԪ�����ض���һ�µĸ����ͼԪ�ĵ�һ���������ɫ��
#define RENDER_STATE_verterNormal_color         16		//���㷨�ߵ���ɫ��ʾ   Ҳ������ν�Ķ�����ɫ��������ɫ��������Gouraud��ɫ��ͼԪ�еĸ����ص���ɫֵ�ɶ������ɫ�����Բ�ֵ�õ���

typedef struct { float r, g, b, a;

	color_t operator*(float value)
	{
	color_t ncolor_t;
	ncolor_t.r = r*value;
	ncolor_t.g = g*value;
	ncolor_t.b = b*value;
	ncolor_t.a = a*value; //TODO:͸��ֵ���������Ժ󲻼���
	return ncolor_t;
	}
} color_t; //��ɫ




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

	//ò��û��ģ�;���

	matrix_t model;         // �������ÿ�����嶼���Լ����������
	matrix_t view;          // ��Ӱ������任 ���絽���������任����
	matrix_t projection;    // ͶӰ�任 �����Ǳ任���������Զƽ��-��ƽ��ռ���
	matrix_t mvp;     // transform = world * view * projection
	float screen_width, screen_height;             // ��Ļ��С

	matrix_t vp;           // view * projection
	matrix_t mv;           // model * view
}
transform_t;

//TODO:�Ժ�ͬʱ��Ⱦ�������

//=====================================================================
// ��Ⱦ�豸   �����Ⱦ�豸 ����ô�����Windwos��gdi�����ϵģ�
//=====================================================================
typedef struct {
	transform_t transform;      // ����任��
	transform_t transform_shadow;      // ����任��
	int width;                  // ���ڿ��
	int height;                 // ���ڸ߶�

								//����������Ļ������  800*600*32
	IUINT32 **framebuffer;      // ���ػ��棺framebuffer[x] �����x��      framebuffer[x][y] �����x�е�y������

								//ÿ�����ص���� 800*600*4   32λϵͳ�char 1λ  short 2λ   float int long ����4λ ֻ��double��8λ
	float **zbuffer;            // ��Ȼ��棺zbuffer[y] Ϊ�� y��ָ��      zbuffer[x][y] �����x�е�y������
	float *shadowbuffer;        // ��Ӱ����
	IUINT32 **texture;          // ����ͬ����ÿ������
	int tex_width;              // ������
	int tex_height;             // ����߶�
	float max_u;                // ��������ȣ�tex_width - 1
	float max_v;                // �������߶ȣ�tex_height - 1
	int render_state;           // ��Ⱦ״̬
	IUINT32 background;         // ������ɫ
	IUINT32 foreground;         // �߿���ɫ

	int cull;   // 0:���ü�;1:�ü�����;2:�ü�����
}
device_t;

// �������
void transform_update(transform_t *ts);
//void transform_update(transform_t *ts, object_simple Obj);

void transform_apply(const transform_t *ts, vector_t *posInCVV, const vector_t *posInObj);

//void vert_shader(device_t *device, a2v *av, v2f *vf);

//void frag_shader(device_t *device, v2f *vf, color_t *color);

void transform_applyVP(const transform_t *ts, vector_t *posInCVV, const vector_t *posInWorld);


#endif