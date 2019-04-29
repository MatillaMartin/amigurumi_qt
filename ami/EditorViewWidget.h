#pragma once

#include <QWidget>
#include "Ui_EditorViewWidget.h"

namespace ami
{
	class EditorViewWidget : public QWidget
	{
	public:
		EditorViewWidget(QWidget * parent);

	private:
		Ui_EditorViewWidget * m_ui = nullptr;
	};
}