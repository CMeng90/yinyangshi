#include "stdafx.h"
#include "shibie.h"

using namespace cv;



//查找源图像中模板图像所在的位置
///Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED
Point ShiBie(char * img_name, char * templ_name, int match_method)
{
	Mat img; Mat templ; Mat result;

	/// Load image and template  载入图像
	img = imread(img_name);
	templ = imread(templ_name);

	/// Source image to display   创建原图像副本
	Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix	创建将存储每个模板位置的匹配结果的结果矩阵。 详细观察结果矩阵的大小（与之匹配的所有可能位置）
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);

	//matchTemplate  源图像 模板图像 结果矩阵 匹配幅度 
	matchTemplate(img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc  使用minMaxLoc定位结果矩阵R中的最小值和最大值 找到最佳匹配
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc; double matchVal;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	//对于SQDIFF和SQDIFF_NORMED，最佳匹配是较低的值。对于所有其他方法，越高越好
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
		matchVal = minVal;
	}
	else
	{
		matchLoc = maxLoc;
		matchVal = maxVal;
	}
	//使用3: TM CCORR NORMED 方法可以判断出是否存在图片 
	if (matchVal != 1 && match_method == 3)
	{
		matchLoc.x = matchLoc.y = 0;
	}
	return matchLoc;
}


uchar ShiBie_Buff[568 * 1005 * 4];

//查找源图像中模板图像所在的位置
///Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED
Point ShiBie(HBITMAP Img_Bitmap, char * templ_name, int match_method)
{
	Mat img; Mat templ; Mat result;

	/// Load image and template  载入图像
	HBITMAP Templ_Bitmap;
	BITMAP bmp;
	int nChannels;

	GetObject(Img_Bitmap, sizeof(BITMAP), &bmp);

	nChannels = bmp.bmBitsPixel / 8;
	GetBitmapBits(Img_Bitmap, bmp.bmHeight*bmp.bmWidth * nChannels, ShiBie_Buff);

	img.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, 3));

	int j = 0;
	for (size_t i = 0; i < bmp.bmHeight*bmp.bmWidth * 4; i++)
	{
		if ((i + 1) % 4 != 0)
		{
			img.data[j++] = ShiBie_Buff[i];
		}

	}


	templ = imread(templ_name);
	//Templ_Bitmap = (HBITMAP)LoadImage(NULL, templ_name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//GetObject(Templ_Bitmap, sizeof(BITMAP), &bmp);

	//templ.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));
	//GetBitmapBits(Templ_Bitmap, bmp.bmHeight*bmp.bmWidth * nChannels, templ.data);


	/// Source image to display   创建原图像副本
	Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix	创建将存储每个模板位置的匹配结果的结果矩阵。 详细观察结果矩阵的大小（与之匹配的所有可能位置）
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);

	//matchTemplate  源图像 模板图像 结果矩阵 匹配幅度 
	matchTemplate(img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc  使用minMaxLoc定位结果矩阵R中的最小值和最大值 找到最佳匹配
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc; double matchVal;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	//对于SQDIFF和SQDIFF_NORMED，最佳匹配是较低的值。对于所有其他方法，越高越好
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
		matchVal = minVal;
	}
	else
	{
		matchLoc = maxLoc;
		matchVal = maxVal;
	}
	//使用3: TM CCORR NORMED 方法可以判断出是否存在图片 
	if (matchVal != 1 && match_method == 3)
	{
		matchLoc.x = matchLoc.y = 0;
	}
	return matchLoc;
}


