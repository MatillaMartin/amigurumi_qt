#pragma once

#include "Pattern.h"

#include <QtWidgets/QWidget>

class Ui_LibraryViewWidget;
class QListWidgetItem;

namespace ami
{
	class PatternIconWidget;
	class LibraryLoader;

	class LibraryViewWidget : public QWidget
	{
		Q_OBJECT;

	public:
		struct Params
		{
			int iconSize = 128;
		};

		struct Item
		{
			QListWidgetItem * item;
			std::unique_ptr<Pattern> pattern;
		};

		LibraryViewWidget(const Params & params = Params(), QWidget * parent = nullptr);
		void setSize(const unsigned int size);
		void load(const QString & path);
		void addPattern(QListWidgetItem * icon, const QString & path);

	signals:
		void itemActivated(QString path);

	private:
		void onItemActivated(QListWidgetItem * item);

		Ui_LibraryViewWidget * m_ui = nullptr;
		Params m_params;
		std::unique_ptr<LibraryLoader> m_loader;
	};
}