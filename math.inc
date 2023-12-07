; Ivan Orshak, group #1321, 26.11.2023

; This file contains procedures for calculating results

; The procedure calculates the final value of the expression 
; written in postfix form in the queue
proc CalculateResult
    pushad
    call MathResetValues
  
    ; Getting stack pointer
    mov ebx, [mathStackPtr]
  
    ; Getting number of queue elements
    mov ecx, [expQueuePtr]
    
    mov esi, 0
    
  CalculateResultCycle:
      push ecx 
    
      ; Check is element number or digit
      mov eax, [expQueueFlags+4*esi]
      cmp eax, 1
      mov eax, [expQueue+4*esi]
      
      je CalculateResultIsOperator
      jne CalculateResultIsNumber
    
    CalculateResultIsNumber:
      call MathPushToStack
      jmp CalculateResultNextIt
    
    CalculateResultIsOperator:
      ; Getting 2 last number from stack
      call GetNumbers  
      
      ; Which operation need to do
      cmp eax, '+'
      je CalculateResultAdd
      cmp eax, '-'
      je CalculateResultSub 
      cmp eax, '*'
      je CalculateResultMult
      cmp eax, '/'
      je CalculateResultDiv 
      
    CalculateResultAdd:
      call AddNumber
      jmp CalculateResultSaveRes
    
    CalculateResultSub:
      call SubNumber
      jmp CalculateResultSaveRes 
    
    CalculateResultMult:
      call MulNumber
      jmp CalculateResultSaveRes
    
    CalculateResultDiv:
      push edx
      call DivNumber
      cmp dl, 1
      pop edx
      je CalculateResultDivExeption 
      jmp CalculateResultSaveRes
    
    CalculateResultSaveRes:
      call MathPushToStack
    
    CalculateResultNextIt:
      add esi, 1  
      pop ecx
  loop CalculateResultCycle
    
  CalculateResultEnd:  
    popad
    mov dl, 0
    ret
    
  CalculateResultDivExeption:
    pop ecx
    popad
    mov dl, 1
    ret
endp

; The procedure gets 2 last numbers from stack and writes them in NumberA and NumberB
proc GetNumbers
  pushad
  
  ; Getting stack pointer
  mov ebx, [mathStackPtr]
  
  sub ebx, 1
      mov eax, [mathStack+4*ebx] 
      mov [NumberA], eax 
      
      sub ebx, 1 
      mov eax, [mathStack+4*ebx]
      mov [NumberB], eax
      
      sub [mathStackPtr], 2

  popad
  ret
endp

; The procedure adds two numbers NumberA and numberB and stores the answer in eax
proc AddNumber  
  mov eax, [NumberB]
  add eax, [NumberA]
  
  ret
endp

; The procedure subs two numbers NumberA and numberB and stores the answer in eax
proc SubNumber  
  mov eax, [NumberB]
  sub eax, [NumberA]
  
  ret
endp

; The procedure multyplies two numbers NumberA and numberB and stores the answer in eax
proc MulNumber  
  mov eax, [NumberB]
  imul eax, [NumberA]
  
  ret
endp

; The procedure divides two numbers NumberA and numberB and stores the answer in eax
proc DivNumber  
    pushad
    mov eax, [NumberB]
    mov ebx, [NumberA]
    mov edx, 0
  
    cmp [NumberA], 0
    jne DivNumberNotNullDiv
    je DivNumberExeption
    
  DivNumberExeption:
    popad
    mov dl, 1
    jmp DivNumberEnd
  
  DivNumberNotNullDiv:
    div ebx
    mov [NumberA], eax
    popad
    mov eax, [NumberA]
    mov dl, 0 
    jmp DivNumberEnd
    
  DivNumberEnd:
    ret
endp

; The procedure pushes the value from eax to the stack
proc MathPushToStack
  pushad
  
  ; Getting stack pointer
  mov ebx, [mathStackPtr]
  
  mov [mathStack+4*ebx], eax
  add [mathStackPtr], 1
  
  popad
  ret
endp

; Procedure prints result in console
proc PrintResult
  pushad
  
  mov eax, [mathStack] 
  cinvoke printf, <'= %d'>, eax
  ; The new string
  cinvoke printf, f1, '', 0
  cinvoke printf, f1, '', 0
  
  popad
  ret
endp

proc MathResetValues
  mov [mathStackPtr], 0
  
  ret
endp