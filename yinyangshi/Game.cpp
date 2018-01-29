#include "stdafx.h"

#include "game.h"

#include "shibie.h"

HWND GameMianHend = NULL;		//游戏主界面

HWND GameMoNiHend = NULL;		//模拟鼠标接口
RECT lpRect;					//游戏界面大小


HDC hdcSrc, hdcMem;			//缓冲DC

int start_x, start_y, end_x, end_y;		//找到位置的坐标


HBITMAP hIMG_Buff;						//缓冲图像数据对象


int bit_err = 0;


//模拟鼠标处理
void momvoerClock()
{
	LPARAM zuobiao = start_y;
	zuobiao = zuobiao << 16;
	zuobiao += start_x;
	SendMessage(GameMoNiHend, WM_LBUTTONDOWN, MK_LBUTTON, zuobiao);
	SendMessage(GameMoNiHend, WM_LBUTTONUP, 0, zuobiao);

	char A[50];
	sprintf_s(A, "A %d,%d \r", start_x, start_y);
	OutputDebugStringA(A);
}

void momvoerClock(int x, int y)
{
	LPARAM zuobiao = y;
	zuobiao = zuobiao << 16;
	zuobiao += x;
	SendMessage(GameMoNiHend, WM_LBUTTONDOWN, MK_LBUTTON, zuobiao);
	SendMessage(GameMoNiHend, WM_LBUTTONUP, 0, zuobiao);

	char A[50];
	sprintf_s(A, "B %d,%d \r", x, y);
	OutputDebugStringA(A);
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
	bi.biSizeImage = Bitmap.bmWidth * 4 * Bitmap.bmHeight;
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
		(LPSTR)lpbi1 + sizeof(BITMAPINFOHEADER),
		(BITMAPINFO *)lpbi1, DIB_RGB_COLORS);


	//第2张图
	GetObject(hBitmap2, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = Bitmap.bmWidth * 4 * Bitmap.bmHeight;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;

	dwBmBitsSize2 = Bitmap.bmWidth * 4 * Bitmap.bmHeight;

	//为位图内容分配内存                
	hDib2 = GlobalAlloc(GHND, dwBmBitsSize2 + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi2 = (LPBITMAPINFOHEADER)GlobalLock(hDib2);
	*lpbi2 = bi;

	////     获取该调色板下新的像素值                
	GetDIBits(hdcMem, hBitmap2, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi2 + sizeof(BITMAPINFOHEADER),
		(BITMAPINFO *)lpbi2, DIB_RGB_COLORS);

	//用long类型进行比较 一次可以比较一个像素 这里是24位 所以是3个字节
	COLORREF * a, *b;
	int Y = 0;  //Y轴，每一次的Y轴应该都是一样的

	char *  tmp1, *tmp2;
	tmp1 = (char *)lpbi1 + 40;			//从文件头开始
	tmp2 = (char *)lpbi2 + 40;

	//因为图片内存buf存储字节是逆序的，由
	a = (COLORREF *)tmp1;
	b = (COLORREF *)tmp2;


	DWORD i = 0, j = 0, z = 0;
	dwBmBitsSize1 = dwBmBitsSize1 / 4;
	while (i < dwBmBitsSize1)
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
			//if (j > 3)
			//{
			//	momvoerClock();
			//	return true;
			//}

		}
		else
		{
			if (j < 3)
			{
				j = 0;
				//if(Y == 0 ) b = (COLORREF *)tmp2;
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

	HBITMAP * P_hBitmap = &hBitmap;

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
	bi.biSizeImage = Bitmap.bmWidth * 4 * Bitmap.bmHeight;
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
	//hPal = GetStockObject(DEFAULT_PALETTE);
	//if (hPal)
	//{
	//	hDC = ::GetDC(NULL);
	//	hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
	//	RealizePalette(hDC);
	//}

	////     获取该调色板下新的像素值                
	//GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
	//	(LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
	//	(BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	////恢复调色板                    
	//if (hOldPal)
	//{
	//	::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
	//	RealizePalette(hDC);
	//	::ReleaseDC(NULL, hDC);
	//}

	GetDIBits(hdcMem, hBitmap, 0, (UINT)Bitmap.bmHeight,		//扫描高度
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER),				//数据缓存指针
		(BITMAPINFO *)lpbi, DIB_RGB_COLORS);				//数据类型
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
	WriteFile(fh, (LPSTR)lpbi, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
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
///	BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcSrc, nx, ny, SRCCOPY);				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	{
		bit_err++;
	}

	is_ok = BitmapToBitmap32(hBitmap, pRGBBuf);

	DeleteObject(hBitmap);

	return is_ok;
}

//搜图处理
bool Dodge_Handling(HBITMAP pRGBBuf)
{
	bool is_ok;
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top);			//创建于DC兼容的兼容位图
	HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);							//选择对象到DC中（会替换原有对象）
	//BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY);				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	{
		bit_err++;
	}

	is_ok = BitmapToBitmap32(hBitmap, pRGBBuf);

	DeleteObject(hBitmap);

	return is_ok;
}

//保存整个图片
void Save_Bmp()
{
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top);			//创建于DC兼容的兼容位图

	HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);							//选择对象到DC中（会替换原有对象）																				//拷贝图像到DC上
	//BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY);				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	{
		bit_err++;
	}

	SaveBitmapToFile32(hBitmap, L"image//tmp.bmp");

	DeleteObject(hBitmap);
}

//保存部分整个图片
void Save_Bmp(long nx, long ny, long nWidth, long nHeight, LPCWSTR szfilename)
{
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, nWidth, nHeight);			//创建于DC兼容的兼容位图
	HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);							//选择对象到DC中（会替换原有对象）,绑定HGDIOBJ到hdcMem DC上
	//BitBlt(hdcMem, 0, 0, nWidth, nHeight, hdcSrc, nx, ny, SRCCOPY);				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	{
		bit_err++;
	}

//	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, L"image//tmp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	SaveBitmapToFile32(hBitmap, szfilename);
	DeleteObject(hBitmap);
}


////寻找怪物 并击杀
//void ShaGuai(char * imgtemp)
//{
//	cv::Point Addr;
//
//	//只寻找3次
//	//for (int i = 0; i < 3; i++)
//	//{
//		Save_Bmp();		//保存源图片 作为缓冲
//		Addr = ShiBie("image//tmp.bmp", imgtemp, 0);
//
//		//判断是否合法
//		if (Addr.y > 150 )
//		{
//			momvoerClock(Addr.x, Addr.y);	//攻击
//		}
//		
//		Sleep(200);
////	}
//}



//DWORD WINAPI FindThreadProc()
//{
//	HBITMAP ZiTu = NULL;
//	int Status = Challenge;
//
//	int xiaoguai_num = 0;
//	int xiaoguai_time = 0;
//
//	while (true)
//	{
//		switch (Status)
//		{
//		case Find_Level:
//			ZiTu = (HBITMAP)LoadImage(NULL, L"image//Find_Level.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//			break;
//		case Challenge:
//			ZiTu = (HBITMAP)LoadImage(NULL, L"image//tansuo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//			xiaoguai_num = 0;	//每个探索一般是6只小怪
//			xiaoguai_time = 0;
//			break;
//		case Mobs:
//			//寻找小怪 并攻击小怪
//			ShaGuai("image//Mobs.bmp");
//			//查看是否结束
//			ZiTu = (HBITMAP)LoadImage(NULL, L"image//Judgment_ends.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//			xiaoguai_time++;
//			//当60s 还没有找到一只怪 强制位移一段距离
//			if (xiaoguai_time > 20)
//			{
//				momvoerClock(900, 450);
//			}
//			break;
//		case Boss:
//			ZiTu = (HBITMAP)LoadImage(NULL, L"image//Boss.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//			break;
//		default:
//			break;
//		}
//
//		if (Dodge_Handling(GameMianHend, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, ZiTu))
//		{
//			if (Status == Mobs)
//			{
//				xiaoguai_num++;
//				xiaoguai_time = 0;
//				if (xiaoguai_num == 5)
//				{
//					Status = Boss;
//				}
//			}
//			else
//			{
//				Status++;
//			}
//			
//		}
//		Sleep(3000);
//	}
//
//}

////击杀一次怪物
//Rea 是否需要准备
int JiSha(bool Rea ,int TIME)
{
	//cv::Point Addr;
	HBITMAP ZiTu = (HBITMAP)LoadImage(NULL, L"image//Judgment_ends.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		for (int i = 0; i < TIME; i++)
		{
			//Save_Bmp();		//保存源图片 作为缓冲
			//Addr = ShiBie("image//tmp.bmp", "image//Judgment_ends.bmp", 0);
	
			////判断是否合法
			//if (Addr.x >250 && Addr.y > 250)
			//{
			//	momvoerClock(Addr.x, Addr.y);	//攻击
			//}
			
			if (Dodge_Handling(GameMianHend, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, ZiTu))
			{
				//快速重复点击两次 防止失败 （好像结束摇的的时候点击无效）
				Sleep(500);
				momvoerClock();
				Sleep(100);
				momvoerClock();
				return 1;
			}
			Sleep(1000);
		}
		return 0;
}


////获取纸人奖励
int Paper_Man_Set()
{
	cv::Point Addr;
	int obj = 0;

Start:	

	Addr = ShiBie(hIMG_Buff, "image//Paper_Man.bmp", 0);

	//判断是否合法 纸人大致范围
	if (Addr.y >280 && Addr.y < 450)
	{
		momvoerClock(Addr.x, Addr.y);
		Sleep(300);
		momvoerClock(500, 450);		//需要连续两次点击才算完成 第2次不能点击中心..... 不然不算 坑爹
		Sleep(800);

		SelectObject(hdcMem, hIMG_Buff);							//选择对象到DC中（会替换原有对象）																				//拷贝图像到DC上
		//BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY);				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
		if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
		{
			bit_err++;
		}

		obj = 1;
		goto Start;		//不知道具体数量 这里无限循环 失败退出
	}
	else
	{
		return obj;
	}
}

//游戏状态
bool Game_Start_State = true;

//判断是否游戏结束
void Game_Over()
{
	//查看觉醒标志 只有在地图上才会有
	cv::Point Addr;


	//Sleep(4000);	//延时4S 等待页面跳转
	SelectObject(hdcMem, hIMG_Buff);							//选择对象到DC中（会替换原有对象）																				//拷贝图像到DC上

	//BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY);				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	{
		bit_err++;
	}

	Addr = ShiBie(hIMG_Buff, "image//juexing.bmp", 0);
	//只有这个位置才是觉醒图标点
	if (Addr.x > 40  && Addr.x < 65 && Addr.y > 490 && Addr.y < 520)		//55 505
	{
		Game_Start_State = false;
	}
}


DWORD WINAPI FindThreadProc()
{
	char * ZiTu = NULL;
	int Status = Find_Level;
	cv::Point Addr;

	int xiaoguai_num = 0;


	while (true)
	{
//		Save_Bmp();		//保存源图片 作为缓冲

		SelectObject(hdcMem, hIMG_Buff);							//选择对象到DC中（会替换原有对象）				//拷贝图像到DC上

		if (BitBlt(hdcMem, 0, 0, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top, hdcSrc, 0, 0, SRCCOPY) == false)				//对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
		{
			bit_err++;
		}
		switch (Status)
		{
		case Find_Level:
			//ZiTu = "image//Find_Level.bmp";
			//Addr = ShiBie("image//tmp.bmp", ZiTu, 3);
			//if ((Addr.x == 0 && Addr.y == 0) || Addr.x < 850) //判断是否合法
			//{
			//	Status = Challenge;
			//	break;
			//}
			//momvoerClock(Addr.x, Addr.y);
			momvoerClock(500, 250);	//点击屏幕中心
			Sleep(1000);
			momvoerClock(500, 250);	//点击屏幕中心
			Status = Challenge;
			Game_Start_State = true;
			Sleep(1000);
			break;
		case Challenge:
			ZiTu = "image//tansuo.bmp";
			Addr = ShiBie(hIMG_Buff, ZiTu, 0);
			if (Addr.x > 700 && Addr.x < 750 && Addr.y > 400 && Addr.y < 420)	//703 412
			{
				momvoerClock(Addr.x, Addr.y);
				Status = Mobs;
				xiaoguai_num = 0;
			}
			Sleep(7000);		//等这几把BB完了再寻怪
			break;
		case Mobs:		
			if (Game_Start_State == false)
			{
				Status = Find_Level;
				break;
			}

			ZiTu = "image//Mobs.bmp";
			Addr = ShiBie(hIMG_Buff, ZiTu, 5);
			if ((Addr.x == 0 && Addr.y == 0) || Addr.y < 150 || Addr.y > 480)		//判断是否合法
			{
				//if (xiaoguai_num > 2)		//小怪被杀光
				if(Paper_Man_Set())
				{
					//Status = Boss;
					//Paper_Man_Set();
					//Status = Find_Level;
					//break;
				}
				else						//还存在小怪
				{
					if (xiaoguai_num < 3)
					{
						momvoerClock(900, 450);	//右移动一段距离
					}
					else 
					{
						momvoerClock(100, 450);	//左移动一段距离
					}					
					xiaoguai_num = (xiaoguai_num > 6)?0:xiaoguai_num++;
				}
			}
			else
			{
				momvoerClock(Addr.x, Addr.y);								

				//等待击杀完成
				JiSha(1, 100);
			}
			Sleep(9000);
			Game_Over();		//判断是否结束
			break;
		default:
			break;
		}				
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
int Game_Init()
{
	HWND Main_From = FindWindow(L"Qt5QWindowIcon", nullptr);
	HWND Ci_From = FindWindowEx(Main_From, nullptr, L"Qt5QWindowIcon", L"m_pMainWidgetWindow");
	HWND CiCi_From = FindWindowEx(Ci_From, nullptr, L"Qt5QWindowIcon", L"m_pViewWidgetWindow");
	HWND CiCiCi_From = FindWindowEx(CiCi_From, nullptr, L"BlueStacksApp", L"_ctl.Window");

	if (CiCiCi_From == NULL)
	{
		MessageBoxW(NULL, L" 阴阳师未运行！", NULL, MB_OK);
		return false;
	}

	//获取游戏窗口大小
	GetWindowRect(CiCiCi_From, &lpRect);

	GameMianHend = CiCiCi_From;		//主界面

	GameMoNiHend = CiCi_From;		//Windows外部接口

	hdcSrc = GetDC(GameMianHend);													//获取句柄的上下文环境来创建DC
	hdcMem = CreateCompatibleDC(hdcSrc);										//创建兼容DC

	hIMG_Buff = CreateCompatibleBitmap(hdcSrc, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top);			//创建于DC兼容的兼容位图


	return  true;
}

//游戏退出，释放资源
void Game_Exit()
{
	DeleteObject(hdcMem);

	ReleaseDC(GameMianHend, hdcSrc);  //必须要释放句柄 不然会导致下次调用失败
	DeleteObject(hdcSrc);
	DeleteObject(hIMG_Buff);
}