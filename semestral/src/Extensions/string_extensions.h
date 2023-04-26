#ifndef STRING_EXTENSIONS_H
#define STRING_EXTENSIONS_H

static inline void ltrim(std::string & s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [ ] (unsigned char ch) {
        return !std::isspace(ch);
        }));
}

static inline void rtrim(std::string & s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [ ] (unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

static inline void trim(std::string & s) {
    rtrim(s);
    ltrim(s);
}

// source https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring


static inline bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

// source https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c

#endif /* STRING_EXTENSIONS_H */
