//
// Created by Terry on 15/9/11.
//

#ifndef AI_WORD_H
#define AI_WORD_H

#include "utfstring.h"
#include <string>

using namespace std;

class word {

public:

    utfstring *utfword;

    char *word_type;

    bool is_new_char;

    double word_probability;

public:

    word(utfstring *a_utfword);

    word(utfstring *a_utfword, char *a_word_type, double a_word_probability);

    ~word();

};


#endif //AI_WORD_H
