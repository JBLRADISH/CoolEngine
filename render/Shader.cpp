#include "Shader.h"
#include "UObject.h"
#include "ShadingCalculate.h"
#include "Pipline.h"



//IShader::~IShader() {}

//Ϊ��ʹ��GPU�Ĳ�������

//�Զ���ִ�� 
//1.����任��͸��ͶӰ��  
//2.�𶥵���ռ��㡢���㶯����ɫ����Ƥ��������㡢����
//1.��������ϵ����������ϵ��ת��
//2.CVV�ռ�ü�(��׶�ü�)
//2.��ʼ���� uv�ĸ�ֵ


//�������ɱ任�Լ����յĶ��㣬��λ�úͷ����������������ռ��ʾ��

//���� Vec4f ���������
Vec3f Shader::vertex(Vec3f iface, int nthvert) {
	Vec3f gl_Vertex = iface;

	return gl_Vertex;
}

//�м��դ��

//���ؽ׶Σ����붥��
bool Shader::fragment(Vec3f bar, TGAColor& color) {




	return false;
}