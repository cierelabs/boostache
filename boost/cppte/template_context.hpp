#ifndef _TEMPLATE_CONTEXT_HPP_
#define _TEMPLATE_CONTEXT_HPP_ 0x100

#include <list>
#include <map>
#include <string>
#include <vector>

namespace template_engine
{

class Context;

using Context_list_t = std::list<Context>;

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

	// Should we have this?
	void removeVariable(std::string key)
	{
		mVariables.erase(key);
	}

	void addSubContext(std::string key, std::vector<Context> value)
	{
		mSubContext[key] = value;
	}
	// NOTE! Now what? do we allow you to remove some of the context?
	void removeSubContext(std::string key)
	{
		mSubContext.erase(key);
	}
private:
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
	// NOTE! Should this just return {}?
	std::vector<Context> getSubContext(std::string key) const
	{
		auto v = mSubContext.find(key);
		if (v == mSubContext.end())
			throw std::runtime_error("variable not found");
		return v->second;
	}

	friend std::string getVariable(const Context_list_t& data, const std::string& variableName);
	friend std::vector<Context> getSection(const Context_list_t& data, const std::string& sectionName);
};


std::vector<Context> getSection(const Context_list_t& data, const std::string& sectionName)
{
	for (const auto& i : data)
	{
		if (i.hasSubContext(sectionName))
		{
			return i.getSubContext(sectionName);
		}
	}
	//throw std::runtime_error("section not found");
	// NOTE! return empty instead
	return {};
}

std::string getVariable(const Context_list_t& data, const std::string& variableName)
{
	for (const auto& i : data)
	{
		if (i.hasVariable(variableName))
		{
			return i.getVariable(variableName);
		}
	}
	//throw std::runtime_error("variable not found");
	// NOTE! print something instead
	return "";
}

} // namespace template_engine

#endif // _TEMPLATE_CONTEXT_HPP_
