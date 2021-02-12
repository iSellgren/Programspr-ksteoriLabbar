#pragma once
#include <vector>
#include <string>

enum struct Type{
    STRING,
    STAR, // *
    PLUS, // +
    LEFT_PAR, // (
    RIGHT_PAR, // )
    DOT, // .
    BRACKETS, //{}
    RIGHT_BRAC,
    LEFT_BRAC,
    BACKSLASH_I,
    BACKSLASH_O,
    ERROR 
};

struct Token{
    Type type;
    std::string value;
};

std::ostream &operator<<(std::ostream &os, const Token &token) {
    return os << static_cast<int>(token.type) << ' ' << token.value; 
}

using Tokens = std::vector<Token>;
using TokenIterator = Tokens::iterator;

class Tokenizer{
public:
    Tokenizer()= default;
    Tokens tokenizer(const std::string &str);
    Token backslash(std::string::const_iterator end);
    std::string stringToken(std::string::const_iterator begin, std::string::const_iterator end);
    Token brackets(std::string::const_iterator begin, std::string::const_iterator end);

private:
    std::string::const_iterator iterator;
};
std::string Tokenizer::stringToken(std::string::const_iterator begin, std::string::const_iterator end)
{
    std::string tmp;
    char letter;
    for(auto it = begin; it != end; it++)
    {
        letter = *it;
        switch(letter){
            case '*':
                return tmp;
            case '+':
                return tmp;
            case '(':
                return tmp;
            case ')':
                return tmp;
            case '.':
                return tmp;  
            case '}':
                return tmp;
            case '{':
                return tmp;               
            case '\\':
                return tmp;
            default:
            tmp += letter;
        }
    }
    return tmp;
}

Token Tokenizer::brackets(std::string::const_iterator begin, std::string::const_iterator end)
{
    Token token;
    std::string str;
    
    while(iterator != end && *iterator != '}')
    {
        str += *iterator;
        std::advance(iterator,1);
        
    }

    str += *iterator;
    token.type = Type::BRACKETS;
    token.value = str;
    return token;
}

Token Tokenizer::backslash(std::string::const_iterator end)
{
    Token token;
    
    if(std::next(iterator) != end && *std::next(iterator) == 'I' )
    {
        token.type = Type::BACKSLASH_I;
        std::advance(iterator,1);
        token.value = std::string(std::prev(iterator), std::next(iterator));
        return token;
    }
    else if (std::next(iterator) != end && *std::next(iterator) == 'O')
    {
        token.type = Type::BACKSLASH_O;
        std::advance(iterator,1);
        if(*std::next(iterator) == ('{'))
        {
            std::advance(iterator,2);
            std::string str;
            while(iterator != end && *iterator != '}')
            {
                
                str += *iterator;
                std::advance(iterator,1);
                
            }
            token.value = str;
            if(std::next(iterator) == end)
             return token;
        }
    }
    token.type = Type::ERROR;
    return token;
}
Tokens Tokenizer::tokenizer(const std::string &str)
{
    
    std::string::const_iterator end = str.cend();
    iterator = str.cbegin();
    Tokens tokens;
    Token token;

    while(iterator != end)
    {
        switch(*iterator){

            case '*':
                token.type = Type::STAR;
                token.value = *iterator;
                tokens.push_back(token);
                break;

            case '+':
                token.type = Type::PLUS;
                token.value = *iterator;
                tokens.push_back(token);
                break;    
            case '(':
                token.type = Type::LEFT_PAR;
                token.value = *iterator;
                tokens.push_back(token);
                break;  
                
            case ')':
                token.type = Type::RIGHT_PAR;
                token.value = *iterator;
                tokens.push_back(token);
                break;  
                
            case '.':
                token.type = Type::DOT;
                token.value = *iterator;
                tokens.push_back(token);
                break;  
                
            /*case '}':
                token.type = Type::RIGHT_BRAC;
                token.value = *iterator;
                tokens.push_back(token);
                break;*/

            case '{':
                
                token = brackets(iterator,end);
                //token.type = Type::LEFT_BRAC;
                //token.value = *iterator;
                tokens.push_back(token);
                break;   
                
            case '\\':
                token = backslash(end);
                tokens.push_back(token);
                break;  

            default:

                token.type = Type::STRING;
                token.value = stringToken(iterator, end);
                iterator += token.value.size() - 1;
                tokens.push_back(token);
                token.value.clear();
            
        }
        iterator++;
    }
    return tokens;
}

