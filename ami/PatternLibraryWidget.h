#pragma once

#include <QtWidgets/QWidget>

class Ui_PatternLibraryWidget;
class QListWidgetItem;

namespace ami
{
	class PatternIconWidget;

	class PatternLibraryWidget : public QWidget
	{
		Q_OBJECT;

	public:
		struct Params
		{
			int iconSize = 128;
		};

		PatternLibraryWidget(const Params & params = Params(), QWidget * parent = nullptr);

		void addPattern(QListWidgetItem * icon, const QString & path);
		void setSize(const unsigned int size);

	signals:
		void itemActivated(QString path);

	private:
		void onItemActivated(QListWidgetItem * item);

		Ui_PatternLibraryWidget * m_ui = nullptr;
		Params m_params;
	};
}