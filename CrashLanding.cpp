#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>


int difficulty = 1;					//Setup difficulty
const char* CSI = "\033[";			//Setup escape
const uint8_t width = 40;				//Setup field width
const uint8_t height = 29;				//Setup field height



void redColor()								//function to set output to red color
{
	std::cout << CSI << 31 << "m";
}



void yellowColor()							//function to set output to yellow color
{
	std::cout << CSI << 33 << "m";
}



void blueColor()							//function to set output to blue color
{
	std::cout << CSI << 36 << "m";
}



void clearColor()							//function to clear output color
{
	std::cout << CSI << 0 << "m";
}


void clearScreen()							//function to clear screen
{
	std::cout << CSI << "2J" << std::endl;
}



int selectDifficulty()			//Difficulty selection function start
{
	clearScreen();																							//clear screen
	std::cout << CSI << 1 << ";" << 1 << "H";
	int diffSel = 0;																						//difficulty selection int
	do {
		std::cout << "Difficulty is " << difficulty << std::endl;											//Display difficulty
		std::cout << "Select Difficulty\n1.Plot\n2.Arcade\n3.Dark Souls\n\n4.Exit" << std::endl;			//Show difficulty selection menu
		std::cin >> diffSel;
		std::cin.ignore();
		if (diffSel == 4) { return 0; }
	} while (diffSel != 1 && diffSel != 2 && diffSel != 3);													//Check if difficulty selection is correct
	difficulty = diffSel;																					//Set difficulty
	std::cout << "Difficulty is " << difficulty << std::endl;												//Display difficulty
	std::this_thread::sleep_for(std::chrono::milliseconds(500));											//Wait to display difficulty		
	return diffSel;																							//Return difficulty
}							//Difficulty selection function end



void showHighScores()
{
	clearScreen();																	//clear screen

}



void generateCave(bool cave[height][width])								//cave generation function
{
	static int genStartCache = 5, genEndCache = 10, counter = 0;		//create variables for cache
	int genStart = 0, genEnd = 0; 							//create variables for generation

	for (int i = 0; i < width; i++)					//set zeroes to whole row
	{
		cave[0][i] = 0;
	}
	cave[0][0] = 1;						//set 1s to cave sides
	cave[0][width - 1] = 1;				//

	if (counter == 0) {
		for (int i = 0; i < width; i++)	//set ones to whole row
		{
			cave[0][i] = 1;
		}

		do {																						//generate start and end of divide
			genStart = (rand() % (width - 1)), genEnd = (genStart + (9 - rand() % difficulty));
		} while (genStart > genEndCache or genEnd < genStartCache or genStart>width - 3 or genEnd < 3);					//Generate again if there is no way to get through

		for (int i = genStart; i <= genEnd; i++)
		{
			cave[0][i] = 0;
		}

		genStartCache = genStart, genEndCache = genEnd;							//save previous vars to cache vars
	}
	counter == (5 - difficulty) ? counter = 0 : counter++;					//skip rows according to difficulty
}



void moveRenderDown(bool cave[height][width])						//move each cave row down for 1 row
{
	for (int line = height - 1; line >= 0; line--)					//cycle to go over each row from below
	{
		for (int elt = 0; elt < width; elt++)						//cycle to go over each element and move it from upper row to lower row
		{
			cave[line][elt] = cave[line - 1][elt];					//copy element with the same index
		}
	}
}



void renderLava()										// render lava at the bottom of cave
{
	std::cout << CSI << (int)height << ";" << 0 << "H";				//Set cursor coordinates to lower part
	redColor();												//Set color to red
	for (int row = 0; row < width; row++)					// render lava at the bottom
	{
		std::cout << "^";									// render lava piece
	}
	clearColor();										//turn off red color
}



void renderCave(bool cave[height][width])
{
	moveRenderDown(cave);									//move all cave array down for 1 row
	generateCave(cave);										//generate top row of cave

	for (int line = 0; line < height; line++)				//display cave on console screen
	{
		for (int row = 0; row < width; row++)
		{
			cave[line][row] ? std::cout << "*" : std::cout << " ";		//if console symbol is 1 - display "*", if 0 - display space
		} std::cout << std::endl;
	}


	renderLava();
}



void renderShip(int* xCoord, int* yCoord)					//render ship according to coordinates
{
	char key = 'x', menu = 0;
	if (_kbhit()) {
		key = _getch();
		if (key == 'w') {
			if (*yCoord > 3) {
				*yCoord += -1;
			}
			blueColor();
			std::cout << CSI << *yCoord << ";" << *xCoord << "H" << "0" << std::endl;
			yellowColor();
			std::cout << CSI << *yCoord + 1 << ";" << *xCoord << "H" << "V" << std::endl;
			clearColor();

		}

		else if (key == 's') {
			*yCoord += 1;
			yellowColor();
			std::cout << CSI << *yCoord - 1 << ";" << *xCoord << "H" << "^" << std::endl;
			blueColor();
			std::cout << CSI << *yCoord << ";" << *xCoord << "H" << "0" << std::endl;
			clearColor();
		}

		else if (key == 'a') {
			if (*xCoord > 2) {
				*xCoord += -2;
			}
			blueColor();
			std::cout << CSI << *yCoord << ";" << *xCoord << "H" << "0" << std::endl;
			yellowColor();
			std::cout << CSI << *yCoord << ";" << *xCoord + 1 << "H" << ">" << std::endl;
			clearColor();

		}

		else if (key == 'd') {
			if (*xCoord < width - 2) {
				*xCoord += 2;
			}
			blueColor();
			std::cout << CSI << *yCoord << ";" << *xCoord << "H" << "0" << std::endl;
			yellowColor();
			std::cout << CSI << *yCoord << ";" << *xCoord - 1 << "H" << "<" << std::endl;
			clearColor();

		}

		else {
			blueColor();
			std::cout << CSI << *yCoord << ";" << *xCoord << "H" << "0" << std::endl;
			clearColor();
		}
		//*yCoord += 1;
	}
	else {
		blueColor();
		std::cout << CSI << *yCoord << ";" << *xCoord << "H" << "0" << std::endl;
		clearColor();
	}
	//*yCoord += 1;
}



bool checkCollision(int* xCoord, int* yCoord, bool cave[height][width])				//check ship collisison according to coordinates and cave array
{
	if ((cave[*yCoord - 1][*xCoord - 1]) or *yCoord >= height)									//check ship coordinates and cave array to see if there is collision, also check if ship hit the bottom
	{
		return 1;
	}
	return 0;
}



int game()
{
	int score = 0;
	int shipX = width / 2, shipY = height / 2;				//create initial ship coordinates
	int* xCoord = &shipX;
	int* yCoord = &shipY;
	bool caveDisplay[height][width] = { 0 };			//create cave render array

	while (1)												//Game cycle
	{
		clearScreen();													//clear screen
		std::cout << CSI << "?25l";										//Remove cursor from screen
		std::cout << CSI << 0 << ";" << 0 << "H";						//Set cursor to coordinates 1;1

		renderCave(caveDisplay);										//render cave function
		renderShip(xCoord, yCoord);										//render ship on coordinates
		std::cout << CSI << height + 1 << ";" << 0 << "H" << "Score: " << ++score;		//render score

		if (checkCollision(xCoord, yCoord, caveDisplay))		//check collisison function
		{
			std::cout << CSI << *yCoord << ";" << *xCoord << "H";			//setup display coordinates
			redColor();
			std::cout << "You   died";													//show red You died sign
			std::cout << CSI << *yCoord + 1 << ";" << *xCoord << "H" << "Score: " << score;	//show final score
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));				//demonstrate sign for 3 seconds
			clearColor();																//turn off red color
			return 13;																	//return 13;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));					//wait 800 ms for next cycle
	}
	return 0;
}



int main()
{
	int menu = 0;					//Create main menu selector

	do
	{
		std::cout << CSI << "2J" << std::endl;																	//clear screen
		std::cout << CSI << 1 << ";" << 1 << "H";						//Set cursor to coordinates 1;1
		std::cout << "Crash Landing...\n1.New Game\n2.Select Difficulty\n3.High scores\n\n4.Exit" << std::endl;		//Show main menu selection
		std::cin >> menu;				//enter menu selection
		std::cin.ignore();				//ignore Enter key to avoid further input problems
		switch (menu)					//switch for menu
		{
		case 1:						//selection 1 - New game
		{
			game();					//start game
			break;
		}
		case 2: 					//selection 2 - Select Difficulty
		{
			selectDifficulty();		//difficulty selection function
			break;
		}
		case 3: 					//selection 3 - Display high scores
		{
			showHighScores();		//Show high scores function
			break;
		}
		case 4: 					//selection 4 - Exit
		{
			return 0;				//Exit the program
			break;
		}
		default: 					//default selection - wrong input message
		{
			std::cout << "Wrong input, try again" << std::endl;
			break;
		}
		}
	} while (menu != '4');

}

