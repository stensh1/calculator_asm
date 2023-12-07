; Ivan Orshak, group #1321, 26.11.2023

; This is the main file of program

format pe console 
entry Start

include 'win32ax.inc'

section '.code' code readable executable
  Start:    
    call ConsoleSetConfig
    call ConsoleRun  
    
  Exit:
    invoke ExitProcess, 0 
  
  ; Upload modules with procedures
  include 'console.inc'    
    
section '.const' data readable
  ; Console module data
  include 'console_consts.inc'
  
section '.data' data readable writeable
  ; Parser module data
  include 'parser_data.inc'
  ; Math module data
  include 'math_data.inc'
  
  ; The raw long string is added last. GLOBAL
  exp dd 255 dup (0)
  
section '.import' import data readable
  library kernel,'KERNEL32.DLL', \          
  msvcrt, 'MSVCRT.DLL'
  
  import kernel,\ 
    SetConsoleTitleA, 'SetConsoleTitleA',\ 
    StrLen, 'lstrlen',\    
    ExitProcess, 'ExitProcess'
    
  import msvcrt, \
    getch,  '_getch', \ 
    system, 'system', \             
    printf, 'printf', \
    scanf,  'scanf'