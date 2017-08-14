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

/* This file defines the phoneset used to transcribe text.
 *
 * A cst_phoneset consists fundamentally of:
 *  - A list of phones
 *  - A list of linguistic features for each phone
 *
 * Most phonesets define the same set of features (see fr_featnames),
 * related to how are these phones pronounced.
 *
 */
#include <stdlib.h>
#include "cst_phoneset.h"
#include "cst_val.h"
#include "cst_string.h"
#include "fr_lang.h"

static const char *const fr_featnames[] = {
    "vc", /* vowel or consonant */
    "vlng", /* vowel length: short long dipthong schwa */
    "vheight", /* vowel height: high mid low */
    "vfront", /* vowel frontness: front mid back */
    "vrnd", /* lip rounding */
    "ctype", /* consonant type: stop fricative affricative nasal liquid trill
                tap approximant */
    "cplace", /* place of articulation: labial alveolar palatal labio-dental
                 dental interdental velar */
    "cvox", /* consonant voicing */
    NULL
};


static const char *const fr_phonenames[] = {
    "@",  "Ahn",  "E",  "Ehn",  "H",  "O",  "Ohn",  "P2",  "P9",  "P9hn",  "R",  "S",  "Z",
    "a",  "b",  "c",  "d",  "e",  "f",  "g",  "h",  "i",  "j",  "k",  "l",  "m",  "n",
    "o",  "p",  "s",  "t",  "u",  "v",  "w",  "y",  "z", "pau", "h#", "brth",
    NULL
};

/* NOOOOOOOOOOOOOOOOOOOOOOO THIS IS FROM SPANISH AND NEEDS TO BE ADAPTED. The first feature (vowel) is the only one that is all right */

/* The numbers defined here are related to the feature values above
 * through the featvals array defined below */
static const int fr_fv_000[] = { 0, 4, 3, 3, 3, 5, 1, 3, -1 }; /* "@" */
static const int fr_fv_001[] = { 0, 4, 3, 3, 3, 5, 1, 0, -1 }; /* "Ahn" */
static const int fr_fv_002[] = { 0, 4, 3, 3, 3, 5, 8, 3, -1 }; /* "E" */
static const int fr_fv_003[] = { 0, 4, 3, 3, 3, 5, 8, 0, -1 }; /* "Ehn" */
static const int fr_fv_004[] = { 3, 4, 3, 3, 3, 5, 15, 3, -1 }; /* "H" */
static const int fr_fv_005[] = { 0, 4, 3, 3, 3, 5, 15, 0, -1 }; /* "O" */
static const int fr_fv_006[] = { 0, 4, 3, 3, 3, 13, 1, 0, -1 }; /* "Ohn" */
static const int fr_fv_007[] = { 0, 4, 3, 3, 3, 13, 9, 0, -1 }; /* "P2" */
static const int fr_fv_008[] = { 0, 4, 3, 3, 3, 13, 11, 0, -1 }; /* "P9" */
static const int fr_fv_009[] = { 0, 4, 3, 3, 3, 13, 15, 0, -1 }; /* "P9hn" */

static const int fr_fv_010[] = { 3, 4, 3, 3, 3, 9, 11, 3, -1 }; /* "R" */
static const int fr_fv_011[] = { 3, 4, 3, 3, 3, 12, 14, 3, -1 }; /* "S" */
static const int fr_fv_012[] = { 3, 4, 3, 3, 3, 12, 17, 3, -1 }; /* "Z" */
static const int fr_fv_013[] = { 0, 4, 3, 3, 3, 12, 9, 3, -1 }; /* "a" */
static const int fr_fv_014[] = { 3, 4, 3, 3, 3, 12, 9, 0, -1 }; /* "b" */
static const int fr_fv_015[] = { 3, 4, 3, 3, 3, 12, 11, 0, -1 }; /* "c" */
static const int fr_fv_016[] = { 3, 4, 3, 3, 3, 12, 15, 3, -1 }; /* "d" */
static const int fr_fv_017[] = { 0, 4, 3, 3, 3, 1, 9, 0, -1 }; /* "e" */
static const int fr_fv_018[] = { 3, 4, 3, 3, 3, 1, 11, 0, -1 }; /* "f" */
static const int fr_fv_019[] = { 3, 4, 3, 3, 3, 18, 9, 0, -1 }; /* "g" */

static const int fr_fv_020[] = { 3, 4, 3, 3, 3, 20, 11, 0, -1 }; /* "h" */
static const int fr_fv_021[] = { 0, 4, 3, 3, 3, 20, 15, 0, -1 }; /* "i" */
static const int fr_fv_022[] = { 3, 4, 3, 3, 3, 20, 1, 0, -1 }; /* "j" */
static const int fr_fv_023[] = { 3, 4, 3, 3, 3, 20, 8, 0, -1 }; /* "k" */
static const int fr_fv_024[] = { 3, 4, 3, 3, 3, 20, 15, 0, -1 }; /* "l" */
static const int fr_fv_025[] = { 3, 4, 3, 3, 3, 19, 9, 0, -1 }; /* "m" */
static const int fr_fv_026[] = { 3, 1, 2, 7, 3, 4, 4, 3, -1 }; /* "n" */
static const int fr_fv_027[] = { 0, 1, 7, 6, 3, 4, 4, 3, -1 }; /* "o" */
static const int fr_fv_028[] = { 3, 1, 6, 6, 3, 4, 4, 3, -1 }; /* "p" */
static const int fr_fv_029[] = { 3, 1, 7, 2, 0, 4, 4, 3, -1 }; /* "s" */

static const int fr_fv_030[] = { 3, 1, 6, 2, 0, 4, 4, 3, -1 }; /* "s" */
static const int fr_fv_031[] = { 3, 1, 2, 7, 3, 4, 4, 3, -1 }; /* "t" */
static const int fr_fv_032[] = { 0, 1, 7, 6, 3, 4, 4, 3, -1 }; /* "u" */
static const int fr_fv_033[] = { 3, 1, 6, 6, 3, 4, 4, 3, -1 }; /* "v" */
static const int fr_fv_034[] = { 3, 1, 7, 2, 0, 4, 4, 3, -1 }; /* "w" */
static const int fr_fv_035[] = { 0, 1, 6, 2, 0, 4, 4, 3, -1 }; /* "y" */
static const int fr_fv_036[] = { 3, 4, 3, 3, 3, 9, 11, 0, -1 }; /* "z" */
static const int fr_fv_037[] = { 3, 4, 3, 3, 3, 4, 4, 3, -1 }; /* "pau" */
static const int fr_fv_038[] = { 3, 4, 3, 3, 3, 4, 4, 3, -1 }; /* "h#" */
static const int fr_fv_039[] = { 3, 4, 3, 3, 3, 4, 4, 3, -1 }; /* "brth" */

static const int *const fr_fvtable[] = {
    fr_fv_000, fr_fv_001, fr_fv_002, fr_fv_003, fr_fv_004, fr_fv_005, fr_fv_006,
    fr_fv_007, fr_fv_008, fr_fv_009, fr_fv_010, fr_fv_011, fr_fv_012, fr_fv_013,
    fr_fv_014, fr_fv_015, fr_fv_016, fr_fv_017, fr_fv_018, fr_fv_019, fr_fv_020,
    fr_fv_021, fr_fv_022, fr_fv_023, fr_fv_024, fr_fv_025, fr_fv_026, fr_fv_027,
    fr_fv_028, fr_fv_029, fr_fv_030, fr_fv_031, fr_fv_032, fr_fv_033, fr_fv_034,
    fr_fv_035, fr_fv_036, fr_fv_037, fr_fv_038, fr_fv_039
};

DEF_STATIC_CONST_VAL_STRING(featval_0, "+");
DEF_STATIC_CONST_VAL_STRING(featval_1, "l");
DEF_STATIC_CONST_VAL_STRING(featval_2, "3");
DEF_STATIC_CONST_VAL_STRING(featval_3, "-");
DEF_STATIC_CONST_VAL_STRING(featval_4, "0");
DEF_STATIC_CONST_VAL_STRING(featval_5, "s");
DEF_STATIC_CONST_VAL_STRING(featval_6, "1");
DEF_STATIC_CONST_VAL_STRING(featval_7, "2");
DEF_STATIC_CONST_VAL_STRING(featval_8, "d");
DEF_STATIC_CONST_VAL_STRING(featval_9, "a");
DEF_STATIC_CONST_VAL_STRING(featval_10, "r");
DEF_STATIC_CONST_VAL_STRING(featval_11, "p");
DEF_STATIC_CONST_VAL_STRING(featval_12, "f");
DEF_STATIC_CONST_VAL_STRING(featval_13, "n");
DEF_STATIC_CONST_VAL_STRING(featval_14, "b");
DEF_STATIC_CONST_VAL_STRING(featval_15, "v");
DEF_STATIC_CONST_VAL_STRING(featval_16, "g");
DEF_STATIC_CONST_VAL_STRING(featval_17, "i"); /* interdental */
DEF_STATIC_CONST_VAL_STRING(featval_18, "tr"); /* trill */
DEF_STATIC_CONST_VAL_STRING(featval_19, "t"); /* tap */
DEF_STATIC_CONST_VAL_STRING(featval_20, "ap"); /* approximant */

static const cst_val *const fr_featvals[] = {
    (cst_val *) &featval_0,  (cst_val *) &featval_1,
    (cst_val *) &featval_2,  (cst_val *) &featval_3,
    (cst_val *) &featval_4,  (cst_val *) &featval_5,
    (cst_val *) &featval_6,  (cst_val *) &featval_7,
    (cst_val *) &featval_8,  (cst_val *) &featval_9,
    (cst_val *) &featval_10, (cst_val *) &featval_11,
    (cst_val *) &featval_12, (cst_val *) &featval_13,
    (cst_val *) &featval_14, (cst_val *) &featval_15,
    (cst_val *) &featval_16, (cst_val *) &featval_17,
    (cst_val *) &featval_18, (cst_val *) &featval_19,
    (cst_val *) &featval_20, NULL
};

const cst_phoneset fr_phoneset = {
    "fr",
    fr_featnames,
    fr_featvals,
    fr_phonenames,
    "pau",
    39,
    fr_fvtable,
    0 /* not freeable */
};
