#include <iostream>
#include <typeinfo>

#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/support_attributes.hpp>

namespace qi = boost::spirit::qi;

struct ast
{
};

template <typename Iterator>
struct skipper
 : qi::grammar<Iterator>
{
};

template <typename Iterator>
struct grammar
 : qi::grammar<Iterator, ast(), skipper<Iterator>>
{
};

template <typename T>
void test()
{
   typedef typename boost::spirit::traits::attribute_of<T>::type
      attribute_type;

   typedef typename T::skipper_type skipper_type;
}

int main()
{
   typedef std::string::const_iterator iterator_type;
   typedef grammar<iterator_type> grammar_type;

   test<grammar_type>();
}
