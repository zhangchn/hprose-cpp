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
	string mac("52:54:00:12:34:56");
	// subscribe
	//string topic = client.Invoke<string>("#");
	cout << "subscribe using topic: "<< mac << endl;
	uint64_t counter = 0;
	while (true) {
		//string args = {"0000111100001111"};
		//int o = client.Invoke<int>("hb", args);
		try {
		string args[] = {mac};
		vector<Any> list = client.Invoke<vector<Any>>("heartbeat", args);
		if (list.size() < 2) {
			continue;
		}
		int cmd = Any::cast<int>(list[0]);
		auto token = Any::cast<string>(list[1]);
		cout<< "cmd:" << cmd << endl;
		cout<< "token: " << token << endl;
		if (cmd == 39 ) {
			vector<Any> args1 = {Any(39), Any(mac), Any(token), Any(1)};
			bool r = client.Invoke<bool, vector<Any>>("control", args1);
			cout << "result of return: " << (r ? "t" : "f") << endl;
		} 
		} // end of try
		catch (const HproseException &e) {
			sleep(2);
			cout << "Exception: " << e.what();
		}
		catch (const boost::system::system_error &e) {
			sleep(2);
			cout << "Error: " << e.what();
		}
		catch (...) {
			sleep(2);
		}
	}	
	return 0;
}
