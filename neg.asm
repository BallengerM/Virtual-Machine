@SP
M=M-1
A=M
D=M
@FIRST_TRUE
D;JLT
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@END
0;JMP
(FIRST_TRUE)
@SP
M=M-1
A=M
D=M
@SECOND_TRUE
D;JLT
(SECOND_TRUE)
