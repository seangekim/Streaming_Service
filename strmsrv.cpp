#include "strmsrv.h"
#include <iostream>
#include <fstream>
using namespace std;

StreamService::StreamService()

{
    cUser_ = NULL;
}


StreamService::~StreamService() // deallocates vectors of users and content
{
    for(size_t i=0; i<users_.size(); i++){
        delete users_[i];
    }
    for(size_t i=0; i<content_.size(); i++){
        delete content_[i];
    }

}

// reads and parses through data files
void StreamService::readAndParseData(std::istream& is, Parser& p)
{
    p.parse(is, content_, users_);
    cout << "Read " << content_.size() << " content items." << endl;
    cout << "Read " << users_.size() << " users." << endl;
}

// Logins a user into streaming service
void StreamService::userLogin(const std::string& uname)
{
    if(getUserIndexByName(uname) == -1){
        throw std::invalid_argument("Username not in inventory");
    } 
    if(cUser_ == NULL){
        cUser_ = users_[getUserIndexByName(uname)];
    }
    else{
        throw std::runtime_error("User already logged in");
    }

    cUser_ = users_[getUserIndexByName(uname)];

}

// Logouts user out of streaming service
void StreamService::userLogout()
{
    cUser_ = NULL;

}


// Searches through content to either list all content or lookup a string and returns any title with that string. 
std::vector<CID_T> StreamService::searchContent(const std::string& partial) const
{
    std::vector<CID_T> results;
    for(size_t i = 0; i < content_.size(); i++){
        if(partial == "*"){
            results.push_back(i);
        }        
        else if((content_[i]->name()).find(partial) != string::npos) {
            results.push_back(i);
        }
    }
    return results;
}

// returns user history
std::vector<CID_T> StreamService::getUserHistory() const
{
    throwIfNoCurrentUser();
    return cUser_->history;
}

// adds content to user's watch history and to content's listing of users
void StreamService::watch(CID_T contentID)
{
    throwIfNoCurrentUser();
    if(!isValidContentID(contentID)){
        throw ReviewRangeError("Content does not exist");
    }

    if(content_[contentID]->rating() > (cUser_->ratingLimit)){
        throw RatingLimitError("Rating above user rating limit");
    }


    cUser_->addToHistory(contentID);
    content_[contentID]->addViewer(cUser_->uname);

}

// Adds review to content
void StreamService::reviewShow(CID_T contentID, int numStars)
{
    throwIfNoCurrentUser();
    if(numStars > 5 || numStars < 0){
        throw ReviewRangeError("numStars error");
    }
    content_[contentID]->review(numStars);

}

// Suggests best similar content for user given a piece of content
CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{
    User* tempUser = NULL;
    int maxView = 0;
    int indofRec = -1;
    
    for(size_t i = 0; i<content_[contentID]->getViewers().size(); i++){
        if(cUser_->uname != content_[contentID]->getViewers()[i]){
            tempUser = users_[getUserIndexByName(content_[contentID]->getViewers()[i])];
            for(size_t i = 0; i < tempUser->history.size(); i++){
                content_[tempUser->history[i]]->viewC_ ++;
            }
        }
    }
    for(size_t i = 0; i<content_.size(); i++){
        cout << "Content: " << i << " numSuggest: " << content_[i]->viewC_ << endl;
        if(content_[i]->viewC_ > maxView && !content_[i]->hasViewed(cUser_->uname) && (signed) i != contentID){  
            cout << "MaxViews " << content_[i]->viewC_ << " ContentID " << contentID << " temp ind " << i <<endl ;
            maxView = content_[i]->viewC_;
            indofRec = i;
        }
    }
    for(size_t i = 0; i<content_.size(); i++){
        content_[i]->viewC_ = 0;
    }

    cout << "indofRec " << indofRec << endl;
    return indofRec;

}

// Displays content info
void StreamService::displayContentInfo(CID_T contentID) const
{
    if(! isValidContentID(contentID)){
        throw std::invalid_argument("Watch: invalid contentID");
    }
}

// Checks if content ID is valid
bool StreamService::isValidContentID(CID_T contentID) const
{
    return (contentID >= 0) && (contentID < (int) content_.size());
}

// Checks if a user is logged in 
void StreamService::throwIfNoCurrentUser() const
{
    if(cUser_ == NULL){
        throw UserNotLoggedInError("No user is logged in");
    }
}

// Gives a user index given the name
int StreamService::getUserIndexByName(const std::string& uname) const
{
    for(size_t i = 0; i < users_.size(); i++){
        if(uname == users_[i]->uname) {
            return (int)i;
        }
    }
    return -1;
}
