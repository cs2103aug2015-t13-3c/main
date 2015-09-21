// @@author Chin Kiat Boon

#include "stdafx.h"
#include "Logic.h"

Logic::Logic() {
	isActive = false;
	sizeOfArray = 0;
}

Logic::Logic(bool status): isActive(status), io(ON) {}

Logic::~Logic() {}

bool Logic::getStatus() {
	return isActive && canCallIO();
}

bool Logic::canCallIO()
{
	return io.getStatus();
}

int Logic::getSize(void) {
	return sizeOfArray;
}

//swaps everything in an array element
void Logic::swapElement(textInfo& ElemA, textInfo& ElemB) {
	std::string tempStr;
	int tempInt;
	tempStr = ElemA.text;
	ElemA.text = ElemB.text;
	ElemB.text = tempStr;

	tempInt = ElemA.startDate;
	ElemA.startDate = ElemB.startDate;
	ElemB.startDate = tempInt;

	tempInt = ElemA.startTime;
	ElemA.startTime = ElemB.startTime;
	ElemB.startTime = tempInt;

	tempInt = ElemA.endDate;
	ElemA.endDate = ElemB.endDate;
	ElemB.endDate = tempInt;

	tempInt = ElemA.endTime;
	ElemA.endTime = ElemB.endTime;
	ElemB.endTime = tempInt;

	return;
}

//adds value to time while ensuring it still follows HHMM format
//assume addValue is lesser than 60
//**problem for case of 2359, change later (need to add day parameter)
int Logic::addTime(int time, int addValue) {
	if (addValue < 60) {
		int newTime;
		int minutes;
		int hours;

		minutes = time % 100;
		hours = time / 100;

		minutes = minutes + addValue;

		if (minutes >= 60) {
			minutes = minutes - 60;
			hours = hours + 1;
			newTime = hours * 100 + minutes;
		} else {
			newTime = hours * 100 + minutes;
		}

		return newTime;
	} else {
		return 9999;						//error		
	}
}

//copies infoArray into a given array
//for testing purposes
void Logic::copyArray(struct textInfo stringArray[]) {
	for (int i = 0; i < sizeOfArray; i++) {
		infoArray[i].text = stringArray[i].text;
		infoArray[i].startDate = stringArray[i].startDate;
		infoArray[i].startTime = stringArray[i].startTime;
		infoArray[i].endDate = stringArray[i].endDate;
		infoArray[i].endTime = stringArray[i].endTime;
	}

}

void Logic::addInfo(std::string textInput, int startDateInput, int startTimeInput, 
						 int endDateInput, int endTimeInput) {

	if (sizeOfArray < SIZE) {
		infoArray[sizeOfArray].text = textInput;
		infoArray[sizeOfArray].startDate = startDateInput;
		infoArray[sizeOfArray].startTime = startTimeInput;
		infoArray[sizeOfArray].endDate = endDateInput;
		infoArray[sizeOfArray].endTime = endTimeInput;

		sizeOfArray = sizeOfArray + 1;
	}
	return;
}

void Logic::deleteInfo(int index) {

	if (index < sizeOfArray) {	
		for (int i = index; i < sizeOfArray - 1; i++) {
			infoArray[i].text = infoArray[i+1].text;
			infoArray[i].startDate = infoArray[i+1].startDate;
			infoArray[i].startTime = infoArray[i+1].startTime;
			infoArray[i].endDate = infoArray[i+1].endDate;
			infoArray[i].endTime = infoArray[i+1].endTime;
		}
		sizeOfArray = sizeOfArray - 1;
	}

	return;
}

//returns information stored in the array element that index points to
//information passed in order of text, startDate, startTime, endDate, endTime separated by "|" as of now
std::string Logic::returnInfo(int index) {

	if (index < sizeOfArray){
		std::ostringstream infoContainer;

		infoContainer << infoArray[index].text << "|";
		infoContainer << infoArray[index].startDate << "|";
		infoContainer << infoArray[index].startTime << "|";
		infoContainer << infoArray[index].endDate << "|";
		infoContainer << infoArray[index].endTime;

		return infoContainer.str();
	} else {
		return "Nothing to return.";
	}
}

//sorts date and time in increasing order of startDate and startTime
//eg: 0th element will contain the earliest due date/time
void Logic::sortDateAndTime() {
	if (sizeOfArray != 0) {
		for (int i = 0; i < sizeOfArray; i++) {
			for (int j = i+1; j < sizeOfArray; j++) {
				if (infoArray[j].startTime < infoArray[i].startTime) {
					swapElement(infoArray[i], infoArray[j]);
				}
			}
		}

		//sorts date after time
		for (int i = 0; i < sizeOfArray; i++) {
			for (int j = i+1; j < sizeOfArray; j++) {
				if (infoArray[j].startDate < infoArray[i].startDate) {
					swapElement(infoArray[i], infoArray[j]);
				}
			}
		}
	}
}

//changes information in a given array element
void Logic::changeInfo(std::string textInput, int startDateInput, int startTimeInput, 
				int endDateInput, int endTimeInput, int index) {

	//if text is not meant to be changed, textInput will be "*"
	if (textInput != "*") {
		infoArray[index].text = textInput;	
	}

	if (startDateInput != 999999) {
		infoArray[index].startDate = startDateInput;
	}

	if (startTimeInput != 9999) {
		infoArray[index].startTime = startTimeInput;
	}

	if (endDateInput != 999999) {
		infoArray[index].endDate = endDateInput;
	}

	if (endTimeInput != 9999) {
		infoArray[index].endTime = endTimeInput;
	}

	return;
}


//searches for the first available slot from input date and time onwards
std::string Logic::freeSlotSearch(int date, int time) {
	std::ostringstream freeSlot;

	if (sizeOfArray == 0) {
		freeSlot << date << "|" << time;
		return freeSlot.str();					
	} else {
		int freeDate = date;
		int freeTime = time;
		
		sortDateAndTime();
		
		//if date and time is in between ith array element,
		//move to next element, updating the possible free time to date and time right after
		//ith array element
		for (int i = 0; i < sizeOfArray; i++) {
			 
			//below condition checks if date and time is in between the duration of ith array element
			if ((infoArray[i].startDate < freeDate) || 
				((infoArray[i].startDate == freeDate) && (infoArray[i].startTime <= freeTime))) {
					if ((infoArray[i].endDate > freeDate) ||
						((infoArray[i].endDate == freeDate) && (infoArray[i].endTime >= freeTime))) {

						//condition to prevent freeDate/freeTime from going back to the past
						if ((freeDate < infoArray[i].endDate) ||
							((freeDate == infoArray[i].endDate) && (freeTime < infoArray[i].endTime))) {
								freeDate = infoArray[i].endDate;
								freeTime = addTime(infoArray[i].endTime, 1);
						}
					} 
			}
		}

		freeSlot << freeDate << "|" << freeTime;
		return freeSlot.str();
	}
}
