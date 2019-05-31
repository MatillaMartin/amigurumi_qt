#include <map>

#include <QObject>
#include <QMetaEnum>
#include "Operation.h"

namespace ami
{
	namespace Operation
	{
		bool getOperation(const QString & op, Operation::Type & outOp)
		{
			// cast enum int to the corresponding type
			auto metaEnum = QMetaEnum::fromType<Operation::Type>();
			int enumIndex = metaEnum.keyToValue(op.toStdString().c_str());

			if (enumIndex >= 0 && enumIndex < metaEnum.keyCount())
			{
				outOp = static_cast<Operation::Type>(enumIndex);
				return true;
			}
			
			return false;
		}
	}
}
