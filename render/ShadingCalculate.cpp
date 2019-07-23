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

//���������ĵ������
float ComputeCameraToVertor(point_t* vertex, const vector_t* normal, const point_t *camera_pos)
{
	vector_t  CameraToVertor;

	vector_sub(&CameraToVertor, camera_pos, vertex);

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
	float surfaceLight = ComputeNDotL(v1, normal, &dirLight.dir); //����ƹ�

	return surfaceLight;

}

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
	float cull = ComputeCameraToVertor(&center, &surfaceNormal, camera_pos); 

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
	float surfaceLight = ComputeNDotL(&center, &surfaceNormal, &dirLight.dir); //����ƹ�
	//printf("surfaceLight��ǰ%f", surfaceLight);
	return surfaceLight;

}


//1.����������:���ظö������ɫ

color_t Lambert(transform_t* mainTrans, vector_t *v_Obj, vector_t* normal, vector_t* lightPos, color_t diffuseColor, color_t ambientColor)
{
	//������任����ͼ�ռ�  
	vector_t*  ObjInView = NULL;
	transform_apply(mainTrans, ObjInView, v_Obj);


	//������任�����͸�ӿռ�  
	vector_t*  ObjInCVV = NULL;
	transform_apply(mainTrans, ObjInCVV, v_Obj);

	vector_t* normalInView = NULL;
	//�����߱任����ͼ�ռ�
	matrix_apply(normalInView, normal, &mainTrans->mv);


	//TODO:����ѵƹ�ת����ͼ�ռ�

	// ������ͼ�ռ�ĵƹⷽ��
	vector_t* lightDirInView = NULL;

	vector_sub(lightDirInView, lightPos, ObjInView);

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
	float diffuse = max(0, vector_dotproduct(normalInView, lightDirInView));

	return ambientColor + diffuseColor * diffuse;
}


//2.������߹�������� Ҳ����Phong����ģ��
color_t Phong(transform_t* mainTrans, vector_t *posInObj, vector_t *normal, vector_t* lightPos, vector_t* cameraPos, color_t diffuseColor, color_t ambientColor, color_t specularColor)
{

	vector_t* posInCvv = NULL, *normalInView = NULL, *posInView = NULL;

	//	Output.position = mul(Input.position, matWorldViewProjection);
	//	Output.normalInView = normalize(mul(Input.normal, matWorldView));
	//	Output.lightDirInView = normalize(lightPos - mul(Input.position, matWorldView));
	//	float3 position = mul(Input.position, matWorldView);
	//	Output.viewDirInView = normalize(matView[3].xyz - position);
	//	return Output;

	//������任����ͼ�ռ�  
	transform_apply(mainTrans, posInObj, posInView);


	//������任�����͸�ӿռ�  
	transform_apply(mainTrans, posInObj, posInCvv);

	//�����߱任����ͼ�ռ�
	matrix_apply(normalInView, normal, &mainTrans->mv);

	// ������ͼ�ռ�ĵƹⷽ��
	vector_t* lightDirInView = NULL;

	vector_sub(lightDirInView, lightPos, posInView);

	//������ͼ�ؼ��������������ķ���
	vector_t* cameraDirInView = NULL;

	vector_sub(cameraDirInView, cameraPos, posInView);

	vector_normalize(cameraDirInView);

	//----------------pix shader-----------------------------------//
	//	float4 diffuse = max(0, dot(In.normalInView, In.lightDirInView));
	//	diffuse = diffuse * diffuseColor;
	//
	//	float3 vReflect = normalize(2 * dot(In.normalInView, In.lightDirInView) * In.normalInView - In.lightDirInView);
	//
	//	float4 specular = specularColor * pow(max(0, dot(vReflect, In.viewDirInView)), 2);
	//
	//	Out.color = ambientColor + diffuse + specular;

	float diffuse = max(0, vector_dotproduct(normalInView, lightDirInView));
	diffuseColor = diffuseColor* diffuse;

	//��������
	vector_t  vReflect = (*normalInView) * (2 * vector_dotproduct(normalInView, lightDirInView)) - *lightDirInView;
	vector_normalize(&vReflect);
	//��ͨ���������߹�

	return  ambientColor + diffuseColor + specularColor * pow(max(0, vector_dotproduct(&vReflect, cameraDirInView)), 2);
}

//TODO: ��ֵ�Ժ����
float PixShader_Phong()
{
	return 0;
}

//Lambertģ���ܺܺõı�ʾ�ֲڱ���Ĺ��գ������ܱ��ֳ����淴��߹⡣1975��Phong Bui Tong������Phongģ�ͣ�����˼��㾵��߹�ľ���ģ�ͣ����淴���ǿ�뷴����ߺ����ߵļн�a��أ�
//
//Ispecular = Ks * Is*(cos a) n
//
//����KsΪ�������ĸ߹�ϵ����IsΪ��ǿ��a�Ƿ���������ߵļнǣ�nΪ�߹�ָ����nԽ�������Խ�⻬�������Խ���У��߹ⷶΧԽС�����V��ʾ���㵽�ӵ�ĵ�λ������R��ʾ����ⷴ����cos a�ɱ�ʾΪV��R�ĵ����ģ�Ϳɱ�ʾΪ��
//
//Ispecular = Ks * Is*(V��R)n
//
//��������R������������L������ָ���Դ�������巨����N�����
//
//R = (2N��L)N �C L
//
//1) VertexShader
//float4x4 matWorldViewProjection;
//float4x4 matWorldView;
//float4x4 matView;
//float3 lightPos;
//
//struct VS_INPUT
//{
//	float4 position : POSITION0;
//	float3 normal : NORMAL;
//};
//
//struct VS_OUTPUT
//{
//	float4 position : POSITION0;
//	float3 normalInView : TEXCOORD0;
//	float3 lightDirInView : TEXCOORD1;
//	float3 viewDirInView : TEXCOORD2;
//};
//
//VS_OUTPUT vs_main(VS_INPUT Input)
//{
//	VS_OUTPUT Output;
//
//	Output.position = mul(Input.position, matWorldViewProjection);
//	Output.normalInView = normalize(mul(Input.normal, matWorldView));
//	Output.lightDirInView = normalize(lightPos - mul(Input.position, matWorldView));
//	float3 position = mul(Input.position, matWorldView);
//	Output.viewDirInView = normalize(matView[3].xyz - position);
//	return Output;
//}
//
//
//
//2)	PixelShader
//
//float4 ambientColor;
//float4 diffuseColor;
//float4 specularColor;
//
//struct PS_INPUT
//{
//	float3 normalInView : TEXCOORD0;
//	float3 lightDirInView : TEXCOORD1;
//	float3 viewDirInView : TEXCOORD2;
//};
//
//struct PS_OUTPUT
//{
//	float4 color : COLOR0;
//};
//
//
//PS_OUTPUT ps_main(PS_INPUT In)
//{
//	PS_OUTPUT Out;
//
//	float4 diffuse = max(0, dot(In.normalInView, In.lightDirInView));
//	diffuse = diffuse * diffuseColor;
//
//	float3 vReflect = normalize(2 * dot(In.normalInView, In.lightDirInView) * In.normalInView - In.lightDirInView);
//
//	float4 specular = specularColor * pow(max(0, dot(vReflect, In.viewDirInView)), 2);
//
//	Out.color = ambientColor + diffuse + specular;
//
//	return Out;
//}

