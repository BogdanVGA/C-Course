#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TYPE_COUNT 8
#define KWORD_COUNT 4

const char *keyword[16] = { "static", "const", "extern", "volatile"};
const char *type[16] = {"int", "double", "float", "short", "unsigned", "long", "char", "void"};

struct declaration {
    char keyword[32];
    char leftId[32];
    char type[32];
    char extType[32];
    char arrDim[32];
    char funcArgs[32];
    char **args;
    int *argIsPtr;
    int isPtr;
    int isFunc;
    int argCount;
    int isArray;
    int keywordFound;
    int extTypeFound;   
};

char *readBlock(char *ptr, char *destination, char *delim);

int checkLong(char *tempStr, char *current, char *destType, char *destExtType);

int checkPtr(char *string, char *star);

char *getLeftId(char *ptr, char *string, char *delim1, char *delim2);

int checkIsFunc(char *destination, char *ptr);

int checkIsArray(char *destination, char *ptr);

void cleanIdentifier(char *string);

int getArgCount(char *string);

char **separateArgs(char *string, int count);

int checkError(struct declaration *declarationPtr, char *string);

void printDeclaration(struct declaration *declarationPtr);

int main() {        

    char buffer[64];
    char temp[32];            

    char *cursor = NULL;      

    struct declaration decl = {
        .argCount = 1,
        .isPtr = 0,
        .isFunc = 0,
        .isArray = 0,
        .keywordFound = 0,
        .extTypeFound = 0        
    };       

    memset(decl.leftId, '\0', 32);
    memset(decl.type, '\0', 32);
    memset(decl.extType, '\0', 32);
    memset(decl.funcArgs, '\0', 32);
    memset(decl.keyword, '\0', 32);
    memset(decl.arrDim, '\0', 32);

    memset(temp, '\0', 32);

    struct declaration *declPtr = &decl;

    #ifdef DEBUG
    printf("***** DEBUG mode on *****\n");

    FILE *fptr = fopen("debug_info.txt", "a+");

    if(NULL == fptr) {
        printf("Unable to open file...\n");
        return 1;
    }
    #endif

    printf("Enter a declaration: ");
    fgets(buffer, 63, stdin);
    buffer[strcspn(buffer, "\n")] = 0;   
    
    /* set cursor at begining of string */
    cursor = &buffer[0];

    /* get first block of characters, from begining to first space, put it in temp */
    cursor = readBlock(cursor, temp, " ");

    /* check if the first block of characters forms one of the keywords */
    for(int i=0; i<KWORD_COUNT; i++) {
        if(strcmp(temp, keyword[i]) == 0) {
            strcpy(decl.keyword, temp);
            decl.keywordFound = 1;
        }        
    }

    /* if it's not a keyword, then it's a data type */
    if(decl.keywordFound == 0) {
        /* check if it's a two word data type, e.g. long int */
        decl.extTypeFound = checkLong(temp, cursor, decl.type, decl.extType);
        /* refresh cursor positon */
        if(decl.extTypeFound == 1) {
            /* move cursor across the space */
            cursor++;
            cursor = readBlock(cursor, temp, " ");            
        }                        
    }

    memset(temp, '\0', 32);

    /* if it was a keyword, read the second block of chars as data type */
    if(decl.keywordFound == 1) {
        /* move cursor across the space */
        cursor++;
        cursor = readBlock(cursor, temp, " ");        

        /* check if it's a two word data type, e.g. long int */
        decl.extTypeFound = checkLong(temp, cursor, decl.type, decl.extType);
        /* refresh cursor position */
        if(decl.extTypeFound == 1) {
            /* move cursor across the space */
            cursor++;
            cursor = readBlock(cursor, temp, " ");            
        }        
    }

    memset(temp, '\0', 32);

    /* move cursor across the space to get to the identifier */
    cursor++;    

    /* read remaining characters until the end of the string */
    cursor = readBlock(cursor, temp, "\0");

    /* takes identifier from temp, moves it to decl.leftId */
    cursor = getLeftId(temp, decl.leftId, "(", "[");

    /* if '*' char is found in identifier, returns decl.isPtr = 1 or 3 if it finds it more than once */
    decl.isPtr = checkPtr(decl.leftId, "*");

    /* if array, returns decl.isArray = 1 */
    decl.isArray = checkIsArray(decl.arrDim, cursor);

    /* if function, returns decl.isFunc = 1 */
    decl.isFunc = checkIsFunc(decl.funcArgs, cursor);

    if(decl.isFunc == 1) {
        /* counts the number of arguments */
        decl.argCount = getArgCount(decl.funcArgs);

        /* separates arguments, returns a pointer to where the arguments are stored */
        decl.args = separateArgs(decl.funcArgs, decl.argCount);
        if(NULL == decl.args) {
            printf("Unable to allocate memory...\n");
            return 1;
        }

        /* initialize the array of argument pointer flags to zero */
        decl.argIsPtr = (int *)malloc(decl.argCount * sizeof(int));
        if(NULL == decl.argIsPtr) {
            printf("Argument pointer flags: unable to allocate memory...\n");
            return 1;
        }
        for(int i=0; i<decl.argCount; i++) {
            decl.argIsPtr[i] = 0;
        }        

        /* check if the '*' char is attached to arg type */
        for(int i=0; i<decl.argCount; i++) {
            cursor = decl.args[i];
            while(*cursor) {                
                if((*cursor) == '*') {
                    // eliminate the '*' character               
                    (*cursor) = '\0';
                    // flag the argument as pointer
                    decl.argIsPtr[i] = 1;
                    break;
                }
                cursor++;
            }
        }        
    }

    /* if identifier needs to be dereferenced first, updates decl.isPtr to 2 */
    if((decl.leftId[0] == '(') && (decl.leftId[strlen(decl.leftId) - 1] == ')')) {
        decl.isPtr = 2;                              
    }

    /* eliminates residual characters from identifier name */
    cleanIdentifier(decl.leftId);

    /* reset cursor */    
    cursor = &buffer[0];

    printf("\nYou entered: %s\n", buffer);

    #ifdef DEBUG    
    printf("\nPrinting debug info to file...");
    fprintf(fptr, "\ncursor is: |%s|\n", cursor);
    fprintf(fptr, "temp is: |%s|\n", temp);
    fprintf(fptr, "decl.keyword is: |%s|\n", decl.keyword);
    fprintf(fptr, "decl.extType is: |%s|\n", decl.extType);
    fprintf(fptr, "decl.type is: |%s|\n", decl.type);
    fprintf(fptr, "decl.leftId is: |%s|\n", decl.leftId);
    fprintf(fptr, "decl.funcArgs is: |%s|\n", decl.funcArgs);
    fprintf(fptr, "decl.arrDim is: |%s|\n", decl.arrDim);
    fprintf(fptr, "decl.isPtr is: %d\n", decl.isPtr);
    fprintf(fptr, "decl.isFunc is: %d\n", decl.isFunc);
    fprintf(fptr, "decl.argCount is: %d\n", decl.argCount);
    fprintf(fptr, "decl.isArray is: %d\n\n", decl.isArray);

    if(decl.isFunc == 1) {
        for(int i=0; i<decl.argCount; i++) {
            fprintf(fptr, "decl.args[%d] is: |%s|\n", i, decl.args[i]);
        }

        for(int i=0; i<decl.argCount; i++) {
            fprintf(fptr, "decl.argIsPtr[%d] is: %d\n", i, decl.argIsPtr[i]);
        }
    }

    printf("done!\nDebug file is located in current directory...\n");
    fclose(fptr);
    #endif

    /* if an error is found, the checkError function returns 1 */
    if( 0 != checkError(declPtr, buffer)) {
        printf("Invalid declaration...\n");
        return 1;
    }

    /* print result after parsing the declaration */
    printDeclaration(declPtr);    

   /* clean up used memory */
    if(decl.isFunc == 1) {
        for(int i=0; i<decl.argCount; i++) {
            free(decl.args[i]);
        }
        free(decl.args);
        free(decl.argIsPtr);
    }
    return 0;
}

/* read a block of chars until a "delim" char is found, returns a pointer at the end of the block it just read */
char *readBlock(char *ptr, char *destination, char *delim){

    int charCount = 0;        
    while((*ptr) != (*delim)) {
        destination[charCount] = *ptr;
        charCount++;
        ptr++;
        /* if ptr reached the end of the string without finding delimiter */
        if((*ptr) == '\0') break;
    }
    return ptr;
}

/* check if it's a two word data type, e.g. long int */
int checkLong(char *tempStr, char *current, char *destType, char *destExtType) {
    
    int extTypeFound = 0;
    
    if(strcmp(tempStr, "long") == 0) {
        /* assume it's a two word data type */
        strcpy(destExtType, tempStr);        
        memset(tempStr, '\0', 32);
        /* move cursor across the space and read next block of chars */
        current++;
        current = readBlock(current, tempStr, " ");
        /* check if the next char block is also a data type */
        for(int i=0; i<TYPE_COUNT; i++) {
            if(strcmp(tempStr, type[i]) == 0) {
                strcpy(destType, tempStr);
                extTypeFound = 1;
            }
        }
        /* if not, then it was a one word data type */
        if(extTypeFound == 0) {
            strcpy(destType, destExtType);            
        }
    }
    else if(strcmp(tempStr, "long") != 0) {
        strcpy(destType, tempStr);
    }
    
    return extTypeFound;
}

/* check if a "star" character is contained within the string */
int checkPtr(char *string, char *star) {

    int isPtr = 0;
    int multiple = 0;
    while(*string) {
        if((*star) == (*string) ) {            
            isPtr = 1;
            multiple++;
        }
        string++;
        if((*string) == '\0') break;        
    }
    /* rudimentary multiple indirection detector */
    if(multiple > 1) return 3;   
    return isPtr;
}

/* gets identifier */
char *getLeftId(char *temp, char *string, char *delim1, char *delim2) {

    int charCount = 0;
    char *current = &temp[0];    
    while( *current ) {
        string[charCount] = temp[charCount];
        charCount++;
        current++;
        if((*current) == *delim1 || (*current) == *delim2) break;
    }
    return current;
}

/* if identifier name is followed by '(', then update its status as a function */
int checkIsFunc(char *destination, char *ptr) {

    int isFunc = 0;
    int charCount = 0;
    while(*ptr) {
        if((*ptr) == '(') {
            isFunc = 1;
            ptr++;
            /* get function arguments placed between ( and ) */
            while((*ptr) != ')') {
                destination[charCount] = *ptr;                
                charCount++;
                ptr++;
                /* if ptr reached the end of the string without finding ")" */
                if((*ptr) == '\0') break;               
            }
        }        
        ptr++;            
    }
    return isFunc;
}

/* if identifier name is followed by '[', then update its status as an array */
int checkIsArray(char *destination, char *ptr) {

    int isArray = 0;
    int charCount = 0;
    while(*ptr) {
        if((*ptr) == '[') {
            isArray = 1;
            ptr++;
            /* get the dimension of the array between [ and ] */
            while((*ptr) != ']') {
                destination[charCount] = *ptr;                
                charCount++;
                ptr++;
                /* if ptr reached the end of the string without finding "]" */
                if((*ptr) == '\0') break;               
            }
        }        
        ptr++;            
    }
    return isArray;
}

/* eliminates residual chars from identifier name */
void cleanIdentifier(char *string) {

    int charCount = 0;
    char *strPtr = NULL;
    char temp[32];

    memset(temp, '\0', 32);
    strPtr = &string[0];        
    while(*strPtr) {        
        if(((*strPtr) == '*') || ((*strPtr) == '(') || ((*strPtr) == ')')) {
            strPtr++;
            continue;
        }
        temp[charCount] = *strPtr;
        charCount++;
        strPtr++;
    }
    strcpy(string, temp);
}

/* gets number of arguments */
int getArgCount(char *string) {

    int count = 1;
    char *current = &string[0];

    /* get argument count by counting the commas between them */
    while(*current) {
        if((*current) == ',') {
            count++;
        }
        current++;
    }
    return count;
}

/* separates function arguments */
char **separateArgs(char *string, int count) {

    char *current = NULL;
    char tempStr[32];
    int index = 0;
    int charCount = 0;

    /* allocate memory for pointers to each argument */
    char **argPtr = (char **)malloc(count * sizeof(char*));
    if(NULL == argPtr) {
        printf("ERRARG01: ");
        return NULL;
    }

    /* allocate memory for each argument */
    for(int i=0; i<count; i++) {
        argPtr[i] = (char *)malloc(32 * sizeof(char));
        if(NULL == argPtr[i]) {
            printf("ERRARG02: ");
            return NULL;
        }
    }
    
    /* separates arguments, using the comma char as a delimiter */
    current = strtok(string, ",");
    while (current != NULL) {
        strcpy(argPtr[index], current);
        index++;
        current = strtok(NULL, ",");
    }

    /* remove spaces from arguments, e.g "char *" -> "char*" */
        for(int i=0; i<count; i++) {
            memset(tempStr, '\0', 32);
            charCount = 0;
            current = argPtr[i];
            while((*current)) {                
                if((*current) == ' ') {
                    current++;
                    continue;
                }
                tempStr[charCount] = *current;
                charCount++;
                current++;
            }
            strcpy(argPtr[i], tempStr);
        }
    return argPtr;
}

/* checks elements of the struct for errors */
int checkError(struct declaration *declarationPtr, char *string) {

    int count = 0;
    int brCount = 0;
    char *current = NULL;
    /* validate type */
    for(int i=0; i<TYPE_COUNT; i++) {
        if(strcmp(declarationPtr->type, type[i]) == 0) {            
            count++;
        }
    }
    if(count != 1) {
        printf("\nERR01: ");                
        return 1;
    }

    /* ckeck for presence of identifier */
    if(strlen(declarationPtr->leftId) == 0) {
        printf("\nERR02: ");
        return 1;
    }        
    
    if(declarationPtr->isFunc == 1) {
        /* check for presence of arguments */
        if(strlen(declarationPtr->funcArgs) == 0) {
            printf("\nERR03: ");
            return 1;
        }
        /* check if arguments are valid data types */
        count = 0;        
        for(int i=0; i<declarationPtr->argCount; i++) {            
            for(int j=0; j<TYPE_COUNT; j++) {                                
                if(strcmp(declarationPtr->args[i], type[j]) == 0) {
                count++;                
                }           
            }           
        }        
        if(count != declarationPtr->argCount) {            
            printf("\nERR04: ");                
            return 1;
        }                
    }
    /* validate array dimension */
    if(declarationPtr->isArray == 1) {
        if(0 == atoi(declarationPtr->arrDim)) {
        printf("\nERR05: ");
        return 1;
        }
    }
    /* check if first character of identifier is a letter or "_" */
    if(0 == isalpha(declarationPtr->leftId[0]) && declarationPtr->leftId[0] != '_') {
        printf("\nERR06: ");
        return 1;
    }
    /* check if identifier is a data type */
    count = 0;
    for(int i=0; i<TYPE_COUNT; i++) {
        if(strcmp(declarationPtr->leftId, type[i]) == 0) {            
            count++;
        }
    }
    if(count != 0) {
        printf("\nERR07: ");                
        return 1;
    }
    /* check if open ( or [ have matching ) or ] */
    count = 0;
    brCount = 0;
    current = &string[0];

    while(*current) {
        if((*current) == '(') {
            count++;            
        }
        else if((*current) == ')') {
            count--;            
        }
        else if((*current) == '[') {
            brCount++;            
        }
        else if((*current) == ']') {
            brCount--;            
        }
        current++;
    }    
    if(count != 0 || brCount != 0) {
        printf("\nERR08: ");
        return 1;
    }

    if(declarationPtr->isPtr == 3) {
        printf("\nMultiple indirection not supported. ");
        return 1;
    }

    return 0;
}

/* prints elements of the declaration */
void printDeclaration(struct declaration *declarationPtr) {

    printf("\nDeclare %s as ", declarationPtr->leftId);

    if(declarationPtr->keywordFound == 1) {
        printf("%s ", declarationPtr->keyword);
    }

    if(declarationPtr->isPtr == 2) {
        printf("pointer to ");
    }

    if(declarationPtr->isFunc == 1) {
        printf("function taking");
        for(int i=0; i<declarationPtr->argCount; i++) {
            if(declarationPtr->argIsPtr[i] == 1) {
            printf(" pointer to %s", declarationPtr->args[i]);            
            }        
            else {
                printf(" %s", declarationPtr->args[i]);
            }
            (i == (declarationPtr->argCount - 1)) ? printf("") : printf(",");           
        }
        printf(" returning ");        
    }    

    if(declarationPtr->isArray == 1) {
        printf("array %d of ", atoi(declarationPtr->arrDim));
    }

    if(declarationPtr->isPtr == 1) {
        printf("pointer to ");               
    }

    if(declarationPtr->extTypeFound == 1) {
        printf("%s %s\n", declarationPtr->extType, declarationPtr->type);
    }
    else if(declarationPtr->extTypeFound == 0) {
        printf("%s\n", declarationPtr->type);
    }
}