#include <iostream>
#include <sstream>
#include "hprose/ext/CTime.hpp"
#include "hprose/client.hpp"

using namespace std;
using namespace hprose;

HproseHTTPClient client("http://192.168.2.150/api/endpoint.php");


string testUUID() {
	return client.Invoke<string>("loadUUID");
}

string testStatus() {
	return client.Invoke<string>("status");
}

string testCreate(const string& iso) {
	string args[] = {iso};
	return client.Invoke<string>("createInstance", args);
	
}

bool testStart() {
	return client.Invoke<bool>("startInstance");
}

bool testResetStatus() {
	return client.Invoke<bool>("resetInstance");
}

bool testShutdown() {
	return client.Invoke<bool>("shutdown");
}

bool testConclude() {
	return client.Invoke<bool>("conclude");
}

int main() {

	const string& s = testStatus();
	cout << "Test UUID: " << s << endl;
	if (s != "ready") {
		cout << ">> Resetting: " << (testResetStatus() ? "success" : "failed") << endl;
	}
	cout << "Test status: " << testStatus() << endl;
	string uuid = testCreate("cn_windows_10_enterprise_x64_dvd_6846957");
	if (uuid.size() == 0) {
		cout << "Test create failed." << endl;
		return 0;
	}
	cout << "Test create: " << uuid << endl;
	cout << "Test start: " << (testStart() ? "success" : "faield") << endl;
	char holder;
	cin >> &holder;
	cout << "Test shutdown: " << (testShutdown() ? "success" : "failed") << endl;
	cout << "Test conclude: " << (testConclude() ? "success" : "failed") << endl;
	cout << "End" << endl;
	return 0;
}

