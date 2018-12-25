#include "Camera.h"


float Forwardoffset = 0.01f;
camera cameras[MAX_NUM_CAMERA]; 
camera camera_main =  camera();

//TODO�������õ��� ò�Ʋ�����������������ת����������õ���������ת�þ���
// ���������  eye��Ŀ�⿴��Ľ���  at���������������  up��Y��
void matrix_set_lookat(matrix_t *m, const vector_t *eye, const vector_t *at, const vector_t *up)
{
	vector_t xaxis, yaxis, zaxis;

	//�������������������ͳ�������������X Y Z

	//zaxis �� Ŀ�⿴�򽹵��ʸ��   �����Ǹ�openglһ���� Z�����ᳯ����Ļ����
	vector_sub(&zaxis, at, eye); //Z

								 //���֮ǰҪ��һ���� 
	vector_normalize(&zaxis);
	vector_crossproduct(&xaxis, up, &zaxis);  //��˵õ� X��

	vector_normalize(&xaxis);
	vector_crossproduct(&yaxis, &zaxis, &xaxis);  //upֻ�ǵ���������Ϊ ���ϡ���Ŀ�⿴�򽹵� ����һ��ƽ�棬�������X�ᣬ Ȼ�����ﻹҪ���һ��������Y��

//��������ܱ�ʾ���������ĽǶȷ�Χ , ���ҿ��Ա�ʾ�ڱ�׼������ϵ�ڵ�X , Y ,Z ���ϵĶ�Ӧλ�� 

//TODO������û�б�׼��������

//���������ϵ��ԭ�㲻һ������������ϵ�غϣ�ͬʱ�����������ת��������Ҳһ��������������ϵ��������ƽ�С�Ϊ��ɹ���������Ҫ��Ϊ������1.����ƽ�ƣ��������ƽ������������ϵԭ�㣬2.����������������ϵת�������������ϵ��
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
	m->m[3][0] = -vector_dotproduct(&xaxis, eye);  //��������д�� ƽ�Ʊ仯�Ļ�����

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

// ��ʼ����������Ļ����
void transform_init(transform_t *ts, int width, int height) {
	float aspect = (float)width / ((float)height);
	matrix_set_identity(&ts->model);
	matrix_set_identity(&ts->view);

	//�ڶ�����������������µļн�&  y������ӽ�   cot(@/2) Ҳ���� near/ͶӰ��ĸ߶ȵ�һ�� ��ȻҲ����     cot(@/2)  = far/Զ����߶ȵ�һ��

	//�����趨���¼н�Ϊ 90��  2PI=360��  PI=180��
	matrix_set_perspective(&ts->projection, PI * 0.5f, aspect, 1.0f, 500.0f); //�趨��ƽ��Ϊ1������WȡֵΪ1�ͺ��ˡ����ŵ�ͶӰ��ȽϷ���
	ts->screen_width = (float)width;
	ts->screen_height = (float)height;
	transform_update(ts);
}

void CameraInit()
{

	
}

//�����λ��ˢ�º�,�豸�ľ���ı�
void camera_update(device_t *device, camera * caneraMain) 
{

	//��������� �������λ��
	matrix_set_lookat(&device->transform.view, &(caneraMain->pos), &caneraMain->front, &caneraMain->worldup);

	//���¾��󣬻��MVP����
	transform_update(&device->transform);
}


//����Ƕ�̬�ƹ⣬��Ҫˢ��
void camera_updateShadow(device_t *device, camera * caneraShadow)
{

	//��������� �������λ��
	matrix_set_lookat(&device->transform_shadow.view, &(caneraShadow->pos), &caneraShadow->front, &caneraShadow->worldup);

	//���¾��󣬻��MVP����
	transform_update(&device->transform_shadow);

}

void Forward()
{
	
}

//����������֡����  ���Դ棩֡���� -�����ڴ棩֡���� -�����豸��֡����
// �豸��ʼ����fbΪ�ⲿ֡���棬�� NULL �������ⲿ֡���棨ÿ�� 4�ֽڶ��룩
void device_init(device_t *device, int width, int height, void *fb)
{

	int lengthVoid = sizeof(void*);//sizeof(void*)��ȡ��������������ж��ٸ��ֽڶ�����ȡ������ָ����ɶ��x86ͨ��4bytes.  x64ͨ��8bytes.

	int need = lengthVoid * (height * 2 + 1024) + width * height * 8;

	//ptr�Ƕ���ָ�롣  �ڴ�֡����ָ��ptr��ptr��ָ��
	char *ptr = (char*)malloc(need + 64); //Ϊʲô�� char*

	char *nativeFramebuf, *zbuf;
	int j;
	assert(ptr); //����


	device->framebuffer = (IUINT32**)ptr; 	//1.���Դ棩֡�������Ⱦ������ȫ��ͬ ��Ҫ���� lengthVoid * height Ҳ���� height��ô���ָ�룬����ָ�� ���ڴ棩֡����
	ptr += lengthVoid * height; // 2. ָ�����lengthVoid * height ָ����һ������


	device->zbuffer = (float**)ptr;  // 2.��Ȼ�����Ҫ���� lengthVoid * height  ��Ϊ��ɨ���ߣ����԰��߶�����¼
	ptr += lengthVoid * height;

	device->texture = (IUINT32**)ptr;   //3. ת����ռ� ����ͬ����ÿ�������� 1024��ָ�룬��ָ������������
	ptr += lengthVoid * 1024;

	//	nativeFramebuf = (char*)ptr; //4.���ڴ棩֡����  width * height * 4 �����Ƿ�ֹû���õ�windows��֡����
	ptr += width * height * 4;

	zbuf = (char*)ptr;	//5.zbuffer��� ÿһ����������Zbuf  width * height * 4;
	ptr += width * height * 4;

	// device->framebuffer �����ptr��ַ
	//ͬʱdevice->framebuffer[j] ����� nativeFramebuf  Ҳ���� fb�����ַ
	//���� �൱�� ptrǰ��һ���ڴ�������һ��ָ�룬 ָ��fb��ָ��
	if (fb != NULL)
		nativeFramebuf = (char*)fb; //�õ�windows��֡����

									//��ֵ �豸��� ֡���� �� Z��Ȼ���
	for (j = 0; j < height; j++)  // �߶���ô���ɨ����
	{
		device->framebuffer[j] = (IUINT32*)(nativeFramebuf + width * 4 * j); //������֡���帳ֵ��ָ����豸֡����,ÿһ��֡������ width * 4 ���  framebufferֱ��ָ����
		device->zbuffer[j] = (float*)(zbuf + width * 4 * j);
	}

	//����һ����Ӱ����  ��ʹ�Ƕ�ƹ⣬Ҳ�������������ε��Ӽ��㣬������ʱֻ���㵥�ƹ���Ӱ
	float *shadowbuffer = (float*)malloc(height * width * sizeof(float));
	device->shadowbuffer = shadowbuffer;

	device->texture[0] = (IUINT32*)ptr;  //�� ����ָ�볤��ò�Ʋ���
	device->texture[1] = (IUINT32*)(ptr + 16);

	memset(device->texture[0], 0, 64);
	device->tex_width = 2;
	device->tex_height = 2;
	device->max_u = 1.0f;
	device->max_v = 1.0f;
	device->width = width;
	device->height = height;
	device->background = 0xc0c0c0;  //������ɫ
	device->foreground = 0; //�߿���ɫ
	transform_init(&device->transform, width, height);
	device->render_state = RENDER_STATE_WIREFRAME;
}

// ɾ���豸
void device_destroy(device_t *device) {
	if (device->framebuffer)
		free(device->framebuffer);
	device->framebuffer = NULL;
	device->zbuffer = NULL;
	device->texture = NULL;
}

// ���õ�ǰ����
void device_set_texture(device_t *device, void *bits, long pitch, int w, int h) {


	char *ptr = (char*)bits; //
	int j;
	assert(w <= 1024 && h <= 1024); //���ܳ���1024

	for (j = 0; j < h; ptr += pitch, j++) 	//���¼���ÿ�������ָ��    device->texture������ ������ĸ߶� 
		device->texture[j] = (IUINT32*)ptr;
	device->tex_width = w;
	device->tex_height = h;
	device->max_u = (float)(w - 1);
	device->max_v = (float)(h - 1);
}

// ��� framebuffer �� zbuffer
void device_clear(device_t *device, int mode)
{
	int y, x, height = device->height;
	for (y = 0; y < device->height; y++)
	{
		IUINT32 *dst = device->framebuffer[y];
		IUINT32 cc = (height - 1 - y) * 230 / (height - 1);
		cc = (cc << 16) | (cc << 8) | cc;
		if (mode == 0)
			cc = device->background;
		for (x = device->width; x > 0; dst++, x--)
			dst[0] = cc;
	}
	for (y = 0; y < device->height; y++)
	{
		float *dst = device->zbuffer[y];
		for (x = device->width; x > 0; dst++, x--)
			dst[0] = 0.0f;
	}

	//���shaderbuffer,Ĭ�������CVV������Զ��������ıߵ���ȣ�Ҳ����1
	if (device->shadowbuffer != NULL) {
		for (int y = 0; y < device->height; y++)
			for (int x = 0; x < device->width; x++)
				device->shadowbuffer[y * device->width + x] = 1.0f;
	}
}