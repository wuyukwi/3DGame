/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _INPUT_H_		// このマクロ定義がされてなかったら
#define _INPUT_H_		// ２重インクルード防止のマクロ定義


#include "main.h"

// --------------------------------------------------
// キーの種類
// --------------------------------------------------
typedef enum
{
	JOYKEY_UP = 0,		// 十字キー(上)
	JOYKEY_DOWN,		// 十字キー(下)
	JOYKEY_LEFT,		// 十字キー(左)
	JOYKEY_RIGHT,		// 十字キー(右)
	JOYKEY_START,		// スタート
	JOYKEY_BACK,		// バック
	JOYKEY_L_PUSH,		// Lスティック押す
	JOYKEY_R_PUSH,		// Rスティック押す
	JOYKEY_L1,			// L1ボタン
	JOYKEY_R1,			// R1ボタン
	JOYKEY_NONE1,		// 反応なし
	JOYKEY_NONE2,		// 反応なし
	JOYKEY_A,			// A
	JOYKEY_B,			// B
	JOYKEY_X,			// X
	JOYKEY_Y,			// Y
	JOYKEY_L_STICK,		// Lスティック
	JOYKEY_R_STICK,		// Rスティック
	JOYKEY_MAX
}JOYKEY;

// --------------------------------------------------
// プロトタイプ宣言
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