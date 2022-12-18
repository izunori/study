; Reference: https://www.shido.info/lisp/callcc.html
(define mul (lambda (x y) (begin (display "mul,")(display (* x y))(newline)(* x y))))
(define prod
  (lambda (ls)
    (call/cc
     (lambda (cc)
       (begin (display ls) (newline)
       (if (null? ls)
           1
         (let ((x (car ls)))
           (if (= 0 x)
               (begin (display "find 0")(newline)(cc 0))                           ;(2)-a
             (mul x (prod (cdr ls)))
           )
         )
       )
       )
       )))  
  )
(define v '(1 2 3 0 5))
(display (+ 100 (prod v)))
