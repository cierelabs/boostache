#pragma once


#define TEST_F(fixture_, name_) \
	    BOOST_FIXTURE_TEST_CASE(plustache_##name_, fixture_)
#define EXPECT_EQ(lhs, rhs) BOOST_CHECK_EQUAL(lhs, rhs)

namespace testing
{
	struct Test {};
}
