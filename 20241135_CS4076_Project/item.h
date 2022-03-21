#ifndef ITEM_H_
#define ITEM_H_

#include <map>
#include <iostream>
using namespace std;

class ItemProperties{
public:
    enum typeOfItem : int {HINT = 1, HEALTH = 2, RAFFLE = 4};
};

class Item;

namespace useItemFunctions{
string useItemDefault(Item*);

string useItemRaffle(Item*);
}

class Item : public ItemProperties{
    friend class ZorkUL;
protected:
    string description;
    string longDescription;

    int weightGrams;
    int value;
    bool sellable;

    string usedDialogue;
    typeOfItem itemType;

    string (*useFunc)(Item*);

public:
    Item (string description, int inWeight, int inValue);
    Item (string description, string usedDialogue, typeOfItem typeOfItem);
    Item (string (*useFunc)(Item*)=&(useItemFunctions::useItemDefault), string description="",
          string usedDialogue="", typeOfItem typeOfItem=HINT);
    Item (string description, string usedDialogue);
    Item (string description);

    // Copy constructor
    Item(const Item& other);

    string getShortDescription();
    string getLongDescription();
    int getWeight();
    void setWeight(int weightGrams);
    float getValue();
    void setValue(int value);
    string getUsedDialogue();
    typeOfItem getTypeOfItem();
};

class ConsumableItem : public Item{
public:
    void consumeItem();
};

// Items that utilize some kind of luck. Each of these will have
// a corresponding "success chance"
class LuckItem : public Item{
private:
    float successChance;
public:
    bool tryChance();
};

class Raffle : public ConsumableItem, public LuckItem{
};

#endif /*ITEM_H_*/
