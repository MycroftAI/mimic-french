;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;                                                                       ;;
;;;                Centre for Speech Technology Research                  ;;
;;;                     University of Edinburgh, UK                       ;;
;;;                         Copyright (c) 1998                            ;;
;;;                        All Rights Reserved.                           ;;
;;;                                                                       ;;
;;;  Permission is hereby granted, free of charge, to use and distribute  ;;
;;;  this software and its documentation without restriction, including   ;;
;;;  without limitation the rights to use, copy, modify, merge, publish,  ;;
;;;  distribute, sublicense, and/or sell copies of this work, and to      ;;
;;;  permit persons to whom this work is furnished to do so, subject to   ;;
;;;  the following conditions:                                            ;;
;;;   1. The code must retain the above copyright notice, this list of    ;;
;;;      conditions and the following disclaimer.                         ;;
;;;   2. Any modifications must be clearly marked as such.                ;;
;;;   3. Original authors' names are not deleted.                         ;;
;;;   4. The authors' names are not used to endorse or promote products   ;;
;;;      derived from this software without specific prior written        ;;
;;;      permission.                                                      ;;
;;;                                                                       ;;
;;;  THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK        ;;
;;;  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      ;;
;;;  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   ;;
;;;  SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE     ;;
;;;  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    ;;
;;;  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   ;;
;;;  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          ;;
;;;  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       ;;
;;;  THIS SOFTWARE.                                                       ;;
;;;                                                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;;  Definition of Festival lexicon
;;;

(require 'pos)

(define (fr_lts_function word feats)
  "(fr_lts_function word feats)
Function called for fr lexicon when word is not found in lexicon.  Uses
LTS rules trained from the original lexicon, and lexical stress
prediction rules."
  (require 'lts)
  (if (not (boundp 'fr_lts_rules))
      (load "fr_lts_rules.scm"))
  (let ((syls) (phones))
    (begin
	  (set! phones (lts_predict word fr_lts_rules))
      ; This only works if the only phonemes ending with a number are vowels, with the syllabic stress being given by the number
	  (set! syls (lex.syllabify.phstress phones))
    )
    (list word nil syls)
  )
)


(define (frlex_addenda)
  "(frlex_addenda)
Add entries to the current lexicon.  These are basically
words that are not in the lexicon."
  ;(lex.add.entry
  ; '("bought" v (((b ao t) 1))))
  ;(lex.add.entry
  ; '("edinburgh" n (((eh d) 1) ((ah n) 0) ((b ax ) 0) ((r ow) 0))))
  ;(lex.add.entry
  ; '("non-intoxicating" () 
  ;    (((n aa n) 1) ((ih n t) 0) ((aa k) 1) ((s ih k) 0) ((ey t) 1) ((ih ng) 0))))
  )

(load "fr_phoneset.scm")
(lex.create "fr")
(lex.set.compile.file "fr_dict.out")
(lex.set.phoneset "fr")
(lex.set.lts.method 'fr_lts_function)
(frlex_addenda)

