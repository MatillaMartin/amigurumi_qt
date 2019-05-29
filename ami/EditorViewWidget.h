#pragma once

#include <QWidget>
#include "ui_EditorViewWidget.h"

namespace ami
{
	class PatternEditorWidget;
	class PatternGraph;

	class EditorViewWidget : public QWidget
	{
		Q_OBJECT;

	public:
		EditorViewWidget(QWidget * parent = nullptr);
		~EditorViewWidget();
	private:
		Ui_EditorViewWidget * m_ui = nullptr;
		std::unique_ptr<PatternGraph> m_graph;

		void onUpdatePattern();
	};
}
