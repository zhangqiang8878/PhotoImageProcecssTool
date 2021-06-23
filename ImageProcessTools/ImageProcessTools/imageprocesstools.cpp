#include "imageprocesstools.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <leptonica/allheaders.h>
#include <Windows.h>

#define EP 1e-4
#define PI 3.1415926
#define GRAY_THRESH 190

using namespace cv;
using namespace std;

QSettings setting("./Setting.ini", QSettings::IniFormat);          //为了能记住上次打开的路径
QString lastPath = setting.value("LastFilePath").toString();
tesseract::TessBaseAPI ocr_tool;

//弹出窗口提示信息
void WarringDlg(QString text)
{
	QMessageBox warring;
	warring.setIcon(QMessageBox::Information);
	warring.setText(text);
	warring.setWindowTitle(u8"提示");
	warring.addButton(u8"确定", QMessageBox::YesRole);
	warring.exec();
}

//弹出窗口提示信息
int MessageCheckDlg(QString text, bool timeout = false)
{
	QMessageBox warring;
	warring.setIcon(QMessageBox::Information);
	warring.setText(text);
	warring.setWindowTitle(u8"提示");
	QPushButton *confirmButton = warring.addButton(u8"上", QMessageBox::YesRole);
	QPushButton *cancelButton = warring.addButton(u8"下", QMessageBox::NoRole);

	if (timeout)
	{
		warring.show();
		qApp->processEvents();
		int timeSecond = 60;
		while (true)
		{
			confirmButton->setText(QString(u8"是(%1)").arg(--timeSecond / 10));
			warring.show();
			qApp->processEvents();
			if (warring.clickedButton() == confirmButton) {
				return 1;
			}
			else if (warring.clickedButton() == cancelButton) {
				return 0;
			}
			if (!timeSecond)
				break;
			Sleep(100);
		}
	}
	else
	{
		warring.exec();
		if (warring.clickedButton() == confirmButton)
			return 1;
		else
			return 0;
	}
	return 0;
}

//OpenCV转QImage
QImage opencvMatToQImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS=1
	if (mat.type() == CV_8UC1)
	{
		// Set the color table (used to translate color indexes to qRgb values)
		QVector<QRgb> colorTable;
		for (int i = 0; i<256; i++)
			colorTable.push_back(qRgb(i, i, i));

		// Copy input Mat
		const uchar *qImageBuffer = (const uchar*)mat.data;

		// Create QImage with same dimensions as input Mat
		QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
		img.setColorTable(colorTable);
		return img;
	}
	else if (mat.type() == CV_16UC1)
	{
		cv::Mat ucharMatScaled;
		cv::Mat ushortMatScaled;
		cv::Mat floatMatScaled;
		double minImage, maxImage;
		minMaxLoc(mat, &minImage, &maxImage);
		mat.convertTo(floatMatScaled, CV_32FC1);

		// to ensure [0-1.0]
		floatMatScaled = (floatMatScaled - minImage) / (maxImage - minImage);
		floatMatScaled.convertTo(ucharMatScaled, CV_8UC1, 255, 0);
		return opencvMatToQImage(ucharMatScaled);
	}
	else if (mat.type() == CV_32FC1)
	{
		cv::Mat ucharMatScaled;
		cv::Mat floatMatScaled;
		double minImage, maxImage;
		minMaxLoc(mat, &minImage, &maxImage);

		// to ensure [0-1.0]
		floatMatScaled = (mat - minImage) / (maxImage - minImage);
		floatMatScaled.convertTo(ucharMatScaled, CV_8UC1, 255, 0);

		return opencvMatToQImage(ucharMatScaled);
	}
	else if (mat.type() == CV_32FC3)
	{
		cv::Mat ucharMatScaled;
		cv::Mat floatMatScaled;
		double minImage, maxImage;
		minMaxLoc(mat, &minImage, &maxImage);

		normalize(mat, floatMatScaled, 0.0, 1.0, cv::NORM_MINMAX);
		pow(floatMatScaled, 1. / 5, floatMatScaled); // apply gamma curve: img = img ** (1./5)
		mat.convertTo(ucharMatScaled, CV_8UC3, 255, 0);

		qDebug() << "type ucharMatScaled error " << endl;

		return opencvMatToQImage(ucharMatScaled);
	}

	// 8-bits unsigned, NO. OF CHANNELS=3
	else
	{
		cv::Mat rgbMat;
		int qImageFormat = QImage::Format_RGB888;
		if (mat.type() == CV_8UC4)
		{
			qImageFormat = QImage::Format_ARGB32;
			rgbMat = mat;
		}
		else if (mat.type() == CV_8UC3)
		{
			rgbMat = mat;
		}
		else
		{
			cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
		}

		// Copy input Mat
		const uchar *qImageBuffer = (const uchar*)rgbMat.data;

		// Create QImage with same dimensions as input Mat
		QImage img(qImageBuffer, rgbMat.cols, rgbMat.rows, rgbMat.step, (QImage::Format) qImageFormat);
		return img.rgbSwapped();     // deep copy !!
									 //}
	}
}

//（全局）QImage 转换 cv::Mat
cv::Mat QImageToCVMat(QImage &image)
{
	if (image.isNull())
		return cv::Mat();

	cv::Mat mat, mat2;
	//void* buffertemp = (uchar*)image.bits();
	//灰度图转换
	if (image.format() == QImage::Format_Grayscale8 || image.format() == QImage::Format_Indexed8)
	{
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (uchar*)image.bits(), image.bytesPerLine());

		return mat.clone();
	}
	else if (image.format() == QImage::Format_RGB888)
	{
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (uchar*)image.bits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);

		return mat.clone();
	}
	else
	{
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (uchar*)image.bits(), image.bytesPerLine());
		mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3);
		int from_to[] = { 0,0,  1,1,  2,2 };
		mixChannels(&mat, 1, &mat2, 1, from_to, 3);
		if (image.format() == QImage::Format_RGBA8888)
		{
			cv::cvtColor(mat2, mat2, cv::COLOR_BGR2RGB);
		}
	}
	return mat2.clone();
}

//Mat转PIX 仅灰度
PIX* cvtMat2PIX(Mat& imgGray)
{
	int cols = imgGray.cols;
	int rows = imgGray.rows;

	PIX *pixS = pixCreate(cols, rows, 8);

	for (int y = 0; y < rows; y++)
		for (int x = 0; x < cols; x++)
			pixSetPixel(pixS, x, y, (l_uint32)imgGray.at<uchar>(y, x));
	return pixS;
}

//PIX转Mat 仅灰度
Mat PIX2cvMat(PIX* pix_input)
{
	Mat outputImage(Size(pix_input->w, pix_input->h), CV_8UC1);

	uint32_t *line = pix_input->data;
	for (uint32_t y = 0; y < pix_input->h; ++y) {
		for (uint32_t x = 0; x < pix_input->w; ++x) {
			outputImage.at<uchar>(y, x) = GET_DATA_BYTE(line, x);
		}
		line += pix_input->wpl;
	}
	return outputImage;
}

//lept图像增强处理
int leptThresh(Mat& imgGray, int method)
{
	//const char        *infile;
	l_int32      w, d, threshval, ival, newval;
	l_uint32     val;
	PIX         *pixs, *pixg, *pixg2;
	PIX         *pix1, *pix2, *pix8;
	//PIXA        *pixa;

	//infile = imagePath.c_str();

	//pixa = pixaCreate(5);
	//pixs = pixRead(infile);
	pixs = cvtMat2PIX(imgGray);
	pixGetDimensions(pixs, &w, NULL, &d);
	//pixSaveTiled(pixs, pixa, 1.0, 1, 50, 32);
	pixDisplay(pixs, 100, 0);

	switch (method)
	{
	case 1:
		/* 1. Standard background normalization with a global threshold.  */
		pixg = pixConvertTo8(pixs, 0);
		pix1 = pixBackgroundNorm(pixg, NULL, NULL, 20, 30, 10, 50, 255, 2, 2);

		//pixWrite("lept1-1.jpg", pixg, IFF_JFIF_JPEG);

		pix2 = pixThresholdToBinary(pix1, 160);
		//pixWrite("lept1.jpg", pix1, IFF_JFIF_JPEG);
		imgGray = PIX2cvMat(pix1).clone();
		//pixDisplay(pix2, 100, 0);
		//pixSaveTiled(pix2, pixa, 1.0, 1, 50, 32);
		pixDestroy(&pixg);
		pixDestroy(&pix1);
		pixDestroy(&pix2);
		break;
	case 2:
		/* 2. Background normalization followed by Otsu thresholding.  Otsu
		* binarization attempts to split the image into two roughly equal
		* sets of pixels, and it does a very poor job when there are large
		* amounts of dark background.  By doing a background normalization
		* first (to get the background near 255), we remove this problem.
		* Then we use a modified Otsu to estimate the best global
		* threshold on the normalized image.  */
		pixg = pixConvertTo8(pixs, 0);
		pix1 = pixOtsuThreshOnBackgroundNorm(pixg, NULL, 10, 15, 100,
			50, 255, 2, 2, 0.10, &threshval);
		//fprintf(stderr, "thresh val = %d\n", threshval);
		//pixSaveTiled(pix1, pixa, 1.0, 1, 50, 32);
		//pixWrite("lept2.jpg", pix1, IFF_JFIF_JPEG);
		pix8 = pixConvert1To8(NULL, pix1, 255, 0);
		imgGray = PIX2cvMat(pix8).clone();
		//pixDisplay(pix1, 100, 200);
		pixDestroy(&pixg);
		pixDestroy(&pix1);
		pixDestroy(&pix8);
		break;
	case 3:
		/* 3. Background normalization with Otsu threshold estimation and
		* masking for threshold selection.  */
		pixg = pixConvertTo8(pixs, 0);
		pix1 = pixMaskedThreshOnBackgroundNorm(pixg, NULL, 10, 15, 100,
			50, 2, 2, 0.10, &threshval);
		//fprintf(stderr, "thresh val = %d\n", threshval);
		//pixSaveTiled(pix1, pixa, 1.0, 1, 50, 32);
		//pixWrite("lept3.jpg", pix1, IFF_JFIF_JPEG);
		pix8 = pixConvert1To8(NULL, pix1, 255, 0);
		imgGray = PIX2cvMat(pix8).clone();
		//pixDisplay(pix1, 100, 400);
		pixDestroy(&pixg);
		pixDestroy(&pix1);
		pixDestroy(&pix8);
		break;
	case 4:
		/* 4. Background normalization followed by Sauvola binarization */
		if (d == 32)
			pixg = pixConvertRGBToGray(pixs, 0.2, 0.7, 0.1);
		else
			pixg = pixConvertTo8(pixs, 0);
		pixg2 = pixContrastNorm(NULL, pixg, 20, 20, 130, 2, 2);
		pixSauvolaBinarizeTiled(pixg2, 25, 0.40, 1, 1, NULL, &pix1);
		//pixSaveTiled(pix1, pixa, 1.0, 1, 50, 32);
		//pixWrite("lept4.jpg", pix1, IFF_JFIF_JPEG);
		pix8 = pixConvert1To8(NULL, pix1, 255, 0);
		imgGray = PIX2cvMat(pix8).clone();
		//pixDisplay(pix1, 100, 600);
		pixDestroy(&pixg);
		pixDestroy(&pixg2);
		pixDestroy(&pix1);
		pixDestroy(&pix8);
		break;
	case 5:
		/* 5. Contrast normalization followed by background normalization, and
		* thresholding. */
		if (d == 32)
			pixg = pixConvertRGBToGray(pixs, 0.2, 0.7, 0.1);
		else
			pixg = pixConvertTo8(pixs, 0);

		pixOtsuAdaptiveThreshold(pixg, 5000, 5000, 0, 0, 0.1, &pix1, NULL);

		pixGetPixel(pix1, 0, 0, &val);
		ival = (l_int32)val;
		newval = ival + (l_int32)(0.6 * (110 - ival));
		fprintf(stderr, "th1 = %d, th2 = %d\n", ival, newval);
		pixDestroy(&pix1);

		pixContrastNorm(pixg, pixg, 50, 50, 130, 2, 2);
		pixg2 = pixBackgroundNorm(pixg, NULL, NULL, 20, 20, newval, 40, 200, 2, 2);//70

																				   //ival = L_MIN(ival, 110);
		ival = L_MIN(newval, 110);
		pix1 = pixThresholdToBinary(pixg2, ival);
		//pixSaveTiled(pix1, pixa, 1.0, 1, 50, 32);

		//pixWrite("lept5.jpg", pix1, IFF_JFIF_JPEG);
		pix8 = pixConvert1To8(NULL, pix1, 255, 0);
		imgGray = PIX2cvMat(pix8).clone();
		//pixDisplay(pix1, 100, 800);
		pixDestroy(&pixg);
		pixDestroy(&pixg2);
		pixDestroy(&pix1);
		pixDestroy(&pix8);
		break;
	default:
		break;
	}

	pixDestroy(&pixs);
	return 0;

}

//页面旋转
void RotImg(Mat &src, float angle)
{
	Point center(src.cols / 2, src.rows / 2);
	Mat rotMat = getRotationMatrix2D(center, angle, 1.0);
	Mat dstImg = Mat::ones(src.size(), CV_8UC3);
	warpAffine(src, dstImg, rotMat, src.size(), 1, 0, Scalar(255, 255, 255));
	src = dstImg.clone();
}
void ImgRotAdjFFT_2(Mat &src, float &angelD)
{
	// Xuefeng Wang 20200515 整体算法代码更新
	cv::Mat srcImg;
	src.copyTo(srcImg);
#if 0
	// Xuefeng Wang 20200304
	cv::resize(srcImg, srcImg, cv::Size(0, 0), 800.0 / srcImg.rows, 800.0 / srcImg.rows);
	//	cv::imwrite("resize.jpg", srcImg);
	cv::Canny(srcImg, srcImg, 63, 191);
	cv::Mat erodeElement = getStructuringElement(cv::MORPH_RECT, cv::Size(15, 1));
	cv::dilate(srcImg, srcImg, erodeElement);
	//	cv::threshold(srcImg, srcImg, 127, 255, cv::THRESH_OTSU);
	// Xuefeng Wang 20200304
	// cv::imwrite("erode.jpg", srcImg);
#else
	// Xuefeng Wang 20200520 针对于灰头和表格线情况的优化
	if (srcImg.channels() != 1)
		cv::cvtColor(srcImg, srcImg, cv::COLOR_BGR2GRAY);
	cv::threshold(srcImg, srcImg, 127, 255, cv::THRESH_OTSU);
	cv::resize(srcImg, srcImg, cv::Size(0, 0), 800.0 / srcImg.rows, 800.0 / srcImg.rows);
	cv::erode(srcImg, srcImg, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 1)));
	cv::dilate(srcImg, srcImg, getStructuringElement(cv::MORPH_RECT, cv::Size(5, 1)));
	//	cv::imwrite("dilate.jpg", srcImg);
	cv::Canny(srcImg, srcImg, 63, 191);
	cv::Mat erodeElement = getStructuringElement(cv::MORPH_RECT, cv::Size(15, 1));
	cv::dilate(srcImg, srcImg, erodeElement);
	cv::GaussianBlur(srcImg, srcImg, cv::Size(3, 3), 1);
	cv::threshold(srcImg, srcImg, 63, 255, cv::THRESH_BINARY);
	// Xuefeng Wang 20200520
#endif
	cv::Mat padded;
	int opWidth = cv::getOptimalDFTSize(srcImg.rows);
	int opHeight = cv::getOptimalDFTSize(srcImg.cols);
	cv::copyMakeBorder(srcImg, padded, 0, opWidth - srcImg.rows, 0, opHeight - srcImg.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F) };
	cv::Mat comImg;
	cv::merge(planes, 2, comImg);
	cv::dft(comImg, comImg);
	cv::split(comImg, planes);
	cv::magnitude(planes[0], planes[1], planes[0]);
	cv::Mat magMat = planes[0];
	magMat += cv::Scalar::all(1);
	cv::log(magMat, magMat);
	magMat = magMat(cv::Rect(0, 0, magMat.cols & -2, magMat.rows & -2));

	int cx = magMat.cols / 2;
	int cy = magMat.rows / 2;

	cv::Mat q0(magMat, cv::Rect(0, 0, cx, cy));
	cv::Mat q1(magMat, cv::Rect(0, cy, cx, cy));
	cv::Mat q2(magMat, cv::Rect(cx, cy, cx, cy));
	cv::Mat q3(magMat, cv::Rect(cx, 0, cx, cy));

	cv::Mat tmp;
	q0.copyTo(tmp);
	q2.copyTo(q0);
	tmp.copyTo(q2);

	q1.copyTo(tmp);
	q3.copyTo(q1);
	tmp.copyTo(q3);

	cv::normalize(magMat, magMat, 0, 1, cv::NORM_MINMAX);
	cv::Mat magImg(magMat.size(), CV_8UC3);
	magMat.convertTo(magImg, CV_8UC1, 255, 0);
	//	cv::imwrite("FFT_ori.jpg", magImg);
	cv::threshold(magImg, magImg, GRAY_THRESH, 255, cv::THRESH_BINARY);
	//	cv::imwrite("FFT.jpg", magImg);
	float k1 = 0.0;
	float k2 = 0.0;
	float angel = 0;
	int count = 0;
	int countMax = 8;

	vector<Point> points;
	cv::Point p1, p2;
	count = 0;
	for (int i = 0; i <magImg.rows; i++)
		for (int j = 0; j <magImg.cols; j++)
			if (magImg.at<uchar>(i, j) != 0)
			{
				count++;
				if (count <= countMax)
				{
					p1.x = j;
					p1.y = i;
					points.push_back(p1);
					if (count == countMax)
						goto label_next;
					//break;
				}
			}
label_next:
	count = 0;
	for (int i = magImg.rows - 1; i >= 0; i--)
		for (int j = magImg.cols - 1; j >= 0; j--)
			if (magImg.at<uchar>(i, j) != 0)
			{
				count++;
				if (count <= countMax)
				{
					p2.x = j;
					p2.y = i;
					points.push_back(p2);
					if (count == countMax)
						goto label_end;
					//break;
				}
			}
label_end:

	//直线拟合
	if (!points.size())
	{
		return;
	}
	Vec4f line;
	fitLine(points, line, DIST_HUBER, 0, 0.01, 0.01);
	double cos_theta = line[0];
	double sin_theta = line[1];
	double x0 = line[2], y0 = line[3];

	double theta = atan2(sin_theta, cos_theta) + PI / 2.0;
	double rho = y0 * cos_theta - x0 * sin_theta;

	if (theta < PI / 4. || theta > 3.*PI / 4.)// ~vertical line
	{
		p1 = Point(rho / cos(theta), 0);
		p2 = Point((rho - magImg.rows * sin(theta)) / cos(theta), magImg.rows);
	}
	else
	{
		p1 = Point(0, rho / sin(theta));
		p2 = Point(magImg.cols, (rho - magImg.cols * cos(theta)) / sin(theta));
	}
	//Mat lineSign(magImg.size(), CV_8UC3);
	//cv::line(lineSign, p1, p2, Scalar(0, 255, 0), 1);
	//imwrite("lineimage.jpg", lineSign);

	k1 = (float)(p2.x - p1.x) / (float)(p2.y - p1.y - EP)*(float)magImg.rows / (float)magImg.cols;
	angel = -atan(k1);
	angelD = angel * 180 / (float)CV_PI;
	// Xuefeng Wang 20200515 
	RotImg(src, angelD);
}

//输出文字内容到txt
int write_QString_to_file(QString filePath, QString text, int flag = 0)
{
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return 1;

	if (flag == 0)
	{
		QTextStream textStream(&file);
		textStream << text;
	}
	else if (flag == 1)
	{
		QDataStream datStream(&file);
		datStream << text;
	}
	else
	{
		file.close();
		return 1;
	}

	file.close();
	return 0;
}

//UTF8转Unicode
char* UTF8ToUnicode(const char* szU8)
{
	//UTF8 to Unicode
	//由于中文直接复制过来会成乱码，编译器有时会报错，故采用16进制形式

	//预转换，得到所需空间的大小
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);
	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间    11 |
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);
	//最后加上'\0'
	wszString[wcsLen] = '\0';

	int nLen = WideCharToMultiByte(CP_ACP, 0, wszString, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
	{
		return NULL;
	}
	char* pResult = new char[nLen];

	WideCharToMultiByte(CP_ACP, 0, wszString, -1, pResult, nLen, NULL, NULL);

	return pResult;
}

//输出指定颜色为透明色
int ToTransPNG(cv::Mat&img, int trans = 100, int m_B = 255, int m_G = 255, int m_R = 255, int m_T = 0)
{
	if (img.channels() != 4)
	{
		if (img.channels() == 1)
			cv::cvtColor(img, img, COLOR_GRAY2BGRA);
		else if (img.channels() == 3)
			cv::cvtColor(img, img, COLOR_BGR2BGRA);
		else
			return 1;
	}

	for (int y = 0; y < img.rows; ++y)
	{
		for (int x = 0; x < img.cols; ++x)
		{
			cv::Vec4b& pixel = img.at<cv::Vec4b>(y, x);
			if (pixel[0] == m_B && pixel[1] == m_G && pixel[2] == m_R)
			{
				pixel[3] = m_T;
			}
			else
			{
				pixel[3] = 255.0 * trans * 0.01;
			}
		}
	}

	return 0;
}

//文字提取进程
void getWordThread::run()
{
	Mat cvImage = QImageToCVMat(inputImage);
	QString textList;
	ocr_tool.SetImage((uchar*)cvImage.data, cvImage.size().width, cvImage.size().height, cvImage.channels(), cvImage.step1());
	ocr_tool.Recognize(0);
	tesseract::ResultIterator* ri = ocr_tool.GetIterator();
	tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;
	if (ri != 0) {
		do {
			char *word = ri->GetUTF8Text(level);
			if (word == nullptr)
				continue;
			char *word_uni = UTF8ToUnicode(word);
			textList.append(QString::fromLocal8Bit(word_uni));

			if (word != nullptr)
			{
				delete[]word;
				word = nullptr;
			}
		} while (ri->Next(level));
		delete ri;
	}
	ocr_tool.Clear();

	QString oriFileName = QFileInfo(inputFilePath).fileName();
	QString oriFileSuffix = QFileInfo(inputFilePath).suffix();
	textOutPath = QString(runningPath + "\\OutputText\\%1.txt").arg(oriFileName.left(oriFileName.size() - oriFileSuffix.size() - 1));
	write_QString_to_file(textOutPath, textList);

	emit processFinish();
}

ImageProcessTools::ImageProcessTools(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton_load, SIGNAL(clicked()), this, SLOT(loadImage()));
	connect(ui.pushButton_save, SIGNAL(clicked()), this, SLOT(saveImage()));
	connect(ui.pushButton_rmGround, SIGNAL(clicked()), this, SLOT(rmBackground()));
	connect(ui.pushButton_return, SIGNAL(clicked()), this, SLOT(returnProcess()));
	connect(ui.pushButton_merge, SIGNAL(clicked()), this, SLOT(mergeImage()));
	connect(ui.pushButton_rotate, SIGNAL(clicked()), this, SLOT(rotateImage()));
	connect(ui.pushButton_getText, SIGNAL(clicked()), this, SLOT(getImageText()));
	connect(ui.pushButton_trans, SIGNAL(clicked()), this, SLOT(transImage()));

	if (initPara())
		WarringDlg(u8"初始化异常，请检查路径下文件是否完整！");
}

ImageProcessTools::~ImageProcessTools()
{
	processedImageList.swap(vector<DetectRecord>());

	ocr_tool.End();
	delete proc;
}

//初始化
int ImageProcessTools::initPara()
{
	processInfo.setIcon(QMessageBox::Information);
	processInfo.setWindowTitle(u8"提示");
	processInfo.setStandardButtons(0);

	ui.pushButton_region->setEnabled(false);
	//ui.pushButton_trans->setEnabled(false);
	//ui.pushButton_getText->setEnabled(false);
	ui.pushButton_project->setEnabled(false);

	ui.comboBox_rmGroundFunc->addItem(u8"方法一(最弱)");
	ui.comboBox_rmGroundFunc->addItem(u8"方法二");
	ui.comboBox_rmGroundFunc->addItem(u8"方法三");
	ui.comboBox_rmGroundFunc->addItem(u8"方法四(默认)");
	ui.comboBox_rmGroundFunc->addItem(u8"方法五(最强)");
	ui.comboBox_rmGroundFunc->setCurrentIndex(3);

	QString appName = QApplication::applicationName();        //程序名称
	QString appPath = QApplication::applicationFilePath();    //程序名称+路径
	runningPath = QFileInfo(appPath).absolutePath();  //程序路径
	QDir::setCurrent(runningPath);
	runningPath.replace("/", "\\");

	const char* languagePath = QString(runningPath + "\\FCharacterData").toLocal8Bit().data();
	int nRet = ocr_tool.Init(languagePath, "eng+FZL_chi_sim+osd", tesseract::OEM_TESSERACT_LSTM_COMBINED);//OEM_TESSERACT_LSTM_COMBINED     OEM_LSTM_ONLY
	if (nRet)
		return 1;

	thread_ocr = new getWordThread();
	connect(thread_ocr, SIGNAL(processFinish()), this, SLOT(processDialogFinish()));
	
	proc = new QProcess();

	return 0;
}

void ImageProcessTools::processDialogFinish()
{
	processInfo.hide();

	WarringDlg(u8"文字提取完成！点击确定打开*.txt");

	QString cmd_string = QString("notepad.exe \"%1\"").arg(thread_ocr->textOutPath);
	proc->start(cmd_string);
}

//加载图像
void ImageProcessTools::loadImage(QString openFileName)
{
	if (openFileName.isEmpty())
	{
		openFileName = QFileDialog::getOpenFileName(
			this,
			u8"打开文件：【支持 图像】",
			lastPath,
			"*.tif *.tiff *.bmp *.jpg *.jpeg *.jpe *.png");
		if (openFileName.isEmpty())
		{
			return;
		}
	}
	
	inputImage.load(openFileName);
	if (inputImage.isNull())
	{
		WarringDlg(u8"加载图像失败！");
		return;
	}
	ui.graphicsView->setImage(inputImage);
	ui.graphicsView->fitScale();
	inputFilePath = openFileName;
	processedImageList.swap(vector<DetectRecord>());
	processedImageList.push_back(DetectRecord(inputImage, p_oriImage));
	//inputImage_temp = inputImage;
}

//拖拽加载
void ImageProcessTools::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}
void ImageProcessTools::dropEvent(QDropEvent*event)
{
	const QMimeData *qm = event->mimeData();//获取MIMEData
	QString openFileNames;
	QList<QUrl> loadurlList = qm->urls();
	if (!loadurlList.size())
	{
		return;
	}
	QString loadfilepath = loadurlList[0].toLocalFile();
	QString loadfilesuffix = QFileInfo(loadfilepath).suffix();
	if (!loadfilesuffix.compare("tif", Qt::CaseInsensitive)
		|| !loadfilesuffix.compare("tiff", Qt::CaseInsensitive)
		|| !loadfilesuffix.compare("bmp", Qt::CaseInsensitive)
		|| !loadfilesuffix.compare("jpg", Qt::CaseInsensitive)
		|| !loadfilesuffix.compare("jpeg", Qt::CaseInsensitive)
		|| !loadfilesuffix.compare("jpe", Qt::CaseInsensitive)
		|| !loadfilesuffix.compare("png", Qt::CaseInsensitive)
		)
	{
		openFileNames = loadfilepath;
	}
	else
	{
		WarringDlg(u8"包含不支持的文件格式！");
		return;
	}

	loadImage(openFileNames);
}

//保存图像
void ImageProcessTools::saveImage()
{
	if (!processedImageList.size())
	{
		WarringDlg(u8"暂无任何处理，保存失败！");
		return;
	}
	QString oriFileName = QFileInfo(inputFilePath).fileName();
	QString oriFileSuffix = QFileInfo(inputFilePath).suffix();
	QString saveSuffix = processedImageList[processedImageList.size() - 1].second == p_transImage ? "*.png;;*.jpg" : "*.jpg;;*.png";
	QString saveFileName = QFileDialog::getSaveFileName(
		this,
		u8"保存处理后图像",
		lastPath + QString("\\%1_[Done]").arg(oriFileName.left(oriFileName.size() - oriFileSuffix.size() - 1)),
		saveSuffix);
	if (saveFileName.isEmpty())
	{
		return;
	}
	if (processedImageList[processedImageList.size() - 1].first.save(saveFileName))
	{
		WarringDlg(u8"保存处理图像成功！");
	}
}

//撤销
void ImageProcessTools::returnProcess()
{
	if (processedImageList.size() <= 1)
	{
		WarringDlg(u8"已撤销至原始图像！");
		return;
	}
	processedImageList.erase(processedImageList.begin() + processedImageList.size() - 1);
	//inputImage_temp.~QImage();
	//inputImage.load(inputFilePath);
	ui.graphicsView->setImage(processedImageList[processedImageList.size() - 1].first);
	ui.graphicsView->fitScale();
	WarringDlg(u8"已撤销");
}

//去背景
void ImageProcessTools::rmBackground()
{
	if (!processedImageList.size())
	{
		WarringDlg(u8"暂未加载图像！");
		return;
	}
	QImage inputImage_temp;
	for (int i = processedImageList.size() - 1; i >=  0; i--)
	{
		if (processedImageList[i].second != p_threshImage)
		{
			inputImage_temp = processedImageList[i].first;
			break;
		}		
	}
	Mat cvImage = QImageToCVMat(inputImage_temp);
	//Mat cvImage =  QImageToCVMat(inputImage);

	if (cvImage.channels() != 1)
	{
		cvtColor(cvImage, cvImage, COLOR_BGR2GRAY);
	}
	int leptMethodFlag = ui.comboBox_rmGroundFunc->currentIndex() + 1;
	if (leptMethodFlag < 1 || leptMethodFlag > 5)
	{
		leptMethodFlag = 4;
	}
	int flag = leptThresh(cvImage, leptMethodFlag);
	if (flag)
	{
		WarringDlg(u8"去背景处理算法异常！");
		return;
	}
	inputImage_temp = opencvMatToQImage(cvImage);
	ui.graphicsView->setImage(inputImage_temp);
	ui.graphicsView->fitScale();
	processedImageList.push_back(DetectRecord(inputImage_temp, p_threshImage));
	WarringDlg(u8"去背景处理操作【已完成】");
}

//合并图像
void ImageProcessTools::mergeImage()
{
	if (inputImage.isNull())
	{
		WarringDlg(u8"请先加载基本图！");
		return;
	}
	QString openFileName = QFileDialog::getOpenFileName(
		this,
		u8"打开待合并文件：【支持 图像】",
		lastPath,
		"*.tif *.tiff *.bmp *.jpg *.jpeg *.jpe *.png");
	if (openFileName.isEmpty())
	{
		return;
	}

	QImage mergeImg(openFileName);
	if (mergeImg.isNull())
	{
		WarringDlg(u8"待合并图像加载失败！");
		return;
	}
	
	int mergeFlag = MessageCheckDlg(u8"请选择合并图像的位置");

	QImage inputImage_temp;
	inputImage_temp = processedImageList[processedImageList.size() - 1].first;
	Mat input1 = mergeFlag ? QImageToCVMat(mergeImg) : QImageToCVMat(inputImage_temp);
	Mat input2 = mergeFlag ? QImageToCVMat(inputImage_temp) : QImageToCVMat(mergeImg);
	if (input1.channels() != 3)
	{
		cvtColor(input1, input1, COLOR_GRAY2RGB);
	}
	if (input2.channels() != 3)
	{
		cvtColor(input2, input2, COLOR_GRAY2RGB);
	}
	int width_common = max(input1.cols, input2.cols);
	double zoom_factor1 = (double)width_common / (double)input1.cols;
	double zoom_factor2 = (double)width_common / (double)input2.cols;
	cv::resize(input1, input1, Size(width_common, input1.rows * zoom_factor1));
	cv::resize(input2, input2, Size(width_common, input2.rows * zoom_factor2));
	Mat output(Size(width_common, input1.rows + input2.rows), input1.type());
	input1.copyTo(output(Rect(0, 0, input1.cols, input1.rows)));
	input2.copyTo(output(Rect(0, input1.rows, input2.cols, input2.rows)));

	inputImage_temp = opencvMatToQImage(output);
	ui.graphicsView->setImage(inputImage_temp);
	ui.graphicsView->fitScale();
	processedImageList.push_back(DetectRecord(inputImage_temp, p_mergeImage));
	WarringDlg(u8"合并图像操作【已完成】");
	return;
}

//旋转图像
void ImageProcessTools::rotateImage()
{
	Mat cvImage = QImageToCVMat(processedImageList[processedImageList.size() - 1].first);

	ocr_tool.SetPageSegMode(tesseract::PSM_OSD_ONLY);
	ocr_tool.SetImage((uchar*)cvImage.data, cvImage.size().width, cvImage.size().height, cvImage.channels(), cvImage.step1());
	ocr_tool.Recognize(0);
	char *infm = ocr_tool.GetOsdText(0);
	if (infm != NULL)
	{
		if (strstr(infm, "Rotate: 90"))
		{
			transpose(cvImage, cvImage);
			flip(cvImage, cvImage, 1);
		}
		else if (strstr(infm, "Rotate: 180"))
		{
			flip(cvImage, cvImage, -1);
		}
		else if (strstr(infm, "Rotate: 270"))
		{
			transpose(cvImage, cvImage);
			flip(cvImage, cvImage, 0);
		}
	}
	ocr_tool.Clear();

	//提取右上角区域，并校正
	float angelD = 0.0;
	ImgRotAdjFFT_2(cvImage, angelD);

	QImage inputImage_temp = opencvMatToQImage(cvImage);
	ui.graphicsView->setImage(inputImage_temp);
	ui.graphicsView->fitScale();
	processedImageList.push_back(DetectRecord(inputImage_temp, p_rotateImage));
	WarringDlg(u8"旋转校正操作【已完成】");
}

//提取文字：输出到txt中，并打开
void ImageProcessTools::getImageText()
{
	processInfo.setText(u8"正在进行文字提取，请稍后...");
	processInfo.show();
	
	thread_ocr->inputImage = processedImageList[processedImageList.size() - 1].first;
	thread_ocr->inputFilePath = inputFilePath;
	thread_ocr->runningPath = runningPath;
	thread_ocr->start();
}

//透明化：白色背景透明化
void ImageProcessTools::transImage()
{
	Mat cvImage = QImageToCVMat(processedImageList[processedImageList.size() - 1].first);

	ToTransPNG(cvImage);

	QImage inputImage_temp = opencvMatToQImage(cvImage);
	ui.graphicsView->setImage(inputImage_temp);
	ui.graphicsView->fitScale();
	processedImageList.push_back(DetectRecord(inputImage_temp, p_transImage));
	WarringDlg(u8"透明图像操作【已完成】");
}

//区域提取


//透视变换