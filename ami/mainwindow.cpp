#include "ami/mainwindow.h"
#include "ami/LibraryViewWidget.h"
#include "ami/EditorViewWidget.h"

#include <QtWidgets>
#include <ui_MainWindow.h>

namespace ami
{
	MainWindow::MainWindow()
		:
		m_ui(new Ui_MainWindow()),
		m_patternLibrary(new LibraryViewWidget({ 128 }, this)),
		m_patternEditor(new EditorViewWidget(this))
	{
		// setup designer UI
		m_ui->setupUi(this);

		setView(View::LIBRARY);

		m_patternLibrary->load(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)); // load from appdata

	}


	void MainWindow::setView(View view)
	{
		m_patternLibrary->hide();
		m_patternEditor->hide();

		switch (view)
		{
		case View::LIBRARY:
			this->setCentralWidget(m_patternLibrary);
			m_patternLibrary->show();
			break;

		case View::EDITOR:
			this->setCentralWidget(m_patternEditor);
			m_patternEditor->show();
			break;
		}
	}
}
