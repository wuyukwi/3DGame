
#ifndef _BILLBOARD_H_		// このマクロ定義がされてなかったら
#define _BILLBOARD_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos, float fWidth, float fHeight, LPDIRECT3DTEXTURE9 *pTexture);
void LoadBillboard(HWND hWnd);

#endif // !_BILLBOARD_H_