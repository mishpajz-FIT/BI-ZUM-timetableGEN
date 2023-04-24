#include <utility>

#include "importers.h"

ImporterException::ImporterException(std::string message): msg(std::move(message)) {}

const char *ImporterException::what() const noexcept {
    return msg.c_str();
}