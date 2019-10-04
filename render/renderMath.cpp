#include "renderMath.h"




//A����ϵת��B����ϵ   �κ���������֮��Ľ���������
//ŷ���� ���ݻ����� ת���� ����Y-X-Z���˳�� ���ݾ���˷��ɽ�������ӳ�һ������
void matrix_World2Obj(matrix_t<4, 4, float>*m, Vec4f rot, Vec4f pos, float scale)
{

	float rot_x = rot.x;
	float rot_y = rot.y;
	float rot_z = rot.z;

	float xOffset = pos.x;
	float yOffest = pos.y;
	float zOffset = pos.z;

	float sinX = (float)sin(-rot_x);
	float sinY = (float)sin(-rot_y);
	float sinZ = (float)sin(-rot_z);

	float cosX = (float)cos(-rot_x);
	float cosY = (float)cos(-rot_y);
	float cosZ = (float)cos(-rot_z);

	(*m)[0][0] = cosX * cosZ;
	(*m)[0][1] = -sinZ;
	(*m)[0][2] = -sinY * cosZ;

	(*m)[1][0] = cosX * cosY* sinZ - sinX * sinY;
	(*m)[1][1] = cosX * cosZ;
	(*m)[1][2] = -cosX * sinY*sinZ - sinX * cosY;

	(*m)[2][0] = sinX * cosY* sinZ + cosX * sinY;
	(*m)[2][1] = sinX * cosZ;
	(*m)[2][2] = -sinX * sinY * sinZ + cosX * cosY;

	//ƽ��
	(*m)[0][3] = 0;
	(*m)[1][3] = 0;
	(*m)[2][3] = 0;


	(*m)[3][0] = 0;
	(*m)[3][1] = 0;
	(*m)[3][2] = 0;
	(*m)[3][3] = 1.0f;
}

	//������ת������������ϵ��ת�������3DMAX��������������ϵ�����汣��һ��
	//A����ϵת��B����ϵ   �κ���������֮��Ľ���������
	//ŷ���� ���ݻ����� ת���� 
	//�������� X-Y-Z���˳�� ���ݾ���˷��ɽ�������ӳ�һ������
	//�ҳ�
	void matrix_Obj2World(matrix_t<4, 4, float>*m, Vec4f rot, Vec4f pos)
	{
		float rot_x = rot.x;
		float rot_y = rot.y;
		float rot_z = rot.z;

		float xOffset = pos.x;
		float yOffest = pos.y;
		float zOffset = pos.z;

		float sinX = (float)sin(rot_x);
		float sinY = (float)sin(rot_y);
		float sinZ = (float)sin(rot_z);

		float cosX = (float)cos(rot_x);
		float cosY = (float)cos(rot_y);
		float cosZ = (float)cos(rot_z);

		(*m)[0][0] = cosY * cosZ;
		(*m)[0][1] = cosY * sinZ;
		(*m)[0][2] = -sinY;

		(*m)[1][0] = sinX * sinY* cosZ - cosX * sinZ;
		(*m)[1][1] = (sinX * sinY*sinZ + cosX * cosZ);
		(*m)[1][2] = sinX * cosY;

		(*m)[2][0] = cosX * sinY* cosZ + sinX * sinZ;
		(*m)[2][1] = cosX * sinY* sinZ - sinX * cosZ;
		(*m)[2][2] = (-sinX  + cosX * cosY);

		//ƽ��
		(*m)[0][3] = 0;
		(*m)[1][3] = 0;
		(*m)[2][3] = 0;

		(*m)[3][0] = 0;
		(*m)[3][1] = 0;
		(*m)[3][2] = 0;
		//(*m)[3][0] = xOffset;
		//(*m)[3][1] = yOffest;
		//(*m)[3][2] = zOffset;
		(*m)[3][3] = 1.0f;	
	}


	//��Ԫ��ת����
	// ����ָ������ת
//TODO��������������

//����ϵ�ı仯 = �������λ��+����ϵ����ת   ����ϵ����ת �� ��������ϵ�ڲ���������ת��һ����
//��������ǻ���ָ��������ת����ת���� ��ָ������Ϊ (x,y,z)


//���
void matrix_set_rotate(matrix_t<4, 4, float>*m, float x, float y, float z, float theta, float xOffset,float yOffest, float zOffset)
{
	//���Ǻ����õ��ǻ��ȣ����ǽǶ�
	Vec4f vec = { x, y, z, 1.0f }; //ת��Ϊ�������  w=1������һ����.   w=0����������.  ��Ϊw����λ��,����λ��������

	//������Ԫ�����ĸ�����֮ǰ����Ҫ����ת���׼��Ϊ��λ����
	vec.normalize();
	//�����Ǽ������Ԫ�����ĸ�����
	//����Ԫ��Q��x, y, z, w����ʾ����a��xa, ya, za������theta����ת��Ľ������x��y��z��w�ֱ�Ϊ��
	x = vec.x * (float)sin(theta * 0.5f); //(x,y,z)��һ������
	y = vec.y * (float)sin(theta * 0.5f);
	z = vec.z * (float)sin(theta * 0.5f);

	// w �Ǳ���
	float w = (float)cos(theta * 0.5f);

	//����������Ԫ����ת�������ľ���
	(*m)[0][0] = 1 - 2 * y * y - 2 * z * z;
	(*m)[1][0] = 2 * x * y - 2 * w * z;
	(*m)[2][0] = 2 * x * z + 2 * w * y;
	(*m)[0][1] = 2 * x * y + 2 * w * z;
	(*m)[1][1] = 1 - 2 * x * x - 2 * z * z;
	(*m)[2][1] = 2 * y * z - 2 * w * x;
	(*m)[0][2] = 2 * x * z - 2 * w * y;
	(*m)[1][2] = 2 * y * z + 2 * w * x;
	(*m)[2][2] = 1 - 2 * x * x - 2 * y * y;

	(*m)[0][3] = (*m)[1][3] = (*m)[2][3] = 0.0f;

	(*m)[3][0] = xOffset;
	(*m)[3][1] = yOffest;
	(*m)[3][2] = zOffset;


	(*m)[3][3] = 1.0f;
}