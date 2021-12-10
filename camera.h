/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _Camera_H_		// ���̃}�N����`������ĂȂ�������
#define _Camera_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

typedef struct
{
	D3DXVECTOR3 posV;		// ���_
	D3DXVECTOR3 posR;		// �����_
	D3DXVECTOR3 vecU;		// ������x�N�g��
	D3DXVECTOR3 moveV;		// ���_�̈ړ���
	D3DXVECTOR3 moveR;		// �����_�̈ړ���
	D3DXVECTOR3 rot;
	float fDistance;
	float fAngle;
	D3DXMATRIX mtxProjection;// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;		// �r���[�}�g���b�N�X
}CAMERA;

// �v���g�^�C�v�錾
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
CAMERA *GetCamera(void);

#endif