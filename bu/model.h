/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _Model_H_		// このマクロ定義がされてなかったら
#define _Model_H_		// ２重インクルード防止のマクロ定義

#include "main.h"

typedef struct
{
	D3DXVECTOR3			pos;		// 位置
	D3DXVECTOR3			rot;		// 回転量
	D3DXVECTOR3			rotDest;	// 目標回転量
	ID3DXMesh*			Mesh;		// メッシュ情報
	D3DMATERIAL9		*Materials;	// マテリアル
	LPDIRECT3DTEXTURE9	*Textures;	// テクスチャ
	LPD3DXBUFFER		mtrlBuffer;		//バッファ
	LPD3DXBUFFER		adjBuffer;	//adjバッファ
	DWORD				numMtrls;		//情報の数
	D3DXMATRIX			mtxWorld;	// ワールドマトリックス
}MODEL;

void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
MODEL *GetModel(void);

#endif