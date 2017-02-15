#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <stdlib.h>
#include "GPIOClass.h"
#include <unistd.h>

//using robotics_members.txt file to save the members of the robotics team.
using namespace std;

class Member{
private:
	string name;
	string id;

public:
	// Constructors
	Member(){
		name = "Unknown";
		id = "Unknown";
	}
	Member(string id){
		name = "Uknown";
		this->id = id;
	}
	Member(string name, string id){
		this->name = name;
		this->id = id;
	}
	// Getters
	string getMemberName(){
		return name;
	}
	string getMemberId(){
		return id;
	}
	
	void printInformation(){
			cout << "Name: " << getMemberName() << endl << "ID: " << getMemberId() << endl;
	}
}; //end of class*****

// Validates the person trying to enter is member
bool isValid(vector<Member> &vector_records, string input){
	for(unsigned i = 0; i < vector_records.size(); ++i){
		if(vector_records[i].getMemberId() == input || vector_records[i].getMemberName() == input){
		// Door will open
		return true;
		}	
	}
	// Door won't openDoor will open
	return false;
}

// Returns the outcome of the attempt
string attempt(int ans){
	if(ans == 0) return "Unsuccessful";
	else return "Successful";
}

// Logs any attempt to enter the room
void log(Member swipe, int ans){
	time_t t = time(0);   // get time now
  // ctime(&t) returns the current time from the computer
	
	
	// Logging info
	std::fstream a;
  a.open ("log.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	a << attempt(ans) 
	<< " attempt by [" << swipe.getMemberId() 
	<< "] to enter the Robotics Team Room, at "<<  ctime(&t);
	std::cout << attempt(ans) << '\n';
	a.close();

}

void populateClub(vector<Member> &vector_records){

	// Unpopulate the older database (if exist) before adding the new one
	while(vector_records.size() > 0){
		vector_records.pop_back();
	}


	fstream records;
	records.open("robotics_members.txt");

	string data; // Used to add memebers
	
	// Repopulating
	while( !records.eof()){
		getline(records, data);
		vector_records.push_back(Member(data));
	}
	records.close();
}


int main(){
	vector<Member> club; 

	string swipeReader = "";
	string run = "1";


    GPIOClass* gpio4 = new GPIOClass("20"); //create new GPIO object to be attached to  GPIO4

    gpio4->export_gpio(); //export GPIO17


    gpio4->setdir_gpio("out"); // GPIO17 set to input

	while(1){
	gpio4->setval_gpio("1");
	usleep(500000);
	gpio4->setval_gpio("0");
	usleep(500000);

	}


	// Actual authintication
	while(run != "0"){
	
		populateClub(club);
		for (int i = 0; i < club.size(); i++){
		club[i].printInformation();
			}
		getline(cin, swipeReader);
		log(swipeReader, isValid(club, swipeReader));
		
		// Reset the input for security and keep "run" for exit purpose.
		run = swipeReader;
		swipeReader = "";
	}
}
