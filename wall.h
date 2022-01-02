/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _Wall_H_
#define _Wall_H_

#include "main.h"

// �}�N����`


//�ǂ̍\����
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXCOLOR		col;			// �J���[
	float fWidth;				//�ǂ̕�
	float fHeight;				//�ǂ̍���
	bool			bUse;			// �g�p��
}Wall;


//�v���g�^�C�v�錾
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col);

#endif // !_Wall_H_
