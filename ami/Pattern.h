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
		typedef Operations Round;

		Pattern()
		{
		}

		const std::vector<Round> & getRounds() const {
			return m_rounds;
		}

		std::vector<Round> & getRounds() {
			return m_rounds;
		}

		void add(const Round & round)
		{
			m_rounds.push_back(round);
		}

	private:
		std::vector<Round> m_rounds;
	};
}