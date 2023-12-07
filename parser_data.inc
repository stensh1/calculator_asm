; Ivan Orshak, group #1321, 26.11.2023

; This file contains variables for parser's procedures

; Number from input
number      dd 0

; Is the number being processed
numberFlag  db '0'

; Queue and stack and their pointers for algorithm
expQueue      dd 255 dup(0)
expQueueFlags dd 255 dup(0)
expStack      dd 255 dup(0)
expStackPtr   dd 0
expQueuePtr   dd 0