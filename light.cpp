#include "light.h"
#include "input.h"

D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

//
// グローバル変数
//
D3DLIGHT9 dirLight;		// 平行光源
D3DLIGHT9 pointLight;	// 点光源
D3DLIGHT9 spotLight;	// スポットライト光源

D3DXVECTOR3 vecDir;		// ライトの方向ベクトル

//
// ライトの初期化処理
//
void InitLight(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 平行光源の初期化処理
	D3DXVECTOR3 vecDir = D3DXVECTOR3(0.707f, -0.707f, 0.707f);
	D3DXVec3Normalize((D3DXVECTOR3*)&dirLight.Direction, &vecDir);
	D3DXCOLOR dircol = WHITE;
	dirLight = InitDirectionalLight(&vecDir, &dircol);

	// 平行光源をアクティブにする
	pDevice->SetLight(0, &dirLight);
	pDevice->LightEnable(0, true);

	// 点光源の初期化処理
	D3DXVECTOR3 pointPos(0.0f, 0.0f, 0.0f);
	D3DXCOLOR   pointCol = WHITE;
	pointLight = InitPointLight(&pointPos, &pointCol);

	// 点光源をアクティブにする
	pDevice->SetLight(1, &pointLight);
	pDevice->LightEnable(1, true);

	// スポットライト光源の初期化処理
	D3DXVECTOR3 spotPos(0.0f, 200.0f, 0.0f);
	D3DXVECTOR3 spotDir(0.0f, -0.2f, 0.0f);
	D3DXCOLOR   spotCol = GREEN;
	spotLight = InitSpotLight(&spotPos, &spotDir, &spotCol);

	// 点光源をアクティブにする
	pDevice->SetLight(2, &spotLight);
	pDevice->LightEnable(2, true);

}

//
// ライトの終了処理
//
void UninitLight(void)
{

}

//
// ライトの更新処理
//
void UpdateLight(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float time = *GetTime();

	if (GetKeyboardPress(DIK_NUMPAD8))
	{
		spotLight.Direction.z += 0.5f * time;

		// スポットライト光源をアクティブにする
		pDevice->SetLight(2, &spotLight);
		pDevice->LightEnable(2, true);
	}
	if (GetKeyboardPress(DIK_NUMPAD2))
	{
		spotLight.Direction.z -= 0.5f * time;

		// スポットライト光源をアクティブにする
		pDevice->SetLight(2, &spotLight);
		pDevice->LightEnable(2, true);
	}
	if (GetKeyboardPress(DIK_NUMPAD4))
	{
		spotLight.Direction.x -= 0.5f * time;

		// スポットライト光源をアクティブにする
		pDevice->SetLight(2, &spotLight);
		pDevice->LightEnable(2, true);
	}
	if (GetKeyboardPress(DIK_NUMPAD6))
	{
		spotLight.Direction.x += 0.5f * time;

		// スポットライト光源をアクティブにする
		pDevice->SetLight(2, &spotLight);
		pDevice->LightEnable(2, true);
	}

	BOOL Enable[3];
	if (GetKeyboardTrigger(DIK_NUMPAD7))
	{	
		pDevice->GetLightEnable(0, &Enable[0]);
		if (Enable[0])
		{
			// 平行光源をインアクティブにする
			pDevice->SetLight(0, &dirLight);
			pDevice->LightEnable(0, false);
		}
		else
		{
			// 平行光源をアクティブにする
			pDevice->SetLight(0, &dirLight);
			pDevice->LightEnable(0, true);
		}
	}

	if (GetKeyboardTrigger(DIK_NUMPAD5))
	{
		pDevice->GetLightEnable(1, &Enable[1]);
		if (Enable[1])
		{
			// 点光源をインアクティブにする
			pDevice->SetLight(1, &pointLight);
			pDevice->LightEnable(1, false);
		}
		else
		{
			// 点光源をアクティブにする
			pDevice->SetLight(1, &pointLight);
			pDevice->LightEnable(1, true);
		}
	}

	if (GetKeyboardTrigger(DIK_NUMPAD9))
	{
		pDevice->GetLightEnable(2, &Enable[2]);
		if (Enable[2])
		{
			// スポットライト光源をインアクティブにする
			pDevice->SetLight(2, &spotLight);
			pDevice->LightEnable(2, false);
		}
		else
		{
			// スポットライト光源をアクティブにする
			pDevice->SetLight(2, &spotLight);
			pDevice->LightEnable(2, true);
		}
	}
}

//
// 平行光源の初期化関数
//
D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Direction = *direction;
	return light;
}

//
// 点光源の初期化関数
//
D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_POINT;
	light.Ambient = *color * 0.6f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Position = *position;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;

	return light;
}

//
// スポットライト光源の初期化関数
//
D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_SPOT;
	light.Ambient = *color * 0.0f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Position = *position;
	light.Direction = *direction;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta = 0.4f;
	light.Phi = 0.9f;

	return light;
}

//
// ライト情報取得
//
D3DLIGHT9* GetLight(void)
{
	return &spotLight;
}