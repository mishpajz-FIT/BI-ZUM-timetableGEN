#include <utility>

#include "importers.h"

ImporterException::ImporterException(std::string message) : msg(std::move(message)) { }

Semester Importer::import() {
    // For future implemenation (actions after load but before import)
    return load();
}

const char * ImporterException::what() const noexcept {
    return msg.c_str();
}