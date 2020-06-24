#include "SurfaceCheck.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	SurfaceCheck w;
	w.show();
	return a.exec();
}
