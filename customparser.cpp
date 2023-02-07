// Complete me
#include "customparser.h"
#include <iostream>
#include <sstream>

using namespace std;

const char* error_msg_1 = "Cannot read integer n";
const char* error_msg_2 = "Error in content parsing";
const char* error_msg_3 = "Error in user parsing";


// Function to parse through file
void CustomFormatParser::parse(std::istream& is, std::vector<Content*>& content, std::vector<User*>& users)
{
    // Erase any old contents
    content.clear();
    users.clear();

    
    int numContent; 
    is >> numContent; // receives content to use for for loop
    if(is.fail()){
        throw ParserError(error_msg_1);
    }
    int tempID, tempType;
    string tempName;
    string emptyL;
    int tempReviews, tempStars, tempRating, tempEp;
    string viewers;


    for(int i=0; i<numContent; i++){ // for loop to run through content
        try{
            is >> tempID >> tempType;
            if(is.fail()){
                throw ParserError(error_msg_2);
            }
            getline(is, emptyL); 
            getline(is, tempName);
            stringstream stringTemp(tempName);
            string hp;
            string n = "";
            while(stringTemp >> hp){ // removes leading whitespace
                n += hp + " ";
            }
            n.pop_back();        

            is >> tempReviews; 
            if(is.fail()){
                throw ParserError(error_msg_2);
            }
            is >> tempStars;
            if(is.fail()){
                throw ParserError(error_msg_2);
            }
            is >> tempRating;
            if(is.fail()){
                throw ParserError(error_msg_2);
            }

            if(tempType == 1){ // if type is a series, adds episode count
                is >> tempEp;
            }

            getline(is, emptyL);
            getline(is, viewers);
            stringstream ss(viewers);
            string tempView;
                    
            if(is.fail()){
                throw ParserError(error_msg_2);
            }

            Content* newc = NULL; // creates null content type
            if(tempType == 0){ // dynamically allocating a movie or series
                newc = new Movie(tempID, n, tempReviews, tempStars, tempRating);
            }   
            else{
                newc = new Series(tempID, n, tempReviews, tempStars, tempRating, tempEp);
            }
            while(ss >> tempView){ // adds viewers to content
                newc->addViewer(tempView);
            }
            if(is.fail()){ 
                throw ParserError("Error adding viewer to viewer list");
            }
            content.push_back(newc); // adds new content to vector of contents
        }
        catch(std::exception& e){
            throw ParserError(error_msg_2);
        }
    } 

    string tempUser;
    int limit;
    string tempWatched;
    string tempContent;
    
    while(is >> tempUser){ // while there is still input in file
        try{
            is >> limit;
            if(is.fail()){
                throw ParserError(error_msg_3);
            }
            getline(is, emptyL);
            getline(is, tempWatched);
            stringstream ss(tempWatched); 
            User* u = new User(tempUser, limit); // dynamically allocates new user 

            while(ss >> tempContent){ // adds content to viewers' watch history
                u->addToHistory(stoi(tempContent));
            }
            users.push_back(u); // adds user to vector of users
        }
        catch(std::exception& e){
            throw ParserError(error_msg_3);
        }


    }



}