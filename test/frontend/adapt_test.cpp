// file used during initial dev
// will be removed
#include <boost/boostache/frontend/parse.hpp>
#include <boost/boostache/stache.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp>
#include <string>

namespace boostache = boost::boostache;
using boostache::frontend::parse;


int main()
{
   std::string input("foo");
   auto iter = input.begin();
   auto ast = parse<boostache::format::stache>(iter,input.end());
}
