#include "PatternEditorItem.h"

namespace ami
{
	QTableWidgetItem * PatternEditorItem::clone() const
	{
		return new PatternEditorItem(*this);
	}

	void PatternEditorItem::setText(const QString & text)
	{
		setData(Qt::EditRole, text);
	}

	void PatternEditorItem::setData(int role, const QVariant & value)
	{
		QTableWidgetItem::setData(role, value);
		if (role == Qt::EditRole)
		{
			QString text = value.toString();
			if (text.isEmpty())
			{
				m_operation = Operation::Type::NONE;
				m_isValid = true;
			}
			else
			{
				m_isValid = Operation::getOperation(text, m_operation);
			}

			updateUI();
		}
	}

	bool PatternEditorItem::operation(Operation::Type & outOp)
	{
		if (m_isValid)
		{
			outOp = m_operation;
		}

		return m_isValid;
	}

	void PatternEditorItem::setValid(bool valid) 
	{ 
		m_isValid = valid;
		updateUI(); 
	}

	void PatternEditorItem::updateUI()
	{
		if (m_isValid)
		{
			setBackground(Qt::GlobalColor::white);
		}
		else
		{
			setBackground(Qt::GlobalColor::red);
		}
	}
}