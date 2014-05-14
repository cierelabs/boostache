#include "liaw2014/renderer.hpp"

namespace liaw2014
{
	Renderer::Renderer(RenderAST ast)
		: m_ast(std::move(ast))
	{
	}

	std::string Renderer::render(const RenderData& data)
	{
		return m_ast.text();
	}
}
