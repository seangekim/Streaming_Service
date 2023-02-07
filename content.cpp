#include "content.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Constructor for content
Content::Content(int id, std::string name, int nr, int ts, int rating) 

{
    id_ = id;
    name_ = name;
    numReviews_ = nr;
    totalStars_ = ts;
    rating_ = rating;
}

Content::~Content()
{
 
}

// Display function
void Content::display(std::ostream& ostr) const
{
    ostr << "========" << endl;
    ostr << "ID: " << id_ << endl;
    ostr << "Name: " << name_ << endl;
    ostr << "Views: " << usersWhoViewed_.size() << endl;
    ostr << "Average review: " << setprecision(1) << fixed << getStarAverage() << endl;
    
    if(0 >= rating_ && rating_ < 5) {
        const char** ratingMapping = this->ratingStrings();
        ostr << "Rating: " << ratingMapping[rating_] << endl;
    }
}

// Returns ID of content
int Content::id() const
{
    return id_;
}

// Returns name of content
std::string Content::name() const
{
    return name_;
}

// Returns rating
int Content::rating() const
{
    return rating_;
}

// Adds review to content
void Content::review(int numStars) 
{
    numReviews_++;
    totalStars_ += numStars;
}

// Adds viewer to content
void Content::addViewer(const std::string& username)
{
    usersWhoViewed_.push_back(username);
}

// Returns vector of users who have viewed given content
const std::vector<std::string>& Content::getViewers() const
{
    return usersWhoViewed_;
}

// Bool returning if certain user has viewed content
bool Content::hasViewed(const std::string& uname) const
{
    for(size_t m = 0; m < usersWhoViewed_.size(); m++){
        if(usersWhoViewed_[m] == uname){
            return true;
        }
    }
    return false;
}


// Holds movie ratings
const char** Movie::ratingStrings() const
{
    // Global variable of movie rating mappings that you can use for 
    //  displaying Movie objects. We use "G" for rating 0 and 1.
    static const char* movierating[] = {"G", "G", "PG", "PG-13", "R"};
    return movierating;
}

// Holds series ratings
const char** Series::ratingStrings() const
{
    // DEFAULT TO THE SERIES / TV Rating Mappings
    
    // Global variable of series rating mapping (0 = "TVY", 1 = "TVG", etc.)
    static const char* tvrating[] = {"TVY", "TVG", "TVPG", "TV14", "TVMA"};
    return tvrating;
}

// Returns average review of given show
double Content::getStarAverage() const 
{
    if(numReviews_ == 0){
        return 0;
    }
    double avg = static_cast<double>(totalStars_) / numReviews_;
    return avg;
}





// Returns number of episodes in series 
int Series::numEpisodes() const
{
    return numEpisodes_;
}

// Constructor for movies 
Movie::Movie(int id, std::string name, int nr, int ts, int rating) :
    Content(id, name, nr, ts, rating){
    
}

// Deconstructor for movies
Movie::~Movie(){

}

// Constructor for series
Series::Series(int id, std::string name, int nr, int ts, int rating, int numEpisodes) : 
    Content(id, name, nr, ts, rating){
    numEpisodes_ = numEpisodes;
}

// Deconstructor for series
Series::~Series(){

}

// Overloading function to display series
void Series::display(std::ostream& ostr) const{
    Content::display(ostr);
    cout << "Episodes: " << numEpisodes_ << endl;
}

