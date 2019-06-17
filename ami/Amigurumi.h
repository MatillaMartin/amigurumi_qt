#pragma once

#include "ami/Pattern.h"
#include "ami/PatternGraph.h"

#include <map>
#include <set>
#include <QtXml>
#include <memory>

namespace ami
{
	class Amigurumi
	{
	public:
		struct Command
		{
			virtual void run(Amigurumi & ami) = 0;
		};
		struct PatternCommand : public Command
		{
			PatternCommand(const Pattern & pattern, const QString & name) : m_pattern(pattern), m_name(m_name) {};
			virtual void run(Amigurumi & ami) override;

			Pattern m_pattern;
			QString m_name;
		};
		struct CopyCommand : public Command
		{
			CopyCommand(const QString & from, const QString & make) : m_from(from), m_make(make) {};
			virtual void run(Amigurumi & ami) override;

			QString m_from;
			QString m_make;
		};
		struct JoinCommand : public Command
		{
			JoinCommand(const QString & from, const QString & with, const QString & make) : m_from(from), m_with(with), m_make(make) {};
			virtual void run(Amigurumi & ami) override;

			QString m_from;
			QString m_with;
			QString m_make;
		};

		Amigurumi();

		/// Returns specified pattern if found
		bool pattern(const QString & pattern, PatternGraph & outPattern) 
		{ 
			auto it = m_patterns.find(pattern);
			if (it != m_patterns.end())
			{
				outPattern = it->second;
				return true;
			}

			return false;
		}

		/// Returns last pattern
		bool pattern(PatternGraph & outPattern) { return pattern(*m_patternOrder.end(), outPattern); }

		/// Adds pattern. If it exists, overwrites the previous pattern
		void addPattern(QString name, const PatternGraph & pattern);

		// Adds a command
		void addCommand(std::unique_ptr<Command> command);

		static Amigurumi fromFile(const std::string & file);

	private:
		static std::unique_ptr<Command> getCommand(const QString & type, QDomElement & data);
		static std::unique_ptr<PatternCommand> makePatternCommand(QDomElement & data);
		static std::unique_ptr<CopyCommand> makeCopyCommand(QDomElement & data);
		static std::unique_ptr<JoinCommand> makeJoinCommand(QDomElement & data);

		QString generatePatternName();

		std::map<QString, PatternGraph> m_patterns;
		std::vector<std::unique_ptr<Command>> m_commands;
		std::vector<QString> m_patternOrder;
	};

}
