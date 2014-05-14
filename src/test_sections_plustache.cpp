#include <string>
#include <iostream>
#include <fstream>
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "template.hpp"
#include "plustache_types.hpp"
#include "gmock_to_boost.hpp"

// The fixture for testing class Foo.
class SectionsTest : public ::testing::Test
{
 public:
    std::string result_string;
    std::string result_file;
    std::string template_string;
    PlustacheTypes::ObjectType ctx;
    std::string file;

    SectionsTest()
    {
		 SetUp();
    }

    virtual ~SectionsTest()
    {
		 TearDown();
    }

    virtual void SetUp()
    {
        template_string = "Hi I am {{name}}.\n";
        template_string += "{{# showme}}";
        template_string += "I like {{pet}}.";
        template_string += "{{/ showme}}";
        template_string += "{{# dontshowme}}";
        template_string += "If you see this, something went wrong.";
        template_string += "{{/ dontshowme}}";
        file = "sections.mustache";

        std::ofstream myfile;
        myfile.open (file.c_str());
        myfile << template_string;
        myfile.close();

        ctx["name"] = "Daniel";
        ctx["pet"] = "turtles";
        ctx["showme"] = "true";
        ctx["dontshowme"] = "false";
        Plustache::template_t t;
        result_string = t.render(template_string, ctx);
        result_file = t.render(file, ctx);
    }

    virtual void TearDown()
    {
        remove(file.c_str());
    }

};

// Tests that a simple mustache tag is replaced
TEST_F(SectionsTest, TestSectionsMustacheFromString)
{
    std::string expected = "Hi I am Daniel.\n";
          expected += "I like turtles.";
    EXPECT_EQ(expected, result_string);
}

TEST_F(SectionsTest, TestSectionsMustacheFromFile)
{
    std::string expected = "Hi I am Daniel.\n";
          expected += "I like turtles.";
    EXPECT_EQ(expected, result_file);
}
