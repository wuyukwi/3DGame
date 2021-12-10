#include "camera.h"
#include "input.h"

// �O���[�o���ϐ�
static CAMERA s_camera;		// �J�������


void InputCamera();

// �J�����̏���������
void InitCamera(void)
{
	// ���_�E�����_�E�������ݒ肷��
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

	// �Ίp���̊p�x���Z�o����
	s_camera.fAngle = atan2f((x), (z));
	// �Ίp���̒������Z�o����
	s_camera.fDistance = sqrtf((x) * (x)+(z) * (z));

}

// �J�����̏I������
void UninitCamera(void)
{

}

// �J�����̍X�V����
void UpdateCamera(void)
{
	// �J�����̑���
	InputCamera();

	
}

// �J�����̐ݒ�
void SetCamera(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&s_camera.mtxView,
		&s_camera.posV,
		&s_camera.posR,
		&s_camera.vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &s_camera.mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&s_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		1.0f,
		1000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &s_camera.mtxProjection);
}

void InputCamera()
{
	float CameraSpeed = 0.1f;
	float MoveSpeed = 1.0f;


	// ���_�̐���
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
	// ���_�̏㉺�ړ�
	if (GetKeyboardPress(DIK_T))
	{
		s_camera.posV.y += CameraSpeed * 100;
	}
	if (GetKeyboardPress(DIK_G))
	{
		s_camera.posV.y -= CameraSpeed * 100;
	}
	

	// �����_�̐���
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
	// �����_�̏㉺�ړ�
	if (GetKeyboardPress(DIK_Y))
	{
		s_camera.posR.y += CameraSpeed*100;
	}
	if (GetKeyboardPress(DIK_H))
	{
		s_camera.posR.y -= CameraSpeed*100;
	}

	float fRot;

	// �J�����̈ړ�
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


	// �p�x�̐��K��
	if (s_camera.rot.y > D3DX_PI)
	{
		s_camera.rot.y = -D3DX_PI;
	}
	else if (s_camera.rot.y < -D3DX_PI)
	{
		s_camera.rot.y = D3DX_PI;
	}

}

// �J�������擾
CAMERA *GetCamera(void)
{
	return &s_camera;
}