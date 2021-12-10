#include "camera.h"
#include "input.h"

void Camera::getViewMatrix(D3DXMATRIX* V)
{
	// Keep camera's axes orthogonal to eachother
	D3DXVec3Normalize(&_look, &_look);
	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);
	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);

	// Build the view matrix:
	float x = -D3DXVec3Dot(&_right, &_pos);
	float y = -D3DXVec3Dot(&_up, &_pos);
	float z = -D3DXVec3Dot(&_look, &_pos);

	(*V)(0, 0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
	(*V)(1, 0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
	(*V)(2, 0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
	(*V)(3, 0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
}

void Camera::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_right, angle);

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&_up, &_up, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}

void Camera::yaw(float angle)
{
	D3DXMATRIX T;

	// rotate around world y (0, 1, 0) always for land object
	if (_cameraType == LANDOBJECT)
		D3DXMatrixRotationY(&T, angle);

	// rotate around own up vector for aircraft
	if (_cameraType == AIRCRAFT)
		D3DXMatrixRotationAxis(&T, &_up, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&_right, &_right, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}

void Camera::roll(float angle)
{
	// only roll for aircraft type
	if (_cameraType == AIRCRAFT)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &_look, angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&_right, &_right, &T);
		D3DXVec3TransformCoord(&_up, &_up, &T);
	}
}

void Camera::walk(float units)
{
	// move only on xz plane for land object
	if (_cameraType == LANDOBJECT)
		_pos += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;
	if (_cameraType == AIRCRAFT)
		_pos += _look * units;
}
void Camera::strafe(float units)
{
	// move only on xz plane for land object
	if (_cameraType == LANDOBJECT)
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;
	if (_cameraType == AIRCRAFT)
		_pos += _right * units;
}
void Camera::fly(float units)
{
	// move only on y-axis for land object
	if (_cameraType == LANDOBJECT)
		_pos.y += units;
	if (_cameraType == AIRCRAFT)
		_pos += _up * units;
}

bool Display(float timeDelta)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (pDevice)
	{
		//
		// Update: Update the camera.
		//

		if (::GetAsyncKeyState('W') & 0x8000f)
			TheCamera.walk(4.0f * timeDelta);
		if (::GetAsyncKeyState('S') & 0x8000f)
			TheCamera.walk(-4.0f * timeDelta);
		if (::GetAsyncKeyState('A') & 0x8000f)
			TheCamera.strafe(-4.0f * timeDelta);
		if (::GetAsyncKeyState('D') & 0x8000f)
			TheCamera.strafe(4.0f * timeDelta);
		if (::GetAsyncKeyState('R') & 0x8000f)
			TheCamera.fly(4.0f * timeDelta);
		if (::GetAsyncKeyState('F') & 0x8000f)
			TheCamera.fly(-4.0f * timeDelta);
		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			TheCamera.pitch(1.0f * timeDelta);
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			TheCamera.pitch(-1.0f * timeDelta);
		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			TheCamera.yaw(-1.0f * timeDelta);
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			TheCamera.yaw(1.0f * timeDelta);
		if (::GetAsyncKeyState('N') & 0x8000f)
			TheCamera.roll(1.0f * timeDelta);
		if (::GetAsyncKeyState('M') & 0x8000f)
			TheCamera.roll(-1.0f * timeDelta);

		// Update the view matrix representing the cameras 
		// new position/orientation.
		D3DXMATRIX V;
		TheCamera.getViewMatrix(&V);
		Device->SetTransform(D3DTS_VIEW, &V);
		//
		// Render
		//
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();
		d3d::DrawBasicScene(Device, 1.0f);
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}
