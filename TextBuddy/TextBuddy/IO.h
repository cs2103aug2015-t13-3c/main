#ifndef IO_H_
#define IO_H_

#include<vector>

using namespace std;

class IO{
private:
	bool isActive;
public:
	IO();
	IO(bool status);
	bool getStatus();

	static vector<std::string> loadFile(string fileName);
};

#endif