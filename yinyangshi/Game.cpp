#include "stdafx.h"


HWND GameMianHend = NULL;		//��Ϸ������

HWND GameMoNiHend = NULL;		//ģ�����ӿ�
RECT lpRect;					//��Ϸ�����С


HDC hdcSrc, hdcMem;			//����DC

int start_x, start_y, end_x, end_y;		//�ҵ�λ�õ�����

//��Ϸ״̬
enum Game_Status
{
	Find_Level,				//Ѱ�ҹؿ�
	Challenge,				//ѡ����ս
	Mobs,					//Ѱ��С��
	Judgment_ends,			//�жϽ���
	Boss,					//Ѱ��BOOSS
	Paper_Man				//Ѱ�ҽ���ֽ��
};


//ģ����괦��
void momvoerClock()
{
	LPARAM zuobiao = start_y;
	zuobiao = zuobiao << 16;
	zuobiao += start_x;
	SendMessage(GameMoNiHend, WM_LBUTTONDOWN, MK_LBUTTON, zuobiao);
	SendMessage(GameMoNiHend, WM_LBUTTONUP, 0, zuobiao);
}



// ͼƬת���ڴ�������
//	hBitmap		ͼƬ���
//  lpbi		ָ��λͼ��Ϣͷ�ṹ     
BOOL BitmapToBitmap32(HBITMAP   hBitmap1, HBITMAP hBitmap2)
{
	HDC     hDC;
	//��ǰ�ֱ�����ÿ������ռ�ֽ���            
	int     iBits;
	//λͼ��ÿ������ռ�ֽ���            
	WORD     wBitCount;
	//�����ɫ���С��     λͼ�������ֽڴ�С     ��λͼ�ļ���С     ��     д���ļ��ֽ���                
	DWORD     dwPaletteSize = 0, dwBmBitsSize1 = 0, dwBmBitsSize2 = 0, dwDIBSize = 0, dwWritten = 0;
	//λͼ���Խṹ                
	BITMAP     Bitmap;
	//λͼ�ļ�ͷ�ṹ            
	BITMAPFILEHEADER     bmfHdr;
	//ָ��λͼ��Ϣͷ�ṹ                    
	LPBITMAPINFOHEADER     lpbi1, lpbi2;
	//λͼ��Ϣͷ�ṹ                
	BITMAPINFOHEADER     bi;

	//�����ļ��������ڴ�������ɫ����                
	HANDLE     hDib1, hDib2, hOldPal = NULL;

	//����λͼ�ļ�ÿ��������ռ�ֽ���  
	wBitCount = 32;

	GetObject(hBitmap1, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;

	// 24λ��ɫ��ȼ����С ��ȴ洢�Ĵ�С ����Ҫ4�ı��� 
	//��ͼƬ��� * λ�� + 31��/32 ��ʾ�����ɫ��Сλ����Ϊ32�ı��� ��һλ��������һ��long��С����Ϊ���������û���������룩������ոպã��򲻻��һ��long��С
	// ����24λ���ش�С��ͼƬ��ʵ��СӦ��Ϊ ���bmWidth*3 Ϊ4����������size = bmWidth*3*bmHeight������size = ((Bitmap.bmWidth *wBitCount + 31) / 32) * 4 * Bitmap.bmHeight
	dwBmBitsSize1 = Bitmap.bmWidth * 4 * Bitmap.bmHeight;



	//Ϊλͼ���ݷ����ڴ�                
	hDib1 = GlobalAlloc(GHND, dwBmBitsSize1 + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi1 = (LPBITMAPINFOHEADER)GlobalLock(hDib1);
	*lpbi1 = bi;

	//     ��ȡ�õ�ɫ�����µ�����ֵ                
	GetDIBits(hdcMem, hBitmap1, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi1 + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
		(BITMAPINFO *)lpbi1, DIB_RGB_COLORS);


	//��2��ͼ
	GetObject(hBitmap2, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;

	dwBmBitsSize2 = Bitmap.bmWidth * 4 * Bitmap.bmHeight;

	//Ϊλͼ���ݷ����ڴ�                
	hDib2 = GlobalAlloc(GHND, dwBmBitsSize2 + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi2 = (LPBITMAPINFOHEADER)GlobalLock(hDib2);
	*lpbi2 = bi;

	//     ��ȡ�õ�ɫ�����µ�����ֵ                
	GetDIBits(hdcMem, hBitmap2, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi2 + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
		(BITMAPINFO *)lpbi2, DIB_RGB_COLORS);

	//��long���ͽ��бȽ� һ�ο��ԱȽ�һ������ ������24λ ������3���ֽ�
	COLORREF * a, *b;
	int Y = 0;  //Y�ᣬÿһ�ε�Y��Ӧ�ö���һ����

	char *  tmp1, *tmp2;
	tmp1 = (char *)lpbi1 + 54;
	tmp2 = (char *)lpbi2 + 54;

	//��ΪͼƬ�ڴ�buf�洢�ֽ�������ģ���
	a = (COLORREF *)tmp1;
	b = (COLORREF *)tmp2;


	DWORD i = 0, j = 0, z = 0;
	while (i < dwBmBitsSize1 / 4)
	{
		if (*a == *b)
		{
			start_x = i % 1005;
			start_y = 568 - i / 1005;
			b++;
			j++;
			if (j > Bitmap.bmHeight)
			{
				Y++;
				j = 0;
				if (Y > 1)
				{
					momvoerClock();
					return true;
				}
				
			}			
		}
		else
		{
			if (j < 10)
			{
				j = 0;
			}
			//else
			//{
			//	end_x = i % 1005;
			//	end_y = 568 - i / 1005;
			//	j = 0;
			//}
		}
		a++;
		i++;
	}


	//���                    
	GlobalUnlock(hDib1);
	GlobalFree(hDib1);
	GlobalUnlock(hDib2);
	GlobalFree(hDib2);
	return  false;

}


//��RGBת��Ϊ32λBMPͼƬ
BOOL SaveBitmapToFile32(HBITMAP   hBitmap, LPCWSTR szfilename)
{
	HDC     hDC;
	//��ǰ�ֱ�����ÿ������ռ�ֽ���            
	int     iBits;
	//λͼ��ÿ������ռ�ֽ���            
	WORD     wBitCount;
	//�����ɫ���С��     λͼ�������ֽڴ�С     ��λͼ�ļ���С     ��     д���ļ��ֽ���                
	DWORD     dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	//λͼ���Խṹ                
	BITMAP     Bitmap;
	//λͼ�ļ�ͷ�ṹ            
	BITMAPFILEHEADER     bmfHdr;
	//λͼ��Ϣͷ�ṹ                
	BITMAPINFOHEADER     bi;
	//ָ��λͼ��Ϣͷ�ṹ                    
	LPBITMAPINFOHEADER     lpbi;
	//�����ļ��������ڴ�������ɫ����                
	HANDLE     fh, hDib, hPal, hOldPal = NULL;

	//����λͼ�ļ�ÿ��������ռ�ֽ���  
	//���㵱ǰϵͳ����ɫλ��1 4 8 24
	hDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL)*GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	wBitCount = 32;

	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;

	dwBmBitsSize = Bitmap.bmWidth * 4 * Bitmap.bmHeight;

	//Ϊλͼ���ݷ����ڴ�                
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	//     �����ɫ��                    
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	//     ��ȡ�õ�ɫ�����µ�����ֵ                
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
		(BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	//�ָ���ɫ��                    
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	//����λͼ�ļ�                    
	fh = CreateFile(szfilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)         return     FALSE;

	//     ����λͼ�ļ�ͷ                
	bmfHdr.bfType = 0x4D42;     //     "BM"           �����ļ�����     
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//     д��λͼ�ļ�ͷ                
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//     д��λͼ�ļ���������                
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//���                    
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return     TRUE;

}



//��ͼ����
bool Dodge_Handling(HWND hWnd, long nx, long ny, long nWidth, long nHeight, HBITMAP pRGBBuf)
{
	bool is_ok;
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, nWidth, nHeight);			//������DC���ݵļ���λͼ
	HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);							//ѡ�����DC�У����滻ԭ�ж���
	BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcSrc, nx, ny, SRCCOPY);				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������

	is_ok = BitmapToBitmap32(hBitmap, pRGBBuf);

	DeleteObject(hBitmap);

	return is_ok;
}

//��������ͼƬ
void Save_Bmp()
{
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top);			//������DC���ݵļ���λͼ

	HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);							//ѡ�����DC�У����滻ԭ�ж���																				//����ͼ��DC��
	BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY);				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������

	SaveBitmapToFile32(hBitmap, L"image//tmp.bmp");

	DeleteObject(hBitmap);
}

//���沿������ͼƬ
void Save_Bmp(long nx, long ny, long nWidth, long nHeight, LPCWSTR szfilename)
{
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, nWidth, nHeight);			//������DC���ݵļ���λͼ
	HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);							//ѡ�����DC�У����滻ԭ�ж���,��HGDIOBJ��hdcMem DC��
	BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcSrc, nx, ny, SRCCOPY);				//��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������


//	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, L"image//tmp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	SaveBitmapToFile32(hBitmap, szfilename);
	DeleteObject(hBitmap);
}



DWORD WINAPI FindThreadProc()
{
	HBITMAP ZiTu = NULL;
	int Status = 2;

	while (true)
	{
		switch (Status)
		{
		case Find_Level:
			ZiTu = (HBITMAP)LoadImage(NULL, L"image//Find_Level.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			break;
		case Challenge:
			ZiTu = (HBITMAP)LoadImage(NULL, L"image//tiansuo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			break;
		case Mobs:
			ZiTu = (HBITMAP)LoadImage(NULL, L"image//xioagua.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			break;
		default:
			break;
		}

		if (Dodge_Handling(GameMianHend, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, ZiTu))
		{
			Status++;
		}
		Sleep(3000);
	}

}

void Game_Start()
{
	static int Is_Start = 1;
	if (Is_Start)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)FindThreadProc, nullptr, 0, 0);	//������Ϸ����߳�
		Is_Start = 0;
	}
	else
	{
		MessageBoxW(NULL, L"��Ϸ�ѿ�ʼ", NULL, MB_OK);
	}
	
}

//��ʼ����������Դ
void Game_Init()
{
	HWND Main_From = FindWindow(L"Qt5QWindowIcon", nullptr);
	HWND Ci_From = FindWindowEx(Main_From, nullptr, L"Qt5QWindowIcon", L"m_pMainWidgetWindow");
	HWND CiCi_From = FindWindowEx(Ci_From, nullptr, L"Qt5QWindowIcon", L"m_pViewWidgetWindow");
	HWND CiCiCi_From = FindWindowEx(CiCi_From, nullptr, L"BlueStacksApp", L"_ctl.Window");

	if (CiCiCi_From == NULL)
	{
		MessageBoxW(NULL, L" ����ʦδ���У�", NULL, MB_OK);
		return;
	}

	//��ȡ��Ϸ���ڴ�С
	GetWindowRect(CiCiCi_From, &lpRect);

	GameMianHend = CiCiCi_From;		//������

	GameMoNiHend = CiCi_From;		//Windows�ⲿ�ӿ�

	hdcSrc = GetDC(GameMianHend);													//��ȡ����������Ļ���������DC
	hdcMem = CreateCompatibleDC(hdcSrc);										//��������DC
}

//��Ϸ�˳����ͷ���Դ
void Game_Exit()
{
	DeleteObject(hdcMem);

	ReleaseDC(GameMianHend, hdcSrc);  //����Ҫ�ͷž�� ��Ȼ�ᵼ���´ε���ʧ��
	DeleteObject(hdcSrc);
}