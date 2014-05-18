#ifndef _TEMPLATE_PARSER_HPP_
#define _TEMPLATE_PARSER_HPP_ 0x100

#include <string>
#include <algorithm>

namespace template_engine
{

namespace details
{

// Pattern matching junk I assume will be fixed with Spirit

static const std::string kStartDelimiter = "{{";
static const std::string kFinishDelimiter = "}}";

template<typename Iterator>
bool startsWith(const std::string pattern, Iterator begin, Iterator end)
{
	return std::distance(begin, end) >= pattern.size() && std::equal(pattern.begin(), pattern.end(), begin);
}

template<typename Iterator>
bool isStartDelimiter(Iterator begin, Iterator end)
{
	return startsWith(kStartDelimiter, begin, end);
}

template<typename Iterator>
bool isEndDelimiter(Iterator begin, Iterator end)
{
	return startsWith(kFinishDelimiter, begin, end);
}

template<typename Iterator>
std::pair<Iterator, Iterator> getTag(Iterator begin, Iterator end)
{
	if (!isStartDelimiter(begin, end))
	{
		return {begin, begin};
	}
	std::advance(begin, 2);
	auto tag_begin = begin;
	// found start delimiter
	while (!isEndDelimiter(begin, end))
	{
		if (begin == end)
		{
			throw std::runtime_error("incorrect tag");
		}
		++begin;
	}
	return {tag_begin, begin};
}

// assuming it is already in a section
template<typename Iterator>
Iterator getSectionEnd(Iterator begin, Iterator end, const std::string& sectionName)
{
	auto sectionEnd = std::string("{{/") + sectionName + "}}";
	while (std::distance(begin, end) >= sectionEnd.size() && !equal(sectionEnd.begin(), sectionEnd.end(), begin))
	{
		++begin;
	}
	if (std::distance(begin, end) < sectionEnd.size())
	{
		throw std::runtime_error("incorrect closing tag");
	}
	return begin;
}

} // namespace details

} // namespace template_engine

#endif // _TEMPLATE_PARSER_HPP_
