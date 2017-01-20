/* 
 * File:   tokenizer.h
 * Author: wouter
 *
 * Created on August 6, 2010, 3:37 PM
 */

#ifndef TOKENIZER_H
#define	TOKENIZER_H

namespace common
{

class Tokenizer
{
public:
	Tokenizer(const char* data, int size);
	Tokenizer(const Tokenizer& orig);
	virtual ~Tokenizer();

    const char* GetToken();
    const char* GetNextToken();
    int GetNextIntegerToken();
    float GetNextFloatToken();
    bool NextToken();
    bool ExpectToken(const char* token);
	
    bool IsNumericToken();

public:
	static bool isSeperator(char c);
	static bool isQuote(char c);
	
private:
	const char* data;
	int dataSize;
	int cursor;
	char* token;
	int tokenSize;

};

}

#endif	/* TOKENIZER_H */

