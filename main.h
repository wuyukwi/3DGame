/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _MAIN_H_		// このマクロ定義がされてなかったら
#define _MAIN_H_		// ２重インクルード防止のマクロ定義

#include <stdint.h>
#include <stdio.h>
#include <windows.h>
#include "d3dx9.h"									// 描画処理に必要
#define  DIRECTINPUT_VERSION		(0x0800)		// ビルド時の警戒対処用マクロ
#include "dinput.h"									// 入力処理に必要
#include "xinput.h"									// ジョイパッド処理に必要
#include "xaudio2.h"								// サウンド処理に必要

// --------------------------------------------------
// ライブラリのリンク
// --------------------------------------------------
#pragma comment(lib, "d3d9.lib")		// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")		// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")		// DirectXコンポーネント(部品)使用に必要
#pragma comment(lib, "winmm.lib")		// システム時刻取得に必要
#pragma comment(lib, "dinput8.lib")		// 入力処理に必要
#pragma comment(lib, "xinput.lib")		// ジョイパッド処理に必要

// --------------------------------------------------
// マクロ定義
// --------------------------------------------------
#define SCREEN_WIDTH		(1280)												// ウインドウの幅
#define SCREEN_HEIGHT		(720)												// ウインドウの高さ
#define FVF_VERTEX_PT		(D3DFVF_XYZ|D3DFVF_TEX1)		// 頂点フォーマット
#define FVF_VERTEX_PNT		(D3DFVF_XYZ | D3DFVF_NORMAL |D3DFVF_TEX1)		// 頂点フォーマット
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE |D3DFVF_TEX1)		// 頂点フォーマット[3D]

typedef enum
{
	MODE_TITLE = 0,	// タイトル画面
	MODE_GAME,		// ゲーム画面
	MODE_RESULT,	// リゾート画面
	MODE_TUTORIAL,	// 
	MODE_MAX
}MODE;

// 頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3		pos;		// 頂点座標
	D3DXVECTOR3		nor;		// 法線ベクトル
	D3DCOLOR		col;		// 頂点カラー
	D3DXVECTOR2		tex;		// 頂点座標
}VERTEX_3D;
// 頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3		pos;		// 頂点座標
	D3DXVECTOR2		tex;		// 頂点座標
}VERTEX_PT;

typedef struct
{
	D3DXVECTOR3		pos;		// 頂点座標
	D3DXVECTOR3		nor;		// 法線ベクトル
	D3DXVECTOR2		tex;		// 頂点座標
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
// プロトタイプ宣言
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