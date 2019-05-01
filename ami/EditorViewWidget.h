#pragma once

#include <QWidget>
#include "Ui_EditorViewWidget.h"

namespace ami
{
	class PatternEditorWidget;

	class EditorViewWidget : public QWidget
	{
		Q_OBJECT;

	public:
		EditorViewWidget(QWidget * parent = nullptr);

	private:
		Ui_EditorViewWidget * m_ui = nullptr;
	};
}