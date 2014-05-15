#ifndef _TEMPLATE_GENERATE_HPP_
#define _TEMPLATE_GENERATE_HPP_ 0x100

#include "template_context.hpp"
#include <ostream>

namespace template_engine
{

template<typename Iterator>
std::ostream& render_helper(std::ostream& os, const Context_list_t& data, Iterator begin, Iterator end)
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
					auto section_end_ptr = getSectionEnd(begin, end, sectionName);
					auto theSections = getSection(data, sectionName);
					if (*tag_begin == '#')
					{
						for (auto& section : theSections)
						{
							// have new state
							auto new_data = data;
							new_data.push_front(section);
							render_helper(os, new_data, begin, end);
						}
					}
					else if ((*tag_begin == '^') && theSections.empty())
					{
						render_helper(os, data, begin, end);
					}
					// move past section end;
					begin = section_end_ptr;
				}
					break;
				case '!':
				{
				}
					break;
				default:
				{
					std::string variableName(tag.first, tag.second);
					os << getVariable(data, variableName);
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

template<typename Iterator>
std::ostream& render(std::ostream& os, const Context& model, Iterator begin, Iterator end)
{
	Context_list_t data{model};
	return render_helper(os, data, begin, end);
}

} // namespace template_engine

#endif // _TEMPLATE_GENERATE_HPP_
