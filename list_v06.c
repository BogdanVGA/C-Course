
/*** create double linked list with option to add or eliminate a node ***/

#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *previous;
    struct node *next;
};

struct node *createList(struct node *ptr, int nodeCount);
struct node *insertNode(struct node *ptrHead, struct node *ptrLast, int nodeCount);
struct node *removeNode(struct node *ptrHead, struct node *ptrLast);

void printList(struct node *ptr);
void freeList(struct node *ptr);

int main() {

    struct node *head = NULL;
    struct node *last = NULL;    

    int nodeCount = 0;
    int choice = 0;


    printf("Enter number of nodes: ");
    scanf("%d", &nodeCount);

    if(nodeCount <= 0) {
        printf("Invalid number of nodes...\n");
        return 1;
    }

    head = (struct node *)malloc(sizeof(struct node));
    if(head == NULL) {
        printf("Unable to allocate memory...\n");
        return 1;
    }
    printf("Value #1: ");
    scanf("%d", &(head->value));
    head->previous = NULL;
    head->next = NULL;
    /* create list and set "last" ptr at the end of it */
    last = createList(head, nodeCount);

    printList(head);

    printf("Insert (1) or remove (2) node: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1: {
            head = insertNode(head, last, nodeCount);
            /* if end of list was modified, reset "last" ptr at the end of it */
            last = head;
            while(last->next) {
                last = last->next;
            }            
            break;
        }
        case 2: {
            head = removeNode(head, last);
            /* if end of list was modified, reset "last" ptr at the end of it */
            last = head;
            while(last->next) {
                last = last->next;
            }
            break;
        }
        default: {
            printf("Invalid option...\n");
            freeList(head);
            return 1;
        }        
    }    

    printList(head);

    freeList(head);

    return 0;
}

/* create list */
struct node *createList(struct node *ptr, int nodeCount) {

    struct node *current = NULL;
    current = ptr;

    for(int i=1; i<nodeCount; i++) {
        current->next = (struct node *)malloc(sizeof(struct node));
        if(current->next == NULL) {
            printf("Unable to allocate memory...\n");
            return NULL;
        }
        current->next->previous = current;
        current = current->next;

        printf("Value #%d: ", i+1);
        scanf("%d", &(current->value));
        current->next = NULL;
    }

    return current;    
}
/* print elements in the list */
void printList(struct node *ptr) {

    printf("The list is: ");
    struct node *current = NULL;
    current = ptr;

    while(current) {
        printf("%d", current->value);
        current = current->next;
        (current != NULL) ? printf(" -> ") : printf("\n");
    }    
}
/* inserts a node, returns a pointer to the head of the modified list */
struct node *insertNode(struct node *ptrHead, struct node *ptrLast, int nodeCount) {

    struct node *current = NULL;
    struct node *temp = NULL;

    int insertValue = 0;
    int insertPos = 0;
    int count = 1;
    int modified = 0;

    /* ask user to enter a value to insert and where to insert it */
    printf("Value of the node to insert: ");
    scanf("%d", &insertValue);
    printf("Insert position in the list: ");
    scanf("%d", &insertPos);

    if((insertPos < 1) || (insertPos > nodeCount)) {
        printf("Invalid position! List was not modified...\n");
        return ptrHead;
    }
    
    current = ptrHead;
    temp = ptrHead;

    /* check if the node to be inserted is at the head of the list */
    if(insertPos == 1) {        
        printf("Moving head of the list...\n");
        ptrHead->previous = (struct node *)malloc(sizeof(struct node));
        temp = ptrHead;
        ptrHead = ptrHead->previous;
        ptrHead->next = temp;
        ptrHead->value = insertValue;       
        ptrHead->previous = NULL;
        temp = NULL;
        modified = 1;                        
    }

    /* check if the node to be inserted is at the end of the list */
    if(insertPos == nodeCount) {
        printf("Moving end of the list...\n");
        ptrLast->next = (struct node *)malloc(sizeof(struct node));
        temp = ptrLast;               
        ptrLast = ptrLast->next;
        ptrLast->previous = temp;
        ptrLast->value = insertValue;
        ptrLast->next = NULL;
        temp = NULL;
        modified = 1;                       
    }    

    /* position "current" ptr to point to the node before the new node */
    while((current != NULL) && (count <= nodeCount) && (modified == 0)) {        
        if(count == (insertPos - 1)) {            
            printf("Inserting...\n");            
            temp = (struct node *)malloc(sizeof(struct node));           
            current->next->previous = temp;
            temp->next = current->next;
            current->next = temp;
            temp->previous = current;
            temp->value = insertValue;
            temp = NULL;
            modified = 1;
        }        
        current = current->next;
        count++;
    }

    return ptrHead;    
}
/* removes a node, returns a pointer to the head of the modified list */
struct node *removeNode(struct node *ptrHead, struct node *ptrLast) {

    struct node *current = NULL;
    int removeValue = 0;
    int found = 0;

    printf("Value of the node to remove: ");
    scanf("%d", &removeValue);

    /* set "current" ptr to point to the same memory location as "head" ptr */
    current = ptrHead;    

    /* check if the node to be removed is at the head of the list */
    if(ptrHead->value == removeValue) {
        found = 1;
        printf("Moving head of the list...\n");        
        ptrHead = ptrHead->next;
        free(ptrHead->previous);
        ptrHead->previous = NULL;                       
    }

    /* check if the node to be removed is at the end of the list */
    if(ptrLast->value == removeValue) {        
        found = 1;
        printf("Moving end of the list...\n");        
        ptrLast = ptrLast->previous;
        free(ptrLast->next);
        ptrLast->next = NULL;                       
    }    

    /* position "current" ptr to point to the node before the node to be removed */
    while(current->next) {        
        if(current->next->value == removeValue) {
            found = 1;
            printf("Removing...\n");
            current->next->next->previous = current;
            free(current->next);           
            current->next = current->next->next;                                                          
            break;
        }        
        current = current->next;
    }    

    /* check if removeValue is found in the list */
    if(found == 0) {
        printf("Value to remove not found! List was not modified...\n");        
    }

    return ptrHead;
}
/* release used memory */
void freeList(struct node *ptr) {

    struct node *current = NULL;
    current = ptr;

    while(ptr) {
        current = ptr;
        ptr = ptr->next;
        free(current);
    }
}