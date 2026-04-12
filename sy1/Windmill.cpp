#include "pch.h"
#include "Windmill.h"
#include <math.h>

#define PI 3.1415926
#define ARC(angle) (angle) * PI / 180
#define ROUND(d) int(d + 0.5)

CWindmill::CWindmill(void) {
    R = 200; r = 60; // 初始化内外圆半径 [cite: 6]
    ReadPoint();
    ReadFace();
}

CWindmill::~CWindmill(void) {}

void CWindmill::SetParameter(int R, int r) {
    this->R = R;
    this->r = r;
    ReadPoint();
}

void CWindmill::ReadPoint(void) {
    P[0] = CP2(0, 0); // 原点 [cite: 8, 9]
    for (int i = 0; i < 4; i++) {
        // 大圆顶点 P2, P4, P6, P8 [cite: 14]
        P[2 * i + 2].x = R * cos(ARC(i * 90));
        P[2 * i + 2].y = R * sin(ARC(i * 90));
        // 小圆顶点 P1, P3, P5, P7 [cite: 14]
        P[2 * i + 1].x = r * cos(ARC(i * 90 + 45));
        P[2 * i + 1].y = r * sin(ARC(i * 90 + 45));
    }
}

void CWindmill::ReadFace(void) {


    for (int i = 0; i < 4; i++) {
        F[i].vI[0] = 0;           // 中心点
        F[i].vI[1] = 2 * i + 2;   // 大圆点
        F[i].vI[2] = 2 * i + 1;   // 小圆点
    }
}

void CWindmill::Draw(CDC* pDC) {
    for (int i = 0; i < 4; i++) {
        // 使用 MoveTo/LineTo 绘制三角形叶片 [cite: 10]
        pDC->MoveTo(ROUND(P[F[i].vI[0]].x), ROUND(P[F[i].vI[0]].y));
        pDC->LineTo(ROUND(P[F[i].vI[1]].x), ROUND(P[F[i].vI[1]].y));
        pDC->LineTo(ROUND(P[F[i].vI[2]].x), ROUND(P[F[i].vI[2]].y));
        pDC->LineTo(ROUND(P[F[i].vI[0]].x), ROUND(P[F[i].vI[0]].y));
    }
}