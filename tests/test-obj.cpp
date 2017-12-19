#include <iostream>
#include <sstream>
#include "hprose/ext/CTime.hpp"
#include "hprose/client.hpp"
#include "hprose/io/ClassManager.hpp"

using namespace std;
using namespace hprose;

HproseHTTPClient client("http://192.168.2.150/api/endpoint1.php");
class Test1 {
public:
	string prop1;
	int prop2;
};

int main() {
	HPROSE_REG_CLASS_EX(Test1, "test1");
	HPROSE_REG_PROPERTY(Test1, prop1);
	HPROSE_REG_PROPERTY(Test1, prop2);

	auto o = client.Invoke<Test1>("runTest1");
	cout << "{" << o.prop1 << ": " << o.prop2 << "}" << endl;
	return 0;
}
