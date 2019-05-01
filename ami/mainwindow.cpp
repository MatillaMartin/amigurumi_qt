#include "mainwindow.h"
#include "LibraryViewWidget.h"
#include "EditorViewWidget.h"

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

		setView(View::EDITOR);

		for(int i = 0; i < 20; ++i)
			//m_patternLibrary->addPattern(new PatternIconWidget(QPixmap(128,128), "Pattern " + QString::number(i) , this));
			m_patternLibrary->addPattern(new QListWidgetItem(QPixmap(128, 128), "Pattern " + QString::number(i)), "patternpath.ami");

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

		case View::EDITOR:
			this->setCentralWidget(m_patternEditor);
			m_patternEditor->show();
		}
	}
}
