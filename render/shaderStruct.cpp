#include "shaderStruct.h"

//TODO:��Ҫ���Ϊ ��֡���¡����������
// �������
void transform_update(transform_t *ts) {

	matrix_mul(&ts->mv, &ts->model, &ts->view);
	matrix_mul(&ts->vp, &ts->view, &ts->projection);
	matrix_mul(&ts->mvp, &ts->mv, &ts->projection);
}


//�����ǡ���������˾���ʱ����������������������ҳ˾���ʱ����������������������ǲ��еġ�����DirectX��ʹ�õ�������������OpenGL��ʹ�õ�����������
//�����õ��ҳ�

// w ҲҪ���뵽����任�ļ���  ��ʱ����CVV��ʱ�� x,y,z,wҪ����W���˿� w=1,ͶӰ����Ļ������
//�õ�����Ļ�ϵ�����. (������Ļ��Zֵ��1)

void transform_apply(const transform_t *ts, vector_t *posInCVV, const vector_t *posInObj) {
	 
	//�õ�MVP���󣬴���������ת��CVV����
	matrix_apply(posInCVV, posInObj, &ts->mvp);
}


void transform_applyVP(const transform_t *ts, vector_t *posInCVV, const vector_t *posInObj) {

	//�õ�MVP���󣬴���������ת��CVV����
	matrix_apply(posInCVV, posInObj, &ts->vp);
}
