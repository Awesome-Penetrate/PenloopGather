//
// Created by payloads on 18-4-11.
//

#include "ThreadClass.h"



void ThreadClass::setThreadNum(int Num) {
    _threadNum = Num;
}

void ThreadClass::runThread() {

    for (int i = 0; i < _threadNum; ++i) {
        _threadLists.emplace_back(std::thread([=](){
            auto it = intList.rbegin();
            while(it!=intList.rend()){
                mutex.lock();
                std::cout << " [*] Num : " << *it << "\n";
                it ++;
                intList.pop_back();
                mutex.unlock();
            }
        }));
    }
    for(auto it = _threadLists.begin();it!=_threadLists.end();it++){
        (*it).join();
    }

}

ThreadClass::ThreadClass() {
    for (int i = 0; i < 1000; ++i) {
        //std::cout << "[*] Load " << i << std::endl;
        intList.emplace_back(i);
    }


}
