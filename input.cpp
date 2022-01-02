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
#define ITEMS_NUM		(10)

// �U�����̍\����
typedef struct
{
	int			nTime;			// ��������
	int			nStrength;		// ����(0 - 65535)
}VIBRATION;

// --------------------------------------------------
// �v���g�^�C�v�錾
// --------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd);
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);
void UpdataKeyboard(void);
void UpdataGamepad(void);
void UpdataMouse(void);

// --------------------------------------------------
// �X�^�e�B�b�N�ϐ�
// --------------------------------------------------
static LPDIRECTINPUT8			s_pInput = NULL;						// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
static LPDIRECTINPUTDEVICE8		s_pKeyboardDevice = NULL;				// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
static LPDIRECTINPUTDEVICE8		s_pMouseDevice = NULL;				// ���̓f�o�C�X(�}�E�X)�ւ̃|�C���^
DIMOUSESTATE					g_MouseState;


static BYTE						s_aKeyState[NUM_KEY_MAX];				// �L�[�{�[�h�̃v���X���
static BYTE						s_aKeyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̃g���K�[���
static BYTE						s_aKeyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̃����[�X���
static BYTE						s_aKeyStateRepeat[NUM_KEY_MAX];			// �L�[�{�[�h�̃��s�[�g���
static XINPUT_STATE				s_joyKeyState;							// �W���C�p�b�h�̃v���X���
static XINPUT_STATE				s_joyKeyStateTrigger;					// �W���C�p�b�h�̃g���K�[���
static D3DXVECTOR3				s_joyKeyStick;							// �W���C�p�b�h�̃X�e�B�b�N
XINPUT_VIBRATION				s_joyMoter;								// �W���C�p�b�g�̃��[�^�[
VIBRATION						s_Vibration;							// �U�����̎擾
float							s_JoypadLStickAngle;					// �X�e�B�b�N�̊p�x
int Key[256];

// --------------------------------------------------
// �L�[�{�[�h�̏���������
// --------------------------------------------------
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&s_pInput,
		NULL)))
	{
		::MessageBox(0, "DirectInput�I�u�W�F�N�g�̐��� - FAILED", 0, 0);
		return E_FAIL;
	}

	if (InitKeyboard(hInstance, hWnd))
	{
		::MessageBox(0, "InitKeyboard() - FAILED", 0, 0);
		return E_FAIL;
	}

	if (InitGamepad(hInstance, hWnd))
	{
		::MessageBox(0, "InitGamepad() - FAILED", 0, 0);
		return E_FAIL;
	}

	if (InitMouse(hInstance, hWnd))	
	{
		::MessageBox(0, "InitMouse() - FAILED", 0, 0);
		return E_FAIL;
	}

	return S_OK;
}

//---------------------------------------------------------------------------
// ���͂̏I������
//---------------------------------------------------------------------------
void UninitInput(void)
{
	//���̓f�o�C�X�̔j��
	SAFE_RELEASE(s_pMouseDevice);
	SAFE_RELEASE(s_pKeyboardDevice);

	//DirectInput�I�u�W�F�N�g�̔j��
	SAFE_RELEASE(s_pInput);

	//XInput�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);
}

// --------------------------------------------------
// �L�[�{�[�h�̏���������
// --------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(s_pInput->CreateDevice(
		GUID_SysKeyboard,
		&s_pKeyboardDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(s_pKeyboardDevice->SetDataFormat(
		&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(s_pKeyboardDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�̃A�N�Z�X�����l��
	s_pKeyboardDevice->Acquire();

	return S_OK;
}

// --------------------------------------------------
// �Q�[���p�b�h�̏���������
// --------------------------------------------------
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd)
{
	// �������̃N���A
	memset(&s_joyKeyState, 0, sizeof(XINPUT_STATE));

	// XInput�̃X�e�[�g��ݒ�(�L���ɂ���)
	XInputEnable(true);

	// �W���C�p�b�h�̐U�������0�N���A
	ZeroMemory(&s_joyMoter, sizeof(XINPUT_VIBRATION));

	// �U������p�̏�����
	s_Vibration.nTime = 0;
	s_Vibration.nStrength = 0;

	return S_OK;
}

// --------------------------------------------------
// �}�E�X�̏���������
// --------------------------------------------------
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(s_pInput->CreateDevice(
		GUID_SysMouse,
		&s_pMouseDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(s_pMouseDevice->SetDataFormat(
		&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(s_pMouseDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_EXCLUSIVE))))
	{
		return E_FAIL;
	}

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = 10;				
	s_pMouseDevice->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
	
	// �L�[�{�[�h�̃A�N�Z�X�����l��
	s_pMouseDevice->Acquire();

	s_pMouseDevice->Poll();

	return S_OK;
}

// --------------------------------------------------
// �X�V����
// --------------------------------------------------

void UpdataInput()
{
	UpdataKeyboard();

	UpdataGamepad();

	UpdataMouse();
}

// --------------------------------------------------
// �L�[�{�[�h�̍X�V����
// --------------------------------------------------
void UpdataKeyboard(void)
{
	int nCntKey;
	BYTE aKeyState[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��

										// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(s_pKeyboardDevice->GetDeviceState(
		sizeof(aKeyState),
		&aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			s_aKeyStateTrigger[nCntKey] = ~s_aKeyState[nCntKey] & aKeyState[nCntKey];		// �L�[�{�[�h�̃g���K�[����ۑ�
			s_aKeyStateRelease[nCntKey] = s_aKeyState[nCntKey] & ~aKeyState[nCntKey];		// �L�[�{�[�h�̃����[�X����ۑ�

			s_aKeyState[nCntKey] = aKeyState[nCntKey];										// �L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		s_pKeyboardDevice->Acquire();		// �L�[�{�[�h�̃A�N�Z�X�����l��
	}


}

// --------------------------------------------------
// �W���C�p�b�h�̍X�V����
// --------------------------------------------------
void UpdataGamepad(void)
{
	XINPUT_STATE joykeyState;	//�W���C�p�b�h�̓���

	//�W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		s_joyKeyStateTrigger.Gamepad.wButtons = (s_joyKeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;	  //�W���C�p�b�h�̃g���K�[����ۑ�
		s_joyKeyState = joykeyState;		//�W���C�p�b�h�̃v���X����ۑ�
	}

	s_joyMoter.wLeftMotorSpeed = s_Vibration.nStrength;
	s_joyMoter.wRightMotorSpeed = s_Vibration.nStrength;

	//�U��������R���g���[���[�̐��l
	XInputSetState(0, &s_joyMoter);

	if (s_Vibration.nTime > 0)
	{
		s_Vibration.nTime--;
	}
	else
	{
		s_Vibration.nStrength = 0;
		s_Vibration.nTime = 0;
	}
}

// --------------------------------------------------
// �}�E�X�̍X�V����
// --------------------------------------------------
void UpdataMouse(void)
{
	ZeroMemory(&g_MouseState, sizeof(g_MouseState));

	if (SUCCEEDED(s_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_MouseState)))
	{

	}
	else
	{
		s_pMouseDevice->Acquire();
	}
	/*HRESULT hr;

	DWORD dwReadnum;

	DIDEVICEOBJECTDATA	diDeviceObjectData[ITEMS_NUM];

	short x = 0, y = 0;

	for (int i = 0; i < ITEMS_NUM; i++)
	{
		hr = s_pMouseDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &diDeviceObjectData[i], &dwReadnum, 0);

		if (hr == DIERR_INPUTLOST)
		{
			s_pMouseDevice->Acquire();
			hr = s_pMouseDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &diDeviceObjectData[i], &dwReadnum, 0);

			if (FAILED(hr))
			{
				::MessageBox(0, "GetDeviceData()-FAILED", 0, 0);
			}
		}

		if ((diDeviceObjectData[i].dwOfs == DIMOFS_BUTTON0) && (diDeviceObjectData[i].dwData & 0x80))
		{
			if (diDeviceObjectData[i].dwOfs == DIMOFS_X)
			{
				x += (short)diDeviceObjectData->dwData;
			}
			if (diDeviceObjectData[i].dwOfs == DIMOFS_Y)
			{
				y += (short)diDeviceObjectData->dwData;
			}
		}
	}*/

	
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
	D3DXVECTOR3 stick;

	switch (key)
	{
	case JOYKEY_L_STICK:
		stick = D3DXVECTOR3(s_joyKeyState.Gamepad.sThumbLX / 30000.0f, -s_joyKeyState.Gamepad.sThumbLY / 30000.0f, 0.0f);

		s_JoypadLStickAngle = atan2f(s_joyKeyState.Gamepad.sThumbLX / 30000.0f, -s_joyKeyState.Gamepad.sThumbLY / 30000.0f);
		break;

	case JOYKEY_R_STICK:
		stick = D3DXVECTOR3(s_joyKeyState.Gamepad.sThumbRX / 30000.0f, -s_joyKeyState.Gamepad.sThumbRY / 30000.0f, 0.0f);
		break;
	}

	return stick;
}

// �W���C�p�b�h�̐U������
void JoypadVib(int nVibTime, int nVibStrength)
{
	s_Vibration.nTime = nVibTime;
	s_Vibration.nStrength = nVibStrength;
}

// �W���C�L�[�X�e�B�b�N�̊p�x���̎擾����
float GetJoyStickAngle(void)
{
	return s_JoypadLStickAngle;
}

DIMOUSESTATE *GetMouseState(void)
{
	return &g_MouseState;
}