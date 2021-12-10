/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _MAIN_H_		// ���̃}�N����`������ĂȂ�������
#define _MAIN_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

#include <stdint.h>
#include <stdio.h>
#include <windows.h>
#include "d3dx9.h"									// �`�揈���ɕK�v
#define  DIRECTINPUT_VERSION		(0x0800)		// �r���h���̌x���Ώ��p�}�N��
#include "dinput.h"									// ���͏����ɕK�v
#include "xinput.h"									// �W���C�p�b�h�����ɕK�v
#include "xaudio2.h"								// �T�E���h�����ɕK�v

// --------------------------------------------------
// ���C�u�����̃����N
// --------------------------------------------------
#pragma comment(lib, "d3d9.lib")		// �`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")		// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib, "dxguid.lib")		// DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib, "winmm.lib")		// �V�X�e�������擾�ɕK�v
#pragma comment(lib, "dinput8.lib")		// ���͏����ɕK�v
#pragma comment(lib, "xinput.lib")		// �W���C�p�b�h�����ɕK�v

// --------------------------------------------------
// �}�N����`
// --------------------------------------------------
#define SCREEN_WIDTH		(1280)												// �E�C���h�E�̕�
#define SCREEN_HEIGHT		(720)												// �E�C���h�E�̍���
#define FVF_VERTEX_PT		(D3DFVF_XYZ|D3DFVF_TEX1)		// ���_�t�H�[�}�b�g
#define FVF_VERTEX_PNT		(D3DFVF_XYZ | D3DFVF_NORMAL |D3DFVF_TEX1)		// ���_�t�H�[�}�b�g
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE |D3DFVF_TEX1)		// ���_�t�H�[�}�b�g[3D]

typedef enum
{
	MODE_TITLE = 0,	// �^�C�g�����
	MODE_GAME,		// �Q�[�����
	MODE_RESULT,	// ���]�[�g���
	MODE_TUTORIAL,	// 
	MODE_MAX
}MODE;

// ���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3		pos;		// ���_���W
	D3DXVECTOR3		nor;		// �@���x�N�g��
	D3DCOLOR		col;		// ���_�J���[
	D3DXVECTOR2		tex;		// ���_���W
}VERTEX_3D;
// ���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3		pos;		// ���_���W
	D3DXVECTOR2		tex;		// ���_���W
}VERTEX_PT;

typedef struct
{
	D3DXVECTOR3		pos;		// ���_���W
	D3DXVECTOR3		nor;		// �@���x�N�g��
	D3DXVECTOR2		tex;		// ���_���W
}VERTEX_PNT;

const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));
const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0, 255, 0));
const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0, 0, 255));
const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255, 255, 0));
const D3DXCOLOR CYAN(D3DCOLOR_XRGB(0, 255, 255));
const D3DXCOLOR MAGENTA(D3DCOLOR_XRGB(255, 0, 255));

// --------------------------------------------------
// �v���g�^�C�v�錾
// --------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void);
float* GetTime();
void SetMode(MODE mode);
MODE GetMode(void);


template<class T> void Release(T t)
{
	if (t)
	{
		t->Release();
		t = 0;
	}
}

template<class T> void Delete(T t)
{
	if (t)
	{
		delete t;
		t = 0;
	}
}
#endif //  !_MAIN_H_