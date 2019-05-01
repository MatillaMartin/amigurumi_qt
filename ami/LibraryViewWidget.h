#pragma once

#include <QtWidgets/QWidget>

class Ui_LibraryViewWidget;
class QListWidgetItem;

namespace ami
{
	class PatternIconWidget;

	class LibraryViewWidget : public QWidget
	{
		Q_OBJECT;

	public:
		struct Params
		{
			int iconSize = 128;
		};

		LibraryViewWidget(const Params & params = Params(), QWidget * parent = nullptr);

		void addPattern(QListWidgetItem * icon, const QString & path);
		void setSize(const unsigned int size);

	signals:
		void itemActivated(QString path);

	private:
		void onItemActivated(QListWidgetItem * item);

		Ui_LibraryViewWidget * m_ui = nullptr;
		Params m_params;
	};
}