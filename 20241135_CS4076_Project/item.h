#ifndef ITEM_H_
#define ITEM_H_

#include <map>
#include <iostream>
using namespace std;

class ItemProperties{
public:
    enum typeOfItem : int {HINT = 1, HEALTH = 2, LUCK = 4};
};

class Item;
class LuckItem;

namespace useItemFunctions{
string useItemDefault(Item*);

string useLuckItemDefault(LuckItem*);

string useItemRaffle(LuckItem*);
}

class Item : public ItemProperties{
    friend class ZorkUL;
protected:
    string description;

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
          string usedDialogue="", typeOfItem typeOfItem=HINT, int value=0);
    Item (string description, string usedDialogue);
    Item (string description);

    virtual ~Item();

    // Copy constructor
    Item(const Item& other);

    string getShortDescription();
    //string getLongDescription();
    int getWeight();
    void setWeight(int weightGrams);
    int getValue();
    void setValue(int value);
    string getUsedDialogue();
    typeOfItem getTypeOfItem();
};

class ConsumableItem{
public:
    virtual void consumeItem() = 0;
};

// Items that utilize some kind of luck. Each of these will have
// a corresponding "success chance"
class LuckItem : public Item{
private:
    float successChance;
public:
    bool tryChance();

    string (*useFunc)(LuckItem*);

    LuckItem(string (*useFunc)(LuckItem*)=&(useItemFunctions::useLuckItemDefault), string description="",
             string usedDialogue="", typeOfItem typeOfItem=LUCK, float successChance=0.5, int value=0);
};

class Raffle : public ConsumableItem, public LuckItem{

public:
    Raffle(int value=0, float successChance=0.5, string description="Raffle",
           string usedDialogue="You have tried to use raffle.", typeOfItem typeOfItem=LUCK);

    virtual void consumeItem() override;
};

#endif /*ITEM_H_*/
