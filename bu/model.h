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
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転量
	D3DXVECTOR3 moveRot;		// 移動量
	LPD3DXMESH MeshModel;
	D3DMATERIAL9		*MeshMaterials;	// メッシュのマテリアル
	LPDIRECT3DTEXTURE9	*MeshTextures ;	// メッシュのテクスチャ
	int			shadow;
}MODEL;

void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
MODEL *GetModel(void);

#endif