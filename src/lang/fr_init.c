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
#include "cst_ffeatures.h"
#include "cst_synth.h"
#include "fr_lex.h"
#include "fr_lang_internal.h"

void fr_init(cst_voice *v)
{
    /* Basic generic functions that need to be registered always */
    basic_ff_register(v->ffunctions);

    /* 1. Tokenizer: Splits text into tokens */
    fr_init_tokenizer(v);

    /* 2. Utterance break function */
    feat_set(v->features, "utt_break", breakfunc_val(&default_utt_break));

    /* 3. Text analyser: es_text_analysis.c */
    feat_set(v->features, "tokentowords_func", itemfunc_val(&fr_tokentowords));

    /* 4. very simple POS tagger */
    ff_register(v->ffunctions, "gpos", fr_guess_pos);
    feat_set(v->features, "pos_tagger_func", uttfunc_val(&fr_pos_tagger));

    /* 5. Phrasing: The us_phrasing_cart is good enough for most latin languages.
     *    It takes care of adding pauses after "," "." etc.
     */
    feat_set(v->features, "phrasing_cart", cart_val(&us_phrasing_cart));

    /* 6. Phoneset */
    feat_set(v->features, "phoneset", phoneset_val(&fr_phoneset));
    feat_set_string(v->features, "silence", fr_phoneset.silence);

    feat_set(v->features, "intonation_func", uttfunc_val(&fr_intonation));
}

void fr_plugin_init()
{
   mimic_add_lang("fr", fr_init, fr_lex_init);
}

void fr_plugin_exit()
{
}

