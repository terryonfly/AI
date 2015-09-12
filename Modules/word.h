//
// Created by Terry on 15/9/11.
//

#ifndef AI_WORD_H
#define AI_WORD_H


#include "utfstring.h"

class word {

public:

    utfstring *utfword;

    char *type;

public:

    word(utfstring *a_utfword);

    word(utfstring *a_utfword, char *a_type);

    ~word();

};


#endif //AI_WORD_H
