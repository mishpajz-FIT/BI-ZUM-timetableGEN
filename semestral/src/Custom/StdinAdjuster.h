#ifndef STDINADJUSTER_H
#define STDINADJUSTER_H

#include "Utility/adjuster.h"
#include "Data/subjects.h"
#include "Extensions/string_extensions.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

#define STDIN_ADJUSTER_SEPARATOR_LENGTH 80

class StdinAdjuster : public Adjuster {
public:
    enum class ReturnStatus {
        GOOD,
        BAD,
        QUIT
    };

private:

    bool checkInputForFail();

    void adjustBool(const std::string & infoText, bool & value);

    void adjustTime(const std::string & infoText, uint8_t & value);

    template <typename T>
    void adjustValue(const std::string & infoText, T & value);

    std::pair<std::shared_ptr<Course>, StdinAdjuster::ReturnStatus> retrieveCourse(Semester & semester);

    std::pair<std::shared_ptr<Schedule>, StdinAdjuster::ReturnStatus> retrieveSchedule(std::shared_ptr<Course> & course);

    std::pair<std::shared_ptr<Entry>, StdinAdjuster::ReturnStatus> retrieveEntry(std::shared_ptr<Schedule> & schedule);

    void adjustBonuses(Semester & semester);

    void adjustIgnored(Semester & semester);

public:
    Priorities operator()(Semester & semester) override;
};

#endif /* STDINADJUSTER_H */