//
// Created by Terry on 15/9/11.
//

#include "word.h"
#include <stdlib.h>
#include <string.h>

using namespace std;

word::word(utfstring *a_utfword)
{
    utfword = new utfstring(a_utfword->c_str());
    word_type = new char(0);
    is_new_char = false;
    word_probability = 0;
}

word::word(utfstring *a_utfword, char *a_word_type, double a_word_probability)
{
    utfword = new utfstring(a_utfword->c_str());
    word_type = new char(sizeof(a_word_type));
    strcpy(word_type, a_word_type);
    is_new_char = false;
    word_probability = a_word_probability;
}

word::~word()
{
//    delete(utfword);
//    delete(word_type);
}
