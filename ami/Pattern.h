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

		Pattern(const std::vector<Operations> & rounds = std::vector<Operations>())
			:
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

		void clear()
		{
			m_rounds.clear();
		}

	private:
		std::vector<Operations> m_rounds;
	};
}