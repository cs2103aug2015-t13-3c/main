#ifndef IO_H_
#define IO_H_

class IO{
private:
	bool isActive;
public:
	IO();
	IO(bool status);
	bool getStatus();
};

#endif