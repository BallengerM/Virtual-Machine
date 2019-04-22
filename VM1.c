/*
	Makayla Ballenger
	CS 271
	VM I
    March 23, 2018

    Citations -
    Bri helped with connecting @ with token3.
    Converting int to a string - https://stackoverflow.com/questions/36274902/convert-int-to-string-in-standard-c
    Return assembly code help - http://nand2tetris-questions-and-answers-forum.32033.n3.nabble.com/Return-code-or-Call-code-not-working-cont-td4031395.html
        NOTE: ONLY USED CADET1620's SECOND RESPONSE
    Project 8 Lecture slides
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


char* getToken(char*, int);
char* processLine(char*);

//Arithmetic Commands
char* add(); 	//return the assembly for add
char* sub(); 	//return the assembly for sub
char* neg(); 	//return the assembly for neg
char* eq(); 	//return the assembly for eq
char* gt(); 	//return the assembly for gt
char* lt();		//return the assembly for lt
char* and();	//return the assembly for and
char* or();		//return the assembly for or
char* not();	//return the assembly for not
char* arithmetic(char*); //given a command, call the appropriate function above

//Memory Commands
char* pop(char*,char*);		//return the assembly for pop
char* push(char*, char*); 	//return the assembly for push
char* memory(char*, char*, char*);	//given a command, call push or pop

//Branching Commands
char* label(char*);
char* gotoloop(char*);
char* ifgoto(char*);
char* branchingCommands(char*, char*);

//Function Commands
char* function(char*,char*);
char* call(char*,char*);
char* terminate();
char* functionCommands(char*,char*,char*);

int counter();
char* intToString();
int charToInt(char digit);
int stringToInt(char* string);

int main(){
    /*
    FILE *fp;
    char buffer[256];

    fp = fopen("vmtestfile.txt", "r");

    FILE *op;
    op = fopen("out.txt", "w");

    if(fp == NULL){
        printf("Error opening file\n");
    }
    else if(op == NULL){
        printf("Error opening file\n");
    }
    else{

        while(fgets(buffer, 256, fp) != NULL){

			buffer[strcspn(buffer, "\r\n")] = 0;

            printf("---%s\n",buffer);
            char* result = processLine(buffer);
            printf("%s\n",result);
            fputs(result,op);
            fputs("\n",op);
            free(result);
        }

        fclose(fp);
        fclose(op);
    }
    */

    printf("FUNCTION\n");
    char* line = "function SimpleFunction 2";
    printf("%s\n",processLine(line));
    printf("CALL\n");
    char* line2 = "call SimpleFunction 1";
    printf("%s\n",processLine(line2));
    printf("RETURN\n");
    char* line3 = "return";
    printf("%s\n",processLine(line3));
	return 0;
}

//You should write your functions down here
char* getToken(char* string, int position){
    int i = position;
    int end = position;
    while (string[end] != ' '){
        end++;
    }
    int size = end - position;
    char* command = malloc((sizeof(char)*size)+1);
    int j = 0;
    while(string[position] != ' '){
        command[j]=string[position];
        position++;
        j++;
    }
    command[size]='\0';
    return command;
}
char* processLine(char* string){

    int tokenCount = 0;
    char* token1 = NULL;
    char* token2 = NULL;
    char* token3 = NULL;
    int position = 0;
    while (tokenCount < 3){
        //Gets the command token
        if (tokenCount == 0){
            token1 = getToken(string,position);
            position = strlen(token1) + 1;
        }
        else {
            //If there is more than one token in the string

            if (strlen(string)!= strlen(token1)){
                //Gets the memory segment token
                if (tokenCount == 1){
                    token2 = getToken(string,position);
                    position = position + strlen(token2) + 1;
                }
                //Gets the index token
                else if (tokenCount == 2){
                    token3 = getToken(string,position);
                }
            }
            //Makes sure that token2 and token3 are both null if they need to be

            else{
                token2 = NULL;
                token3 = NULL;
            }

        }
        tokenCount++;
    }
    //Means that there was only one token
    if (token2 == NULL && token3 == NULL){
        //Feeds token1 into the arithmetic decision maker
        if (strcmp(token1,"return") == 0){
            return functionCommands(token1,token2,token3);
        }
        else {
            return arithmetic(token1);
        }
    }
    else{
        //Feeds token1, token2, token3 into the memory decision maker
        if (strcmp(token1,"push") == 0 || strcmp(token1,"pop") == 0){
            return memory(token1,token2,token3);
        }
        else if (strcmp(token1,"function") == 0 || strcmp(token1,"call") == 0){
            return functionCommands(token1,token2,token3);
        }
        else{
            return branchingCommands(token1,token2);
        }
    }

}
char* add(){
    char* assembly = "@SP\nM=M-1\nA=M\nD=M\n@13\nM=D\n@SP\nM=M-1\nA=M\nM=M+D\n@SP\nM=M+1";
    return assembly;
}
char* sub(){
    char* assembly = "@SP\nM=M-1\nA=M\nD=M\n@13\nM=D\n@SP\nM=M-1\nA=M\nM=M-D\n@SP\nM=M+1";
    return assembly;
}
char* neg(){
    char* assembly = "@SP\nM=M-1\nA=M\nM=-M\n@SP\nM=M+1";
    return assembly;
}
char* eq(){
    char* assembly = malloc((sizeof(char))*300);
    char* start = "@SP\nM=M-1\nA=M\nD=M\n@13\nM=D\n@SP\nM=M-1\nA=M\nM=M-D\nD=M\n@EQUAL";
    char* labels = intToString(counter());
    strcpy(assembly,start);
    strcat(assembly,labels);
    char* eqFalse = "\nD;JEQ\n@0\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@END";
    strcat(assembly,eqFalse);
    strcat(assembly,labels);
    char* next = "\n0;JMP\n(EQUAL";
    strcat(assembly,next);
    strcat(assembly,labels);
    char* eqTrue = ")\nA=-1\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n(END";
    strcat(assembly,eqTrue);
    strcat(assembly,labels);
    char* end = ")";
    strcat(assembly,end);
    return assembly;
}
char* gt(){
    char* assembly = malloc((sizeof(char))*300);
    char* start = "@SP\nM=M-1\nA=M\nD=M\n@13\nM=D\n@SP\nM=M-1\nA=M\nM=M-D\nD=M\n@GREATER_THAN";
    char* labels = intToString(counter());
    strcpy(assembly,start);
    strcat(assembly,labels);
    char* eqFalse = "\nD;JGT\n@0\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@END";
    strcat(assembly,eqFalse);
    strcat(assembly,labels);
    char* next = "\n0;JMP\n(GREATER_THAN";
    strcat(assembly,next);
    strcat(assembly,labels);
    char* eqTrue = ")\nA=-1\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n(END";
    strcat(assembly,eqTrue);
    strcat(assembly,labels);
    char* end = ")";
    strcat(assembly,end);
    return assembly;
}
char* lt(){
    char* assembly = malloc((sizeof(char))*300);
    char* start = "@SP\nM=M-1\nA=M\nD=M\n@13\nM=D\n@SP\nM=M-1\nA=M\nM=M-D\nD=M\n@LESS_THAN";
    char* labels = intToString(counter());
    strcpy(assembly,start);
    strcat(assembly,labels);
    char* eqFalse = "\nD;JLT\n@0\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n@END";
    strcat(assembly,eqFalse);
    strcat(assembly,labels);
    char* next = "\n0;JMP\n(LESS_THAN";
    strcat(assembly,next);
    strcat(assembly,labels);
    char* eqTrue = ")\nA=-1\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n(END";
    strcat(assembly,eqTrue);
    strcat(assembly,labels);
    char* end = ")";
    strcat(assembly,end);
    return assembly;
}
char* and(){
    char* assembly = "@SP\nM=M-1\nA=M\nD=M\nA=A-1\nM=D&M";
    return assembly;
}
char* or(){
    char* assembly = "@SP\nM=M-1\nA=M\nD=M\nA=A-1\nM=D|M";
    return assembly;
}
char* not(){
    char* assembly = "@SP\nA=M-1\nM=!M";
    return assembly;
}
char* arithmetic(char* command){
    if (strcmp(command,"add") == 0){
        return add();
    }
    else if (strcmp(command,"sub") == 0){
        return sub();
    }
    else if (strcmp(command,"neg") == 0){
        return neg();
    }
    else if (strcmp(command,"eq") == 0){
        return eq();
    }
    else if (strcmp(command,"gt") == 0){
        return gt();
    }
    else if (strcmp(command,"lt") == 0){
        return lt();
    }
    else if (strcmp(command,"and") == 0){
        return and();
    }
    else if (strcmp(command,"or") == 0){
        return or();
    }
    else if (strcmp(command,"not") == 0){
        return not();
    }
    else{
        return "ERROR\n";
    }
}
char* pop(char* memorySegment, char* index){
    if (strcmp(memorySegment,"static") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* start = "@SP\nM=M-1\nA=M\nD=M\n@";
        strcpy(assembly,start);
        int place = stringToInt(index) + 16;
        char* location = intToString(place);
        strcat(assembly,location);
        char* end = "\nM=D";
        strcat(assembly,end);
        return assembly;
    }
    else if (strcmp(memorySegment,"this") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* first = "@";
        char* item = index;
        strcpy(assembly,first);
        strcat(assembly,item);
        char* end = "\nD=A\n@14\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@13\nM=D\n@14\nD=M\n@THIS\nD=D+M\n@14\nM=D\n@13\nD=M\n@14\nA=M\nM=D";
        strcat(assembly,end);
        return assembly;
    }
    else if (strcmp(memorySegment,"local") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* first = "@";
        char* item = index;
        strcpy(assembly,first);
        strcat(assembly,item);
        char* end = "\nD=A\n@14\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@13\nM=D\n@14\nD=M\n@LCL\nD=D+M\n@14\nM=D\n@13\nD=M\n@14\nA=M\nM=D";
        strcat(assembly,end);
        return assembly;
    }
    else if (strcmp(memorySegment,"argument") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* first = "@";
        char* item = index;
        strcpy(assembly,first);
        strcat(assembly,item);
        char* end = "\nD=A\n@14\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@13\nM=D\n@14\nD=M\n@ARG\nD=D+M\n@14\nM=D\n@13\nD=M\n@14\nA=M\nM=D";
        strcat(assembly,end);
        return assembly;
    }
    else if (strcmp(memorySegment,"that") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* first = "@";
        char* item = index;
        strcpy(assembly,first);
        strcat(assembly,item);
        char* end = "\nD=A\n@14\nM=D\n@SP\nM=M-1\nA=M\nD=M\n@13\nM=D\n@14\nD=M\n@THAT\nD=D+M\n@14\nM=D\n@13\nD=M\n@14\nA=M\nM=D";
        strcat(assembly,end);
        return assembly;
    }
    else if (strcmp(memorySegment,"constant") == 0){
        char* assembly = "@SP\nM=M-1";
        return assembly;
    }
    else if (strcmp(memorySegment,"pointer") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* start = "@SP\nM=M-1\nA=M\nD=M\n@";
        strcpy(assembly,start);
        int place = stringToInt(index) + 3;
        char* location = intToString(place);
        strcat(assembly,location);
        char* end = "\nM=D";
        strcat(assembly,end);
        return assembly;
    }
    else if (strcmp(memorySegment,"temp") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* start = "@SP\nM=M-1\nA=M\nD=M\n@";
        strcpy(assembly,start);
        int place = stringToInt(index) + 5;
        char* location = intToString(place);
        strcat(assembly,location);
        char* end = "\nM=D";
        strcat(assembly,end);
        return assembly;
    }
    else {
        return "ERROR\n";
    }
}
char* push(char* memorySegment, char* index){
    if (strcmp(memorySegment,"static") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* start = "@";
        strcpy(assembly,start);
        int place = stringToInt(index) + 16;
        char* location = intToString(place);
        strcat(assembly,location);
        char* end = "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        strcat(assembly,end);
        return assembly;
    }
    else if (strcmp(memorySegment,"this") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* first = "@";
        char* item = index;
        strcpy(assembly,first);
        strcat(assembly,item);
        char* end = "\nD=A\n@THIS\nA=M\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        strcat(assembly,end);
        return assembly;
    }
    else if (strcmp(memorySegment,"local") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* first = "@";
        char* item = index;
        strcpy(assembly,first);
        strcat(assembly,item);
        char* end = "\nD=A\n@LCL\nA=M\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        strcat(assembly,end);
        return assembly;
    }
    else if (strcmp(memorySegment,"argument") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* first = "@";
        char* item = index;
        strcpy(assembly,first);
        strcat(assembly,item);
        char* end = "\nD=A\n@ARG\nA=M\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        strcat(assembly,end);
        return assembly;
    }
    else if (strcmp(memorySegment,"that") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* first = "@";
        char* item = index;
        strcpy(assembly,first);
        strcat(assembly,item);
        char* end = "\nD=A\n@THAT\nA=M\nA=D+A\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        strcat(assembly,end);
        return assembly;
    }
    else if (strcmp(memorySegment,"constant") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* start = "@";
        char* item = index;
        strcpy(assembly,start);
        strcat(assembly,item);
        char* rest = "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        strcat(assembly,rest);
        return assembly;
    }
    else if (strcmp(memorySegment,"pointer") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* start = "@";
        strcpy(assembly,start);
        int place = stringToInt(index) + 3;
        char* location = intToString(place);
        strcat(assembly,location);
        char* end = "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        strcat(assembly,end);
        return assembly;
    }
    else if (strcmp(memorySegment,"temp") == 0){
        char* assembly = malloc((sizeof(char))*200);
        char* start = "@";
        strcpy(assembly,start);
        int place = stringToInt(index) + 5;
        char* location = intToString(place);
        strcat(assembly,location);
        char* end = "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        strcat(assembly,end);
        return assembly;
    }
    else {
        return "ERROR\n";
    }
}
char* memory(char* command, char* memorySegment, char* index){
    if (strcmp(command,"pop") == 0){
        return pop(memorySegment,index);
    }
    else if (strcmp(command,"push") == 0){
        return push(memorySegment,index);
    }
    else{
        return "ERROR";
    }
}
char* label(char* name){
    char* assembly = malloc((sizeof(char))*100);
    char* start = "(";
    char* label = name;
    strcpy(assembly,start);
    strcat(assembly,name);
    char* end = ")";
    strcat(assembly,end);
    return assembly;
}
char* gotoloop(char* name){
    char* assembly = malloc((sizeof(char))*100);
    char* start = "@";
    char* label = name;
    strcpy(assembly,start);
    strcat(assembly,label);
    char* end = "\n0;JMP";
    strcat(assembly,end);
    return assembly;
}
char* ifgoto(char* name){
    char* assembly = malloc((sizeof(char))*100);
    char* start = "@SP\nM=M-1\nA=M\nD=M\n@";
    char* label = name;
    strcpy(assembly,start);
    strcat(assembly,label);
    char* end = "\nD;JNE";
    strcat(assembly,end);
    return assembly;
}
char* branchingCommands(char* command, char* name){
    if (strcmp(command,"label") == 0){
        return label(name);
    }
    else if (strcmp(command,"goto") == 0){
        return gotoloop(name);
    }
    else if (strcmp(command,"if-goto") == 0){
        return ifgoto(name);
    }
    else{
        return "ERROR\n";
    }
}
char* function(char* name, char* numVar){
    char* assembly = malloc((sizeof(char))*1000);
    char* start = "(";
    char* label = name;
    strcpy(assembly,start);
    strcat(assembly,label);
    char* end = ")\n";
    strcat(assembly,end);
    char* pushNumVar;
    char* ending = "\n";
    int runNum = stringToInt(numVar);
    while (runNum > 0){
        pushNumVar = push("constant", "0");
        runNum--;
        strcat(assembly,pushNumVar);
        strcat(assembly,ending);
    }
    return assembly;
}
char* call(char* name, char* numVar){
    char* assembly = malloc((sizeof(char))*1000);
    char* pushra = "@SP\nA=M\n";
    char* pushlcl = "@LCL\nA=M\n";
    char* pusharg = "@ARG\nA=M\n";
    char* pushthis = "@THIS\nA=M\n";
    char* pushthat = "@THAT\nA=M\n";
    char* pushstuff = "D=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    strcpy(assembly,pushra);
    strcat(assembly,pushstuff);
    strcat(assembly,pushlcl);
    strcat(assembly,pushstuff);
    strcat(assembly,pusharg);
    strcat(assembly,pushstuff);
    strcat(assembly,pushthis);
    strcat(assembly,pushstuff);
    strcat(assembly,pushthat);
    strcat(assembly,pushstuff);
    char* arg = "@5\nD=A\n@";
    strcat(assembly,arg);
    strcat(assembly,numVar);
    char* arg2 = "\nD=A-D\n@SP\nA=M\nD=A-D\n@ARG\nM=D\n";
    strcat(assembly,arg2);
    char* lcl = "@SP\nD=M\n@LCL\nM=D\n@";
    strcat(assembly,lcl);
    strcat(assembly,name);
    char* end = "\n0;JMP\n";
    strcat(assembly,end);
    return assembly;
}
char* terminate(){
    char* assembly = malloc((sizeof(char))*1000);
    char* start = "@LCL\nD=M\n@14\nM=D\n@5\nA=D-A\nA=M\nD=M\n@15\nM=D\n";
    char* argpop = pop("argument","0");
    strcpy(assembly,start);
    strcat(assembly,argpop);
    char* end = "\n@ARG\nM=D\n@SP\nM=D+1\n@14\nM=D\n@1\nA=D-A\nA=M\nD=M\n@THAT\nM=D\n@14\nM=D\n@2\nA=D-A\nA=M\nD=M\n@THIS\nM=D\n@14\nM=D\n@3\nA=D-A\nA=M\nD=M\n@ARG\nM=D\n@14\nM=D\n@4\nA=D-A\nA=M\nD=M\n@LCL\nM=D\n@15\n0;JMP\n";
    strcat(assembly,end);
    return assembly;
}
char* functionCommands(char* command, char* name, char* numVar){
    if (strcmp(command,"function") == 0){
        return function(name,numVar);
    }
    else if (strcmp(command,"call") == 0){
        return call(name,numVar);
    }
    else if (strcmp(command,"return") == 0){
        return terminate();
    }
    else{
        return "ERROR\n";
    }
}
int counter(){
    static int count = 0;
    count++;
    return count;
}
char* intToString(int num){
    char* buffer = malloc(sizeof(char)*sizeof(int)*4+1);
    if(buffer){
        sprintf(buffer,"%d", num);
    }
    return buffer;
}
int charToInt(char digit){
    return digit - '0';
}
int stringToInt(char* string){
    int len = strlen(string);
    int sum = 0;
    int power = 0;

    int i;
    for(i=len-1; i>=0; i--){
        sum = sum + (charToInt(string[i])*pow(10,power++));
    }
    return sum;
}
