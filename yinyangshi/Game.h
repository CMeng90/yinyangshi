#pragma once

#include "stdafx.h"


//游戏状态
enum Game_Status
{
	Ready,					//准备
	Judgment_ends,			//判断结束
	Find_Level,				//关卡
	Challenge,				//挑战
	Mobs,					//小怪
	Boss,					//BOOSS
	Paper_Man				//结束纸人
};

int Game_Init(void);

void Game_Exit(void);

void Game_Start(void);

void Save_Bmp();

void Save_Bmp(long nx, long ny, long nWidth, long nHeight, LPCWSTR szfilename);


bool Dodge_Handling( HBITMAP pRGBBuf);

void momvoerClock();

void momvoerClock(int x, int y);