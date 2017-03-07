#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <stdlib.h>
#include "GPIOClass.h"
#include <unistd.h>


// TODO:  Make a GPOI class to use for the RPi (http://www.hertaville.com/introduction-to-accessing-the-raspberry-pis-gpio-in-c.html)
// #include "GPIOClass.h"


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
		name = "Unknown";
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
	cout << input << endl;
	if(input == "")return 0;

	for(unsigned i = 0; i < vector_records.size(); ++i){
		if(vector_records[i].getMemberId() == input || vector_records[i].getMemberName() == input && input != ""){

			// Door will open
			GPIOClass* gpio = new GPIOClass("20");
			gpio->export_gpio();
			gpio->setdir_gpio("out");
			gpio->setval_gpio("1");
			usleep(3000000);
			gpio->setval_gpio("0");

			delete gpio;
			gpio = NULL;
		return 1;
		}
	}
	return 0;
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

	// Actual authintication
	vector<Member> club;

	while(1){

		ifstream myfile("/dev/tty1");
		string swipe;

		while (! myfile.eof() ){
//			populateClub(club);

//			for (size_t i = 0; i < club.size(); i++) {
//			std::cout << i << "# populate: " << club[i].getMemberName() << " && " << club[i].getMemberId() << '\n';
//			}

			getline (myfile,swipe);
			populateClub(club);
			log(swipe, isValid(club, swipe));
			swipe = "";
		}
		myfile.close();
	}
}
