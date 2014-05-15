#pragma once
#include <string>
#include <memory>
#include <vector>

namespace liaw2014
{
	struct ASTNode;
	typedef std::shared_ptr<ASTNode> ASTNodePtr;

	struct ASTNode
	{
		virtual ~ASTNode()
		{
		}
	};

	struct Literal : public ASTNode
	{
		Literal(const std::string& t) : text(t) { }
		std::string text;
	};
	struct Substitution : public ASTNode
	{
		Substitution(const std::string& t) : tag(t) { }
		std::string tag;
	};

	class RenderAST
	{
	public:
		RenderAST()
		{
		}

		RenderAST(const std::string& template_text)
			: m_template_text(template_text)
		{
		}

		static RenderAST load_file(const std::string& path)
		{
			return RenderAST();
		}

		void set_template_text(std::string template_text);

		void add_literal(const std::string& text);
		void add_substitution(const std::string& tag);
		std::string template_text();

		template <typename Fn>
		void visit(Fn callback)
		{
			for( auto i : m_nodes )
			{
				callback(*i);
			}
		}
	private:
		std::vector<ASTNodePtr> m_nodes;

		std::string m_template_text;
	};
}
