/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#include "polygon.h"
#include "set.h"
#include "model.h"

typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 rot;	// 回転座標
	int nSurfaceWidth;	// 面の幅数
	int nSurfaceHeight;	// 面の高さ数
	float fLineWidth;	// 辺の幅
	float fLineHeight;	// 辺の高さ
	int vertexCnt;		// 頂点数
	int polygonCnt;		// ポリゴン数
	int IdxCnt;			// インデックス数
	D3DXMATRIX mtxWorld;// ワールドマトリックス
}Cylinder;

//
// グローバル
//
static LPDIRECT3DVERTEXBUFFER9 s_pVB = NULL;
static LPDIRECT3DINDEXBUFFER9 s_pIB = NULL;
static LPDIRECT3DTEXTURE9 WallTex = NULL; // テクスチャへのポインタ

D3DMATERIAL9 MirrorMtrl = RED_MTRL;

void RenderScene();
void RenderMirror();

static const int NumVertex = 10;
static const int PrimitiveCount = 9;
//
// 初期化処理
//
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	
	Device->CreateVertexBuffer(
		NumVertex * sizeof(VERTEX_3D),
		0, // usage
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVB,
		0);

	VERTEX_3D* v = NULL;

	s_pVB->Lock(0, 0, (void**)&v, 0);

	float size = 60.0f;

	v[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	v[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	SetCol(0, v);
	v[0].tex= D3DXVECTOR2(0.0f, 0.0f);
	for (int i = 0; i < PrimitiveCount; i++)
	{
		float theta = (2*D3DX_PI*i) / PrimitiveCount;

		v[i+1].pos.x= size * sinf(theta);
		v[i+1].pos.y = 0.0f;
		v[i+1].pos.z = size * cosf(theta);
		v[i+1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		SetCol(i+1, v);
		v[i+1].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	s_pVB->Unlock();

	// インデックス
	WORD Indices[] = { 0,1,2, 0,2,3, 0,3,4, 0,4,5, 0,5,6, 0,6,7, 0,7,8, 0,8,9, 0,9,1 };

	// インデックスバッファの生成
	Device->CreateIndexBuffer(
		sizeof(Indices) * sizeof(WORD),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&s_pIB,
		NULL);

	VOID* pIndices;
	// インデックスバッファの書き込み
	s_pIB->Lock(0, sizeof(Indices), (void**)&pIndices, 0);
	memcpy(pIndices, Indices, sizeof(Indices));
	s_pIB->Unlock();

	//
	// Load Textures, set filters.
	//
	D3DXCreateTextureFromFile(Device, "data\\TEXTURE\\ice.bmp", &WallTex);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);
}

//=========================================
// 終了処理
//=========================================
void UninitPolygon(void)
{
	// 頂点バッファーの解放
	Release<LPDIRECT3DVERTEXBUFFER9>(s_pVB);

	// インデックスバッファーの解放
	Release<LPDIRECT3DINDEXBUFFER9>(s_pIB);

	// テクスチャの解放
	Release<LPDIRECT3DTEXTURE9>(WallTex);
}


//
// 更新処理
//
void UpdatePolygon(void)
{

}

//
// 描画処理
//
void DrawPolygon(void)
{
	RenderScene();

	RenderMirror();
}


D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

void RenderScene()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	Device->SetTransform(D3DTS_WORLD, &I);

	Device->SetStreamSource(0, s_pVB, 0, sizeof(VERTEX_3D));
	Device->SetFVF(FVF_VERTEX_3D);

	// 床の描画
	Device->SetMaterial(&MirrorMtrl);
	Device->SetTexture(0, 0);
	Device->SetIndices(s_pIB);	//インデックスバッファの設定
	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NumVertex, 0, PrimitiveCount);
}

void RenderMirror()
{
	//LPDIRECT3DDEVICE9 Device = GetDevice();

	//Device->SetRenderState(D3DRS_STENCILENABLE, true);
	//Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	//Device->SetRenderState(D3DRS_STENCILREF, 0x1);
	//Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	//Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	//Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	//Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	//Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

	//// disable writes to the depth and back buffers
	//Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	//Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	//Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//// draw the mirror to the stencil buffer
	//Device->SetStreamSource(0, VB, 0, sizeof(VERTEX_3D));
	//Device->SetFVF(FVF_VERTEX_3D);
	//Device->SetMaterial(&MirrorMtrl);
	//Device->SetTexture(0, MirrorTex);
	//D3DXMATRIX I;
	//D3DXMatrixIdentity(&I);
	//Device->SetTransform(D3DTS_WORLD, &I);
	//Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);

	//// re-enable depth writes
	//Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	//// only draw reflected teapot to the pixels where the mirror
	//// was drawn to.
	//Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	//Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	//// position reflection
	//D3DXMATRIX W, T, P,R;
	//D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f); // xy plane
	//D3DXMatrixReflect(&P, &plane);

	//MODEL *pModel = GetModel();

	//// 向きを反映
	//D3DXMatrixRotationY(&R, pModel->rot.y);

	//// 位置を反映
	//D3DXMatrixTranslation(&T,
	//	pModel->pos.x,
	//	pModel->pos.y,
	//	pModel->pos.z);

	//W = T * P ;

	//// clear depth buffer and blend the reflected teapot with the mirror
	//Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	//Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	//Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	//// Finally, draw the reflected teapot
	//Device->SetTransform(D3DTS_WORLD, &W);
	//Device->SetMaterial(pModel->Materials);
	//Device->SetTexture(0, *pModel->Textures);

	//Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	//pModel->Mesh->DrawSubset(0);

	//// Restore render states.
	//Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//Device->SetRenderState(D3DRS_STENCILENABLE, false);
	//Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
