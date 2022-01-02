/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#include "Wall.h"
#include "model.h"

#define MAX_WALL_VERTEX			(4)					// 壁の頂点数
#define MAX_WALL				(128)				// 壁の最大数
#define MAX_WALL_WIDH			(200.0f)			// 壁の幅
#define MAX_WALL_HEIGHT			(100.0f)			// 壁の高さ

//グローバル変数	
static LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffWall = NULL;		// 頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9			g_pTextureWall = NULL;		// テクスチャへのポインタ
static Wall							g_aWall[MAX_WALL];			// 壁情報の取得

//=========================================
// 初期化処理
//=========================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\checker.jpg",
		&g_pTextureWall);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D*pVtx = NULL;

	// 頂点バッファをロック
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//壁の位置の初期化
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//壁の向きの初期化
		g_aWall[nCntWall].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			//壁のカラーの初期化
		g_aWall[nCntWall].fWidth =200.0f;									//壁の幅の初期化
		g_aWall[nCntWall].fHeight = 50.0f;									//壁の高さの初期化
		g_aWall[nCntWall].bUse = false;										//使用されていない状態にする

																			//頂点座標の設定
		pVtx[(nCntWall * 4) + 0].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z + g_aWall[nCntWall].fHeight);
		pVtx[(nCntWall * 4) + 1].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z + g_aWall[nCntWall].fHeight);
		pVtx[(nCntWall * 4) + 2].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z - g_aWall[nCntWall].fHeight);
		pVtx[(nCntWall * 4) + 3].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z - g_aWall[nCntWall].fHeight);

		//テクスチャ座標の設定
		pVtx[(nCntWall * 4) + 0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[(nCntWall * 4) + 1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[(nCntWall * 4) + 2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[(nCntWall * 4) + 3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[(nCntWall * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntWall * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntWall * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntWall * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[(nCntWall * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 1].tex = D3DXVECTOR2(1.0f + (g_aWall[nCntWall].fWidth / 10.0f), 0.0f);
		pVtx[(nCntWall * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f + (g_aWall[nCntWall].fHeight / 10.0f));
		pVtx[(nCntWall * 4) + 3].tex = D3DXVECTOR2(1.0f + (g_aWall[nCntWall].fWidth / 10.0f), 1.0f + (g_aWall[nCntWall].fHeight / 10.0f));

	}

	// 頂点バッファのアンロック
	g_pVtxBuffWall->Unlock();
}

//=========================================
// 終了処理
//=========================================
void UninitWall(void)
{
	//頂点バッファの破棄
	SAFE_RELEASE(g_pVtxBuffWall);

	//テクスチャへのポインタの破棄
	SAFE_RELEASE(g_pTextureWall);
}

//=========================================
// 更新処理
//=========================================
void UpdateWall(void)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		
		if(g_aWall[nCntWall].bUse)									
		{
			ModelInteraction(&g_aWall[nCntWall].pos, &g_aWall[nCntWall].pos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10, 10, 10);
		}

	}

}

//=========================================
// 描画処理
//=========================================
void DrawWall(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;			// 計算用マトリックス

													// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureWall);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{// 壁が使用されているとき
		 // ワールドマトリックスの初期化
		 // 行列初期化関数(第一引数の[行列]を[単位行列]に初期化)
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

			// 向きの反映
			// 行列回転関数 (第一引数に[ヨー(y)ピッチ(x)ロール(z)]方向の回転行列を作成)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);

			// 行列掛け算関数 (第二引数 * 第三引数を第一引数に格納)
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);

			// 位置を反映
			// 行列移動関数 (第一引数にX,Y,Z方向の移動行列を作成)
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);		// 行列掛け算関数

																										// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

			// 頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			// 頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_3D);

			// 壁描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, MAX_WALL_VERTEX * nCntWall, 2);
		}
	}

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);
}



//---------------------------------------------------------------------------
// 壁設定処理
//---------------------------------------------------------------------------
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	// 変数宣言
	int nCntWall;

	// 頂点情報の取得
	VERTEX_3D *pVtx = NULL;

	// 頂点バッファをロック
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{
			// 壁情報の設定
			g_aWall[nCntWall].pos = pos;																		// 位置
			g_aWall[nCntWall].rot = D3DXVECTOR3((D3DX_PI * rot.x), (D3DX_PI * rot.y), (D3DX_PI * rot.z));		// 向き
			g_aWall[nCntWall].mtxWorld = {};																	// ワールドマトリックス
			g_aWall[nCntWall].col = col;																		// カラー
			g_aWall[nCntWall].bUse = true;																	// 使用状況

																											// 頂点カラーの設定
			pVtx[0].col = g_aWall[nCntWall].col;
			pVtx[1].col = g_aWall[nCntWall].col;
			pVtx[2].col = g_aWall[nCntWall].col;
			pVtx[3].col = g_aWall[nCntWall].col;

			break;
		}

		pVtx += 4;		//頂点データのポインタを4つ分進める
	}

	// 頂点バッファのアンロック
	g_pVtxBuffWall->Unlock();
}