%define NULL 0
%define STD_OUTPUT_HANDLE -11
%define endl 10

GLOBAL _printChar
GLOBAL _printText

EXTERN __imp__WriteConsoleA@20                          ; dword
EXTERN __imp__GetStdHandle@4                            ; dword
EXTERN _putchar

SECTION .text

_printChar: ; void ( const unsigned int _decimalCharacter[0] ) {
        push    ebp
        mov     ebp, esp
        %ifdef _WIN32
        push    NULL                           ;
        push    NULL                           ;
        push    1                              ; text length
        lea     eax, [ebp+8]                   ; decimal text as pointer
        push    eax                            ;
        push    STD_OUTPUT_HANDLE              ;
        call    near [__imp__GetStdHandle@4]   ;
        push    eax                            ; HANDLE
        call    near [__imp__WriteConsoleA@20] ;
        %else
        mov     eax, dword [ebp+8]
        push    eax
        call    _putchar
        %endif
        pop     ebp
        ret
;}

_printText: ; void ( unsigned int _lengthOfText[0], const unsigned int _decimalText[1...] ) {
        push    ebp
        mov     ebp, esp
        sub     esp, 20
        push    esi                  ;
        mov     esi, dword [ebp+8]   ; _lengthOfText
        push    edi                  ;
        lea     edi, dword [ebp+12]  ; _decimalText[ 0 ]
.printLoop:
        dec     esi                  ; lengthOfText
        js      _printText.return    ;
        push    dword [edi+esi*4]    ;
        call    _printChar           ;
        jmp     _printText.printLoop ;
.return:
        mov     esp, ebp
        pop     ebp
        ret
; }