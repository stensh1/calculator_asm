; Ivan Orshak, group #1321, 26.11.2023

; This file contains constants for console's procedures

; User commands
conExit   db '/ext', 0  
conHelp   db '/hlp', 0
conCls    db '/cls',  0

; Help message
conHelper db '----------------------------------------------------------------------', 0dh, 0ah, \
             '|Write down the expression you want to calculate using the operators:|', 0dh, 0ah, \
             '|                             +, -, *, /                             |', 0dh, 0ah, \
             '|as well as curly brackets:                                          |', 0dh, 0ah, \
             '|                             "(" and ")"                            |', 0dh, 0ah, \ 
             '|!Please put "=" at the end of the expression!                       |', 0dh, 0ah, \
             '|To exit the program, type "/ext", to clear the console "/cls"       |', 0dh, 0ah, \ 
             '----------------------------------------------------------------------', 0dh, 0ah, 0ah, 0 

; Unknown command message
conUnknownCmd db 'Unknown command! Expected "/ext", "/cls", "/help"', 0dh, 0ah, 0ah, 0

; Exeption divided by zero
conDivByZero db 'Division by zero.', 0dh, 0ah, 0ah, 0

; The first prog msg
conGreetings  db 'Hello! Type "/hlp" to see more.', 0dh, 0ah, 0ah, 0

; Wrong input alert
conWrongInput db 'The input contains errors, please check the correctness of the expression.', 0dh, 0ah, 0ah, 0
 
; String breaker
f1            db '%s', 13, 10, 0