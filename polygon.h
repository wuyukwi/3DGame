/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

void InitPolygon(void);		// ポリゴンの初期化処理
void UninitPolygon(void);	// ポリゴンの終了処理
void UpdatePolygon(void);	// ポリゴンの更新処理
void DrawPolygon(void);		// ポリゴンの描画処理

// マテリアルの初期化処理
D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

// マテリアルの定義
const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 2.0f);
const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

#endif // !_POLYGON_H_
