#include "LibraryViewWidget.h"
#include "Ui_LibraryViewWidget.h"
#include "LibraryLoader.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QListWidget>
#include <QFileInfo>

namespace ami
{
	LibraryViewWidget::LibraryViewWidget(const Params & params, QWidget * parent)
		:
		QWidget(parent),
		m_ui(new Ui_LibraryViewWidget()),
		m_params(params),
		m_loader(std::make_unique<LibraryLoader>())
	{
		m_ui->setupUi(this);
		setSize(m_params.iconSize);
		connect(m_ui->listWidget, &QListWidget::itemActivated, this, &LibraryViewWidget::onItemActivated);
	}


	void LibraryViewWidget::addPattern(QListWidgetItem * icon, const QString & path)
	{
		icon->setData(Qt::UserRole, path);
		m_ui->listWidget->addItem(icon);
	}


	void LibraryViewWidget::setSize(const unsigned int size)
	{
		m_params.iconSize = size;
	}

	void LibraryViewWidget::load(const QString & path)
	{
		auto files = m_loader->getFiles(path);
		for (auto & file : files)
		{
			QFileInfo fileInfo(file);
			addPattern(new QListWidgetItem(QPixmap(128, 128), fileInfo.baseName()), file);
		}
	}


	void LibraryViewWidget::onItemActivated(QListWidgetItem * item)
	{
		emit itemActivated(item->data(Qt::UserRole).toString());
	}
}
