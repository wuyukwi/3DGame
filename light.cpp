#include "light.h"
#include "input.h"

D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

//
// �O���[�o���ϐ�
//
D3DLIGHT9 dirLight;		// ���s����
D3DLIGHT9 pointLight;	// �_����
D3DLIGHT9 spotLight;	// �X�|�b�g���C�g����

D3DXVECTOR3 vecDir;		// ���C�g�̕����x�N�g��

//
// ���C�g�̏���������
//
void InitLight(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���s�����̏���������
	D3DXVECTOR3 vecDir = D3DXVECTOR3(0.707f, -0.707f, 0.707f);
	D3DXVec3Normalize((D3DXVECTOR3*)&dirLight.Direction, &vecDir);
	D3DXCOLOR dircol = WHITE;
	dirLight = InitDirectionalLight(&vecDir, &dircol);

	// ���s�������A�N�e�B�u�ɂ���
	pDevice->SetLight(0, &dirLight);
	pDevice->LightEnable(0, true);

	// �_�����̏���������
	D3DXVECTOR3 pointPos(0.0f, 0.0f, 0.0f);
	D3DXCOLOR   pointCol = WHITE;
	pointLight = InitPointLight(&pointPos, &pointCol);

	// �_�������A�N�e�B�u�ɂ���
	pDevice->SetLight(1, &pointLight);
	pDevice->LightEnable(1, true);

	// �X�|�b�g���C�g�����̏���������
	D3DXVECTOR3 spotPos(0.0f, 200.0f, 0.0f);
	D3DXVECTOR3 spotDir(0.0f, -0.2f, 0.0f);
	D3DXCOLOR   spotCol = GREEN;
	spotLight = InitSpotLight(&spotPos, &spotDir, &spotCol);

	// �_�������A�N�e�B�u�ɂ���
	pDevice->SetLight(2, &spotLight);
	pDevice->LightEnable(2, true);

}

//
// ���C�g�̏I������
//
void UninitLight(void)
{

}

//
// ���C�g�̍X�V����
//
void UpdateLight(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float time = *GetTime();

	if (GetKeyboardPress(DIK_NUMPAD8))
	{
		spotLight.Direction.z += 0.5f * time;

		// �X�|�b�g���C�g�������A�N�e�B�u�ɂ���
		pDevice->SetLight(2, &spotLight);
		pDevice->LightEnable(2, true);
	}
	if (GetKeyboardPress(DIK_NUMPAD2))
	{
		spotLight.Direction.z -= 0.5f * time;

		// �X�|�b�g���C�g�������A�N�e�B�u�ɂ���
		pDevice->SetLight(2, &spotLight);
		pDevice->LightEnable(2, true);
	}
	if (GetKeyboardPress(DIK_NUMPAD4))
	{
		spotLight.Direction.x -= 0.5f * time;

		// �X�|�b�g���C�g�������A�N�e�B�u�ɂ���
		pDevice->SetLight(2, &spotLight);
		pDevice->LightEnable(2, true);
	}
	if (GetKeyboardPress(DIK_NUMPAD6))
	{
		spotLight.Direction.x += 0.5f * time;

		// �X�|�b�g���C�g�������A�N�e�B�u�ɂ���
		pDevice->SetLight(2, &spotLight);
		pDevice->LightEnable(2, true);
	}

	BOOL Enable[3];
	if (GetKeyboardTrigger(DIK_NUMPAD7))
	{	
		pDevice->GetLightEnable(0, &Enable[0]);
		if (Enable[0])
		{
			// ���s�������C���A�N�e�B�u�ɂ���
			pDevice->SetLight(0, &dirLight);
			pDevice->LightEnable(0, false);
		}
		else
		{
			// ���s�������A�N�e�B�u�ɂ���
			pDevice->SetLight(0, &dirLight);
			pDevice->LightEnable(0, true);
		}
	}

	if (GetKeyboardTrigger(DIK_NUMPAD5))
	{
		pDevice->GetLightEnable(1, &Enable[1]);
		if (Enable[1])
		{
			// �_�������C���A�N�e�B�u�ɂ���
			pDevice->SetLight(1, &pointLight);
			pDevice->LightEnable(1, false);
		}
		else
		{
			// �_�������A�N�e�B�u�ɂ���
			pDevice->SetLight(1, &pointLight);
			pDevice->LightEnable(1, true);
		}
	}

	if (GetKeyboardTrigger(DIK_NUMPAD9))
	{
		pDevice->GetLightEnable(2, &Enable[2]);
		if (Enable[2])
		{
			// �X�|�b�g���C�g�������C���A�N�e�B�u�ɂ���
			pDevice->SetLight(2, &spotLight);
			pDevice->LightEnable(2, false);
		}
		else
		{
			// �X�|�b�g���C�g�������A�N�e�B�u�ɂ���
			pDevice->SetLight(2, &spotLight);
			pDevice->LightEnable(2, true);
		}
	}
}

//
// ���s�����̏������֐�
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
// �_�����̏������֐�
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
// �X�|�b�g���C�g�����̏������֐�
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
// ���C�g���擾
//
D3DLIGHT9* GetLight(void)
{
	return &spotLight;
}