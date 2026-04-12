#pragma once

class CP2
{
public:
	CP2(void);
	virtual ~CP2(void);
	CP2(double x, double y);
	friend CP2 operator + (const CP2& p0, const CP2& p1);//运算符重载
	friend CP2 operator - (const CP2& p0, const CP2& p1);
	friend CP2 operator * (const CP2& p, double scalar);
	friend CP2 operator * (double scalar, const CP2& p);
	friend CP2 operator / (const CP2& p, double scalar);
	friend CP2 operator += (CP2& p0, CP2& p1);
	friend CP2 operator -= (CP2& p0, CP2& p1);
	friend CP2 operator *= (CP2& p, double scalar);
	friend CP2 operator /= (CP2& p, double scalar);
public:
	double x;
	double y;
	double w; //齐次坐标系数，规范化的齐次坐标w=1
};

