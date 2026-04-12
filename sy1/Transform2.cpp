#include "pch.h"
#include "Transform2.h"
#include <math.h>

#define PI 3.1415926
#define ARC(angle) (angle) * PI / 180

CTransform2::CTransform2(void) { P = NULL; ptNumber = 0; }
CTransform2::~CTransform2(void) {}

void CTransform2::Identity(void) {
	T[0][0] = 1; T[0][1] = 0; T[0][2] = 0;
	T[1][0] = 0; T[1][1] = 1; T[1][2] = 0;
	T[2][0] = 0; T[2][1] = 0; T[2][2] = 1;
}

void CTransform2::SetMatrix(CP2* P, int ptNumber) {
	this->P = P;
	this->ptNumber = ptNumber;
}

void CTransform2::Rotate(double beta) {
	Identity();
	double rad = ARC(beta);
	T[0][0] = cos(rad);  T[0][1] = sin(rad);
	T[1][0] = -sin(rad); T[1][1] = cos(rad);
	MultiplyMatrix();
}

void CTransform2::MultiplyMatrix(void) {
	for (int i = 0; i < ptNumber; i++) {
		double oldX = P[i].x;
		double oldY = P[i].y;
		P[i].x = oldX * T[0][0] + oldY * T[1][0] + T[2][0];
		P[i].y = oldX * T[0][1] + oldY * T[1][1] + T[2][1];
	}
}