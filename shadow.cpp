/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#include "polygon.h"
#include "model.h"
#include "camera.h"
void InitShadow(void)
{

}

//--------------------------------------------------
// I—¹
//--------------------------------------------------
void UninitShadow(void)
{

}

//--------------------------------------------------
// XV
//--------------------------------------------------
void UpdateShadow(void)
{

}

//--------------------------------------------------
// •`‰æ
//--------------------------------------------------
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	Device->SetRenderState(D3DRS_STENCILENABLE,		true);
	Device->SetRenderState(D3DRS_STENCILFUNC,		D3DCMP_EQUAL);
	Device->SetRenderState(D3DRS_STENCILREF,		0x1);
	Device->SetRenderState(D3DRS_STENCILMASK,		0xffffffff);
	Device->SetRenderState(D3DRS_STENCILWRITEMASK,	0xffffffff);
	Device->SetRenderState(D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP);
	Device->SetRenderState(D3DRS_STENCILFAIL,		D3DSTENCILOP_KEEP);
	Device->SetRenderState(D3DRS_STENCILPASS,		D3DSTENCILOP_INCR);

	// position shadow
	D3DXVECTOR4 lightDirection(0.707f, -0.707f, 0.707f, 0.0f);
	D3DXPLANE groundPlane(0.0f, -1.0f, 0.0f, 0.0f);
	

	D3DXMATRIX S;
	D3DXMatrixShadow(
		&S,
		&lightDirection,
		&groundPlane);

	MODEL *pModel = GetModel();
	CAMERA *pCamera = GetCamera();

	D3DXMATRIX R,T,W;

	// Œü‚«‚ð”½‰f
	D3DXMatrixRotationY(&R, pCamera->rot.y+1.5f);

	D3DXMatrixTranslation(&T, pModel->pos.x, pModel->pos.y, pModel->pos.z);
	//D3DXMatrixMultiply(&W, &W, &T);

	W = R*T*S;


	Device->SetTransform(D3DTS_WORLD, &W);

	// alpha blend the shadow
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DMATERIAL9 mtrl = InitMtrl(BLACK, BLACK, BLACK, BLACK, 0.0f);
	mtrl.Diffuse.a = 0.5f; // 50% transparency.

	// Disable depth buffer so that z-fighting doesn't occur when we
	// render the shadow on top of the floor.
	Device->SetRenderState(D3DRS_ZENABLE, false);

	/*Device->SetMaterial(pModel->MeshMaterials);
	Device->SetTexture(0,*pModel->MeshTextures);
	pModel->MeshModel->DrawSubset(0);*/
	Device->SetMaterial(&mtrl);
	Device->SetTexture(0, 0);
	pModel->Mesh->DrawSubset(0);

	Device->SetRenderState(D3DRS_ZENABLE, true);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	Device->SetRenderState(D3DRS_STENCILENABLE, false);
}

