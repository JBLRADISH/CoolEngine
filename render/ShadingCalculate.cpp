#include "ShadingCalculate.h"

// ����������ͷ�������֮��Ƕȵ�����  
// ����0��1֮���ֵ  
float ComputeNDotL(const point_t* vertex, const vector_t* normal, const point_t* lightPosition)
{
    vector_t lightDirection;

	//TODO:���������򵥼���нǣ��� �ƹ�λ��-���嶥��λ�� ��ʸ��   
	//����� ���㷴�䣬��Ҫ�� ���嶥��λ��-�ƹ�λ�� ��ʸ��
	//vector_sub(&lightDirection, vertex , lightPosition);

	vector_sub(&lightDirection, lightPosition, vertex);

	vector_normalize(&lightDirection);

	float dot = vector_dotproduct(normal, &lightDirection);

	//float correctionDot = (dot + 1)*0.5f;
	return CMIDFloat(dot, 0, 1);
}

//�����������������
float ComputeCameraToVertor(point_t* vertex, const vector_t* normal, const point_t *camera_pos)
{
	vector_t  CameraToVertor;

	vector_sub(&CameraToVertor, vertex, camera_pos);

	vector_normalize(&CameraToVertor);

	float dot = vector_dotproduct(normal, &CameraToVertor);

	return dot; 
}

//���㶥�㵽�ƹ�ľ���,���ڲ�����Ӱ
//float DisVertexToLight(const point_t *Light, vector_t* VertertPosInWorld)
//{
//
//	vector_t dis;
//	vector_sub(&dis, Light, VertertPosInWorld);
//	return vector_length(&dis);
//}


//���㶥��ĵƹ�ǿ�Ⱥ���ɫ
float calculateVertexLight(const point_t *v1, vector_t* normal)
{
	vector_normalize(normal);
	//�ƹ���������ڳ˻�����ɫ     (0-1)֮�� ��������ɫ������
	float surfaceLight = ComputeNDotL(v1, normal, &lightPosition); //����ƹ�

	return surfaceLight;

}

//����������ʱ��
//�����޳� �������Ϊ���
float CullCalcutate(const point_t *v1, const point_t *v2, const point_t *v3, const point_t *camera_pos)
{
	vector_t cross21, cross31, surfaceNormal;
	point_t center;

	vector_sub(&cross21, v2, v3);
	vector_sub(&cross31, v2, v1);
	vector_crossproduct(&surfaceNormal, &cross21, &cross31);	//��˷���  �������������жϲ�˽���ķ���ʹ�����ֶ��ɣ���������ϵ��ʹ�����֡����ǵ�mesh������˳ʱ��
																
	vector_add(&center, v1, v2);
	vector_add(&center, &center, v3);
	vector_scale(&center, 0.333f);

	vector_normalize(&surfaceNormal);

	//�޳����� 
	float cull = ComputeCameraToVertor(&center, &surfaceNormal, camera_pos); //����ƹ�

	return cull;

}


//����ƽ����Ⱦ������淨�ߡ����սǶ�
float calculateGroudShader(const point_t *v1, const point_t *v2, const point_t *v3)
{
	vector_t cross1, cross2, surfaceNormal;
	point_t center;

	//����ƽ����Ⱦ����ʹ���淨�ߣ������������ǹ�Դλ�õ�������ĵ㡣

	//���ڶ�����Ⱦ��û���淨�ߣ��������� ÿ�����أ��������������㷨ʸ����ֵ���ġ�

	// �������ϵ������Ǹ÷������ÿ�����㷨�������ĵ��ƽ��ֵ

	//�������Ч��  �ڶ���׶κ����ؽ׶μ��㶼���ԡ�
	//���Ǽ�����յ����ȣ� ��Ϊ �ƹ��������������꣬����������ת����������ϵ����׶μ���������
	vector_sub(&cross1, v2, v3);
	vector_sub(&cross2, v2, v1);
	vector_crossproduct(&surfaceNormal, &cross1, &cross2);	//��˷���  �������������жϲ�˽���ķ���ʹ�����ֶ��ɣ���������ϵ��ʹ�����֡����ǵ�mesh������˳ʱ��

	//��ķ������� === 3������ķ����������������ۼӺ����3��
	vector_add(&center, v1, v2);
	vector_add(&center, &center, v3);
	vector_scale(&center, 0.333f);

	vector_normalize(&surfaceNormal);

	//�ƹ���������ڳ˻�����ɫ     (0-1)֮�� ��������ɫ������ 
	float surfaceLight = ComputeNDotL(&center, &surfaceNormal, &lightPosition); //����ƹ�
	//printf("surfaceLight��ǰ%f", surfaceLight);
	//printf("surfaceLight��ǰ");
	return surfaceLight;

}


//1.����������

float Lambert(transform_t* mainTrans,  vector_t *v_Obj, vector_t* normal, vector_t* lightPos,color_t diffuseColor, color_t ambientColor)
{

	//������任�����͸�ӿռ� 
	//Output.position = mul(Input.position, matWorldViewProjection);

	//�����߱任����ͼ�ռ�
	//	Output.normalInView = mul(Input.normal, matWorldView);

	//	// ������ͼ�ռ�ĵƹⷽ��
	//	Output.lightDirInView = lightPos - mul(Input.position, matWorldView);

	//������任����ͼ�ռ�  
	vector_t*  ObjInView;
	transform_apply(mainTrans, v_Obj, ObjInView);


	//������任�����͸�ӿռ�  
	vector_t*  ObjInCVV;
	transform_apply(mainTrans, v_Obj, ObjInCVV);

	vector_t* normalInView;
	//�����߱任����ͼ�ռ�
	matrix_apply(normalInView, normal, &mainTrans->mv);


	//TODO:����ѵƹ�ת����ͼ�ռ�

	// ������ͼ�ռ�ĵƹⷽ��
	vector_t* lightDirInView;

	vector_sub(lightDirInView,lightPos, ObjInView);

	//-----------------���ؽ׶�------------

	//	// ��һ��
	//	In.normalInView = normalize(In.normalInView);
	//	In.lightDirInView = normalize(In.lightDirInView);
	//
	//	// ����Lambertģ�ͣ����ߵ������ⷽ�����������
	//	float4 diffuse = max(0, dot(In.normalInView, In.lightDirInView));
	//	diffuse = diffuse * diffuseColor;
	//
	//	Out.color = ambientColor + diffuse;


	//����Lambertģ�ͣ����ߵ������ⷽ�����������
	float diffuse = max(0,vector_dotproduct(normalInView, lightDirInView));


	//TODO:�Ժ����ز�����
	diffuse = ambientColor + diffuseColor * diffuse;
}


//2.������߹��������
