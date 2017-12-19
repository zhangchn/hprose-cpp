#include <iostream>
#include <sstream>
#include "hprose/ext/CTime.hpp"
#include "hprose/client.hpp"
#include "hprose/io/ClassManager.hpp"

using namespace std;
using namespace hprose;

HproseHTTPClient client("http://192.168.0.213/lmsbase/backend/backend/web/index.php?r=api");
class Test1 {
public:
	string prop1;
	int prop2;
};


int main() {
	
	string args[] = {"schooladmin", "123456"};
	auto o = client.Invoke<std::tr1::unordered_map<std::string, Any>>("user_login", args);
	cout << "{" << endl;
	for (auto it = o.begin(); it != o.end(); ++it) {
		auto k = it->first;
		auto v = it->second;
		if ( v.type() == typeid(string)) {
			cout << k << ":" << Any::cast<string>(v) << endl;
		} else if (v.type() == typeid(int)) {
			cout << k << ":" << Any::cast<int>(v) << endl;
		} else if (v.empty()) {
			cout << k << ": <null>" << endl;
		}
	}
	cout << "}" << endl;
	return 0;
}
