#include "Mythread.h"
#include <iostream>

bool Mythread::setThreads(int thread) {
    if(thread < 100){
        _thread = thread;
    }else{
        _thread = 100;
    }
    return true;
}


bool Mythread::runThreads(void (*function)(void *) , void * object) {
    for (int i = 0; i < _thread; ++i) {
        threads.emplace_back(std::thread(function,object));
    }
    return true;
}

bool Mythread::setDetach(){
    for (int i = 0; i < _thread; ++i) {
        threads[i].detach();
    }
    return true;
}

bool Mythread::setJoin(){
    for (int i = 0; i < _thread; ++i) {
        threads[i].join();
    }
    return true;
}

Mythread::Mythread() {
    //std::cout << "start threads" << std::endl;
}


Mythread::~Mythread() {
    //std::cout << "end threads" << std::endl;
    threads.clear();
}