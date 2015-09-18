//
// Created by Terry on 15/9/11.
//

#include "sentence.h"

using namespace std;

sentence::sentence()
{

}

void sentence::add_word(word *a_word)
{
    word *sword = new word(a_word->utfword, a_word->word_type, a_word->word_probability);
    words.push_back(sword);
}

void sentence::add_sentence(sentence *a_sentence)
{
    for (int i = 0; i < a_sentence->words.size(); i++) {
	add_word(a_sentence->words[i]);
    }
}

sentence *sentence::copy()
{
    sentence *n_sentence = new sentence();
    n_sentence->add_sentence(this);
    return n_sentence;
}

unsigned long sentence::length()
{
    return (int)words.size();
}

word *sentence::at(int index)
{
    if (index >= length()) {
        return NULL;
    }
    return words[index];
}

const char *sentence::c_str()
{
    string *cstr = new string();
    for (int i = 0; i < words.size(); i++) {
        cstr->append(words[i]->utfword->c_str());
//        if (words[i]->is_new_char)
//            cstr->append("*");
//        cstr->append(words[i]->word_type);
        cstr->append(" ");
    }
    return cstr->c_str();
}

double sentence::get_score()
{
    double score = 1.0;
    for (int i = 0; i < words.size(); i++) {
        if (words[i]->word_probability > 0) score *= words[i]->word_probability;
    }
    return score;
}

sentence::~sentence()
{
    for (int i = 0; i < words.size(); i++) {
        delete(words[i]);
    }
    words.clear();
}

