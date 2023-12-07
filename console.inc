; Ivan Orshak, group #1321, 26.11.2023

; This file contains procedures for working with the console
; ConsoleSetConfig, ConsoleRun, CmdHandler

; Configuring the console
proc ConsoleSetConfig
  invoke SetConsoleTitleA, 'Calculator'  
    
  ret
endp

; The main procedure of the program, daemon
proc ConsoleRun
    cinvoke printf, f1, conGreetings
    
  ConsoleRunStart:
    cinvoke printf, 'Calculate: ', 0    
    cinvoke scanf, <'%s', 0>, exp
    
    push edx
    call IsCommand
    cmp dl, 1
    pop edx
    je ConsoleRunHandleCmd
    jne ConsoleRunParse 
    
  ConsoleRunHandleCmd:
    call CmdHandler
    jmp ConsoleRunStart 
      
  ConsoleRunParse:
    push edx
    call ParseIt
    cmp dl, 1
    pop edx
    je ConsoleRunInputExeption
      
    push edx
    call CalculateResult
    cmp dl, 1
    pop edx
    je ConsoleRunDivExeption
    jne ConsoleRunPrintResult 
      
  ConsoleRunPrintResult:
    call PrintResult
    jmp ConsoleRunStart
      
  ConsoleRunDivExeption:
    cinvoke printf, conDivByZero
    jmp ConsoleRunStart
      
  ConsoleRunInputExeption:
    cinvoke printf, conWrongInput
    jmp ConsoleRunStart 
  
    ret
endp

; Checks is any command entered
proc IsCommand
    pushad
    xor eax, eax
  
    mov ebx, exp
    mov al, [ebx]
  
    cmp eax, '/'
    je IsCommandTrue
    jne IsCommandFalse
  
  IsCommandTrue:
    popad
    mov dl, 1
    jmp IsCommandEnd 
    
  IsCommandFalse:
    popad
    mov dl, 0
    jmp IsCommandEnd
  
  IsCommandEnd:
    ret
endp

; The procedure for handling user-entered commands
proc CmdHandler
    pushad
  
    mov eax, [exp]
  
    cmp eax, '/ext'
    je Exit
  
    cmp eax, '/hlp'
    je CmdHandlerPrintHelp
  
    cmp eax, '/cls'
    je CmdHandlerClearConsole
    jne CmdHandlerUnknownCommand
    
  CmdHandlerUnknownCommand:
    cinvoke printf, <'%s'>, conUnknownCmd
    jmp CmdHandlerEnd 
  
  CmdHandlerClearConsole:
    cinvoke system, 'cls'
    jmp CmdHandlerEnd 
    
  CmdHandlerPrintHelp:
    cinvoke system, 'cls'
    cinvoke printf, <'%s'>, conHelper
    jmp CmdHandlerEnd
  
  CmdHandlerEnd:
    popad
    ret

endp

; Upload modules with procedures
include 'parser.inc'
include 'math.inc'