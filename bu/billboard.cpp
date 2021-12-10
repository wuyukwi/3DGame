#include "billboard.h"
#include "main.h"
#include "set.h"
#include "camera.h"

//
// �\���̂��`
//

//------------���_�t�H�[�}�b�g
#define FVF_BILLBOARDTEX (D3DFVF_XYZ|D3DFVF_TEX1)


//------------���_�\����
struct VertexBillboard
{
	float _x, _y, _z;//�ʒu���W
	float _u, _v; //�e�N�X�`�����W


	VertexBillboard(float x, float y, float z, float u, float v) : _x(x), _y(y), _z(z), _u(u), _v(v) {}
	VertexBillboard() {}
};


//
// �X�^�e�B�b�N�ϐ�
//
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTexture;					// �e�N�X�`���ւ̃|�C���^

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitBillboard(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VertexBillboard vertex[4];

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VertexBillboard) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);


	VertexBillboard *pVtx = NULL;

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���
	vertex[0] = VertexBillboard(-100.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertex[1] = VertexBillboard(-100.0f, 700.0f, 0.0f, 0.0f, 0.0f);
	vertex[2] = VertexBillboard(100.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	vertex[3] = VertexBillboard(100.0f, 700.0f, 0.0f, 1.0f, 0.0f);


	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXT("data//TEXTURE//miku.jpg"), &s_pTexture);
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitBillboard(void)
{
	// ���_�o�b�t�@�̔p��
	Release<LPDIRECT3DVERTEXBUFFER9>(s_pVtxBuff);
	
	//�@�e�N�X�`���̔p��
	Release<LPDIRECT3DTEXTURE9>(s_pTexture);
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateBillboard(void)
{
	
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawBillboard(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	D3DXMATRIX  mtxView;		

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3DXMATRIX mtxCharacter, mtxWorld;
	D3DXMatrixTranslation(&mtxWorld, 0.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&mtxCharacter, 0.0f, 0.0f, 0.0f);

	// �J�������̎擾
	CAMERA *pCamera = GetCamera();


	// �J�����̋t�s����v�Z
	D3DXMATRIX matBillboard;
	D3DXMatrixIdentity(&matBillboard);
	matBillboard._11 = pCamera->mtxView._11;
	matBillboard._13 = pCamera->mtxView._13;
	matBillboard._31 = pCamera->mtxView._31;
	matBillboard._33 = pCamera->mtxView._33;
	D3DXMatrixInverse(&matBillboard, NULL, &matBillboard);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxCharacter);
	mtxCharacter = matBillboard * mtxCharacter;


	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VertexBillboard));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_BILLBOARDTEX);

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,						// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��
	

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);

	// ���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fHeight, LPDIRECT3DTEXTURE9 *pTexture)
{
	
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void LoadBillboard(HWND hWnd)
{
	
}