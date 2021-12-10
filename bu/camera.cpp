#include "camera.h"
#include "input.h"

// グローバル変数
static CAMERA s_camera;		// カメラ情報


void InputCamera();

// カメラの初期化処理
void InitCamera(void)
{
	// 視点・注視点・上方向を設定する
	s_camera.posV = D3DXVECTOR3(0.0f, 200.0f, -300.0f);
	s_camera.posR = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
	s_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	s_camera.moveV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_camera.moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_camera.rot = D3DXVECTOR3(0.0f, -D3DX_PI, 0.0f);
	s_camera.fDistance = 0.0f;
	s_camera.fAngle = 0.0f;

	float x = s_camera.posV.x - s_camera.posR.x;
	float z = s_camera.posV.z - s_camera.posR.z;

	// 対角線の角度を算出する
	s_camera.fAngle = atan2f((x), (z));
	// 対角線の長さを算出する
	s_camera.fDistance = sqrtf((x) * (x)+(z) * (z));

}

// カメラの終了処理
void UninitCamera(void)
{

}

// カメラの更新処理
void UpdateCamera(void)
{
	// カメラの操作
	InputCamera();

	
}

// カメラの設定
void SetCamera(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&s_camera.mtxView,
		&s_camera.posV,
		&s_camera.posR,
		&s_camera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &s_camera.mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&s_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&s_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		1.0f,
		1000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &s_camera.mtxProjection);
}

void InputCamera()
{
	float CameraSpeed = 0.1f;
	float MoveSpeed = 1.0f;


	// 視点の旋回
	if (GetKeyboardPress(DIK_Z))
	{
		s_camera.rot.y += CameraSpeed;
		s_camera.posV.x = s_camera.posR.x + sinf(s_camera.rot.y)*s_camera.fDistance;
		s_camera.posV.z = s_camera.posR.z + cosf(s_camera.rot.y)*s_camera.fDistance;
	}
	else if (GetKeyboardPress(DIK_C))
	{
		s_camera.rot.y -= CameraSpeed;
		s_camera.posV.x = s_camera.posR.x + sinf(s_camera.rot.y)*s_camera.fDistance;
		s_camera.posV.z = s_camera.posR.z + cosf(s_camera.rot.y)*s_camera.fDistance;
	}
	// 視点の上下移動
	if (GetKeyboardPress(DIK_T))
	{
		s_camera.posV.y += CameraSpeed * 100;
	}
	if (GetKeyboardPress(DIK_G))
	{
		s_camera.posV.y -= CameraSpeed * 100;
	}
	

	// 注視点の旋回
	if (GetKeyboardPress(DIK_Q))
	{
		s_camera.rot.y -= CameraSpeed;
		s_camera.posR.x = s_camera.posV.x - sinf(s_camera.rot.y)*s_camera.fDistance;
		s_camera.posR.z = s_camera.posV.z - cosf(s_camera.rot.y)*s_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_E))
	{
		s_camera.rot.y += CameraSpeed;
		s_camera.posR.x = s_camera.posV.x - sinf(s_camera.rot.y)*s_camera.fDistance;
		s_camera.posR.z = s_camera.posV.z - cosf(s_camera.rot.y)*s_camera.fDistance;
	}
	// 注視点の上下移動
	if (GetKeyboardPress(DIK_Y))
	{
		s_camera.posR.y += CameraSpeed*100;
	}
	if (GetKeyboardPress(DIK_H))
	{
		s_camera.posR.y -= CameraSpeed*100;
	}

	float fRot;

	// カメラの移動
	if (GetKeyboardPress(DIK_W))
	{
		fRot = s_camera.rot.y;
		s_camera.posV.x -= sinf(fRot)*MoveSpeed;
		s_camera.posV.z -= cosf(fRot)*MoveSpeed;

		s_camera.posR.x = s_camera.posV.x - sinf(fRot)*s_camera.fDistance;
		s_camera.posR.z = s_camera.posV.z - cosf(fRot)*s_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_S))
	{
		fRot = s_camera.rot.y;

		s_camera.posV.x += sinf(fRot)*MoveSpeed;
		s_camera.posV.z += cosf(fRot)*MoveSpeed;

		s_camera.posR.x = s_camera.posV.x - sinf(fRot)*s_camera.fDistance;
		s_camera.posR.z = s_camera.posV.z - cosf(fRot)*s_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_A))
	{
		fRot = s_camera.rot.y + D3DX_PI*0.5f;
		s_camera.posV.x += sinf(fRot)*MoveSpeed;
		s_camera.posV.z += cosf(fRot)*MoveSpeed;

		s_camera.posR.x = s_camera.posV.x - sinf(s_camera.rot.y)*s_camera.fDistance;
		s_camera.posR.z = s_camera.posV.z - cosf(s_camera.rot.y)*s_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_D))
	{
		fRot = s_camera.rot.y + D3DX_PI*0.5f;

		s_camera.posV.x -= sinf(fRot)*MoveSpeed;
		s_camera.posV.z -= cosf(fRot)*MoveSpeed;

		s_camera.posR.x = s_camera.posV.x - sinf(s_camera.rot.y)*s_camera.fDistance;
		s_camera.posR.z = s_camera.posV.z - cosf(s_camera.rot.y)*s_camera.fDistance;
	}


	if (GetKeyboardPress(DIK_U)&& s_camera.fDistance<300.0f)
	{
		s_camera.fDistance += MoveSpeed;
		s_camera.posV.x = s_camera.posR.x + sinf(s_camera.rot.y)*s_camera.fDistance;
		s_camera.posV.z = s_camera.posR.z + cosf(s_camera.rot.y)*s_camera.fDistance;
	}
	if (GetKeyboardPress(DIK_J) && s_camera.fDistance > 10.0f)
	{
		s_camera.fDistance -= MoveSpeed;
		s_camera.posV.x = s_camera.posR.x + sinf(s_camera.rot.y)*s_camera.fDistance;
		s_camera.posV.z = s_camera.posR.z + cosf(s_camera.rot.y)*s_camera.fDistance;
	}


	// 角度の正規化
	if (s_camera.rot.y > D3DX_PI)
	{
		s_camera.rot.y = -D3DX_PI;
	}
	else if (s_camera.rot.y < -D3DX_PI)
	{
		s_camera.rot.y = D3DX_PI;
	}

}

// カメラ情報取得
CAMERA *GetCamera(void)
{
	return &s_camera;
}