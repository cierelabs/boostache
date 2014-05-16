#include "template_engine.hpp"
#include "template_context.hpp"

#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <list>


using namespace template_engine;

int main(int argc, char* argv[])
{
	using std::cout;
	
	std::string ctemplate = "hello {{NAME}}";

	cout<<"test case 1\n";
	{
	ctemplate = "hello {{NAME}}";
	Context e;
	e.addVariable("NAME", "World");
	cout<<"before:\n"<<ctemplate<<"\n";
	cout<<"after:\n";
	render(std::cout, Context_list_t(e), ctemplate.data(), ctemplate.data() + ctemplate.size());
	cout<<"\n";
	}

	cout<<"test case 2\n";
	{
	ctemplate = "{{NAME}} test {{NAME}}";
	Context e;
	e.addVariable("NAME", "Richard");
	cout<<"before:\n"<<ctemplate<<"\n";
	cout<<"after:\n";
	render(std::cout, Context_list_t(e), ctemplate.data(), ctemplate.data() + ctemplate.size());
	cout<<"\n";
	}

	cout<<"test case 3\n";
	{
	ctemplate = "Shown.\n{{#person}}\n Never shown!\n {{/person}}\n";
	Context e;
	e.addSubContext("person", {});
	cout<<"before:>>>\n"<<ctemplate<<"<<<\n";
	cout<<"after:>>>\n";
	render(std::cout, Context_list_t(e), ctemplate.data(), ctemplate.data() + ctemplate.size());
	cout<<"<<<\n";
	}

	cout<<"test case 4\n";
	{
	ctemplate = "{{#repo}}\n<b>{{name}}</b>\n{{/repo}}\n";
	Context e;
	e.addSubContext("person", {});
	cout<<"before:>>>\n"<<ctemplate<<"<<<\n";
	cout<<"after:>>>\n";
	render(std::cout, Context_list_t(e), ctemplate.data(), ctemplate.data() + ctemplate.size());
	cout<<"<<<\n";
	}

	cout<<"test case 5\n";
	{
	ctemplate = "{{#repo}}\n<b>{{name}}</b>\n{{/repo}}\n{{^repo}}\nNo repos :(\n{{/repo}}\n";
	Context e;
	cout<<"before:>>>\n"<<ctemplate<<"<<<\n";
	cout<<"after:>>>\n";
	render(std::cout, Context_list_t(e), ctemplate.data(), ctemplate.data() + ctemplate.size());
	cout<<"<<<\n";
	}

	cout<<"test case 6\n";
	{
	ctemplate = "<h1>Today{{! ignore me }}.</h1>\n";
	Context e;
	cout<<"before:>>>\n"<<ctemplate<<"<<<\n";
	cout<<"after:>>>\n";
	render(std::cout, Context_list_t(e), ctemplate.data(), ctemplate.data() + ctemplate.size());
	cout<<"<<<\n";
	}

	// test case 2
	{
	ctemplate = ""
"{{company_name}}\n"
"{{#employees}}\n"
"  {{company_name}} {{name}} {{job}}\n"
"{{#reports}} has a report {{name}} {{/reports}}\n"
"{{/employees}}";
	Context e1;
	e1.addVariable("name", "Richard");
	e1.addVariable("job", "Programmer");
	Context e2;
	e2.addVariable("name", "Michael");
	e2.addVariable("job", "Programmer");
	Context e3;
	e3.addVariable("name", "Joe Bossman");
	e3.addVariable("job", "boss");
	Context e31;
	e31.addVariable("name", "Richard");
	Context e32;
	e32.addVariable("name", "Michael");
	e3.addSubContext("reports", { e31, e32 });
	Context e;
	e.addSubContext("employees", { e1, e2, e3 });
	e.addVariable("name", "BoostCon");
	e.addVariable("company_name", "ACCU");

	cout<<"before:\n"<<ctemplate<<"\n";
	cout<<"after:\n";
	render(std::cout, Context_list_t(e), ctemplate.data(), ctemplate.data() + ctemplate.size());
	cout<<"\n";
	}


}


