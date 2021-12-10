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

#define FILE_NAME	"data/model/timo.x"

static void MoveModel();
static void normalized(float *rot);

// グローバル変数
MODEL				gModel;					// モデル構造体の宣言
DWORD				MatOut=NULL;
std::ofstream		OutFile;				// used to dump mesh data to file

ID3DXMesh* SphereMesh = 0;
ID3DXMesh* BoxMesh = 0;
bool RenderBoundingSphere = true;
//
// Prototypes
//

void dumpVertices(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpIndices(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpAttributeBuffer(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpAdjacencyBuffer(std::ofstream& outFile, ID3DXMesh* mesh);
void dumpAttributeTable(std::ofstream& outFile, ID3DXMesh* mesh);

// モデルの初期化処理
void InitModel(void)
{
	HRESULT hr = 0;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ZeroMemory(&gModel, sizeof(MODEL));

	// xファイルの読み込み
	D3DXLoadMeshFromX(FILE_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		&gModel.adjBuffer,
		&gModel.mtrlBuffer,
		NULL,
		&gModel.numMtrls,
		&gModel.Mesh);


	// 初期化処理
	gModel.pos = D3DXVECTOR3(0.0f, 50.0f, -50.0f);
	gModel.rot = D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	gModel.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	


	//
	// マテリアルとテクスチャの読み込み
	//

	gModel.Materials = new D3DMATERIAL9[gModel.numMtrls];

	gModel.Textures = new LPDIRECT3DTEXTURE9[gModel.numMtrls];

	if (gModel.mtrlBuffer != 0 && gModel.numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)gModel.mtrlBuffer->GetBufferPointer();

		for (DWORD i = 0; i < gModel.numMtrls; i++)
		{
			// matd3dプロパティはマテリアル読み込みの時はAmbient設定してないから、ここで設定する
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// マテリアルの設定
			gModel.Materials[i] = mtrls[i].MatD3D;

			// マテリアルにはテクスチャあるかどうかのチェック
			if (mtrls[i].pTextureFilename != 0)
			{
				//ある場合はテクスチャをサブセットに読み込み
				IDirect3DTexture9* tex = NULL;
				D3DXCreateTextureFromFile(
					pDevice,
					mtrls[i].pTextureFilename,
					&tex);

				//読み込んだテクスチャを保存する
				gModel.Textures[i] = tex;
			}
			else
			{
				//テクスチャない場合
				gModel.Textures[i] = 0;
			}
		}
	}

	//
	// メッシュの最適化
	//
	hr = gModel.Mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)gModel.adjBuffer->GetBufferPointer(),
		0,// new adjacency info
		0, 0);

	BoundingBox boundingBox;
	BoundingSphere boundingSphere;

	ComputeBoundingSphere(gModel.Mesh, &boundingSphere);
	ComputeBoundingBox(gModel.Mesh, &boundingBox);

	D3DXCreateSphere(
		pDevice,
		boundingSphere._radius,
		20,
		20,
		&SphereMesh,
		0);

	D3DXCreateBox(
		pDevice,
		boundingBox._max.x - boundingBox._min.x,
		boundingBox._max.y - boundingBox._min.y,
		boundingBox._max.z - boundingBox._min.z,
		&BoxMesh,
		0);

	//if (FAILED(hr))
	//{
	//	MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
	//	Release<ID3DXBuffer*>(gModel.adjBuffer); // free
	//}

	////
	//// プログレッシブメッシュの作成
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

	dumpVertices(OutFile, gModel.Mesh);
	dumpIndices(OutFile, gModel.Mesh);
	dumpAttributeTable(OutFile, gModel.Mesh);
	dumpAttributeBuffer(OutFile, gModel.Mesh);
	dumpAdjacencyBuffer(OutFile, gModel.Mesh);

#endif //   _DEBUG

}

// モデルの終了処理
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

	// メッシュの廃棄
	if (gModel.Mesh != NULL)
	{
		gModel.Mesh->Release();
		gModel.Mesh = NULL;
	}


	// 頂点バッファの廃棄
	if (gModel.mtrlBuffer != NULL)
	{
		gModel.mtrlBuffer->Release();
		gModel.mtrlBuffer = NULL;
	}

	Release<ID3DXMesh*>(SphereMesh);
	Release<ID3DXMesh*>(BoxMesh);
}

// モデルの更新処理
void UpdateModel(void)
{
	float DownSpeed = 0.1f;	//スピード減衰係数

	MoveModel();
	
	float fAngle = gModel.rotDest.y - gModel.rot.y;

	// 角度の正規化
	normalized(&fAngle);

	//慣性・向きを更新 (減衰させる)
	gModel.rot.y += fAngle * DownSpeed;

	// 角度の正規化
	normalized(&gModel.rot.y);

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		RenderBoundingSphere = !RenderBoundingSphere;
	}
}


// モデルの描画処理
void DrawModel(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
	

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&gModel.mtxWorld);		//  行列初期化関数(第1引数の行列を単位行列に初期化)

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, gModel.rot.y, gModel.rot.x, gModel.rot.z);		//  行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
	D3DXMatrixMultiply(&gModel.mtxWorld, &gModel.mtxWorld, &mtxRot);								//  行列掛け算関数(第2引数×第3引数第を１引数に格納)
																									
	D3DXMatrixTranslation(&mtxTrans, gModel.pos.x, gModel.pos.y, gModel.pos.z);				//  行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixMultiply(&gModel.mtxWorld, &gModel.mtxWorld, &mtxTrans);								//  行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &gModel.mtxWorld);
	

	for (DWORD i = 0; i < gModel.numMtrls; i++)
	{
		//  マテリアルの設定
		pDevice->SetMaterial(&gModel.Materials[i]);
		pDevice->SetTexture(0, gModel.Textures[i]);

		// モデルパーツの描写
		gModel.Mesh->DrawSubset(i);

#ifdef  _DEBUG

		//pDevice->SetMaterial(&YELLOW_MTRL);
		//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//gModel.Mesh->DrawSubset(i);
		//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

#endif //   _DEBUG

	}

	D3DMATERIAL9 blue = BLUE_MTRL;
	blue.Diffuse.a = 0.10f;

	pDevice->SetMaterial(&blue);
	pDevice->SetTexture(0, 0);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (RenderBoundingSphere)
	{
		SphereMesh->DrawSubset(0);
	}
	else
	{
		BoxMesh->DrawSubset(0);
	}

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

MODEL *GetModel(void)
{
	return &gModel;
}

static void MoveModel()
{
	CAMERA *pCamera = GetCamera();		//カメラの取得
	float fRot = 0.0f;
	float fMoveSpeed = 2.0f;	//スピード

	// モデルの移動
	if (GetKeyboardPress(DIK_LEFT))
	{// ←キーが押された
		if (GetKeyboardPress(DIK_UP))
		{// ↑キーが押された
			fRot = pCamera->rot.y + (D3DX_PI * 0.75f);

			gModel.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.25f);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ↓キーが押された
			fRot = pCamera->rot.y + (D3DX_PI * 0.25f);

			gModel.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.75f);
		}
		else
		{
			fRot = pCamera->rot.y + (D3DX_PI * 0.5f);

			gModel.rotDest.y = pCamera->rot.y + (-D3DX_PI * 0.5f);
		}

	}
	else if (GetKeyboardPress(DIK_RIGHT))
	{// →キーが押された
		if (GetKeyboardPress(DIK_UP))
		{// ↑キーが押された
			fRot = pCamera->rot.y + (-D3DX_PI * 0.75f);

			gModel.rotDest.y = pCamera->rot.y + (D3DX_PI * 0.25f);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{// ↓キーが押された
			fRot = pCamera->rot.y + (-D3DX_PI * 0.25f);

			gModel.rotDest.y = pCamera->rot.y + (D3DX_PI * 0.75f);
		}
		else
		{
			fRot = pCamera->rot.y + (-D3DX_PI * 0.5f);

			gModel.rotDest.y = pCamera->rot.y  + (D3DX_PI * 0.5f);
		}


	}
	else if (GetKeyboardPress(DIK_UP))
	{// ↑キーが押された
		fRot = pCamera->rot.y + D3DX_PI;

		gModel.rotDest.y = pCamera->rot.y;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{// ↓キーが押された
		fRot = pCamera->rot.y;

		gModel.rotDest.y = pCamera->rot.y + D3DX_PI;
	}

	if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) ||
		GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
	{// ←, →, ↑, ↓キーが押された
		gModel.pos.x += sinf(fRot) * fMoveSpeed;
		gModel.pos.z += cosf(fRot) * fMoveSpeed;
	}

	// モデルの回転
	if (GetKeyboardPress(DIK_NUMPAD2))
	{
		gModel.rotDest.y += 0.1f;
	}
	else if (GetKeyboardPress(DIK_NUMPAD3))
	{
		gModel.rotDest.y -= 0.1f;
	}

	// モデルの上下移動
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
		outFile << "頂点情報 " << i << ": (";
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
