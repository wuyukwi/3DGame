/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#include "input.h"

// --------------------------------------------------
// マクロ定義
// --------------------------------------------------
#define NUM_KEY_MAX		(256)		// キーの最大数
#define ITEMS_NUM		(10)

// 振動情報の構造体
typedef struct
{
	int			nTime;			// 持続時間
	int			nStrength;		// 強さ(0 - 65535)
}VIBRATION;

// --------------------------------------------------
// プロトタイプ宣言
// --------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd);
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);
void UpdataKeyboard(void);
void UpdataGamepad(void);
void UpdataMouse(void);

// --------------------------------------------------
// スタティック変数
// --------------------------------------------------
static LPDIRECTINPUT8			s_pInput = NULL;						// DirectInputオブジェクトへのポインタ
static LPDIRECTINPUTDEVICE8		s_pKeyboardDevice = NULL;				// 入力デバイス(キーボード)へのポインタ
static LPDIRECTINPUTDEVICE8		s_pMouseDevice = NULL;				// 入力デバイス(マウス)へのポインタ
DIMOUSESTATE					g_MouseState;


static BYTE						s_aKeyState[NUM_KEY_MAX];				// キーボードのプレス情報
static BYTE						s_aKeyStateTrigger[NUM_KEY_MAX];		// キーボードのトリガー情報
static BYTE						s_aKeyStateRelease[NUM_KEY_MAX];		// キーボードのリリース情報
static BYTE						s_aKeyStateRepeat[NUM_KEY_MAX];			// キーボードのリピート情報
static XINPUT_STATE				s_joyKeyState;							// ジョイパッドのプレス情報
static XINPUT_STATE				s_joyKeyStateTrigger;					// ジョイパッドのトリガー情報
static D3DXVECTOR3				s_joyKeyStick;							// ジョイパッドのスティック
XINPUT_VIBRATION				s_joyMoter;								// ジョイパットのモーター
VIBRATION						s_Vibration;							// 振動情報の取得
float							s_JoypadLStickAngle;					// スティックの角度
int Key[256];

// --------------------------------------------------
// キーボードの初期化処理
// --------------------------------------------------
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&s_pInput,
		NULL)))
	{
		::MessageBox(0, "DirectInputオブジェクトの生成 - FAILED", 0, 0);
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
// 入力の終了処理
//---------------------------------------------------------------------------
void UninitInput(void)
{
	//入力デバイスの破棄
	SAFE_RELEASE(s_pMouseDevice);
	SAFE_RELEASE(s_pKeyboardDevice);

	//DirectInputオブジェクトの破棄
	SAFE_RELEASE(s_pInput);

	//XInputのステートを設定(無効にする)
	XInputEnable(false);
}

// --------------------------------------------------
// キーボードの初期化処理
// --------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// 入力デバイス(キーボード)の生成
	if (FAILED(s_pInput->CreateDevice(
		GUID_SysKeyboard,
		&s_pKeyboardDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(s_pKeyboardDevice->SetDataFormat(
		&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(s_pKeyboardDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードのアクセス権を獲得
	s_pKeyboardDevice->Acquire();

	return S_OK;
}

// --------------------------------------------------
// ゲームパッドの初期化処理
// --------------------------------------------------
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd)
{
	// メモリのクリア
	memset(&s_joyKeyState, 0, sizeof(XINPUT_STATE));

	// XInputのステートを設定(有効にする)
	XInputEnable(true);

	// ジョイパッドの振動制御の0クリア
	ZeroMemory(&s_joyMoter, sizeof(XINPUT_VIBRATION));

	// 振動制御用の初期化
	s_Vibration.nTime = 0;
	s_Vibration.nStrength = 0;

	return S_OK;
}

// --------------------------------------------------
// マウスの初期化処理
// --------------------------------------------------
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	// 入力デバイス(キーボード)の生成
	if (FAILED(s_pInput->CreateDevice(
		GUID_SysMouse,
		&s_pMouseDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(s_pMouseDevice->SetDataFormat(
		&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
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
	
	// キーボードのアクセス権を獲得
	s_pMouseDevice->Acquire();

	s_pMouseDevice->Poll();

	return S_OK;
}

// --------------------------------------------------
// 更新処理
// --------------------------------------------------

void UpdataInput()
{
	UpdataKeyboard();

	UpdataGamepad();

	UpdataMouse();
}

// --------------------------------------------------
// キーボードの更新処理
// --------------------------------------------------
void UpdataKeyboard(void)
{
	int nCntKey;
	BYTE aKeyState[NUM_KEY_MAX];		// キーボードの入力情報

										// 入力デバイスからデータを取得
	if (SUCCEEDED(s_pKeyboardDevice->GetDeviceState(
		sizeof(aKeyState),
		&aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			s_aKeyStateTrigger[nCntKey] = ~s_aKeyState[nCntKey] & aKeyState[nCntKey];		// キーボードのトリガー情報を保存
			s_aKeyStateRelease[nCntKey] = s_aKeyState[nCntKey] & ~aKeyState[nCntKey];		// キーボードのリリース情報を保存

			s_aKeyState[nCntKey] = aKeyState[nCntKey];										// キーボードのプレス情報を保存
		}
	}
	else
	{
		s_pKeyboardDevice->Acquire();		// キーボードのアクセス権を獲得
	}


}

// --------------------------------------------------
// ジョイパッドの更新処理
// --------------------------------------------------
void UpdataGamepad(void)
{
	XINPUT_STATE joykeyState;	//ジョイパッドの入力

	//ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		s_joyKeyStateTrigger.Gamepad.wButtons = (s_joyKeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;	  //ジョイパッドのトリガー情報を保存
		s_joyKeyState = joykeyState;		//ジョイパッドのプレス情報を保存
	}

	s_joyMoter.wLeftMotorSpeed = s_Vibration.nStrength;
	s_joyMoter.wRightMotorSpeed = s_Vibration.nStrength;

	//振動させるコントローラーの数値
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
// マウスの更新処理
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
// キーボードのプレス情報を取得
// --------------------------------------------------
bool GetKeyboardPress(int nKey)
{
	return (s_aKeyState[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// ジョイパッドのプレス情報を取得
// --------------------------------------------------
bool GetJoypadPress(JOYKEY key)
{
	return (s_joyKeyState.Gamepad.wButtons & 0x01 << key) ? true : false;
}

// --------------------------------------------------
// キーボードのトリガー情報を取得
// --------------------------------------------------
bool GetKeyboardTrigger(int nKey)
{
	return (s_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// ジョイパッドのトリガー情報を取得
// --------------------------------------------------
bool GetJoypadTrigger(JOYKEY key)
{
	return (s_joyKeyStateTrigger.Gamepad.wButtons & 0x01 << key) ? true : false;
}

// --------------------------------------------------
// キーボードのリリース情報を取得
// --------------------------------------------------
bool GetKeyboardRelease(int nKey)
{
	return (s_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// キーボードのリピート情報を取得
// --------------------------------------------------
bool GetKeyboardRepeat(int nKey)
{
	return (s_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// ジョイパッドのスティック情報を取得
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

// ジョイパッドの振動制御
void JoypadVib(int nVibTime, int nVibStrength)
{
	s_Vibration.nTime = nVibTime;
	s_Vibration.nStrength = nVibStrength;
}

// ジョイキースティックの角度情報の取得処理
float GetJoyStickAngle(void)
{
	return s_JoypadLStickAngle;
}

DIMOUSESTATE *GetMouseState(void)
{
	return &g_MouseState;
}