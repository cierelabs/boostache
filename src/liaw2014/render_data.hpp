#pragma once
#include <string>
#include <map>

namespace liaw2014
{
	class RenderData
	{
		public:
			std::string& operator[](const std::string& key)
			{
				return m_data[key];
			}

		private:
			std::map<std::string, std::string> m_data;
	};
}
