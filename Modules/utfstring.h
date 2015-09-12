//
// Created by Terry on 15/9/12.
//

#ifndef AI_UTFSTRING_H
#define AI_UTFSTRING_H

#include <vector>
#include <string>

using namespace std;

class utfstring {

public:

    vector<string *> utfchars;

public:

    utfstring();

    utfstring(const char *str);

    ~utfstring();

    const char *c_str();

    utfstring *substring(int begin, int length);

    void append(string *str);

    utfstring *at(int index);

    unsigned long length();
};


#endif //AI_UTFSTRING_H
