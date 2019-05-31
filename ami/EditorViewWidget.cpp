#include "EditorViewWidget.h"
#include "ui_EditorViewWidget.h"

#include "PatternEditorWidget.h"
#include "PatternGraph.h"

#include <QtWidgets/QPushButton>
#include <QDebug>

namespace ami
{
	EditorViewWidget::EditorViewWidget(QWidget * parent)
		:
		QWidget(parent),
		m_ui(new Ui_EditorViewWidget())
	{
		m_ui->setupUi(this);

		connect(m_ui->updateButton, &QPushButton::clicked, this, &EditorViewWidget::onUpdatePattern);
	}

	EditorViewWidget::~EditorViewWidget()
	{
	}

	void EditorViewWidget::onUpdatePattern()
	{
		Pattern pattern;
		if (m_ui->patternEditor->pattern(pattern))
		{
			try
			{
				m_graph = std::make_unique<PatternGraph>(pattern);
			}
			catch (const std::runtime_error & ex)
			{
				qDebug() << "Could not update pattern: " << ex.what();
			}
		}
		else
		{
			qDebug() << "Invalid pattern";
		}
	}
}