/* 
 * File:   tokenizer.cpp
 * Author: wouter
 * 
 * Created on August 6, 2010, 3:37 PM
 */

#include "tokenizer.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>

using namespace common;

Tokenizer::Tokenizer(const char* data, int size)
	: data(data), dataSize(size), cursor(0), token(0), tokenSize(0)
{
}

Tokenizer::Tokenizer(const Tokenizer& orig)
	: data(orig.data), dataSize(orig.dataSize), cursor(0), token(0), tokenSize(0)
{
}

Tokenizer::~Tokenizer()
{
	if (this->token)
		delete []this->token;
}

const char* Tokenizer::GetToken()
{
	return this->token;
}

const char* Tokenizer::GetNextToken()
{
    if (this->NextToken())
        return this->GetToken();

    common::Log::Error("Unexpected end of data");
    return "";
}

int Tokenizer::GetNextIntegerToken()
{
    if (this->NextToken())
        return atoi(this->token);

    common::Log::Error("Unexpected end of data");
    return 0;
}

float Tokenizer::GetNextFloatToken()
{
    if (this->NextToken())
        return float(atof(this->token));

    common::Log::Error("Unexpected end of data");
    return 0;
}

///*
bool Tokenizer::NextToken()
{
	// Check if we are at the end
	if (cursor >= this->dataSize)
		return false;
	
	// Reset the token
	if (this->token != 0)
		delete []this->token;
	this->token = 0;

	// Trim to the token
	while (cursor < this->dataSize && Tokenizer::isSeperator(data[cursor]))
		cursor++;

	// Check if we are at the end
	if (cursor >= this->dataSize)
		return false;

	int c = 0;

	if (data[cursor] == '/' && data[cursor+1] == '/')
	{
		while (cursor + c < this->dataSize && data[cursor] != '\n')
			cursor++;

		// Trim to the token
		while (cursor < this->dataSize && Tokenizer::isSeperator(data[cursor]))
			cursor++;

		// Make sure we stop when the cursor is hits the end of the file
		if (cursor >= this->dataSize)
			return false;
	}

	// Are we at a quoted token?
	if (Tokenizer::isQuote(data[cursor]))
	{
		while (data[cursor] != data[cursor + c + 1] && data[cursor + c + 1] != 0 && cursor < this->dataSize)
			c++;
		this->token = new char[c + 2];
		memcpy(this->token, data + cursor + 1, c);
		this->token[c] = 0;
		cursor+= c + 2;
	}
	else
	{
		while (Tokenizer::isSeperator(data[cursor + c]) == false && cursor < this->dataSize)
			c++;
		this->token = new char[c + 2];
		memcpy(this->token, data + cursor, c);
		this->token[c] = 0;
		cursor+= c;
	}

	return true;
}

bool Tokenizer::ExpectToken(const char* token)
{
    if (this->NextToken() == false)
        return false;

    int c = 0;
    while (token[c] == this->token[c] && token[c] != '\0')
        c++;

    if ((token[c] == this->token[c]) == false)
    {
        common::Log::Error("Unexpected token: '%s' instead of '%s'", this->token, token);
        return false;
    }
    return true;
}

bool Tokenizer::IsNumericToken()
{
	char* tmp = this->token;
	while (tmp[0] != '\0')
	{
        if (tmp[0] <= '0' && tmp[0] <= '9' && tmp[0] != '.')
			return false;
		tmp++;
	}
	return true;
}

bool Tokenizer::isSeperator(char c)
{
	if (c <= ' ') return true;

	return false;
}

bool Tokenizer::isQuote(char c)
{
	if (c == '\"') return true;
	if (c == '\'') return true;

	return false;
}
