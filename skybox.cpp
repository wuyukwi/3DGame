#include "main.h"
#include "Skybox.h"
#include "set.h"

//
// �}�N����`
//
#define MAX_SKYBOX_SIZE		(5000)		//��

//
// �X�^�e�B�b�N�ϐ�
//
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^

static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
static IDirect3DIndexBuffer9*		s_pIdxBuff = NULL;
//
// ������
//
void InitSkybox(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\skybox.jpg",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_PT) * 24,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_PT,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_PT *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fxsize=1.0f/4;
	float fysize=1.0f/3;


	// �O
	SetAllPT(0,pVtx,-MAX_SKYBOX_SIZE , -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE , fxsize*3, fysize * 2);
	SetAllPT(1, pVtx, -MAX_SKYBOX_SIZE , MAX_SKYBOX_SIZE , MAX_SKYBOX_SIZE , fxsize * 3, fysize);
	SetAllPT(2, pVtx, MAX_SKYBOX_SIZE , -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE , fxsize * 4, fysize * 2);
	SetAllPT(3, pVtx, MAX_SKYBOX_SIZE , MAX_SKYBOX_SIZE , MAX_SKYBOX_SIZE , fxsize * 4, fysize);

	// ���
	SetAllPT(4, pVtx, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize, fysize * 2);
	SetAllPT(5, pVtx, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize, fysize);
	SetAllPT(6, pVtx, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize * 2, fysize * 2);
	SetAllPT(7, pVtx, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize * 2, fysize);

	// ��
	SetAllPT(8, pVtx, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize*2, fysize * 2);
	SetAllPT(9, pVtx, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize * 2, fysize);
	SetAllPT(10, pVtx, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, fxsize * 3 , fysize * 2);
	SetAllPT(11, pVtx, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, fxsize * 3, fysize);

	// �E
	SetAllPT(12, pVtx, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, 0.0f, fysize * 2);
	SetAllPT(13, pVtx, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, 0.0f, fysize);
	SetAllPT(14, pVtx, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize , fysize * 2);
	SetAllPT(15, pVtx, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize , fysize);

	// ��
	SetAllPT(16, pVtx, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize, fysize);
	SetAllPT(17, pVtx, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, fxsize, 0.0f);
	SetAllPT(18, pVtx, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize * 2, fysize);
	SetAllPT(19, pVtx, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, fxsize * 2, 0.0f);

	// ��
	SetAllPT(20, pVtx, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize * 2, fysize * 2);
	SetAllPT(21, pVtx, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, fxsize * 2, fysize * 3);
	SetAllPT(22, pVtx, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, fxsize, fysize * 2);
	SetAllPT(23, pVtx, MAX_SKYBOX_SIZE, -MAX_SKYBOX_SIZE, MAX_SKYBOX_SIZE, fxsize, fysize * 3);

	// ���_�o�b�t�@���A�����b�N����
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
// �I��
//
void UninitSkybox(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pIdxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		s_pIdxBuff->Release();
		s_pIdxBuff = NULL;
	}

}

//
// �X�V
//
void UpdateSkybox(void)
{

}

//
// �`��
//
void DrawSkybox(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans,mtxWorld;		// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixRotationY(&mtxRot, -0.000005f*timeGetTime());   

	mtxWorld = mtxRot*mtxTrans;

	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1); 
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);   

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_PT));
	pDevice->SetIndices(s_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_PT);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < 6; i++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			i*4,							// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��
	}

	//pDevice->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST,
	//	0,
	//	0,
	//	24,
	//	0,
	//	12);

	//// �e�N�X�`���̉���
	//pDevice->SetTexture(0, NULL);
}

