#ifndef objectManager_h
#define objectManager_h
#include "shaderStruct.h"
#include <stdbool.h>

//���ڼ��������ģ�͡����񡢶��������ʡ���ͼ


//=====================================================================
// ���μ��㣺���㡢ɨ���ߡ���Ե�����Ρ���������
//=====================================================================


//�����ص�˵�� pos��Wֵ�� ��color_t��Wֵ������
//pos������ϵת����Ϊ��ͳһת������չ������ά�� pos��Wֵ�����������4ά��3ά�Ļ�ԭ��Ҳ����CVV����դ���Ľ׶�
//color_t��Wֵ�������ڼ�¼�ö���/���ص����ȣ���������޳�

typedef struct {
	point_t pos;//λ��
	texcoord_t tc; //��������
	color_t color; //��ɫ       
	vector_t normal; //���Ĵ������ӷ���    ������ϲ� float rhw �ϲ���;  //�ö����֮ǰ��w�ĵ����� Ҳ�������Ŵ�С
	point_t worldPos;  //֮ǰ�����������λ��
	point_t shadowPos; //ͶӰ������Ĺ�դ��λ��
	float vertexLight; //����ƹ��ǿ��
} vertex_t; //����


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


//����
typedef struct {
	unsigned long mesh_num;
	int* material_ids;
	int texture_id;  
	bool shadow;  //�Ƿ���������Ӱ  TODO���Ժ�����Ƿ������Ӱ

	bool dirty;
	point_t pos; //λ��
	vector_t scale; //����

	vector_t axis;
	float theta;
	matrix_t matrix;

	//ҲҲ����������
	//struct
	//{
		//int data;   // ������
		//struct node *next;   // ָ����
	//} *node;

	//mesh[0]��mesh[]��ռ�ÿռ䣬�ҵ�ַ�����ڽṹ���棬��vertex_t *data��Ϊָ�룬ռ��4���ֽڣ���ַ���ڽṹ֮��
	vertex_t mesh[0]; //��ʼ��ַ �ɱ�����   vertex_t data[0]��vertex_t *data, vertex_t data[]������ 
} object_t;

//����
typedef struct {
	unsigned long mesh_num;
	point_t pos; //λ��
	vector_t axis;
	float theta;
	float scale;
	vertex_t* mesh;
} object_simple;


void vertex_add(vertex_t *y, const vertex_t *x);

//�ƹ�  ���Դ  ���ڼ�����Ǿ�̬��Դ
extern vector_t lightPosition;

//�����⣬��ʱֻ��һ���������ǿ�Ⱥ���ɫ
extern Light_t AmbientLight;


//�Ժ���ĵ����������
//����
extern vertex_t ground_mesh[6];


//��β��� ��ɫ
extern vertex_t mesh[8];

//rhw  Ҳ����  reciprocal homogeneous w    ʵ������ͼ�ռ�������ĵ�-w�ĵ��������ڻ�ԭZ�ᣬ���������ͽ���ԶС



//��Ϊ��ɫ�ǵ㣬����w= 1��   normals������������w =0
extern vertex_t box_mesh[36];
#endif