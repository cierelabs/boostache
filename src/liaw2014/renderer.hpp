#pragma once
#include "liaw2014/render_ast.hpp"
#include "liaw2014/render_data.hpp"
#include <string>

namespace liaw2014
{
	class Renderer
	{
	public:
		Renderer(RenderAST ast);
		std::string render(const RenderData& data);
	private:
		RenderAST m_ast;
	};
}
