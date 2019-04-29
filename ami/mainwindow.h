#pragma once
#include <QMainWindow>

class Ui_MainWindow;

namespace ami
{
	class PatternLibraryWidget;
	class EditorViewWidget;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow();

	private:
		enum class View { LIBRARY, EDITOR };

		void setView(View view);

		Ui_MainWindow * m_ui = nullptr;

		PatternLibraryWidget * m_patternLibrary = nullptr;
		EditorViewWidget * m_patternEditor = nullptr;
	};
}
