# Assembly Language
CHIP-8 formally only defines an instruction set, no assembly language. However, I will be
using the "psuedo-assembly" defined within Cowgod's CHIP-8 Technical Reference 
(see [here](references.md)), along with a few minor adjustments:

1. The file extension will be `.c8asm`.
2. I will support comments, which start with semicolons `;`.
3. I will also support *labels*, which start with a colon `:` and
   essentially map a specific line number with an easier-to-read label.
4. Lastly, I will have registers start with the dollar sign `$`. For example,
   the "VF" register will be written like: `$F`.

Example program:
```asm
; Simple infinite loop
:loop_start
   cls             ; Clear screen
   se $0 0x01      ; Set V0 to 1
   jp loop_start   ; Jump to the label above
```

## EBNF
For those unfamiliar, see [here](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form).

```ebnf
alphanum ::= ('A'...'Z' | 'a'...'z' | '0'...'9')* ;
whitespace ::= { " " | "\t" | "\r" | "\f" | "\b" | comment } ;
comment ::= ";", $any$, "\n" ;
line ::= { label | instruction } ;
label ::= alphanum, ":" ;
instruction ::= opcode, (operands)* ;
program ::= ( whitespace, line, whitespace, "\n" )* ;
```

## Instruction Set (Offline)
```
00E0 - CLS
00EE - RET
0nnn - SYS addr
1nnn - JP addr
2nnn - CALL addr
3xkk - SE Vx, byte
4xkk - SNE Vx, byte
5xy0 - SE Vx, Vy
6xkk - LD Vx, byte
7xkk - ADD Vx, byte
8xy0 - LD Vx, Vy
8xy1 - OR Vx, Vy
8xy2 - AND Vx, Vy
8xy3 - XOR Vx, Vy
8xy4 - ADD Vx, Vy
8xy5 - SUB Vx, Vy
8xy6 - SHR Vx {, Vy}
8xy7 - SUBN Vx, Vy
8xyE - SHL Vx {, Vy}
9xy0 - SNE Vx, Vy
Annn - LD I, addr
Bnnn - JP V0, addr
Cxkk - RND Vx, byte
Dxyn - DRW Vx, Vy, nibble
Ex9E - SKP Vx
ExA1 - SKNP Vx
Fx07 - LD Vx, DT
Fx0A - LD Vx, K
Fx15 - LD DT, Vx
Fx18 - LD ST, Vx
Fx1E - ADD I, Vx
Fx29 - LD F, Vx
Fx33 - LD B, Vx
Fx55 - LD [I], Vx
Fx65 - LD Vx, [I]
```
