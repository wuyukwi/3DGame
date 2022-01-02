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


Input::Input(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&_pInput,
		NULL)))
	{
		::MessageBox(0, "DirectInput�I�u�W�F�N�g�̐��� - FAILED", 0, 0);
	}

	if(InitKeyboard(hInstance, hWnd))
	::MessageBox(0, "InitKeyboard() - FAILED", 0, 0);

	if (InitGamepad(hInstance, hWnd))
		::MessageBox(0, "InitGamepad() - FAILED", 0, 0);

	if (InitMouse(hInstance, hWnd))
		::MessageBox(0, "InitMouse() - FAILED", 0, 0);
}

Input::~Input()
{
	// XInput�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);

	_pKeyboardDevice->Unacquire();

	// DirectInput�I�u�W�F�N�g�̔j��
	SAFE_RELEASE(_pInput);
}

// --------------------------------------------------
// �L�[�{�[�h�̏���������
// --------------------------------------------------
HRESULT Input::InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(_pInput->CreateDevice(
		GUID_SysKeyboard,
		&_pKeyboardDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(_pKeyboardDevice->SetDataFormat(
		&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(_pKeyboardDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�̃A�N�Z�X�����l��
	_pKeyboardDevice->Acquire();

	return S_OK;
}

// --------------------------------------------------
// �Q�[���p�b�h�̏���������
// --------------------------------------------------
HRESULT Input::InitGamepad(HINSTANCE hInstance, HWND hWnd)
{
	// �������̃N���A
	memset(&_joyKeyState, 0, sizeof(XINPUT_STATE));

	// XInput�̃X�e�[�g��ݒ�(�L���ɂ���)
	XInputEnable(true);

	return S_OK;
}

// --------------------------------------------------
// �}�E�X�̏���������
// --------------------------------------------------
HRESULT Input::InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(_pInput->CreateDevice(
		GUID_SysMouse,
		&_pMouseDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(_pKeyboardDevice->SetDataFormat(
		&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(_pKeyboardDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�̃A�N�Z�X�����l��
	_pKeyboardDevice->Acquire();

	return S_OK;
}

// --------------------------------------------------
// �X�V����
// --------------------------------------------------

void Input::Updata()
{
	UpdataKeyboard();

	UpdataGamepad();
}

// --------------------------------------------------
// �L�[�{�[�h�̍X�V����
// --------------------------------------------------
void Input::UpdataKeyboard(void)
{
	int nCntKey;
	BYTE aKeyState[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(_pKeyboardDevice->GetDeviceState(
		sizeof(aKeyState),
		&aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			_aKeyStateTrigger[nCntKey] = ~_aKeyState[nCntKey] & aKeyState[nCntKey];		// �L�[�{�[�h�̃g���K�[����ۑ�
			_aKeyStateRelease[nCntKey] = _aKeyState[nCntKey] & ~aKeyState[nCntKey];		// �L�[�{�[�h�̃����[�X����ۑ�

			// �܂��ł��ĂȂ�
			_aKeyStateRepeat[nCntKey] = ~_aKeyState[nCntKey] & ~aKeyState[nCntKey];		// �L�[�{�[�h�̃��s�[�g����ۑ�
			
			_aKeyState[nCntKey] = aKeyState[nCntKey];										// �L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		_pKeyboardDevice->Acquire();		// �L�[�{�[�h�̃A�N�Z�X�����l��
	}


}

// --------------------------------------------------
// �W���C�p�b�h�̍X�V����
// --------------------------------------------------
void Input::UpdataGamepad(void)
{
	XINPUT_STATE joykeyState;		// �W���C�p�b�h�̓��͏��

	// �W���C�p�b�h�̏�Ԃ��擾	
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{// 0�̕��������삵�Ă���̂����ڂ̃p�b�h����\���Ă�
		// �W���C�p�b�h�̃g���K�[����ۑ�
		_joyKeyStateTrigger.Gamepad.wButtons = ~_joyKeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons;

		if (_joyKeyState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			_joyKeyState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			_joyKeyState.Gamepad.sThumbLX = 0;
		}

		if (_joyKeyState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			_joyKeyState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			_joyKeyState.Gamepad.sThumbLY = 0;
		}

		// �W���C�p�b�h�̃v���X����ۑ�
		_joyKeyState = joykeyState;
	}
}

// --------------------------------------------------
// �L�[�{�[�h�̃v���X�����擾
// --------------------------------------------------
bool Input::GetKeyboardPress(int nKey)
{
	return (_aKeyState[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// �W���C�p�b�h�̃v���X�����擾
// --------------------------------------------------
bool Input::GetJoypadPress(JOYKEY key)
{
	return (_joyKeyState.Gamepad.wButtons & 0x01 << key) ? true : false;
}

// --------------------------------------------------
// �L�[�{�[�h�̃g���K�[�����擾
// --------------------------------------------------
bool Input::GetKeyboardTrigger(int nKey)
{
	return (_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// �W���C�p�b�h�̃g���K�[�����擾
// --------------------------------------------------
bool Input::GetJoypadTrigger(JOYKEY key)
{
	return (_joyKeyStateTrigger.Gamepad.wButtons & 0x01 << key) ? true : false;
}

// --------------------------------------------------
// �L�[�{�[�h�̃����[�X�����擾
// --------------------------------------------------
bool Input::GetKeyboardRelease(int nKey)
{
	return (_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// �L�[�{�[�h�̃��s�[�g�����擾
// --------------------------------------------------
bool Input::GetKeyboardRepeat(int nKey)
{
	return (_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// �W���C�p�b�h�̃X�e�B�b�N�����擾
// --------------------------------------------------
D3DXVECTOR3 Input::GetGamepadStick(JOYKEY key)
{
	switch (key)
	{
	case JOYKEY_L_STICK:		// L�X�e�B�b�N
		_joyKeyStick = D3DXVECTOR3(_joyKeyState.Gamepad.sThumbLX / 60000.0f, -_joyKeyState.Gamepad.sThumbLY / 60000.0f, 0.0f);
		break;
	case JOYKEY_R_STICK:		// R�X�e�B�b�N
		_joyKeyStick = D3DXVECTOR3(_joyKeyState.Gamepad.sThumbRX / 30000.0f, -_joyKeyState.Gamepad.sThumbRY / 30000.0f, 0.0f);
		break;
	}

	return _joyKeyStick;
}