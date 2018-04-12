//
// Created by payloads on 18-4-11.
//

#ifndef PENLOOPGATHER_EMPTYCLASS_H
#define PENLOOPGATHER_EMPTYCLASS_H

#include "ThreadClass.h"

class EmptyClass :public ThreadClass{

public:
    void * run(void * obj);
    EmptyClass();
private:
    std::vector<int>intList;
};


#endif //PENLOOPGATHER_EMPTYCLASS_H
