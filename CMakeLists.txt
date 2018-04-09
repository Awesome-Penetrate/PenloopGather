cmake_minimum_required(VERSION 3.9)
project(PenloopGather)

set(CMAKE_CXX_STANDARD 11)

add_executable(PenloopGather main.cpp http.cpp http.h dns.cpp dns.h Mythread.cpp Mythread.h MyportScan.cpp MyportScan.h MysqlClass.cpp MysqlClass.h GLOBAL.h ServerClass.cpp ServerClass.h Mystr.cpp Mystr.h)

target_link_libraries(PenloopGather curl htmlcxx ldns net pthread mysqlclient)