/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 2001                             */
/*                        All Rights Reserved.                           */
/*                                                                       */
/*  Permission is hereby granted, free of charge, to use and distribute  */
/*  this software and its documentation without restriction, including   */
/*  without limitation the rights to use, copy, modify, merge, publish,  */
/*  distribute, sublicense, and/or sell copies of this work, and to      */
/*  permit persons to whom this work is furnished to do so, subject to   */
/*  the following conditions:                                            */
/*   1. The code must retain the above copyright notice, this list of    */
/*      conditions and the following disclaimer.                         */
/*   2. Any modifications must be clearly marked as such.                */
/*   3. Original authors' names are not deleted.                         */
/*   4. The authors' names are not used to endorse or promote products   */
/*      derived from this software without specific prior written        */
/*      permission.                                                      */
/*                                                                       */
/*  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         */
/*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      */
/*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
/*  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    */
/*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   */
/*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          */
/*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       */
/*  THIS SOFTWARE.                                                       */
/*                                                                       */
/*************************************************************************/
/*             Author:  Alan W Black (awb@cs.cmu.edu)                    */
/*               Date:  January 2001                                     */
/*************************************************************************/
/*                                                                       */
/*  CMU Lexicon definition                                               */
/*                                                                       */
/*************************************************************************/

#include "mimic.h"

#include "fr_lex.h"
#include "fr_lang.h"

extern const int fr_lex_entry[];
extern const unsigned char fr_lex_data[];
extern const int fr_lex_num_entries;
extern const int fr_lex_num_bytes;
extern const char * const fr_lex_phone_table[54];
extern const char * const fr_lex_phones_huff_table[];
extern const char * const fr_lex_entries_huff_table[];

//static const char * const addenda0[] = { "p,", NULL };

static const char * const * const addenda[] = {
    NULL };

cst_lexicon fr_lex;
cst_lts_rules fr_lts_rules;

extern cst_lts_rule fr_lts_model[];
extern const char * const fr_lts_phone_table[];
extern const map_unicode_to_int fr_lts_letter_index;


static int fr_is_silence(const char *p)
{
    if (cst_streq(p,"pau"))
	return TRUE;
    else
	return FALSE;
}

static int fr_is_vowel(const char *p)
{
    const char *vowel_symbol = phone_feature_string(&fr_phoneset, p, "vc");
    if (vowel_symbol != NULL && cst_streq(vowel_symbol, "+"))
        return TRUE;
    return FALSE;
}

static int fr_has_vowel_in_syl(const cst_item *i)
{
    const cst_item *n;

    for (n=i; n; n=item_prev(n))
	if (fr_is_vowel(item_feat_string(n,"name")))
	    return TRUE;
    return FALSE;
}


static int fr_has_vowel_in_list(const cst_val *v)
{
    const cst_val *t;

    for (t=v; t; t=val_cdr(t))
	if (fr_is_vowel(val_string(val_car(t))))
	    return TRUE;
    return FALSE;
}

static int fr_dist_to_vowel(const cst_val *rest)
{
    if (rest == 0)
        return 0;  /* shouldn't get here */
    else if (fr_is_vowel(val_string(val_car(rest))))
        return 0;
    else
        return 1+fr_dist_to_vowel(val_cdr(rest));
}


/* I don't know if this syllabification approach is good enough for French. Testing needed */
static const char * const fr_onset_trigrams[] = {
     NULL
};
static const char * const fr_onset_bigrams[] = {
    NULL
};

static int fr_onset_bigram(const cst_val *rest)
{
    char x[16];
    int i;

    cst_sprintf(x,"%s%s",val_string(val_car(rest)),
           val_string(val_car(val_cdr(rest))));
    for (i=0; fr_onset_bigrams[i]; i++)
        if (cst_streq(x, fr_onset_bigrams[i]))
            return TRUE;
    return FALSE;
}

static int fr_onset_trigram(const cst_val *rest)
{
    char x[32];
    int i;

    cst_sprintf(x,"%s%s%s",val_string(val_car(rest)),
           val_string(val_car(val_cdr(rest))),
           val_string(val_car(val_cdr(val_cdr(rest)))));
    for (i=0; fr_onset_trigrams[i]; i++)
        if (cst_streq(x, fr_onset_trigrams[i]))
            return TRUE;
    return FALSE;
}



int fr_syl_boundary(const cst_item *i,const cst_val *rest)
{

    if (rest == NULL)
	return TRUE;
    else if (fr_is_silence(val_string(val_car(rest))))
	return TRUE;
    else if (!fr_has_vowel_in_list(rest)) 
        /* no more vowels so rest *all* coda */
	return FALSE;
    else if (!fr_has_vowel_in_syl(i))  /* need a vowel */
        /* no vowel yet in syl so keep copying */
	return FALSE;
    else if (fr_is_vowel(val_string(val_car(rest))))
        /* next is a vowel, syl has vowel, so this is a break */
	return TRUE;
    else 
    {
        /* want to know if from rest to the next vowel is a valid onset */
        int d2v = fr_dist_to_vowel(rest);
        if (d2v < 2)
            return TRUE;
        else if (d2v > 3)
            return FALSE;
        else if (d2v == 2) 
            return fr_onset_bigram(rest);
        else /* if (d2v == 3) */
            return fr_onset_trigram(rest);
        return TRUE;
    }

}



cst_utterance *fr_postlex(cst_utterance *u)
{
    /* Post lexical rules: To be done */
    return u;
}

cst_lexicon *fr_lex_init()
{
    if (fr_lts_rules.name)
        return &fr_lex;  /* Already initialized */

    fr_lts_rules.name = "fr";
    fr_lts_rules.model = (cst_lts_rule*) fr_lts_model;
    fr_lts_rules.phone_table = fr_lts_phone_table;
    fr_lts_rules.context_window_size = 4;
    fr_lts_rules.context_extra_feats = 1;
    fr_lts_rules.letter_index = (map_unicode_to_int *) &fr_lts_letter_index;

    fr_lex.name = "fr";
    fr_lex.num_entries = fr_lex_num_entries;
    fr_lex.data = (unsigned char *)(void *)fr_lex_data;
    fr_lex.num_bytes = fr_lex_num_bytes;
    fr_lex.phone_table = (char **) fr_lex_phone_table;
    fr_lex.syl_boundary = fr_syl_boundary;
    fr_lex.addenda = (char ***) addenda;
    fr_lex.lts_rule_set = (cst_lts_rules *) &fr_lts_rules;

    fr_lex.phone_hufftable = fr_lex_phones_huff_table;
    fr_lex.entry_hufftable = fr_lex_entries_huff_table;

    fr_lex.postlex = fr_postlex;

    return &fr_lex;

}
