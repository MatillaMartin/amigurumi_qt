#include "LibraryViewWidget.h"
#include "ui_LibraryViewWidget.h"
#include "LibraryLoader.h"
#include "Amigurumi.h"

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

	void LibraryViewWidget::setSize(const unsigned int size)
	{
		m_params.iconSize = size;
	}

	void LibraryViewWidget::import(const QString & path)
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

	void LibraryViewWidget::addPattern(QListWidgetItem * item, const QString & path)
	{
		try
		{
			Amigurumi ami = Amigurumi::fromFile(path);
			item->setData(Qt::UserRole, path);
			m_ui->listWidget->addItem(item);
		}
		catch (std::runtime_error & ex)
		{
			qDebug() << "Error adding pattern: " << path << ". " << ex.what();
		}
	}

	void LibraryViewWidget::onCustomContextMenuRequested(const QPoint & pos)
	{
		QMenu *menu = new QMenu(this);
		menu->addAction(new QAction(tr("Edit"), this));
		menu->addAction(new QAction(tr("Rename"), this));
		menu->addAction(new QAction(tr("Remove"), this));
		menu->addAction(new QAction(tr("Join.."), this));
		menu->popup(m_ui->listWidget->viewport()->mapToGlobal(pos));
	}
}
