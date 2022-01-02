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
	D3DXVECTOR3	DestPosV;			// 視点の目的の位置
	D3DXVECTOR3	DestPosR;			// 注視点の目的の位置
	D3DXVECTOR3	rotDest;			// 目的の向き
	D3DXVECTOR3 rot;
	float fDistance;
	float fDistanceFocalPoint;		//注視点からプレイヤーまでの距離
	float fAngle;
	D3DXMATRIX mtxProjection;// プロジェクションマトリックス
	D3DXMATRIX mtxView;		// ビューマトリックス
	bool bfollow;
	int nDelay;
}CAMERA;

// プロトタイプ宣言
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
void FollowCamera(void);
CAMERA *GetCamera(void);

#endif