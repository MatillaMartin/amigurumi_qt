#pragma once
#include <map>

#include <QObject>

namespace ami
{
	class PatternGraph;
	
	namespace Operation
	{
		Q_NAMESPACE
		enum Type {
			// loop
			LP,
			// chain
			CH,
			// single crochet
			SC,
			// increase
			INC,
			// decrease
			DEC,
			// magic ring
			MR,
			// finish off
			FO,
			// slip stitch
			SLST,
			// join
			JOIN,
			// skip
			SKIP,
			// nothing
			NONE
		};
		Q_ENUM_NS(Type)
	}

	typedef std::vector<Operation::Type> Operations;
}