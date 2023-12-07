; Ivan Orshak, group #1321, 26.11.2023

; This file contains procedures for parsing an input string 
; and implementing an algorithm for translating an expression infix from to a postfix form.

; Parser, a converter of the infix expression form to the postfix form
proc ParseIt
    pushad
    call ResetValues
    ; Setting the counter equal to the number of characters in the string
    invoke StrLen, exp
    ; Length in eax now
    mov ecx, eax
    ; Uploading the address of the string
    mov ebx, exp

  ; While input string not end
  ParseItCycle:
    xor eax, eax
    mov al, [ebx]
      
    push ecx
    
    ; If current char is digit  
    call IsDigit 
    cmp dl, 0
    je ParseItNextIt
    
    ; If current char is operator
    call IsOperator
    cmp dl, 0
    je ParseItNextIt
    
    ; If current char is bracket or '='
    call IsBracket 
    ; If the edn of input
    cmp dl, 1
    je ParseItSaveReg
    ; If not allowed char
    cmp dl, 2
    je ParseItExeption
        
  ParseItNextIt: 
    pop ecx
    ; Next symbol in input string
    add ebx, 1
  loop ParseItCycle

  ParseItEnd:
    ret
    
  ParseItSaveReg:
    pop ecx
    popad
    jmp ParseItEnd
    
  ParseItExeption:
    pop ecx
    popad
    mov dl, 1
    jmp ParseItEnd    
    
endp

; Checks whether the current character is an bracket or equal sign
; or is not allowed
proc IsBracket
    pushad
    ; If current char is '('
    cmp eax, '('
    je IsBracketPushToStack
      
    ; If current char is ')'
    cmp eax, ')'
    je IsBracketPushToQueue  
     
    ; If current char is '='
    cmp eax, '='
    je IsBracketPushToQueueEverything
    ; If current char is not allowed
    jne IsBracketExeption   
  
  IsBracketPushToStack:
    call PushStack
    popad
    mov dl, 0
    jmp IsBracketEnd
  
  IsBracketPushToQueue:
    call PushQueue
    popad
    mov dl, 0
    jmp IsBracketEnd
  
  IsBracketPushToQueueEverything:
    call PushQueue
    popad
    mov dl, 1
  
  IsBracketEnd:
    ret
    
  IsBracketExeption:
    popad
    mov dl, 2
    jmp IsBracketEnd
endp

; Checks whether the current character is an operator
proc IsOperator    
    pushad
  
    ; If current char is operator
    cmp eax, '+'
    je IsOperatorCheckConditions
    cmp eax, '-'
    je IsOperatorCheckConditions
    cmp eax, '*'
    je IsOperatorCheckConditions
    cmp eax, '/'
    je IsOperatorCheckConditions
    jne IsOperatorNotOperator
    
  IsOperatorCheckConditions:
    ; If stack is empty
    pushad
    mov ebx, [expStackPtr]
    cmp ebx, 0
    popad
    je IsOperatorPushToStack
    
    ; If '(' on the stack top
    pushad
    mov esi, [expStackPtr]
    sub esi, 1
    mov eax, [expStack+4*esi]
    cmp eax, '('
    popad
    je IsOperatorPushToStack 
    
    ; If the current operator has a higher priority
    push edx
    call CheckTopPriority
    cmp dl, 1
    pop edx
    je IsOperatorOperator 
    
    ; If the current operator has a lower or equal priority
    jmp IsOperatorPushToQueue
    
  IsOperatorPushToQueue:
    call PushQueue
      
  IsOperatorPushToStack:
    call PushStack
    jmp IsOperatorOperator 
  
  IsOperatorEnd:
    ret
    
  IsOperatorNotOperator:
    popad
    mov dl, 1 
    jmp IsOperatorEnd 
    
  IsOperatorOperator:
    popad
    mov dl, 0
    jmp IsOperatorEnd  
  
endp

; Checks whether '+' or '-' operators of the lowest priority are at the top of the stack
proc CheckTopPriority
    pushad
  
    ; Getting pointer of the last stack element
    mov esi, [expStackPtr]
    sub esi, 1
  
    mov ebx, [expStack+4*esi]
    cmp ebx, '+'
  
    popad 
    je CheckTopPriorityPushOperator
        
    pushad
  
    mov esi, [expStackPtr]
    sub esi, 1
    mov ebx, [expStack+4*esi]
    cmp ebx, '-'
  
    popad
    je CheckTopPriorityPushOperator
    jne CheckTopPriorityNotPushOperator 
  
  CheckTopPriorityPushOperator:
    cmp eax, '*'
    je CheckTopPriorityPriorityTrue
    cmp eax, '/'
    je CheckTopPriorityPriorityTrue
    jne CheckTopPriorityNotPushOperator
    
  CheckTopPriorityPriorityTrue:  
    call PushStack
    mov dl, 1
    jmp CheckTopPriorityEnd
  
  CheckTopPriorityNotPushOperator:
    mov dl, 0 
    jmp CheckTopPriorityEnd  

  CheckTopPriorityEnd:
    ret
endp

; Puts the value from the eax register on the stack
proc PushStack
  pushad   
  
  ; Getting pointer
  mov esi, [expStackPtr]
   
  mov [expStack+4*esi], eax 
  add [expStackPtr], 1
   
  popad
  ret
endp

; Pushes values from the stack into the queue until meets '(' or until reachs the end of the stack
proc PushQueue
    pushad
    mov ecx, 255
  
  ; Iteration by the size of stack (255)
  PushQueueCycle:
      push ecx
    
      ; Getting pointers
      mov esi, [expQueuePtr]
      mov ebx, [expStackPtr]
    
      ; If it is the end of stack
      cmp ebx, 0
      je PushQueueBreakCycle
    
      ; The pointer points to the next empty element
      sub ebx, 1
      mov eax, [expStack+4*ebx]
    
      ; If we meet left bracket '(' - should delete it
      cmp eax, '('
      je PushQueueDeleteBracket
      jne PushQueueAddOperator 
    
    PushQueueDeleteBracket:
      sub [expStackPtr], 1
      jmp PushQueueBreakCycle 
      
    PushQueueBreakCycle:
      pop ecx
      jmp PushQueueEnd
    
    PushQueueAddOperator:
      mov [expQueue+4*esi], eax
      ; Flag: 1 - the number in queue is operator
      mov [expQueueFlags+4*esi], 1
      add [expQueuePtr], 1
      sub [expStackPtr], 1
          
      pop ecx
  loop PushQueueCycle
  
  PushQueueEnd:
    popad
    ret
endp

; Checks if the current character is a digit and adds it to the current number or,
; if the number is made up, sends it to the queue
proc IsDigit
    ; 0 <= number
    cmp al, '0'
    jge IsDigitTopLimit    
    ; or check is smth in number
    mov dl, 1
    call IsNumber
    je IsDigitEnd
    ; if smth in number
    call AddQueue
    jmp IsDigitEnd
        
  IsDigitTopLimit:
    ; number <= 9
    cmp al, '9'
    jle IsDigitAddDigit
    mov dl, 1
    call IsNumber
    je IsDigitEnd     
    ; if smth in number
    call AddQueue
    jmp IsDigitEnd  
    
  ; Multiply by 10 the number and add the next digit  
  IsDigitAddDigit:
    mov dl, 0
    pushad
      
    ; If the 1-st digit
    call IsNumber
    je IsDigitAdd 
      
    ; number *= 10
    pushad
    xor eax, eax
    mov eax, [number]
    mov bx, 10
    mul bx
    mov [number], eax
    popad 
      
  IsDigitAdd:
    pushad
    mov [numberFlag], '1'
    sub al, 48
    add [number], eax
    popad
        
    popad
      
  IsDigitEnd:
    ret
endp
  
; Is there some number in 'number'
proc IsNumber
  push eax
  mov al, [numberFlag]
  cmp al, '0'
  pop eax
    
  ret
endp
  
; if current symbol not a digit -> write number in queue
proc AddQueue
  pushad
  mov esi, [expQueuePtr]
  mov eax, [number]
  mov [expQueue+4*esi], eax
  mov [expQueueFlags+4*esi], 0 
  add [expQueuePtr], 1
  mov [numberFlag], '0'
  mov [number], 0
  popad
  
  ret
endp

; Reset vars before each parsing
proc ResetValues
  mov [number], 0
  mov [numberFlag], '0'
  mov [expQueuePtr], 0
  mov [expStackPtr], 0

  ret
endp