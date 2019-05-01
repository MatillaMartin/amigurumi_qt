#pragma once

#include <QtWidgets/QTableWidget>

namespace ami
{
	class PatternEditorWidget : public QTableWidget
	{
		Q_OBJECT;

	public:
		PatternEditorWidget(QWidget * parent = nullptr);

	public slots:
		void addRound();
		void removeRound(unsigned int index);


	private:
		void updateRoundHeaders();
		void paste();
		void copy();
		void clearSelected();
	};
}