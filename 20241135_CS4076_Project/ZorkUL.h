#ifndef ZORKUL_H_
#define ZORKUL_H_

#include "Command.h"
#include "Parser.h"
#include "Room.h"
#include "item.h"
#include <iostream>
#include <string>
#include "WordleEngine.h"
#include "mainwindow.h"

using namespace std;

class ZorkUL {
private:
    static Room *currentRoom;
    void printWelcome();
    static void printHelp();
    static bool goRoom(Command command);
    void createItems();
    void displayItems();

public:
	ZorkUL();
	void play();
	string go(string direction);
    static void updateRoom(Room *room, MainWindow *window);
    static bool processCommand(Command command);
    static Parser *parser;
    static Room *getCurrentRoom();
    static vector<Room*> createRooms();
};

#endif ZORKUL_H_
