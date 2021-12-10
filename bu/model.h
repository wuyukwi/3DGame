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
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]��
	D3DXVECTOR3 moveRot;		// �ړ���
	LPD3DXMESH MeshModel;
	D3DMATERIAL9		*MeshMaterials;	// ���b�V���̃}�e���A��
	LPDIRECT3DTEXTURE9	*MeshTextures ;	// ���b�V���̃e�N�X�`��
	int			shadow;
}MODEL;

void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
MODEL *GetModel(void);

#endif