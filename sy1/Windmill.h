#pragma once
#include "P2.h"
#include "Face.h"

class CWindmill
{
public:
	CWindmill(void);
	virtual~CWindmill(void);
	void SetParameter(int R, int r);
	void ReadPoint(void);
	void ReadFace(void);
	void Draw(CDC* pDC);
public:
	int R, r; //风车大小半径
	CP2 P[9]; //风车顶点，中心点为P[0]
	CFace F[4]; //叶片
};

