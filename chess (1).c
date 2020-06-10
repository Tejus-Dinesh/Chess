#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<ctype.h>
#include <wchar.h>
#include <locale.h>
#include<io.h>


/*
-1  - Black Queen
-2  - Black King
-3  - Black Bishop
-4  - Black Knight
-5  - Black Rook
-6  - Black Pawn not moved
-7 -  Black Pawn moved
1 - White Queen
2 - White King
3 - White Bishop
4 - White Knight
5 - White Rook
6 - White Pawn not moved
7 - White Pawn moved
*/

void clrscr();                  // Clear the screen
void make_temp();               //Makes a copy of Board in tempboard
void make_board();              // Reset the Board
void printboard(int [][8]);     // Print the board
void piece(int);                // Prints the symbol of the chess piece

int king(int, int, int, int, int, int[][8]);    // Validates the king's move on the given board
int knight(int, int, int, int, int, int[][8]);  // Validates the Knight's move on the given board
int bishop(int, int, int, int, int, int[][8]);  // Validates the Bishop's move on the given board
int rook(int, int, int, int, int, int[][8]);    // Validates the Rook's move on the given board
int queen(int, int, int, int, int, int[][8]);   // Validates the QUeen's move on the given board
int pawn(int, int, int, int, int, int[][8]);    // Validates the Pawn's move on the given board
void move(int, int, int, int, int[][8]);        // Makes the move on the given board
int validate_move(int, int, int, int, int, int[][8]);   // Function that calls the above functions based on the piece present

int validate_input(int);            // Checks whether the string input given by player is within parameters of the board
int check_check(int, int, int);     // Checks if the given king is under check
int rook_check(int, int, int);      // Checks if the given Rook can break the check
int knight_check(int, int, int);    // Checks if the given Knight can break the check
int bishop_check(int, int, int);    // Checks if the given bishop can break the check
int queen_check(int, int, int);     // Checks if the given Queen can break the check
int king_check(int, int, int);      // Checks if the given King can break the check
int pawn_check(int, int, int, int); // Checks if the given Pawn can break the check
int checkmate(int);


// The state of the chess board at the start of the game, here so that it can be copied and pasted for convinience
#if 0
{-5, -4, -3, -1, -2, -3, -4, -5, 
    -6, -6, -6, -6, -6, -6, -6, -6,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    6, 6, 6, 6, 6, 6, 6, 6,
    5, 4, 3, 1, 2, 3, 4, 5};
#endif

int defaultboard[8][8] = 
   {-5, -4, -3, -1, -2, -3, -4, -5, 
    -6, -6, -6, -6, -6, -6, -6, -6,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    6, 6, 6, 6, 6, 6, 6, 6,
    5, 4, 3, 1, 2, 3, 4, 5};

int board[8][8];
int tempboard[8][8];

void make_board()
{   
    /*
    Resets the Board
    */
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            board[i][j] = defaultboard[i][j];
    
}

void make_temp()
{
    /*
    Makes a copy of Board in tempboard
    */
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            tempboard[i][j] = board[i][j];
}

int m[4];


void piece(int a)
{
    /*
    Prints the symbol of the piece
    The symbols are UNICODE, so the format of Command Prompt needs to be changed to Unicode
    */
    int oldMode = _setmode(_fileno(stdout), 0x00020000);    // Format of Command Prompt changed to Unicode and previous format stored in oldMode

    switch(a)
    {
        case 1: wprintf(L"\x2655"); 
                break;
        case 2: 
                wprintf(L"\x2654"); 
                break;
        case 3: wprintf(L"\x2657"); 
                break;
        case 4: wprintf(L"\x2658"); 
                break;
        case 5: wprintf(L"\x2656"); 
                break;
        case 6:  
        case 7: wprintf(L"\x2659"); 
                break;
        case -1: wprintf(L"\x265B"); 
                 break;
        case -2: wprintf(L"\x265A");  
                 break;
        case -3: wprintf(L"\x265D"); 
                 break;
        case -4: wprintf(L"\x265E"); 
                 break;
        case -5: wprintf(L"\x265C"); 
                 break;
        case -6: 
        case -7: wprintf(L"\x265F"); 
                 break;
    }
   _setmode(_fileno(stdout), oldMode);  // Format of Command Prompt changed back to original
}

void clrscr()
{
    system("@cls||clear");
}

void printboard(int a[][8])
{

    for(int i = 0; i < 8; i++)
        printf("    %c    ", i + 97); // Print the alphabets at the top of the chess board

    printf("\n");

    int c = 1; // A variable used to switch between black and white colour

    for(int i = 0; i < 8; i++)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 15 Means black background with white text // 15 Means black background with white text
        printf("-------------------------------------------------------------------------\n");


        for(int j = 0; j < 3; j++)
        {
            if(i % 2 == 0) // Every alternate row should change colour
                c = 1; 
            else
                c = 0;
            for(int k = 0; k < 9; k++)
            {
               
               if(k < 8)
               {
                    if (c)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); // 240 Means white background with black text
                        c = 0;
                    }
                    else
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 176); // 176 Means Blue background with Black text
                        c = 1;
                    }
               }

                
                
                if (k == 8)
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 15 Means black background with white text
        
                if (board[i][k] && j == 1 && k != 8)
                {
                    if(c == 0)
                    {
                        if(k == 0 && i%2 == 0)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); // 240 Means white background with black text
                            printf("|");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); // 240 Means white background with black text
                            printf("   ");
                            piece(a[i][k]);
                            printf("   ");
                        }
                        else
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 176); // 176 Means Blue background with Black text
                            printf("|");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); // 240 Means white background with black text
                            printf("   ");
                            piece(a[i][k]);
                            printf("   ");
                        }
   
                    }
                    else
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 176); // 176 Means Blue background with Black text
                        printf("|");
                        printf("   ");
                        piece(a[i][k]);
                        printf("   "); 
                    }
                }
                else
                {
                    if(c == 0)
                    {
                        if(k < 8)
                        {
                            if(k == 0 && i%2 == 0)
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); // 240 Means white background with black text
                                printf("|");
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); // 240 Means white background with black text
                                printf("        ");
                            }
                            else
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 176); // 176 Means Blue background with Black text
                                printf("|");
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); // 240 Means white background with black text
                                printf("        ");
                            }

                           
                        }
                        else
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); // 240 Means white background with black text
                            printf("|");
                            if(k == 8)
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 15 Means black background with white text
                            printf("        ");
                        }
                    }
                    else
                    {
                        if(k == 8)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 176); // 176 Means Blue background with Black text
                            printf("|");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 15 Means black background with white text
                            printf("        ");

                        }
                        else
                            printf("|        ");
                    }

                }

            }
            
            if(j == 1)
            printf("\b\b\b\b\b\b\b%d", 8 - i); // Printing the Number of the row on the right of the board
            printf("\n");
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 15 Means black background with white text
    printf("-------------------------------------------------------------------------\n");
}

int king(int fr, int fc, int tr, int tc, int bw, int a[][8])
{
    /*
    A King has eight possible moves.
    We check if the 'to row' and the 'to column' entered
    match with any of the eight valid moves.
    */

    int flag = 1, m1, m2, m3, m4, m5, m6, m7, m8;

    m1 = tr == fr-1 && tc == fc;
    m2 = tr == fr+1 && tc == fc;
    m3 = tr == fr   && tc == fc-1;
    m4 = tr == fr   && tc == fc+1;
    m5 = tr == fr-1 && tc == fc-1;
    m6 = tr == fr-1 && tc == fc+1;
    m7 = tr == fr+1 && tc == fc-1;
    m8 = tr == fr+1 && tc == fc+1;

    if(!(m1 || m2 || m3 || m4 || m5 || m6 || m7 || m8)) // If any of the moves are true
        flag = 0;

    if (flag)
    {
        
        if(a[tr][tc] * bw > 0)  // Check if piece in destination is of different colour
        {
            flag = 0;
        }
    }

    return flag;
}


int knight(int fr, int fc, int tr, int tc, int bw, int a[][8])
{
    /*
    A knight has eight possible moves.
    We check if the 'to row' and the 'to column' entered
    match with any of the eight valid moves.
    */

    int flag = 1, m1, m2, m3, m4, m5, m6, m7, m8;

    m1 = tr == fr-1 && tc == fc+2;
    m2 = tr == fr-1 && tc == fc-2;
    m3 = tr == fr+1 && tc == fc-2;
    m4 = tr == fr+1 && tc == fc+2;
    m5 = tr == fr-2 && tc == fc+1;
    m6 = tr == fr-2 && tc == fc-1;
    m7 = tr == fr+2 && tc == fc+1;
    m8 = tr == fr+2 && tc == fc-1;

    if(!(m1 || m2 || m3 || m4 || m5 || m6 || m7 || m8)) // If any of the moves are true
        flag = 0;

    if (flag)
    {
        
        if(a[tr][tc] * bw > 0)  // Check if piece in destination is of different colour
        {
            flag = 0;
        }
    }

    return flag;
}

int bishop(int fr, int fc, int tr, int tc, int bw, int a[][8])
{
    /*
    First make sure that the change in magnitude of (from row - to row) is equal to (from column - to column)

    Then check if the Bishop encounters any piece between source and destination (Destination exclusive)

    If it didn't, then check if destination has a piece and if so, if it's different colour
    */
    int flag = 1;
    
    if(!(abs(tr - fr) == abs(tc - fc)))
    {
        flag = 0;
    }

    //Bottom
    if(fr < tr)
    {
        //Bottom Right
        if(fc < tc)
        {
            int j = fc + 1;
            for(int i = fr + 1; i < tr && j < tc; i++, j++)
            {
               if(a[i][j])  //If a piece is encountered during the journey, it's invalid
                {
                    flag = 0;
                    break;
                }
            }
        }
        else // Bottom Left
        {
            int j = fc - 1;
            for(int i = fr + 1; i < tr && j > tc; i++, j--)
            {
               if(a[i][j])  //If a piece is encountered during the journey, it's invalid
                {
                    flag = 0;
                    break;
                }
            }
        }
        
    }
    else if(fr > tr)
    {
        //Top Right
        if(fc < tc)
        {
            int j = fc + 1;
            for(int i = fr - 1; i > tr && j < tc; i--, j++)
            {
               if(a[i][j])  //If a piece is encountered during the journey, it's invalid
                {
                    flag = 0;
                    break;
                }
            }
        }
        else // Top Left
        {
            int j = fc - 1;
            for(int i = fr - 1; i > tr && j > tc; i--, j--)
            {
               if(a[i][j])  //If a piece is encountered during the journey, it's invalid
                {
                    flag = 0;
                    break;
                }
            }
        }
        
    }

    if(flag)
        {
            if(a[tr][tc] * bw > 0)  // Check if piece in destination is of different colour
                flag = 0; 
        }

    return flag;
}

int rook(int fr, int fc, int tr, int tc, int bw, int a[][8])
{
    /*
    The if condition checks if the rook is moving in the same row or same column
    The next if checks if the Rook is moving (Up or Down)/(Left of Right)
    If the Rook encounters a piece between source and destination (Destination exclusive) then the move is not valid
    
    Later The check if the piece in the destination square is of different colour
    */
    int flag = 1;

    if(fr == tr)    // Same Row
    {
        if(fc - tc < 0) // Moving Down
        {
            for(int i = fc + 1; i < tc; i++)
            {
                if(a[fr][i])    //If a piece is encountered during the journey, it's invalid
                {
                    flag = 0;
                    break;
                }
            }
        }
        else    // Moving Up
        {
            
            for(int i = fc - 1; i > tc; i--)
            {
                if(a[fr][i])    //If a piece is encountered during the journey, it's invalid
                {
                    flag = 0;
                    break;
                }
            }
        
        }
        

    }

    else if(fc == tc)   // Same coulnm
    {
        if(fr - tr < 0) // Moving Right
        {
            for(int i = fr + 1; i < tr; i++)
            {
                if(a[i][fc])    //If a piece is encountered during the journey, it's invalid
                {
                    flag = 0;
                    break;
                }
            }
        }
        else    // Moving Left
        {
            for(int i = fr - 1 ; i > tr; i--)
            {
                if(a[i][fc])    //If a piece is encountered during the journey, it's invalid
                {
                    flag = 0;
                    break;
                }
            }
        
        }

    }

    if(flag)   
        {
            if(a[tr][tc] * bw > 0) // Check if piece in destination is of different colour
                flag = 0; 
        }

    return flag;

}

int queen(int fr, int fc, int tr, int tc, int bw, int a[][8])
{
    /*
    Queen's movement is like rook and bishop combined
    So if the row or column is same, rook function if called
    else bishop function is called
    */
    int flag = 1;

    if(fr == tr || fc == tc)
    {
        flag = rook(fr, fc, tr, tc, bw, a);
    }
    else
    {
        flag = bishop(fr, fc, tr, tc, bw, a);
    }
    
    return flag;
}

int pawn(int fr, int fc, int tr, int tc, int bw, int a[][8])
{
    /*
    A Pawn can have 3 or 4 legal moves
    Check if the given 'to square' matches with any of the legal moves
    */

    int flag = 0, m1, m2, m3, m4;


    if(a[fr][fc]*bw == 6) // Pawn that hasn't moved before
    {
        m1 = tr == fr - 1*bw && tc == fc && !a[tr][tc];
        m2 = tr == fr - 2*bw && tc == fc && !a[tr][tc] && !a[fr - 1*bw][tc];

        m3 = tr == fr - 1*bw && tc == fc - 1*bw && a[tr][tc]*bw < 0;
        m4 = tr == fr - 1*bw && tc == fc + 1*bw && a[tr][tc]*bw < 0;

        if(m1 || m2 || m3 || m4)
        {
            a[fr][fc] = 7*bw;
            flag = 1;
        }
        
    }

    else    // Pawn that has moved before
    {
        m1 = tr == fr - 1*bw && tc == fc && !a[tr][tc];
        m3 = tr == fr - 1*bw && tc == fc - 1*bw && a[tr][tc]*bw < 0;
        m4 = tr == fr - 1*bw && tc == fc + 1*bw && a[tr][tc]*bw < 0;


         if(m1 || m3 || m4)
            flag = 1;
    }
    

    return flag;
}

int pawn_check(int p, int q, int bw, int moved)
{
    /*
    Moves the given Pawn to every possbile position and check's if the check is broken
    */
    int check = -1, i, j;

    //Move 1
    if((bw == 1 && p > 0) ||(bw == -1 && p < 7))
    {
        make_temp(); // Make a copy of the current board

        i = p - 1*bw; 
        j = q;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    if(moved == 6)
    {
        //Move 2 
        if((bw == 1 && p > 1) ||(bw == -1 && p < 6))
        {
            make_temp(); // Make a copy of the current board

            i = p - 2*bw; 
            j = q;

            if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
            {    
                //If it can be moved, then move the piece on the temporary board and see if the check is broken
                move(p, q, i, j, tempboard);
                check = check_check(bw, 0, 1);
            }

            if(check == 0)
            {
                return 1;
            }
        }
    }

    return 0;
}

int king_check(int p, int q, int bw)
{
    /*
    Moves the King to every possbile position and check's if the check is broken
    */
    int flag = 1;

    int i, j, check = -1;

    //Move 1 Moving Up
    if(p > 0)
    {
        make_temp(); // Make a copy of the current board

        i = p - 1; 
        j = q;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 2 Moving Down
    if(p < 7)
    {
        make_temp(); // Make a copy of the current board

        i = p + 1; 
        j = q;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 3 Moving Left
    if(q > 0)
    {
        make_temp(); // Make a copy of the current board

        i = p; 
        j = q - 1;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 4 Moving Right
    if(q < 7)
    {
        make_temp(); // Make a copy of the current board

        i = p; 
        j = q + 1;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 5 Moving Top right
    if(p > 0 && q < 7)
    {
        make_temp(); // Make a copy of the current board

        i = p - 1; 
        j = q + 1;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 6 Moving Top left
    if(p > 0 && q > 0)
    {
        make_temp(); // Make a copy of the current board

        i = p - 1; 
        j = q - 1;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 7 Moving Bottom right
    if(p < 7 && q < 7)
    {
        make_temp(); // Make a copy of the current board

        i = p + 1; 
        j = q + 1;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 8 Moving Bottom left
    if(p < 7 && q > 0)
    {
        make_temp(); // Make a copy of the current board

        i = p + 1; 
        j = q - 1;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    return 0;
}

int queen_check(int p, int q, int bw)
{
    /*
    Moves the Queen to every possbile position and check's if the check is broken
    As Queen can move like both Bishop and Rook, the functions are resued
    */
    int check;

    check = rook_check(p, q, bw) || bishop_check(p, q, bw);

    return check;
}

int bishop_check(int p, int q, int bw)
{
    /*
    Moves the Bishop to every possbile position and check's if the check is broken
    */
    int flag = 1, check = -1;

    int j;
    //Moving the Bishop Bottom Right
    j = q + 1;
    for(int i = p + 1; i < 8 && j < 8; i++, j++)
    {
        make_temp(); // Make a copy of the current board

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Moving the Bishop Bottom Left
    j = q - 1;
    for(int i = p + 1; i < 8 && j >= 0; i++, j--)
    {
       make_temp(); // Make a copy of the current board

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }
    
    
    //Moving the Bishop Top Right

    j = q + 1;
    for(int i = p - 1; i >= 0 && j < 8; i--, j++)
    {
        make_temp(); // Make a copy of the current board

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }
        

    //Moving the Bishop Top Left
    j = q - 1;
    for(int i = p - 1; i >= 0 && j >= 0; i--, j--)
    {
        make_temp(); // Make a copy of the current board

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }
        
    return 0; 
    
}

int knight_check(int p, int q, int bw)
{

    /*
    Moves the Knight to every possbile position and check's if the check is broken
    */
    int flag = 1;

    int i, j, check;

    //Move 1 Moving Top right
    if(p > 0 && q <= 5)
    {
        make_temp(); // Make a copy of the current board

        i = p - 1; 
        j = q + 2;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 2 Moving Top Left
    if(p > 0 && q >= 2)
    {
        make_temp(); // Make a copy of the current board

        i = p - 1; 
        j = q - 2;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 3 Moving Bottom Left
    if(p < 7 && q >= 2)
    {
        make_temp(); // Make a copy of the current board

        i = p + 1; 
        j = q - 2;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 4 Moving Bottom Right
    if(p < 7 && q <= 5)
    {
        make_temp(); // Make a copy of the current board

        i = p + 1; 
        j = q + 2;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 5 Moving Top Right
    if(p >= 2 && q < 7)
    {
        make_temp(); // Make a copy of the current board

        i = p - 2; 
        j = q + 1;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 6 Moving Top Left
    if(p >= 2 && q >= 1)
    {
        make_temp(); // Make a copy of the current board

        i = p - 2; 
        j = q - 1;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 7 Moving Bottom Right
    if(p <= 5 && q < 7)
    {
        make_temp(); // Make a copy of the current board

        i = p + 2; 
        j = q + 1;

        if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
        {    
            //If it can be moved, then move the piece on the temporary board and see if the check is broken
            move(p, q, i, j, tempboard);
            check = check_check(bw, 0, 1);
        }

        if(check == 0)
        {
            return 1;
        }
    }

    //Move 8 Moving Bottom Left
    if(p <= 5 && q > 0)
    make_temp(); // Make a copy of the current board

    i = p + 2; 
    j = q - 1;

    if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
    {    
        //If it can be moved, then move the piece on the temporary board and see if the check is broken
        move(p, q, i, j, tempboard);
        check = check_check(bw, 0, 1);
    }

    if(check == 0)
    {
        return 1;
    }

    return 0; // Control will reach here ONLY if none of the previous moves are not possible
}

int rook_check(int p, int q, int bw)
{
    /*
    Moves the rook to every possbile position and check's if the check is broken
    */
    int flag = 1, check = -1;
    int i = p;
    int j = q;

    //Moving the Rook down
    if(p < 7)
    {
        j = q;

        for(int i = p + 1; i < 8; i++)
        {
            make_temp(); // Make a copy of the current board

            if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
            {    
                //If it can be moved, then move the piece on the temporary board and see if the check is broken
                move(p, q, i, j, tempboard);
                check = check_check(bw, 0, 1);
            }

            if(check == 0)
            {
                return 1;
            }
        }
    }

    //Moving the Rook Up
    if(p > 0)
    {
        j = q;

        for(int i = p - 1; i  >= 0; i--)
        {
            make_temp(); // Make a copy of the current board

            if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
            {    
                //If it can be moved, then move the piece on the temporary board and see if the check is broken
                move(p, q, i, j, tempboard);
                check = check_check(bw, 0, 1);
            }

            if(check == 0)
            {
                return 1;
            }
        }
    }

    //Moving the Rook Left
    if(q < 8)
    {
        i = p;

        for(int j = q + 1; j < 8; j++)
        {

            make_temp(); // Make a copy of the current board

            if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
            {    
                //If it can be moved, then move the piece on the temporary board and see if the check is broken
                move(p, q, i, j, tempboard);
                check = check_check(bw, 0, 1);
            }

            if(check == 0)
            {
                return 1;
            }
        }
    }

    //Moving the Rook Right
    if(q > 0)
     {
        i = p;

        for(int j = q - 1; j >= 0; j--)
        {
            
            make_temp(); // Make a copy of the current board

            if(validate_move(p, q, i, j, bw, tempboard) == 1) // Check if the piece can be moved to the specified square
            {    
                //If it can be moved, then move the piece on the temporary board and see if the check is broken
                move(p, q, i, j, tempboard);
                check = check_check(bw, 0, 1);
            }

            if(check == 0)
            {
                return 1;
            }
        }
    }

    return 0;   // Control reaches here if no move of the Rook can break the check
        
}

int checkmate(int bw)
{
    
    int flag;

    // The functions in the switch return 1 if the check can be broken

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
           {
               if(board[i][j] * bw > 0)
               {
                   switch (board[i][j] * bw)
                   {
                        case 1: flag = queen_check(i, j, bw); break; // Can a Queen's move break the check
                        
                        case 2: flag = king_check(i, j, bw); break; // Can a King's move break the check
                        
                        case 3: flag = bishop_check(i, j, bw);  break; // Can a Bishop's move break the check

                        case 4: flag = knight_check(i, j, bw);  break; // Can a Knight's move break the check

                        case 5: flag = rook_check(i, j, bw); break; // Can a Rook's move break the check

                        case 6: flag = pawn_check(i, j, bw, 6); break; // Can a Pawn's move break the check

                        case 7: flag = pawn_check(i, j, bw, 7); break; // Can a Pawn's move break the check
                   }

                   if(flag == 1)
                   {
                       return 0;
                   }   
               }
           }

    return 1;   //Control reaches here only if it's checkmate
}

void move(int fr, int fc, int tr, int tc, int a[][8])
{
    a[tr][tc] = a[fr][fc];
    a[fr][fc] = 0;
}

int validate_move(int fr, int fc, int tr, int tc, int bw, int a[][8])
{
    /*
    Since each piece is denoted by a number, based on the number in the 'from'
    square different functions are called to make the move
    */

   int flag;

    switch(a[fr][fc])
    {
        case  1:
        case -1:   flag = queen(fr, fc, tr, tc, bw, a); break; 
        case  2:  
        case -2:   flag = king(fr, fc, tr, tc, bw, a); break;  
        case  3:  
        case -3:   flag = bishop(fr, fc, tr, tc, bw, a); break;
        case  4:  
        case -4:   flag = knight(fr, fc, tr, tc, bw, a); break;
        case  5:  
        case -5:   flag = rook(fr, fc, tr, tc, bw, a); break;
        case  6:  
        case -6:   
        case  7:
        case -7:   flag = pawn(fr, fc, tr, tc, bw, a); break;  
    }

    if(flag == 0)
    {
        return 0;
    }
    else
    {
        m[0] = fr;
        m[1] = fc;
        m[2] = tr;
        m[3] = tc;
        return 1;
    }
}

int validate_input(int bw)
{
    char m[10], fr, fc, tr, tc; // array stores what move user enters, like e2e3.
    // fr - From row
    // fc - From Column
    // tr - To row
    // tc - To column

    int flag; // Holds whether the move is valid or not
    
    while(1)
    {
        flag = 1;
        fflush(stdin);
        scanf(" %s", m);    // Reading the move

        // Move should always be of length four. e1e2 -> Valid;   e1e22 -> Invalid
        if(strlen(m) != 4)
        {
            flag = 0;
            printf("Invalid entry, try again: ");
            continue;
        }

        else
        {
            // Convert entered move to lower case. Ex: E1E2 -> e1e2
            m[0] = tolower(m[0]);
            m[2] = tolower(m[2]); 
        
            // From 
            switch (m[0])  // The first character is the from fc. Ex: e1f1 ---- e is from column 
            {
                case 'a': fc = 0; break;
                case 'b': fc = 1; break;
                case 'c': fc = 2; break;
                case 'd': fc = 3; break;
                case 'e': fc = 4; break;
                case 'f': fc = 5; break;
                case 'g': fc = 6; break;
                case 'h': fc = 7; break;
                
                default: flag = 0;  // If user entered anything other that a - h
            
            }

            fr = 8 - (m[1] - 48);   // Make into ASCII integer and store in 'from row'

            if(fr < 0 || fr > 7)    // Row should be from 1 - 8 (For the program it's 0 - 7)
                flag = 0;


            // To
            switch (m[2])   // The third character is the from tc. Ex: e1f1 ---- f is to column 
            {
                case 'a': tc = 0; break;
                case 'b': tc = 1; break;
                case 'c': tc = 2; break;
                case 'd': tc = 3; break;
                case 'e': tc = 4; break;
                case 'f': tc = 5; break;
                case 'g': tc = 6; break;
                case 'h': tc = 7; break;
                
                default: flag = 0;  // If user entered anything other that a - h
            
            }

            tr = 8 - (m[3] - 48);   // Make into ASCII integer and store in 'to row'


            if(tr < 0 || tr > 7)    // Row should be from 1 - 8 (For the program it's 0 - 7)
                flag = 0;


            if(fr == tr && fc == tc) // Make sure 'from' and 'to' squares are not the same
                flag = 0;

            if(bw * board[fr][fc] < 0) // White/Black's move validation
            {
                /*
                bw is -1 if black's move, if in the 'from' square there is white piece (Which is +ve number in the matrix),
                Multiplication of these two will give -ve number, means invalid!

                if bw is -1 and in the 'from' square there is black piece (Which is -ve number in the matrix),
                Multiplication of these two will give +ve number, means Valid!
                */

                flag = 0;
            }

            if(!board[fr][fc]) // If there is no piece in the from square, it's invalid
                flag = 0;


            if(flag == 0)
            {
                printf("Invalid entry, try again: ");
            }
            else
                break;

        }

    }

    return validate_move(fr, fc, tr, tc, bw, board);
    
}

int check_check(int bw, int moveornot, int mate)
{
    int kr, kc, check = 0;
   
    if(!mate)
    {
        make_temp(); // Make a copy of the current board
    }

    //printboard(tempboard);
    
    if(moveornot)
    {
        move(m[0], m[1], m[2], m[3], tempboard);
    }

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(bw * tempboard[i][j] == 2)
            {
                kr = i;
                kc = j;
                break;
            }

    //Top
    if(kr != 0)
    {
        for(int i = kr - 1; i >= 0; i--)
        {
            if(tempboard[i][kc] * bw < 0)
            {
                
                if(validate_move(i, kc, kr, kc, bw * -1, tempboard))
                {
                    //printf("Top");
                    check = 1;
                    break;
                }
                else
                    break;
            }
            else if(tempboard[i][kc] * bw > 0)
                break;
        }
    }

    //Down
    if(kr != 7)
    {
        for(int i = kr + 1; i <= 7; i++)
        {
            if(tempboard[i][kc] * bw < 0)
            {
                if(validate_move(i, kc, kr, kc, bw * -1, tempboard))
                {
                    //printf("Down");
                    check = 1;
                    break;
                }
                else
                    break;
            }
            else if(tempboard[i][kc] * bw > 0)
                break;
            
        }
    }

    //Left
    if(kc != 0)
    {
        for(int j = kc - 1; j >= 0; j--)
        {
            if(tempboard[kr][j] * bw < 0)
            {
                if(validate_move(j, kc, kr, kc, bw * -1, tempboard))
                {
                    //printf("Left");
                    check = 1;
                    break;
                }
                else
                    break;
            }
            else if(tempboard[kr][j] * bw > 0)
                break;
        }
    }

    //Right
    if(kc != 7)
    {
        for(int j = kc + 1; j <= 7; j++)
        {
            if(tempboard[kr][j] * bw < 0)
            {
                if(validate_move(j, kc, kr, kc, bw * -1, tempboard))
                {
                    //printf("Right");
                    check = 1;
                    break;
                }
                else
                    break;
            }
            else if(tempboard[kr][j] * bw > 0)
                break;
        }
    }

    //Top Right Diagonal
    if(kr != 0 && kc != 7)
    {
        int j = kc + 1;
        for(int i = kr - 1; i >= 0 && j <= 7; i--, j++)
        {
            if(tempboard[i][j] * bw < 0)
            {
                
                if(validate_move(i, j, kr, kc, bw * -1, tempboard))
                {
                    check = 1;
                    break;
                }
                else
                    break;
            }
            else if(tempboard[i][j] * bw > 0)
                break;
        }
    }


    //Top Left Diagonal
    if(kr != 0 && kc != 0)
    {
        int j = kc - 1;
        for(int i = kr - 1; i >= 0 && j >= 0; i--, j--)
        {
            if(tempboard[i][j] * bw < 0)
            {
                
                if(validate_move(i, j, kr, kc, bw * -1, tempboard))
                {
                    check = 1;
                    break;
                }
                else
                    break;
            }
            else if(tempboard[i][j] * bw > 0)
                break;
        }
    }

    //Bottom Right Diagonal
    if(kr != 7 && kc != 7)
    {
        int j = kc + 1;
        for(int i = kr + 1; i <= 7 && j <= 7; i++, j++)
        {
            if(tempboard[i][j] * bw < 0)
            {
                
                if(validate_move(i, j, kr, kc, bw * -1, tempboard))
                {
                    check = 1;
                    break;
                }
                else
                    break;
            }
            else if(tempboard[i][j] * bw > 0)
                break;
        }
    }

    //Bottom Left Diagonal
    if(kr != 7 && kc != 0)
    {

        int j = kc - 1;
        for(int i = kr + 1; i <= 7 && j >= 0; i++, j--)
        {
            if(tempboard[i][j] * bw < 0)
            {

                if(validate_move(i, j, kr, kc, bw * -1, tempboard))
                {
                    check = 1;
                    break;
                }
                else
                    break;
            }
            else if(tempboard[i][j] * bw > 0)
                break;
        }
    }

    
    return check;
}

int main()
{
    int choice = 1;

    while(choice != 2)
    {
        clrscr();
        printf("Press ENTER to start the Chess Game\n");

        getch();

        clrscr();

        make_board();
        printboard(board);

        int t = 1; // Decides whether black's move or white's move
        int input, check;

        // If anyone makes more than three illegal moves the game is over
        int black_illegal = 0;
        int white_illegal = 0;

        while(choice != 2)
        {
            if(t) // If t == 1 then it's white's move
            {
                printf("White's Move: ");

                input = validate_input(1);  // Returns one if the piece can move
                check = check_check(1, 1, 0);   // Returns 1 if the previous move results in a check, i.e. illegal

                if(input && !check) // Passing 1 so that the function knows it's white's move
                {   
                    // If 1 was returned, change the value of t, so that next time black's move is exceuted
                    t = 0;
                    move(m[0], m[1], m[2], m[3], board);   
                    clrscr();
                    printboard(board);
                    
                }
                else if(input == 0) // Invalid move
                {
                    printf("Invalid move, enter again! \n");
                }
                else
                {
                    if(++white_illegal == 3)
                    {
                        printf("You've made three illegal moves! Black wins\n");
                        break;
                    }
                    else
                        printf("Your move results in a check for your King, try again!\n");
                }

                if(check_check(-1, 0, 0))   // Checks if White's move results in check for black
                {   // If it does, then check if it's checkmate
                    if(checkmate(-1))
                    {
                        printf("CHECKMATE! White Wins!");
                        break;
                    }
                    printf("Your King is under check! \n");
                }
            }


            else // If t == 0 then it's black's move
            {
                printf("Black's Move: ");

                input = validate_input(-1);
                check = check_check(-1, 1, 0);

                if(input && !check)       // Passing -1 so that the function knows it's black's move
                {
                    // If 1 was returned, change the value of t, so that next time white's move is exceuted

                    t = 1;
                    move(m[0], m[1], m[2], m[3], board);
                    clrscr();
                    printboard(board);
                }
                else if(input == 0)
                {
                    printf("Invalid move, enter again! \n");
                }
                else
                {
                    if(++black_illegal == 3)
                    {
                        printf("You've made three illegal moves! White wins\n");
                        break;
                    }
                    else
                        printf("Your move results in a check for your King, try again!\n");
                    
                }

                if(check_check(1, 0, 0))    // Checks if Black's move results in check for White
                {
                    if(checkmate(1)) // If it does, then check if it's checkmate
                    {
                        printf("CHECKMATE! Black Wins!");
                        break;
                    }
                    printf("Your King is under check! \n");
                }
                
            }
        }
        
        printf("\n\nWould You like to:\n1.Start a new game \n2.Exit\n");

        scanf("%d", &choice);

    }

    return 0;
}