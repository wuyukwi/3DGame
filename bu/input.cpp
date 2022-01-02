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


Input::Input(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&_pInput,
		NULL)))
	{
		::MessageBox(0, "DirectInputオブジェクトの生成 - FAILED", 0, 0);
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
	// XInputのステートを設定(無効にする)
	XInputEnable(false);

	_pKeyboardDevice->Unacquire();

	// DirectInputオブジェクトの破棄
	SAFE_RELEASE(_pInput);
}

// --------------------------------------------------
// キーボードの初期化処理
// --------------------------------------------------
HRESULT Input::InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// 入力デバイス(キーボード)の生成
	if (FAILED(_pInput->CreateDevice(
		GUID_SysKeyboard,
		&_pKeyboardDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(_pKeyboardDevice->SetDataFormat(
		&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(_pKeyboardDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードのアクセス権を獲得
	_pKeyboardDevice->Acquire();

	return S_OK;
}

// --------------------------------------------------
// ゲームパッドの初期化処理
// --------------------------------------------------
HRESULT Input::InitGamepad(HINSTANCE hInstance, HWND hWnd)
{
	// メモリのクリア
	memset(&_joyKeyState, 0, sizeof(XINPUT_STATE));

	// XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

// --------------------------------------------------
// マウスの初期化処理
// --------------------------------------------------
HRESULT Input::InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	// 入力デバイス(キーボード)の生成
	if (FAILED(_pInput->CreateDevice(
		GUID_SysMouse,
		&_pMouseDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(_pKeyboardDevice->SetDataFormat(
		&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(_pKeyboardDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードのアクセス権を獲得
	_pKeyboardDevice->Acquire();

	return S_OK;
}

// --------------------------------------------------
// 更新処理
// --------------------------------------------------

void Input::Updata()
{
	UpdataKeyboard();

	UpdataGamepad();
}

// --------------------------------------------------
// キーボードの更新処理
// --------------------------------------------------
void Input::UpdataKeyboard(void)
{
	int nCntKey;
	BYTE aKeyState[NUM_KEY_MAX];		// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(_pKeyboardDevice->GetDeviceState(
		sizeof(aKeyState),
		&aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			_aKeyStateTrigger[nCntKey] = ~_aKeyState[nCntKey] & aKeyState[nCntKey];		// キーボードのトリガー情報を保存
			_aKeyStateRelease[nCntKey] = _aKeyState[nCntKey] & ~aKeyState[nCntKey];		// キーボードのリリース情報を保存

			// まだできてない
			_aKeyStateRepeat[nCntKey] = ~_aKeyState[nCntKey] & ~aKeyState[nCntKey];		// キーボードのリピート情報を保存
			
			_aKeyState[nCntKey] = aKeyState[nCntKey];										// キーボードのプレス情報を保存
		}
	}
	else
	{
		_pKeyboardDevice->Acquire();		// キーボードのアクセス権を獲得
	}


}

// --------------------------------------------------
// ジョイパッドの更新処理
// --------------------------------------------------
void Input::UpdataGamepad(void)
{
	XINPUT_STATE joykeyState;		// ジョイパッドの入力情報

	// ジョイパッドの状態を取得	
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{// 0の部分が操作しているのが何個目のパッドかを表してる
		// ジョイパッドのトリガー情報を保存
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

		// ジョイパッドのプレス情報を保存
		_joyKeyState = joykeyState;
	}
}

// --------------------------------------------------
// キーボードのプレス情報を取得
// --------------------------------------------------
bool Input::GetKeyboardPress(int nKey)
{
	return (_aKeyState[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// ジョイパッドのプレス情報を取得
// --------------------------------------------------
bool Input::GetJoypadPress(JOYKEY key)
{
	return (_joyKeyState.Gamepad.wButtons & 0x01 << key) ? true : false;
}

// --------------------------------------------------
// キーボードのトリガー情報を取得
// --------------------------------------------------
bool Input::GetKeyboardTrigger(int nKey)
{
	return (_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// ジョイパッドのトリガー情報を取得
// --------------------------------------------------
bool Input::GetJoypadTrigger(JOYKEY key)
{
	return (_joyKeyStateTrigger.Gamepad.wButtons & 0x01 << key) ? true : false;
}

// --------------------------------------------------
// キーボードのリリース情報を取得
// --------------------------------------------------
bool Input::GetKeyboardRelease(int nKey)
{
	return (_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// キーボードのリピート情報を取得
// --------------------------------------------------
bool Input::GetKeyboardRepeat(int nKey)
{
	return (_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

// --------------------------------------------------
// ジョイパッドのスティック情報を取得
// --------------------------------------------------
D3DXVECTOR3 Input::GetGamepadStick(JOYKEY key)
{
	switch (key)
	{
	case JOYKEY_L_STICK:		// Lスティック
		_joyKeyStick = D3DXVECTOR3(_joyKeyState.Gamepad.sThumbLX / 60000.0f, -_joyKeyState.Gamepad.sThumbLY / 60000.0f, 0.0f);
		break;
	case JOYKEY_R_STICK:		// Rスティック
		_joyKeyStick = D3DXVECTOR3(_joyKeyState.Gamepad.sThumbRX / 30000.0f, -_joyKeyState.Gamepad.sThumbRY / 30000.0f, 0.0f);
		break;
	}

	return _joyKeyStick;
}