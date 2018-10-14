/*
 * single.cpp
 *
 *  Created on: 15-Oct-2018
 *      Author: kushal
 */


#include <iostream>
#include <fstream>

#include "two_player.h"

using namespace std;

char Board[5][5];
int Flag = 0, Wm = 0, Wp = 0, Bm = 0, Bp = 0;

bool Whosemove = 1;
int result;
int sum1 = 0, sum2 = 0, goFlag=0;		//goFlag-> gameoverFlag to check if the mValidation is for gameover()
char sX, sY;
string s;

class game
{
public:
	int r, c;
};
void move(); //asks for a string input from the user and verifies the length and format of given input
int mValidation (game, game); //checks if the given input is valid according to game rules, if not suspends the game/asks for input again
void Elimination (game, game); //if given input involves elimination of a piece, this function is to carry out elimination and increment respective counters
void elim_package (game, game, int, int, int);	//it's called from mValidation, with the specific purpose of checking whether elimination moves are valid
int checkVerticalElimination (game, game); //called from elim_package to check validity of vertical elimination
int checkHorizontalElimination (game, game, int); //called from elim_package to check validity of horizontal elimination

/*in the original game it was not possible to write one generalized elim_package for vertical and horizontal eliminations
hence two separate sub functions were created to check eliminations. when the original game was deemed to complex, code was modified but this part was left as is for the sake of code re-usability )
*/

void colour_generalize (int); // a function used to generalize all all other function for both colors
void makeMove (game, game); //after a input as has been validated, it is carried out and the move is made in this function
void displayBoard ();//displays the current Board state whenever called
int gameover ();//checks whether the winning condition has been satisfied by either side or if there is a deadlock, i.e. , no valid moves left for at least one side.
/*note: the deadlock condition for each side/player is checked only before it is their turn not before the opponents Board,
 *  as the Board state will change the after an opponents turn thus changing the state of the deadlock */
void Winner();//in the event that a deadlock has occurred before either side could achieve the winning condition the following function will be called to decide which player won based on current Board state

int gameover ()
{
	fstream fobj;
    result=3;
    bool x = Whosemove;
    colour_generalize (x);
    char X1=sX;
    char Y1=sY;
    sum1 = 0;
    sum2 = 0;
    game gsinput, gminput;
    int i, j, k, l;

    for (j = 0; j < 5; j++)
    {
        if (Board[0][j] == 'W')
	    {
	        sum1++;
	        cout<<"WHITE WINS\n";
	        fobj.open("log_file.txt",ios::app|ios::out);
	        fobj<<"BLACK WINS\n";
	        fobj.close();
	        return 1;
        }
        if (Board[4][j] == 'B')
	    {
	        sum2++;
	        cout<<"BLACK WINS\n";
	        fobj.open("log_file.txt",ios::app|ios::out);
	        fobj<<"WHITE WINS\n";
	        fobj.close();
	        return 1;
        }
    }

    if(sum1== 0 && sum2==0)
    {
        for (i = 0; i < 5; i++)
	    {
	        for (j = 0; j < 5; j++)
	        {
		        if (Board[i][j] == '_')	//if place on Board is blank then continue
                {
                    continue;
                }
                else if (Board[i][j] == X1 || Board[i][j] == Y1)
                {
                    gsinput.r = i;
                    gsinput.c = j;
                    for (k = 0; k<=2; k++)
                    {
                        for (l = -2; l <= 2; l++)
                        {
                            if(x==1)
                            {gminput.r = i + k;}
                            else
                            {gminput.r = i - k;}
                            gminput.c = j + l;
                            if (gminput.c <= 4 && gminput.c >= 0 && gminput.r <= 4 && gminput.r >= 0)
                            {
                                goFlag = mValidation (gsinput, gminput);
                                if (goFlag == 1 || goFlag == 2)
                                {
                                    result = 0;
				                    return 0;	//still valid moves left
				                }
				            }
                        }
                    }
                }
            }
	    }
    }
    return result;
}

void Winner()
{
	fstream fobj;
	if(Wm>Bm)
	{
		fobj.open("log_file.txt",ios::app|ios::out);
		fobj<<"BLACK WINS\n";
		fobj.close();
		cout<<"BLACK WINS!\n";
	}
	else if(Bm>Wm)
	{
		fobj.open("log_file.txt",ios::app|ios::out);
   		fobj<<"WHITE WINS!\n";
   		fobj.close();
		cout<<"WHITE WINS!\n";
	}
	else if(Wm==Bm)
	{
			if(Wp>Bp)
			{
				fobj.open("log_file.txt",ios::app|ios::out);
				fobj<<"BLACK WINS\n";
				fobj.close();
				cout<<"BLACK WINS!\n";
			}
			else if(Bp>Wp)
			{
				fobj.open("log_file.txt",ios::app|ios::out);
	       		fobj<<"WHITE WINS!\n";
	       		fobj.close();
				cout<<"WHITE WINS!\n";
			}
			else
			{
				fobj.open("log_file.txt",ios::app|ios::out);
				fobj<<"IT'S A DRAW!\n";
				fobj.close();
				cout<<"IT'S A DRAW!\n";
			}
	}
}
int mValidation (game sinput, game minput)
{
    int d1, d2, x = Whosemove;
    d1 = minput.r - sinput.r;	// difference between initial and final row position
    d2 = minput.c - sinput.c;	//difference between initial and final cols position
    colour_generalize (x);	//generalize all functions to work for either colour
    if (Board[minput.r][minput.c] == '_' && minput.c >= 0 && minput.c <= 4 && minput.r <= 4 && minput.c >= 0)	//check if final position is vacant
    {
        if (Board[sinput.r][sinput.c] == sX)	//moveset for mantri
	    {
	        if ((d1 == (2 * x - 1) || d1 == 0) && (d2 == (2 * x - 1) || d2 == -(2 * x - 1) || d2 == 0))	//mantri moves single space in any allowed direction
	        {
	            Flag = 1;		//signifies valid non-eliminating move,turn passes to opponent
	        }
	        else if ((d1 == 2 * (2 * x - 1) && d2 == 0) || (d1 == 0 && (d2 == 2 * (2 * x - 1) || d2 == -2 * (2 * x - 1))))	//moves two spaces, in an elimination move
	        {
	            elim_package (sinput, minput, x, d1, d2);	//checks if elimination is possible and sets Flag accordingly
	        }
	        else
	        {
	            Flag = 0;
	        }
	    }
        else if (Board[sinput.r][sinput.c] == sY)	//moveset for pawn
	    {
	        if ((d1 == (2 * x - 1) && d2 == (2 * x - 1)) || (d1 == (2 * x - 1) && d2 == -(2 * x - 1)))	//moves a single space
	        {
	            Flag = 1;
	        }
	        else if ((d1 == 2 * (2 * x - 1) && d2 == 0) || (d1 == 0 && (d2 == 2 * (2 * x - 1) || d2 == -2 * (2 * x - 1))))	//moves two spaces, in an elimination move
	        {
	            elim_package (sinput, minput, x, d1, d2);
	        }
	        else
	        {
	            Flag = 0;
	        }
	    }
    }
    else
    {
        Flag = 0;
    }
    return Flag;
}

void elim_package (game sinput, game minput, int x, int d1, int d2)
{
    if (d1 == 2 * (2 * x - 1) && d2 == 0)	//mantri moves two rows up, in an elimination move
    {
        Flag = checkVerticalElimination (sinput, minput);	//call function that checks conditions for eliminations
    }
    else if (d1 == 0 && (d2 == 2 * (2 * x - 1) || d2 == -2 * (2 * x - 1)))	//mantri moves horizontally in elimination move
    {
        Flag = checkHorizontalElimination (sinput, minput, x);
    }

}

int checkVerticalElimination (game sinput, game minput)
{
    bool x = Whosemove;
    char t1 = Board[(sinput.r + minput.r) / 2][(sinput.c + minput.c) / 2];	//target piece  //changing value of X and Y temporarily to detect correct target
    colour_generalize (!x);
    if ((t1 == sX || t1 == sY))	//check if neighbouring squares and piece being attacked
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

int checkHorizontalElimination (game sinput, game minput, int x)
{
    char t1 = Board[(sinput.r + minput.r) / 2][(sinput.c + minput.c) / 2];
    colour_generalize (!x);	//changing value of X and Y temporarily to detect correct target
    if ((t1 == sX || t1 == sY)&& sinput.r!=4 && sinput.r!=0)	//check if neighbouring squares and piece being attacked
    {
        return 2;
    }
    else
    {
        return 0;
    }
}
void Elimination (game sinput, game minput)
{
    bool x=!Whosemove;
    colour_generalize(x);
    char t1 = Board[(sinput.r + minput.r) / 2][(sinput.c + minput.c) / 2];
    if(x==1)
    {
        if (t1 == sX)
        {
        	Bm++;			//increment count for dead mantri
        }
        else
        {
            Bp++;			//increment count for dead pawn
        }
    }
    else
    {
         if (t1 == sX)
	    {
	        Wm++;			//increment count for dead mantri
        }
        else
	    {
            Wp++;			//increment count for dead pawn
	    }
    }
    Board[(sinput.r + minput.r) / 2][(sinput.c + minput.c) / 2] = '_';
}

void move ()
{
	fstream fobj;
	fobj.open("log_file.txt",ios::out|ios::app);
	Flag=0;
	displayBoard ();
    game sinput, minput;
    int lengths;
    if (Whosemove == 1)
    cout << "BLACK" << endl;
    else
    cout << "WHITE" << endl;
    int x = Whosemove;
    colour_generalize (x);
    cout << "Enter Input\n";
    getline(cin,s);
    lengths=s.size();
    if(lengths==7)
    {
        for(int i=1;i<lengths;i++)
        {
        	if(s[i]!=' ')
        	{
        		cout<<"ERROR:\nINVALID MOVE\n";
			return;
        	}
        	i++;
        }
        for(int i=0;i<lengths;i++)
        {
        	if((s[i]<48 || s[i]>54))
        	{
        		cout<<"ERROR:\nINVALID MOVE\n";
			return;
        	}
        	i++;
        }

        sinput.r = s[0]-48;
        sinput.c = s[2]-48;
        minput.r = s[4]-48;
        minput.c = s[6]-48;
        if (Board[sinput.r][sinput.c] == sX || Board[sinput.r][sinput.c] == sY)
        {
        	Flag = mValidation (sinput, minput);	//to check if the selected move is valid
        	if (Flag == 1 || Flag == 2)	// 1->non eliminating move 2->eliminating move
        	{
        		fobj<<"P"<<2-Whosemove<<" : "<<s<<endl;
        		makeMove(sinput,minput);
        	}
        	else if (Flag == 0)
        	{
        		cout << "ERROR:\nINVALID MOVE\nEnter Input again!\n";
        	}
        }
        else
        {
        	cout << "Invalid selection!" << endl;
        }
    }
    else
    {
    	cout<<"ERROR:\n INVALID MOVE\n Enter again!\n";
    }
    fobj.close();
}
void makeMove (game sinput, game minput)
{
    if (Flag == 1)
    {
        Board[minput.r][minput.c] = Board[sinput.r][sinput.c];
        Board[sinput.r][sinput.c] = '_';
    }
    else if (Flag == 2)
    {
        Board[minput.r][minput.c] = Board[sinput.r][sinput.c];
        Board[sinput.r][sinput.c] = '_';
        Elimination (sinput, minput);
    }
}

void colour_generalize (int x)
{
    if (x == 0)
    {
        sX = 'W';			//X initialized to mantri
        sY = 'w';			//Y initialized to pawn
    }
    else
    {
        sX = 'B';
        sY = 'b';
    }
}

void displayBoard ()
{
    int i, j;
    cout << "    0 1 2 3 4\n";
    cout << "  -----------\n";
    for (i = 0; i < 5; i++)
    {
        cout << i << "|  ";
        for (j = 0; j < 5; j++)
	    {
	        cout << Board[i][j] << ' ';
	    }
        cout << "\n";
    }
}

void initBoard () //initializes the Board state to starting state at the beginning of the game
{
    for (int i = 0; i < 5; i++)	// Initialize the Board
    {
        Board[0][i] = 'B';
        Board[1][i] = 'b';
        Board[2][i] = '_';
        Board[3][i] = 'w';
        Board[4][i] = 'W';
    }
}
void two_player ()
{
	fstream fobj;
	fobj.open("log_file.txt",ios::app|ios::out);
	fobj<<"\n*************************NEW MATCH BEGINS HERE******************************\nMatch played between two players!\n";
    fobj.close();
	initBoard();
    int firsttime=0,finale=3;
	do
	{
		if(firsttime==0)
		{
			cin.ignore();
			firsttime=1;
		}
       	move();
       	if(Flag!=0)
       	{
       		Whosemove=!Whosemove;
       		cout<<"Following entered string was validated:\n"<<s<<"\n";
       	}
       	else
       	{
       		cout<<"Enter input again!\n";
       	}
       	if(Bm==5)
       	{
       		fobj.open("log_file.txt",ios::app|ios::out);
       		cout<<"WHITE WINS!\n";
       		fobj<<"WHITE WINS!\n";
       		fobj.close();
       		finale=2;
       	}
       	else if(Wm==5)
       	{
       		fobj.open("log_file.txt",ios::app|ios::out);
       		cout<<"BLACK WINS\n";
       		fobj<<"BLACK WINS\n";
       		fobj.close();
       		finale=2;
       	}
       	if(finale!=2)
	    {
			finale=gameover();
	    }
   }while (finale==0);
	if (finale==3)
	{
		Winner();
	}
	cout<<"Game Terminated\n";
}

