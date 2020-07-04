#include "LibraryLoader.h"

#include <QDir>

namespace ami
{
	LibraryLoader::LibraryLoader() {}

	QStringList LibraryLoader::getFiles(const QString & path)
	{
		QFileInfo fileInfo(path);
		if (fileInfo.isDir())
		{
			QDir directory(path);
			return directory.entryList(QStringList() << "*.ami", QDir::Files);
		}
		else
		{
			return QStringList() << path;
		}
	}
}
