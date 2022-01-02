#include "camera.h"
#include "input.h"
#include "model.h"
// �O���[�o���ϐ�
CAMERA g_camera;		// �J�������

float g_falpha;					//�v���C���[�ƒ����_�̊p�x
float g_fbeta;					//�v���C���[�Ǝ��_�̊p�x

void InputCamera();

// �J�����̏���������
void InitCamera(void)
{
	g_camera.bfollow = true;

	if (g_camera.bfollow)
	{
		float fXZ, fd, alpha;				//�v�Z�p�̃��[�J���ϐ�
		MODEL *pPlayer = GetModel();		//�v���C���[���ւ̃|�C���^

											//���_�E�����_�E�������ݒ肷��
		g_camera.posV = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 20.0f, pPlayer->pos.z - 50.0f);
		g_camera.posR = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z + 2.0f);
		g_camera.DestPosV = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 20.0f, pPlayer->pos.z - 50.0f);
		g_camera.DestPosR = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z + 2.0f);
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera.fDistance = sqrtf(((pPlayer->pos.x - g_camera.posV.x) * (pPlayer->pos.x - g_camera.posV.x)) + ((pPlayer->pos.z - g_camera.posV.z) * (pPlayer->pos.z - g_camera.posV.z)));
		g_camera.fDistanceFocalPoint = sqrtf(((pPlayer->pos.x - g_camera.posR.x) * (pPlayer->pos.x - g_camera.posR.x)) + ((pPlayer->pos.z - g_camera.posR.z) * (pPlayer->pos.z - g_camera.posR.z)));
		g_camera.nDelay = 120;

		//�������̊p�x�̌v�Z
		//���_���璍���_�܂ł̋������v�Z����
		fXZ = sqrtf(((g_camera.posV.x - g_camera.posR.x) * (g_camera.posV.x - g_camera.posR.x)) + ((g_camera.posV.z - g_camera.posR.z) * (g_camera.posV.z - g_camera.posR.z)));
		fd = sqrtf((fXZ * fXZ) + ((g_camera.posV.y - g_camera.posR.y) * (g_camera.posV.y - g_camera.posR.y)));

		alpha = asinf(fXZ / fd);

		g_camera.rot = D3DXVECTOR3(alpha, 0.0f, 0.0f);			//�������̊p�x�̐ݒ�

																//�v���C���[�Ǝ��_�̊p�x�̌v�Z
		fXZ = sqrtf(((pPlayer->pos.x - g_camera.DestPosV.x) * (pPlayer->pos.x - g_camera.DestPosV.x)) + ((pPlayer->pos.z - g_camera.DestPosV.z) * (pPlayer->pos.z - g_camera.DestPosV.z)));
		g_fbeta = D3DX_PI + (float)asin((pPlayer->pos.x - g_camera.DestPosV.x) / fXZ);

		//�v���C���[�ƒ����_�̊p�x�̌v�Z
		fXZ = sqrtf(((pPlayer->pos.x - g_camera.DestPosR.x) * (pPlayer->pos.x - g_camera.DestPosR.x)) + ((pPlayer->pos.x - g_camera.DestPosR.z) * (pPlayer->pos.x - g_camera.DestPosR.z)));
		g_falpha = D3DX_PI + (float)asin((pPlayer->pos.x - g_camera.DestPosR.x) / fXZ);
	}
	else
	{
		// ���_�E�����_�E�������ݒ肷��
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

		// �Ίp���̊p�x���Z�o����
		g_camera.fAngle = atan2f((x), (z));
		// �Ίp���̒������Z�o����
		g_camera.fDistance = sqrtf((x) * (x)+(z) * (z));
	}	

}

// �J�����̏I������
void UninitCamera(void)
{

}

// �J�����̍X�V����
void UpdateCamera(void)
{
	if (GetKeyboardTrigger(DIK_0))
	{
		g_camera.bfollow = !g_camera.bfollow;
	}

	if (g_camera.bfollow)
	{
		MODEL *pPlayer = GetModel();					//�v���C���[�̏����擾

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

		//�����_�̈ʒu�̍X�V
		g_camera.DestPosR.x = pPlayer->pos.x + (sinf(pPlayer->rot.y + g_falpha) * g_camera.fDistanceFocalPoint);
		g_camera.DestPosR.y = pPlayer->pos.y;
		g_camera.DestPosR.z = pPlayer->pos.z + (cosf(pPlayer->rot.y + g_falpha) * g_camera.fDistanceFocalPoint);

		//��������
		g_camera.posR += (g_camera.DestPosR - g_camera.posR) * 0.1f;

		//���_�̈ʒu�̍X�V
		g_camera.DestPosV.x = pPlayer->pos.x + (sinf(g_fbeta + (g_camera.rot.y)) * g_camera.fDistance);
		g_camera.DestPosV.z = pPlayer->pos.z + (cosf(g_fbeta + (g_camera.rot.y)) * g_camera.fDistance);

		//��������
		g_camera.posV += (g_camera.DestPosV - g_camera.posV) * 0.1f;
	}
	else
	{
		// �J�����̑���
		InputCamera();
	}


	
}

// �J�����̐ݒ�
void SetCamera(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		1.0f,
		10000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
}

void InputCamera()
{
	float CameraSpeed = 0.01f;
	float MoveSpeed = 1.0f;

	DIMOUSESTATE* pMouse = GetMouseState();

	// ���_�̐���
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
	// ���_�̏㉺�ړ�
	if (GetKeyboardPress(DIK_T))
	{
		g_camera.posV.y += CameraSpeed * 100;
	}
	if (GetKeyboardPress(DIK_G))
	{
		g_camera.posV.y -= CameraSpeed * 100;
	}
	

	// �����_�̐���
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
	// �����_�̏㉺�ړ�
	if (GetKeyboardPress(DIK_Y))
	{
		g_camera.posR.y += CameraSpeed*100;
	}
	if (GetKeyboardPress(DIK_H))
	{
		g_camera.posR.y -= CameraSpeed*100;
	}

	float fRot;

	// �J�����̈ړ�
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


	// �p�x�̐��K��
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
//// �J�����̒Ǐ]����
////---------------------------------------------------------------------------
//void FollowCamera(void)
//{
//	// �v���C���[���̎擾
//	Player *pPlayer = GetPlayer();
//
//	// �ړI�̒����_�̎Z�o
//	/*s_camera.posRDest.z = pPlayer->pos.z + sinf(pPlayer->rot.x) * cosf(pPlayer->rot.y) * -10.0f;
//	s_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.x) * sinf(pPlayer->rot.y) * -10.0f;
//	s_camera.posRDest.y = pPlayer->pos.y + cosf(pPlayer->rot.x) * -10.0f;*/
//
//	s_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rot.y) * -10.0f;
//	s_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.y) * -10.0f;
//	s_camera.posRDest.y = 0.0f;
//
//	// �ړI�̎��_�̎Z�o
//	/*s_camera.posVDest.z = pPlayer->pos.z - sinf(s_camera.rot.x) * cosf(s_camera.rot.y) * s_camera.fDistance;
//	s_camera.posVDest.x = pPlayer->pos.x - sinf(s_camera.rot.x) * sinf(s_camera.rot.y) * s_camera.fDistance;
//	s_camera.posVDest.y = pPlayer->pos.y - cosf(s_camera.rot.x) * s_camera.fDistance;*/
//
//	s_camera.posVDest.z = pPlayer->pos.z - cosf(s_camera.rot.y) * s_camera.fDistance;
//	s_camera.posVDest.x = pPlayer->pos.x - sinf(s_camera.rot.y) * s_camera.fDistance;
//	s_camera.posVDest.y = 0.0f;
//
//	// �����_�̈ړ�
//	s_camera.posR.x += (s_camera.posRDest.x - s_camera.posR.x) * 0.1f;
//	/*s_camera.posR.y += (s_camera.posRDest.y - s_camera.posR.y) * 0.1f;*/
//	s_camera.posR.z += (s_camera.posRDest.z - s_camera.posR.z) * 0.1f;
//
//	// ���_�̈ړ�
//	s_camera.posV.x += (s_camera.posVDest.x - s_camera.posV.x) * 0.1f;
//	/*s_camera.posV.y += (s_camera.posVDest.y - s_camera.posV.y) * 0.1f;*/
//	s_camera.posV.z += (s_camera.posVDest.z - s_camera.posV.z) * 0.1f;
//}


// �J�������擾
CAMERA *GetCamera(void)
{
	return &g_camera;
}