/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#include "Wall.h"
#include "model.h"

#define MAX_WALL_VERTEX			(4)					// �ǂ̒��_��
#define MAX_WALL				(128)				// �ǂ̍ő吔
#define MAX_WALL_WIDH			(200.0f)			// �ǂ̕�
#define MAX_WALL_HEIGHT			(100.0f)			// �ǂ̍���

//�O���[�o���ϐ�	
static LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffWall = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9			g_pTextureWall = NULL;		// �e�N�X�`���ւ̃|�C���^
static Wall							g_aWall[MAX_WALL];			// �Ǐ��̎擾

//=========================================
// ����������
//=========================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\checker.jpg",
		&g_pTextureWall);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D*pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//�ǂ̈ʒu�̏�����
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//�ǂ̌����̏�����
		g_aWall[nCntWall].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			//�ǂ̃J���[�̏�����
		g_aWall[nCntWall].fWidth =200.0f;									//�ǂ̕��̏�����
		g_aWall[nCntWall].fHeight = 50.0f;									//�ǂ̍����̏�����
		g_aWall[nCntWall].bUse = false;										//�g�p����Ă��Ȃ���Ԃɂ���

																			//���_���W�̐ݒ�
		pVtx[(nCntWall * 4) + 0].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z + g_aWall[nCntWall].fHeight);
		pVtx[(nCntWall * 4) + 1].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z + g_aWall[nCntWall].fHeight);
		pVtx[(nCntWall * 4) + 2].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z - g_aWall[nCntWall].fHeight);
		pVtx[(nCntWall * 4) + 3].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z - g_aWall[nCntWall].fHeight);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[(nCntWall * 4) + 0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[(nCntWall * 4) + 1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[(nCntWall * 4) + 2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[(nCntWall * 4) + 3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[(nCntWall * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntWall * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntWall * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntWall * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[(nCntWall * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 1].tex = D3DXVECTOR2(1.0f + (g_aWall[nCntWall].fWidth / 10.0f), 0.0f);
		pVtx[(nCntWall * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f + (g_aWall[nCntWall].fHeight / 10.0f));
		pVtx[(nCntWall * 4) + 3].tex = D3DXVECTOR2(1.0f + (g_aWall[nCntWall].fWidth / 10.0f), 1.0f + (g_aWall[nCntWall].fHeight / 10.0f));

	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffWall->Unlock();
}

//=========================================
// �I������
//=========================================
void UninitWall(void)
{
	//���_�o�b�t�@�̔j��
	SAFE_RELEASE(g_pVtxBuffWall);

	//�e�N�X�`���ւ̃|�C���^�̔j��
	SAFE_RELEASE(g_pTextureWall);
}

//=========================================
// �X�V����
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
// �`�揈��
//=========================================
void DrawWall(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X

													// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureWall);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{// �ǂ��g�p����Ă���Ƃ�
		 // ���[���h�}�g���b�N�X�̏�����
		 // �s�񏉊����֐�(��������[�s��]��[�P�ʍs��]�ɏ�����)
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

			// �����̔��f
			// �s���]�֐� (��������[���[(y)�s�b�`(x)���[��(z)]�����̉�]�s����쐬)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);

			// �s��|���Z�֐� (������ * ��O������������Ɋi�[)
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			// �s��ړ��֐� (��������X,Y,Z�����̈ړ��s����쐬)
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);		// �s��|���Z�֐�

																										// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �Ǖ`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, MAX_WALL_VERTEX * nCntWall, 2);
		}
	}

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);
}



//---------------------------------------------------------------------------
// �ǐݒ菈��
//---------------------------------------------------------------------------
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	// �ϐ��錾
	int nCntWall;

	// ���_���̎擾
	VERTEX_3D *pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{
			// �Ǐ��̐ݒ�
			g_aWall[nCntWall].pos = pos;																		// �ʒu
			g_aWall[nCntWall].rot = D3DXVECTOR3((D3DX_PI * rot.x), (D3DX_PI * rot.y), (D3DX_PI * rot.z));		// ����
			g_aWall[nCntWall].mtxWorld = {};																	// ���[���h�}�g���b�N�X
			g_aWall[nCntWall].col = col;																		// �J���[
			g_aWall[nCntWall].bUse = true;																	// �g�p��

																											// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aWall[nCntWall].col;
			pVtx[1].col = g_aWall[nCntWall].col;
			pVtx[2].col = g_aWall[nCntWall].col;
			pVtx[3].col = g_aWall[nCntWall].col;

			break;
		}

		pVtx += 4;		//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffWall->Unlock();
}