/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _Set_H_		// このマクロ定義がされてなかったら
#define _Set_H_		// ２重インクルード防止のマクロ定義

#include "main.h"

//
// Bounding Objects
//

struct BoundingBox
{
	BoundingBox();

	bool isPointInside(D3DXVECTOR3& p);

	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
};

struct BoundingSphere
{
	BoundingSphere();

	D3DXVECTOR3 _center;
	float		_radius;
};

//
// Constants
//

const float INFIN = FLT_MAX;
const float EPSILON = 0.001f;

//
// プロトタイプ
//

void SetAll( int cnt, VERTEX_3D *pVtx,
	float x, float y, float z,
	float nx, float ny, float nz,
	float u, float v);

void SetAllPT(int cnt, VERTEX_PT *pVtx,
	float x, float y, float z,
	float u, float v);

void SetAllPNT(int cnt, VERTEX_PNT *pVtx,
	float x, float y, float z,
	float nx, float ny, float nz,
	float u, float v);

bool ComputeBoundingSphere(ID3DXMesh* mesh, BoundingSphere* sphere);
bool ComputeBoundingBox(ID3DXMesh* mesh, BoundingBox*    box);


#endif


