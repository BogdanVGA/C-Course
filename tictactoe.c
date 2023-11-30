#include <stdio.h>
#include <stdlib.h>

void drawBoard (char arr[10])
{    
    printf("\t Tic Tac Toe\n");
    printf("\nPlayer One (X) Player Two (O)\n\n");
    printf("\t     |     |     \n");
    printf("\t  %c  |  %c  |  %c  \n", arr[1], arr[2], arr[3]);
    printf("\t_____|_____|_____\n");
    printf("\t     |     |     \n");
    printf("\t  %c  |  %c  |  %c  \n", arr[4], arr[5], arr[6]);
    printf("\t_____|_____|_____\n");
    printf("\t     |     |     \n");
    printf("\t  %c  |  %c  |  %c  \n", arr[7], arr[8], arr[9]);
    printf("\t     |     |     \n");    
}

void markBoardOne (int i, char arr[10])
{    
    while(arr[i]=='O' || arr[i]=='X' || i<1 || i>9)
    {
        printf("The field is already taken or invalid selection!\n");
        printf("Enter a different number: ");
        scanf("%d", &i);                                                
    }     

    arr[i]='X';           
}

void markBoardTwo (int j, char arr[10])
{    
    while(arr[j]=='X' || arr[j]=='O' || j<1 || j>9)
    {
        printf("The field is already taken or invalid selection!\n");
        printf("Enter a different number: ");
        scanf("%d", &j);                                        
    }

    arr[j]='O';        
}

int checkWin (char arr[10])
{
    int win = 0;

    if(arr[1]=='X'&& arr[2]=='X' && arr[3]=='X') win = 1;
    if(arr[1]=='O'&& arr[2]=='O' && arr[3]=='O') win = 2;

    if(arr[4]=='X'&& arr[5]=='X' && arr[6]=='X') win = 1;
    if(arr[4]=='O'&& arr[5]=='O' && arr[6]=='O') win = 2;

    if(arr[7]=='X'&& arr[8]=='X' && arr[9]=='X') win = 1;
    if(arr[7]=='O'&& arr[8]=='O' && arr[9]=='O') win = 2;

    if(arr[1]=='X'&& arr[4]=='X' && arr[7]=='X') win = 1;
    if(arr[1]=='O'&& arr[4]=='O' && arr[7]=='O') win = 2;

    if(arr[2]=='X'&& arr[5]=='X' && arr[8]=='X') win = 1;
    if(arr[2]=='O'&& arr[5]=='O' && arr[8]=='O') win = 2;

    if(arr[3]=='X'&& arr[6]=='X' && arr[9]=='X') win = 1;
    if(arr[3]=='O'&& arr[6]=='O' && arr[9]=='O') win = 2;

    if(arr[1]=='X'&& arr[5]=='X' && arr[9]=='X') win = 1;
    if(arr[1]=='O'&& arr[5]=='O' && arr[9]=='O') win = 2;

    if(arr[7]=='X'&& arr[5]=='X' && arr[3]=='X') win = 1;
    if(arr[7]=='O'&& arr[5]=='O' && arr[3]=='O') win = 2;

    return win;
}

int main ()
{
    int playOne = 0;
    int playTwo = 0;    
    int count = 0;
    int chWin = 0;

    char field[10]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    system("clear");
    drawBoard(field);

    while (count<5)
    {
        printf("\nPlayer 1, enter a number: ");
        scanf("%d", &playOne);

        while (playOne < 1 || playOne > 9)
        {
            printf("\nInvalid selection, enter a number: ");
            scanf("%d", &playOne);
        }       

        markBoardOne(playOne, field);        
        system("clear");
        drawBoard(field);
        
        chWin = checkWin(field);
        if(chWin==1)
        {
            printf("\n==> Player 1 wins!\n\n");
            break;
        }   

        count++;
        if(count==5) break;       
        
        printf("\nPlayer 2, enter a number: ");
        scanf("%d", &playTwo);

        while (playTwo < 1 || playTwo > 9)
        {
            printf("\nInvalid selection, enter a number: ");
            scanf("%d", &playTwo);
        }

        markBoardTwo(playTwo, field);
        system("clear");
        drawBoard(field);
        
        chWin = checkWin(field);
        if(chWin==2)
        {
            printf("\n==> Player 2 wins!\n\n");
            break;
        }                
    }

    if(chWin==0) printf("\n==> The game is a draw!\n\n"); 

    return 0;
}