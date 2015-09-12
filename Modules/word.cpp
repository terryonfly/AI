//
// Created by Terry on 15/9/11.
//

#include "word.h"

using namespace std;

word::word(utfstring *a_utfword)
{
    utfword = a_utfword;
    type = new char(0);
}

word::word(utfstring *a_utfword, char *a_type)
{
    utfword = a_utfword;
    type = a_type;
}

word::~word()
{

}
