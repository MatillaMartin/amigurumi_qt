#include "ami/LibraryLoader.h"

#include <QDir>

namespace ami
{
	LibraryLoader::LibraryLoader() {}

	QStringList LibraryLoader::getFiles(const QString & path)
	{
		QDir directory(path);
		return directory.entryList(QStringList() << "*.ami", QDir::Files);
	}
}
