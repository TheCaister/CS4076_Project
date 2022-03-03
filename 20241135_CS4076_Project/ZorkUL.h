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


struct Quantities{
        unsigned int KeysPresent : 4;
        unsigned int Bombs : 4;
};

class ZorkUL {
private:
    static Room *currentRoom;

    void printWelcome();
    static string printHelp();
    static bool goRoom(Command command);
    void createItems();
    void displayItems();



public:
    static Quantities *allQuantities;
    static Parser *parser;

    static vector<Item*> itemsInInventory;
    static bool keysPresent[5];

    ZorkUL();

    void play();
    string go(string direction);
    static void updateRoom(Room *room, MainWindow *window);
    static string processCommand(Command command, MainWindow *window);
    static Room *getCurrentRoom();
    static vector<Room*> createRooms();
    static void confirmKey(int index);
    static void addItem(Item item);
    static void useItem(Item& item);
    static int findItemIndex(const string& item);

};

#endif ZORKUL_H_
