#include <string>
#include <map>
#include <ostream>

class TextTemplateEngine {
public:
  
  TextTemplateEngine(std::string templ="") :
    templ_(templ)
  {}
  
  ///Direct way to set an individual tag
  void add(std::string key, std::string value)
  {
    tags_.emplace(key, value);
  }
  

  template<class CollType> 
  void add(std::string key, const CollType& coll)
  {}

  //  void add(std::string key, std::function);
  void render(std::ostream& os) const
  {
    std::string output = templ_;
    auto end = tags_.end();
    auto itr = tags_.begin();
    while (itr != end) {

    }
    os << templ_;
  }
  
  ///Load a template file from a path replacing any
  ///existing template
  void load_file(std::string path)
  {}

  ///Load a template from a string replacing any
  ///existing template
  void load(std::string templ)
  {
    templ_ = templ;
  }

private:
  std::map<std::string, std::string> tags_;
  //  std::map<std::string, ?? sections
  std::string templ_;
};
