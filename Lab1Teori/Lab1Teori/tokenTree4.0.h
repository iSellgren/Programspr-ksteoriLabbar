//
//  tokenTree4.0.h
//  Lab1Teori
//
//  Created by Fredrik Sellgren on 2020-02-15.
//  Copyright © 2020 Fredrik Sellgren. All rights reserved.
//

#ifndef tokenTree3_0_h
#define tokenTree3_0_h

#include "token.hpp"
#include <memory>
#include <utility>

class Node;
using ChildNode = std::unique_ptr<Node>;
using constIterator = std::string::const_iterator;

struct Pair {
    constIterator first, last;
    
};

struct Digger{
    size_t level = 0;
    int IsCaseSensitive = 0;
    constIterator inputFirst, inputLast, resultFirst, resultLast;
    //std::string input;
    bool WasGreedy = false;
    bool KidIsDot = false;
    int IslastParre = false;
    std::vector<Pair>parrenVec;

    
};

class Node
{
public:
    virtual ~Node() = default;
    virtual bool eval(Digger &digger) = 0;
    virtual void print() = 0;
    void addChildNode(ChildNode childnode);
    std::vector<ChildNode> childnodes;
};
class NodeLine : public Node
{
    bool eval(Digger &digger) override{
    RESTART_FUNC:
        auto oldPos = digger.resultLast;
        bool result = childnodes.front()->eval(digger);
        
        
        while (!result && digger.inputLast != digger.resultLast) {
            digger.resultLast = ++digger.resultFirst;
            result = childnodes.front()->eval(digger);
        }
        auto multiplePlaneCheck = digger.resultFirst;
        for(auto i = 1; i < childnodes.size() && result; i++)
        {
            if(digger.WasGreedy){
                digger.WasGreedy = false;
                auto movingBack = digger.inputLast;
                digger.resultLast = movingBack;
                result = childnodes[i]->eval(digger);
                while (!result && oldPos < digger.resultLast) {
                    digger.resultLast = --movingBack;
                    result = childnodes[i]->eval(digger);
                }
                if(!result){
                    digger.resultLast = digger.resultFirst = digger.inputLast;
                    return false;
                }
                
                if(!digger.parrenVec.empty()){
                    digger.parrenVec[digger.IslastParre].last = movingBack;
                }
                
                oldPos = digger.resultLast;
            }else{
                oldPos = digger.resultLast;
                result = childnodes[i]->eval(digger);
            }
        }
        if(!result && digger.resultLast < digger.inputLast){
            goto RESTART_FUNC;
        }
        if(multiplePlaneCheck != digger.resultFirst)
        {
            return false;
        }


        return result;
    }
    void print() override{std::cout << "Line" << std::endl;}

};
class NodeBrack : public Node
{
    
public:
    NodeBrack(int v) : values(v){};
    bool eval(Digger &digger) override{
        for(int i = 0; i < values; i++)
        {
            if(!childnodes.front()->eval(digger)){
                                return false;
            }
        }
        return true;
    };
    void print() override{std::cout << "Brack" << std::endl;}
    int values;
};
class NodeStar : public Node
{
    bool eval(Digger &digger) override{
        digger.KidIsDot = false;
        
        bool result = childnodes.front()->eval(digger);
        if(!result)
        {
            return false;
        }
        auto previous = std::prev(digger.resultLast);
        if(*digger.resultLast == *previous)
        {
            while(digger.resultLast < digger.inputLast && *digger.resultLast == *previous)
            {
                digger.resultLast++;
            }
            return true;
            
        }else if(digger.KidIsDot){
            digger.KidIsDot = false;
            digger.WasGreedy = true;
            digger.resultLast = digger.inputLast;
        }
        return true;
    }
    
    void print() override{std::cout << "Star" << std::endl;};
};


class NodePlus : public Node
{
    bool eval(Digger &digger) override{
        bool rhs;
        bool lhs;
        
        Digger saveDigger = digger;
        lhs = childnodes.front()->eval(digger);
        
        Digger lhsDigger = digger;
        digger = saveDigger;
        
        rhs = childnodes.back()->eval(digger);
        
        Digger rhsDigger = digger;
        

        if(!lhs && !rhs)
        {
            digger = rhsDigger.resultLast < lhsDigger.resultLast ? rhsDigger : lhsDigger;
            return false;
        }
        else if(lhs && !rhs){
            digger = lhsDigger;
            return true;
        }
        else if(!lhs && rhs)
        {
            digger = rhsDigger;
            return true;
        }
        digger = rhsDigger.resultLast < lhsDigger.resultLast ? rhsDigger : lhsDigger;
        return true;
            
    };
    void print() override{std::cout << "Plus" << std::endl;};
};

class NodeBackslash_I : public Node{

    public:
        bool eval(Digger &digger) override{
            
            digger.IsCaseSensitive++;
            bool result = childnodes.front()->eval(digger);
            digger.IsCaseSensitive--;
            return result;
        };
        void print() override{std::cout << "\\I" << std::endl;};
};
class NodeBackslash_O : public Node{
public:
    NodeBackslash_O(int value) : value(value) {};
    bool eval(Digger &digger) override{
        bool result = childnodes.front()->eval(digger);
        if(result && value > 0){
            digger.resultFirst = digger.parrenVec[value - 1].first;
            digger.resultLast = digger. parrenVec[value - 1].last;
        }
        
        return result;
    };
    void print() override{std::cout << "\\O" << std::endl;};
    int value;
};

class NodeDot : public Node{

public:
    NodeDot(const std::string &str) : value(str){};
    bool eval(Digger &digger) override{
        
        if(digger.resultLast == digger.inputLast) return false;
        digger.resultLast++;
        digger.KidIsDot = true;
        return true;
    };
    void print() override{std::cout << "Dot" << std::endl;};
    std::string value;
};

class NodeParens : public Node{

public:
    NodeParens(int value) : index(value) {};
    bool eval(Digger &digger) override{
        auto startPos = digger.resultLast;
        bool result = childnodes.front()->eval(digger);
        if(result) {
            digger.parrenVec[index] = {startPos, digger.resultLast};
        } else {
            digger.parrenVec[index] = {digger.inputLast, digger.inputLast};
        }
        digger.IslastParre = index;
        return result;
    };
    void print() override{std::cout << "Parre" << std::endl;};
    int index;
};

class NodeString : public Node{

public:
    NodeString(const std::string &str) : value(str){};
    bool eval(Digger &digger) override{
        for(auto chars : value){
            bool isUpper = digger.IsCaseSensitive != 0;
            if(!isUpper)
            {
                if(digger.resultLast == digger.inputLast || *digger.resultLast != chars)
                {
                    return false;
                    
                }
                
            }else if (std::toupper(*digger.resultLast) != std::toupper(chars)){
                return false;
            }
            digger.resultLast++;
        }
        return true;
    };
    
    void print() override{std::cout << value << std::endl;};
    std::string value;
};

class Parser
{
    public:
    ChildNode parseToken(Tokens &&tokens, Digger *digger);
    Token *findTokens(Type token);
    ChildNode buildLine();
    
    ChildNode buildValue();
    ChildNode buildUnary(ChildNode &child);
    ChildNode buildBinary(ChildNode &child);

    ChildNode buildPlus(ChildNode &childNode);
    ChildNode buildStar(ChildNode &childNode);
    ChildNode buildParres();
    ChildNode buildBrack(ChildNode &childNode);
    ChildNode buildBackSlash_I(ChildNode &childNode);
    ChildNode buildBackSlash_O();
    ChildNode buildString();
    ChildNode buildDot();
    
    Tokens tokens;
    TokenIterator tokeniterator;
    bool canStar = true;
    int index = 0;
    Digger *digger = nullptr;
};

void Node::addChildNode(ChildNode childNode){

    childnodes.push_back(std::move(childNode));
    
}
ChildNode Parser::parseToken(Tokens &&token, Digger *digger){
    this->digger = digger;
    this->tokens = std::move(token);
    this->tokeniterator = this->tokens.begin();
    return buildLine();
}

Token *Parser::findTokens(Type token){

    if (tokeniterator == tokens.end() || tokeniterator->type != token){
        return nullptr;
    }
    return &*(tokeniterator++);
}
ChildNode Parser::buildLine(){
    auto line = std::make_unique<NodeLine>();
    while(tokeniterator != tokens.end() ) {
        
        ChildNode parent = buildBackSlash_O();
        if(parent) {
            if(tokeniterator == tokens.end() ) {
                parent->addChildNode(std::move(line));
                return parent;
            }
            return nullptr;
        } if (tokeniterator == tokens.end())
            break;
        
        ChildNode node = buildValue();
        if(!node) {
            if(!canStar || line->childnodes.empty() ) {
                return nullptr;
            }
            return line;
        }
        
        ChildNode un = buildUnary(node);
        while(un) {
            node = std::move(un);
            un = buildUnary(node);
        }
        
        ChildNode plusChild = std::make_unique<NodeLine>();
        ChildNode bin = buildBinary(plusChild);
        if(bin) {
            bin->childnodes.front()->addChildNode(std::move(node));
            node = std::move(bin);
        }
            
        line->addChildNode(std::move(node) );
    }
    if(line->childnodes.empty() ) {
        return nullptr;
    }
   
    return line;
}

ChildNode Parser::buildValue() {
    ChildNode value = buildString();
    if(value) {
        canStar = true;
        return value;
    }
    
    value = buildDot();
    if(value) {
        canStar = true;
        return value;
    }
    
    value = buildParres();
    if(value) {
        canStar = false;
        return value;
    }
    return nullptr;
}

ChildNode Parser::buildString() {
    auto tok = findTokens(Type::STRING);
    if(tok) {
        //std::cout << tok->value << std::endl;
        return std::make_unique<NodeString>(tok->value);
    }
    return nullptr;
}

ChildNode Parser::buildDot() {
    auto tok = findTokens(Type::DOT);
    if(tok) {
        return std::make_unique<NodeDot>(tok->value);
    }
    return nullptr;
}

ChildNode Parser::buildParres() {
    if(!findTokens(Type::LEFT_PAR)) {
        return nullptr;
    }
    
    ChildNode child = buildLine();
    if(!child) {
        tokeniterator--;
        return nullptr;
    }
    
    if(!findTokens(Type::RIGHT_PAR)) {
        return nullptr;
    }
    
    ChildNode parres = std::make_unique<NodeParens>(index++);
    digger->parrenVec.emplace_back();
    //digger->wasGreddy.push_back(false);
    parres->addChildNode(std::move(child));
    return parres;
}

ChildNode Parser::buildUnary(ChildNode &child) {
    ChildNode unary = nullptr;
    
    if(canStar) {
        unary = buildStar(child);
    }
    
    if(unary) {
        return unary;
    }
    
    unary = buildBackSlash_I(child);
    if(unary) {
        return unary;
    }
    
    unary = buildBrack(child);
    return unary;
}

ChildNode Parser::buildStar(ChildNode &child) {
    if(!findTokens(Type::STAR)) {
        return nullptr;
    }
//    digger->WasGreedy = true;
    ChildNode star = std::make_unique<NodeStar>();
    star->addChildNode(std::move(child) );
    return star;
}

ChildNode Parser::buildBackSlash_I(ChildNode &child) {
    if(!findTokens(Type::BACKSLASH_I)) {
        return nullptr;
    }
    ChildNode backI = std::make_unique<NodeBackslash_I>();
    backI->addChildNode(std::move(child) );
    return backI;
}

ChildNode Parser::buildBrack(ChildNode &child) {
    Token *tok = findTokens(Type::BRACKETS);
    if(!tok) {
        return nullptr;
    }
    tok->value.pop_back();
    tok->value = tok->value.substr(1);
    int value = std::stoi(tok->value);
    ChildNode brack = std::make_unique<NodeBrack>(value);
    brack->addChildNode(std::move(child) );
    return brack;
}

ChildNode Parser::buildBinary(ChildNode &child) {
    return buildPlus(child);
}

ChildNode Parser::buildPlus(ChildNode &child) {
    if(!findTokens(Type::PLUS)) {
        return nullptr;
    }
    
    ChildNode rhs = buildLine();
    if(!rhs) {
        return nullptr;
    }
    ChildNode plus = std::make_unique<NodePlus>();
    plus->addChildNode(std::move(child) );
    plus->addChildNode(std::move(rhs) );
    return plus;
}
ChildNode Parser::buildBackSlash_O(){
    Token *tok = findTokens(Type::BACKSLASH_O);
    if(!tok){
        return nullptr;
    }
    ChildNode backSlash_O = std::make_unique<NodeBackslash_O>(stoi(tok->value));
    return backSlash_O;
}

#endif /* tokenTree4_0_h */
