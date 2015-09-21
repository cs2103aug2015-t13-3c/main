// @@author Ng Ren Zhi

#ifndef IO_H_
#define IO_H_

class IO{
private:
	bool isActive;

public:
	IO();
	IO(bool status);
	~IO();

	bool getStatus();
};

#endif