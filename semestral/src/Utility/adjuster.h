#ifndef ADJUSTER_H
#define ADJUSTER_H

#include "Evolution/priorities.h"
#include "Data/subjects.h"

class Adjuster {
public:
    virtual Priorities operator ()(Semester & semester) = 0;
};

#endif /* ADJUSTER_H */