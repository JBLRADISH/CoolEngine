#include "shaderStruct.h"

//TODO:��Ҫ���Ϊ ��֡���¡����������
// �������
void transform_update(transform_t *ts) {

	ts->mv = ts->model * ts->view;
	ts->vp = ts->view * ts->projection;
	ts->mvp = ts->mv * ts->projection;

}


//�����ǡ���������˾���ʱ����������������������ҳ˾���ʱ����������������������ǲ��еġ�����DirectX��ʹ�õ�������������OpenGL��ʹ�õ�����������
//�����õ��ҳ�

// w ҲҪ���뵽����任�ļ���  ��ʱ����CVV��ʱ�� x,y,z,wҪ����W���˿� w=1,ͶӰ����Ļ������
//�õ�����Ļ�ϵ�����. (������Ļ��Zֵ��1)

void transform_apply(const transform_t *ts, Vec4f& posInCVV, const Vec4f& posInObj) {
	 
	//�õ�MVP���󣬴���������ת��CVV����
	posInCVV= posInObj * ts->mvp;
}


void transform_applyVP(const transform_t *ts, Vec4f &posInCVV, const Vec4f& posInObj) {

	//�õ�MVP���󣬴���������ת��CVV����
	posInCVV= posInObj * (ts->vp);
}
