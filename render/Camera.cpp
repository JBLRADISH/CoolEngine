#include "Camera.h"

//TODO�������õ��� ò�Ʋ�����������������ת����������õ����� �˿� ת�þ��� =�����
// ���������  eye�������� front��ǰ��  up��Y��
void matrix_set_lookat(matrix_t *m, const vector_t *eye, const vector_t *at, const vector_t *up)
{
	vector_t xaxis, yaxis, zaxis;

	//zaxis �����Z��
	vector_sub(&zaxis, at, eye); //Z

	//���֮ǰҪ��һ���� 
	vector_normalize(&zaxis);
	vector_crossproduct(&xaxis, up, &zaxis);  //��˵õ� X��

	vector_normalize(&xaxis);
	vector_crossproduct(&yaxis, &zaxis, &xaxis);  //upֻ�ǵ���������Ϊ ���ϡ���Ŀ�⿴�򽹵� ����һ��ƽ�棬�������X�ᣬ Ȼ�����ﻹҪ���һ��������Y��

	//���� ������������������ϵ����ת 

	//������Ҫ������ת�������������-��
	float cos_rot_x  = -vector_dotproduct(&xaxis, eye);
	float cos_rot_y = -vector_dotproduct(&yaxis, eye);
	float cos_rot_z = -vector_dotproduct(&zaxis, eye);

	//matrix_Obj2World(m,  vector_t(cos_rot_x, cos_rot_y, cos_rot_z, 0), *eye, 1);

	//return;
//��������ܱ�ʾ���������ĽǶȷ�Χ , ���ҿ��Ա�ʾ�ڱ�׼������ϵ�ڵ�X , Y ,Z ���ϵĶ�Ӧλ�� 

//���������ϵ 1.����ƽ�ƣ��������ƽ������������ϵԭ�㣬2.����������������ϵת�������������ϵ��
//ʹ�õ�λ����U, V, W�ֱ�������������ϵX, Y, Z������ĵ�λ��������������ϵ�еı�ʾ�����������������ϵ����������ϵԭ���غϵ�����£����嶥������������������������ԭ��ָ�����嶥���������
//��U, V, W�ϵ�ͶӰ��С�������嶥�������������ϵ�µ�����ֵ����ΪU, V, W�ǵ�λ������ʹ�ö��ߵĵ�˼����Եõ������ͶӰ��С��
//P  =  (ax,by,cz)  a b c����֪��
//ת������һ������ϵ��Ϊ  PN = (mU, nV,qP)

//����ת��ͼ�ռ����

//float m[4][4] = { { xaxis.x,xaxis.y,xaxis.z,0 },{ V.x,V.y,V.z,0 },{ W.x,W.y,W.z,0 },{ 0,0,0,1 } };


//����˷� =  ���任������  ���   ����Ķ�Ӧ����һ�б�ʶ��������������б仯��Ҳ������ˣ�����ÿһ�У�

 //��ʵ��ʱ�����Ч������  Px * xaxis = newPx;  ԭ������������ϵ��X���꣬����xaxis������������������x���ϵ�ͶӰ�� Ҳ���������������ϵ�����X������
 //�ҳˡ����Ǿ�������壬�����б�߲���ֵ�ľ��󣬴��б任�����ǡ�  �������֣����б任���ǱȽϺá�
	m->m[0][0] = xaxis.x;
	m->m[1][0] = xaxis.y;
	m->m[2][0] = xaxis.z;
	m->m[3][0] = -vector_dotproduct(&xaxis, eye);  //cos

												   //�������� ���������ϵ��Y��Ļ�����
												   //��ʱ��������������������Y������,  �õ���Ҳ������������ϵ��ĵ㣬�� ���������ϵ X���ϵ�ͶӰ
	m->m[0][1] = yaxis.x;
	m->m[1][1] = yaxis.y;
	m->m[2][1] = yaxis.z;

	//��Ϊ������Ļ�����ƽ���ˣ����Ե�ҲҪ����ƽ�ơ�
	m->m[3][1] = -vector_dotproduct(&yaxis, eye);

	m->m[0][2] = zaxis.x;
	m->m[1][2] = zaxis.y;
	m->m[2][2] = zaxis.z;
	m->m[3][2] = -vector_dotproduct(&zaxis, eye);//��������д�� ƽ�Ʊ仯�Ļ�����

	m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
	m->m[3][3] = 1.0f;
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

	matrix_set_perspective(&(&caneraMain->transform)->projection, caneraMain->fov, caneraMain->aspect, caneraMain->zn, caneraMain->zf); //�趨��ƽ��Ϊ1������WȡֵΪ1�ͺ��ˡ����ŵ�ͶӰ��ȽϷ���

	//���� view���� ���Ǽ����matrix_Obj2World,������ǰ�camera����obj�� World2view ������������� TOdo:�Ժ����Ӱ�������������
	
	//matrix_World2Obj(&device->transform.view, caneraMain->rotation,caneraMain->pos, 1);

	vector_t right, at, up, front;
	vector_add(&at, &caneraMain->pos, &caneraMain->front);
	//��������� �������λ��
	matrix_set_lookat(&(&caneraMain->transform)->view, &(caneraMain->pos), &at, &caneraMain->worldup);

}


//����Ƕ�̬�ƹ⣬��Ҫˢ��
void camera_updateShadow(camera * caneraShadow)
{
	//matrix_set_perspective(&(&device->transform)->projection, camera_main.fov, camera_main.aspect, camera_main.zn, camera_main.zf); //�趨��ƽ��Ϊ1������WȡֵΪ1�ͺ��ˡ����ŵ�ͶӰ��ȽϷ���

	//��������� �������λ��
	matrix_set_lookat(&caneraShadow->transform.view, &(caneraShadow->eye), &caneraShadow->front, &caneraShadow->worldup);
}

