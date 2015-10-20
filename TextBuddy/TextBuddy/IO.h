// @@author A0130463R (Ng Ren Zhi)

#ifndef IO_H_
#define IO_H_

#include "Rapidjson\include\rapidjson\document.h"
#include "Task.h"

using namespace rapidjson;

class IO {
private:
	static IO* theOne;
	IO();
	std::string filePath;
	static const std::string lastSavedLocation;
	void setLastSavedLocation(std::string fileName);

	bool fileIsOpen(std::ifstream& inputFile);
	bool fileIsOpen(std::ofstream& outputFile);
	Task extractTaskFromJsonObject(Value& item);
	void writeTaskIntoJsonFormat(std::ofstream &newFile, Task task);
	void initialiseRunningCount(std::vector<Task> taskVector);

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
	~IO();
	static IO* getInstance();
	std::string getFilePath();
	bool setFilePath(std::string newFilePath, std::vector<Task> taskVector);

	std::vector<Task> loadFile(std::string fileName);
	bool saveFile(std::string fileName, std::vector<Task> taskVector);
	// bool changeSourceFileLocation (std::string newFileLocation);

	//========== Getter for Testing ==========
	std::vector<std::string> getText(std::string fileName);
};

#endif