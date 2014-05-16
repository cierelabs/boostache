#pragma once

#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <string>
#include <map>
#include <functional>

namespace boost
{
	namespace cppte
	{
		namespace front_end
		{
			typedef std::function<std::string()> stache_function;

			typedef boost::make_recursive_variant<
				std::string,
				stache_function,
				std::map<std::string, boost::recursive_variant_>
				>::type stache_variant;

			typedef std::map<std::string, stache_variant> stache_model;
		}
	}
}
