#include <iostream>
#include <sstream>
#include "hprose/ext/CTime.hpp"
//#include "hprose/client.hpp"
#include "hprose/client/HproseTCPClient.hpp"
#include "hprose/io/ClassManager.hpp"
#include <unistd.h>

using namespace std;
using namespace hprose;

HproseTCPClient client("tcp://172.99.2.150:9090");
/*
class Test1 {
public:
	string prop1;
	int prop2;
};
*/

int main() {
/*
	HPROSE_REG_CLASS_EX(Test1, "test1");
	HPROSE_REG_PROPERTY(Test1, prop1);
	HPROSE_REG_PROPERTY(Test1, prop2);
*/
	uint64_t counter = 0;
	while (true) {
		auto o = client.Invoke<Test1>("runTest1");
		if (o.prop1.empty()) {
			cout << "!" << counter << endl;
			return 0;
		}
		cout << "{" << o.prop1 << ": " << o.prop2 << "}" << endl;
		usleep(100);
		++counter;
	}	
	return 0;
}
