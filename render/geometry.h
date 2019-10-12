#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__
#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

//��������� int float  ��ά/��ά/��ά ������/���� ������
//�㡢���������ߡ�����ȵȼ���ѧ�ϵ�����ͼ�⣬����ı任������

//ģ����������������ں���
template<size_t DimCols, size_t DimRows, typename T> 
class matrix_t;

//����һ����������
template <size_t DIM, typename T> 
struct vec {
	
	vec() 
	{ 
		for (size_t i = DIM; i--; data_[i] = T()); 
	}

	//�ѷ���ʹ��assert()��ԭ���ǣ�Ƶ���ĵ��ûἫ���Ӱ���������ܣ����Ӷ���Ŀ�����
	T& operator[](const size_t i) 
	{ 
		//assert(i < DIM); 
		return data_[i]; 
	}
	const T& operator[](const size_t i) const 
	{ 
		//assert(i < DIM); 

		return data_[i];
	}
//private:
public:
	T data_[DIM]; 
};

/////////////////////////////////////////////////////////////////////////////////

//ģ��ƫ�ػ�
template <typename T> 
struct vec<2, T> {
	vec() : x(T()), y(T()) {}
	vec(T X, T Y) : x(X), y(Y) {}

	//Ƕ�� ���㴫�����͸���������T ��һ��
	template <class U> 
	vec<2, T>(const vec<2, U>& v);

	T& operator[](const size_t i) { 
		//assert(i < 2); 
		return i <= 0 ? x : y;
	}

	const T& operator[](const size_t i) const { 
		//assert(i < 2); 
		return i <= 0 ? x : y; 
	}

	vec<2, T>& operator-(const vec<2, T>& v) {

		x -= v.x;
		y -= v.y;
		return *this;
	}

	vec<2, T>& operator+=(const vec<2, T>& v) {

		x += v.x;
		y += v.y;
		return *this;
	}

	vec<2, T>& operator*(const vec<2, T>& v) {

		x *= v.x;
		y *= v.y;
		return *this;
	}
	vec<2, T>& operator*=(T f) {
		x *= f;
		y *= f;
		return *this;
	}

	vec<2, T>& operator=(const vec<2, T>& v) {

		x = v.x;
		y = v.y;
		return *this;
	}

	T x, y;
};

/////////////////////////////////////////////////////////////////////////////////

template <typename T> 
struct vec<3, T> {
	vec() : x(T()), y(T()), z(T()) {}
	vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

	template <class U> 
	vec<3, T>(const vec<3, U>& v);

	T& operator[](const size_t i) { 
		//assert(i < 3); 
		return i <= 0 ? x : (1 == i ? y : z); 
	}

	template <class U>
	vec<3, T>& operator=(const vec<3, U>& v) {
		
		x = T(v.x + .5f);
		y = T(v.y + .5f);
		z = T(v.z + .5f);
		return *this;
	}

	const T& operator[](const size_t i) const 
	{ 
		//assert(i < 3); 
		return i <= 0 ? x : (1 == i ? y : z);
	}
	// ʸ����һ��  ע���һ�������漰W�� ֻ����ά����άͶӰ��ʱ��W��һ��������
	float norm() { return std::sqrt(x * x + y * y + z * z); }
	vec<3, T>& normalize(T l = 1) {
		//if (length != 0.0f)
		*this = (*this) * (l / norm()); 
		return *this;
	}

	T x, y, z;
};


template <typename T>
struct vec<4, T> {
	vec() : x(T()), y(T()), z(T()),w(T()) {}
	vec(T X, T Y, T Z) : x(X), y(Y), z(Z), w(T()) {}
	vec(T X, T Y, T Z,T W) : x(X), y(Y), z(Z), w(W) {}
	template <class U>
	vec<4, T>(const vec<4, U>& v);

	T& operator[](const size_t i) {
		//assert(i < 4);
		return i <= 0 ? x : (1 == i ? y : (2 == i ? z:w));
	}

	vec<4, T>& operator=(const vec<4, T>& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	vec<4, T>& operator*=(T f) {
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}

	template <class U>
	vec<4, T>& operator=(const vec<4, U>& v) {

		x = T(v.x + .5f);
		y = T(v.y + .5f);
		z = T(v.z + .5f);
		w = T(v.w + .5f);
		return *this;
	}
	
//�������ĵڶ�����Ч����������������.  wΪ1��Ϊ��.  wΪ0��Ϊ������  ��Ϊw������λ�ƣ�����λ��ûʲô���塣
// z = x + y  wһֱΪ1
	template <class U>
	vec<4, T>& operator+=(const vec<4, U>& v) {

		x += T(v.x);
		y += T(v.y);
		z += T(v.z);
		w += T(v.w);
		return *this;
	}

	vec<4, T>& operator+=(const vec<4, T>& v) {

		x += (v.x);
		y += (v.y);
		z += (v.z);
		w += (v.w);
		return *this;
	}

	template <class U>
	vec<4, T>& operator-(const vec<4, U>& u) {

		x = T(this->x - u.x);
		y = T(this->y - u.y);
		z = T(this->z - u.z);
		return *this;
	}

	vec<4, T>& operator-(const vec<4, T>& u) {

		x = T(this->x - u.x);
		y = T(this->y - u.y);
		z = T(this->z - u.z);
		return *this;
	}

	vec<4, T>& operator*(float u) {

		x = T(this->x * u);
		y = T(this->y * u);
		z = T(this->z * u);
		return *this;
	}

	//ʸ�����
	template <class U>
	float operator*(vec<4, U> v) {
		return x * v.x + y * v.y + z * v.z;
	}


	const T& operator[](const size_t i) const
	{
	//	assert(i < 4);
		return i <= 0 ? x : (1 == i ? y : (2 == i ? z : w));
	}

	//vector_length   |v|
	float norm() { 
		return std::sqrt(x * x + y * y + z * z); 
	}

	//
	vec<4, T>& normalize(T l = 1) { 
		x= x * (l / norm());
		y = y * (l / norm());
		z = z * (l / norm());
		return *this; 
	}

	T x, y, z,w;
};


/////////////////////////////////////////////////////////////////////////////////

template<typename T>
T interp(T x1, T x2, T t) {
	return x1 + (x2 - x1) * t;
}


template<typename T, typename U>
T interp(const T& x1, const T& x2, U t) {
	return x1 + (x2 - x1) * t;
}

template<typename T>
//�������������� int
T CMID(T x, T min, T max)
{
	return (x < min) ? min : ((x > max) ? max : x);
}


/////////////////////////////////////////////////////////////////////////////////

template<size_t DIM, typename T> 
T operator*(const vec<DIM, T>& lhs, const vec<DIM, T>& rhs) {
	T ret = T();
	for (size_t i = DIM; i--; ret += lhs[i] * rhs[i]);
	return ret;
}



template<size_t DIM, typename T>vec<DIM, T> 
operator+(vec<DIM, T> lhs, const vec<DIM, T>& rhs)
{
	for (size_t i = DIM; i--; lhs[i] += rhs[i]);
	return lhs;
}

template<size_t DIM, typename T>vec<DIM, T> 
operator-(vec<DIM, T> lhs, const vec<DIM, T>& rhs) {
	for (size_t i = DIM; i--; lhs[i] -= rhs[i]);
	return lhs;
}

template<size_t DIM, typename T> vec<DIM, T> 
operator*(vec<DIM, T> lhs, const T& rhs) {
	for (size_t i = DIM; i--; lhs[i] = lhs[i] * rhs);
	return lhs;
}

template<size_t DIM, typename T, typename U> vec<DIM, T> 
operator/(vec<DIM, T> lhs, const U& rhs) {
	for (size_t i = DIM; i--; lhs[i] /= rhs);
	return lhs;
}

template<size_t LEN, size_t DIM, typename T> vec<LEN, T> 
embed(const vec<DIM, T>& v, T fill = 1) {
	vec<LEN, T> ret;
	for (size_t i = LEN; i--; ret[i] = (i < DIM ? v[i] : fill));
	return ret;
}

template<size_t LEN, size_t DIM, typename T> 
vec<LEN, T> proj(const vec<DIM, T>& v) {
	vec<LEN, T> ret;
	for (size_t i = LEN; i--; ret[i] = v[i]);
	return ret;
}

template <typename T> 
vec<3, T> cross(const vec<3, T>& v1, const vec<3, T>& v2) {
	return vec<3, T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}
// ʸ�����  ��� x y ��׼������������Ĳ��ZҲ�Ǳ�׼����xyƽ��ķ��ߡ� ����Ļ���Ҫ��׼��һ��Z
template <typename T>
vec<4, T> cross(const vec<4, T>& v1, const vec<4, T>& v2) { 
	return vec<4, T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x,1);
}

template <size_t DIM, typename T> 
std::ostream& operator<<(std::ostream& out, vec<DIM, T>& v) {
	for (unsigned int i = 0; i < DIM; i++) {
		out << v[i] << " ";
	}
	return out;
}

/////////////////////////////////////////////////////////////////////////////////

template<size_t DIM, typename T> 
struct dt {
	static T det(const matrix_t<DIM, DIM, T>& src) {
		T ret = 0;
		for (size_t i = DIM; i--; ret += src[0][i] * src.cofactor(0, i));
		return ret;
	}
};

template<typename T> 
struct dt<1, T> {
	static T det(const matrix_t<1, 1, T>& src) {
		return src[0][0];
	}
};

/////////////////////////////////////////////////////////////////////////////////
//�� -��
template<size_t DimRows, size_t DimCols, typename T> 
class matrix_t {
	vec<DimCols, T> rows[DimRows];
public:
	matrix_t() {
		
	}

	vec<DimCols, T>& operator[] (const size_t idx) {
		//assert(idx < DimRows);
		return rows[idx];
	}

	const vec<DimCols, T>& operator[] (const size_t idx) const {
		//assert(idx < DimRows);
		return rows[idx];
	}

	 matrix_t<DimRows, DimCols, T>& set_zero(){
		for (size_t i = DimRows; i--; )
			for (size_t j = DimCols; j--; (*this)[i][j] = (T)0);
		return *this;
	}

	 //���ã����ܲ�
	vec<DimRows, T> col(const size_t idx) const {
		//assert(idx < DimCols);
		vec<DimRows, T> ret;
		for (size_t i = DimRows; i--; ret[i] = rows[i][idx]);
		return ret;
	}

	void set_col(size_t idx, vec<DimRows, T> v) {
		//assert(idx < DimCols);
		for (size_t i = DimRows; i--; rows[i][idx] = v[i]);
	}
	//�����������ʽ
	T det() const {
		return dt<DimCols, T>::det(*this);
	}

	//��С��������
	matrix_t<DimRows - 1, DimCols - 1, T> get_minor(size_t row, size_t col) const {
		matrix_t<DimRows - 1, DimCols - 1, T> ret;
		for (size_t i = DimRows - 1; i--; )
			for (size_t j = DimCols - 1; j--; ret[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1]);
		return ret;
	}

	//��������ʽ����
	T cofactor(size_t row, size_t col) const {
		return get_minor(row, col).det() * ((row + col) % 2 ? -1 : 1);
	}

	//�������
	matrix_t<DimRows, DimCols, T> adjugate() const {
		matrix_t<DimRows, DimCols, T> ret;
		for (size_t i = DimRows; i--; )
			for (size_t j = DimCols; j--; ret[i][j] = cofactor(i, j));
		return ret;
	}

	//��ת�þ���
	matrix_t<DimRows, DimCols, T> invert_transpose() {
		matrix_t<DimRows, DimCols, T> ret = adjugate();
		T tmp = ret[0] * rows[0];
		return ret / tmp;
	}

	//�����
	matrix_t<DimRows, DimCols, T> invert() {
		return invert_transpose().transpose();
	}

	//ת�þ���
	matrix_t<DimCols, DimRows, T> transpose() {
		matrix_t<DimCols, DimRows, T> ret;
		for (size_t i = DimCols; i--; ret[i] = this->col(i));
		return ret;
	}

	//��׼����
	matrix_t<DimRows, DimCols, T>& identity() {

		for (size_t i = DimRows; i--; )
			for (size_t j = DimCols; j--; (rows)[i][j] = (i == j)); //true =1  false=0
		return *this;
	}
};

//////////////////////////��������������///////////////////////////////////////////////////////


//�����ľ���任
template<size_t DimRows, size_t DimCols, typename T>
void  cross(vec<DimRows, T>& ret,const vec<DimCols, T>& rhs, const matrix_t<DimRows, DimCols, T>& lhs) {
	for (size_t i = DimRows; i--; ret[i] = lhs[i] * rhs);
}


// TODO: ���һ�����ž��� ���
template<size_t DimRows, typename T>
matrix_t<DimRows, DimRows, T> matrix_set_scale(const vec<DimRows, T>& rhs) {
	matrix_t<DimRows, DimRows, T> ret = matrix_t<DimRows, DimRows, T>().identity();
	for (size_t i = DimRows; i--; )
		ret[i][i] = rhs[i];
	return ret;
}


// TODO: ���һ��ƽ�Ʊ任���� ���
template<size_t DimRows, size_t DimCols, typename T>
matrix_t<DimRows, DimCols, T> matrix_set_translate(const vec<DimRows, T>& rhs) {
	matrix_t<DimRows, DimCols, T> ret = identity();
	for (size_t i = DimRows; i--; )
		 ret[i][DimCols-1] = rhs[i];
	return ret;
}

//���ܲ����
template<size_t R1, size_t C1, size_t C2, typename T>
matrix_t<R1, C2, T> operator*(const matrix_t<R1, C1, T>& lhs, const matrix_t<C1, C2, T>& rhs) {
	matrix_t<R1, C2, T> result;

	for (size_t i = R1; i--;)
		//for (size_t j = C2; j--; result[i][j] = lhs[i] * rhs.col(j));
		for (size_t j = C2; j--;)
		{
			for (int k = 0; k < C1; k++)
			{
					result[i][j] += lhs[i][k] * rhs[k][j];
			}
		}
	return result;
}
//void matrix_mul(Matrix44f*c, const Matrix44f*left , const Matrix44f*right) {
//	int i, j;
//	for (i = 0; i < 4; i++)
//	{
//		for (j = 0; j < 4; j++)
//		{
//			c[i][j] =
//				(left[i][0] * right[0][j]) +
//				(left[i][1] * right[1][j]) +
//				(left[i][2] * right[2][j]) +
//				(left[i][3] * right[3][j]);
//		}
//	}
//}



template<size_t DimRows, size_t DimCols, typename T>
matrix_t<DimCols, DimRows, T> operator/(matrix_t<DimRows, DimCols, T> lhs, const T& rhs) {
	for (size_t i = DimRows; i--; lhs[i] = lhs[i] / rhs);
	return lhs;
}

template <size_t DimRows, size_t DimCols, class T> 
std::ostream& operator<<(std::ostream& out, matrix_t<DimRows, DimCols, T>& m) {
	for (size_t i = 0; i < DimRows; i++) out << m[i] << std::endl;
	return out;
}

/////////////////////////////////////////////////////////////////////////////////

typedef vec<2, float> Vec2f;
typedef vec<2, int>   Vec2i;
typedef vec<3, float> Vec3f;
typedef vec<3, int>   Vec3i;
typedef vec<4, float> Vec4f;
typedef matrix_t<4, 4, float> Matrix44f;
typedef Vec4f point_t;
typedef Vec4f color_t;


#endif //__GEOMETRY_H__



