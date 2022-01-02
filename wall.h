/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _Wall_H_
#define _Wall_H_

#include "main.h"

// マクロ定義


//壁の構造体
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	D3DXCOLOR		col;			// カラー
	float fWidth;				//壁の幅
	float fHeight;				//壁の高さ
	bool			bUse;			// 使用状況
}Wall;


//プロトタイプ宣言
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col);

#endif // !_Wall_H_
