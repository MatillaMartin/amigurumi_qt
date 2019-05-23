#pragma once

#include "Operation.h"
#include <vector>

namespace ami
{
	class Pattern
	{
	public:
		// A pattern consists on a series of rounds
		// Each round consists a series of operation types

		Pattern(const QString & name = "", const std::vector<Operations> & rounds = std::vector<Operations>())
			:
			m_name(name),
			m_rounds(rounds)
		{}

		const std::vector<Operations> & getRounds() const {
			return m_rounds;
		}

		std::vector<Operations> & getRounds() {
			return m_rounds;
		}

		void add(const Operations & round)
		{
			m_rounds.push_back(round);
		}

		void setName(const QString & name)
		{
			m_name = name;
		}

		QString name() { return m_name; }

	private:
		QString m_name;
		std::vector<Operations> m_rounds;
	};
}