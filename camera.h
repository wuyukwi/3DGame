/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _Camera_H_		// このマクロ定義がされてなかったら
#define _Camera_H_		// ２重インクルード防止のマクロ定義

#include "main.h"

typedef struct
{
	D3DXVECTOR3 posV;		// 視点
	D3DXVECTOR3 posR;		// 注視点
	D3DXVECTOR3 vecU;		// 上方向ベクトル
	D3DXVECTOR3 moveV;		// 視点の移動量
	D3DXVECTOR3 moveR;		// 注視点の移動量
	D3DXVECTOR3 rot;
	float fDistance;
	float fAngle;
	D3DXMATRIX mtxProjection;// プロジェクションマトリックス
	D3DXMATRIX mtxView;		// ビューマトリックス
}CAMERA;

// プロトタイプ宣言
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
CAMERA *GetCamera(void);

#endif