CXX ?= g++

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g           #编译带调试信息的可执行文件
else
    CXXFLAGS += -O2          #优化

endif

server: main.cpp  server.cpp
	$(CXX) -o server  $^ $(CXXFLAGS) -lpthread -lmysqlclient

clean:
	rm  -r server
