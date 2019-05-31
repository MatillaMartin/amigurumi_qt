#include "PatternEditorWidget.h"
#include "PatternEditorItem.h"

#include <QtWidgets/QHeaderView>
#include <QApplication>
#include <QClipboard>
#include <QShortcut>

namespace ami
{
	PatternEditorWidget::PatternEditorWidget(QWidget * parent)
		:
		QTableWidget(parent)
	{
		setColumnCount(36);
		setRowCount(36);
		setItemPrototype(new PatternEditorItem);

		updateRoundHeaders();
		resizeColumnsToContents();
		resizeRowsToContents();

		connect(new QShortcut(QKeySequence(QKeySequence::Copy), this), &QShortcut::activated, this, &PatternEditorWidget::copy);
		connect(new QShortcut(QKeySequence(QKeySequence::Paste), this), &QShortcut::activated, this, &PatternEditorWidget::paste);
		connect(new QShortcut(QKeySequence(QKeySequence::Delete), this), &QShortcut::activated, this, &PatternEditorWidget::clearSelected);
	}

	void PatternEditorWidget::addRound()
	{
		// create a new row in the table
		insertRow(rowCount());
	}

	void PatternEditorWidget::removeRound(unsigned int index)
	{
		removeRow(index);
	}

	std::unique_ptr<Pattern> PatternEditorWidget::pattern()
	{
		std::unique_ptr<Pattern> pattern = std::make_unique<Pattern>();

		for (int row = 0; row < this->rowCount(); ++row)
		{
			Operations round;
			for (int col = 0; col < this->columnCount(); ++col)
			{
				auto item = this->item(row, col);
				if (item)
				{
					Operation::Type op;
					if (Operation::getOperation(item->text(), op))
					{
						round.push_back(op);
					}
				}
			}

			if (!round.empty())
			{
				pattern->add(round);
			}
		}

		return pattern;
	}

	PatternEditorItem * PatternEditorWidget::item(int row, int column)
	{
		return static_cast<PatternEditorItem*>(QTableWidget::item(row, column));
	}

	void PatternEditorWidget::updateRoundHeaders()
	{
		QStringList labels;

		for (int i = 0; i < this->rowCount(); ++i)
		{
			labels << QString("Round ") + QString::number(i+1);
		}

		setVerticalHeaderLabels(labels);
	}

	void PatternEditorWidget::paste()
	{
		QString pastedText = QApplication::clipboard()->text();

		QStringList rows = pastedText.split('\n');
		
		int row = currentRow();
		for (const auto & rowText : rows)
		{
			int column = currentColumn();
			QStringList columnsText = rowText.split('\t');
			for (const auto & cellText : columnsText)
			{
				if (row < rowCount() && column < columnCount())
				{
					auto * cell = item(row, column);
					if (!cell)
					{
						cell = new PatternEditorItem();
						setItem(row, column, cell);
					}
					cell->setText(cellText);
				}
				++column;
			}

			++row;
		}
	}

	void PatternEditorWidget::copy()
	{
		QString selectedText;
		
		auto indices = selectedIndexes();
		if (indices.empty())
		{
			return;
		}
		
		int lastRow = indices.first().row();

		for (auto index : indices)
		{
			if (index.row() != lastRow)
			{
				selectedText.chop(1); // remove trailing \t
				selectedText.append('\n');
			}

			auto * cell = item(index.row(), index.column());
			if (cell)
			{
				selectedText.append(cell->text());
			}

			selectedText.append('\t');

			lastRow = index.row();
		}
		selectedText.chop(1); // remove trailing \t

		QApplication::clipboard()->setText(selectedText);
	}

	void PatternEditorWidget::clearSelected()
	{
		for (auto * cell : selectedItems())
		{
			// cast to our custom Item
			PatternEditorItem * item = static_cast<PatternEditorItem*>(cell);
			item->setText("");
		}
	}
}
