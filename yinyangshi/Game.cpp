#include "stdafx.h"


HWND GameMianHend = NULL;		//游戏主界面

HWND GameMoNiHend = NULL;		//模拟鼠标接口
RECT lpRect;					//游戏界面大小


HDC hdcSrc, hdcMem;			//缓冲DC

int start_x, start_y, end_x, end_y;		//找到位置的坐标

//游戏状态
enum Game_Status
{
	Find_Level,				//寻找关卡
	Challenge,				//选择挑战
	Mobs,					//寻找小怪
	Judgment_ends,			//判断结束
	Boss,					//寻找BOOSS
	Paper_Man				//寻找结束纸人
};


//模拟鼠标处理
void momvoerClock()
{
	LPARAM zuobiao = start_y;
	zuobiao = zuobiao << 16;
	zuobiao += start_x;
	SendMessage(GameMoNiHend, WM_LBUTTONDOWN, MK_LBUTTON, zuobiao);
	SendMessage(GameMoNiHend, WM_LBUTTONUP, 0, zuobiao);
}



// 图片转到内存数据中
//	hBitmap		图片句柄
//  lpbi		指向位图信息头结构     
BOOL BitmapToBitmap32(HBITMAP   hBitmap1, HBITMAP hBitmap2)
{
	HDC     hDC;
	//当前分辨率下每象素所占字节数            
	int     iBits;
	//位图中每象素所占字节数            
	WORD     wBitCount;
	//定义调色板大小，     位图中像素字节大小     ，位图文件大小     ，     写入文件字节数                
	DWORD     dwPaletteSize = 0, dwBmBitsSize1 = 0, dwBmBitsSize2 = 0, dwDIBSize = 0, dwWritten = 0;
	//位图属性结构                
	BITMAP     Bitmap;
	//位图文件头结构            
	BITMAPFILEHEADER     bmfHdr;
	//指向位图信息头结构                    
	LPBITMAPINFOHEADER     lpbi1, lpbi2;
	//位图信息头结构                
	BITMAPINFOHEADER     bi;

	//定义文件，分配内存句柄，调色板句柄                
	HANDLE     hDib1, hDib2, hOldPal = NULL;

	//计算位图文件每个像素所占字节数  
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

	// 24位颜色深度计算大小 宽度存储的大小 必须要4的倍数 
	//（图片宽度 * 位数 + 31）/32 表示如果颜色大小位数不为32的倍数 多一位都会增加一个long大小（因为计算机里面没有四舍五入），如果刚刚好，则不会多一个long大小
	// 所以24位像素大小的图片真实大小应该为 如果bmWidth*3 为4的整数，则size = bmWidth*3*bmHeight，否则size = ((Bitmap.bmWidth *wBitCount + 31) / 32) * 4 * Bitmap.bmHeight
	dwBmBitsSize1 = Bitmap.bmWidth * 4 * Bitmap.bmHeight;



	//为位图内容分配内存                
	hDib1 = GlobalAlloc(GHND, dwBmBitsSize1 + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi1 = (LPBITMAPINFOHEADER)GlobalLock(hDib1);
	*lpbi1 = bi;

	//     获取该调色板下新的像素值                
	GetDIBits(hdcMem, hBitmap1, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi1 + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
		(BITMAPINFO *)lpbi1, DIB_RGB_COLORS);


	//第2张图
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

	//为位图内容分配内存                
	hDib2 = GlobalAlloc(GHND, dwBmBitsSize2 + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi2 = (LPBITMAPINFOHEADER)GlobalLock(hDib2);
	*lpbi2 = bi;

	//     获取该调色板下新的像素值                
	GetDIBits(hdcMem, hBitmap2, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi2 + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
		(BITMAPINFO *)lpbi2, DIB_RGB_COLORS);

	//用long类型进行比较 一次可以比较一个像素 这里是24位 所以是3个字节
	COLORREF * a, *b;
	int Y = 0;  //Y轴，每一次的Y轴应该都是一样的

	char *  tmp1, *tmp2;
	tmp1 = (char *)lpbi1 + 54;
	tmp2 = (char *)lpbi2 + 54;

	//因为图片内存buf存储字节是逆序的，由
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


	//清除                    
	GlobalUnlock(hDib1);
	GlobalFree(hDib1);
	GlobalUnlock(hDib2);
	GlobalFree(hDib2);
	return  false;

}


//把RGB转存为32位BMP图片
BOOL SaveBitmapToFile32(HBITMAP   hBitmap, LPCWSTR szfilename)
{
	HDC     hDC;
	//当前分辨率下每象素所占字节数            
	int     iBits;
	//位图中每象素所占字节数            
	WORD     wBitCount;
	//定义调色板大小，     位图中像素字节大小     ，位图文件大小     ，     写入文件字节数                
	DWORD     dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	//位图属性结构                
	BITMAP     Bitmap;
	//位图文件头结构            
	BITMAPFILEHEADER     bmfHdr;
	//位图信息头结构                
	BITMAPINFOHEADER     bi;
	//指向位图信息头结构                    
	LPBITMAPINFOHEADER     lpbi;
	//定义文件，分配内存句柄，调色板句柄                
	HANDLE     fh, hDib, hPal, hOldPal = NULL;

	//计算位图文件每个像素所占字节数  
	//计算当前系统的颜色位数1 4 8 24
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

	//为位图内容分配内存                
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	//     处理调色板                    
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	//     获取该调色板下新的像素值                
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
		(BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	//恢复调色板                    
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	//创建位图文件                    
	fh = CreateFile(szfilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)         return     FALSE;

	//     设置位图文件头                
	bmfHdr.bfType = 0x4D42;     //     "BM"           设置文件类型     
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//     写入位图文件头                
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//     写入位图文件其余内容                
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//清除                    
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return     TRUE;

}



//搜图处理
bool Dodge_Handling(HWND hWnd, long nx, long ny, long nWidth, long nHeight, HBITMAP pRGBBuf)
{
	bool is_ok;
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, nWidth, nHeight);			//创建于DC兼容的兼容位图
	HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);							//选择对象到DC中（会替换原有对象）
	BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcSrc, nx, ny, SRCCOPY);				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。

	is_ok = BitmapToBitmap32(hBitmap, pRGBBuf);

	DeleteObject(hBitmap);

	return is_ok;
}

//保存整个图片
void Save_Bmp()
{
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top);			//创建于DC兼容的兼容位图

	HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);							//选择对象到DC中（会替换原有对象）																				//拷贝图像到DC上
	BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY);				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。

	SaveBitmapToFile32(hBitmap, L"image//tmp.bmp");

	DeleteObject(hBitmap);
}

//保存部分整个图片
void Save_Bmp(long nx, long ny, long nWidth, long nHeight, LPCWSTR szfilename)
{
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, nWidth, nHeight);			//创建于DC兼容的兼容位图
	HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);							//选择对象到DC中（会替换原有对象）,绑定HGDIOBJ到hdcMem DC上
	BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcSrc, nx, ny, SRCCOPY);				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。


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
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)FindThreadProc, nullptr, 0, 0);	//创建游戏外挂线程
		Is_Start = 0;
	}
	else
	{
		MessageBoxW(NULL, L"游戏已开始", NULL, MB_OK);
	}
	
}

//初始化，加载资源
void Game_Init()
{
	HWND Main_From = FindWindow(L"Qt5QWindowIcon", nullptr);
	HWND Ci_From = FindWindowEx(Main_From, nullptr, L"Qt5QWindowIcon", L"m_pMainWidgetWindow");
	HWND CiCi_From = FindWindowEx(Ci_From, nullptr, L"Qt5QWindowIcon", L"m_pViewWidgetWindow");
	HWND CiCiCi_From = FindWindowEx(CiCi_From, nullptr, L"BlueStacksApp", L"_ctl.Window");

	if (CiCiCi_From == NULL)
	{
		MessageBoxW(NULL, L" 阴阳师未运行！", NULL, MB_OK);
		return;
	}

	//获取游戏窗口大小
	GetWindowRect(CiCiCi_From, &lpRect);

	GameMianHend = CiCiCi_From;		//主界面

	GameMoNiHend = CiCi_From;		//Windows外部接口

	hdcSrc = GetDC(GameMianHend);													//获取句柄的上下文环境来创建DC
	hdcMem = CreateCompatibleDC(hdcSrc);										//创建兼容DC
}

//游戏退出，释放资源
void Game_Exit()
{
	DeleteObject(hdcMem);

	ReleaseDC(GameMianHend, hdcSrc);  //必须要释放句柄 不然会导致下次调用失败
	DeleteObject(hdcSrc);
}