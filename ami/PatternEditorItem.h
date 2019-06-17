#pragma once

#include "ami/Operation.h"

#include <QtWidgets/QTableWidget>

namespace ami
{
	class PatternEditorItem : public QTableWidgetItem
	{
	public:
		QTableWidgetItem * clone() const;
		void setText(const QString & text);
		void setData(int role, const QVariant &value);
		bool operation(Operation::Type & outOp);
		bool isValid() { return m_isValid; }
		void setValid(bool valid);

	private:
		void updateUI();

		bool m_isValid = false;
		Operation::Type m_operation = Operation::Type::NONE;
	};
}