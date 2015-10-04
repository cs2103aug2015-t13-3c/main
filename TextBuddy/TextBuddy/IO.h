// @@author Ng Ren Zhi

#ifndef IO_H_
#define IO_H_

#include "stdafx.h"
#include "Rapidjson\include\rapidjson\document.h"

using namespace rapidjson;

class IO {
private:
	static Utilities u;

	static bool fileIsOpen(std::ifstream& inputFile);
	static bool fileIsOpen(std::ofstream& outputFile);
	static Task getTask(std::ifstream& inputFile);
	static Task extractTaskFromJsonObject(Value& item);

	static void extractName(Task &newTask, Value &item);
	static void extractType(Task &newTask, Value &item);
	static void extractID(Task &newTask, Value &item);
	static void extractLabel(Task &newTask, Value &item);
	static void extractDone(Task &newTask, Value &item);
	static void extractPriority(Task &newTask, Value &item);
	static void extractStartDay(Task &newTask, Value &item);
	static void extractStartDate(Task &newTask, Value &item);
	static void extractStartTime(Task &newTask, Value &item);
	static void extractEndDay(Task &newTask, Value &item);
	static void extractEndDate(Task &newTask, Value &item);
	static void extractEndTime(Task &newTask, Value &item);

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