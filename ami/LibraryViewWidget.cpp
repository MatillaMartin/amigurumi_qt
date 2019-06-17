#include "ami/LibraryViewWidget.h"
#include "ui_LibraryViewWidget.h"
#include "ami/LibraryLoader.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QListWidget>
#include <QFileInfo>
#include <QtWidgets/QMenu>

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

		m_ui->listWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
		connect(m_ui->listWidget, &QListWidget::customContextMenuRequested, this, &LibraryViewWidget::onCustomContextMenuRequested);

	}
	
	LibraryViewWidget::~LibraryViewWidget(){}

	void LibraryViewWidget::addPattern(QListWidgetItem * item, const QString & path)
	{
		item->setData(Qt::UserRole, path);
		m_ui->listWidget->addItem(item);
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

	void LibraryViewWidget::onCustomContextMenuRequested(const QPoint & pos)
	{
		QMenu *menu = new QMenu(this	);
		menu->addAction(new QAction("Rename", this));
		menu->addAction(new QAction("Join..", this));
		menu->popup(m_ui->listWidget->viewport()->mapToGlobal(pos));
	}

}
