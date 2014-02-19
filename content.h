#ifndef CONTENT_H
#define CONTENT_H

#include <string>

namespace Kyubu
{
namespace Content
{
	template <class T>
	T Load(std::string assetName)
	{
		return T::FromFile(assetName);
	}
}
}

#endif
