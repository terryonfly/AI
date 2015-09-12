//
// Created by Terry on 15/9/11.
//

#include "sentence.h"

using namespace std;

sentence::sentence()
{

}

sentence::~sentence()
{

}

void sentence::add_word(word *a_word)
{
    words.push_back(a_word);
}

void sentence::add_sentence(sentence *a_sentence)
{
    for (int i = 0; i < a_sentence->words.size(); i++) {
        words.push_back(a_sentence->words[i]);
    }
}

const char *sentence::c_str()
{
    string *cstr = new string();
    for (int i = 0; i < words.size(); i++) {
        cstr->append(words[i]->utfword->c_str());
//        cstr->append("/");
//        cstr->append(words[i]->type);
        cstr->append(" ");
    }
    return cstr->c_str();
}
