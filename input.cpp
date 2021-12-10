/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#include "input.h"

// --------------------------------------------------
// �}�N����`
// --------------------------------------------------
#define NUM_KEY_MAX		(256)		// �L�[�̍ő吔

// --------------------------------------------------
// �X�^�e�B�b�N�ϐ�
// --------------------------------------------------
static LPDIRECTINPUT8			s_pInput = NULL;						// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
static LPDIRECTINPUTDEVICE8		s_pDevKeyboard = NULL;					// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
static BYTE						s_aKeyState[NUM_KEY_MAX];				// �L�[�{�[�h�̃v���X���
static BYTE						s_aKeyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̃g���K�[���
static BYTE						s_aKeyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̃����[�X���
static BYTE						s_aKeyStateRepeat[NUM_KEY_MAX];			// �L�[�{�[�h�̃��s�[�g���
static XINPUT_STATE				s_joyKeyState;							// �W���C�p�b�h�̃v���X���
static XINPUT_STATE				s_joyKeyStateTrigger;					// �W���C�p�b�h�̃g���K�[���
static D3DXVECTOR3				s_joyKeyStick;							// �W���C�p�b�h�̃X�e�B�b�N
int Key[256];
// --------------------------------------------------
// �L�[�{�[�h�̏���������
// --------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&s_pInput,
		NULL)))
	{
		return E_FAIL;
	}

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(s_pInput->CreateDevice(
		GUID_SysKeyboard,
		&s_pDevKeyboard,
		NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(s_pDevKeyboard->SetDataFormat(
		&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(s_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�̃A�N�Z�X�����l��
	s_pDevKeyboard->Acquire();

	return S_OK;
}

// --------------------------------------------------
// �W���C�p�b�h�̏���������
// --------------------------------------------------
HRESULT InitJoypad(void)
{
	// �������̃N���A
	memset(&s_joyKeyState, 0, sizeof(XINPUT_STATE));

	// XInput�̃X�e�[�g��ݒ�(�L���ɂ���)
	XInputEnable(true);

	return S_OK;
}

// --------------------------------------------------
// �L�[�{�[�h�̏I������
// --------------------------------------------------
void UninitKeyboard(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (s_pDevKeyboard != NULL)
	{
		s_pDevKeyboard->Unacquire();
		s_pDevKeyboard->Release();
		s_pDevKeyboard = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
	if (s_pInput != NULL)
	{
		s_pInput->Release();
		s_pInput = NULL;
	}
}

// --------------------------------------------------
// �W���C�p�b�h�̏I������
// --------------------------------------------------
void UninitJoypad(void)
{
	// XInput�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);
}

// --------------------------------------------------
// �L�[�{�[�h�̍X�V����
// --------------------------------------------------
void UpdateKeyboard(void)
{
	int nCntKey;
	BYTE aKeyState[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(s_pDevKeyboard->GetDeviceState(
		sizeof(aKeyState),
		&aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			s_aKeyStateTrigger[nCntKey] = ~s_aKeyState[nCntKey] & aKeyState[nCntKey];		// �L�[�{�[�h�̃g���K�[����ۑ�
			s_aKeyStateRelease[nCntKey] = s_aKeyState[nCntKey] & ~aKeyState[nCntKey];		// �L�[�{�[�h�̃����[�X����ۑ�

			// �܂��ł��ĂȂ�
			s_aKeyStateRepeat[nCntKey] = ~s_aKeyState[nCntKey] & ~aKeyState[nCntKey];		// �L�[�{�[�h�̃��s�[�g����ۑ�
			
			s_aKeyState[nCntKey] = aKeyState[nCntKey];										// �L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		s_pDevKeyboard->Acquire();		// �L�[�{�[�h�̃A�N�Z�X�����l��
	}


}

// --------------------------------------------------
// �W���C�p�b�h�̍X�V����
// --------------------------------------------------
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;		// �W���C�p�b�h�̓��͏��

	// �W���C�p�b�h�̏�Ԃ��擾	
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{// 0�̕��������삵�Ă���̂����ڂ̃p�b�h����\���Ă�
		// �W���C�p�b�h�̃g���K�[����ۑ�
		s_joyKeyStateTrigger.Gamepad.wButtons = ~s_joyKeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons;

		if (s_joyKeyState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			s_joyKeyState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			s_joyKeyState.Gamepad.sThumbLX = 0;
		}

		if (s_joyKeyState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			s_joyKeyState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			s_joyKeyState.Gamepad.sThumbLY = 0;
		}

		// �W���C�p�b�h�̃v���X����ۑ�
		s_joyKeyState = joykeyState;
	}
}

// --------------------------------------------------
// �L�[�{�[�h�̃v���X�����擾
// --------------------------------------------------
bool GetKeyboardPress(int nKey)
{
	return (s_aKeyState[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// �W���C�p�b�h�̃v���X�����擾
// --------------------------------------------------
bool GetJoypadPress(JOYKEY key)
{
	return (s_joyKeyState.Gamepad.wButtons & 0x01 << key) ? true : false;
}

// --------------------------------------------------
// �L�[�{�[�h�̃g���K�[�����擾
// --------------------------------------------------
bool GetKeyboardTrigger(int nKey)
{
	return (s_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// �W���C�p�b�h�̃g���K�[�����擾
// --------------------------------------------------
bool GetJoypadTrigger(JOYKEY key)
{
	return (s_joyKeyStateTrigger.Gamepad.wButtons & 0x01 << key) ? true : false;
}

// --------------------------------------------------
// �L�[�{�[�h�̃����[�X�����擾
// --------------------------------------------------
bool GetKeyboardRelease(int nKey)
{
	return (s_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// �L�[�{�[�h�̃��s�[�g�����擾
// --------------------------------------------------
bool GetKeyboardRepeat(int nKey)
{
	return (s_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// �W���C�p�b�h�̃X�e�B�b�N�����擾
// --------------------------------------------------
D3DXVECTOR3 GetJoypadStick(JOYKEY key)
{
	switch (key)
	{
	case JOYKEY_L_STICK:		// L�X�e�B�b�N
		s_joyKeyStick = D3DXVECTOR3(s_joyKeyState.Gamepad.sThumbLX / 60000.0f, -s_joyKeyState.Gamepad.sThumbLY / 60000.0f, 0.0f);
		break;
	case JOYKEY_R_STICK:		// R�X�e�B�b�N
		s_joyKeyStick = D3DXVECTOR3(s_joyKeyState.Gamepad.sThumbRX / 30000.0f, -s_joyKeyState.Gamepad.sThumbRY / 30000.0f, 0.0f);
		break;
	}

	return s_joyKeyStick;
}