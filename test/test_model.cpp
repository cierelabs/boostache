#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/cppte/frontend/stache_model.hpp>

using namespace boost::cppte::front_end;

BOOST_AUTO_TEST_CASE(test_model_build)
{
	stache_model model;
	model["NAME"] = "Bob";
	model["LOCATION"] = stache_model { { "CITY", "Aspen" }, { "STATE", "Colorado" }, { "COUNTRY", "United States" } };

	BOOST_CHECK_EQUAL("Bob", boost::get<std::string>(model["NAME"]));
	BOOST_CHECK_THROW(boost::get<int>(model["NAME"]), boost::bad_get);
	BOOST_CHECK_EQUAL("Aspen", boost::get<std::string>(boost::get<stache_model>(model["LOCATION"])["CITY"]));
}
