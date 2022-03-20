#ifndef DIALOGUES_H
#define DIALOGUES_H

#include <string>
using std::string;

// Struct containing dialogues and some printing functions.
struct Dialogues{
    static string printCurrentRoom(string& description);
    static string printAttemptsLeft(int& attemptsLeft);

    const static string welcome;
    const static string inputError;
    const static string noMoreRooms;
    const static string welcomeWordle;
    const static string wordleIncorrectInput;
    const static string wordleSuccess;
    const static string wordleOutOfAttempts;
};

namespace RoomDialogues{
    const string cityCentre = "Main City Centre. Blinding lights violate your eyes from"
                               " every direction. Keep an eye out for tourist traps.";

    const string sewers = "The city sewers. Hidden from the glamour of life above-ground, you can't help"
                          " but gag at the foul odours emanating from this area. Maybe it's best that you leave soon...";

    const string chineseRestaurant = "The local Chinese restaurant. Your stomach rumbles as the scent of"
                                     " fried pork and soy sauce emanates from the establishment.";

    const string cafe = "You arrive at one of the popular cafes in town. Nobody bats an eye when you entered."
                        " They are too busy either lost in their conversations or working with headphones on.";

    const string cave = "You head out further away from the big city and ended up in a cave."
                        " The walls are damp and there's not a soul in sight.";

    const string clawMachine = "As you wander about, you see a claw machine on the side of"
                               " the street. Maybe you can try to grab something to sell later on?";

    const string conveyorSushi = "You arrive at a restaurant that serves sushi on convenient conveyor belts."
                                 " The prices here aren't exorbitant so maybe you can treat yourself every now and again?";

    const string livelyAlley = "As you head further down the alley, you come across what seems to be"
                               " a gang of people moping about. This could be their hangout spot. Provoking"
                               " them might not be a good idea.";

    const string noodleStall = "You see a small noodle stall, operated by a lone man. \"You"
                               " must have had it rough, huh? I feel sorry for kids like you.\"";

    const string underBridge = "You cross under a bridge. There doesn't seem to be anyone here.";


}

namespace ItemDialogues{
    const string frog = "The frog stares at you, eyes gleaming with... passion?";

    const string weirdMagazine = "A dubious magazine that you picked up from the street. Upon further inspection,\n"
                                 " you see that this magazine contains copious amounts of photos with\n"
                                 " people on their knees, pleading for forgiveness. Apparently, there are\n"
                                 " folks out there who are really into apologies.";

    const string pen = "A pen that you got from a man in the train. It smells kinda funky and the tip seems slightly chewed on.";

}

namespace InteractDialogues{
    const string cafeCompleteInteract = "\"Yo! Thanks again for the pen, broski!\"";

    const string cafeNotCompleteInteract = "\"Hmm... what to do... if only I had another pen...\"";
}

namespace CompletionDialogues{
    const string peiComplete = "You set the frog free into Pei Street. It"
                               " looks back at you with moist eyes, overcome with sadness."
                               " Maybe, just maybe, you two might meet again...\n";

    const string finalGoalComplete = "You present the money to your father. He nods"
                                     " and you go home together. It looks like you'll"
                                     " be able to eat dinner tonight.\n Congratulations!"
                                     " you have beaten this game! Feel free to quit or if you"
                                     " want, you can also play the Wordle challenges again!";

    const string finalGoalNotComplete = "Your father is sitting on the bench, scowling at you."
                                        " He knows that you haven't earned enough money yet. Better"
                                        " get back to it.";

    const string cafeComplete = "You give the pen to the student. A big grin appears on his face. \"Thanks man! I really appreciate"
                                " you helping a brotha out! Here, take this!\"";

    const string cafeNotComplete = "As you search for free samples and leftovers, you come across someone"
                                   " that seems to be a student. \"Hey man,\" he said. \"I was tryna take down some"
                                   " notes but my pen's all out. Going to the store is gonna be such a pain... Say,"
                                   " would you have a spare pen on you? I got something nice for you in return!\"";
}

#endif // DIALOGUES_H
