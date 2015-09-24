/**
 *  \file test/mustache/parser_test.cpp
 *
 *  Test driver for parsers. Calls parsers on input file, generates a text output from
 *  the AST and then compares that result to the contents of an expected file.
 *
 *  Simply set the value of a global std::string (test_dir) to the location
 *  of where to find the input and expect files. The file pairs must be named [file].input
 *  and [file].expect. Add more files to the test_dir will add the additional test.
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_TEST_NO_MAIN

#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <string>
#include <fstream>
#include <iostream>


/**
 *  Implemented by the specific parser test. Filename is the
 *  input file. The function will parse the contents of
 *  the input file and then convert the resulting AST
 *  into a std::string representation. The representation
 *  is what the expect file contains.
 */
std::string print_ast(std::string const & filename);


void test_parse(std::string const & filename)
{
   auto input_ast = print_ast(filename+".input");

   std::ifstream expect_stream(std::string(filename + ".expect").c_str());
   if(!expect_stream)
   {
      BOOST_CHECK_MESSAGE(false, "Failed to open expect file for " << filename);
      return;
   }

   std::string expected( std::istreambuf_iterator<char>{expect_stream}
                       , std::istreambuf_iterator<char>{});

   BOOST_CHECK_EQUAL(input_ast,expected);
}


extern std::string test_dir;

bool init_unit_test_suite()
{
   if(!boost::filesystem::is_directory(test_dir.c_str()))
   {
      return false;
   }

   std::vector<std::string> test_files;

   boost::filesystem::directory_iterator dir_iter(test_dir);
   boost::filesystem::directory_iterator end_dir_iter;

   std::for_each( dir_iter, end_dir_iter
               , [&test_files](boost::filesystem::path file)
                 {
                    if(file.extension() == ".input")
                    {
                       file.replace_extension("");
                       test_files.push_back(file.string());
                    }
                 } );

   std::cout << "Found " << test_files.size() << " test cases." << std::endl;
   for(auto & test : test_files)
   {
      std::cout << "   " << test << std::endl;
   }

   boost::unit_test::framework::master_test_suite().add( BOOST_PARAM_TEST_CASE( &test_parse
                                                                              , test_files.begin()
                                                                              , test_files.end() ) );

   return true;
}

int main(int argc, char* argv[])
{
    return boost::unit_test::unit_test_main(init_unit_test_suite, argc, argv);
}
