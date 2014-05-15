#include "liaw2014/render_ast.hpp"

namespace liaw2014
{
	void RenderAST::set_template_text(std::string template_text)
	{
		m_template_text = std::move(template_text);
	}
	void RenderAST::add_literal(const std::string& text)
	{
		m_nodes.push_back(ASTNodePtr(new Literal { text }));

		m_template_text += text;
	}
	void RenderAST::add_substitution(const std::string& tag)
	{
		m_nodes.push_back(ASTNodePtr(new Substitution { tag }));
		m_template_text += "{{" + tag + "}}";
	}
	std::string RenderAST::template_text()
	{
		return m_template_text;
	}
}
