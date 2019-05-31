#pragma once
#include "Pattern.h"

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QItemDelegate>

namespace ami
{
	class PatternEditorItem;

	class PatternEditorWidget : public QTableWidget
	{
		Q_OBJECT;

	public:
		PatternEditorWidget(QWidget * parent = nullptr);

	public slots:
		void addRound();
		void removeRound(unsigned int index);
		std::unique_ptr<Pattern> pattern();

	private:
		PatternEditorItem * item(int row, int column);

		void updateRoundHeaders();
		void paste();
		void copy();
		void clearSelected();
	};
}