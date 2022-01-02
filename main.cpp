/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#include "main.h"
#include "input.h"
#include "light.h"
#include "polygon.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "text.h"
#include "billboard.h"
#include "skybox.h"
#include "wall.h"

// --------------------------------------------------
// マクロ定義
// --------------------------------------------------
#define CLASS_NAME		"windowClass"		// ウインドウクラスの名前
#define WINDOW_NAME		"polygon"			// ウインドウの名前 (キャプションに表示)
#define ID_TIMER				(101)				// タイマーのID
#define TIMER_INTERVAL		(1000 / 60)				// タイマーの発生間隔 ( ミリ秒 )

// --------------------------------------------------
// プロトタイプ宣言
// --------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);

// --------------------------------------------------
// グローバル変数
// --------------------------------------------------
LPDIRECT3D9				g_pD3D = NULL;				// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;		// Direct3Dデバイスへのポインタ
LPD3DXFONT				g_pFont = NULL;				// フォントへのポインタ
float					TimeElapsed = 0;			//経過した時間
DWORD					FrameCnt = 0;				//
float					FPS = 0;					//FPS
char					FPSString[9];				//
bool					g_bUseDebug = true;			// デバッグ表示をするか 
bool					g_bUsePause = false;			// ポーズ中かどうか
float					timeDelta = 0.0f;
bool                    antiAlisa = true;    //
bool                    wireframe = false;
MODE g_mode = MODE_TITLE;		// 現在のモード

D3DXMATRIX proj;
// --------------------------------------------------
// MAIN関数
// --------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{// ウインドウクラスの構造体
		sizeof(WNDCLASSEX),						// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,								// ウインドウのスタイル
		WindowProc,								// ウインドウプロシージャ
		0,										// ０にする (通常は使用しない)
		0,										// ０にする (通常は使用しない)
		hInstance,								// インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),		// タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),			// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),				// クライアント領域の背景色
		NULL,									// メニューバー
		CLASS_NAME,								// ウインドウクラスの名前
	};

	HWND hWnd;												// ウインドウハンドル (識別子)
	MSG msg;												// メッセージを格納する変数
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};		// 画面サイズの構造体
	DWORD dwCorrentTime;									// 現在時刻
	DWORD dwExecLastTime;									// 最後に処理した時刻
	DWORD dwFrameCount;										// フレームカウント
	DWORD dwLastTimeFPS;									// 最後にFPSを計算した時刻

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを作成
	hWnd = CreateWindowEx(
		0,								// 拡張ウインドウスタイル
		CLASS_NAME,						// ウインドウスタイルの名前
		WINDOW_NAME,					// ウインドウの名前
		WS_OVERLAPPEDWINDOW,			// ウインドウスタイル
		CW_USEDEFAULT,					// ウインドウの左上X座標
		CW_USEDEFAULT,					// 　　〃　　の左上Y座標
		(rect.right - rect.left),		// 　　〃　　の幅
		(rect.bottom - rect.top),		// 　　〃　　の高さ
		NULL,							// 親ウインドウのハンドル
		NULL,							// メニューハンドルまたは子ウインドウID
		hInstance,						// インスタンスハンドル
		NULL);							// ウインドウ作成データ

	
	// 初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	dwCorrentTime = 0;					// 初期化する
	dwExecLastTime = timeGetTime();		// 現在時刻を取得 (保存)
	dwFrameCount = 0;					// 初期化する
	dwLastTimeFPS = timeGetTime();		// 現在時刻を取得 (保存)

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);			// ウインドウの表示状態を設定
	UpdateWindow(hWnd);					// クライアント領域を更新

	while (1)
	{// メッセージループ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{// メッセージの設定
				TranslateMessage(&msg);		// 仮想メッセージを文字のメッセージへ変換
				DispatchMessage(&msg);		// ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{// DirectXの処理
			dwCorrentTime = timeGetTime();		// 現在時刻を取得

			// 経過した時間を保存
			timeDelta = ((float)dwCorrentTime - (float)dwExecLastTime)*0.001f;

			if ((dwCorrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60分の1秒経過
				dwExecLastTime = dwCorrentTime;		// 処理開始の時刻[現在時刻]を保存

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;						// フレームカウントを加算
			}
		}
	}
	

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

// --------------------------------------------------
// ウインドウプロシージャ
// --------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;			// 返り値を格納

	const RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };				// ウインドウの領域 (短形)

	switch (uMsg)
	{
	case WM_DESTROY:		// ウインドウ破棄のメッセージ
							// WM_QOITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_CREATE:			// ウインドウ生成のメッセージ
							// タイマーの設定 ( WM_TIMERメッセージを定期的に送る )
		SetTimer(hWnd, ID_TIMER, TIMER_INTERVAL, NULL);
		break;

	case WM_CLOSE:		// 閉じるボタン押下のメッセージ

		nID = MessageBox(hWnd, "終了する？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

		if (nID == IDYES)
		{// YESを押された
		 // ウインドウを破棄する (WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;		// 0を返さないと終了してしまう
		}
		break;

	case WM_KEYDOWN:		// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:		// [ESC]キーが押された

			nID = MessageBox(hWnd, "終了する？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

			if (nID == IDYES)
			{// YESを押された
			 // ウインドウを破棄する (WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;


	case WM_LBUTTONDOWN:		// マウス左クリックのメッセージ	
		antiAlisa = !antiAlisa;
		break;

	case WM_RBUTTONDOWN:		// マウス右クリックのメッセージ	
		wireframe = !wireframe;
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);		// 規定の処理を返す
}

// --------------------------------------------------
// 初期化処理
// --------------------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;				// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;		// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{// 開かなかった時用の確認
		return E_FAIL;
	}

	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{// 現在のディスプレイモードを取得
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							// バックバッファの形式
	d3dpp.BackBufferCount = 1;										// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;							// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;	// FALSE; 							// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	d3dpp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;

	if (!d3dpp.Windowed)
	{//フルースクリーンの場合解像度を画面に合わせる
		d3dpp.BackBufferWidth = d3ddm.Width;
		d3dpp.BackBufferHeight = d3ddm.Height;
		d3dpp.BackBufferFormat = d3ddm.Format;
	}

	// Direct3Dデバイスの生成(描画処理と頂点処理をハードウェア)
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		// Direct3Dデバイスの生成(描画処理はハードウェア、頂点処理はCPUで行う)
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// Direct3Dデバイスの生成(描画処理と頂点処理をCPUで行う)
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// -------------------------
	// 各種オブジェクトの初期化処理
	// -------------------------

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

	//g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//// αの加算合成の設定 色＝対象の色　＊対象のα値 ＋ 背景の色　＊1.0
	//g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//// αの減算合成の設定  色＝ 背景の色　＊1.0 - 対象の色　＊対象のα値
	//g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	//g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//// 設定を元に戻す 色＝対象の色　＊対象のα値 ＋ 背景の色　＊(1-対象のα値)
	//g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	

	// サンプラーステートの設定
	
	//// Linear filtering
	// g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// 画像を小さくしても綺麗にする
	// g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// 画像を大きくしても綺麗にする

	//
	// テクスチャステージステートの設定
	//

	// Anisotropic filtering
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// テクスチャのUの繰り返し方を設定
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// テクスチャのVの繰り返し方を設定
	// g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);	// テクスチャのUの繰り返し方を設定
	// g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);	// テクスチャのVの繰り返し方を設定

	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		// ポリゴンとテクスチャのαをまぜる
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// １つ目の色はテクスチャの色
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		// ２つ目の色は現在の色

	// デバッグ表示用フォントの生成
	D3DXCreateFont(
		g_pD3DDevice,
		20,// 高さ
		0,// 幅
		20,// 重み
		0,// mipmapレベル
		FALSE,// 斜体のフォントの場合は True、それ以外の場合は false。
		SHIFTJIS_CHARSET,// フォントの文字セット
		OUT_DEFAULT_PRECIS, // 目的のフォントサイズと特性を実際のフォントと照合する方法を指定します。 OUT TT only を使用する のは、常に TrueType フォントを取得できるようにするため
		DEFAULT_QUALITY,// Windows が目的のフォントと実際のフォントをどのように一致させるかを指定します。 ラスターフォントにのみ適用され、TrueType フォントには影響しません。
		DEFAULT_PITCH,// ピッチとファミリのインデックス
		"Consolas", // タイプフェイス名を含む文字列。 コンパイラの設定で Unicode が必要な場合、データ型 LPCTSTR は LPCWSTR に解決されます。 それ以外の場合、文字列データ型は LPCSTR に解決されます。
		&g_pFont);// タイプフェイス名を含む文字列。 コンパイラの設定で Unicode が必要な場合、データ型 LPCTSTR は LPCWSTR に解決されます。 それ以外の場合、文字列データ型は LPCSTR に解決されます。

	// キーボードの初期化処理
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}


	// // サウンドの初期化処理
	// InitSound(hWnd);

	// // フェードの設定
	// InitFade(g_mode);

	// スカイボックスの初期化処理
	InitSkybox();

	// ポリゴンの初期化処理
	InitPolygon();

	// 影の初期化処理
	InitShadow();

	//壁の初期化処理
	InitWall();

	//壁の設定
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 200.0f), D3DXVECTOR3(-0.5f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(0.0f, 0.0f, -200.0f), D3DXVECTOR3(0.5f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(200.0f, 0.0f, 0.0f), D3DXVECTOR3(0.5f, -0.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(-200.0f, 0.0f, 0.0f), D3DXVECTOR3(0.5f, 0.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	SetWall(D3DXVECTOR3(0.0f, 0.0f, 200.0f), D3DXVECTOR3(0.5f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	SetWall(D3DXVECTOR3(0.0f, 0.0f, -200.0f), D3DXVECTOR3(-0.5f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	SetWall(D3DXVECTOR3(200.0f, 0.0f, 0.0f), D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	SetWall(D3DXVECTOR3(-200.0f, 0.0f, 0.0f), D3DXVECTOR3(-0.5f, 0.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	
	// カメラの初期化処理
	InitCamera();

	// ビルボードの初期化処理
	InitBillboard();

	// ビルボードの読み込み
	LoadBillboard(hWnd);

	// ライトの初期化処理
	InitLight();

	// モデルの初期化処理
	InitModel();

	// 3Dテキストの初期化処理
	InitCreateText();

	return S_OK;
}

// --------------------------------------------------
// 終了処理
// --------------------------------------------------
void Uninit(void)
{
	// 各種オブジェクトの終了処理-------------------------

	// 入力の終了処理
	UninitInput();

	// // タイトル画面の終了処理
	// UninitTitle();

	// // ゲーム画面の終了処理
	// UninitGame();

	// // リサイクル画面の終了処理
	// UninitResult();

	// // サウンドの終了処理
	// UninitSound();

	// // フェードの終了処理
	// UninitFade();

	// スカイボックスの終了処理
	UninitSkybox();

	// ポリゴンの終了処理
	UninitPolygon();

	// 影の終了処理
	UninitShadow();

	// 壁の終了処理
	UninitWall();

	// カメラの終了処理
	UninitCamera();

	// ビルボードの終了処理
	UninitBillboard();

	// ライトの終了処理
	UninitLight();

	// モデルの終了処理
	UninitModel();

	// 3Dテキストの終了処理
	UninitCreateText();

	// ------------------------------------------------

	if (g_pFont != NULL)
	{// デバッグ表示用フォントの破棄
		g_pFont->Release();
		g_pFont = NULL;
	}

	if (g_pD3DDevice != NULL)
	{// Direct3Dデバイスの破棄
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{// Direct3Dオブジェクトの破棄
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

// --------------------------------------------------
// 更新処理
// --------------------------------------------------
void Update(void)
{
	// // 各種オブジェクトの更新処理----------------------

	// switch (g_mode)
	// {
	// case MODE_TITLE:
	// 	UpdateTitle();
	// 	// ENTERボタンの更新処理
	// 	UpdateEnter();
	// 	break;

	// case MODE_TUTORIAL:
	// 	UpdateTutorial();
	// 	// ENTERボタンの更新処理
	// 	UpdateEnter();
	// 	break;

	// case MODE_GAME:
	// 	UpdateGame();
	// 	break;

	// case MODE_RESULT:
	// 	UpdateResult();
	// 	// ENTERボタンの更新処理
	// 	UpdateEnter();
	// 	break;
	// default:
	// 	break;
	// }

	// // フェードの更新処理
	// UpdateFade();

	// 入力の更新処理
	UpdataInput();

	// スカイボックスの更新処理
	UpdateSkybox();

	// ポリゴンの更新処理
	UpdatePolygon();

	// 影の更新処理
	UpdateShadow();

	// 壁の更新処理
	UpdateWall();

	// カメラの更新処理
	UpdateCamera();

	// ビルボードの更新処理
	UpdateBillboard();

	// ライトの更新処理
	UpdateLight();

	// モデルの更新処理
	UpdateModel();

	// 3Dテキストの更新処理
	UpdateCreateText();

	// ----------------------------------------------

#ifdef  _DEBUG

	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_Y))
	{// ポーズキー(Pキー)が押されたかどうか
		g_bUsePause = !g_bUsePause;
	}


#endif //   _DEBUG

	if (!g_bUsePause)
	{// ポーズしてない時

	}

#ifdef  _DEBUG

	if (GetKeyboardTrigger(DIK_F1) || GetJoypadTrigger(JOYKEY_BACK))
	{// F1キーが押された
		g_bUseDebug = !g_bUseDebug;
	}

#endif //   _DEBUG

}

// --------------------------------------------------
// 描画処理
// --------------------------------------------------
void Draw(void)
{
	// 画面クリア(バッグバッファ＆Zバッファのクリア)
	g_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER |D3DCLEAR_TARGET),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	// 描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合
		// -------------------------
		// 各種オブジェクトの描画処理
		// -------------------------
		
		if(wireframe)
		//g_pD3DDevice->SetMaterial(&YELLOW_MTRL);
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		else
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		// switch (g_mode)
		// {
		// case MODE_TITLE:

		// 	DrawTitle();
		// 	// ENTERボタンの描画処理
		// 	DrawEnter();
		// 	break;

		// case MODE_TUTORIAL:
		// 	DrawTutorial();
		// 	// ENTERボタンの描画処理
		// 	DrawEnter();
		// 	break;

		// case MODE_GAME:
		// 	DrawGame();
		// 	break;
		// case MODE_RESULT:
		// 	DrawResult();
		// 	// ENTERボタンの描画処理
		// 	DrawEnter();
		// 	break;
		// default:
		// 	break;
		// }

		// // フェードの描画処理
		// DrawFade();

		// カメラの設定
		SetCamera();

		// スカイボックスの描画処理
		DrawSkybox();

		// 壁の描画処理
		DrawWall();

		// ビルボードの描画処理
		DrawBillboard();

		// ポリゴンの描画処理
		DrawPolygon();

		// 影の描画処理
		DrawShadow();

		// モデルの描画処理
		DrawModel();

	

		// 3Dテキストの描画処理
		DrawCreateText();

#ifdef  _DEBUG

		if (g_bUseDebug)
		{// デバッグ表示
			// デバッグの表示
			DrawDebug();
		}

		// FSAA
		if (antiAlisa)
			g_pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
		else
			g_pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

		
#endif //   _DEBUG

		// 描画終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

// --------------------------------------------------
// デバイスの取得
// --------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}


LPDIRECT3DDEVICE9 GetSoundDevice(void)
{
	return g_pD3DDevice;
}

// モードの設定
void SetMode(MODE mode)
{
	// // 現在の画面(モード)の終了処理
	// switch (g_mode)
	// {
	// case MODE_TITLE:

	// 	UninitTitle();
	// 	// ENTERボタンの終了処理
	// 	UninitEnter();
	// 	break;

	// case MODE_TUTORIAL:

	// 	UninitTutorial();
	// 	// ENTERボタンの終了処理
	// 	UninitEnter();
	// 	break;

	// case MODE_GAME:
	// 	UninitGame();
	// 	break;

	// case MODE_RESULT:
	// 	UninitResult();
	// 	// ENTERボタンの終了処理
	// 	UninitEnter();
	// 	break;

	// default:
	// 	break;
	// }

	// // 現在の画面(モード)の終了処理
	// switch (mode)
	// {
	// case MODE_TITLE:

	// 	// ENTERボタンの初期化処理
	// 	InitEnter();
	// 	InitTitle();
	// 	break;

	// case MODE_TUTORIAL:

	// 	// ENTERボタンの初期化処理
	// 	InitEnter();
	// 	InitTutorial();
	// 	break;

	// case MODE_GAME:

	// 	InitGame();
	// 	break;

	// case MODE_RESULT:

	// 	// ENTERボタンの初期化処理
	// 	InitEnter();
	// 	InitResult();
	// 	break;

	// default:
	// 	break;
	// }

	g_mode = mode;
}

// モードの取得
MODE GetMode(void)
{
	return g_mode;
}

// --------------------------------------------------
// デバッグの表示
// --------------------------------------------------
void DrawDebug(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[1024];		// デバッグ表示用
	int nLength;

	FrameCnt++;

	TimeElapsed += timeDelta;

	if (TimeElapsed >= 1.0f)
	{
		FPS = (float)FrameCnt / TimeElapsed;


		sprintf(FPSString, "%.0f", FPS);
		FPSString[8] = '\0'; // mark end of string

		TimeElapsed = 0.0f;
		FrameCnt = 0;
	}

	// 文字列の代入
	CAMERA* pCamera = GetCamera();		//カメラの情報を取得

	sprintf(&aStr[0], "[ 操作説明 ]\n");
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "<< カメラ操作 >>\n");
	nLength = (int)strlen(&aStr[0]);

	sprintf(&aStr[nLength], "0          : カメラの随従(%d)\n", pCamera->bfollow);
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "A, S, D, W          : 視点の移動\n");
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "Z, C                  : 視点の旋回\n");
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "Q, E                  : 注視点の旋回\n");
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "T, G                  : 視点の上下移動\n");
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "Y, H                  : 注視点の上下移動\n");
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "U, J                  : 視点～注視点間の距離変更\n");
	nLength = (int)strlen(&aStr[0]);

	


	sprintf(&aStr[nLength], "視点の座標             : (%.3f, %.3f, %.3f)\n", pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "注視点の座標            : (%.3f, %.3f, %.3f)\n", pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "注視点と視点の角度      : %.3f\n", pCamera->rot.y);
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "注視点と視点の距離      : %.3f\n", pCamera->fDistance);
	nLength = (int)strlen(&aStr[0]);

	MODEL *pModel = GetModel();		//モデルの情報を取得
	DWORD NumVertices = pModel->Mesh->GetNumVertices();
	DWORD NumFaces = pModel->Mesh->GetNumFaces();

	sprintf(&aStr[nLength], "\n<< モデル操作 >>\n");
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "モデル頂点数：%d　　モデル三角形数：%d \n", NumVertices, NumFaces);
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "↑, ↓, ←, → ,1,0       : 移動 (%.3f, %.3f, %.3f)\n", pModel->pos.x, pModel->pos.y, pModel->pos.z);
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "2, 3                  : Y軸回転 (%.3f, %.3f, %.3f)\n", pModel->rot.x, pModel->rot.y, pModel->rot.z);
	nLength = (int)strlen(&aStr[0]);

	D3DLIGHT9 *plight = GetLight();

	BOOL Enable[3];
	for (int i = 0; i < 3; i++)
	{
		g_pD3DDevice->GetLightEnable(i, &Enable[i]);
	}

	sprintf(&aStr[nLength], "8,2,4,6        : スポットライト光源 (%.3f, %.3f, %.3f)\n", plight->Direction.x, plight->Direction.y, plight->Direction.z);
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "\n<< 光源操作 >>\n");
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "7           : 平行光源 (%d)\n", Enable[0]);
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "5           : 点光源 (%d)\n", Enable[1]);
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "9           : スポットライト光源 (%d)\n", Enable[2]);
	nLength = (int)strlen(&aStr[0]);

	sprintf(&aStr[nLength], "\n<< 設定 >>\n");
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "FSAA           :  (%d)\n", antiAlisa);
	nLength = (int)strlen(&aStr[0]);
	sprintf(&aStr[nLength], "wireframe      :  (%d)\n", wireframe);
	nLength = (int)strlen(&aStr[0]);


	// テキストの描画
	g_pFont->DrawText(NULL, 
		&aStr[0],
		-1, 
		&rect, 
		DT_LEFT,
		D3DCOLOR_RGBA(0,0, 0, 255));

	g_pFont->DrawText(NULL,
		&FPSString[0],
		-1,
		&rect,
		DT_RIGHT,
		D3DCOLOR_RGBA(0, 0, 0, 255));
}

// 経過した時間を取得
float* GetTime()
{
	return &timeDelta;
}