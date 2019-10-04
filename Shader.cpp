#include "Shader.h"
#include "render/UObject.h"
#include "render/ShadingCalculate.h"
#include "render/Pipline.h"



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
		//model->vert(iface, nthvert);
	vertex_t* v1 = new vertex_t();
	v1->pos =  point_t(gl_Vertex.x, gl_Vertex.y, gl_Vertex.z,1);  //�������һ�㼸��Ӧ���� ����

	//1--------����ռ�------------------------------

	point_t world_pos1,
		//world_pos2, world_pos3, //��������
	// ��դ�����꣬��һ��2Dƽ���ϣ���Ӧ����Ļ�������������ˣ���Ӧ��ͨ����ǰ�Ĳ�����ʽ����Ӧ������ֱ���ǰ���Ļ���ش�С���ţ�	
	raster_pos1,
	//raster_pos2, raster_pos3,
	project_pos1 ;
		//project_pos2, project_pos3; //ͶӰ���� cvv�ռ�

	//2--------����ռ�----------�������---------------------------------����Ǻ決 û������������ڵ�������������ֱ�Ӽ���---//

	//����ת������ռ�
	world_pos1 = v1->pos * transform.model;


	//// �����޳�
	//if (cull > 0)
	//{
	//	float cullValue = CullCalcutate(&world_pos1, &world_pos2, &world_pos3, &curCamera.eye);
	//	if (cull == 1)
	//	{
	//		if (cullValue <= 0)
	//			return Vec3f();
	//	}
	//	else if (cull == 2) {
	//		if (cullValue > 0)
	//			return Vec3f();
	//	}
	//}

	// ����Ĳü���׼ȷ��ֻҪ�ж��㲻���㣬���޳�����������Ϊ�����жϼ������� cvv���Լ�ͬcvv�ཻƽ����������
	// ���н�һ����ϸ�ü�����һ���ֽ�Ϊ������ȫ���� cvv�ڵ�������

	//--------------------------------4.CVV�ռ�ü�(��׶�ü�)-----------------------------
	project_pos1 = v1->pos * transform.mvp;

	if (transform_check_cvv(&project_pos1) != 0) return Vec3f();


	//����ת��������ռ�
	Vec4f world_normal1, world_normal2, world_normal3;
	world_normal1 = v1->normal * transform.model;
	//��ӰԤ��
	//DisVertexToLight(lightPosition, &world_pos1);

	//ת�����ƹ�ռ�


	//--------����Ƕ�̬����-------����׶�ü�֮����㣬�ȽϽ�Լ����---------����,����shader��Ч��Ҫ��������ռ���㷨�ߡ���������ƹ����ߵĽǶ�------//


	//�ƹ���������ڳ˻�����ɫ     (0-1)֮�� ��������ɫ������ 
	float surfaceLight = 0; //����ƹ�
	float v1Light = 0;

	if (render_state & RENDER_STATE_surfaceNormal_color)
	{
		//surfaceLight = calculateGroudShader(&world_pos1, &world_pos2, &world_pos3); //����ƹ�
																					//printf("surfaceLight����%f", surfaceLight);
	}
	else if (render_state & RENDER_STATE_verterNormal_color)
	{
		v1->vertexLight = calculateVertexLight(&world_pos1, &world_normal1);
	}


	//�����ظ�����rhw. �ڹ�դ�����ڼ������˾ͱ���
	//2.------------------��դ���ռ�--------------5.��դ�����̣��ȹ�һ������CVV�ռ䵽���οռ䣬Ȼ��� -1��1�任��0��1��ȡֵ��Χ��Ȼ�� �õ���Ļ�ĳ���ȡ����Ļ����-------------
	transform_homogenize(&transform, &raster_pos1, &project_pos1);


	//�����դ������ ��shader�����Ļ���
	//varying_tri

	Vec3f _verter = Vec3f(raster_pos1.x, raster_pos1.y, raster_pos1.z);
	varying_tri.set_col(nthvert,_verter);

	//---------TODO��������Ӱ����������ͼ
	//�����Ӱ,��ֵ
	point_t shadow_view_pos1, shadow_view_pos2, shadow_view_pos3;

	//�л�����Դ�ӵ㣬��Ⱦһ��ͼ��ƽ�й�������ͶӰ,���Դ��͸��ͶӰ��

	transform_t shadow_transform = transform;

	shadow_view_pos1 =  v1->pos * shadow_transform.mv;


	//TODO�������Ժ�Ҫ��ΪͶӰ������ĳ����С ������Ļ��С
   //transform_homogenize(&shadow_transform, &shadow_raster_pos1, &shadow_project_pos1);
   //transform_homogenize(&shadow_transform, &shadow_raster_pos2, &shadow_project_pos2);
   //transform_homogenize(&shadow_transform, &shadow_raster_pos3, &shadow_project_pos3);
   //point_t shadow_points[3] = point_t{ shadow_raster_pos1, shadow_raster_pos2, shadow_raster_pos3 };

	return gl_Vertex;
}

//�м��դ��

//���ؽ׶Σ����붥��
bool Shader::fragment(Vec3f bar, TGAColor& color) {




	return false;
}