//
// Created by payloads on 18-3-4.
//

#ifndef NEWPRO_MYTHREAD_H
#define NEWPRO_MYTHREAD_H

#include <thread>
#include <mutex>
#include <vector>
#include "Mythread.h"
class Mythread {
public:
    Mythread();
    ~Mythread();
    bool setThreads(int thread=0);
    bool runThreads(void (*function)(void * object),void * object);
    bool setDetach();
    bool setJoin();
private:
    int _thread = 0;
    int _thread_id;
    std::vector<std::thread> threads;
};


#endif //NEWPRO_MYTHREAD_H
