#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFile>

#include "mainwindow.h"

using namespace ami;

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(application);

	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("Ami");
	QCoreApplication::setApplicationName("Application Example");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);
	QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("file", "The file to open.");
	parser.process(app);

	// read stylesheet from resource or from folder
	QFile file(":/stylesheet.qss");
	file.open(QFile::ReadOnly);
	QString style(file.readAll());
	app.setStyleSheet(style);


	MainWindow mainWin;
	mainWin.show();
	return app.exec();
}