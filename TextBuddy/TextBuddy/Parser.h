#ifndef PARSER_H_
#define PARSER_H_

class Parser{
private:
	bool isActive;
public:
	Parser();
	Parser(bool status);
	bool getStatus();
};

#endif