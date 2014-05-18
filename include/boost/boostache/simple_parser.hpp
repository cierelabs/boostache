#ifndef _SIMPLE_PARSER_HPP_
#define _SIMPLE_PARSER_HPP_ 0x100

#include <boost/boostache/frontend/stache_ast.hpp>
#include <iosfwd>

namespace boost { namespace boostache 
{
   namespace frontend 
   {
      bool simple_parse(std::istream& input, ast::stache_root& ast);
      bool simple_parse(const std::string& input, ast::stache_root& ast);
   }

   using boostache::frontend::ast::stache_root;

   template <typename InputType>
   bool simple_parse_template(InputType&& input, stache_root& ast)
   {
      return boostache::frontend::simple_parse(std::forward<InputType>(input), ast);
   }
}}

#endif // _SIMPLE_PARSER_HPP_
