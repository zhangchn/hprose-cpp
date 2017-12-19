#include <iostream>
#include <sstream>
#include "hprose/ext/CTime.hpp"
//#include "hprose/client.hpp"
#include "hprose/client/HproseTCPClient.hpp"
#include "hprose/io/ClassManager.hpp"
#include <unistd.h>

using namespace std;
using namespace hprose;

HproseTCPClient client("tcp://192.168.2.150:9090");

int main() {

	uint64_t counter = 0;
	while (true) {
		string args = {"0000111100001111"};
		int o = client.Invoke<int>("hb", args);
		cout<< o << endl;
		++counter;
	}	
	return 0;
}
