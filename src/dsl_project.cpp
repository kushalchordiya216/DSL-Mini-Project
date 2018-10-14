//============================================================================
// Name        : dsl_project.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "bot.h"
#include "two_player.h"
using namespace std;

int main()
{
	int choice;
	cout<<"******************************WELCOME TO CHAKRAVYUH*****************************\n\n\n";
	cout<<"Enter your choice!"<<endl<<"1.Single Player"<<endl<<"2.Two Player"<<endl;
	cin>>choice;
	switch(choice)
	{
	case 1:
		bot_play();
		break;
	case 2:
		two_player();
		break;
	}
	return 0;
}
