/* Copyright 2017 Sergio Oller
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/* Introduction
 * =============
 *
 * Provides the fr_guess_pos function that guesses the part of speech (POS)
 * of a given a word in a simple way (just a "guess").
 *
 * We can stablish 9 part of speech in French:
 *
 * - NOUN
 * - VERB
 * - ADJ
 * - ADV
 * - DET
 * - PRON
 * - PREP
 * - CONJ
 * - INTERJ
 *
 * Nouns, verbs, adjectives and adverbs are in general "content" words
 * in opposition to determinants, pronouns, prepositions, conjunctions and
 * interjections that are mostly "function" words. (This is just a
 * simplification, but it is enough for us).
 *
 * The difference between the fr_guess_pos and a real pos tagger is
 * that the fr_guess_pos guesses without considering context. A good POS
 * tagger can consider the context of the words for better prediction.
 *
 * Knowing the POS in a speech synthesis engine can help to:
 *
 * - Disambiguate: In some languages some homograph words are not homophones.
 *   Two words that are written in the same way (homographs) may sound different
 * (not homophones).
 *   The part of speech can be used to disambiguate the pronounciation.
 *
 * - Give emphasis in a sentence.
 *   For instance, given "le chat est noir", we know that the "content" words
 *   are "chat" and "noir" so we may speak the sentence giving more weight or
 *   emphasis to those words.
 *   This is used in the intonation model "fr_intonation".
 *   The guessed POS is also considered for this purpose in the clustergen and
 *   HTS voices as a contextual language feature to model speech.
 */
/*
 * Implementation
 * ===============
 * We follow the lang/usenglish/us_ffeatures.c approach.
 *
 * We will define one list for each of the non-content categories. Each
 * list will contain all the words in that category.
 *
 * Given a word, if it is found in any of the lists then its category will be
 * returned, otherwise we will return "content" as it will belong to a content
 * word category.
 *
 * As the English approach uses cst_val, we follow the same approach. That's why
 * there are so many DEF_STATIC_CONST_VAL_STRING macros everywhere
 */
#include "cst_ffeatures.h"
#include "cst_item.h"
#include "cst_val.h"

/* The categories we will return */
DEF_STATIC_CONST_VAL_STRING(val_string_det, "DET");
DEF_STATIC_CONST_VAL_STRING(val_string_pron, "PRON");
DEF_STATIC_CONST_VAL_STRING(val_string_conj, "CONJ");
DEF_STATIC_CONST_VAL_STRING(val_string_prep, "PREP");
DEF_STATIC_CONST_VAL_STRING(val_string_interj, "INTERJ");

/* ===== determinants: ====== */
/* TO DO: Please add more determinants */
DEF_STATIC_CONST_VAL_STRING(fr_gpos_det_le, "le");
DEF_STATIC_CONST_VAL_STRING(fr_gpos_det_la, "la");
DEF_STATIC_CONST_VAL_STRING(fr_gpos_det_les, "les");
DEF_STATIC_CONST_VAL_STRING(fr_gpos_det_des, "des");
DEF_STATIC_CONST_VAL_STRING(fr_gpos_det_de, "de");

/* determinant list of words: */
static const cst_val *const fr_gpos_det_list[] = {
    (cst_val *) &val_string_det, /* first item is word category "det"
                                    ("determinant") */
    (cst_val *) &fr_gpos_det_le, /* The words of that category */
    (cst_val *) &fr_gpos_det_la, (cst_val *) &fr_gpos_det_les,
    (cst_val *) &fr_gpos_det_des, (cst_val *) &fr_gpos_det_des,
    /* TO DO: Please add more determinants */
    0
}; /* Terminate with 0 */

/* ======= pronouns: ======= */
/* TO DO: Please add more pronouns */
DEF_STATIC_CONST_VAL_STRING(fr_gpos_pron_il, "il");
DEF_STATIC_CONST_VAL_STRING(fr_gpos_pron_elle, "elle");
DEF_STATIC_CONST_VAL_STRING(fr_gpos_pron_mon, "mon");

/* pronoun list of words: */
static const cst_val *const fr_gpos_pron_list[] = {
    (cst_val *) &val_string_pron, /* first item is word category */
    (cst_val *) &fr_gpos_pron_il, /* The words of that category */
    (cst_val *) &fr_gpos_pron_elle,
    (cst_val *) &fr_gpos_pron_mon,
    /* TO DO: Please add more pronouns */
    0
}; /* Terminate with 0 */

/* ======= conjunctions: ======= */
/* TO DO: Please add more conjunctions */
DEF_STATIC_CONST_VAL_STRING(fr_gpos_conj_et, "et");
DEF_STATIC_CONST_VAL_STRING(fr_gpos_conj_car, "car");

/* conj list of words: */
static const cst_val *const fr_gpos_conj_list[] = {
    (cst_val *) &val_string_conj, /* first item is word category */
    (cst_val *) &fr_gpos_conj_et, /* The words of that category */
    (cst_val *) &fr_gpos_conj_car,
    /* TO DO: Please add more conjunctions */
    0
}; /* Terminate with 0 */

/* ======= prepositions: ======= */
/* TO DO: Please add more prepositions */
DEF_STATIC_CONST_VAL_STRING(fr_gpos_prep_sur, "sur");
DEF_STATIC_CONST_VAL_STRING(fr_gpos_prep_sous, "sous");

/* prep list of words: */
static const cst_val *const fr_gpos_prep_list[] = {
    (cst_val *) &val_string_prep, /* first item is word category */
    (cst_val *) &fr_gpos_prep_sur, /* The words of that category */
    (cst_val *) &fr_gpos_prep_sous,
    /* TO DO: Please add more prepositions */
    0
}; /* Terminate with 0 */

/* ======= interjections: ======= */
/* TO DO: Please add more interjections */
DEF_STATIC_CONST_VAL_STRING(fr_gpos_interj_ben, "ben");
DEF_STATIC_CONST_VAL_STRING(fr_gpos_interj_eh, "eh");

/* prep list of words: */
static const cst_val *const fr_gpos_interj_list[] = {
    (cst_val *) &val_string_interj, /* first item is word category */
    (cst_val *) &fr_gpos_interj_ben, /* The words of that category */
    (cst_val *) &fr_gpos_interj_eh,
    /* TO DO: Please add more interjections */
    0
}; /* Terminate with 0 */

/* All the categories. Includes non-content categories (articles, prepositions,
 * conjunctions...) */
const cst_val *const *const fr_gpos_data[] = {
    fr_gpos_det_list,  fr_gpos_pron_list,   fr_gpos_conj_list,
    fr_gpos_prep_list, fr_gpos_interj_list, 0
};

const cst_val *fr_guess_pos(const cst_item *word)
{
    /* Guess at part of speech (function/content) */
    return generic_gpos(word, fr_gpos_data);
}
