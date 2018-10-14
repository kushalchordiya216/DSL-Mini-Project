/*
 * bot.cpp
 *
 *  Created on: 15-Oct-2018
 *      Author: kushal
 */

#include "bot.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int flag,mobility,tempmob=0;
int a;
bool whosemove,countP,countM,elimFlag=0,reachhome=0,turn;
char X,Y;
int virginity;

char board[5][5] ;
struct Move
{
  int score=0,p=0;
  vector <vector<int>> moveset;
  bool whosemove,elimFlag=0;
};
struct game
{
	int r,c;
};
struct bMove
{
	game fsinput,fminput;
};

Move checkMove (Move );
int mValidation(game ,game ,Move);
void makeMove(game ,game ,Move &,Move &,char &);
void elim_package(game ,game ,bool ,int ,int ,Move);
int checkVerticalElimination(game ,game ,Move);
int checkHorizontalElimination(game ,game ,Move);
void Elimination(game ,game ,Move &,char &);
void colour_generalize(bool );
int negamax(int ,Move* ,int ,int);
bMove findBestMove();

void displayboard ()
{
    int i, j;
    cout << " \n \n    0 1 2 3 4\n";
    cout << "  -------------\n";
    for (i = 0; i < 5; i++)
    {
        cout << i << "|  ";
        for (j = 0; j < 5; j++)
	    {
	        cout << board[i][j] << ' ';
	    }
        cout << "\n";
    }
}

void colour_generalize (bool x)
{
      	if (x == 0)
      	{
          	X = 'W';			//X initialized to mantri
          	Y = 'w';			//Y initialized to pawn
        }
      	else
      	{
          	X = 'B';
          	Y = 'b';
        }
}

int winner()
{
	int sum1=0,sum2=0;
	for (int j = 0; j < 7; j++)
	{
	        if (board[0][j] == 'W')
		    {
		        sum1++;
		        cout<<"WHITE WINS\n";
		        a=0;
		        return 0;
	        }
	        if (board[4][j] == 'B')
		    {
		        sum2++;
		        cout<<"BLACK WINS\n";
		        a=1;
		        return 1;
	        }
	}
	return 3;
}

int mValidation (game sinput, game minput,Move currentobj)
{
    int d1, d2;
    bool x = currentobj.whosemove;
    d1 = minput.r - sinput.r;	// difference between initial and final row position
    d2 = minput.c - sinput.c;	//difference between initial and final cols position
    colour_generalize (x);
    if (board[minput.r][minput.c] == '_' && minput.c >= 0 && minput.c <= 4 && minput.r <= 4 && minput.c >= 0)	//check if final position is vacant
    {
        	if(board[sinput.r][sinput.c]==X)
	    	{
	        	if ((d1 == (2 * x - 1) || d1 == 0) && (d2 == (2 * x - 1) || d2 == -(2 * x - 1) || d2 == 0))	//mantri moves single space in any allowed direction
	        	{
	        		flag = 1;		//signifies valid non-eliminating move,turn passes to opponent
	        	}
	        	else if ((d1 == 2 * (2 * x - 1) && d2 == 0) || (d1 == 0 && (d2 == 2 || d2 == -2 )))	//moves two spaces, in an elimination move
	        	{
	            		elim_package (sinput, minput, x, d1, d2,currentobj);	//checks if elimination is possible and sets flag accordingly
	        	}
	        	else
	        	{
	            		flag = 0;
	        	}
	    	}
        	else if (board[sinput.r][sinput.c] == Y)	//moveset for pawn
        	{
	        	if ((d1 == (2 * x - 1) && d2 == (2 * x - 1)) || (d1 == (2 * x - 1) && d2 == -(2 * x - 1)))	//moves a single space
	        	{
	            		flag = 1;
	        	}
	        	else if ((d1 == 2 * (2 * x - 1) && d2 == 0) || (d1 == 0 && (d2 == 2 || d2 == -2 )))	//moves two spaces, in an elimination move
	        	{
	            		elim_package (sinput, minput, x, d1, d2,currentobj);
	        	}
	        	else
	        	{
	            		flag = 0;
	        	}
	    	}
    }
    else
    {
        flag = 0;
    }
    return flag;
}

void elim_package (game sinput, game minput, bool x, int d1, int d2,Move currentobj)
{
    if (d1 == 2 * (2 * x - 1) && d2 == 0)	//mantri moves two rows up, in an elimination move
    {
        flag = checkVerticalElimination (sinput, minput,currentobj);	//call function that checks conditions for eliminations
    }
    else if (d1 == 0 && (d2 == 2 * (2 * x - 1) || d2 == -2 * (2 * x - 1)))	//mantri moves horizontally in elimination move
    {
        flag = checkHorizontalElimination (sinput, minput,currentobj);
    }

}

int checkVerticalElimination (game sinput, game minput,Move currentobj)
{
    bool x = currentobj.whosemove;
    char t1 = board[(sinput.r + minput.r) / 2][(sinput.c + minput.c) / 2];	//target piece  //changing value of X and Y temporarily to detect correct target
    colour_generalize (!x);
    if (t1 == X || t1 == Y)
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

int checkHorizontalElimination (game sinput, game minput,Move currentobj)
{
    bool x = currentobj.whosemove;
    char t1 = board[(sinput.r + minput.r) / 2][(sinput.c + minput.c) / 2];
    colour_generalize (!x);	//changing value of X and Y temporarily to detect correct target
    if ((t1 == X || t1 == Y) && sinput.r!=4 && sinput.r!=0)	//check if neighbouring squares and piece being attacked
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

Move checkMove (Move currentobj)
{
	bool x=currentobj.whosemove;
	mobility=0;
	Move obj1;
	game gsinput, gminput;
	colour_generalize(x);
	for (int i = 0; i < 5; i++)
    {
	   	for (int j = 0; j < 5; j++)
        {
	       	if (board[i][j] == X || board[i][j] == Y)
	       	{
               	gsinput.r = i;
		       	gsinput.c = j;
		       	for (int k = 0; k <= 2; k++)
		       	{
		           	for (int l = -2; l <= 2; l++)
		           	{
			          	if (x == 1)
			           	{
			           		gminput.r = i + k;
			           	}
			           	else
			            {
			               	gminput.r = i - k;
		            	}
		            	gminput.c = j + l;
		            	if (gminput.c <= 4 && gminput.c >= 0 && gminput.r <= 4 && gminput.r >= 0)
			           	{
			               	flag = mValidation (gsinput, gminput,currentobj);
			               	colour_generalize(x);
			               	if (flag == 1 || flag == 2)
			               	{
			               		mobility++;
			               		vector<int> myrow;
			               		myrow.push_back(gsinput.r);
			               		myrow.push_back(gsinput.c);
			               		myrow.push_back(gminput.r);
			               		myrow.push_back(gminput.c);
			               		obj1.moveset.push_back(myrow);
				               	obj1.p++;
				            }
			         	 }
		           	 }
		       	 }
	       	 }
    	 }
     }
     return obj1;
}

void Elimination (game sinput, game minput,Move &currentobj,char &T)
{
	currentobj.elimFlag = 1;
    bool x=!currentobj.whosemove;
    colour_generalize(x);
    char t1 = board[(sinput.r + minput.r) / 2][(sinput.c + minput.c) / 2];
    if (t1 == X)
    {
    	countM=1;			//increment count for dead mantri
    }
    else
    {
       	countP=1;			//increment count for dead pawn
    }
    T=board[(sinput.r + minput.r) / 2][(sinput.c + minput.c) / 2];
    board[(sinput.r + minput.r) / 2][(sinput.c + minput.c) / 2]='_';
}

void makeMove (game sinput, game minput ,Move &currentobj,Move &nextobj,char &T)
{
	bool x=currentobj.whosemove;
	colour_generalize(x);
	int d1=minput.r-sinput.r;
	int d2=minput.c-sinput.c;
    if (d1 == (2*x-1)|| d2==1 || d2==-1)
   	{
        board[minput.r][minput.c] = board[sinput.r][sinput.c];
        board[sinput.r][sinput.c] = '_';
    }
    else if (d1 == 2*(2*x-1) || d2==2 || d2==-2)
    {
        board[minput.r][minput.c] = board[sinput.r][sinput.c];
        board[sinput.r][sinput.c] = '_';
        Elimination(sinput, minput, currentobj, T);
    }
    colour_generalize(x);
	if(minput.r==4*x && sinput.r!=4*x && board[minput.r][minput.c]==X)
	{
		reachhome = 1;
	}
	nextobj.whosemove=!currentobj.whosemove;
}
void undoMove(game sinput,game minput,char &T,Move &currentobj)
{
	board[sinput.r][sinput.c]=board[minput.r][minput.c];
	board[minput.r][minput.c]='_';
	if(currentobj.elimFlag==1)
	{
		board[(sinput.r+minput.r)/2][(sinput.c+minput.c)/2]=T;
		currentobj.elimFlag=0;
	}
}
void makefinalMove(game sinput, game minput, bool x)
{
		colour_generalize(x);
		int d1=minput.r-sinput.r;
		int d2=minput.c-sinput.c;
	    if (d1 == (2*x-1)|| d2==1 || d2==-1)
	   	{
	        board[minput.r][minput.c] = board[sinput.r][sinput.c];
	        board[sinput.r][sinput.c] = '_';
	    }
	    else if (d1 == 2*(2*x-1) || d2==2 || d2==-2)
	    {
	        board[minput.r][minput.c] = board[sinput.r][sinput.c];
	        board[sinput.r][sinput.c] = '_';
	        board[(sinput.r+minput.r)/2][(sinput.c+minput.c)/2] = '_';
	    }
}


int evaluate (game sinput, game minput,Move currentobj,int depth)
{
	bool x=currentobj.whosemove;
	int result=0;
    if (currentobj.elimFlag == 1)
    {
	    if (countM == 1)
	    {
	        	result = result+(500*(2*x-1) + 50*(4*x-(sinput.r+minput.r)/2))*(8-depth);
	    }
	    else if (countP == 1)
	    {
	    	result = result + (100*(2*x-1) + 10*(4*x-(sinput.r+minput.r)/2))*(8-depth);
	    }
    }
    if (reachhome == 1)
    {
    	result =  result + 10000*(2*x-1)*(8-depth);
    }
    int mobility_factor = mobility - tempmob;
    result =  result + mobility_factor*2*(2*x-1);
    if(x==1)
    {
    	result =  result + 5*(minput.r)*(2*x-1);
    }
    else
    {
    	result =  result + 5*(4-minput.r)*(2*x-1);
    }
    countM=0,countP=0;
    return result;
}

int negamax (int depth,Move obj[],int alpha,int beta)
{

	int best;
	game sinput,minput;
	Move currentobj = *obj;
	Move nextobj = *(obj+1);
	Move prevobj = *(obj-1);
	int score=0;
	best=200000*(1-2*currentobj.whosemove);
	bool x=currentobj.whosemove;
	if(reachhome==1)
	{
	    reachhome=0;
	    return currentobj.score;
	}
    if(depth==5)
    {
    	*(obj-1)=prevobj;
    	return currentobj.score;
    }
    int tempscore = currentobj.score;
    currentobj = checkMove (currentobj);
    currentobj.score = tempscore;
    currentobj.whosemove=x;
    (*obj)=currentobj;
    for (int i = 0; i < currentobj.p; i++)
    {
    	sinput.r = currentobj.moveset[i][0];
    	sinput.c = currentobj.moveset[i][1];
    	minput.r = currentobj.moveset[i][2];
    	minput.c = currentobj.moveset[i][3];
    	char T=' ';
    	makeMove (sinput, minput,currentobj,nextobj,T);
    	score = evaluate (sinput, minput,currentobj,depth);
    	nextobj.score = currentobj.score+score;
    	tempmob = mobility;
    	*(obj+1)=nextobj;
    	if(currentobj.whosemove)
    	{
    		best = max (best, negamax (depth + 1,obj+1,alpha,beta));
    		alpha = max (alpha,best);
    		if(beta<=alpha)
    		{
    			undoMove(sinput,minput,T,currentobj);
    			break;
    		}
    	}
    	else
    	{
    		best = min (best, negamax (depth + 1,obj+1,alpha,beta));
    		beta = min (beta,best);
    		if(beta<=alpha)
    		{
    			undoMove(sinput,minput,T,currentobj);
    			break;
    		}
    	}
    	undoMove(sinput,minput,T,currentobj);
    }
    return best;
}


bMove findBestMove ()
{
	Move obj[10];
    bMove bestMove;
    obj[0].whosemove=turn;
    obj[0] = checkMove (obj[0]);
    obj[0].whosemove=turn;
    game sinput,minput;
    int bestVal;
    if(turn==0)
    {
    	bestVal = 250000;
    }
    else
    {
    	bestVal = -250000;
    }
	int i;
	for (i = 0; i<obj[0].p;i++)
	{
		sinput.r = obj[0].moveset[i][0];
	    sinput.c = obj[0].moveset[i][1];
	    minput.r = obj[0].moveset[i][2];
	    minput.c = obj[0].moveset[i][3];
	    char T;
	    makeMove (sinput, minput,obj[0],obj[1],T);
	    obj[1].score = evaluate(sinput,minput,obj[0],-1);
	    virginity=0;
		int moveVal = negamax (0,obj+1,-200000,200000);
		undoMove(sinput,minput,T,obj[0]);
		if(turn==1)
		{
			if (moveVal > bestVal)
			{
				bestMove.fsinput.r =sinput.r;
				bestMove.fsinput.c =sinput.c;
				bestMove.fminput.r =minput.r;
				bestMove.fminput.c =minput.c;
				bestVal = moveVal;
			}
		}
		else if(turn==0)
		{
			if (moveVal < bestVal)
			{
				bestMove.fsinput.r =sinput.r;
				bestMove.fsinput.c =sinput.c;
				bestMove.fminput.r =minput.r;
				bestMove.fminput.c =minput.c;
				bestVal = moveVal;
			}
		}
	}
    return bestMove;
}

void initboard ()
{
    for (int i = 0; i < 5; i++)
    {
        board[0][i] = 'B';
        board[1][i] = 'b';
        board[2][i] = '_';
        board[3][i] = 'w';
        board[4][i] = 'W';
    }
}
// Driver code
void bot_play ()
{
	initboard();
	displayboard();
	game sinput,minput;
	string input,binput;
	fstream fobj;
	int preference;
	fobj.open("log_file.txt",ios::out|ios::app);
	fobj<<"\n*************************NEW MATCH BEGINS HERE******************************\nMatch played against Bot!\n";
	int firsttime=0,finale=3;
	cout<<"Would you like to be Player 1 or Player 2[1/2]\n";
	cin>>preference;
	if(preference==1)
	{
		whosemove=0;
		turn=0;
	}
	else if(preference==2)
	{
		whosemove=1;
		turn=1;
	}
	do
	{
		if(whosemove)
		{
			bMove bestMove = findBestMove();
			cout<<bestMove.fsinput.r<<" "<<bestMove.fsinput.c<<" "<<bestMove.fminput.r<<" "<<bestMove.fminput.c<<endl;
			makefinalMove(bestMove.fsinput,bestMove.fminput,turn);
			whosemove=!whosemove;
			displayboard();
			fobj<<"bot move : "<<bestMove.fsinput.r<<" "<<bestMove.fsinput.c<<" "<<bestMove.fminput.r<<" "<<bestMove.fminput.c<<endl;
		}
		else
		{
			if(firsttime==0)
			{
				cin.ignore();
			}
			firsttime=1;
			getline(cin,input);
			sinput.r=input[0]-48;
			sinput.c=input[2]-48;
			minput.r=input[4]-48;
			minput.c=input[6]-48;
			makefinalMove(sinput,minput,!turn);
			displayboard();
			whosemove=!whosemove;
			fobj<<"player move : "<<input<<endl;
		}
		finale=winner();
	} while(finale==3);
	if(finale==preference-1)
	{
		fobj<<"Bot wins!"<<"\n";
	}
	else
	{
		fobj<<"Player wins!"<<"\n";
	}
}
