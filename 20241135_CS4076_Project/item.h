#ifndef ITEM_H_
#define ITEM_H_

#include <map>
#include <string>
#include <iostream>
using namespace std;

class Item {
private:
    string description;
    string longDescription;

    int weightGrams;
    float value;
    bool weaponCheck;

    string usedDialogue;

public:
    Item (string description, int inWeight, float inValue);
    Item (string description, string usedDialogue);
    Item (string description);

    // Copy constructor
    Item(const Item& other);

    string getShortDescription();
    string getLongDescription();
    int getWeight();
    void setWeight(int weightGrams);
    float getValue();
    void setValue(float value);
    string getUsedDialogue();
    //int getWeaponCheck();
    //void setWeaponCheck(int weaponCheck);
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
public:
    int value;
};

#endif /*ITEM_H_*/
