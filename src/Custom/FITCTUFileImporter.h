/**
 * @file FITCTUFileImporter.h
 * @author Michal Dobes
 * @date 2023-05-14
 *
 * @brief Importer for importing Semester from FIT CTU'S KOS formatted entries
 * in a file
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef FITCTUFILEIMPORTER_H
#define FITCTUFILEIMPORTER_H

#include "Utility/importers.h"
#include "Extensions/string_extensions.h"
#include "Data/timeinterval.h"

#include <fstream>
#include <regex>
#include <map>

/**
 * @brief Importer for file with entries formatted from FIT CTU's KOS
 *
 * Should be subclassed with language specific regexes and mappings.
 *
 */
class FITCTUFileImporter : public Importer {

    std::ifstream file;

public:

    /**
     * @brief Construct a new FITCTUFileImporter object
     *
     * Opens a file
     *
     * @throws ImporterException if file could not opened
     *
     * @param filename
     */
    FITCTUFileImporter(const std::string & filename);

protected:

    Semester load() override;

    /**
     * @brief States during reading from file (for state machine)
     *
     */
    enum class ReadingStates {
        Course,
        Id,
        Schedule,
        Capacity,
        Time,
        Additional,
    };

    std::map<std::string, TimeInterval::Day> dayMapping; //!< Mapping of day names
    std::regex capacityRegex; //!< Regex for reading capacity
    std::regex timeRegex; //!< Regex for reading time
    std::map<std::string, TimeInterval::Parity> parityMapping; //!< Mapping for parity

};

/**
 * @brief Importer for file with entries formatted from FIT CTU's KOS in Czech
 *
 */
class CS_FITCTUFileImporter : public FITCTUFileImporter {
public:
/**
     * @brief Construct a new CS_FITCTUFileImporter object
     *
     * Opens a file
     *
     * @throws ImporterException if file could not opened
     *
     * @param filename
     */
    CS_FITCTUFileImporter(const std::string & filename);
};

#endif /* FITCTUFILEIMPORTER_H */
