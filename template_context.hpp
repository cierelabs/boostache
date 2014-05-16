#ifndef _TEMPLATE_CONTEXT_HPP_
#define _TEMPLATE_CONTEXT_HPP_ 0x100

#include <list>
#include <map>
#include <string>
#include <vector>


#include "template_object_interface.hpp"

// Context is an example of how we could create a class that can be used by boostache
class Context;

class Context
{
	// mapping of available variables
	std::map<std::string, std::string> mVariables;
	// mapping of available subcontext
	std::map<std::string, std::vector<Context>> mSubContext;

public:
	// overrides a context
	void addVariable(std::string key, std::string value)
	{
		mVariables[key] = value;
	}

	void addSubContext(std::string key, std::vector<Context> value)
	{
		mSubContext[key] = value;
	}

	bool hasVariable(std::string key) const
	{
		return mVariables.find(key) != mVariables.end();
	}

	// NOTE! Should this just return ""?
	std::string getVariable(std::string key) const
	{
		auto v = mVariables.find(key);
		if (v == mVariables.end())
			throw std::runtime_error("variable not found");
		return v->second;
	}

	bool hasSubContext(std::string key) const
	{
		auto v = mSubContext.find(key);
		return (v != mSubContext.end());
	}

	std::pair<std::vector<Context>::const_iterator, std::vector<Context>::const_iterator>
	getSubContext(std::string key) const
	{
		auto v = mSubContext.find(key);
		if (v == mSubContext.end())
			throw std::runtime_error("variable not found");
		return { v->second.begin(), v->second.end() };
	}

};


struct Context_list_iterator_t;

struct Context_list_t
{
	Context_list_t(const Context& c) { mData.push_front(c); }
	std::list<Context> mData;
	typedef Context_list_iterator_t Iterator;
};

struct Context_list_iterator_t
{
	Context_list_iterator_t(std::vector<Context>::const_iterator i, const Context_list_t& p) : iter(i), parent_context(p) {}
	bool operator==(const Context_list_iterator_t& rhs) const
	{
		return iter == rhs.iter;
	}

	bool operator!=(const Context_list_iterator_t& rhs) const
	{
		return iter != rhs.iter;
	}

	Context_list_iterator_t& operator++()
	{
		++iter;
		return *this;
	}

	Context_list_t operator*() const
	{
		auto result = parent_context;
		result.mData.push_front(*iter);
		return result;
	}

	std::vector<Context>::const_iterator iter;
	const Context_list_t& parent_context;
};

// NOTE!  putting this in the other namespace?
namespace template_engine {

template<>
std::pair<Context_list_iterator_t, Context_list_iterator_t> GetRange<Context_list_t>(const Context_list_t& data, const std::string& key)
{
	for (const auto& i : data.mData)
	{
		if (i.hasSubContext(key))
		{
			auto result = i.getSubContext(key);
			typename Context_list_t::Iterator begin(result.first, data);
			typename Context_list_t::Iterator end(result.second, data);
			return { begin, end };
		}
	}
	throw std::runtime_error("section not found");
}


template <>
void Render<Context_list_t>(const Context_list_t& data, std::ostream& os, const std::string& key)
{
	for (const auto& i : data.mData)
	{
		if (i.hasVariable(key))
		{
			os<<i.getVariable(key);
			return;
		}
	}
	return ;
}

template <>
bool HasKey<Context_list_t>(const Context_list_t& data, const std::string& key)
{
	for (const auto& i : data.mData)
	{
		if (i.hasVariable(key) || i.hasSubContext(key))
		{
			return true;
		}
	}
	return false;
}



} // namespace template_engine

#endif // _TEMPLATE_CONTEXT_HPP_
