(set! vowels '(Ahn a @ E Ehn O Ohn P2 P9 P9hn e i o u y ))

(defPhoneSet
  fr
  ;;;  Phone Features
  (;; vowel or consonant
   (vc + -)  
   ;; vowel length: short long dipthong schwa
   (vlng s l d a 0)
   ;; vowel height: high mid low
   (vheight 1 2 3 0)
   ;; vowel frontness: front mid back
   (vfront 1 2 3 0)
   ;; lip rounding
   (vrnd + - 0)
   ;; consonant type: stop fricative affricate nasal lateral approximant
   (ctype s f a n l r 0)
   ;; place of articulation: labial alveolar palatal labio-dental
   ;;                         dental velar glottal
   (cplace l a p b d v g 0)
   ;; consonant voicing
   (cvox + - 0)
   )
  ;; Phone set members
  (
   ;; FIXME: The phone features are not set (They are randomly set)
   (Ahn  +   l   3   3   -   0   0   0)
   (a  +   l   3   3   -   0   0   0) 
   (@  +   s   3   1   -   0   0   0) 
   (E  +   s   2   2   -   0   0   0) 
   (Ehn  +   l   3   3   +   0   0   0) 
   (H  +   d   3   2   -   0   0   0) 
   (O  +   a   2   2   -   0   0   0) 
   (Ohn +   a   2   2   -   r   a   +)
   (P2  +   d   3   2   -   0   0   0)
   (P9  +   d   3   2   -   0   0   0) 
   (P9hn  +   d   3   2   -   0   0   0) 
   (b   -   0   0   0   0   s   l   +)
   (c  -   0   0   0   0   a   p   -)
   (d   -   0   0   0   0   s   a   +)
   (e  +   s   2   1   -   0   0   0) 
   (f   -   0   0   0   0   f   b   -)
   (g   -   0   0   0   0   s   v   +)
   (h  -   0   0   0   0   f   g   -)
   (i  +   s   1   1   -   0   0   0) 
   (j  -   0   0   0   0   a   p   +)
   (k   -   0   0   0   0   s   v   -)
   (l   -   0   0   0   0   l   a   +)
   (m   -   0   0   0   0   n   l   +)
   (n   -   0   0   0   0   n   a   +)
   (o  +   d   2   3   +   0   0   0)
   (p   -   0   0   0   0   s   l   -)
   (P   -   0   0   0   0   s   l   -)
   (R   -   0   0   0   0   r   a   +)
   (S   -   0   0   0   0   f   a   -)
   (s  -   0   0   0   0   f   p   -)
   (t   -   0   0   0   0   s   a   -)
   (u  +   s   1   3   +   0   0   0) 
   (v   -   0   0   0   0   f   b   +)
   (w   -   0   0   0   0   r   l   +)
   (y   -   0   0   0   0   r   p   +)
   (Z   -   0   0   0   0   f   a   +)
   (z   -   0   0   0   0   f   a   +)
   (# -   0   0   0   0   0   0   -)
   (pau -   0   0   0   0   0   0   -)
   (h#  -   0   0   0   0   0   0   -)
   (brth -   0   0   0   0   0   0   -)
  )
)

(PhoneSet.silences '(pau h# brth #))

