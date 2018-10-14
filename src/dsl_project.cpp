//============================================================================
// Name        : dsl_project.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "single_player.h"
#include "bot.h"
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
		single_player();
		break;
	case 2:
		bot_play();
	}
	return 0;
}
