#include "user.h"
#include <algorithm>
#include <iostream>
using namespace std;

// Adds content to viewer watching history
void User::addToHistory(CID_T contentID)
{
    history.push_back(contentID);
}

// Returns bool of if user has watched a given piece of content
bool User::haveWatched(CID_T contentID)
{
    auto my_iterator = std::find(history.begin(), history.end(), contentID);
    return my_iterator != history.end();
}

