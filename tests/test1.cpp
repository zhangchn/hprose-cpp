#include <iostream>
#include <sstream>
#include "hprose/ext/CTime.hpp"
#include "hprose/client.hpp"
#include "hprose/io/ClassManager.hpp"

using namespace std;
using namespace hprose;

//HproseHTTPClient client("http://192.168.2.150/api/endpoint1.php");
HproseTCPClient client("tcp://localhost:8225");
int main() {
	string mac("52:54:00:12:34:56");
	string token("12345");
	while(true) {
		boost::unordered_map<string, Any> stat;
		stat.insert(make_pair(string("Memory"), Any(string("80%"))));
		stat.insert(make_pair(string("Harddisk"), Any(string("80%"))));
		stat.insert(make_pair(string("Image"), Any(string("aba992c4-0735-11e8-981c-078fd8d2a066"))));
		stat.insert(make_pair(string("Currentstate"), Any(0)));
		stat.insert(make_pair(string("Clienttoken"), Any(string(""))));

		vector<Any> args = {Any(mac), Any(stat)};
		auto r = client.Invoke<bool, vector<Any>>("virtualreport", args);
		cout<< "report: " << (r ? "success" : "failure") << endl;
		sleep(10);
	}
	return 0;
}
