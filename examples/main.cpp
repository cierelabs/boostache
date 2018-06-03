#include <vector>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/reversed.hpp>

int example1();
int example2();
int example3();
int example4();
int django1();
int django2();
int simple_generate2();

//#define BOOST_FOREACH_REPLACMENT(VAR, COL)  for(VAR : COL)
//
//#define BOOST_REVERSE_FOREACH_REPLACMENT(VAR, COL)     for(VAR : boost::adaptors::reverse(COL) )
int main()
{
    //std::vector<int> v{ 1,2,3,4 };
    //BOOST_FOREACH_REPLACMENT(int& i, v) {

    //    int j = i;
    //}
    //BOOST_REVERSE_FOREACH_REPLACMENT(int& i, v) {

    //    int j = i;
    //}
    //example1();
	example2();
	//example3();
    //example4();
    //django1();
    //django2();
	//simple_generate2();
}