#include "Camera.h"



//���������
void matrix_set_lookat_r(matrix_t *m, const vector_t *eye, const vector_t *eyeTarget, const vector_t *up)
{
	//��ȡ���������ϵ��3�������� ��Ϊ��ǰ��Ŀ���ΪUnity����������ϵ�����в��Ҳ��ѭ���ַ���
	vector_t xaxis, yaxis, zaxis;

	//zaxis �����Z�� ����Ļ��
	vector_sub(&zaxis, eyeTarget, eye); //Z
	vector_normalize(&zaxis);
	vector_crossproduct(&xaxis, up, &zaxis);
	vector_normalize(&xaxis);
	vector_crossproduct(&yaxis, &zaxis, &xaxis);

	//���� ������������������ϵ����ת 
	matrix_t rotationM;
	rotationM.m[0][0] = xaxis.x;
	rotationM.m[1][0] = xaxis.y;
	rotationM.m[2][0] = xaxis.z;

	rotationM.m[0][1] = yaxis.x;
	rotationM.m[1][1] = yaxis.y;
	rotationM.m[2][1] = yaxis.z;

	rotationM.m[0][2] = zaxis.x;
	rotationM.m[1][2] = zaxis.y;
	rotationM.m[2][2] = zaxis.z;

	rotationM.m[0][3] = rotationM.m[1][3] = rotationM.m[2][3] = 0.0f;
	rotationM.m[3][0] = rotationM.m[3][1] = rotationM.m[3][2] = 0.0f;
	rotationM.m[3][3] = 1.0f;

	//���� ������������������ϵ��λ��
	matrix_t transM;
	matrix_set_identity(&transM);
	transM.m[3][0] = -eye->x;
	transM.m[3][1] = -eye->y;
	transM.m[3][2] = -eye->z;

	*m = transM * rotationM;
	//matrix_mul(m, &transM, &rotationM); //����-��������� ��ƽ�ƣ�����ת
}



//��Ϊ�ʼ��������ϵ����������ϵ�غϣ�������˶����Ǵ���������ϵ ת����������ϵ��������ʵ��״̬�ǣ������š�����ת����ƽ�ƣ�
//������������ϵת����������ϵ  �����෴  ��Ϊ (T*R)������� = ��R�������*��T������� ��ƽ�ơ�����ת�������ţ�
// ���������  eye�������� front��ǰ��  up��Y��
void matrix_set_lookat(matrix_t *m, const vector_t *eye, const vector_t *eyeTarget, const vector_t *up)
{
	//��ȡ���������ϵ��3�������� ��Ϊ��ǰ��Ŀ���ΪUnity����������ϵ�����в��Ҳ��ѭ���ַ���
	vector_t xaxis, yaxis, zaxis;

	//zaxis �����Z�� ����Ļ��
	vector_sub(&zaxis, eyeTarget, eye); //Z
	vector_normalize(&zaxis);
	vector_crossproduct(&xaxis, up, &zaxis); 
	vector_normalize(&xaxis);
	vector_crossproduct(&yaxis, &zaxis, &xaxis);

	//���� ������������������ϵ����ת 
	matrix_t rotationM;
	rotationM.m[0][0] = xaxis.x;
	rotationM.m[1][0] = xaxis.y;
	rotationM.m[2][0] = xaxis.z;

	rotationM.m[0][1] = yaxis.x;
	rotationM.m[1][1] = yaxis.y;
	rotationM.m[2][1] = yaxis.z;

	rotationM.m[0][2] = zaxis.x;
	rotationM.m[1][2] = zaxis.y;
	rotationM.m[2][2] = zaxis.z;

	rotationM.m[0][3] = rotationM.m[1][3] = rotationM.m[2][3] = 0.0f;
	rotationM.m[3][0] = rotationM.m[3][1] = rotationM.m[3][2] = 0.0f;
	rotationM.m[3][3] = 1.0f;

	//���� ������������������ϵ��λ��
	matrix_t transM;
	matrix_set_identity(&transM);
	transM.m[3][0] = -eye->x;
	transM.m[3][1] = -eye->y;
	transM.m[3][2] = -eye->z;

	*m = transM * rotationM;
	//matrix_mul(m, &transM, &rotationM); //����-��������� ��ƽ�ƣ�����ת
}


//�����������ϵ�е�һ�� P_V, ����͸�ӱ任��ΪP_H(�����ֱ�ΪP_Hx, P_Hy, P_Hz, 1)
//P_Hz = -P_Vz  *  (far + near) / (far - near) - 2far*near / (far - near)
//P_Hw = -P_Vz��

//��Ϊ����͸�ӳ����󶥵��w������1  ��ô��Χ�� -1 ��1 ��x ӳ�䵽 0zl �� 1 �� ���� x�� = x/2 + 0.5 

//͸�Ӿ���  �����������ϵ������ת��Ϊ�������õ���Ļ����
//zn ��ƽ��Z
//zf Զƽ��Z
//	float aspect = (float)width / ((float)height); �ֱ��ʱ���
void matrix_set_perspective(matrix_t *m, float fovy, float aspect, float zn, float zf)
{
	float fax = 1.0f / (float)tan(fovy * 0.5f);  //cot@
	matrix_set_zero(m);
	m->m[0][0] = (float)(fax / aspect);
	m->m[1][1] = (float)(fax);
	m->m[2][2] = zf / (zf - zn);
	m->m[3][2] = -zn * zf / (zf - zn);
	m->m[2][3] = 1; //��W=1
}


void CameraInit()
{

	
}

//�����λ��ˢ�º�,�豸�ľ���ı�
void camera_update(camera* caneraMain)
{

	matrix_set_perspective(&caneraMain->projection_trans, caneraMain->fov, caneraMain->aspect, caneraMain->zn, caneraMain->zf); //�趨��ƽ��Ϊ1������WȡֵΪ1�ͺ��ˡ����ŵ�ͶӰ��ȽϷ���

	//���� view���� ���Ǽ����matrix_Obj2World,������ǰ�camera����obj�� World2view ������������� TOdo:�Ժ����Ӱ�������������
	
	//matrix_World2Obj(&device->transform.view, caneraMain->rotation,caneraMain->pos, 1);

	//vector_t right, eyeTarget, up, front;
	//vector_add(&eyeTarget, &caneraMain->eye, &caneraMain->front);
	//��������� �������λ��
	matrix_set_lookat(&caneraMain->view, &(caneraMain->eye), &caneraMain->eyeTarget, &caneraMain->worldup);

}

//ƽ�й�=�������� ���Դ=͸�Ӿ���
//����Ƕ�̬�ƹ⣬��Ҫˢ��

//TODO����ʱʹ�����������fov
void camera_updateShadow(camera * caneraShadow, camera *  camera_main)
{
	matrix_set_perspective(&caneraShadow->projection_trans, camera_main->fov, camera_main->aspect, camera_main->zn, camera_main->zf);
	//��������� �������λ��
	matrix_set_lookat(&caneraShadow->view, &(caneraShadow->eye), &caneraShadow->eyeTarget, &caneraShadow->worldup);



}

