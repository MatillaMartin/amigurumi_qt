#include "EditorViewWidget.h"
#include "ui_EditorViewWidget.h"

#include "PatternEditorWidget.h"

namespace ami
{
	EditorViewWidget::EditorViewWidget(QWidget * parent)
		:
		QWidget(parent),
		m_ui(new Ui_EditorViewWidget())
	{
		m_ui->setupUi(this);
	}
}