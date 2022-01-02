/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _INPUT_H_		// ���̃}�N����`������ĂȂ�������
#define _INPUT_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`


#include "main.h"

// --------------------------------------------------
// �L�[�̎��
// --------------------------------------------------
typedef enum
{
	JOYKEY_UP = 0,		// �\���L�[(��)
	JOYKEY_DOWN,		// �\���L�[(��)
	JOYKEY_LEFT,		// �\���L�[(��)
	JOYKEY_RIGHT,		// �\���L�[(�E)
	JOYKEY_START,		// �X�^�[�g
	JOYKEY_BACK,		// �o�b�N
	JOYKEY_L_PUSH,		// L�X�e�B�b�N����
	JOYKEY_R_PUSH,		// R�X�e�B�b�N����
	JOYKEY_L1,			// L1�{�^��
	JOYKEY_R1,			// R1�{�^��
	JOYKEY_NONE1,		// �����Ȃ�
	JOYKEY_NONE2,		// �����Ȃ�
	JOYKEY_A,			// A
	JOYKEY_B,			// B
	JOYKEY_X,			// X
	JOYKEY_Y,			// Y
	JOYKEY_L_STICK,		// L�X�e�B�b�N
	JOYKEY_R_STICK,		// R�X�e�B�b�N
	JOYKEY_MAX
}JOYKEY;

// --------------------------------------------------
// �v���g�^�C�v�錾
// --------------------------------------------------
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdataInput(void);
bool GetKeyboardPress(int nKey);
bool GetJoypadPress(JOYKEY key);
bool GetKeyboardTrigger(int nKey);
bool GetJoypadTrigger(JOYKEY key);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);

D3DXVECTOR3 GetJoypadStick(JOYKEY key);
DIMOUSESTATE *GetMouseState(void);
#endif //  !_INPUT_H_