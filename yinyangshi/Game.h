#pragma once

#include "stdafx.h"


//��Ϸ״̬
enum Game_Status
{
	Ready,					//׼��
	Judgment_ends,			//�жϽ���
	Find_Level,				//�ؿ�
	Challenge,				//��ս
	Mobs,					//С��
	Boss,					//BOOSS
	Paper_Man				//����ֽ��
};

int Game_Init(void);

void Game_Exit(void);

void Game_Start(void);

void Save_Bmp();

void Save_Bmp(long nx, long ny, long nWidth, long nHeight, LPCWSTR szfilename);


bool Dodge_Handling( HBITMAP pRGBBuf);

void momvoerClock();

void momvoerClock(int x, int y);