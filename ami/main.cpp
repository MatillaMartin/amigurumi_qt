
#include "ami/mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFile>
#include <QDebug>

using namespace ami;

int main(int argc, char *argv[])
{
	qDebug() << "Starting application";
	Q_INIT_RESOURCE(application);

	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("Ami");
	QCoreApplication::setApplicationName("Amigurumi");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);
	QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("file", "The file to open.");
	parser.process(app);

	// read stylesheet from resource or from folder
	qDebug() << "Loading stylesheet";
	QFile file(":/stylesheet.qss");
	file.open(QFile::ReadOnly);
	QString style(file.readAll());
	app.setStyleSheet(style);


	MainWindow mainWin;
	mainWin.show();
	return app.exec();
}