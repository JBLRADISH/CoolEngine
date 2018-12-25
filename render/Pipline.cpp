#include "Pipline.h"
#include "Camera.h"


int render_state;
//λ���㣺
//  a != b----->a = a | b, a ���� b ֻҪ��һ��Ϊ 1, ��ô��a �����ս����Ϊ 1
//  a &= b----->a = a & b, a �� b ���߱��붼Ϊ 1, ��ô��a �����ս����Ϊ 1
//���  a ^= b----->a = a ^ b, ���ҽ��� a �� b ��ֵ��һ��ʱ��a �����ս����Ϊ1������Ϊ0

// ����������ͬ cvv �ı߽�������׶�ü�

//�����w ����Z��������ȼ��� Ŀǰȡ���� 1/Z �����Ե����� ��Z���޴��ʱ�� W=0���˿�����������塣������Ұ������Զ��һ����
int transform_check_cvv(const vector_t *v)
{
	float w = v->w; //�õ������������
	int check = 0;

	//��֤ Z��0-w֮��  ����֤Z/w��0-1֮�䣩
	if (v->z < 0.0f)  // ��CVV����(����ͷ����)
		check |= 1;
	if (v->z >  w) //z�� ������W��ֵ�� ˵��  ��ʱ�� (x,y,z,w)����w��ͶӰ�������壬  Z/W>1.  ���������ʱ�� Z ��Ҫ��0-1֮��
		check |= 2;

	//��֤ x  y ���� -w��w֮�� ��-1��1֮�䣩
	if (v->x < -w)  //x�� ������W��ֵ�� ˵��  ��ʱ�� (x,y,z,w)����w��ͶӰ�������壬  x/W>1.  ���������ʱ�� x ��Ҫ��-1 �� 1֮��
		check |= 4;
	if (v->x >  w)
		check |= 8;
	if (v->y < -w)  //ͬ��
		check |= 16;
	if (v->y >  w)
		check |= 32;
	return check;
}


//transform_homogenize��ת�Ĺ���,���ڰ����ش���Ļ���ص���������ռ䣬�ý��еƹ����ȼ��㡣
void transform_homogenize_reverse(const transform_t *ts, point_t *x, const point_t *y)
{
	float rhw = 1.0f / y->w;

	x->x = (y->x * 2 / ts->screen_width - 1.0f)*rhw;

	x->y = (1.0f-y->y *2 / ts->screen_height ) *rhw;

	x->z = x->z *rhw;

	x->w = rhw;
}


//TODO:�Ժ����������ȣ���Ϊrendering textrue ����ӰͶӰ��ʹ������������Ľ�����ĳ���
// ��һ����Ȼ���դ�����õ���Ļ���� 
void transform_homogenize(const transform_t *ts, point_t *y, const point_t *x)   
{
	float rhw = 1.0f / x->w;  //����ʹ�������Wֵ�ĵ���

							  //ts->w = 800 ts->h =600

							  //��������ּ���
							  // x->x * rhw  ʵ�������ε�CVV�ռ�ת�����οռ䡣   xȡֵ��Χ��-1��1֮��
							  // (x->x * rhw + 1.0f)* 0.5f;  ʵ���� ��-1��1ת�� 0-1ȡֵ
							  // ��� * ts->screen_width  ����Ļ�ķֱ��ʵĿ�ȷŴ󵽺��ʵ�λ�á�

							  // �任���������ʼ��������ϵ�����ĵ�    
							  // ���ǣ�����Ļ�ϣ����������Ͻ�Ϊ��ʼ��    
							  // ������Ҫ���¼���ʹ���ǵ���ʼ�������Ͻ�  ����+1 �ٳ�0.5
	y->x = (x->x * rhw + 1.0f) * ts->screen_width * 0.5f;


	y->y = (1.0f - x->y * rhw) * ts->screen_height * 0.5f;  //ͬ��

	y->z = x->z * rhw;  //�õ��ھ��οռ��Z���
						//y->w = 1.0f;  //���� ��Ϊ �� (x,y,z,w) /w  ���� wֱ�Ӹ�ֵΪ1��
						//�Ż�:����w��������rhw
	y->w = rhw;
}


void vertex_rhw_init(vertex_t *v) {
	//float rhw = v->pos.w;

	////������������W������u v
	//v->tc.u *= rhw;
	//v->tc.v *= rhw;

	////����������������ɫ
	////����������ɫ��ֻ��������Ч����Z��ԽԶ����ɫԽС Խ��
	//v->color.r *= rhw;
	//v->color.g *= rhw;
	//v->color.b *= rhw;
}

//�����Ƕ����ֵ
void vertex_interp(vertex_t *y, const vertex_t *x1, const vertex_t *x2, float t)
{
	//����λ�ò�ֵ��ͨ������������ ���Ŀǰɨ�������ҵ���ʼ��Ľ����㡣
	vector_interp(&y->pos, &x1->pos, &x2->pos, t);

	y->tc.u = interp(x1->tc.u, x2->tc.u, t); //uv��ֵ  ��������������ԭ��  ����Y���ϵı�������ֵ
	y->tc.v = interp(x1->tc.v, x2->tc.v, t);

	y->color.r = interp(x1->color.r, x2->color.r, t);  //��ɫ��ֵ
	y->color.g = interp(x1->color.g, x2->color.g, t);
	y->color.b = interp(x1->color.b, x2->color.b, t);

	if (render_state & (RENDER_STATE_verterNormal_color))
		y->vertexLight = interp(x1->vertexLight, x2->vertexLight, t);
}

//����ɨ���ߵ����ز�ֵ  ����ÿ�����ص�Ĺ��ɵĲ�ֵ  ��������x,y,z,w ��ɫrpg
void vertex_division(vertex_t *y, const vertex_t *x1, const vertex_t *x2, float width) {

	float inv = 1.0f / width; //ʹ��ɨ���ߵĿ������ֵ�� ɨ���߳���/���Ŀ�ȣ��͵������ɨ����ӵ�е���������
	y->pos.x = (x2->pos.x - x1->pos.x) * inv;
	y->pos.y = (x2->pos.y - x1->pos.y) * inv;
	y->pos.z = (x2->pos.z - x1->pos.z) * inv;
	y->pos.w = (x2->pos.w - x1->pos.w) * inv; //ע�⣬�����WֵҲ�ǲ�ֵ��  w����������Ϊ��ȡ�  �����Բ�ֵ���õ� 1/w
	y->tc.u = (x2->tc.u - x1->tc.u) * inv;
	y->tc.v = (x2->tc.v - x1->tc.v) * inv;
	y->color.r = (x2->color.r - x1->color.r) * inv;
	y->color.g = (x2->color.g - x1->color.g) * inv;
	y->color.b = (x2->color.b - x1->color.b) * inv;

	if (render_state & RENDER_STATE_verterNormal_color)//����ɨ������ ��ʼ��ĵƹ��ֵ�Ĳ���
		y->vertexLight = (x2->vertexLight - x1->vertexLight) * inv;
}


// ���������β��Ϊ 0-2 �������Σ����ҷ��غϷ������ε�����
int trapezoid_init_triangle(trapezoid_t *trap, const vertex_t *p1, const vertex_t *p2, const vertex_t *p3) //���ӷ��ߴ���������
{
	const vertex_t *p;

	if (p1->pos.y > p2->pos.y)
		p = p1, p1 = p2, p2 = p; //���P1 ��P2�� ����P1 �� P2
	if (p1->pos.y > p3->pos.y)
		p = p1, p1 = p3, p3 = p;//����P1 �� P3 ȷ��P1������µ�
	if (p2->pos.y > p3->pos.y)
		p = p2, p2 = p3, p3 = p; //ȷ��p2���м�  P3���

	 //��Ϊ����Ҫ����ֵ����������ֱ�Ӽ����б�� �ȿ��Բ�������Σ��ֿ��Ժ����ֵ�ã���ȽϺ�
	if (p1->pos.y == p2->pos.y && p1->pos.y == p3->pos.y)//Y����ͬ
		//X����ͬ
		if (p1->pos.x == p2->pos.x && p1->pos.x == p3->pos.x)
			return 0;

	// P1- - P2  
	// -  -  
	// - -  
	// -  
	// P3
	if (p1->pos.y == p2->pos.y)  //Y����ͬ��ֻ����0��1�������ˣ�����Ҫ�����
	{
		if (p1->pos.x > p2->pos.x)
			p = p1, p1 = p2, p2 = p;

		trap[0].top = p1->pos.y;   //top ��P1��Y
		trap[0].bottom = p3->pos.y; //bottom ��P3��Y
		trap[0].left.v1 = *p1;  // �������ε��������
		trap[0].left.v2 = *p3;  //
		trap[0].right.v1 = *p2; //
		trap[0].right.v2 = *p3; //
		return (trap[0].top < trap[0].bottom) ? 1 : 0;  //����һ���� 
	}

	// ��Ϊ��Ļ��˳����,���Ͻ�Ϊԭ�㣺Y���������  P3>P2>P1 
	// ���ڵ�һ�������˵���������������ģ�   
	// P1  
	// -  
	// --   
	// - -  
	// -  -  
	// -   - P2  
	// -  -  
	// - -  
	// -  
	// P3
	if (p2->pos.y == p3->pos.y) //���µ�������
	{
		if (p2->pos.x > p3->pos.x)
			p = p2, p2 = p3, p3 = p;
		trap[0].top = p1->pos.y;
		trap[0].bottom = p3->pos.y;
		trap[0].left.v1 = *p1;
		trap[0].left.v2 = *p2;
		trap[0].right.v1 = *p1;
		trap[0].right.v2 = *p3;
		return (trap[0].top < trap[0].bottom) ? 1 : 0;
	}

	//����б��
	float dP1P2 = (p2->pos.x - p1->pos.x) / (p2->pos.y - p1->pos.y);
	float dP1P3 = (p3->pos.x - p1->pos.x) / (p3->pos.y - p1->pos.y);

	//����ָ�Ϊ����������
	trap[0].top = p1->pos.y;
	trap[0].bottom = p2->pos.y;
	trap[1].top = p2->pos.y;
	trap[1].bottom = p3->pos.y;

	// ���ڵ�һ�������˵���������������ģ� triangle left  
	//       P1  
	//        -  
	//       --   
	//      - -  
	//     -  -  
	// P2 -   -   
	//     -  -  
	//      - -  
	//        -  
	//       P3 
	if (dP1P2<dP1P3) {
		trap[0].left.v1 = *p1;
		trap[0].left.v2 = *p2;
		trap[0].right.v1 = *p1;
		trap[0].right.v2 = *p3;

		trap[1].left.v1 = *p2;
		trap[1].left.v2 = *p3;
		trap[1].right.v1 = *p1;
		trap[1].right.v2 = *p3;
	}
	// ���ڵڶ��������˵���������������ģ�triangle right  
	// P1  
	// -  
	// --   
	// - -  
	// -  -  
	// -   - P2  
	// -  -  
	// - -  
	// -  
	// P3 
	else {
		trap[0].left.v1 = *p1;
		trap[0].left.v2 = *p3;
		trap[0].right.v1 = *p1;
		trap[0].right.v2 = *p2;

		trap[1].left.v1 = *p1;
		trap[1].left.v2 = *p3;
		trap[1].right.v1 = *p2;
		trap[1].right.v2 = *p3;
	}

	return 2;
}

//��ǰ���б�ʣ��Ż�
//����Y���������������������������Y�Ķ���
void trapezoid_edge_interp(device_t *device, trapezoid_t *trap, float y ) {

	//�ݶ�
	float t1 = (y - trap->left.v1.pos.y) / (trap->left.v2.pos.y - trap->left.v1.pos.y);
	//float worldti = (y - trap->left.v1.worldPos.y) / (trap->left.v2.worldPos.y - trap->left.v1.pos.y);

	vertex_interp(&trap->left.v, &trap->left.v1, &trap->left.v2, t1); //��������ߵ�б�� ����ɨ����������
																  //�ݶ�
	//float t2 = (y - trap->right.v1.pos.y) / (trap->right.v2.pos.y - trap->right.v1.pos.y);

	//vertex_interp(&trap->right.v, &trap->right.v1, &trap->right.v2, t2);//�������ұߵ�б�� ����ɨ�����ұ��յ�
																	//�ݶ�
	//float t1 = (y - trap->left.v1.worldPos.y) / (trap->left.v2.worldPos.y - trap->left.v1.worldPos.y);
//	vertex_interp(&trap->left.v, &trap->left.v1, &trap->left.v2, t1); //��������ߵ�б�� ����ɨ����������
																	  //�ݶ�
	float t2 = (y - trap->right.v1.pos.y) / (trap->right.v2.pos.y - trap->right.v1.pos.y);
	vertex_interp(&trap->right.v, &trap->right.v1, &trap->right.v2, t2);//�������ұߵ�б�� ����ɨ�����ұ��յ�

}

// �����������ߵĶ˵㣬��ʼ�������ɨ���ߵ����Ͳ���
void trapezoid_init_scan_line(const trapezoid_t *trap, scanline_t *scanline, int y) {

	float width = trap->right.v.pos.x - trap->left.v.pos.x; //ɨ���߿��
	scanline->x = (int)(trap->left.v.pos.x + 0.5f); //ɨ�������   +0.5 ��������
	scanline->width = (int)(trap->right.v.pos.x + 0.5f) - scanline->x; //������͵�ɨ���߿��
	scanline->y = y; //ɨ����Yֵ

	scanline->v = trap->left.v; //ɨ���ߵ�������

	if (trap->left.v.pos.x >= trap->right.v.pos.x) //������X����ڵ����ұߣ���˵�� ����ɨ���߲�����
		scanline->width = 0;

	//����ɨ���ߵĲ���  Ҳ����ɨ�����ڲ���ÿһ�����ص���һ�����أ�����Ҫ�Ĳ�ֵ
	vertex_division(&scanline->step, &trap->left.v, &trap->right.v, width);
}


// ���� 1.��դ��2D�㣨�����ڶ�ά�����ϻ��㣬�˽�ɫ�ʻ���ԭ�������Ӱ��������⣩
void device_pixel(device_t *device, int x, int y, IUINT32 color) {
	if (((IUINT32)x) < (IUINT32)device->width && ((IUINT32)y) < (IUINT32)device->height) //�����Ժ��Ż�����դ�������Ѿ��ж��� �����Ƿ�����Ļ��
	{
		device->framebuffer[y][x] = color;
	}
}


// x y������������������
// �����߶� 2.��դ��2Dֱ�ߣ�����ɭ�hķֱ�����㷨����С��ֱ���㷨�ȣ� ���-�յ㣬Ѱ���߶������άդ����������ص�
void device_draw_line(device_t *device, int x1, int y1, int x2, int y2, IUINT32 c)
{
	int x, y, rem = 0;

	//���û��Ѱ�������դ��...   ֻ�Ǽ������¾��롣
	if (x1 == x2 && y1 == y2)  //ͬһ�㣬����һ�����ص�
	{
		device_pixel(device, x1, y1, c);
	}
	else if (x1 == x2)  //˵���Ǵ�ֱ��
	{
		int inc = (y1 <= y2) ? 1 : -1;
		for (y = y1; y != y2; y += inc) //���ϻ��߳��� 
			device_pixel(device, x1, y, c);
		device_pixel(device, x2, y2, c);
	}
	else if (y1 == y2) //˵����ƽ����
	{
		int inc = (x1 <= x2) ? 1 : -1;
		for (x = x1; x != x2; x += inc)
			device_pixel(device, x, y1, c);
		device_pixel(device, x2, y2, c);
	}
	else
	{
		int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
		int dy = (y1 < y2) ? y2 - y1 : y1 - y2;

		if (dx >= dy) //˵����X��н�С��45
		{
			if (x2 < x1)
				x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; x <= x2; x++)
			{
				device_pixel(device, x, y, c);
				rem += dy;
				if (rem >= dx)
				{
					rem -= dx;
					y += (y2 >= y1) ? 1 : -1;
					device_pixel(device, x, y, c);
				}
			}
			device_pixel(device, x2, y2, c);
		}
		else
		{
			if (y2 < y1)
				x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; y <= y2; y++)
			{
				device_pixel(device, x, y, c);
				rem += dx;
				if (rem >= dy)
				{
					rem -= dy;
					x += (x2 >= x1) ? 1 : -1;
					device_pixel(device, x, y, c);
				}
			}
			device_pixel(device, x2, y2, c);
		}
	}
}

//�Ժ�ֱ���ж� texture�Ƿ�Ϊnull��ɾ������ģʽ

// ���������ȡ����
IUINT32 device_texture_read(const device_t *device, float u, float v) {
	int x, y;
	u = u * device->max_u;
	v = v * device->max_v;
	x = (int)(u + 0.5f); //��������
	y = (int)(v + 0.5f);
	x = CMID(x, 0, device->tex_width - 1);
	y = CMID(y, 0, device->tex_height - 1);
	return device->texture[y][x];
}


//=====================================================================
// ���ؽ׶�
//=====================================================================

// ����ɨ����
void device_draw_scanline(device_t *device, scanline_t *scanline, float surfaceLight)
{
	int scanlineY = scanline->y;

	IUINT32 *framebuffer = device->framebuffer[scanlineY]; //�õ���ǰ��һ�е�ɨ���� ֡����
	float *zbuffer = device->zbuffer[scanlineY]; //�õ���ǰ��һ�е�ɨ����  Zbuffer
	int scanlineX = scanline->x;
	int lineWidth = scanline->width;
	int width = device->width; //��ʾ����Χ  TODO���Ժ�Ҫ�����������Χ
	int render_state = device->render_state;

	//����õ���ǰɨ���ߵ�����
	for (; lineWidth > 0; scanlineX++, lineWidth--)
	{
		// ��0-w������x ��ֻҪС���豸��ȣ�����Ⱦ��
		if (scanlineX >= 0 && scanlineX < width)
		{
			//ÿ��forѭ����Ҫ���ƣ�ע�ⲻҪ������
			float rhw = scanline->v.pos.w; //ǰ����������ת�����������ڴ洢���

			//��ʾû�б��ڵ� ���߱�ʾ��������Ȳ���
			if (zbuffer == NULL || rhw >= zbuffer[scanlineX]) //�ж�����Ƿ���ڻ�����Zbuffer  Ĭ�����Ϊ0  //������Ȳ��ԣ����жϵ�ɨ���ߵ�������ȣ�����˵��Ӧ���ں��棬�ж��������
			{
				if (zbuffer != NULL)
					zbuffer[scanlineX] = rhw;//��ǰ������

#pragma region ������Ӱ��ɫ DJLTODO��ûת��

				//�������ͼ��������ȣ��Ƿ�Ҫ������Ӱ
				if (device->shadowbuffer != NULL) 
				{
					float z = scanline->v.pos.z;

					//��¼��С���룩
					if (z <= device->shadowbuffer[scanlineY*width + scanlineX]) {
						device->shadowbuffer[scanlineY*width + scanlineX] = z;


						//TODO����ʱֱ�Ӵ���
					}
				}
#pragma endregion

				//����shadow ������ rendertextrue

				point_t interpos = scanline->v.pos;

				transform_homogenize_reverse(&device->transform,&interpos, &interpos);//��CVV�ռ�ص���������ռ�

				//ʹ��ת�þ��� ����������ռ�ص�����ռ�


				//׼�����ؽ׶ε�����   ������ģ�͵Ķ���ṹת��Ϊ�������ؽ׶ε�v2f�ṹ
				v2f vf;
				vf.color = scanline->v.color;
				vf.pos = scanline->v.pos;
				vf.normal = scanline->v.normal;

				//point_t pos;//λ��

				//texcoord_t tc; //��������
				//color_t color; //��ɫ       
				//vector_t normal; //���Ĵ������ӷ���    ������ϲ� float rhw �ϲ���;  //�ö����֮ǰ��w�ĵ����� Ҳ�������Ŵ�С
				//point_t worldPos;  //֮ǰ�����������λ��
				//float vertexLight; //����ƹ��ǿ��
				//point_t shadowPos; //ͶӰ������Ĺ�դ��λ��

				//frag_shader(device, &vf, &vf.color);


				if (render_state & RENDER_STATE_COLOR) //ʹ��λ�����ж�  //��ɫģʽ��ͨ�� �����Wֵ����ֵ����ƽ����ɫģʽ����
				{
					//��ȡ��ɫ 0-1
					float r = vf.color.r; //*pixRhw;   //��ֵ����
					float g = vf.color.g; //*pixRhw;
					float b = vf.color.b; //*pixRhw;

					//תΪ255
					int R = (int)(r * 255.0f);
					int G = (int)(g * 255.0f);
					int B = (int)(b * 255.0f);

					//��֤��0-255֮��
					R = CMID(R, 0, 255);
					G = CMID(G, 0, 255);
					B = CMID(B, 0, 255);

					//λ���� 
					framebuffer[scanlineX] = (R << 16) | (G << 8) | (B);  //<<�����������    RGB ÿ��ռ8λ.  �����㣬ǡ�ñ�����1��λ�� �ͺϲ�RGBΪһ��int��
				}

				if (render_state & RENDER_STATE_TEXTURE)
				{
					float u = scanline->v.tc.u;//* pixRhw;
					float v = scanline->v.tc.v;//* pixRhw;
					IUINT32 cc = device_texture_read(device, u, v);
					framebuffer[scanlineX] = cc;
				}

				//�����ֵ
				if (render_state &(RENDER_STATE_verterNormal_color))
				{
					//asset(v.color.r<0.2f);
					//��ȡ��ɫ 0-1
					float r = vf.color.r + AmbientLight.r; //* pixRhw; 
					float g = vf.color.g + AmbientLight.g;//* pixRhw;
					float b = vf.color.b + AmbientLight.b;//* pixRhw;

					
					float verterLight = scanline->v.vertexLight + scanline->step.vertexLight;

					//תΪ255
					int R = (int)(r * 255.0f* verterLight);
					int G = (int)(g * 255.0f* verterLight);
					int B = (int)(b * 255.0f* verterLight);

					//��֤��0-255֮��
					R = CMID(R, 0, 255);
					G = CMID(G, 0, 255);
					B = CMID(B, 0, 255);

					//λ���� 
					framebuffer[scanlineX] = (R << 16) | (G << 8) | (B);  //<<�����������    RGB ÿ��ռ8λ.  �����㣬ǡ�ñ�����1��λ�� �ͺϲ�RGBΪһ��int��

				}

				if (render_state &(RENDER_STATE_surfaceNormal_color))
				{
					//��ȡ��ɫ 0-1
					float r = vf.color.r;// *pixRhw; 
					float g = vf.color.g;// *pixRhw;
					float b = vf.color.b;// *pixRhw;

					//תΪ255
					int R = (int)(r * 255.0f*surfaceLight);
					int G = (int)(g * 255.0f*surfaceLight);
					int B = (int)(b * 255.0f*surfaceLight);

					//��֤��0-255֮��
					R = CMID(R, 0, 255);
					G = CMID(G, 0, 255);
					B = CMID(B, 0, 255);

					//λ���� 
					framebuffer[scanlineX] = (R << 16) | (G << 8) | (B);  //<<�����������    RGB ÿ��ռ8λ.  �����㣬ǡ�ñ�����1��λ�� �ͺϲ�RGBΪһ��int��

				}
			}
		}

		if (scanlineX >= width)
			break;
		vertex_add(&scanline->v, &scanline->step); //����֮ǰ��������Ĳ�ֵ�����Ӳ�ֵ������һ���������,������һ����
	}
}

// ����Ⱦ����   ��Ⱦһ��������
void device_render_trap(device_t *device, trapezoid_t *trap, float surfaceLight)
{
	scanline_t scanline;
	int j, top, bottom;
	top = (int)(trap->top + 0.5f);
	bottom = (int)(trap->bottom + 0.5f);

	for (j = top; j < bottom; j++)  //��top��bottom ÿһ��y������һ��ɨ����
	{
		if (j >= 0 && j < device->height)
		{
			// ����Y����������ǰɨ���ߵ�X���꣬���Ҳ�ֵ ��ɫ�����
			trapezoid_edge_interp(device, trap, (float)j + 0.5f);

			// �����������ߵĶ˵㣬��ʼ�������ɨ���ߵ����Ͳ���   ����ɨ���ߵ�x y  z  w�Ĳ��� 
			trapezoid_init_scan_line(trap, &scanline, j);

			//����ɨ���ߣ�ͨ�������õ�����ֵ
			device_draw_scanline(device, &scanline, surfaceLight);
		}
		if (j >= device->height)
			break;
	}
}


/*
1.����
2.����
3.�����(viewing frustum,��׶)----------------- ͶӰ��2�ַ�������ͶӰ��͸��ͶӰ
��ƽ��,���������С�ľ�����,��ΪͶӰƽ��,
Զƽ���������� �ϴ�ľ���,������������е����ж��������ǿɼ���,���������������֮�� �ĳ�������,�ᱻ�ӵ�ȥ��(Frustum Culling,Ҳ��֮Ϊ��׶�ü�)��
4. project and clip space  ͳ�� ͶӰ�Ͳü��ռ�.
�淶������(Canonical view volume, CVV���淶����ͼԪ�ü�)------------������ռ��w=1��ͶӰ�ռ䣨���Σ�CVV �Ľ�ƽ��(�������С�ľ�����)��


ȷ��ֻ�е�ͼԪ��ȫ�򲿷ֵĴ�������׶�ڲ�ʱ,����Ҫ�����դ������һ ��ͼԪ��ȫλ������(��ʱ�����Լ��任Ϊ CVV)�ڲ�ʱ,������ֱ�ӽ����� һ���׶�;��ȫ�������ⲿ��ͼԪ,�����޳�;
5. ��դ��..��ʼ----------���ݷֱ����γɶ�Ӧ�����أ�������ɫ����ȣ��Ŵ󵽷��Ϸֱ��ʵ���Ļ�ռ�

*/

//ֻ�е����Ǳ�̴���3D����Ż���Ҫ������ķ�����������ģ�����3DMax��Maya�ȣ�ͨ���ڴ洢�����ݸ�ʽ�а�������ķ�����Ϣ��
// ���� render_state ����ԭʼ������   
void device_draw_primitive(device_t *device, vertex_t *v1, vertex_t *v2, vertex_t *v3)
{
	//1--------����ռ�------------------------------
	vertex_t *vertice[3] = { v1, v2, v3 };

	point_t world_pos1, world_pos2, world_pos3, //��������
		raster_pos1, raster_pos2, raster_pos3, // ��դ�����꣬��һ��2Dƽ���ϣ���Ӧ����Ļ�������������ˣ���Ӧ��ͨ����ǰ�Ĳ�����ʽ����Ӧ������ֱ���ǰ���Ļ���ش�С���ţ�	
		project_pos1, project_pos2, project_pos3; //ͶӰ���� cvv�ռ�

	render_state = device->render_state;

	transform_t transform = device->transform;

	//2--------����ռ�----------�������---------------------------------����Ǻ決 û������������ڵ�������������ֱ�Ӽ���---//

	//����ת������ռ�
	matrix_apply(&world_pos1, &v1->pos, &transform.model);
	matrix_apply(&world_pos2, &v2->pos, &transform.model);
	matrix_apply(&world_pos3, &v3->pos, &transform.model);

	//����ת��������ռ�
	vector_t world_normal1, world_normal2, world_normal3;
	matrix_apply(&world_normal1, &v1->normal, &transform.model);
	matrix_apply(&world_normal2, &v2->normal, &transform.model);
	matrix_apply(&world_normal3, &v3->normal, &transform.model);

	// �����޳�
	if (device->cull > 0)
	{
		float cullValue = CullCalcutate(&world_pos1, &world_pos2, &world_pos3, &camera_main.pos);
		if (device->cull == 1)
		{
			if (cullValue <= 0)
				return;
		}
		else if (device->cull == 2) {
			if (cullValue> 0)
				return;
		}
	}

	// ����Ĳü���׼ȷ��ֻҪ�ж��㲻���㣬���޳�����������Ϊ�����жϼ������� cvv���Լ�ͬcvv�ཻƽ����������
	// ���н�һ����ϸ�ü�����һ���ֽ�Ϊ������ȫ���� cvv�ڵ�������

	//--------------------------------4.CVV�ռ�ü�(��׶�ü�)-----------------------------

	transform_apply(&transform, &project_pos1, &v1->pos);
	transform_apply(&transform, &project_pos2, &v2->pos);
	transform_apply(&transform, &project_pos3, &v3->pos);

	if (transform_check_cvv(&project_pos1) != 0) return;
	if (transform_check_cvv(&project_pos2) != 0) return;
	if (transform_check_cvv(&project_pos3) != 0) return;



	//��ӰԤ��
	//DisVertexToLight(lightPosition, &world_pos1);

	//ת�����ƹ�ռ�



	//--------����Ƕ�̬����-------����׶�ü�֮����㣬�ȽϽ�Լ����---------����,����shader��Ч��Ҫ��������ռ���㷨�ߡ���������ƹ����ߵĽǶ�------//


	//�ƹ���������ڳ˻�����ɫ     (0-1)֮�� ��������ɫ������ 
	float surfaceLight = 0; //����ƹ�
	float v1Light = 0;

	if (render_state & RENDER_STATE_surfaceNormal_color)
	{
		surfaceLight = calculateGroudShader(&world_pos1, &world_pos2, &world_pos3); //����ƹ�
																					//printf("surfaceLight����%f", surfaceLight);
	}
	else if (render_state & RENDER_STATE_verterNormal_color)
	{
		v1->vertexLight = calculateVertexLight(&world_pos1, &world_normal1);
		v2->vertexLight = calculateVertexLight(&world_pos2, &world_normal2);
		v3->vertexLight = calculateVertexLight(&world_pos3, &world_normal3);
	}


	//�����ظ�����rhw. �ڹ�դ�����ڼ������˾ͱ���
	//2.------------------��դ���ռ�--------------5.��դ�����̣��ȹ�һ������CVV�ռ䵽���οռ䣬Ȼ��� -1��1�任��0��1��ȡֵ��Χ��Ȼ�� �õ���Ļ�ĳ���ȡ����Ļ����-------------
	transform_homogenize(&device->transform, &raster_pos1, &project_pos1);
	transform_homogenize(&device->transform, &raster_pos2, &project_pos2);
	transform_homogenize(&device->transform, &raster_pos3, &project_pos3);


	//---------TODO��������Ӱ����������ͼ
	//�����Ӱ,��ֵ
	point_t shadow_raster_pos1, shadow_raster_pos2, shadow_raster_pos3,
		shadow_project_pos1, shadow_project_pos2, shadow_project_pos3;

	//�л�����Դ�ӵ㣬��Ⱦһ��ͼ��ƽ�й�������ͶӰ,���Դ��͸��ͶӰ��

	transform_apply(&device->transform_shadow, &shadow_project_pos1, &world_pos1);
	transform_apply(&device->transform_shadow, &shadow_project_pos2, &world_pos2);
	transform_apply(&device->transform_shadow, &shadow_project_pos3, &world_pos3);

	transform_homogenize(&device->transform_shadow, &shadow_raster_pos1, &shadow_project_pos1);  //TODO�������Ժ�Ҫ��ΪͶӰ������ĳ����С ������Ļ��С
	transform_homogenize(&device->transform_shadow, &shadow_raster_pos2, &shadow_project_pos2);
	transform_homogenize(&device->transform_shadow, &shadow_raster_pos3, &shadow_project_pos3);
	//point_t shadow_points[3] = (raster_pos1_shadow, raster_pos2_shadow, raster_pos3_shadow);


	//�������ɫ�ʻ���
	if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR | RENDER_STATE_surfaceNormal_color | RENDER_STATE_verterNormal_color))
	{
		vertex_t t1 = *v1, t2 = *v2, t3 = *v3;
		trapezoid_t traps[2];
		int n;

		//������Ļ����
		t1.pos = raster_pos1;
		t2.pos = raster_pos2;
		t3.pos = raster_pos3;

		t1.shadowPos = shadow_raster_pos1;
		t2.shadowPos = shadow_raster_pos2;
		t3.shadowPos = shadow_raster_pos3;

		t1.worldPos = world_pos1;
		t2.worldPos = world_pos2;
		t3.worldPos = world_pos3;


		//vertex_rhw_init(&t1);	// ��ʼ�� w
		//vertex_rhw_init(&t2);	// ��ʼ�� w
		//vertex_rhw_init(&t3);	// ��ʼ�� w

		//TODO:����Ĳ�ֵ��ɫ��Ҫ�����������

		// ���������Ϊ0-2�������Σ����ҷ��ؿ�������������   ע�⣬��ֺ���������εĵױ�ƽ����X�ᣬ�������εĶ���ƽ����X��
		n = trapezoid_init_triangle(traps, &t1, &t2, &t3);

		//���������μ������Ϊɨ���ߣ�һ����һ���ߵ���Ⱦ
		if (n >= 1)
			device_render_trap(device, &traps[0], surfaceLight);
		if (n >= 2)
			device_render_trap(device, &traps[1], surfaceLight);
	}

	else if (render_state & RENDER_STATE_WIREFRAME)
	{
		// �߿����
		device_draw_line(device, (int)raster_pos1.x, (int)raster_pos1.y, (int)raster_pos2.x, (int)raster_pos2.y, device->foreground);
		device_draw_line(device, (int)raster_pos1.x, (int)raster_pos1.y, (int)raster_pos3.x, (int)raster_pos3.y, device->foreground);
		device_draw_line(device, (int)raster_pos3.x, (int)raster_pos3.y, (int)raster_pos2.x, (int)raster_pos2.y, device->foreground);
	}
}

//��������
void frag_shader(device_t *device, v2f *vf, color_t *color) {

	//���������Ӱ
	if(dirLight.shadow)
	{
		// fragPos -> �Ƶ�����ϵ -> �Ƶ�͸�Ӿ��� -> ���z����Ƚ�

		point_t tempPos = vf->pos; //������������ �����������ꣿ
		
		matrix_apply(&tempPos, &tempPos, &device->transform_shadow.mvp);
		transform_homogenize(&device->transform_shadow, &tempPos, &tempPos);
		int y = (int)(tempPos.y + 0.5); //?
		int x = (int)(tempPos.x + 0.5);//?

		//ֻ�����������ص���Ӱ���Եƹ��ӽǣ�
		vector_t tempNormal = vf->normal;
		matrix_apply(&tempNormal, &tempNormal, &(device->transform_shadow.mv));
		float dot = vector_dotproduct(&tempNormal, &cameras[0].front);

		if (dot>0)
		{
			float bias = 0.015 * (1.0 - dot);
			if (bias < 0.002f) bias = 0.001;
			if (y >= 0 && x >= 0 && y < camera_main.height && x < camera_main.width) {
				float shadow = 0.0;
				for (int i = -1; i <= 1; ++i)
				{
					for (int j = -1; j <= 1; ++j)
					{
						if (y + j < 0 || y + j >= camera_main.height || x + i < 0 || x + i >= camera_main.width)
							continue;
						float pcfDepth = device->shadowbuffer[(y + j)*camera_main.width + (x + i)];
						shadow += tempPos.z - bias > pcfDepth ? 1.0 : 0.0;
					}
				}
				shadow /= 9.0;

				color_t temp = { 0.3f,0.3f,0.3f,0.3f }; //��Ӱ��ɫ

				temp = temp * shadow;
				color = &((*color) + temp);
				//color_scale(&temp, shadow);  //��ӰŨ��
				//color_sub(color, color, &temp); //�����Ӱ����ǰ��ɫ
			}
			
		}


	}
}