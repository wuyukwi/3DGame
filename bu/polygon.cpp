/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#include "polygon.h"
#include "set.h"


//------------------------------------
// static変数
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	// 頂点バッファーへのポインタ
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;	// テクスチャへのポインタ
static POLYGON					polygon;			// ポリゴンの構造体

//=========================================
// 初期化処理
//=========================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 初期化処理
	polygon.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	
	polygon.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	
	
	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/field004.jpg",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx = NULL;

	float size=300.0f;

	// 頂点座標をロック
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	SetposWall(pVtx,-size, size, size,-size,0);

	// 各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	float texsize = size / 50.0f;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(texsize, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, texsize);
	pVtx[3].tex = D3DXVECTOR2(texsize, texsize);

	// 頂点座標をアンロック
	s_pVtxBuff->Unlock();

}

//=========================================
// 終了処理
//=========================================
void UninitPolygon(void)
{
	// 頂点バッファーの解放
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//=========================================
// 更新処理
//=========================================
void UpdatePolygon(void)
{
}

//=========================================
// 描画処理
//=========================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&polygon.mtxWorld);	// 行列初期化関数(第1引数の行列を単位行列に初期化)

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, polygon.rot.y, polygon.rot.x, polygon.rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
	D3DXMatrixMultiply(&polygon.mtxWorld, &polygon.mtxWorld, &mtxRot);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, polygon.pos.x, polygon.pos.y, polygon.pos.z);			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixMultiply(&polygon.mtxWorld, &polygon.mtxWorld, &mtxTrans);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &polygon.mtxWorld);	// ワールド座標行列の設定

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

POLYGON *GetPolygon(void)
{
	return &polygon;
}