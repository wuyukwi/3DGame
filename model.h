/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _Model_H_		// ���̃}�N����`������ĂȂ�������
#define _Model_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"
#include "set.h"

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
	bool				bModelInteraction;//�����蔻��
	ID3DXMesh* SphereMesh;
	ID3DXMesh* BoxMesh;

	BoundingBox boundingBox;
	BoundingSphere boundingSphere;

	bool RenderBoundingSphere = true;
}MODEL;

void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
MODEL *GetModel(void);
void ModelInteraction(D3DXVECTOR3*pPos, D3DXVECTOR3*pLastPos, D3DXVECTOR3*pMove, float width, float height, float lenght);

#endif