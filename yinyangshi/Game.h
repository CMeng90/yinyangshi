#pragma once

#include "stdafx.h"
void Game_Init(void);

void Game_Exit(void);

void Game_Start(void);

void Save_Bmp();

void Save_Bmp(long nx, long ny, long nWidth, long nHeight, LPCWSTR szfilename);