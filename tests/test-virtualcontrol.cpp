#include <iostream>
#include <sstream>
#include "hprose/ext/CTime.hpp"
#include "hprose/client.hpp"
#include "hprose/io/ClassManager.hpp"

using namespace std;
using namespace hprose;

//HproseHTTPClient client("http://192.168.2.150/api/endpoint1.php");
HproseTCPClient client("tcp://192.168.0.52:8224");
int main() {
	string mac("52:54:00:12:34:56");
	string token("12345");
	while(true) {
		vector<Any> args = {Any(mac), Any(token), Any(};
		auto r = client.Invoke<bool, vector<Any>>("virtualcontrol");
	}
	return 0;
}
