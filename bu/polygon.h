/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//------------------------------------
// �|���S���̍\���̂��`
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 rot;	// ��]���W
	D3DXMATRIX mtxWorld;// ���[���h�}�g���b�N�X
}POLYGON;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitPolygon(void);		// �|���S���̏���������
void UninitPolygon(void);	// �|���S���̏I������
void UpdatePolygon(void);	// �|���S���̍X�V����
void DrawPolygon(void);		// �|���S���̕`�揈��
POLYGON *GetPolygon(void);	//�@�|���S�����擾

#endif // !_POLYGON_H_
