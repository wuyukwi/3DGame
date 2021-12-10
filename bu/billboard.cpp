#include "billboard.h"
#include "main.h"
#include "set.h"
#include "camera.h"

//
// 構造体を定義
//

//------------頂点フォーマット
#define FVF_BILLBOARDTEX (D3DFVF_XYZ|D3DFVF_TEX1)


//------------頂点構造体
struct VertexBillboard
{
	float _x, _y, _z;//位置座標
	float _u, _v; //テクスチャ座標


	VertexBillboard(float x, float y, float z, float u, float v) : _x(x), _y(y), _z(z), _u(u), _v(v) {}
	VertexBillboard() {}
};


//
// スタティック変数
//
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// 頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9			s_pTexture;					// テクスチャへのポインタ

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitBillboard(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VertexBillboard vertex[4];

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VertexBillboard) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);


	VertexBillboard *pVtx = NULL;

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報
	vertex[0] = VertexBillboard(-100.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertex[1] = VertexBillboard(-100.0f, 700.0f, 0.0f, 0.0f, 0.0f);
	vertex[2] = VertexBillboard(100.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	vertex[3] = VertexBillboard(100.0f, 700.0f, 0.0f, 1.0f, 0.0f);


	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXT("data//TEXTURE//miku.jpg"), &s_pTexture);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitBillboard(void)
{
	// 頂点バッファの廃棄
	Release<LPDIRECT3DVERTEXBUFFER9>(s_pVtxBuff);
	
	//　テクスチャの廃棄
	Release<LPDIRECT3DTEXTURE9>(s_pTexture);
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateBillboard(void)
{
	
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawBillboard(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	D3DXMATRIX  mtxView;		

	// ワールドマトリックスの設定
	D3DXMATRIX mtxCharacter, mtxWorld;
	D3DXMatrixTranslation(&mtxWorld, 0.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&mtxCharacter, 0.0f, 0.0f, 0.0f);

	// カメラ情報の取得
	CAMERA *pCamera = GetCamera();


	// カメラの逆行列を計算
	D3DXMATRIX matBillboard;
	D3DXMatrixIdentity(&matBillboard);
	matBillboard._11 = pCamera->mtxView._11;
	matBillboard._13 = pCamera->mtxView._13;
	matBillboard._31 = pCamera->mtxView._31;
	matBillboard._33 = pCamera->mtxView._33;
	D3DXMatrixInverse(&matBillboard, NULL, &matBillboard);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxCharacter);
	mtxCharacter = matBillboard * mtxCharacter;


	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VertexBillboard));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_BILLBOARDTEX);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,						// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数
	

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fHeight, LPDIRECT3DTEXTURE9 *pTexture)
{
	
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void LoadBillboard(HWND hWnd)
{
	
}