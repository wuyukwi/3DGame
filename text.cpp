/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#include "text.h"
#include "polygon.h"


ID3DXMesh* Text = NULL;

void InitCreateText(void)
{
	// Get a handle to a device context.
	HDC hdc = CreateCompatibleDC(0);
	LPDIRECT3DDEVICE9 Device = GetDevice();
	HFONT hFont;
	HFONT hFontOld;

	//
	// Describe the font we want.
	//

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));

	lf.lfHeight = 5000;    // in logical units
	lf.lfWidth = 12;    // in logical units
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 500;   // boldness, range 0(light) - 1000(bold)
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = 0;
	lf.lfPitchAndFamily = 0;
	strcpy(lf.lfFaceName, "Times New Roman"); // font style

	//GLYPHMETRICSFLOAT sFont;
	//POINTFLOAT pos;

	//sFont.gmfBlackBoxX = 200.0f;
	//sFont.gmfBlackBoxY = 200.0f;
	//sFont.gmfCellIncX = 0.0f;
	//sFont.gmfCellIncX = 0.0f;
	//pos.x = 0.0f;
	//pos.y = 200.0f;
	//sFont.gmfptGlyphOrigin = pos;

	//
	// Create the font and select it with the device context.
	//
    hFont = CreateFontIndirect(&lf);
    hFontOld = (HFONT)SelectObject(hdc, hFont); 

	//
	// Create the text mesh based on the selected font in the HDC.
	//
	D3DXCreateText(Device, hdc, "Direct3D",
		0.001f, 0.4f, &Text, 0, 0);

	//
	// Restore the old font and free the acquired HDC.
	//
	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteDC(hdc);
}

//--------------------------------------------------
// I—¹
//--------------------------------------------------
void UninitCreateText(void)
{
	if (Text != NULL)
	{
		Text->Release();
		Text = NULL;
	}
}

//--------------------------------------------------
// XV
//--------------------------------------------------
void UpdateCreateText(void)
{

}

//--------------------------------------------------
// •`‰æ
//--------------------------------------------------
void DrawCreateText(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	float* time = GetTime();

	D3DXMATRIX yRot, T;

	static float y = 0.0f;

	D3DXMatrixRotationY(&yRot, y);
	y += *time;

	if (y >= 6.28f)
		y = 0.0f;

	D3DXMatrixTranslation(&T, -1.6f, 0.0f, 0.0f);
	T = T * yRot;

	Device->SetTransform(D3DTS_WORLD, &T);

	Device->SetMaterial(&WHITE_MTRL);
	Text->DrawSubset(0);
}

