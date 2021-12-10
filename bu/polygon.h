/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//------------------------------------
// ポリゴンの構造体を定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 rot;	// 回転座標
	D3DXMATRIX mtxWorld;// ワールドマトリックス
}POLYGON;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitPolygon(void);		// ポリゴンの初期化処理
void UninitPolygon(void);	// ポリゴンの終了処理
void UpdatePolygon(void);	// ポリゴンの更新処理
void DrawPolygon(void);		// ポリゴンの描画処理
POLYGON *GetPolygon(void);	//　ポリゴン情報取得

#endif // !_POLYGON_H_
