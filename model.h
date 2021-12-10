/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _Model_H_		// ���̃}�N����`������ĂȂ�������
#define _Model_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

typedef struct
{
	D3DXVECTOR3			pos;		// �ʒu
	D3DXVECTOR3			rot;		// ��]��
	D3DXVECTOR3			rotDest;	// �ڕW��]��
	ID3DXMesh*			Mesh;		// ���b�V�����
	D3DMATERIAL9		*Materials;	// �}�e���A��
	LPDIRECT3DTEXTURE9	*Textures;	// �e�N�X�`��
	LPD3DXBUFFER		mtrlBuffer;		//�o�b�t�@
	LPD3DXBUFFER		adjBuffer;	//adj�o�b�t�@
	DWORD				numMtrls;		//���̐�
	D3DXMATRIX			mtxWorld;	// ���[���h�}�g���b�N�X
}MODEL;

void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
MODEL *GetModel(void);

#endif