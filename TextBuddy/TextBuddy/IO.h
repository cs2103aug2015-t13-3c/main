// @@author Ng Ren Zhi

#ifndef IO_H_
#define IO_H_

#include "stdafx.h"

class IO {
private:
	static Utilities u;

	static bool fileIsOpen(std::ifstream& inputFile);
	static bool fileIsOpen(std::ofstream& outputFile);
	static Task getTask(std::ifstream& inputFile);

public:
	IO();
	~IO();

	static std::vector<Task> loadFile(std::string fileName);
	static bool saveFile(std::string fileName, std::vector<Task> taskVector);

	//======== getter / setter methods for test==========
	static std::vector<std::string> getText(std::string fileName) {
		std::ifstream inputFile(fileName);
		std::vector<std::string> textVector;

		
		//while(!inputFile.eof()) {
			std::string line;
			getline(inputFile,line);

			if(line != "") {
				textVector.push_back(line);
			}
		//}
		
		inputFile.close();
		return textVector;
	}

};

#endif