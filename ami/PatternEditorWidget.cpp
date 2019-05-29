#include "PatternEditorWidget.h"

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

		int nRows = this->rowCount();
		int nCols = this->columnCount();

		for (int row = 0; row < nRows; ++row)
		{
			Operations round;
			for (int col = 0; col < nCols; ++col)
			{
				auto item = this->item(row, col);
				if (item)
				{
					Operations ops;
					Operation::getOperations(item->text(), 1, ops);
					round.insert(round.begin(), ops.begin(), ops.end());
				}
			}

			if (!round.empty())
			{
				pattern->add(round);
			}
		}

		return pattern;
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
					if (cell)
					{
						cell->setText(cellText);
					}
					else
					{
						setItem(row, column, new QTableWidgetItem(cellText));
					}
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
			cell->setText("");
		}
	}

}
