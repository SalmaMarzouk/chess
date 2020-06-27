#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
int turn=1,countwhite=0,countblack=0,pointer=0,undo=0; //counters for turn,captured pieces,undo array & undo times
//array for the game board
char board [8][8]={{'R','N','B','Q','K','B','N','R'},{'P','P','P','P','P','P','P','P'},{'\0','\0','\0','\0','\0','\0','\0','\0'},{'\0','\0','\0','\0','\0','\0','\0','\0'},{'\0','\0','\0','\0','\0','\0','\0','\0'},{'\0','\0','\0','\0','\0','\0','\0','\0'},{'p','p','p','p','p','p','p','p'},{'r','n','b','q','k','b','n','r'}};
char whiteout[16]={'\0'},blackout[16]={'\0'}; // Arrays for captured pieces
char virtualboard[8][8]; //Virtual board to operate undesired moves on it (needed for check)
char undoarr[8][8][1000];
char undowc[16][1000],undobc[16][1000]; // Undo arrays for captured pieces
char cwarr[1000],cbarr[1000]; //Array of captured pieces counters (needed for undo)

typedef struct //structure of positions and a validation element (Needed to deal with functions)
{
   int row1,row2,col1,col2,val;
}move;
void display (); //Display the board
move read (); //Reads input from user
void clear(); //Flushes the extra input
int syntaxtest(move); //checks that the given position are on board
int validate(move); //validate many things (Piece move,piece exist,self attack or check...)
void clone (); //clones the board to virtual board
void backup(); //backs up the arrays & counters into the undo arrays
void restore(); //restore the arrays & counters from the undo arrays
int pieceexist (move); //checks if the desired piece to move exists
int pieceproperty (move); //checks that each player is trying to move his own pieces
int placefree (move,char arr[][8]); //checks if the destination place is free
int selfattack (move); //checks if the destination place has the same color of player playing this turn
int piecemove (move,char arr[][8]); //checks the type of piece to be moved
void domove (move,char arr[][8]); //do move
void capture (move,char arr[][8]); //save the captured piece & do move
int pawnmove (move,char arr[][8]); //checks pawn move according to game rules
int rookmove (move,char arr[][8]); //checks rook move according to game rules
int knightmove (move,char arr[][8]); //checks knight move according to game rules
int bishopmove (move,char arr[][8]); //checks bishop move according to game rules
int queenmove(move,char arr[][8]); //checks queen move according to game rules
int kingmove (move,char arr[][8]); //checks King move according to game rules
void apply(move gmove); //apply move according to place given using do move & capture
move findking (char arr[][8],char); //function to find white or black king position based on char parameter
int check(char temp[][8],move); //function to see if a specific place (King place) is threaten on specific board
int selfcheck(move); //function to see if the player move will put his king under check
int flowconrtol(); //controls the UI
void multiplayer(); //controls the game
int endgame(); //ends the game in case of checkmate of stalemate
void undoredo(char); //undo or redo function
void promotion (move); //promotion function
int save(char); //saving or loading function
int ispossiblemove(); //see if any move is possible to any place
int main()
{
    int flag=flowconrtol();
    if(flag)
        multiplayer();
    system("pause");
    return 0;
}
void display ()
{ //219 stands for ASCII char of white-colored rectangular & 186,187,188,201,204,205 stand for different position of frame char
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),15); //changing color from gray & black to white & black
    system("cls");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\t\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187,201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
    printf("        A          B          C          D          E          F          G          H\t\t\t%c White Captured%c\t\t%c Black Captured%c\n",186,186,186,186);
    printf("  %c",201);
    for (int i=0;i<88;i++) printf("%c",205);
    printf("%c\t\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\t\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",187,204,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,185,204,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,185);
    int c=13,k=0;
    for (int j=1;j<=8;j++,k+=2)
    {
     c=9;
     if(j%2)
        {
            for (int i=1;i<=20;i++)
                {
                if((i-1)%4==0)printf("%c %c",(i==9?9-j+'0':' '),186);
                if(i!=9&&i!=10&&i!=11&&i!=12)
                    printf("%c%c%c%c%c%c%c%c%c%c%c           ",219,219,219,219,219,219,219,219,219,219,219);
                else
                {
                    printf("%c%c%c%c%c%c%c%c%c%c%c     %c     ",219,219,219,219,219,(board[j-1][i-c]=='\0'?219:board[j-1][i-c]),219,219,219,219,219,(board[j-1][i-c+1]=='\0'?' ':board[j-1][i-c+1]));
                    c--;
                }

                if(i%4==0)
                    printf("%c %c\t\t%c   %c\t   %c\t%c\t\t%c   %c\t   %c\t%c\n",186,(i==12?9-j+'0':' '),186,i==12?whiteout[k]:' ',i==12?whiteout[k+1]:' ',186,186,i==12?blackout[k]:' ',i==12?blackout[k+1]:' ',186);
                }
        }
     else
        {
            for (int i=1;i<=20;i++)
                {
                if((i-1)%4==0)printf("%c %c",(i==9?9-j+'0':' '),186);
                if(i!=9&&i!=10&&i!=11&&i!=12)
                        printf("           %c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219);
                else
                {
                    printf("     %c     %c%c%c%c%c%c%c%c%c%c%c",(board[j-1][i-c]=='\0'?' ':board[j-1][i-c]),219,219,219,219,219,(board[j-1][i-c+1]=='\0'?219:board[j-1][i-c+1]),219,219,219,219,219);
                    c--;
                }
                if(i%4==0)
                    printf("%c %c\t\t%c   %c\t   %c\t%c\t\t%c   %c\t   %c\t%c\n",186,(i==12?9-j+'0':' '),186,i==12?whiteout[k]:' ',i==12?whiteout[k+1]:' ',186,186,i==12?blackout[k]:' ',i==12?blackout[k+1]:' ',186);
                }
       }
    }
    printf("  %c",200);
    for (int i=0;i<88;i++) printf("%c",205);
    printf("%c\t\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\t\t%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",188,200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188,200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
    printf("        A          B          C          D          E          F          G          H\n");
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),7);
}
move read ()
{
    char str[6]={'\0'};
    move fmove;
    printf("\n[Input options: (U)ndo || (R)edo || (S)ave || (Q)uit]\n\nPlayer #%d make your move >> ",(turn==1)?1:2);
    fgets(str,6,stdin);
    if((str[0]=='U'||str[0]=='u'||str[0]=='R'||str[0]=='r')&&str[1]=='\n')
    {   fmove.row1=str[0];
        fmove.val=1;
    }
    else if((str[0]=='S'||str[0]=='s')&&str[1]=='\n')
    {   fmove.row1=str[0];
        fmove.val=2;
    }
    else if((str[0]=='Q'||str[0]=='q')&&str[1]=='\n')
        fmove.val=3;
    else if (str[4]=='\n')
    {
        fmove.col1=(str[0]>='A'&&str[0]<='H')?str[0]-'A':str[0]-'a'; //change characters to index
        fmove.row1=8-(str[1]-'0');
        fmove.col2=(str[2]>='A'&&str[2]<='H')?str[2]-'A':str[2]-'a';
        fmove.row2=8-(str[3]-'0');
        fmove.val=4;
    }
    else
    {
    if(str[4]!='\0'&&str[4]!='\n')clear(); //flushes the input if it is extra
    fmove.val=0;
    }

    return fmove;

}
void clear ()
{
 while (getchar()!='\n'); //reading extra inputs (Flushing them)
 }
int validate(move fmove)
{ //Returns 0 if any test of them failed
    if(!pieceexist(fmove))
        return 0;
    if(!pieceproperty(fmove))
        return 0;
    if(!selfattack(fmove))
        return 0;
    if(!piecemove(fmove,board))
        return 0;
    if(!selfcheck(fmove))
        return 0;
    return 1;

}
int syntaxtest(move fmove) //to check if the user input matches with the board
{
    if (!(fmove.col1>=0&&fmove.col1<=7&&fmove.col2>=0&&fmove.col2<=7)){
        return 0;
    }
    if (!(fmove.row1>=0&&fmove.row1<=7&&fmove.row2>=0&&fmove.row2<=7))
    {
        return 0;
    }
return 1;
}
int pieceexist (move tmove)
{
 if(board[tmove.row1][tmove.col1]=='\0')
 {return 0;}
 return 1;
}
int pieceproperty (move pmove)
{
if ((turn==1&&islower(board[pmove.row1][pmove.col1]))||(turn==-1&&!islower(board[pmove.row1][pmove.col1])))
{return 1;}
return 0;
}
int placefree (move smove,char arr[][8])
{
    if(arr[smove.row2][smove.col2]=='\0')
        return 1;
    return 0;
}
int selfattack (move mmove)
{
    if(placefree(mmove,board))
        return 1;
    if((turn==1&&islower(board[mmove.row2][mmove.col2]))||(turn==-1&&!islower(board[mmove.row2][mmove.col2])))
        return 0;
        return 1;
}

int piecemove(move mmove,char arr[][8])
{
    switch (arr[mmove.row1][mmove.col1])
    {
        case 'P':
        case 'p':if(!pawnmove(mmove,arr))return 0;break;
        case 'R':
        case 'r':if(!rookmove(mmove,arr))return 0;break;
        case 'N':
        case 'n':if(!knightmove(mmove,arr))return 0;break;
        case 'B':
        case 'b':if(!bishopmove(mmove,arr))return 0;break;
        case 'Q':
        case 'q':if(!queenmove(mmove,arr))return 0;break;
        case 'K':
        case 'k':if(!kingmove(mmove,arr))return 0;break;
    }
return 1;
}
void domove (move mmove,char arr[][8])
{
     arr[mmove.row2][mmove.col2]=arr[mmove.row1][mmove.col1];
     arr[mmove.row1][mmove.col1]='\0';
}

void capture (move mmove,char arr[][8])
{
    if (turn==1)
        blackout[countblack++]=arr[mmove.row2][mmove.col2];
    else
        whiteout[countwhite++]=arr[mmove.row2][mmove.col2];
    domove(mmove,arr);
}

int pawnmove (move mmove,char arr[][8])
{
    if((placefree(mmove,arr))&&(mmove.col1==mmove.col2)&&(mmove.row1-mmove.row2==(turn==1?1:-1)))
        return 1;
    if((!placefree(mmove,arr))&&(mmove.row1-mmove.row2==(turn==1?1:-1))&&(mmove.col1-mmove.col2==1||mmove.col1-mmove.col2==-1))
        return 1;
    if((placefree(mmove,arr))&&(mmove.col1==mmove.col2)&&(mmove.row1==(turn==1?6:1))&&(mmove.row1-mmove.row2==(turn==1?2:-2))&&((turn==1)?(arr[5][mmove.col1]=='\0'):(arr[2][mmove.col1]=='\0')))
        return 1;
    return 0;
}

int rookmove (move mmove,char arr[][8])
{
    if (mmove.col1==mmove.col2)
    {
        int f=1;
        for (int i=(mmove.row1<mmove.row2?mmove.row1:mmove.row2)+1;i<(mmove.row1>mmove.row2?mmove.row1:mmove.row2);i++)
        {
            if(arr[i][mmove.col1]!='\0')
            {
                f=0;break;
            }
        }
        if(f)
        return 1;
    }
    if(mmove.row1==mmove.row2)
    {
        int f=1;
        for (int i=(mmove.col1<mmove.col2?mmove.col1:mmove.col2)+1;i<(mmove.col1>mmove.col2?mmove.col1:mmove.col2);i++)
        {
        if(arr[mmove.row1][i]!='\0')
            {
                f=0;break;
            }
        }
        if(f)
        return 1;
    }
 return 0;
}
int knightmove (move mmove,char arr[][8])
{
    if((abs(mmove.row1-mmove.row2)==1&&abs(mmove.col1-mmove.col2)==2)||(abs(mmove.row1-mmove.row2)==2&&abs(mmove.col1-mmove.col2)==1))
        return 1;
    return 0;
}
int bishopmove(move mmove,char arr[][8])
{
    if (abs(mmove.col1-mmove.col2)==abs(mmove.row1-mmove.row2))
    {
      if ((mmove.row1>mmove.row2&&mmove.col1>mmove.col2)||(mmove.row2>mmove.row1&&mmove.col2>mmove.col1))
        {
            int j=(mmove.col1<mmove.col2?mmove.col1:mmove.col2)+1,f=1;
            int i=(mmove.row1<mmove.row2?mmove.row1:mmove.row2)+1;
            for (;i<(mmove.row1>mmove.row2?mmove.row1:mmove.row2);i++,j++)
            {
            if(arr[i][j]!='\0'){
            f=0;break;
            }
            }
            if (f)
            return 1;
        }
      if (mmove.row1>mmove.row2&&mmove.col1<mmove.col2)
      {
       int j=mmove.col1+1,f=1;
         for (int i=mmove.row1-1;i>mmove.row2;i--,j++)
         {
         if(arr[i][j]!='\0'){
         f=0;break;
          }
         }
         if (f)
            return 1;
      }
      if (mmove.row1<mmove.row2&&mmove.col1>mmove.col2)
      {
       int j=mmove.col1-1,f=1;
         for (int i=mmove.row1+1;i<mmove.row2;i++,j--)
         {
         if(arr[i][j]!='\0'){
         f=0;break;
          }
         }
         if (f)
            return 1;
      }
    }
return 0;
}

int queenmove (move mmove,char arr[][8])
{
 if(rookmove(mmove,arr)||bishopmove(mmove,arr))
    return 1;
 return 0;
}
int kingmove (move mmove,char arr[][8])
{
if(((abs(mmove.row1-mmove.row2)==1)&&(abs(mmove.col1-mmove.col2)==1))||((mmove.row1==mmove.row2)&&(abs(mmove.col1-mmove.col2)==1))||((mmove.col1==mmove.col2)&&(abs(mmove.row1-mmove.row2)==1)))
    return 1;
return 0;
}

move findking(char arr[][8],char king)
{
 move coordinates;
 int f=0;
 for (int i=0;i<8;i++)
 {
    for (int j=0;j<8;j++)
        if(arr[i][j]==king)
        {
            coordinates.row2=i;
            coordinates.col2=j;
            f=1;break;
        }
    if(f)break;
 }
 return coordinates;
}

int check(char arr[][8],move kp)
{
    for (int i=0;i<8;i++)
    {
        for (int j=0;j<8;j++)
        {
        if((turn==1)?(islower(arr[i][j])==0&&arr[i][j]!='\0'):(islower(arr[i][j])!=0))
            {   turn*=-1; //reversing turn to check black piece threat at white turn
                kp.row1=i;kp.col1=j;
             if(piecemove(kp,arr))
             {
                turn*=-1; //reseting turn
                return 1;
             }
              turn*=-1;     //reseting turn
            }
        }
    }
    return 0;
}
void clone()
{
 for (int i=0;i<8;i++)
    {
        for (int j=0;j<8;j++)
            virtualboard[i][j]=board[i][j];
    }
}

int selfcheck(move dmove)
{
    clone(); //clones the board to virtual board check if there isn't self check apply the move on the main board
    domove(dmove,virtualboard);
    if(check(virtualboard,(turn==1)?findking(virtualboard,'k'):findking(virtualboard,'K')))return 0;
    return 1;
}
void apply(move gmove)
{
    if (placefree(gmove,board))
        domove(gmove,board);
    else
        capture(gmove,board);
}

int ispossiblemove()
{
    move temp;
    for (int i=0;i<8;i++)
    {
        for (int j=0;j<8;j++)
        {
            if(((turn==1)?(islower(board[i][j])!=0):(islower(board[i][j])==0&&board[i][j]!='\0')))
                for (int k=0;k<8;k++)
                {
                    for (int l=0;l<8;l++)
                    {
                    temp.row1=i;
                    temp.col1=j;
                    temp.row2=k;
                    temp.col2=l;
                    if(validate(temp))return 1;
                    }
                }
        }
    }
    return 0;
}
int endgame()
{
    /*if the king checked & there isn't any possible moves >> checkmate ,
    if the king isn't checked & there isn't any possible moves >> stalemate*/
 if(check(board,turn==1?findking(board,'k'):findking(board,'K')))
    {
        if(!ispossiblemove())
        {   SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),2); //changes color to Green
            printf("\n\tCheckmate ! , Player #%d won",turn==1?2:1);
            SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),7); //resting color to B&W
            return 1;
        }
        else
        {   SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),4); //changes color to RED
            printf(turn==1?"\n\t\t\t\t\t\t\t\tWhite king checked !":"\n\t\t\t\t\t\t\t\tBlack King checked !");
            SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),7); //resting color to B&W
        }
    }
    else
    {
        if(!ispossiblemove())
        {   SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),2); //changes color to Green
            printf("\nStalemate ! , drawn by stalemate");
            SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),7); //resting color to B&W
            return 1;
        }
    }
 return 0;
}
void backup()
{
 for (int i=0;i<8;i++)
    for (int j=0;j<8;j++)
        undoarr[i][j][pointer]=board[i][j];
 for (int i=0;i<16;i++)
 {
    undowc[i][pointer]=whiteout[i];
    undobc[i][pointer]=blackout[i];
 }
 cwarr[pointer]=countwhite;
 cbarr[pointer]=countblack;
}
void restore()
{
 for (int i=0;i<8;i++)
    for (int j=0;j<8;j++)
        board[i][j]=undoarr[i][j][pointer];
 for (int i=0;i<16;i++)
 {
    whiteout[i]=undowc[i][pointer];
    blackout[i]=undobc[i][pointer];
 }
 countwhite=cwarr[pointer];
 countblack=cbarr[pointer];
}
void undoredo(char ch)
{
    if((ch=='U'||ch=='u')&&pointer>0)
    {
        undo++;
        pointer--;
        restore();
        turn*=-1;
        return;
    }
    if((ch=='R'||ch=='r')&&undo>0)
    {
        undo--;
        pointer++;
        restore();
        turn*=-1;
        return;
    }
    return;
}
void promotion (move pmove)
{
    char str[3]={'s','\0','\0'};
    if ((turn==1&&board[pmove.row2][pmove.col2]=='p'&&pmove.row2==0)||(turn==-1&&board[pmove.row2][pmove.col2]=='P'&&pmove.row2==7))
        {   printf("Please enter one of the following pieces for promotion (Q,R,N,B) >> ");
            fgets(str,3,stdin);
            while ((str[0]!='q'&&str[0]!='Q'&&str[0]!='r'&&str[0]!='R'&&str[0]!='b'&&str[0]!='B'&&str[0]!='n'&&str[0]!='N')||(str[1]!='\n'))
            {
                if(str[1]!='\0'&&str[1]!='\n')clear(); //flushing extra input
                printf("Please enter one of the following pieces for promotion (Q,R,N,B) >> ");
                fgets(str,3,stdin);
            }
            switch (str[0])
            {
            case 'q':
            case 'Q':board[pmove.row2][pmove.col2]=(turn==1?'q':'Q');break;
            case 'r':
            case 'R':board[pmove.row2][pmove.col2]=(turn==1?'r':'R');break;
            case 'b':
            case 'B':board[pmove.row2][pmove.col2]=(turn==1?'b':'B');break;
            case 'n':
            case 'N':board[pmove.row2][pmove.col2]=(turn==1?'n':'N');break;
            }
        }
}
int save(char ch)
{
    if(ch=='s'||ch=='S')
    {
     FILE *pf;
     pf=fopen("save.txt","w");
     for (int i=0;i<8;i++)
         for (int j=0;j<8;j++)
             fputc(board[i][j],pf);
         for (int i=0;i<16;i++)
         {
            fputc(whiteout[i],pf);
            fputc(blackout[i],pf);
         }
     for (int i=0;i<8;i++)
         for (int j=0;j<8;j++)
                 for (int k=0;k<1000;k++)
                     fputc(undoarr[i][j][k],pf);
     for (int i=0;i<16;i++)
         for (int j=0;j<1000;j++)
             {
                fputc(undowc[i][j],pf);
                fputc(undobc[i][j],pf);
            }
     for (int i=0;i<1000;i++)
        {
            fputc(cwarr[i],pf);
            fputc(cbarr[i],pf);
        }
     fprintf(pf,"%d %d %d",turn,pointer,undo);
     fclose(pf);
    }
    else
    {
      FILE *pf;
      pf=fopen("save.txt","r");
      if (pf==NULL) return 0;
      for (int i=0;i<8;i++)
          for (int j=0;j<8;j++)
              board[i][j]=fgetc(pf);
      for (int i=0;i<16;i++)
      {
          whiteout[i]=fgetc(pf);
          blackout[i]=fgetc(pf);
      }
      for (int i=0;i<8;i++)
          for (int j=0;j<8;j++)
                  for (int k=0;k<1000;k++)
                      undoarr[i][j][k]=fgetc(pf);
      for (int i=0;i<16;i++)
          for (int j=0;j<1000;j++)
          {
              undowc[i][j]=fgetc(pf);
              undobc[i][j]=fgetc(pf);
          }
      for (int i=0;i<1000;i++)
          {
              cwarr[i]=fgetc(pf);
              cbarr[i]=fgetc(pf);
          }
      fscanf(pf,"%d %d %d",&turn,&pointer,&undo);
      fclose(pf);
     }
     return 1;
}
int flowconrtol()
{
    int r;
    char str[3]={'\0'};
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),2); //change color to green
    printf("Welcome to Ahmed W Salma Chess");
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),7); //reset color
    printf("\n\n\nPlease choose one of the following Choices:\n(1) New Game\n(2) Load a game\n(3) Exit\n>> ");
    fgets(str,3,stdin);r=str[0]-'0';
    while (!(str[1]=='\n'&&(r>=1&&r<=3)))
    {
        if(str[1]!='\0'&&str[1]!='\n')clear();
        printf("please enter a valid input >> ");
        fgets(str,3,stdin);r=str[0]-'0';
    }
    if(r==1)
        return 1;
    else if(r==2)
    {
        if(!save('l'))
            {printf("Error Loading file , please make sure that you saved a game before trying to load !");return 0;}
        return 1;
    }
    return 0;
}
void multiplayer()
{
    backup(); //backing the initial positions to undo arrays
    while (1)
    {
        display();
        if(endgame())return;
        move gmove=read();
        if (gmove.val==0)continue;
        if (gmove.val==1){undoredo(gmove.row1);continue;}
        if (gmove.val==2){save(gmove.row1);continue;}
        if (gmove.val==3)return;
        if (!syntaxtest(gmove))continue;
        if(!validate(gmove))continue;
        apply(gmove);
        promotion(gmove);
        pointer++;undo=0; //reseting undo counter & increasing undo array counter
        backup(); //backing to undo arrays
        turn*=-1;
    }
}
