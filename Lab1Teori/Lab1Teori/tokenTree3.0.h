//
//  tokenTree3.0.h
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
    bool CanIGetBig = false;
    bool ParentIsStar = false;
    bool ChildIsDot = false;
    std::vector<Pair>parrenVec;
    std::vector<bool> wasGreddy;

    unsigned index = 0;
    unsigned visitedIndex = -1;
    bool visitedGroup = false;
};

class Node
{
public:
    virtual ~Node() = default;
    virtual Pair eval(constIterator first, constIterator last, Digger &digger) = 0;
    virtual void print() = 0;
    void addChildNode(ChildNode childnode);
    std::vector<ChildNode> childnodes;
};
class NodeGlue : public Node
{
    Pair eval(constIterator first, constIterator last,Digger &digger) override{
        auto repair = [&](constIterator it, constIterator end) {
            if(digger.visitedGroup) {
                digger.visitedGroup = false;
                if(digger.visitedIndex == digger.index && digger.wasGreddy[digger.index]) {
                    if(it < digger.parrenVec[digger.index].first) {
                        digger.parrenVec[digger.index].first = digger.parrenVec[digger.index].last = end;
                    } else {
                        digger.parrenVec[digger.index].last = it;
                    }
                }
            }
        };
        
        Pair lhs = childnodes.front()->eval(first, last, digger);
        if(childnodes.size() == 1) {
            return lhs;
        }
        if(lhs.last == last && lhs.first == last)
            return {last,last};
        if(lhs.last == last && lhs.first == first)
        {
            //Detta säger att lhs har tagit allt.
            Pair rhs = childnodes.back()->eval(first,last,digger);
            Pair prev = rhs;
            while(rhs.first != rhs.last) {
                prev = rhs;
                rhs = childnodes.back()->eval(rhs.last,last,digger);
            }
            rhs = prev;
            if(rhs.first == lhs.last){
                return{
                    last,
                    last
                };
            }
            repair(rhs.first, last);
            return{
                lhs.first,
                rhs.last
            };
        }
        else if(lhs.last == last && lhs.last != first)
        {
            //Detta säger att lhs har tagit allt.
            Pair rhs = childnodes.back()->eval(first,last,digger);
            Pair prev = rhs;
            while(rhs.first != rhs.last) {
                prev = rhs;
                rhs = childnodes.back()->eval(rhs.last,last,digger);
            }
            rhs = prev;
                
            if(rhs.first == lhs.last){
                return{
                    last,
                    last
                };
            }
            repair(rhs.first, last);
            return{
                lhs.first,
                rhs.last
            };
        }
        Pair rhs = childnodes.back()->eval(lhs.last,last,digger);
        if(rhs.first == last)
            return{
              last,
              last
            };
        if(rhs.first == lhs.last){
            return{
                lhs.first,
                rhs.last
            };
        }
        while(lhs.last < rhs.first) {
            lhs = childnodes.front()->eval(lhs.last, last, digger);
            if(rhs.first == lhs.last){
                return{
                    lhs.first,
                    rhs.last
                };
            }
        }
        //str.* käkar allt han ska visa
        return {
            last,
            last
        };
    };
    void print() override{std::cout << "Glue" << std::endl;};
};
class NodeBrack : public Node
{
    Pair eval(constIterator first, constIterator last, Digger &digger) override{
        
        int i = 1;
        Pair subs = childnodes.front()->eval(first, last, digger);
        for(; i < values; i++)
        {
            Pair next = childnodes.front()->eval(subs.last, last, digger);
            
            if(subs.last == next.first) {
                subs.last = next.last;
            } else return eval(subs.last, last, digger);
        }
        if(i == values)
            return subs;
        
        return {
            last,
            last
        };
    };
public:
    NodeBrack(int v) : values(v){};
    void print() override{std::cout << "Brack" << std::endl;}
    int values;
};
class NodeStar : public Node
{
    Pair eval(constIterator first, constIterator last, Digger &digger) override{
        
        Pair result;
        digger.ParentIsStar = true;
        result = childnodes.front()->eval(first, last, digger);
        while(result.first != last)
        {
            first = result.last;
            Pair next = childnodes.front()->eval(first, last, digger);
            if(next.first == next.last) return result;
            size_t size = 0;
            for(;next.first != last; size++)
            {
                
                if(result.last == next.first){
                    first = result.last = next.last;
                }
                    else
                    {
                        std::string tmp(result.first,result.last);
                        for(auto it = result.last; it != last; it++)
                        {
                            auto repetVaule = result.first;
                            repetVaule++;
                            if(*it == *repetVaule){
                                tmp += *it;

                            }
                            else
                                break;
                        }
                        return{
                            result.first,
                            result.first + std::distance(tmp.begin(), tmp.end())
                        };
                    }
                
                next = childnodes.front()->eval(first, last, digger);
                if(next.first == next.last) return result;
            }
            
            if(size > 0)
                return result;
            
        }
        return {
            last,
            last
        }; 
};
    void print() override{std::cout << "Star" << std::endl;};
};
//
class NodePlus : public Node
{
    Pair eval(constIterator first, constIterator last, Digger &digger) override{
        Pair lhs;
        Pair rhs;
        
        lhs = childnodes.front()->eval(first, last, digger);

        
        rhs = childnodes.back()->eval(first, last, digger);
        if(lhs.first < rhs.first)
        {
            return lhs;
        }
        return rhs;
        
    };
    void print() override{std::cout << "Plus" << std::endl;};
};

class NodeBackslash_I : public Node{

    public:
        Pair eval(constIterator first, constIterator last, Digger &digger) override{
            

            digger.CanIGetBig = true;
            
            Pair result = childnodes.front()->eval(first, last, digger);
            
            digger.CanIGetBig = false;
            
            return result;
        };
        void print() override{std::cout << "\\I" << std::endl;};
};
class NodeBackslash_O : public Node{
public:
    NodeBackslash_O(int value) : value(value) {};
    Pair eval(constIterator first, constIterator last, Digger &digger) override{
        if(value == 0) {
            return childnodes.front()->eval(first, last, digger);
        }
        if(value > digger.parrenVec.size() ) {
            std::cerr << "there" << std::endl;

            return { last, last };
        }
        digger.index = value - 1;
        digger.parrenVec[digger.index] = {last , last };
        auto res = childnodes.front()->eval(first, last, digger);
        if(res.first == res.last && res.first == last) {
            return { last, last };
        }
        return digger.parrenVec[value - 1];
    }
    void print() override{std::cout << "\\O" << std::endl;};
    int value;
};

class NodeDot : public Node{

public:
    NodeDot(const std::string &str) : value(str){};
    Pair eval(constIterator first, constIterator last, Digger &digger) override{
        digger.ChildIsDot = false;
        digger.ParentIsStar = false;
        if(first == last) {
            return {
                last,
                last
            };
        }
        return {
            first,
            first + 1
        };
    };
    void print() override{std::cout << "Dot" << std::endl;};
    std::string value;
};

class NodeParens : public Node{

public:
    NodeParens(int value) : index(value) {};
    Pair eval(constIterator first, constIterator last, Digger &digger) override{
        Pair result = childnodes.front()->eval(first, last, digger);
        if(result.first == first && result.last == last) {
            digger.wasGreddy[index] = true;
        }
        
        if(digger.parrenVec[index].first == digger.parrenVec[index].last) {
            digger.parrenVec[index] = result;
        }

        digger.visitedGroup = true;
        digger.visitedIndex = index;
        return result;

    };
    void print() override{std::cout << "Parre" << std::endl;};
    int index;
};
/*class NodeGlue : public Node{

public:
    Pair eval(constIterator first, constIterator end, Digger &runner) override{
        Pair sum;
        return sum;
    };
    void print() override{std::cout << "Glue" << std::endl;};
};*/

class NodeString : public Node{

    public:
        NodeString(const std::string &str) : value(str){};
    
        Pair eval(constIterator first, constIterator last, Digger &digger) override{

            Pair result;
            digger.ChildIsDot = false;
            std::string test(first, last);
            if(digger.CanIGetBig == true)
            {
                for (auto & v: test) v = toupper(v);
                for (auto & c: value) c = toupper(c);
            }
            auto pos = test.find(value);
            if(value == "love")
                std::cout << "Hello" << std::endl;
            if(pos == std::string::npos) {
                return {
                    last,
                    last
                };
            }
                    
            result.first = first + pos;
            result.last = result.first + value.length();
            return result;
        };
        void print() override{std::cout << value << std::endl;};
        std::string value;
};

class Parser
{
    public:
    ChildNode parseToken(Tokens &&tokens, Digger *digger);
    Token *findTokens(Type token);
    ChildNode buildGlue();
    
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
    return buildGlue();
}

Token *Parser::findTokens(Type token){

    if (tokeniterator == tokens.end() || tokeniterator->type != token){
        return nullptr;
    }
    return &*(tokeniterator++);
}
ChildNode Parser::buildGlue(){
    ChildNode glue = std::make_unique<NodeGlue>();
    
    
    while(tokeniterator != tokens.end() ) {
        
        ChildNode parent = buildBackSlash_O();
        if(parent) {
            if(tokeniterator == tokens.end() ) {
                parent->addChildNode(std::move(glue));
                return parent;
            }
            return nullptr;
        } if (tokeniterator == tokens.end())
            break;

            
        if(glue->childnodes.size() == 2)
        {
            ChildNode parent = std::make_unique<NodeGlue>();
            parent->addChildNode(std::move(glue));
            glue = std::move(parent);
        }
        
        ChildNode node = buildValue();
        if(!node) {
            if(!canStar || glue->childnodes.empty() ) {
                return nullptr;
            }
            return glue;
        }
        ChildNode un = buildUnary(node);
        while(un) {
            node = std::move(un);
            un = buildUnary(node);
        }
        
        glue->addChildNode(std::move(node));
        
        ChildNode bin = buildBinary(glue);
        if(bin) {
            return bin;
        }
    }
    if(glue->childnodes.empty() ) {
        return nullptr;
    }
   
    return glue;
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
        std::cout << tok->value << std::endl;
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
    
    ChildNode child = buildGlue();
    if(!child) {
        tokeniterator--;
        return nullptr;
    }
    
    if(!findTokens(Type::RIGHT_PAR)) {
        return nullptr;
    }
    
    ChildNode parres = std::make_unique<NodeParens>(index++);
    digger->parrenVec.emplace_back();
    digger->wasGreddy.push_back(false);
    parres->addChildNode(std::move(child));
    return parres;
}

ChildNode Parser::buildUnary(ChildNode &child) {
    ChildNode unary = nullptr;
    
    if(canStar) {
        unary = buildStar(child);
    }
    
    if(unary) {
//        canStar = false;
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
    
    ChildNode rhs = buildGlue();
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

#endif /* tokenTree3_0_h */
