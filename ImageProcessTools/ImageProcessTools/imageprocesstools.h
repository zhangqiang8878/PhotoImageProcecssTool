#ifndef IMAGEPROCESSTOOLS_H
#define IMAGEPROCESSTOOLS_H

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QThread>
#include <tesseract/baseapi.h>
#include "ui_imageprocesstools.h"

typedef QPair<QImage, int> DetectRecord;

enum processName {
	p_oriImage,
	p_mergeImage,
	p_threshImage,
	p_rotateImage,
	p_transImage
};

class getWordThread : public QThread
{
	Q_OBJECT

public:
	getWordThread() {};
	~getWordThread() {};
	QImage inputImage;
	QString inputFilePath, runningPath, textOutPath;
	void run();

signals:
	void processFinish();
};

class ImageProcessTools : public QMainWindow
{
	Q_OBJECT

public:
	ImageProcessTools(QWidget *parent = 0);
	~ImageProcessTools();
	int initPara();

private:
	Ui::ImageProcessToolsClass ui;
	QString inputFilePath;
	QImage inputImage;
	//QImage inputImage_temp;
	getWordThread *thread_ocr;
	QString runningPath;
	std::vector<DetectRecord> processedImageList;
	QMessageBox processInfo;
	QProcess *proc;

	//定义槽函数
private slots:
	void loadImage(QString openFileName = NULL);
	void saveImage();
	void rmBackground();
	void returnProcess();
	void mergeImage();
	void rotateImage();
	void getImageText();
	void transImage();
	void processDialogFinish();

protected:
	virtual void dragEnterEvent(QDragEnterEvent*event);            //拖动进入事件
	virtual void dropEvent(QDropEvent*event);
};

#endif // IMAGEPROCESSTOOLS_H
