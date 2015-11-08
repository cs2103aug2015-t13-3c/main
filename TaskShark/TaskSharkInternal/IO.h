// Ng Ren Zhi @@author A0130463R

#ifndef IO_H_
#define IO_H_

#include "Rapidjson\include\rapidjson\document.h"

using namespace rapidjson;

class IO {
private:
	static IO* theOne;
	IO();
	static std::string filePath;
	static const std::string configPath;
	void setLastSavedLocation(std::string fileName);

	bool fileIsOpen(std::ifstream& inputFile);
	bool fileIsOpen(std::ofstream& outputFile);
	Task extractTaskFromJsonObject(Value& item);
	void writeTaskIntoJsonFormat(std::ofstream &newFile, Task task);
	void initialiseRunningCount(std::vector<Task> taskVector);
	bool setCommandKeyword(std::string &identifier, std::string keyword);
	void setCustomCommands(std::ifstream& TSconfig);
	void saveCustomCommands(std::ofstream& TSconfig);

	void extractName(Task &newTask, Value &item);
	void extractType(Task &newTask, Value &item);
	void extractID(Task &newTask, Value &item);
	void extractLabel(Task &newTask, Value &item);
	void extractDone(Task &newTask, Value &item);
	void extractPriority(Task &newTask, Value &item);
	void extractStartDate(Task &newTask, Value &item);
	void extractStartTime(Task &newTask, Value &item);
	void extractEndDate(Task &newTask, Value &item);
	void extractEndTime(Task &newTask, Value &item);

	void initialiseJsonText(std::ofstream& newfile);
	void closeJsonText(std::ofstream& newfile);
	std::string insertOpenParanthese();
	std::string insertCloseParanthese();
	std::string writeNameAttribute(Task task);
	std::string writeTypeAttribute(Task task);
	std::string writeIDAttribute(Task task);
	std::string writeLabelAttribute(Task task);
	std::string writeIsDoneAttribute(Task task);
	std::string writeIsPriorityAttribute(Task task);
	std::string writeStartDateAttribute(Task task);
	std::string writeStartTimeAttribute(Task task);
	std::string writeEndDateAttribute(Task task);
	std::string writeEndTimeAttribute(Task task);

	std::string retrieveName(Task task);
	std::string retrieveType(Task task);
	std::string retrieveID(Task task);
	std::string retrieveLabel(Task task);
	std::string retrieveIsDone(Task task);
	std::string retrieveIsPriority(Task task);
	std::string retrieveStartDate(Task task);
	std::string retrieveStartTime(Task task);
	std::string retrieveEndDate(Task task);
	std::string retrieveEndTime(Task task);

public:
	static IO* getInstance();
	~IO();
	std::string getFilePath();
	bool setFilePath(std::string newFilePath, std::vector<Task> taskVector, bool isRemovePrevFile=true);

	std::vector<Task> loadFile(std::string fileName, bool isOverwriteLoadFile=true);
	bool saveFile(std::string filePath, std::vector<Task> taskVector);

	//========== Getter for Testing ==========
	std::vector<std::string> getText(std::string fileName);
};

#endif