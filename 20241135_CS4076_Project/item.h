#ifndef ITEM_H_
#define ITEM_H_

#include <map>
#include <iostream>
using namespace std;

class ItemProperties{
public:
    enum typeOfItem : int {HINT, HEALTH};
};

class Item : public ItemProperties{
private:
    string description;
    string longDescription;

    int weightGrams;
    int value;
    bool sellable;

    string usedDialogue;
    typeOfItem itemType;

public:
    Item (string description, int inWeight, int inValue);
    Item (string description, string usedDialogue, typeOfItem typeOfItem);
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
