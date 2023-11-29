#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CLASS_COUNT 4
#define QUAL_COUNT 2
#define MOD_COUNT 4
#define TYPE_COUNT 5
#define MAX_BUFFER 64
#define MAX_TOKEN_COUNT 50
#define MAX_TOKEN_LENGTH 16

const char *class[16] = { "static", "extern", "auto", "register"};
const char *qualifier[16] = { "const", "volatile" };
const char *modifier[16] = { "short", "long", "signed", "unsigned"};
const char *type[16] = {"int", "double", "float", "char", "void"};

struct token {    
    char type;
    char string[MAX_TOKEN_LENGTH];
};

int top = -1;
int identPos = MAX_TOKEN_COUNT;

struct token stack[MAX_TOKEN_COUNT];
struct token current;

#define push(s) stack[++top] = s
#define pop stack[top--]
#define peek(s) stack[top-s]

/* sort tokens and classify them as storage class, qualifier, modifier, type, identifier */
char sortToken(char *string) {               
    
    for(int i=0; i<CLASS_COUNT; i++) {
        if(strcmp(string, class[i]) == 0) {
            return 'c';            
        }
    }
    for(int i=0; i<MOD_COUNT; i++) {
        if(strcmp(string, modifier[i]) == 0) {
            return 'm';            
        }
    }
    for(int i=0; i<TYPE_COUNT; i++) {
        if(strcmp(string, type[i]) == 0) {
            return 't';            
        }
    }
    for(int i=0; i<QUAL_COUNT; i++) {
        if(strcmp(string, qualifier[i]) == 0) {
            return 'q';            
        }
    }      
    if(atoi(string)) {
        return 'd';
    }    
    return 'i';          
}

/* read tokens, classify them, put them in stack */
int getTokens(char *buffer) {

    int count = 0;       
    char *cursor = buffer;           

    current.type = '\0';
    memset(current.string, '\0', MAX_TOKEN_LENGTH);

    while(*cursor) {
        while(isalnum(*cursor)) {        
            current.string[count] = *cursor;
            count++;                                                             
            cursor++;        
        }        
        if( strlen(current.string) > 0 ) {
            current.type = sortToken(current.string);          
            stack[++top] = current;            
            memset(current.string, '\0', MAX_TOKEN_LENGTH);
            count = 0;
        }                
        /* get identifier position in stack */        
        if(stack[top].type == 'i') {
            identPos = top;            
        }               
        if(*cursor == '*') {                                    
            strcpy(current.string, "pointer to");
            current.type = '*';
            stack[++top] = current;                        
        }        
        if(*cursor == ',') {                                   
            strcpy(current.string, ", ");
            current.type = ',';
            stack[++top] = current;                                                            
        }
        if(*cursor == '(') {                                   
            strcpy(current.string, "(");
            current.type = '(';
            stack[++top] = current;                
        }
        if(*cursor == '[') {                            
            strcpy(current.string, "[");
            current.type = '[';
            stack[++top] = current;            
        }
        if(*cursor == ')') {                                    
            strcpy(current.string, ")");
            current.type = ')';
            stack[++top] = current;           
        }
        if(*cursor == ']') {                                   
            strcpy(current.string, "]");
            current.type = ']';
            stack[++top] = current;                                    
        }
        memset(current.string, '\0', MAX_TOKEN_LENGTH);
        cursor++;
    }
    return 0;
}

/* check declaration for possible errors */
int checkForError() {

    int index = 0;
    int paraCount = 0, brCount = 0, idCount = 0, typeCount = 0, modCount = 0, classCount = 0;
    int argMonitor = 0, argModCount = 0, argCount = 0;

    while(index <= top) {        
        /* count identifiers found */
        if(stack[index].type == 'i') {
            idCount++;            
        }
        /* count types found */
        if(stack[index].type == 't') {
            typeCount++;
        }
        /* get number of storage class identifiers */        
        if(stack[index].type == 'c') {
            classCount++;
            if(classCount > 1) {
                printf("ERR01 ");
                return 1;
            }            
        }
        /* check if qualifier has a storage class in front */
        if(stack[index].type == 'q' && stack[index-1].type == 'c') {
            printf("ERR02 ");
            return 1;            
        }
        /* mark types after identifier as arguments of functions */                
        if(stack[index].type == 't' && index > identPos) {
            stack[index].type = 'a';                        
            typeCount--;
            argCount++;
            argMonitor++;            
        }
        /* get number of modifiers */
        if(stack[index].type == 'm' && index < identPos) {
            modCount++;            
        }
        /* get number of argument modifiers */
        if(stack[index].type == 'm' && index > identPos) {
            argModCount++;            
        }
        /* ckeck if modifier is positioned before type */
        if(stack[index].type == 'm' && stack[index-1].type == 't') {
            printf("ERR03 ");
            return 1;            
        }
        /* ckeck if modifier is positioned before type for arguments */
        if(stack[index].type == 'm' && stack[index-1].type == 'a') {
            printf("ERR04 ");
            return 1;            
        }
        /* argMonitor is used to monitor no. of args in relation to commas */
        if(stack[index].type == ',') {          
            argMonitor--;                                                
        }
        if(stack[index].type == '(') {   
            paraCount++;                        
        }
        if(stack[index].type == '[') {              
            brCount++;                        
        }
        if(stack[index].type == ')') {                                    
            paraCount--;                                    
        }
        if(stack[index].type == ']') {
            brCount--;                                   
            /* check if array dimension is valid */
            if( atoi(stack[index-1].string) == 0 && (stack[index-1].type != '[') ) {
                printf("ERR05 ");
                return 1;
            }
        }
        index++;            
    }

    #ifdef DEBUG
    printf("paraCount is: %d\n", paraCount);
    printf("brCount is: %d\n", brCount);
    printf("idCount is: %d\n", idCount);
    printf("!typeCount is: %d\n", !typeCount);
    printf("argMonitor is: %d\n", argMonitor);
    printf("argModCount is: %d\n", argModCount);
    printf("modCount is: %d\n", modCount);
    
    printf("identPos is: %d\n", identPos);            
    for(int i=0; i <= top; i++) {        
        printf("element #%2d: ", i);        
        printf("|%c|", stack[i].type);
        printf("|%s|\n", stack[i].string);
    }    
    #endif

    /* if unmatching paras or brackets or more than 1 identifier is found or no type is found
    or no. of arguments and no. of commas don't match, an error is generated */
    if(paraCount || brCount || (idCount != 1) || !typeCount || (argMonitor > 1) ) {        
        printf("ERR06 ");
        return 1;
    }
    /* if more than two modifiers are found or more than two modifiers per argument are found, an error is generated */
    if( (modCount > 2) || (argModCount > (2*argCount)) ) {
        printf("ERR07 ");
        return 1;
    }

    return 0;
}

/* translate function declaration and print function arguments */
int ifFunction(int slider) {

    int mark = 0;       
    
    printf("function (");        
    while(stack[slider].type != ')') {
        slider++;        
        if(stack[slider].type == ',' || stack[slider].type == ')') {                        
            if(stack[slider-1].type == '(') printf("void");
            if(stack[slider-1].type == '*') printf("%s ", stack[slider-1].string);
            if(stack[slider-1].type == 'a') printf("%s", stack[slider-1].string);
            mark = slider;
            mark--;
            while(stack[mark].type != '(' && stack[mark].type != ',') {
                mark--;                
            }
            mark++;
            while( mark < (slider - 1) ) {
                if(stack[mark].type == 'a') {
                    printf("%s", stack[mark].string);                                        
                }
                else {
                    printf("%s ", stack[mark].string);
                }                
                mark++;
            }                                                       
            printf("%s", stack[slider].string);                      
        }               
        if(stack[slider].type == ')') printf(" returning ");                
    }
    return slider;
}

/* translate array declaration */
int ifArray(int slider) {

    printf("array ");
    slider++;
    if(stack[slider].type == ']') {        
        printf("of ");
        return slider;
    }
    while(stack[slider].type != ']') {
        printf("%s", stack[slider].string);
        slider++;
        if(stack[slider].type == ']') printf(" of ");
    }
    return slider;
}

void printDeclaration() {

    int slideLeft = 0;    
    int slideRight = 0;
    
    /* print identifier */
    printf("\nDeclare %s as ", stack[identPos].string);

    /* print storage class */
    slideRight = 0;
    while(slideRight < identPos) {
        if(stack[slideRight].type == 'c') {
            printf("%s ", stack[slideRight].string);
        }        
        slideRight++;
    }
    /* starting from identifier... */
    slideRight = identPos;
    slideLeft = identPos;

    while(slideRight <= top) {
        /* ...translate identifier surroundings and...  */
        if(stack[slideRight].type == ')') {
            slideLeft--;
            while(stack[slideLeft].type != '(') {
                printf("%s ", stack[slideLeft].string);
                slideLeft--;
            }
        }
        /* ...if found, translate function, return slideRight at the end of the function */
        if(stack[slideRight].type == '(') {
            slideRight = ifFunction(slideRight);                        
        }
        /* ... if found, translate array, return slideRight at the end of the array */
        if(stack[slideRight].type == '[') {
            slideRight = ifArray(slideRight);
        }
        slideRight++;        
    }
    /* check if there is anything left near identifier or outside paranthesis, do not print types */
    slideLeft--;            
    while(slideLeft >= 0) {        
        if(stack[slideLeft].type == 't') break;
        printf("%s ", stack[slideLeft].string);
        slideLeft--;            
    }                
    
    /* print types, qualifiers and modifiers not printed before */
    slideRight = 0;    
    while(slideRight <= slideLeft) {
        if(stack[slideRight].type == 'c') {
            slideRight++;
        }
        if(stack[slideRight].type == 'q') {
            printf("%s ", stack[slideRight].string);
        }
        if(stack[slideRight].type == 'm' || stack[slideRight].type == 't') {
            printf("%s ", stack[slideRight].string);
        }        
        slideRight++;        
    }
    printf("\n\n");    
}

int main() {

    int checkErr = 0;

    char buffer[MAX_BUFFER];
    memset(buffer, '\0', MAX_BUFFER);    

    printf("Enter a declaration: ");
    fgets(buffer, MAX_BUFFER, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    printf("You entered: %s\n", buffer);        

    getTokens(buffer);

    checkErr = checkForError();
    if(checkErr) {
        printf("Invalid declaration...\n");
        return 1;
    }   

    printDeclaration();     

    return 0;
}