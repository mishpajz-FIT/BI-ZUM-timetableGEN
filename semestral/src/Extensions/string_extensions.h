/**
 * @file string_extensions.h
 * @author Michal Dobes
 * @date 2023-05-14
 *
 * @brief Helper functions for string manipulation
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef STRING_EXTENSIONS_H
#define STRING_EXTENSIONS_H

#include <algorithm>

/**
 * @brief Trim string of whitespace on left
 *
 * @cite https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring
 *
 * @param s string to trim
 */
static inline void ltrim(std::string & s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [ ] (unsigned char ch) {
        return !std::isspace(ch);
        }));
}

/**
 * @brief Trim string of whitespace on right
 *
 * @cite https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring
 *
 * @param s string to trim
 */
static inline void rtrim(std::string & s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [ ] (unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

/**
 * @brief Trim string of whitespace on left and right
 *
 * @cite https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring
 *
 * @param s string to trim
 */
static inline void trim(std::string & s) {
    rtrim(s);
    ltrim(s);
}


/**
 * @brief Detect if string contains only numbers
 *
 * @cite https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
 *
 * @param s string to check
 * @return true string contains only numbers
 * @return false string contains other characters
 */
static inline bool is_number(const std::string & s) {
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [ ] (unsigned char c) { return !std::isdigit(c); }) == s.end();
}

#endif /* STRING_EXTENSIONS_H */
