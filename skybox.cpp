#include "main.h"
#include "Skybox.h"
#include "set.h"

//
// マクロ定義
//
#define MAX_SKYBOX_SIZE		(5000)		//幅

//
// スタティック変数
//
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// テクスチャへのポインタ

static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファへのポインタ
static IDirect3DIndexBuffer9*		s_pIdxBuff = NULL;
//
// 初期化
//
void InitSkybox(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\skybox.jpg",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_PT) * 24,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_PT,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_PT *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fxsize=1.0f/4;
	float fysize=1.0f/3;


	// 前
	SetAllPT(0,pVtx,-MAX_SKYBOX_SIZE , -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE , fxsize*3, fysize * 2);
	SetAllPT(1, pVtx, -MAX_SKYBOX_SIZE , MAX_SKYBOX_SIZE , MAX_SKYBOX_SIZE , fxsize * 3, fysize);
	SetAllPT(2, pVtx, MAX_SKYBOX_SIZE , -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE , fxsize * 4, fysize * 2);
	SetAllPT(3, pVtx, MAX_SKYBOX_SIZE , MAX_SKYBOX_SIZE , MAX_SKYBOX_SIZE , fxsize * 4, fysize);

	// 後ろ
	SetAllPT(4, pVtx, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize, fysize * 2);
	SetAllPT(5, pVtx, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize, fysize);
	SetAllPT(6, pVtx, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize * 2, fysize * 2);
	SetAllPT(7, pVtx, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize * 2, fysize);

	// 左
	SetAllPT(8, pVtx, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize*2, fysize * 2);
	SetAllPT(9, pVtx, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize * 2, fysize);
	SetAllPT(10, pVtx, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, fxsize * 3 , fysize * 2);
	SetAllPT(11, pVtx, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, fxsize * 3, fysize);

	// 右
	SetAllPT(12, pVtx, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, 0.0f, fysize * 2);
	SetAllPT(13, pVtx, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, 0.0f, fysize);
	SetAllPT(14, pVtx, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize , fysize * 2);
	SetAllPT(15, pVtx, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize , fysize);

	// 上
	SetAllPT(16, pVtx, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize, fysize);
	SetAllPT(17, pVtx, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, fxsize, 0.0f);
	SetAllPT(18, pVtx, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize * 2, fysize);
	SetAllPT(19, pVtx, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, fxsize * 2, 0.0f);

	// 下
	SetAllPT(20, pVtx, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize * 2, fysize * 2);
	SetAllPT(21, pVtx, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, fxsize * 2, fysize * 3);
	SetAllPT(22, pVtx, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize, fysize * 2);
	SetAllPT(23, pVtx, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, fxsize, fysize * 3);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	pDevice->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&s_pIdxBuff,
		0);

	WORD* i = 0;
	s_pIdxBuff->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	s_pIdxBuff->Unlock();
}

//
// 終了
//
void UninitSkybox(void)
{
	if (s_pTexture != NULL)
	{// テクスチャの解放
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pIdxBuff != NULL)
	{// 頂点バッファの解放
		s_pIdxBuff->Release();
		s_pIdxBuff = NULL;
	}

}

//
// 更新
//
void UpdateSkybox(void)
{

}

//
// 描画
//
void DrawSkybox(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans,mtxWorld;		// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixRotationY(&mtxRot, -0.000005f*timeGetTime());   

	mtxWorld = mtxRot*mtxTrans;

	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1); 
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);   

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_PT));
	pDevice->SetIndices(s_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_PT);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < 6; i++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			i*4,							// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数
	}

	//pDevice->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST,
	//	0,
	//	0,
	//	24,
	//	0,
	//	12);

	//// テクスチャの解除
	//pDevice->SetTexture(0, NULL);
}

