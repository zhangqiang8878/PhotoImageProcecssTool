#include "imageprocesstools.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ImageProcessTools w;
	w.show();
	return a.exec();
}
