/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

void InitPolygon(void);		// �|���S���̏���������
void UninitPolygon(void);	// �|���S���̏I������
void UpdatePolygon(void);	// �|���S���̍X�V����
void DrawPolygon(void);		// �|���S���̕`�揈��

// �}�e���A���̏���������
D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

// �}�e���A���̒�`
const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 2.0f);
const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

#endif // !_POLYGON_H_
