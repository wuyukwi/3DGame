#include "set.h"

// --------------------------------------------------
// �S�Ă̏���������
// --------------------------------------------------
void SetAll(int cnt, VERTEX_3D *pVtx,
	float x,float y,float z,
	float nx, float ny,float nz,
	float u, float v)
{
	// �@���x�N�g���̏���������
	pVtx[cnt].nor.x = nx;
	pVtx[cnt].nor.y = ny;
	pVtx[cnt].nor.z = nz;

	// ���_���W�̏���������
	pVtx[cnt].pos.x = x;
	pVtx[cnt].pos.y = y;
	pVtx[cnt].pos.z = z;
	
	// ���_�J���[�̏���������
	pVtx[cnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̏���������
	pVtx[cnt].tex.x = u;
	pVtx[cnt].tex.y = v;
}

void SetAllPT(int cnt, VERTEX_PT *pVtx,
	float x, float y, float z,
	float u, float v)
{
	// ���_���W�̏���������
	pVtx[cnt].pos.x = x;
	pVtx[cnt].pos.y = y;
	pVtx[cnt].pos.z = z;


	// �e�N�X�`���̏���������
	pVtx[cnt].tex.x = u;
	pVtx[cnt].tex.y = v;
}

void SetAllPNT(int cnt, VERTEX_PNT *pVtx,
	float x, float y, float z,
	float nx, float ny, float nz,
	float u, float v)
{
	// ���_���W�̏���������
	pVtx[cnt].pos.x = x;
	pVtx[cnt].pos.y = y;
	pVtx[cnt].pos.z = z;

	// �@���x�N�g���̏���������
	pVtx[cnt].nor.x = nx;
	pVtx[cnt].nor.y = ny;
	pVtx[cnt].nor.z = nz;

	// �e�N�X�`���̏���������
	pVtx[cnt].tex.x = u;
	pVtx[cnt].tex.y = v;
}

BoundingBox::BoundingBox()
{
	// infinite small 
	_min.x = INFIN;
	_min.y = INFIN;
	_min.z = INFIN;

	_max.x = -INFIN;
	_max.y = -INFIN;
	_max.z = -INFIN;
}

bool BoundingBox::isPointInside(D3DXVECTOR3& p)
{
	if (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

BoundingSphere::BoundingSphere()
{
	_radius = 0.0f;
}

bool ComputeBoundingSphere(ID3DXMesh* mesh, BoundingSphere* sphere)
{
	HRESULT hr = 0;

	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);
	
	hr = D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)v,
		mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&sphere->_center,
		&sphere->_radius);

	mesh->UnlockVertexBuffer();

	if (FAILED(hr))
		return false;

	return true;
}

bool ComputeBoundingBox(ID3DXMesh* mesh, BoundingBox* box)
{
	HRESULT hr = 0;

	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);

	hr = D3DXComputeBoundingBox(
		(D3DXVECTOR3*)v,
		mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&box->_min,
		&box->_max);

	mesh->UnlockVertexBuffer();

	if (FAILED(hr))
		return false;

	return true;
}


