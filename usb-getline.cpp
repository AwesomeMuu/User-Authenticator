#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(){
  int i = 0;
  int r = 0;
  while(1){
    ifstream myfile("/dev/tty1");
    string line;
    while (! myfile.eof() ){
      getline (myfile,line);
      cout << i << ":" << r << ":" << line << endl;
      i++;
    }
    r++;
    myfile.close();   
  }
}
