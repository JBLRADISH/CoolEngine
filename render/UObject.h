#pragma once
#include <stdbool.h>
#include "Shader.h"
#include "CMaterail.h"


//���ǵĽṹģ�ͣ�UObject = Model +Tramsform +materal +shader
//��Ⱦʱ�Ľṹ  vertex_t edge_t +���� 
//�����ص�˵�� pos��Wֵ�� ��color_t��Wֵ������
//pos������ϵת����Ϊ��ͳһת������չ������ά�� pos��Wֵ�����������4ά��3ά�Ļ�ԭ��Ҳ����CVV����դ���Ľ׶�
//color_t��Wֵ�������ڼ�¼�ö���/���ص����ȣ���������޳�

typedef struct {
	point_t pos;//λ�� �����
				 
	Vec2f tc; //��������
	color_t color; //��ɫ       
	Vec4f normal; //���Ĵ������ӷ���    ������ϲ� float rhw �ϲ���;  //�ö����֮ǰ��w�ĵ����� Ҳ�������Ŵ�С
	point_t worldPos;  //֮ǰ�����������λ�� �����shader�в���Ҫ
	float vertexLight; //����ƹ��ǿ��
	float shadowPos_z; //ͶӰ������Ĺ�դ��λ�� �����shader�в���Ҫ
} vertex_t; //����

			//���������εı�Ե
typedef struct {
	vertex_t v,  //ɨ���� �� б�ߵĽ���
		v1, //������б�ߵ�ԭʼ���
		v2; //������б�ߵ�ԭʼ�յ�
} edge_t; //��Ե

		  //������
typedef struct {
	float top, bottom; //�����ε�����
	edge_t left, right;  //�����ε�����б��  ��ʵ �����εĸ߶ȣ��Ѿ�ȷ����

} trapezoid_t; //����

			   //��Ⱦ���ε�ʱ���ɨ����
typedef struct {
	vertex_t v, step; //���
	int x,
		y,
		width;  //���͵�ɨ���߿��
} scanline_t; //ɨ����


void vertex_add(vertex_t *y, const vertex_t *x);


//����
typedef struct
{
	unsigned long mesh_num;//..

	point_t pos;
	Vec4f axis;
	Vec4f scale;
	//mesh[0]��mesh[]��ռ�ÿռ䣬�ҵ�ַ�����ڽṹ���棬��vertex_t *data��Ϊָ�룬ռ��4���ֽڣ���ַ���ڽṹ֮��
	//vertex_t mesh[0]; //��ʼ��ַ �ɱ�����   vertex_t data[0]��vertex_t *data, vertex_t data[]������ 
	vertex_t* mesh;

	IUINT32 texture[256][256]; //����
	Matrix44f model;         // �������  ���������ȼ������


	materal m_materal; //����

	//TODO���Ժ�����

	//int* material_ids; //ֻ�洢ID����

	//int texture_id;
	//bool shadow;  //�Ƿ���������Ӱ  TODO���Ժ�����Ƿ������Ӱ
}
Object_t;