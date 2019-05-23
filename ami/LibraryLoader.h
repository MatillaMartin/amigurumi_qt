#pragma once
#include <QString>

namespace ami
{
	class LibraryLoader
	{
	public:
		LibraryLoader();

		QStringList getFiles(const QString & path);
	};
}