#pragma once

#include "Pattern.h"
#include <memory>
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

		LibraryViewWidget(const Params & params = {128}, QWidget * parent = nullptr);
		~LibraryViewWidget();
		void setSize(const unsigned int size);
		void load(const QString & path);
		void addPattern(QListWidgetItem * item, const QString & path);

	public slots:
		void onCustomContextMenuRequested(const QPoint & pos);

	signals:
		void itemActivated(QString path);

	private:
		void onItemActivated(QListWidgetItem * item);

		Ui_LibraryViewWidget * m_ui = nullptr;
		Params m_params;
		std::unique_ptr<LibraryLoader> m_loader;
	};
}
