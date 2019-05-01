#pragma once

#include <vector>
#include <QMetaEnum>

namespace ami
{
	class PatternEditorRound : public QObject
	{
		Q_OBJECT
	public:

		enum Operations {
			SC,
			INC,
			DEC
		};
		Q_ENUM(Operations)

		std::vector<Operations> operations;
	};
}