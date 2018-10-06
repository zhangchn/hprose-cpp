#include <iostream>
#include <sstream>
#include "hprose/ext/CTime.hpp"
//#include "hprose/client.hpp"
#include "hprose/client/HproseTCPClient.hpp"
#include "hprose/io/ClassManager.hpp"
#include <unistd.h>

using namespace std;
using namespace hprose;


HproseTCPClient client("tcp://172.99.2.150:8225");

int main() {
    while (true) {
	try {
		vector<Any> args;
		bool result = Any::cast<bool>(client.Invoke<Any>("test", args));
		cout << (result ? "true" : "false") ;
	} catch ( const HproseException &e) {
		cout << e.what();
	}
	sleep(2);
    }
    return 0;
}
