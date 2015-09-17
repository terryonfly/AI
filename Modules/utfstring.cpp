//
// Created by Terry on 15/9/12.
//

#include "utfstring.h"

using namespace std;

utfstring::utfstring()
{

}

utfstring::utfstring(const char *str)
{
    int i = 0;
    while (str[i] != 0x00) {
        if ((signed char)str[i] < 0) {
            string *zh_char = new string({str[i++], str[i++], str[i++]});
            utfchars.push_back(zh_char);
        } else {
            string *en_char = new string({str[i++]});
            utfchars.push_back(en_char);
        }
    }
}

utfstring::~utfstring()
{
    for (int j = 0; j < utfchars.size(); j++) {
        delete(utfchars[j]);
    }
    utfchars.clear();
}

const char *utfstring::c_str()
{
    string *c_str = new string();
    for (int i = 0; i < utfchars.size(); i++) {
        c_str->append(utfchars[i]->c_str());
    }
    return c_str->c_str();
}

utfstring *utfstring::substring(int begin, int length)
{
    utfstring *substring = new utfstring();
    for (int i = begin; i < begin + length; i++) {
        if (i >= utfchars.size()) {
            fprintf(stderr, "get substring out of range [%d, %d] in len = %d\n", begin, length, (int) utfchars.size());
            break;
        }
        substring->append(utfchars[i]);
    }
    return substring;
}

void utfstring::append(string *str)
{
    string *append_str = new string(str->c_str());
    utfchars.push_back(append_str);
}

utfstring *utfstring::at(int index)
{
    utfstring *str = new utfstring();
    if (index >= utfchars.size()) {
        fprintf(stderr, "get index out of range %d in len = %d\n", index, (int) utfchars.size());
        return str;
    }
    str->append(utfchars[index]);
    return str;
}

unsigned long utfstring::length()
{
    return utfchars.size();
}