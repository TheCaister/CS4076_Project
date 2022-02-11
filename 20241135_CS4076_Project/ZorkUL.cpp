#include <iostream>
#include <cstdlib>
#include "mainwindow.h"

#include <QApplication>

#include <QTextStream>
#include <QDebug>

using namespace std;
#include "ZorkUL.h"
// git test
int main(int argc, char *argv[]) {
    // For printing stuff out in the output pane and debugging
    QTextStream out(stdout);
    //ZorkUL temp;
    //temp.play();

    WordleEngine *worldleEngine = new WordleEngine();
    QApplication a(argc, argv);
    MainWindow w;


    w.show();
    w.clearConsole();
    for (string s : worldleEngine->getAllWords()){
        w.addToConsole(s);
    }

    delete worldleEngine;
    return a.exec();
}

ZorkUL::ZorkUL() {
    createRooms();
}

Room *a, *b, *c, *d, *e, *f, *g, *h, *i, *newRoom;

void ZorkUL::createRooms()  {


    // Adding all rooms
    a = new Room("a");
    a->addItem(new Item("x", 1, 11));
    a->addItem(new Item("y", 2, 22));
    b = new Room("b");
    b->addItem(new Item("xx", 3, 33));
    b->addItem(new Item("yy", 4, 44));
    c = new Room("c");
    d = new Room("d");
    e = new Room("e");
    f = new Room("f");
    g = new Room("g");
    h = new Room("h");
    i = new Room("i");
    newRoom = new Room("New Room");

    // Setting exits for each room
    //             (N, E, S, W)
    a->setExits(f, b, d, c);
    b->setExits(newRoom, NULL, NULL, a);
    c->setExits(NULL, a, NULL, NULL);
    d->setExits(a, e, NULL, i);
    e->setExits(NULL, NULL, NULL, d);
    f->setExits(NULL, g, a, h);
    g->setExits(NULL, NULL, NULL, f);
    h->setExits(NULL, f, NULL, NULL);
    i->setExits(NULL, d, NULL, NULL);

    newRoom->setExits(NULL, NULL, b, NULL);

    // Start off at this room.
    currentRoom = a;
}

/**
 *  Main play routine.  Loops until end of play.
 */
void ZorkUL::play() {
    printWelcome();

    // Enter the main command loop.  Here we repeatedly read commands and
    // execute them until the ZorkUL game is over.

    bool finished = false;
    while (!finished) {
        // Create pointer to command and give it a command.
        Command* command = parser.getCommand();
        // Pass dereferenced command and check for end of game.
        finished = processCommand(*command);
        // Free the memory allocated by "parser.getCommand()"
        //   with ("return new Command(...)")
        delete command;
    }
    cout << endl;
    cout << "end" << endl;
}

void ZorkUL::printWelcome() {
    cout << "start"<< endl;
    cout << "info for help"<< endl;
    cout << endl;
    cout << currentRoom->longDescription() << endl;
}

/**
 * Given a command, process (that is: execute) the command.
 * If this command ends the ZorkUL game, true is returned, otherwise false is
 * returned.
 */
bool ZorkUL::processCommand(Command command) {
    cout << command.getCommandWord() << endl;

    if (command.isUnknown()) {
        cout << "invalid input"<< endl;
        return false;
    }

    string commandWord = command.getCommandWord();
    if (commandWord.compare("info") == 0)
        printHelp();

    else if (commandWord.compare("map") == 0)
    {
        cout << "[h] --- [f] --- [g]" << endl;
        cout << "         |         " << endl;
        cout << "         |         " << endl;
        cout << "[c] --- [a] --- [b]" << endl;
        cout << "         |         " << endl;
        cout << "         |         " << endl;
        cout << "[i] --- [d] --- [e]" << endl;
    }

    else if (commandWord.compare("go") == 0)
        goRoom(command);

    // Attempting to write teleport code
    else if(commandWord.compare("Teleport random") == 0){
        cout << "Teleporting to random room..." << endl;

        Room rooms [10] = {*a, *b, *c, *d, *e, *f, *g, *h, *i, *newRoom};
        int randomIndex = rand() % 10;
        currentRoom = &rooms[randomIndex];
        cout << currentRoom->longDescription() << endl;

    }

    // Attempting to write teleport code
    else if(commandWord.compare("Teleport") == 0){
        cout << "Which room would you like to teleport to?" << endl;
        int choice;

        Room rooms [10] = {*a, *b, *c, *d, *e, *f, *g, *h, *i, *newRoom};
        int numberOfRooms = sizeof rooms/ sizeof (rooms[0]);

        cout << "Amount of rooms: " << numberOfRooms << endl;

        for(int i = 0; i < numberOfRooms; i++){
            Room iterRoom = rooms[i];
            cout << i << ": " << iterRoom.shortDescription() << endl;
        }

        cout << "Enter selection here: ";
        cin >> choice;

        cout << "You have chosen: " << choice << endl;
        currentRoom = &rooms[choice];
        cout << currentRoom->longDescription() << endl;

    }

    else if (commandWord.compare("take") == 0)
    {
        if (!command.hasSecondWord()) {
            cout << "incomplete input"<< endl;
        }
        else
            if (command.hasSecondWord()) {
                cout << "you're trying to take " + command.getSecondWord() << endl;
                int location = currentRoom->isItemInRoom(command.getSecondWord());
                if (location  < 0 )
                    cout << "item is not in room" << endl;
                else
                    cout << "item is in room" << endl;
                cout << "index number " << + location << endl;
                cout << endl;
                cout << currentRoom->longDescription() << endl;
            }
    }

    else if (commandWord.compare("put") == 0)
    {

    }
    /*
    {
    if (!command.hasSecondWord()) {
        cout << "incomplete input"<< endl;
        }
        else
            if (command.hasSecondWord()) {
            cout << "you're adding " + command.getSecondWord() << endl;
            itemsInRoom.push_Back;
        }
    }
*/
    else if (commandWord.compare("quit") == 0) {
        if (command.hasSecondWord())
            cout << "overdefined input"<< endl;
        else
            return true; /**signal to quit*/
    }
    return false;
}
/** COMMANDS **/
void ZorkUL::printHelp() {
    cout << "valid inputs are; " << endl;
    parser.showCommands();

}

void ZorkUL::goRoom(Command command) {
    if (!command.hasSecondWord()) {
        cout << "incomplete input"<< endl;
        return;
    }

    string direction = command.getSecondWord();

    // Try to leave current room.
    Room* nextRoom = currentRoom->nextRoom(direction);

    if (nextRoom == NULL)
        cout << "underdefined input"<< endl;
    else {
        currentRoom = nextRoom;
        cout << currentRoom->longDescription() << endl;
    }
}

string ZorkUL::go(string direction) {
    //Make the direction lowercase
    //transform(direction.begin(), direction.end(), direction.begin(),:: tolower);
    //Move to the next room
    Room* nextRoom = currentRoom->nextRoom(direction);
    if (nextRoom == NULL)
        return("direction null");
    else
    {
        currentRoom = nextRoom;
        return currentRoom->longDescription();
    }
}
