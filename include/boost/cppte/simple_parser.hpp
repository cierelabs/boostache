#ifndef _SIMPLE_PARSER_HPP_
#define _SIMPLE_PARSER_HPP_ 0x100

#include <boost/cppte/frontend/stache_ast.hpp>
#include <iosfwd>

namespace boost { namespace cppte 
{
   namespace front_end 
   {
      bool simple_parse(std::istream& input, ast::stache_root& ast);
      bool simple_parse(const std::string& input, ast::stache_root& ast);
   }

   using cppte::front_end::ast::stache_root;

   template <typename InputType>
   bool simple_parse_template(InputType&& input, stache_root& ast)
   {
      return cppte::front_end::simple_parse(std::forward<InputType>(input), ast);
   }
}}

#endif // _SIMPLE_PARSER_HPP_
