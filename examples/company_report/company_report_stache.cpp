#include <boost/cppte/frontend/stache_model.hpp>
#include <boost/cppte/frontend/stache_model_printer.hpp>
#include <boost/cppte/simple_parser.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <map>
#include <list>

using namespace boost::cppte::front_end;

namespace
{
        std::string print(const ast::stache_root& ast, const stache_model& model)
        {
                std::ostringstream out;
                boost::cppte::front_end::ast::print(out, ast, model);
                return out.str();
        }

        ast::stache_root parse(const std::string& text)
        {
                ast::stache_root ast;
                if( !boost::simple_parse_template(text, ast) )
                {
                        throw std::runtime_error("Parse failed");
                }
                return ast;
        }
}


std::string report =
"Company {{NAME}}\n"
"\n"
"{{#YEARS}}\n"
"{{#DIVISIONS}}\n"
"Division: {{NAME}} Budget for year: {{BUDGET}}\n"
"Employees:\n"
"{{#EMPLOYEES}} {{NAME}} salary {{SALARY}}\n"
"{{/EMPLOYEES}}\n"
"{{^EMPLOYEES}}\n"
"Has no employees\n"
"{{/EMPLOYEES}}\n"
"{{/DIVISIONS}}\n"
"{{/YEARS}}\n"
;

int main(int argc, char* argv[])
{
	using std::cout;

	if (argc>1)
	{
		std::ifstream source(argv[1], std::ios::binary);
		std::vector<char> data((std::istreambuf_iterator<char>(source)),
			std::istreambuf_iterator<char>());
		report.assign(data.begin(), data.end());
	}

	stache_model company;

	company["NAME"] = "BigCorp";
	company["YEAR FOUNDED"] = "1999";
	company["YEARS"] = stache_model_vector { stache_model { { "DIVISIONS", stache_model_vector {
		stache_model {
			{ "Name", "R&D" },
			{ "BUDGET", "1000000" },
			{ "EMPLOYEES", stache_model_vector {
				stache_model{ { "NAME", "JOE" }, { "EMPLOYEE ID", "1" }, { "SALARY", "10000" } },
				stache_model{ { "NAME", "Sally" }, { "EMPLOYEE ID", "2" }, { "SALARY", "12000" } },
				},
			},
		},
		stache_model {
			{ "NAME", "Finance" },
			{ "BUDGET", "200000" },
			{ "EMPLOYEES", stache_model_vector {
				stache_model{ { "NAME", "Betty" }, { "EMPLOYEE ID", "5" }, { "SALARY", "8000" } },
				stache_model{ { "NAME", "Jim" }, { "EMPLOYEE ID", "8" }, { "SALARY", "12000" } }, 
				},
			},
		},
		stache_model {
			{ "NAME", "HiddenDivision", },
			{ "EMPLOYEES", stache_model_vector {
				},
			},
		}
	}}}};

	ast::stache_root ast = parse(report);
	cout<<print(ast, company)<<"\n";

}


