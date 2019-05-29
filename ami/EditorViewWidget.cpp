#include "EditorViewWidget.h"
#include "ui_EditorViewWidget.h"

#include "PatternEditorWidget.h"
#include "PatternGraph.h"

#include <QtWidgets/QPushButton>
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
		std::unique_ptr<Pattern> pattern (std::move(m_ui->patternEditor->pattern()));
		
		m_graph = std::make_unique<PatternGraph>(*pattern.get());
	}
}