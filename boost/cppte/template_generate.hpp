#ifndef _TEMPLATE_GENERATE_HPP_
#define _TEMPLATE_GENERATE_HPP_ 0x100

#include "template_context.hpp"
#include "template_object_interface.hpp"
#include "template_parser.hpp"
#include <ostream>

namespace template_engine
{

template<typename T, typename Iterator>
std::ostream& render_helper(std::ostream& os, const T& data, Iterator begin, Iterator end)
{
	using namespace template_engine::details;

	while (begin != end)
	{
		auto tag = getTag(begin, end);
		if (tag.first != tag.second)
		{
			auto tag_begin = tag.first;
			begin = tag.second;
			begin += 2;
			switch (*tag_begin)
			{
				case '#': case '^':
				{
					std::string sectionName(tag.first+1, tag.second);
					if (HasKey(data, sectionName))
					{
						auto section_end_ptr = getSectionEnd(begin, end, sectionName);
						auto theSections = GetRange(data, sectionName);
						auto section_begin = theSections.first;
						auto section_end = theSections.second;
						if (*tag_begin == '#')
						{
							while( section_begin != section_end )
							{
								render_helper(os, *section_begin, begin, end);
								++section_begin;
							}
						}
						else if ((*tag_begin == '^') && (section_begin == section_end))
						{
							render_helper(os, data, begin, end);
						}
						// move past section end;
						begin = section_end_ptr;
					}
				}
					break;
				case '!':
				{
				}
					break;
				default:
				{
					Render(data, os, {tag.first, tag.second});
				}
			}
		}
		else
		{
			os<<*begin++;
		}
	}
	return os;
}

template<typename T, typename Iterator>
std::ostream& render(std::ostream& os, const T& model, Iterator begin, Iterator end)
{
	Context_list_t data{model};
	return render_helper(os, data, begin, end);
}

} // namespace template_engine

#endif // _TEMPLATE_GENERATE_HPP_
