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

// �O���[�o���ϐ�
LPD3DXMESH			g_pMeshModel = NULL;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pBuffMatMoldel = NULL;	// �}�e���A�����ւ̃|�C���^
LPD3DXBUFFER		g_pBuffadj = NULL;			// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatModel = 0;			// �}�e���A�����̐�
D3DXMATRIX			g_mtxWorldModel;			// ���[���h�}�g���b�N�X
D3DMATERIAL9		*g_pMeshMaterials = NULL;	// ���b�V���̃}�e���A��
LPDIRECT3DTEXTURE9	*g_pMeshTextures = NULL;	// ���b�V���̃e�N�X�`��
D3DXMATERIAL		*pMat;						// �}�e���A���f�[�^�ւ̃|�C���^
MODEL				g_aModel;					// ���f���\���̂̐錾
D3DXVECTOR3			g_rotDestModel;

// ���f���̏���������
void InitModel(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(FILE_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		&g_pBuffadj,
		&g_pBuffMatMoldel,
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel);


	// ����������
	g_aModel.pos = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
	g_aModel.rot = D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	g_aModel.moveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotDestModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
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

// ���f���̏I������
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

	// ���b�V���̔p��
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	// �}�e���A���̔p��
	if (g_pBuffMatMoldel != NULL)
	{
		g_pBuffMatMoldel->Release();
		g_pBuffMatMoldel = NULL;
	}
}

// ���f���̍X�V����
void UpdateModel(void)
{
	float DownSpeed = 0.1f;	//�X�s�[�h�����W��

	MoveModel();
	
	float fAngle = g_aModel.moveRot.y - g_aModel.rot.y;

	// �p�x�̐��K��
	normalized(&fAngle);

	//�����E�������X�V (����������)
	g_aModel.rot.y += fAngle * DownSpeed;

	// �p�x�̐��K��
	normalized(&g_aModel.rot.y);

}


// ���f���̕`�揈��
void DrawModel(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldModel);		//  �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel.rot.y, g_aModel.rot.x, g_aModel.rot.z);		//  �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);								//  �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
																									
	D3DXMatrixTranslation(&mtxTrans, g_aModel.pos.x, g_aModel.pos.y, g_aModel.pos.z);				//  �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);								//  �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// ��]
	// D3DXMatrixRotationY(&g_mtxWorldModel, timeGetTime() / 1000.0f);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);
	

	//  ���f���̐ݒ�
	for (DWORD i = 0; i < g_nNumMatModel; i++)
	{
		//  �}�e���A���̐ݒ�
		pDevice->SetMaterial(&g_pMeshMaterials[i]);
		pDevice->SetTexture(0, g_pMeshTextures[i]);

		// ���f���p�[�c�̕`��
		g_pMeshModel->DrawSubset(i);


	}


}

MODEL *GetModel(void)
{
	return &g_aModel;
}

static void MoveModel()
{
	CAMERA *pCamera = GetCamera();		//�J�����̎擾
	float fRot = 0.0f;
	float fMoveSpeed = 2.0f;	//�X�s�[�h

	// ���f���̈ړ�
	if (GetKeyboardPress(DIK_LEFT))
	{// ���L�[�������ꂽ
		if (GetKeyboardPress(DIK_UP))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y + (D3DX_PI * 0.75f);

			g_aModel.moveRot.y = pCamera->rot.y + (-D3DX_PI * 0.25f);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ���L�[�������ꂽ
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
	{// ���L�[�������ꂽ
		if (GetKeyboardPress(DIK_UP))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y + (-D3DX_PI * 0.75f);

			g_aModel.moveRot.y = pCamera->rot.y + (D3DX_PI * 0.25f);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ���L�[�������ꂽ
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
	{// ���L�[�������ꂽ
		fRot = pCamera->rot.y + D3DX_PI;

		g_aModel.moveRot.y = pCamera->rot.y;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{// ���L�[�������ꂽ
		fRot = pCamera->rot.y;

		g_aModel.moveRot.y = pCamera->rot.y + D3DX_PI;
	}

	if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
		GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
	{// ��, ��, ��, ���L�[�������ꂽ
		g_aModel.pos.x += sinf(fRot) * fMoveSpeed;
		g_aModel.pos.z += cosf(fRot) * fMoveSpeed;
	}

	// ���f���̉�]
	if (GetKeyboardPress(DIK_NUMPAD2))
	{
		g_aModel.moveRot.y += 0.1f;
	}
	else if (GetKeyboardPress(DIK_NUMPAD3))
	{
		g_aModel.moveRot.y -= 0.1f;
	}

	// ���f���̏㉺�ړ�
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
