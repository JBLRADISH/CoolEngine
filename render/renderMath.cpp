#include "renderMath.h"


//�������������� 
float CMIDFloat(float x, float min, float max)
{
	return (x < min) ? min : ((x > max) ? max : x);
}


//�������������� int
int CMID(int x, int min, int max)
{
	return (x < min) ? min : ((x > max) ? max : x);
}

// �����ֵ��t Ϊ [0, 1] ֮�����ֵ lerp��ֵ
float interp(float x1, float x2, float t)
{
	return x1 + (x2 - x1) * t;
}

// | v |
float vector_length(const vector_t *v) {
	float sq = v->x * v->x + v->y * v->y + v->z * v->z;
	return (float)sqrt(sq);
}

//�������ĵڶ�����Ч����������������.  wΪ1��Ϊ��.  wΪ0��Ϊ������  ��Ϊw������λ�ƣ�����λ��ûʲô���塣
// z = x + y  wһֱΪ1
void vector_add(vector_t *z, const vector_t *x, const vector_t *y)
{
	z->x = x->x + y->x;
	z->y = x->y + y->y;
	z->z = x->z + y->z;
	z->w = 1.0;
}

// z = x - y
void vector_sub(vector_t *z, const vector_t *x, const vector_t *y)
{
	z->x = x->x - y->x;
	z->y = x->y - y->y;
	z->z = x->z - y->z;
	z->w = 1.0;
}


void vector_scale(vector_t *z, float scale)
{
	z->x = z->x * scale;
	z->y = z->y * scale;
	z->z = z->z * scale;
}

// ʸ�����
float vector_dotproduct(const vector_t *x, const vector_t *y)
{
	return x->x * y->x + x->y * y->y + x->z * y->z;
}


// ʸ�����  ��� x y ��׼������������Ĳ��ZҲ�Ǳ�׼����xyƽ��ķ��ߡ� ����Ļ���Ҫ��׼��һ��Z
void vector_crossproduct(vector_t *z, const vector_t *x, const vector_t *y)
{

	z->x = x->y * y->z - x->z * y->y;
	z->y = x->z * y->x - x->x * y->z;
	z->z = x->x * y->y - x->y * y->x;
	z->w = 1.0f;
}

// ʸ��lerp��ֵ��tȡֵ [0, 1]
void vector_interp(vector_t *z, const vector_t *x1, const vector_t *x2, float t)
{
	z->x = interp(x1->x, x2->x, t);
	z->y = interp(x1->y, x2->y, t);
	z->z = interp(x1->z, x2->z, t);
	z->w = interp(x1->w, x2->w, t);//������Ȳ�ֵ��  �����Բ�ֵ���õ� 1/Z
}

// ʸ����һ��  ע���һ�������漰W�� ֻ����ά����άͶӰ��ʱ��W��һ��������
void vector_normalize(vector_t *v)
{
	float length = vector_length(v);
	if (length != 0.0f) {
		float inv = 1.0f / length;
		v->x *= inv;
		v->y *= inv;
		v->z *= inv;
	}
}

// c = a + b
void matrix_add(matrix_t *c, const matrix_t *a, const matrix_t *b)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			c->m[i][j] = a->m[i][j] + b->m[i][j];
	}
}

// c = a - b
void matrix_sub(matrix_t *c, const matrix_t *a, const matrix_t *b) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			c->m[i][j] = a->m[i][j] - b->m[i][j];
	}
}

// c = a * b
void matrix_mul(matrix_t *c, const matrix_t *left , const matrix_t *right) {         
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			c->m[i][j] =
				(left->m[i][0] * right->m[0][j]) +
				(left->m[i][1] * right->m[1][j]) +
				(left->m[i][2] * right->m[2][j]) +
				(left->m[i][3] * right->m[3][j]);
		}
	}
}

// c = a * f
void matrix_scale(matrix_t *c, const matrix_t *a, float f) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			c->m[i][j] = a->m[i][j] * f;
	}
}

//���� �ҳ˾��� 
void matrix_apply(vector_t *y, const vector_t *x, const matrix_t *m) {
	float X = x->x, Y = x->y, Z = x->z, W = x->w;
	y->x = X * m->m[0][0] + Y * m->m[1][0] + Z * m->m[2][0] + W * m->m[3][0];
	y->y = X * m->m[1][0] + Y * m->m[1][1] + Z * m->m[2][1] + W * m->m[3][1];
	y->z = X * m->m[2][0] + Y * m->m[1][2] + Z * m->m[2][2] + W * m->m[3][2];
	y->w = X * m->m[3][0] + Y * m->m[1][3] + Z * m->m[2][3] + W * m->m[3][3];
}

////���� ��˾��� 
//void matrix_apply_left(vector_t *y, const vector_t *x, const matrix_t *m) {
//	float X = x->x, Y = x->y, Z = x->z, W = x->w;
//	y->x = X * m->m[0][0] + Y * m->m[0][1] + Z * m->m[0][2] + W * m->m[0][3];
//	y->y = X * m->m[1][0] + Y * m->m[1][1] + Z * m->m[1][2] + W * m->m[1][3];
//	y->z = X * m->m[2][0] + Y * m->m[2][1] + Z * m->m[2][2] + W * m->m[2][3];
//	y->w = X * m->m[3][0] + Y * m->m[3][1] + Z * m->m[3][2] + W * m->m[3][3]; 
//}

//��׼���� 4X4 
void matrix_set_identity(matrix_t *m) {
	m->m[0][0] = m->m[1][1] = m->m[2][2] = m->m[3][3] = 1.0f;
	m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
	m->m[1][0] = m->m[1][2] = m->m[1][3] = 0.0f;
	m->m[2][0] = m->m[2][1] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
}


//�����
void matrix_set_zero(matrix_t *m) {
	m->m[0][0] = m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
	m->m[1][0] = m->m[1][1] = m->m[1][2] = m->m[1][3] = 0.0f;
	m->m[2][0] = m->m[2][1] = m->m[2][2] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = m->m[3][3] = 0.0f;
}



// ƽ�Ʊ任 ���
void matrix_set_translate(matrix_t *m, float x, float y, float z) {
	matrix_set_identity(m);
	m->m[0][3] = x;
	m->m[1][3] = y;
	m->m[2][3] = z;
}

// ���ű任
void matrix_set_scale(matrix_t *m, float x, float y, float z) {
	matrix_set_identity(m);
	m->m[0][0] = x;
	m->m[1][1] = y;
	m->m[2][2] = z;
}


//A����ϵת��B����ϵ   �κ���������֮��Ľ���������
//ŷ���� ���ݻ����� ת���� ����Y-X-Z���˳�� ���ݾ���˷��ɽ�������ӳ�һ������
void matrix_World2Obj(matrix_t *m, vector_t rot, vector_t pos, float scale)
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

	m->m[0][0] = cosX * cosZ;
	m->m[0][1] = -sinZ;
	m->m[0][2] = -sinY * cosZ;

	m->m[1][0] = cosX * cosY* sinZ - sinX * sinY;
	m->m[1][1] = cosX * cosZ;
	m->m[1][2] = -cosX * sinY*sinZ - sinX * cosY;

	m->m[2][0] = sinX * cosY* sinZ + cosX * sinY;
	m->m[2][1] = sinX * cosZ;
	m->m[2][2] = -sinX * sinY * sinZ + cosX * cosY;

	//ƽ��
	m->m[0][3] = 0;
	m->m[1][3] = 0;
	m->m[2][3] = 0;


	m->m[3][0] = 0;
	m->m[3][1] = 0;
	m->m[3][2] = 0;
	m->m[3][3] = 1.0f;
}


	//A����ϵת��B����ϵ   �κ���������֮��Ľ���������
	//ŷ���� ���ݻ����� ת���� ����Y-X-Z���˳�� ���ݾ���˷��ɽ�������ӳ�һ������
	void matrix_Obj2World(matrix_t *m, vector_t rot, vector_t pos,float scale)
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

		m->m[0][0] = cosX * cosZ;
		m->m[0][1] = -sinZ;
		m->m[0][2] = -sinY*cosZ;

		m->m[1][0] = cosX * cosY* sinZ - sinX * sinY;
		m->m[1][1] = cosX * cosZ;
		m->m[1][2] = -cosX * sinY*sinZ - sinX * cosY;

		m->m[2][0] = sinX * cosY* sinZ + cosX * sinY;
		m->m[2][1] = sinX * cosZ;
		m->m[2][2] = -sinX * sinY * sinZ + cosX * cosY;

		//ƽ��
		m->m[0][3] = 0;
		m->m[1][3] = 0;
		m->m[2][3] = 0;


		m->m[3][0] = xOffset;
		m->m[3][1] = yOffest;
		m->m[3][2] = zOffset;
		m->m[3][3] = 1.0f;	
	}


	//��Ԫ��ת����
	// ����ָ������ת
//TODO��������������

//����ϵ�ı仯 = �������λ��+����ϵ����ת   ����ϵ����ת �� ��������ϵ�ڲ���������ת��һ����
//��������ǻ���ָ��������ת����ת���� ��ָ������Ϊ (x,y,z)


//���
void matrix_set_rotate(matrix_t *m, float x, float y, float z, float theta, float xOffset,float yOffest, float zOffset)
{
	//���Ǻ����õ��ǻ��ȣ����ǽǶ�
	vector_t vec = { x, y, z, 1.0f }; //ת��Ϊ�������  w=1������һ����.   w=0����������.  ��Ϊw����λ��,����λ��������

									  //������Ԫ�����ĸ�����֮ǰ����Ҫ����ת���׼��Ϊ��λ����
	vector_normalize(&vec);

	//�����Ǽ������Ԫ�����ĸ�����
	//����Ԫ��Q��x, y, z, w����ʾ����a��xa, ya, za������theta����ת��Ľ������x��y��z��w�ֱ�Ϊ��
	x = vec.x * (float)sin(theta * 0.5f); //(x,y,z)��һ������
	y = vec.y * (float)sin(theta * 0.5f);
	z = vec.z * (float)sin(theta * 0.5f);

	// w �Ǳ���
	float w = (float)cos(theta * 0.5f);

	//����������Ԫ����ת�������ľ���
	m->m[0][0] = 1 - 2 * y * y - 2 * z * z;
	m->m[1][0] = 2 * x * y - 2 * w * z;
	m->m[2][0] = 2 * x * z + 2 * w * y;
	m->m[0][1] = 2 * x * y + 2 * w * z;
	m->m[1][1] = 1 - 2 * x * x - 2 * z * z;
	m->m[2][1] = 2 * y * z - 2 * w * x;
	m->m[0][2] = 2 * x * z - 2 * w * y;
	m->m[1][2] = 2 * y * z + 2 * w * x;
	m->m[2][2] = 1 - 2 * x * x - 2 * y * y;

	m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
	//m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
	m->m[3][0] = xOffset;
	m->m[3][1] = yOffest;
	m->m[3][2] = zOffset;

	m->m[3][3] = 1.0f;
}