#ifndef FITCTUIMPORTER_H
#define FITCTUIMPORTER_H

#include "Utility/importers.h"
#include "Extensions/string_extensions.h"
#include "Data/timeinterval.h"

#include <fstream>
#include <regex>
#include <map>

class FITCTUImporter : public Importer {

    std::ifstream file;

public:

    FITCTUImporter(const std::string & filename);

protected:

    Semester load() override;

    enum class ReadingStates {
        Course,
        Id,
        Type,
        Capacity,
        Time,
        Additional,
    };

    std::map<std::string, TimeInterval::Day> dayMapping;
    std::regex capacityRegex;
    std::regex timeRegex;
    std::map<std::string, TimeInterval::Parity> parityMapping;

};

class CS_FITCTUImporter : public FITCTUImporter {
public:
    CS_FITCTUImporter(const std::string & filename);
};

#endif /* FITCTUIMPORTER_H */
