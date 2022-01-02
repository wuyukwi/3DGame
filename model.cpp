/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#include "model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include <fstream>
#include <vector>
#include "polygon.h"
#include "set.h"

const char* FILE_NAME = "data/model/airplane.x";

static void MoveModel();
static void normalized(float *rot);

// �O���[�o���ϐ�
MODEL				gModel;					// ���f���\���̂̐錾

std::ofstream		OutFile;				// used to dump mesh data to file

//
// Prototypes
//

void dumpVertices(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpIndices(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpAttributeBuffer(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpAdjacencyBuffer(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpAttributeTable(std::ofstream& outFile, ID3DXMesh* mesh);

// ���f���̏���������
void InitModel(void)
{
	HRESULT hr = 0;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ZeroMemory(&gModel, sizeof(MODEL));

	// x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(FILE_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		&gModel.adjBuffer,
		&gModel.mtrlBuffer,
		NULL,
		&gModel.numMtrls,
		&gModel.Mesh);


	// ����������
	gModel.pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	gModel.rot = D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	gModel.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gModel.bModelInteraction = true;
	gModel.BoxMesh = NULL;
	gModel.SphereMesh = NULL;

	//
	// �}�e���A���ƃe�N�X�`���̓ǂݍ���
	//

	gModel.Materials = new D3DMATERIAL9[gModel.numMtrls];

	gModel.Textures = new LPDIRECT3DTEXTURE9[gModel.numMtrls];

	if (gModel.mtrlBuffer != 0 && gModel.numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)gModel.mtrlBuffer->GetBufferPointer();

		for (DWORD i = 0; i < gModel.numMtrls; i++)
		{
			// matd3d�v���p�e�B�̓}�e���A���ǂݍ��݂̎���Ambient�ݒ肵�ĂȂ�����A�����Őݒ肷��
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// �}�e���A���̐ݒ�
			gModel.Materials[i] = mtrls[i].MatD3D;

			// �}�e���A���ɂ̓e�N�X�`�����邩�ǂ����̃`�F�b�N
			if (mtrls[i].pTextureFilename != 0)
			{
				//����ꍇ�̓e�N�X�`�����T�u�Z�b�g�ɓǂݍ���
				IDirect3DTexture9* tex = NULL;
				D3DXCreateTextureFromFile(
					pDevice,
					mtrls[i].pTextureFilename,
					&tex);

				//�ǂݍ��񂾃e�N�X�`����ۑ�����
				gModel.Textures[i] = tex;
			}
			else
			{
				//�e�N�X�`���Ȃ��ꍇ
				gModel.Textures[i] = 0;
			}
		}
	}

	//
	// ���b�V���̍œK��
	//
	hr = gModel.Mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)gModel.adjBuffer->GetBufferPointer(),
		0,// new adjacency info
		0, 0);


	ComputeBoundingSphere(gModel.Mesh, &gModel.boundingSphere);
	ComputeBoundingBox(gModel.Mesh, &gModel.boundingBox);

	D3DXCreateSphere(
		pDevice,
		gModel.boundingSphere._radius,
		20,
		20,
		&gModel.SphereMesh,
		0);

	D3DXCreateBox(
		pDevice,
		gModel.boundingBox._max.x - gModel.boundingBox._min.x,
		gModel.boundingBox._max.y - gModel.boundingBox._min.y,
		gModel.boundingBox._max.z - gModel.boundingBox._min.z,
		&gModel.BoxMesh,
		0);

	//if (FAILED(hr))
	//{
	//	MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
	//	Release<ID3DXBuffer*>(gModel.adjBuffer); // free
	//}

	////
	//// �v���O���b�V�u���b�V���̍쐬
	////
	//hr = D3DXGeneratePMesh(
	//	gModel.Mesh,
	//	(DWORD*)gModel.adjBuffer->GetBufferPointer(), // adjacency
	//	0,                  // default vertex attribute weights
	//	0,                  // default vertex weights
	//	1,                  // simplify as low as possible
	//	D3DXMESHSIMP_FACE,  // simplify by face count
	//	&gModel.pMesh);


	//if (FAILED(hr))
	//{
	//	MessageBox(0, "D3DXGeneratePMesh() - FAILED", 0, 0);
	//}

	//// set to original detail
	//DWORD maxFaces = gModel.pMesh->GetMaxFaces();
	//gModel.pMesh->SetNumFaces(maxFaces);


#ifdef  _DEBUG

	OutFile.open("Debug/Mesh Dump.txt");

	//dumpVertices(OutFile, gModel.Mesh);
	//dumpIndices(OutFile, gModel.Mesh);
	//dumpAttributeTable(OutFile, gModel.Mesh);
	//dumpAttributeBuffer(OutFile, gModel.Mesh);
	//dumpAdjacencyBuffer(OutFile, gModel.Mesh);

#endif //   _DEBUG

}

// ���f���̏I������
void UninitModel(void)
{
	if (gModel.Materials != NULL)
	{
		delete[] gModel.Materials;
	}

	if (gModel.Textures)
	{
		for (DWORD i = 0; i < gModel.numMtrls; i++)
		{
			if (gModel.Textures[i])
				gModel.Textures[i]->Release();
		}
		delete[] gModel.Textures;
	}

	// ���b�V���̔p��
	if (gModel.Mesh != NULL)
	{
		gModel.Mesh->Release();
		gModel.Mesh = NULL;
	}


	// ���_�o�b�t�@�̔p��
	if (gModel.mtrlBuffer != NULL)
	{
		gModel.mtrlBuffer->Release();
		gModel.mtrlBuffer = NULL;
	}

	SAFE_RELEASE(gModel.SphereMesh);
	SAFE_RELEASE(gModel.BoxMesh);
}

// ���f���̍X�V����
void UpdateModel(void)
{
	float DownSpeed = 0.1f;	//�X�s�[�h�����W��

	MoveModel();
	
	float fAngle = gModel.rotDest.y - gModel.rot.y;

	// �p�x�̐��K��
	normalized(&fAngle);

	//�����E�������X�V (����������)
	gModel.rot.y += fAngle * DownSpeed;

	// �p�x�̐��K��
	normalized(&gModel.rot.y);

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		gModel.RenderBoundingSphere = !gModel.RenderBoundingSphere;
	}
}


// ���f���̕`�揈��
void DrawModel(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&gModel.mtxWorld);		//  �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, gModel.rot.y, gModel.rot.x, gModel.rot.z);		//  �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&gModel.mtxWorld, &gModel.mtxWorld, &mtxRot);								//  �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
																									
	D3DXMatrixTranslation(&mtxTrans, gModel.pos.x, gModel.pos.y, gModel.pos.z);				//  �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&gModel.mtxWorld, &gModel.mtxWorld, &mtxTrans);								//  �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &gModel.mtxWorld);
	

	for (DWORD i = 0; i < gModel.numMtrls; i++)
	{
		//  �}�e���A���̐ݒ�
		pDevice->SetMaterial(&gModel.Materials[i]);
		pDevice->SetTexture(0, gModel.Textures[i]);

		// ���f���p�[�c�̕`��
		gModel.Mesh->DrawSubset(i);

	}

	D3DMATERIAL9 blue = BLUE_MTRL;
	blue.Diffuse.a = 0.10f;

	pDevice->SetMaterial(&blue);
	pDevice->SetTexture(0, 0);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (gModel.RenderBoundingSphere)
	{
		gModel.SphereMesh->DrawSubset(0);
	}
	else
	{
		gModel.BoxMesh->DrawSubset(0);
	}

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

MODEL *GetModel(void)
{
	return &gModel;
}

static void MoveModel()
{
	CAMERA *pCamera = GetCamera();		//�J�����̎擾
	float fRot = 0.0f;
	float fMoveSpeed = 2.0f;	//�X�s�[�h

	// ���f���̈ړ�
	if (GetKeyboardPress(DIK_RIGHT))
	{//  ���L�[�������ꂽ
		if (GetKeyboardPress(DIK_UP))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y + (D3DX_PI * 0.25f);

			gModel.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.75f);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y + (D3DX_PI * 0.75f);

			gModel.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.25f);
		}
		else
		{
			fRot = pCamera->rot.y + (D3DX_PI * 0.5f);

			gModel.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.5f);
		}

	}
	else if (GetKeyboardPress(DIK_LEFT))
	{// ���L�[�������ꂽ
		if (GetKeyboardPress(DIK_UP))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y + (-D3DX_PI * 0.25f);

			gModel.rotDest.y = pCamera->rot.y + (D3DX_PI * 0.75f);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ���L�[�������ꂽ
			fRot = pCamera->rot.y + (-D3DX_PI * 0.75f);

			gModel.rotDest.y = pCamera->rot.y + (D3DX_PI * 0.25f);
		}
		else
		{
			fRot = pCamera->rot.y + (-D3DX_PI * 0.5f);

			gModel.rotDest.y = pCamera->rot.y  + (D3DX_PI * 0.5f);
		}


	}
	else if (GetKeyboardPress(DIK_DOWN))
	{// ���L�[�������ꂽ
		fRot = pCamera->rot.y + D3DX_PI;

		gModel.rotDest.y = pCamera->rot.y;
	}
	else if (GetKeyboardPress(DIK_UP))
	{// ���L�[�������ꂽ
		fRot = pCamera->rot.y;

		gModel.rotDest.y = pCamera->rot.y + D3DX_PI;
	}

	if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
		GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
	{// ��, ��, ��, ���L�[�������ꂽ
		gModel.pos.x += sinf(fRot) * fMoveSpeed;
		gModel.pos.z += cosf(fRot) * fMoveSpeed;
	}

	// ���f���̉�]
	if (GetKeyboardPress(DIK_NUMPAD2))
	{
		gModel.rotDest.y += 0.1f;
	}
	else if (GetKeyboardPress(DIK_NUMPAD3))
	{
		gModel.rotDest.y -= 0.1f;
	}

	// ���f���̏㉺�ړ�
	if (GetKeyboardPress(DIK_NUMPAD1))
	{
		gModel.pos.y += 0.5f;
	}
	else if (GetKeyboardPress(DIK_NUMPAD0))
	{
		gModel.pos.y -= 0.5f;
	}
}

static void normalized(float *rot)
{
	if (*rot >= D3DX_PI)
	{
		*rot -= D3DX_PI * 2.0f;
	}
	else if (*rot <= -D3DX_PI)
	{
		*rot += D3DX_PI * 2.0f;
	}
}

//�����蔻��
void ModelInteraction(D3DXVECTOR3*pPos, D3DXVECTOR3*pLastPos, D3DXVECTOR3*pMove, float width, float height, float lenght)
{
	bool bInteraction = false;

	//�v�Z�p�̃��[�J���ϐ�
	float ModelRight, ModelLeft, ModelTop, ModelBottom, ModelFront, ModelBack;
	
	ModelLeft = gModel.boundingBox._min.x - (width * 0.5f);			//���f���̍���
	ModelRight = gModel.boundingBox._min.x + (width * 0.5f);			//���f���̉E��
	ModelBottom = gModel.boundingBox._min.y- height;				//���f���̉���
	ModelTop = gModel.boundingBox._min.y;							//���f���̏㑤
	ModelBack = gModel.boundingBox._min.z - (lenght * 0.5f);			//���f���̗���
	ModelFront = gModel.boundingBox._min.z + (lenght * 0.5f);		//���f���̕\��

	if (pPos->x > ModelLeft && pPos->z > ModelBack && pPos->z < ModelFront && pLastPos->x <= ModelLeft && pPos->y > ModelBottom && pPos->y < ModelTop)
	{//���̓����蔻��
		pPos->x = ModelLeft;
		pMove->x = 0.0f;
	}
	else if (pPos->x < ModelRight && pPos->z > ModelBack && pPos->z < ModelFront && pLastPos->x >= ModelRight && pPos->y > ModelBottom && pPos->y < ModelTop)
	{//�E�̓����蔻��
		pPos->x = ModelRight;
		pMove->x = 0.0f;
	}

	if (pPos->z > ModelBack && pPos->x > ModelLeft && pPos->x < ModelRight && pLastPos->z <= ModelBack && pPos->y > ModelBottom && pPos->y < ModelTop)
	{//���̓����蔻��
		pPos->z = ModelBack;
		pMove->z = 0.0f;
	}
	else if (pPos->z < ModelFront && pPos->x > ModelLeft && pPos->x < ModelRight && pLastPos->z >= ModelFront && pPos->y > ModelBottom && pPos->y < ModelTop)
	{//�O�̓����蔻��
		pPos->z = ModelFront;
		pMove->z = 0.0f;
	}

	if (pPos->y > ModelBottom && pLastPos->y <= ModelBottom && pPos->z > ModelBack && pPos->z < ModelFront && pPos->x > ModelLeft && pPos->x < ModelRight)
	{//���̓����蔻��
		pPos->y = ModelBottom;
		pMove->y = 0.0f;
	}
	else if (pPos->y < ModelTop && pLastPos->y >= ModelTop && pPos->z > ModelBack && pPos->z < ModelFront && pPos->x > ModelLeft && pPos->x < ModelRight)
	{//��̓����蔻��
		pPos->y = ModelTop;
		pMove->y = 0.0f;
		bInteraction = true;
	}
}
//
// Prototype Implementations
//

void dumpVertices(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Vertices:" << std::endl;
	outFile << "---------" << std::endl << std::endl;

	VERTEX_3D* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);
	for (DWORD i = 0; i < mesh->GetNumVertices(); i++)
	{
		outFile << "���_��� " << i << ": (";
		outFile << "pos :(" << v[i].pos.x << ", " << v[i].pos.y << ", " << v[i].pos.z << ") ";
		outFile << "nor :(" << v[i].nor.x << ", " << v[i].nor.y << ", " << v[i].nor.z << ") ";
		outFile << "tex :(" << v[i].tex.x << ", " << v[i].tex.y << "))" << std::endl;
	}
	mesh->UnlockVertexBuffer();

	outFile << std::endl << std::endl;
}

void dumpIndices(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Indices:" << std::endl;
	outFile << "--------" << std::endl << std::endl;

	WORD* indices = 0;
	mesh->LockIndexBuffer(0, (void**)&indices);

	for (DWORD i = 0; i < mesh->GetNumFaces(); i++)
	{
		outFile << "Triangle " << i << ": ";
		outFile << indices[i * 3] << " ";
		outFile << indices[i * 3 + 1] << " ";
		outFile << indices[i * 3 + 2] << std::endl;
	}
	mesh->UnlockIndexBuffer();

	outFile << std::endl << std::endl;
}

void dumpAttributeBuffer(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Attribute Buffer:" << std::endl;
	outFile << "-----------------" << std::endl << std::endl;

	DWORD* attributeBuffer = 0;
	mesh->LockAttributeBuffer(0, &attributeBuffer);

	// an attribute for each face
	for (DWORD i = 0; i < mesh->GetNumFaces(); i++)
	{
		outFile << "Triangle lives in subset " << i << ": ";
		outFile << attributeBuffer[i] << std::endl;
	}
	mesh->UnlockAttributeBuffer();

	outFile << std::endl << std::endl;
}

void dumpAdjacencyBuffer(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Adjacency Buffer:" << std::endl;
	outFile << "-----------------" << std::endl << std::endl;

	// three enttries per face
	std::vector<DWORD> adjacencyBuffer(mesh->GetNumFaces() * 3);

	mesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]);

	for (DWORD i = 0; i < mesh->GetNumFaces(); i++)
	{
		outFile << "Triangle's adjacent to triangle " << i << ": ";
		outFile << adjacencyBuffer[i * 3] << " ";
		outFile << adjacencyBuffer[i * 3 + 1] << " ";
		outFile << adjacencyBuffer[i * 3 + 2] << std::endl;
	}

	outFile << std::endl << std::endl;
}

void dumpAttributeTable(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Attribute Table:" << std::endl;
	outFile << "----------------" << std::endl << std::endl;

	// number of entries in the attribute table
	DWORD numEntries = 0;

	mesh->GetAttributeTable(0, &numEntries);

	std::vector<D3DXATTRIBUTERANGE> table(numEntries);

	mesh->GetAttributeTable(&table[0], &numEntries);

	for (DWORD i = 0; i < numEntries; i++)
	{
		outFile << "Entry " << i << std::endl;
		outFile << "-----------" << std::endl;

		outFile << "Subset ID:    " << table[i].AttribId << std::endl;
		outFile << "Face Start:   " << table[i].FaceStart << std::endl;
		outFile << "Face Count:   " << table[i].FaceCount << std::endl;
		outFile << "Vertex Start: " << table[i].VertexStart << std::endl;
		outFile << "Vertex Count: " << table[i].VertexCount << std::endl;
		outFile << std::endl;
	}

	outFile << std::endl << std::endl;
}
