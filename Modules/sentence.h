//
// Created by Terry on 15/9/11.
//

#ifndef AI_SENTENCE_H
#define AI_SENTENCE_H

#include <vector>
#include "word.h"
#include <string>

using namespace std;

class sentence {

public:

    vector<word *> words;

public:

    sentence();

    void add_word(word *a_word);

    void add_sentence(sentence *a_sentence);

    sentence *copy();

    unsigned long length();

    word *at(int index);

    const char *c_str();

    double get_score();

    ~sentence();

};


#endif //AI_SENTENCE_H
