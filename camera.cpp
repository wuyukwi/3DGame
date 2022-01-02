#include "camera.h"
#include "input.h"
#include "model.h"
// グローバル変数
CAMERA g_camera;		// カメラ情報

float g_falpha;					//プレイヤーと注視点の角度
float g_fbeta;					//プレイヤーと視点の角度

void InputCamera();

// カメラの初期化処理
void InitCamera(void)
{
	g_camera.bfollow = true;

	if (g_camera.bfollow)
	{
		float fXZ, fd, alpha;				//計算用のローカル変数
		MODEL *pPlayer = GetModel();		//プレイヤー情報へのポインタ

											//視点・注視点・上方向を設定する
		g_camera.posV = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 20.0f, pPlayer->pos.z - 50.0f);
		g_camera.posR = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z + 2.0f);
		g_camera.DestPosV = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 20.0f, pPlayer->pos.z - 50.0f);
		g_camera.DestPosR = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z + 2.0f);
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera.fDistance = sqrtf(((pPlayer->pos.x - g_camera.posV.x) * (pPlayer->pos.x - g_camera.posV.x)) + ((pPlayer->pos.z - g_camera.posV.z) * (pPlayer->pos.z - g_camera.posV.z)));
		g_camera.fDistanceFocalPoint = sqrtf(((pPlayer->pos.x - g_camera.posR.x) * (pPlayer->pos.x - g_camera.posR.x)) + ((pPlayer->pos.z - g_camera.posR.z) * (pPlayer->pos.z - g_camera.posR.z)));
		g_camera.nDelay = 120;

		//初期化の角度の計算
		//視点から注視点までの距離を計算する
		fXZ = sqrtf(((g_camera.posV.x - g_camera.posR.x) * (g_camera.posV.x - g_camera.posR.x)) + ((g_camera.posV.z - g_camera.posR.z) * (g_camera.posV.z - g_camera.posR.z)));
		fd = sqrtf((fXZ * fXZ) + ((g_camera.posV.y - g_camera.posR.y) * (g_camera.posV.y - g_camera.posR.y)));

		alpha = asinf(fXZ / fd);

		g_camera.rot = D3DXVECTOR3(alpha, 0.0f, 0.0f);			//初期化の角度の設定

																//プレイヤーと視点の角度の計算
		fXZ = sqrtf(((pPlayer->pos.x - g_camera.DestPosV.x) * (pPlayer->pos.x - g_camera.DestPosV.x)) + ((pPlayer->pos.z - g_camera.DestPosV.z) * (pPlayer->pos.z - g_camera.DestPosV.z)));
		g_fbeta = D3DX_PI + (float)asin((pPlayer->pos.x - g_camera.DestPosV.x) / fXZ);

		//プレイヤーと注視点の角度の計算
		fXZ = sqrtf(((pPlayer->pos.x - g_camera.DestPosR.x) * (pPlayer->pos.x - g_camera.DestPosR.x)) + ((pPlayer->pos.x - g_camera.DestPosR.z) * (pPlayer->pos.x - g_camera.DestPosR.z)));
		g_falpha = D3DX_PI + (float)asin((pPlayer->pos.x - g_camera.DestPosR.x) / fXZ);
	}
	else
	{
		// 視点・注視点・上方向を設定する
		g_camera.posV = D3DXVECTOR3(0.0f, 20.0f, 120.0f);
		g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera.moveV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera.moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera.fDistance = 0.0f;
		g_camera.fAngle = 0.0f;

		float x = g_camera.posV.x - g_camera.posR.x;
		float z = g_camera.posV.z - g_camera.posR.z;

		// 対角線の角度を算出する
		g_camera.fAngle = atan2f((x), (z));
		// 対角線の長さを算出する
		g_camera.fDistance = sqrtf((x) * (x)+(z) * (z));
	}	

}

// カメラの終了処理
void UninitCamera(void)
{

}

// カメラの更新処理
void UpdateCamera(void)
{
	if (GetKeyboardTrigger(DIK_0))
	{
		g_camera.bfollow = !g_camera.bfollow;
	}

	if (g_camera.bfollow)
	{
		MODEL *pPlayer = GetModel();					//プレイヤーの情報を取得

		DIMOUSESTATE*pMouse = GetMouseState();

		if (GetKeyboardTrigger(DIK_LSHIFT))
		{
			g_camera.rot.y = pPlayer->rot.y - D3DX_PI;
		}

		g_camera.rot.y += 0.0025f * pMouse->lX;

		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y += 2.0f * -D3DX_PI;
		}
		else if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += 2.0f * D3DX_PI;
		}

		//注視点の位置の更新
		g_camera.DestPosR.x = pPlayer->pos.x + (sinf(pPlayer->rot.y + g_falpha) * g_camera.fDistanceFocalPoint);
		g_camera.DestPosR.y = pPlayer->pos.y;
		g_camera.DestPosR.z = pPlayer->pos.z + (cosf(pPlayer->rot.y + g_falpha) * g_camera.fDistanceFocalPoint);

		//慣性処理
		g_camera.posR += (g_camera.DestPosR - g_camera.posR) * 0.1f;

		//視点の位置の更新
		g_camera.DestPosV.x = pPlayer->pos.x + (sinf(g_fbeta + (g_camera.rot.y)) * g_camera.fDistance);
		g_camera.DestPosV.z = pPlayer->pos.z + (cosf(g_fbeta + (g_camera.rot.y)) * g_camera.fDistance);

		//慣性処理
		g_camera.posV += (g_camera.DestPosV - g_camera.posV) * 0.1f;
	}
	else
	{
		// カメラの操作
		InputCamera();
	}


	
}

// カメラの設定
void SetCamera(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		1.0f,
		10000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
}

void InputCamera()
{
	float CameraSpeed = 0.01f;
	float MoveSpeed = 1.0f;

	DIMOUSESTATE* pMouse = GetMouseState();

	// 視点の旋回
	if (GetKeyboardPress(DIK_Z)|| pMouse->lX>0.0f)
	{
		g_camera.rot.y += CameraSpeed;
		g_camera.rot.y += 0.001f * pMouse->lX;
		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y)*g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y)*g_camera.fDistance;
	}
	else if (GetKeyboardPress(DIK_C) || pMouse->lX<0.0f)
	{
		g_camera.rot.y += 0.001f * pMouse->lX;
		g_camera.rot.y -= CameraSpeed;
		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y)*g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y)*g_camera.fDistance;
	}
	// 視点の上下移動
	if (GetKeyboardPress(DIK_T))
	{
		g_camera.posV.y += CameraSpeed * 100;
	}
	if (GetKeyboardPress(DIK_G))
	{
		g_camera.posV.y -= CameraSpeed * 100;
	}
	

	// 注視点の旋回
	if (GetKeyboardPress(DIK_Q))
	{
		g_camera.rot.y -= CameraSpeed;
		g_camera.posR.x = g_camera.posV.x - sinf(g_camera.rot.y)*g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - cosf(g_camera.rot.y)*g_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_E))
	{
		g_camera.rot.y += CameraSpeed;
		g_camera.posR.x = g_camera.posV.x - sinf(g_camera.rot.y)*g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - cosf(g_camera.rot.y)*g_camera.fDistance;
	}
	// 注視点の上下移動
	if (GetKeyboardPress(DIK_Y))
	{
		g_camera.posR.y += CameraSpeed*100;
	}
	if (GetKeyboardPress(DIK_H))
	{
		g_camera.posR.y -= CameraSpeed*100;
	}

	float fRot;

	// カメラの移動
	if (GetKeyboardPress(DIK_W))
	{
		fRot = g_camera.rot.y;
		g_camera.posV.x -= sinf(fRot)*MoveSpeed;
		g_camera.posV.z -= cosf(fRot)*MoveSpeed;

		g_camera.posR.x = g_camera.posV.x - sinf(fRot)*g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - cosf(fRot)*g_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_S))
	{
		fRot = g_camera.rot.y;

		g_camera.posV.x += sinf(fRot)*MoveSpeed;
		g_camera.posV.z += cosf(fRot)*MoveSpeed;

		g_camera.posR.x = g_camera.posV.x - sinf(fRot)*g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - cosf(fRot)*g_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_A))
	{
		fRot = g_camera.rot.y + D3DX_PI*0.5f;
		g_camera.posV.x += sinf(fRot)*MoveSpeed;
		g_camera.posV.z += cosf(fRot)*MoveSpeed;

		g_camera.posR.x = g_camera.posV.x - sinf(g_camera.rot.y)*g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - cosf(g_camera.rot.y)*g_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_D))
	{
		fRot = g_camera.rot.y + D3DX_PI*0.5f;

		g_camera.posV.x -= sinf(fRot)*MoveSpeed;
		g_camera.posV.z -= cosf(fRot)*MoveSpeed;

		g_camera.posR.x = g_camera.posV.x - sinf(g_camera.rot.y)*g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z - cosf(g_camera.rot.y)*g_camera.fDistance;
	}


	if (GetKeyboardPress(DIK_U)&& g_camera.fDistance<300.0f)
	{
		g_camera.fDistance += MoveSpeed;
		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y)*g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y)*g_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_J) && g_camera.fDistance > 10.0f)
	{
		g_camera.fDistance -= MoveSpeed;
		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y)*g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y)*g_camera.fDistance;
	}


	// 角度の正規化
	if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y = -D3DX_PI;
	}
	else if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y = D3DX_PI;
	}

}

////---------------------------------------------------------------------------
//// カメラの追従処理
////---------------------------------------------------------------------------
//void FollowCamera(void)
//{
//	// プレイヤー情報の取得
//	Player *pPlayer = GetPlayer();
//
//	// 目的の注視点の算出
//	/*s_camera.posRDest.z = pPlayer->pos.z + sinf(pPlayer->rot.x) * cosf(pPlayer->rot.y) * -10.0f;
//	s_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.x) * sinf(pPlayer->rot.y) * -10.0f;
//	s_camera.posRDest.y = pPlayer->pos.y + cosf(pPlayer->rot.x) * -10.0f;*/
//
//	s_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * -10.0f;
//	s_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * -10.0f;
//	s_camera.posRDest.y = 0.0f;
//
//	// 目的の視点の算出
//	/*s_camera.posVDest.z = pPlayer->pos.z - sinf(s_camera.rot.x) * cosf(s_camera.rot.y) * s_camera.fDistance;
//	s_camera.posVDest.x = pPlayer->pos.x - sinf(s_camera.rot.x) * sinf(s_camera.rot.y) * s_camera.fDistance;
//	s_camera.posVDest.y = pPlayer->pos.y - cosf(s_camera.rot.x) * s_camera.fDistance;*/
//
//	s_camera.posVDest.z = pPlayer->pos.z - cosf(s_camera.rot.y) * s_camera.fDistance;
//	s_camera.posVDest.x = pPlayer->pos.x - sinf(s_camera.rot.y) * s_camera.fDistance;
//	s_camera.posVDest.y = 0.0f;
//
//	// 注視点の移動
//	s_camera.posR.x += (s_camera.posRDest.x - s_camera.posR.x) * 0.1f;
//	/*s_camera.posR.y += (s_camera.posRDest.y - s_camera.posR.y) * 0.1f;*/
//	s_camera.posR.z += (s_camera.posRDest.z - s_camera.posR.z) * 0.1f;
//
//	// 視点の移動
//	s_camera.posV.x += (s_camera.posVDest.x - s_camera.posV.x) * 0.1f;
//	/*s_camera.posV.y += (s_camera.posVDest.y - s_camera.posV.y) * 0.1f;*/
//	s_camera.posV.z += (s_camera.posVDest.z - s_camera.posV.z) * 0.1f;
//}


// カメラ情報取得
CAMERA *GetCamera(void)
{
	return &g_camera;
}