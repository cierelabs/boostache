#pragma once

#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <string>
#include <map>

namespace boost
{
	namespace cppte
	{
		namespace front_end
		{
			typedef boost::make_recursive_variant<
				std::string,
				std::map<std::string, boost::recursive_variant_>
				>::type stache_variant;

			typedef std::map<std::string, stache_variant> stache_model;
		}
	}
}
