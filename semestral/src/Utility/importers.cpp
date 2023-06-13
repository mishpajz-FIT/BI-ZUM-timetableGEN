#include <utility>

#include "importers.h"

ImporterException::ImporterException(std::string message) : msg(std::move(message)) { }

const char * ImporterException::what() const noexcept {
    return msg.c_str();
}

Semester Importer::import() {
    // For future implemenation (actions after load but before import)
    return load();
}