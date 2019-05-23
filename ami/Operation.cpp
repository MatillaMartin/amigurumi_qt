#pragma once
#include <map>

#include <QObject>
#include <QMetaEnum>
#include "Operation.h"

namespace ami
{
	namespace Operation
	{
		bool getOperations(const QString & op, unsigned int count, Operations & outOps)
		{
			// cast enum int to the corresponding type
			auto metaEnum = QMetaEnum::fromType<Operation::Type>();
			int enumIndex = metaEnum.keyToValue(op.toStdString().c_str());

			if (enumIndex > metaEnum.keyCount())
			{
				Operation::Type opType = static_cast<Operation::Type>(enumIndex);
				outOps.insert(outOps.end(), count, opType);
				return true;
			}
			
			return false;
		}
	}
}