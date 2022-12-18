; Reference: https://www.shido.info/lisp/callcc.html
(define mul (lambda (x y) (begin (display "mul,")(print (* x y))(* x y))))
(define print (lambda x (begin (display x)(newline))))
(define prod
  (lambda (ls)
    (call/cc
     (lambda (cc)
       (begin (print ls)
       (if (null? ls)
           1
         (let ((x (car ls)))
           (if (= 0 x)
               (begin (print "find 0")(cc 0))                           ;(2)-a
             (mul x (prod (cdr ls)))
           )
         )
       )
       )
       )))  
  )

; Reference: http://www.stdio.h.kyoto-u.ac.jp/jugyo1/scheme/SchemeNotes/continuation.html
(define (mul-cont x)
	  (call/cc (lambda (escape)
	     (letrec ((mul-aux (lambda (y) (print y)
                ;-----------
				(let ((result
    				       (cond ((null? y) 1)
				             ((= (car y) 0) (begin (print "find 0")(escape 0)))
					     (#t (mul (car y) (mul-aux (cdr y))))
                         )))
			           (print (list y result))
				   result)
                ;-----------
				 )))
	       (mul-aux x) ; 結局これが返る
           )
         ))
      )

(define v '(1 2 3 0 5))
(print (prod v))
(print (+ 100 (prod v)))
(print "# second")
(print (mul-cont v))
(print (+ 100 (mul-cont v)))
