/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#include "polygon.h"
#include "set.h"
#include "model.h"

//
// グローバル
//
IDirect3DVertexBuffer9* VB = NULL;// 頂点バッファーへのポインタ

IDirect3DTexture9* FloorTex  = NULL;//
IDirect3DTexture9* WallTex   = NULL;// テクスチャへのポインタ
IDirect3DTexture9* MirrorTex = NULL;//

D3DMATERIAL9 FloorMtrl  = WHITE_MTRL;
D3DMATERIAL9 WallMtrl   = WHITE_MTRL;
D3DMATERIAL9 MirrorMtrl = WHITE_MTRL;

void RenderScene();
void RenderMirror();

//
// 初期化処理
//
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 壁に低い鏡面反射率を持たせる - 20 % 。
	WallMtrl.Specular = WHITE * 0.2f;

	Device->CreateVertexBuffer(
		24 * sizeof(VERTEX_3D),
		0, // usage
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&VB,
		0);

	VERTEX_3D* v = NULL;
	VB->Lock(0, 0, (void**)&v, 0);

	// floor
	SetAll(0, v, -150.0f, 0.0f, -200.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	SetAll(1, v, -150.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	SetAll(2, v, 150.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	SetAll(3, v, -150.0f, 0.0f, -200.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	SetAll(4, v, 150.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	SetAll(5, v, 150.0f, 0.0f, -200.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// wall
	SetAll(6, v, -150.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	SetAll(7, v, -150.0f, 100.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	SetAll(8, v, -50.0f, 100.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	SetAll(9, v, -150.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	SetAll(10, v, -50.0f, 100.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	SetAll(11, v, -50.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Note: We leave gap in middle of walls for mirror

	SetAll(12, v, 50.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	SetAll(13, v, 50.0f, 100.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	SetAll(14, v, 150.0f, 100.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	SetAll(15, v, 50.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	SetAll(16, v, 150.0f, 100.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	SetAll(17, v, 150.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// mirror
	SetAll(18, v, -50.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	SetAll(19, v, -50.0f, 100.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	SetAll(20, v, 50.0f, 100.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	SetAll(21, v, -50.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	SetAll(22, v, 50.0f, 100.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	SetAll(23, v, 50.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	VB->Unlock();

	//
	// Load Textures, set filters.
	//

	D3DXCreateTextureFromFile(Device, "data\\TEXTURE\\checker.jpg", &FloorTex);
	D3DXCreateTextureFromFile(Device, "data\\TEXTURE\\brick0.jpg", &WallTex);
	D3DXCreateTextureFromFile(Device, "data\\TEXTURE\\ice.bmp", &MirrorTex);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);
}

//=========================================
// 終了処理
//=========================================
void UninitPolygon(void)
{
	// 頂点バッファーの解放
	if (VB != NULL)
	{
		VB->Release();
		VB = NULL;
	}

	// テクスチャの解放
	if (FloorTex != NULL)
	{
		FloorTex->Release();
		FloorTex = NULL;
	}
	if (WallTex != NULL)
	{
		WallTex->Release();
		WallTex = NULL;
	}
	if (MirrorTex != NULL)
	{
		MirrorTex->Release();
		MirrorTex = NULL;
	}
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

	Device->SetStreamSource(0, VB, 0, sizeof(VERTEX_3D));
	Device->SetFVF(FVF_VERTEX_3D);

	// draw the floor
	Device->SetMaterial(&FloorMtrl);
	Device->SetTexture(0, FloorTex);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	// draw the walls
	Device->SetMaterial(&WallMtrl);
	Device->SetTexture(0, WallTex);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 6, 4);

	// draw the mirror
	Device->SetMaterial(&MirrorMtrl);
	Device->SetTexture(0, MirrorTex);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
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
