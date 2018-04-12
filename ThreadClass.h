//
// Created by payloads on 18-4-11.
//

#ifndef PENLOOPGATHER_THREADCLASS_H
#define PENLOOPGATHER_THREADCLASS_H

#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <vector>
class ThreadClass {

public:
    std::mutex mutex;
    ThreadClass();
    void setThreadNum(int);
    void runThread();
    std::vector<int>intList;
private:
    int _threadNum;
    std::vector<std::thread>_threadLists;
};


#endif //PENLOOPGATHER_THREADCLASS_H
