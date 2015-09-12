//
// Created by Terry on 15/9/11.
//

#ifndef AI_SENTENCE_H
#define AI_SENTENCE_H

#include <vector>
#include "word.h"

using namespace std;

class sentence {

public:

    vector<word *> words;

public:

    sentence();

    ~sentence();

    void add_word(word *a_word);

    void add_sentence(sentence *a_sentence);

    const char *c_str();

};


#endif //AI_SENTENCE_H
