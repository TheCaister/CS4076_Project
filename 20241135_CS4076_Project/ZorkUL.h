#ifndef ZORKUL_H_
#define ZORKUL_H_

#include "Parser.h"
#include "Room.h"
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
    static vector<Room*> allRooms;

    void printWelcome();
    static string printHelp();
    static bool goRoom(Command command);
    void createItems();
    static string printAllItems();
    static int money;
    const static int goalMoney = 1000;

    static Quantities *allQuantities;
    static Parser *parser;
    static Stack<Room*> recentRooms;

    static vector<Item*> itemsInInventory;
    static bool keysPresent[5];

public:


    ZorkUL();

    static void updateRoom(Room *room, MainWindow *window);
    static string processCommand(Command command, MainWindow *window);
    static string giveReward();
    static Room *getCurrentRoom();
    static vector<Room*> createRooms();
    static int findItemIndex(const string& item);
    static void changeMoney(int moneyAmount);
    static int getMoney();
    static void setAllRooms(vector<Room*> rooms);
    static vector<Room*> getAllRooms();
    static void deleteAll();
    static void setParser(Parser* parser);
    static Parser* getParser();
    static int getGoalMoney();

};

#endif ZORKUL_H_
