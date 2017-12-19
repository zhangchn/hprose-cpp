CXX=clang++
LIBS=-lboost_system -lboost_thread -lpthread -lcrypto -lssl
INCLUDE=-I../
STD=-std=c++11

all: test-main test-vmclient

test-main:
	$(CXX) $(STD) main.cpp $(LIBS) $(INCLUDE) -o $@

test-vmclient:
	$(CXX) $(STD) test-vmclient.cpp $(LIBS) $(INCLUDE) -o $@

test-obj:
	$(CXX) -g $(STD) $@.cpp $(LIBS) $(INCLUDE) -o $@

login:
	$(CXX) -g $(STD) $@.cpp $(LIBS) $(INCLUDE) -o $@

tcp:
	$(CXX) -fno-limit-debug-info -g $(STD) test-$@.cpp $(LIBS) $(INCLUDE) -o $@

subscribe:
	$(CXX) -fno-limit-debug-info -g $(STD) test-$@.cpp $(LIBS) $(INCLUDE) -o $@

.PHONY: clean

clean:
	rm test-main test-vmclient
