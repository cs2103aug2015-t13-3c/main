// @@author Ng Ren Zhi

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
	~IO();

	bool getStatus();

	static vector<std::string> loadFile(string fileName);
};

#endif