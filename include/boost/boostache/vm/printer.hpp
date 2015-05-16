/**
 *  \file vm/printer.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *  Copyright 2015 Michal Bukovsky
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_BOOSTACHE_VM_PRINTER_HPP
#define BOOST_BOOSTACHE_VM_PRINTER_HPP

#include <boost/variant/apply_visitor.hpp>
#include <boost/boostache/vm/engine_ast.hpp>

namespace boost { namespace boostache {
namespace vm { namespace ast { namespace detail {

class printer
{
public:
    typedef void result_type;

    printer(std::ostream &out)
        : out{out}, decorations{true}
    {}

    void operator()(undefined)
    {
        print_decoration();
        out << "[<undefined>]" << std::endl;
    }

    void operator()(literal const & v)
    {
        print_decoration();
        out << "[<literal>: \"" << shrink(v.value) << "\"]" << std::endl;
    }

    void operator()(variable const & v)
    {
        print_decoration();
        out << "[<variable>: " << v.name << "]" << std::endl;
    }

    void operator()(render const & v)
    {
        print_decoration();
        out << "[<render>: " << v.name << "]" << std::endl;
    }

    void operator()(for_each const & v)
    {
        print_decoration();
        out << "[<for_each>: " << v.name << "]" << std::endl;
        indent_decoration(v.value);
        boost::apply_visitor(*this, v.value);
        undent_decoration();
    }

    void operator()(condition const & v)
    {}

    void operator()(select_context const & v)
    {
        print_decoration();
        out << "[<select>: " << v.tag << "]" << std::endl;
        indent_decoration(v.body);
        boost::apply_visitor(*this, v.body);
        undent_decoration();
    }

    void operator()(if_then_else const & v)
    {
        print_decoration();
        out << "[<if>: " << v.condition_.name << "]" << std::endl;

        indent_decoration(true);
        print_decoration();
        out << "[<then>]" << std::endl;
        indent_decoration(v.then_);
        boost::apply_visitor(*this, v.then_);
        undent_decoration();
        undent_decoration();

        indent_decoration(false);
        print_decoration();
        out << "[<else>]" << std::endl;
        indent_decoration(v.else_);
        boost::apply_visitor(*this, v.else_);
        undent_decoration();
        undent_decoration();
    }

    void operator()(node_list const & v)
    {
        for (auto inode = v.nodes.begin(),
                  enode = v.nodes.end();
                  inode != enode; ++inode)
        {
            if (std::next(inode) == enode) decorations.back() = false;
            boost::apply_visitor(*this, *inode);
        }
    }

private:
    std::string shrink(std::string v)
    {
        std::transform(v.begin(), v.end(), v.begin(), [] (char ch) {
                return (std::isgraph(ch) || std::isblank(ch))? ch: '%';
        });
        if (v.size() > 30) {
            v.resize(30);
            v.append("...");
        }
        return v;
    }

    class is_multi_node
    {
    public:
        typedef bool result_type;

        template <typename T>
        bool operator()(const T &) const { return false;}

        bool operator()(const node_list &) const { return true;}
    };

    void print_decoration()
    {
        for (auto idecoration = decorations.begin(),
                  edecoration = decorations.end();
                  idecoration != edecoration; ++idecoration)
        {
            if (std::next(idecoration) == edecoration)
                out << (*idecoration? "├──": "└──");
            else
                out << (*idecoration? "│  ": "   ");
        }

    }

    void indent_decoration(const node &node)
    {
        decorations.emplace_back(boost::apply_visitor(is_multi_node{}, node));
    }

    void indent_decoration(bool multi) { decorations.emplace_back(multi);}

    void undent_decoration() { decorations.pop_back();}

    std::ostream &out;
    std::vector<bool> decorations;
};

} // namespace detail

inline void print(std::ostream& out, node const& root)
{
    out << "┐\n";
    detail::printer p(out);
    boost::apply_visitor(p, root);
}

}}}} // namespace boost::boostache::vm::ast

#endif /* BOOST_BOOSTACHE_VM_PRINTER_HPP */

