CXX=clang++
LIBS=-lboost_system -lboost_thread -lpthread -lcrypto -lssl
INCLUDE=-I../
STD=-std=c++11
BUILD_DIR=./build/

all: test-main test-vmclient

test-main:
	$(CXX) $(STD) main.cpp $(LIBS) $(INCLUDE) -o $(BUILD_DIR)$@

test-vmclient:
	$(CXX) $(STD) test-vmclient.cpp $(LIBS) $(INCLUDE) -o $(BUILD_DIR)$@

test-obj:
	$(CXX) -g $(STD) $@.cpp $(LIBS) $(INCLUDE) -o $(BUILD_DIR)$@

login:
	$(CXX) -g $(STD) $@.cpp $(LIBS) $(INCLUDE) -o $(BUILD_DIR)$@

tcp:
	$(CXX) -fno-limit-debug-info -g $(STD) test-$@.cpp $(LIBS) $(INCLUDE) -o $(BUILD_DIR)$@

timeout:
	$(CXX) -fno-limit-debug-info -g $(STD) test-$@.cpp $(LIBS) $(INCLUDE) -o $(BUILD_DIR)$@

subscribe:
	$(CXX) -fno-limit-debug-info -g $(STD) test-$@.cpp $(LIBS) $(INCLUDE) -o $(BUILD_DIR)$@

virtualreport:
	$(CXX) -fno-limit-debug-info -g $(STD) test-$@.cpp $(LIBS) $(INCLUDE) -o $(BUILD_DIR)$@

TimePushClient:
	$(CXX) -fno-limit-debug-info -g $(STD) $@.cpp $(LIBS) $(INCLUDE) -o $(BUILD_DIR)$@

.PHONY: clean

clean:
	rm $(BUILD_DIR)/*
