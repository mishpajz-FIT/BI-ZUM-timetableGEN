#ifndef STDINADJUSTER_H
#define STDINADJUSTER_H

#include "Utility/adjuster.h"

class StdinAdjuster : public Adjuster {
public:
    Priorities operator ()(const Semester & semester) override;
};

#endif /* STDINADJUSTER_H */