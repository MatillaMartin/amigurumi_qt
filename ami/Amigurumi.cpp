#include "Amigurumi.h"
#include "Operation.h"

#include <unordered_map>
#include <iostream>
#include <functional>

#include <QFile>

namespace ami
{
	Amigurumi::Amigurumi()
	{
		
	}

	void Amigurumi::addPattern(QString name, const PatternGraph & pattern)
	{
		// if pattern name is empty, generate a name for the pattern
		if (name.isEmpty())
		{
			name = generatePatternName();
		}

		// insert or override if it exists
		m_patterns[name] = pattern;
	}

	void Amigurumi::addCommand(std::unique_ptr<Command> command)
	{
		m_commands.push_back(std::move(command));
	}

	QString Amigurumi::generatePatternName()
	{
		return QString("Pattern" + m_patterns.size());
	}

	Amigurumi Amigurumi::fromFile(const std::string & file)
	{
		Amigurumi ami;

		const static QString tagAmi = "Amigurumi";
		const static QString tagCommand = "Command";

		//The QDomDocument class represents an XML document.
		QDomDocument data;
		// Load xml file as raw data
		QFile f(QString::fromStdString(file));
		if (!f.open(QIODevice::ReadOnly))
		{
			// Error while loading file
			throw std::runtime_error("File " + file + " not found");
		}
		// Set data into the QDomDocument before processing
		data.setContent(&f);
		f.close();

		QDomElement root = data.documentElement();
		QDomElement amiElem = root.firstChild().toElement();

		if (amiElem.tagName() == tagAmi)
		{
			// get command
			QDomElement commandElem = amiElem.firstChild().toElement();
			while (!commandElem.isNull() && commandElem.tagName() == tagCommand)
			{
				QString type = commandElem.attribute("type");
				try
				{
					ami.addCommand(getCommand(type, commandElem));
				}
				catch (std::runtime_error & e)
				{
					throw std::runtime_error("Error reading Command, line:" + std::to_string(commandElem.lineNumber()) + ", Command of type " + type.toStdString() + " failed: " + e.what());
				}

				// get next command
				commandElem = commandElem.nextSibling().toElement();
			}
		}
		else
		{
			throw std::runtime_error("Amigurumi tag not found");
		}

		return ami;
	}

	std::unique_ptr<Amigurumi::Command> Amigurumi::getCommand(const QString & type, QDomElement & data)
	{
		static std::unordered_map<std::string, std::function<std::unique_ptr<Amigurumi::Command>(QDomElement &)>> commandFactory
		{
			{ "Pattern", [](QDomElement & data) { return Amigurumi::makePatternCommand(data); } },
			{ "Copy", [](QDomElement & data) { return Amigurumi::makeCopyCommand(data); } },
			{ "Join", [](QDomElement & data) { return Amigurumi::makeJoinCommand(data); } },
		};

		std::unique_ptr<Command> command;

		auto & it = commandFactory.find(type.toStdString());
		if (it != commandFactory.end())
		{
			command = std::move(it->second(data)); // run command
		}
		else
		{
			throw std::runtime_error("Type " + type.toStdString() + " is not a valid command");
		}

		return command;
	}

	std::unique_ptr<Amigurumi::PatternCommand> Amigurumi::makePatternCommand(QDomElement & data)
	{
		Pattern pattern;
		QString name;
		const static QString tagOperation = "Operation";
		const static QString tagRound = "Round";
		const static QString tagPattern = "Pattern";
		
		QDomElement patternElem = data.firstChild().toElement();

		if (patternElem.tagName() == tagPattern)
		{
			name = patternElem.attribute("name").toUInt();
			// get round

			QDomElement roundElem = patternElem.firstChild().toElement();
			while (!roundElem.isNull() && roundElem.tagName() == tagRound)
			{
				Operations roundOps;
				QDomElement opElem = roundElem.firstChild().toElement();
				// for each operation in the row
				while (!opElem.isNull() && opElem.tagName() == tagOperation)
				{
					QString type = opElem.attribute("type");
					unsigned int count = opElem.attribute("count").toUInt();
					if (!type.isEmpty() && count > 0)
					{
						// get operations
						Operations ops;
						if (Operation::getOperations(type, count, ops))
						{
							roundOps.insert(roundOps.end(), ops.begin(), ops.end());
						}
					}
					else
					{
						throw std::runtime_error("Invalid operation at line: " + std::to_string(opElem.lineNumber()) + ", Pattern: " + name.toStdString());
					}

					opElem = opElem.nextSiblingElement();
				}

				pattern.add(roundOps);

				// get next command
				roundElem = roundElem.nextSibling().toElement();
			}
		}
		else
		{
			throw std::runtime_error("Pattern tag not found");
		}
		
		return std::make_unique<PatternCommand>(pattern, name);
	}

	std::unique_ptr<Amigurumi::CopyCommand> Amigurumi::makeCopyCommand(QDomElement & data)
	{
		const static QString tagCopy = "Copy";

		QDomElement copyElem = data.firstChild().toElement();

		if (copyElem.tagName() != tagCopy)
		{
			throw std::runtime_error("Invalid Copy command, missing Copy tag");
		}

		QString from = copyElem.attribute("from");
		QString make = copyElem.attribute("make");

		if (from.isEmpty() || make.isEmpty())
		{
			throw std::runtime_error("Invalid Copy command, missing \"from\" or \"make\" attribute");
		}

		return std::make_unique<CopyCommand>(from, make);
	}

	std::unique_ptr<Amigurumi::JoinCommand> Amigurumi::makeJoinCommand(QDomElement & data)
	{
		const static QString tagJoin = "Join";

		QDomElement joinElem = data.firstChild().toElement();

		if (joinElem.tagName() != tagJoin)
		{
			throw std::runtime_error("Invalid Join command, missing Join tag");
		}

		QString from = data.attribute("from");
		QString with = data.attribute("with");
		QString make = data.attribute("make");

		if (from.isEmpty() || with.isEmpty() || make.isEmpty())
		{
			throw std::runtime_error("Invalid Join command, missing \"pattern\", \"with\" or \"make\" attributes");
		}

		return std::make_unique<JoinCommand>(from, with, make);
	}

	void Amigurumi::PatternCommand::run(Amigurumi & ami)
	{
		PatternGraph pattern;
		// check if pattern exists. If it exists, append
		if (ami.pattern(m_name, pattern))
		{
			pattern.append(m_pattern);
			ami.addPattern(m_name, pattern);
		}
		else
		{
			ami.addPattern(m_name, m_pattern);
		}
	}

	void Amigurumi::CopyCommand::run(Amigurumi & ami)
	{
		PatternGraph pattern;
		if (!ami.pattern(m_from, pattern))
		{ 
			throw std::runtime_error("Invalid Copy command, pattern " + m_from.toStdString() + " to copy does not exist");
		}
		
		ami.addPattern(m_make, pattern);
	}

	void Amigurumi::JoinCommand::run(Amigurumi & ami)
	{
		PatternGraph startPattern;
		PatternGraph joinWith;
		if (!ami.pattern(m_from, startPattern) || !ami.pattern(m_with, joinWith))
		{
			throw std::runtime_error("Invalid Join command, " + m_from.toStdString() + " or " + m_with.toStdString() + " patterns do not exist");
		}
		
		startPattern.join(joinWith);
		ami.addPattern(m_make, startPattern);
	}
}