/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _Wall_H_
#define _Wall_H_

#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitWall(void);		// ポリゴンの初期化処理
void UninitWall(void);	// ポリゴンの終了処理
void UpdateWall(void);	// ポリゴンの更新処理
void DrawWall(void);		// ポリゴンの描画処理

#endif // !_Wall_H_
