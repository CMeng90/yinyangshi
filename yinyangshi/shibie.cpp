#include "stdafx.h"
#include "shibie.h"

using namespace cv;



//����Դͼ����ģ��ͼ�����ڵ�λ��
///Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED
Point ShiBie(char * img_name, char * templ_name, int match_method)
{
	Mat img; Mat templ; Mat result;

	/// Load image and template  ����ͼ��
	img = imread(img_name);
	templ = imread(templ_name);

	/// Source image to display   ����ԭͼ�񸱱�
	Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix	�������洢ÿ��ģ��λ�õ�ƥ�����Ľ������ ��ϸ�۲�������Ĵ�С����֮ƥ������п���λ�ã�
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);

	//matchTemplate  Դͼ�� ģ��ͼ�� ������� ƥ����� 
	matchTemplate(img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc  ʹ��minMaxLoc��λ�������R�е���Сֵ�����ֵ �ҵ����ƥ��
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc; double matchVal;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	//����SQDIFF��SQDIFF_NORMED�����ƥ���ǽϵ͵�ֵ��������������������Խ��Խ��
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
	//ʹ��3: TM CCORR NORMED ���������жϳ��Ƿ����ͼƬ 
	if (matchVal != 1 && match_method == 3)
	{
		matchLoc.x = matchLoc.y = 0;
	}
	return matchLoc;
}


uchar ShiBie_Buff[568 * 1005 * 4];

//����Դͼ����ģ��ͼ�����ڵ�λ��
///Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED
Point ShiBie(HBITMAP Img_Bitmap, char * templ_name, int match_method)
{
	Mat img; Mat templ; Mat result;

	/// Load image and template  ����ͼ��
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


	/// Source image to display   ����ԭͼ�񸱱�
	Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix	�������洢ÿ��ģ��λ�õ�ƥ�����Ľ������ ��ϸ�۲�������Ĵ�С����֮ƥ������п���λ�ã�
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);

	//matchTemplate  Դͼ�� ģ��ͼ�� ������� ƥ����� 
	matchTemplate(img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc  ʹ��minMaxLoc��λ�������R�е���Сֵ�����ֵ �ҵ����ƥ��
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc; double matchVal;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	//����SQDIFF��SQDIFF_NORMED�����ƥ���ǽϵ͵�ֵ��������������������Խ��Խ��
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
	//ʹ��3: TM CCORR NORMED ���������жϳ��Ƿ����ͼƬ 
	if (matchVal != 1 && match_method == 3)
	{
		matchLoc.x = matchLoc.y = 0;
	}
	return matchLoc;
}


