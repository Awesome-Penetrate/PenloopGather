//
// Created by payloads on 18-4-11.
//

#include "EmptyClass.h"

void * EmptyClass::run(void * obj) {
    EmptyClass * objP = (EmptyClass *)obj;
    static auto it = objP->intList.begin();
    while(it!=objP->intList.end()){
        std::cout << " [*] Num : " << *it << ",";
        it ++;
    }
}

EmptyClass::EmptyClass() {
    for (int i = 0; i < 100; ++i) {
        intList.emplace_back(i);
    }
    ThreadClass::setThreadNum(10);
    //ThreadClass::func = this->run;
    ThreadClass::runThread();
}