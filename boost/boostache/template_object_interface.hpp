#ifndef _TEMPLATE_OBJECT_INTERFACE_HPP_
#define _TEMPLATE_OBJECT_INTERFACE_HPP_ 0x100

#include <string>
#include <ostream>

namespace template_engine
{

template <typename T>
std::pair<typename T::Iterator, typename T::Iterator> GetRange(const T&, const std::string& key);

template <typename T>
bool HasKey(const T&, const std::string& key)
{
	return false;
}

template <typename T>
void Render(const T&, std::ostream& os, const std::string& key)
{
}

}

#endif // _TEMPLATE_OBJECT_INTERFACE_HPP_
