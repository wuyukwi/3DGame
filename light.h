/*
--------------------------------------------------
Author: Huang QiYue
--------------------------------------------------
*/
#ifndef _Light_H_		// このマクロ定義がされてなかったら
#define _Light_H_		// ２重インクルード防止のマクロ定義

#include "main.h"

void InitLight(void);
void UninitLight(void);
void UpdateLight(void);
D3DLIGHT9* GetLight(void);


#endif