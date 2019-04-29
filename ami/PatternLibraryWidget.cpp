#include "PatternLibraryWidget.h"
#include "Ui_PatternLibraryWidget.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QListWidget>

namespace ami
{
	PatternLibraryWidget::PatternLibraryWidget(const Params & params, QWidget * parent)
		:
		QWidget(parent),
		m_ui(new Ui_PatternLibraryWidget()),
		m_params(params)
	{
		m_ui->setupUi(this);
		setSize(m_params.iconSize);
		connect(m_ui->listWidget, &QListWidget::itemActivated, this, &PatternLibraryWidget::onItemActivated);
	}


	void PatternLibraryWidget::addPattern(QListWidgetItem * icon, const QString & path)
	{
		icon->setData(Qt::UserRole, path);
		m_ui->listWidget->addItem(icon);
	}


	void PatternLibraryWidget::setSize(const unsigned int size)
	{
		m_params.iconSize = size;
	}


	void PatternLibraryWidget::onItemActivated(QListWidgetItem * item)
	{
		emit itemActivated(item->data(Qt::UserRole).toString());
	}
}
