#pragma once
#include <string>

namespace liaw2014
{
	class RenderAST
	{
	public:
		void set_text(std::string text);
		std::string text();
	private:
		std::string m_text;
	};
}
