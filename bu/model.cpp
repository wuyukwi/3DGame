/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#include "model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"

#define FILE_NAME	"data/model/timo.x"

static void MoveModel();
static void normalized(float *rot);

// グローバル変数
LPD3DXMESH			g_pMeshModel = NULL;		// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pBuffMatMoldel = NULL;	// マテリアル情報へのポインタ
LPD3DXBUFFER		g_pBuffadj = NULL;			// マテリアル情報へのポインタ
DWORD				g_nNumMatModel = 0;			// マテリアル情報の数
D3DXMATRIX			g_mtxWorldModel;			// ワールドマトリックス
D3DMATERIAL9		*g_pMeshMaterials = NULL;	// メッシュのマテリアル
LPDIRECT3DTEXTURE9	*g_pMeshTextures = NULL;	// メッシュのテクスチャ
D3DXMATERIAL		*pMat;						// マテリアルデータへのポインタ
MODEL				g_aModel;					// モデル構造体の宣言
D3DXVECTOR3			g_rotDestModel;

// モデルの初期化処理
void InitModel(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// xファイルの読み込み
	D3DXLoadMeshFromX(FILE_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		&g_pBuffadj,
		&g_pBuffMatMoldel,
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel);


	// 初期化処理
	g_aModel.pos = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
	g_aModel.rot = D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	g_aModel.moveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotDestModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatMoldel->GetBufferPointer();

	g_pMeshMaterials = new D3DMATERIAL9[g_nNumMatModel];
	g_aModel.MeshMaterials = g_pMeshMaterials;

	g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_nNumMatModel];
	g_aModel.MeshTextures = g_pMeshTextures;

	g_aModel.MeshModel = g_pMeshModel;

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
	{
		//  Copy the material
		g_pMeshMaterials[nCntMat] = pMat[nCntMat].MatD3D;

		//  Set the ambient color for the material (D3DX does not do this)
		g_pMeshMaterials[nCntMat].Ambient = g_pMeshMaterials[nCntMat].Diffuse;

		g_pMeshTextures[nCntMat] = NULL;
		if (pMat[nCntMat].pTextureFilename != NULL &&
			lstrlenA(pMat[nCntMat].pTextureFilename) > 0)
		{
			D3DXCreateTextureFromFileA(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pMeshTextures[nCntMat]);
		}

	}


}

// モデルの終了処理
void UninitModel(void)
{
	if (g_pMeshMaterials != NULL)
	{
		delete[] g_pMeshMaterials;
	}

	if (g_pMeshTextures)
	{
		for (DWORD i = 0; i < g_nNumMatModel; i++)
		{
			if (g_pMeshTextures[i])
				g_pMeshTextures[i]->Release();
		}
		delete[] g_pMeshTextures;
	}

	// メッシュの廃棄
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	// マテリアルの廃棄
	if (g_pBuffMatMoldel != NULL)
	{
		g_pBuffMatMoldel->Release();
		g_pBuffMatMoldel = NULL;
	}
}

// モデルの更新処理
void UpdateModel(void)
{
	float DownSpeed = 0.1f;	//スピード減衰係数

	MoveModel();
	
	float fAngle = g_aModel.moveRot.y - g_aModel.rot.y;

	// 角度の正規化
	normalized(&fAngle);

	//慣性・向きを更新 (減衰させる)
	g_aModel.rot.y += fAngle * DownSpeed;

	// 角度の正規化
	normalized(&g_aModel.rot.y);

}


// モデルの描画処理
void DrawModel(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
	

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldModel);		//  行列初期化関数(第1引数の行列を単位行列に初期化)

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel.rot.y, g_aModel.rot.x, g_aModel.rot.z);		//  行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);								//  行列掛け算関数(第2引数×第3引数第を１引数に格納)
																									
	D3DXMatrixTranslation(&mtxTrans, g_aModel.pos.x, g_aModel.pos.y, g_aModel.pos.z);				//  行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);								//  行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 回転
	// D3DXMatrixRotationY(&g_mtxWorldModel, timeGetTime() / 1000.0f);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);
	

	//  モデルの設定
	for (DWORD i = 0; i < g_nNumMatModel; i++)
	{
		//  マテリアルの設定
		pDevice->SetMaterial(&g_pMeshMaterials[i]);
		pDevice->SetTexture(0, g_pMeshTextures[i]);

		// モデルパーツの描写
		g_pMeshModel->DrawSubset(i);


	}


}

MODEL *GetModel(void)
{
	return &g_aModel;
}

static void MoveModel()
{
	CAMERA *pCamera = GetCamera();		//カメラの取得
	float fRot = 0.0f;
	float fMoveSpeed = 2.0f;	//スピード

	// モデルの移動
	if (GetKeyboardPress(DIK_LEFT))
	{// ←キーが押された
		if (GetKeyboardPress(DIK_UP))
		{// ↑キーが押された
			fRot = pCamera->rot.y + (D3DX_PI * 0.75f);

			g_aModel.moveRot.y = pCamera->rot.y + (-D3DX_PI * 0.25f);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ↓キーが押された
			fRot = pCamera->rot.y + (D3DX_PI * 0.25f);

			g_aModel.moveRot.y = pCamera->rot.y + (-D3DX_PI * 0.75f);
		}
		else
		{
			fRot = pCamera->rot.y + (D3DX_PI * 0.5f);

			g_aModel.moveRot.y = pCamera->rot.y + (-D3DX_PI * 0.5f);
		}

	}
	else if (GetKeyboardPress(DIK_RIGHT))
	{// →キーが押された
		if (GetKeyboardPress(DIK_UP))
		{// ↑キーが押された
			fRot = pCamera->rot.y + (-D3DX_PI * 0.75f);

			g_aModel.moveRot.y = pCamera->rot.y + (D3DX_PI * 0.25f);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ↓キーが押された
			fRot = pCamera->rot.y + (-D3DX_PI * 0.25f);

			g_aModel.moveRot.y = pCamera->rot.y + (D3DX_PI * 0.75f);
		}
		else
		{
			fRot = pCamera->rot.y + (-D3DX_PI * 0.5f);

			g_aModel.moveRot.y = pCamera->rot.y  + (D3DX_PI * 0.5f);
		}


	}
	else if (GetKeyboardPress(DIK_UP))
	{// ↑キーが押された
		fRot = pCamera->rot.y + D3DX_PI;

		g_aModel.moveRot.y = pCamera->rot.y;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{// ↓キーが押された
		fRot = pCamera->rot.y;

		g_aModel.moveRot.y = pCamera->rot.y + D3DX_PI;
	}

	if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
		GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
	{// ←, →, ↑, ↓キーが押された
		g_aModel.pos.x += sinf(fRot) * fMoveSpeed;
		g_aModel.pos.z += cosf(fRot) * fMoveSpeed;
	}

	// モデルの回転
	if (GetKeyboardPress(DIK_NUMPAD2))
	{
		g_aModel.moveRot.y += 0.1f;
	}
	else if (GetKeyboardPress(DIK_NUMPAD3))
	{
		g_aModel.moveRot.y -= 0.1f;
	}

	// モデルの上下移動
	if (GetKeyboardPress(DIK_NUMPAD1))
	{
		g_aModel.pos.y += 0.5f;
	}
	else if (GetKeyboardPress(DIK_NUMPAD0))
	{
		g_aModel.pos.y -= 0.5f;
	}
}

static void normalized(float *rot)
{
	if (*rot >= D3DX_PI)
	{
		*rot -= D3DX_PI * 2.0f;
	}
	else if (*rot <= -D3DX_PI)
	{
		*rot += D3DX_PI * 2.0f;
	}
}
