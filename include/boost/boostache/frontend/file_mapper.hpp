/**
 *  \file file_mapper.hpp
 *
 *  Copyright 2015 Michele Santullo
 *
 *  Utility class that maps a partial name to a file on the filesystem.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_BOOSTACHE_FILE_MAPPER_HPP
#define BOOST_BOOSTACHE_FILE_MAPPER_HPP

#include <string>
#include <utility>
#include <fstream>

namespace boost { namespace boostache { namespace frontend
{
template <typename CharType>
class file_mapper
{
public:
   using string_type = std::basic_string<CharType>;

   file_mapper (file_mapper&&) = default;
   file_mapper (const file_mapper&) = default;
   explicit file_mapper (string_type&& ext=".mustache", string_type&& base_path="") :
      extension(std::move(ext)),
      base_path(std::move(base_path))
   {
   }

   void set_extension (string_type&& ext)
   {
      extension = std::move(ext);
   }
   void set_base_path (string_type&& bpath)
   {
      base_path = std::move(bpath);
      if (!base_path.empty() && base_path.back() != static_cast<CharType>('/') && base_path.back() != static_cast<CharType>('\\'))
         base_path += '/';
   }

   string_type operator() (const string_type& tag) const
   {
      //TODO: don't open the file every time, add some sort of buffering instead
      string_type path(base_path + tag + extension);
      std::basic_ifstream<CharType> ifs(path);
      ifs.seekg(0, std::ios_base::end);
      const auto ssize = ifs.tellg();
      ifs.seekg(0, std::ios_base::beg);

      string_type retval;
      retval.resize(ssize, ' ');
      ifs.read(&*retval.begin(), ssize);
      ifs.close();
      return retval;
   }

private:
   string_type extension;
   string_type base_path;
};
}}}
#endif
