//
//  tokenTree2.0.h
//  Lab1Teori
//
//  Created by Fredrik Sellgren on 2020-02-13.
//  Copyright © 2020 Fredrik Sellgren. All rights reserved.
//

#ifndef tokenTree2_0_h
#define tokenTree2_0_h

#include "token.hpp"
#include <memory>
#include <utility>

class Node;
using ChildNode = std::unique_ptr<Node>;
using constIterator = std::string::const_iterator;


struct Digger{
    size_t level = 0;
};

struct Pair {
    constIterator first, last;
};
using Result = std::vector<Pair>;
class Node
{
public:
    virtual ~Node() = default;
    virtual Result eval(constIterator first, constIterator last, Digger &digger) = 0;
    virtual void print() = 0;
    void addChildNode(ChildNode childnode);
    std::vector<ChildNode> childnodes;
};
class NodeGlue : public Node
{
    Result eval(constIterator first, constIterator last,Digger &digger) override{
        /*
        Result result;
//        Result result2;
        result = childnodes[digger.level++]->eval(first,last,digger);
        if(digger.level == childnodes.size())  digger.level--;
        result = childnodes[digger.level]->eval(first,last,digger);
        
        
//        if(childnodes[digger.level] != 0)
//             result2 = childnodes[digger.level++]->eval(first,last,digger);*/
        Result result;
        
        
        return result;
        
    };
    void print() override{std::cout << "Glue" << std::endl;};
};
class NodeBrack : public Node
{
    Result eval(constIterator first, constIterator last, Digger &digger) override{
        Result sum;
        return sum;
    };
    void print() override{std::cout << "Brack" << std::endl;}
};
class NodeStar : public Node
{
    Result eval(constIterator first, constIterator last, Digger &digger) override{
        Result sum;
        return sum;
    };
    void print() override{std::cout << "Star" << std::endl;};
};
class NodePlus : public Node
{
    Result eval(constIterator first, constIterator last, Digger &digger) override{
        Result lhs;
        Result rhs;
        Result sum;
        
        lhs = childnodes[digger.level]->eval(first,last, digger);
        
        rhs = childnodes[digger.level++]->eval(first,last, digger);
        
        
        
        return rhs;
    };
    void print() override{std::cout << "Plus" << std::endl;};
};

class NodeBackslash_I : public Node{

    public:
        Result eval(constIterator first, constIterator last, Digger &digger) override{
//            if(childnodes[digger.level++] != nullptr)
//            Result result = childnodes[digger.level++]->eval(first,last,digger);
            Result sum;
            return sum;
        };
        void print() override{std::cout << "\\I" << std::endl;};
};
class NodeBackslash_O : public Node{

    Result eval(constIterator first, constIterator last, Digger &digger) override{
        Result sum;
        return sum;
    };
    void print() override{std::cout << "\\O" << std::endl;};
};

class NodeDot : public Node{

public:
    NodeDot(const std::string &str) : value(str){};
    Result eval(constIterator first, constIterator last, Digger &digger) override{
                Result result;

        std::string test(first, last);

        for(std::string::size_type pos = 0; pos<test.size(); pos+=value.size())
        {
            
            if(pos == std::string::npos)
                break;
            Pair tmp;
            tmp.first = first + pos;
            tmp.last = tmp.first + value.size();
            result.push_back(tmp);
        }
        
        return result;
    };
    void print() override{std::cout << "Dot" << std::endl;};
    std::string value;
};

class NodeParens : public Node{

public:
    Result eval(constIterator first, constIterator last, Digger &runner) override{
        Result sum;
        return sum;
    };
    void print() override{std::cout << "Parre" << std::endl;};
};
/*class NodeGlue : public Node{

public:
    Result eval(constIterator first, constIterator end, Digger &runner) override{
        Result sum;
        return sum;
    };
    void print() override{std::cout << "Glue" << std::endl;};
};*/

class NodeString : public Node{

    public:
        NodeString(const std::string &str) : value(str){};
    
        Result eval(constIterator first, constIterator last, Digger &digger) override{

            Result result;
            std::string test(first, last);

            for(std::string::size_type pos = 0; pos<test.size(); pos+=value.size())
            {
                
                pos = test.find(value,pos);
                if(pos == std::string::npos)
                    break;
                Pair tmp;
                tmp.first = first + pos;
                tmp.last = tmp.first + value.length();
                result.push_back(tmp);
            }
            
            return result;
        };
    
        void print() override{std::cout << value << std::endl;};
        std::string value;
};

class Parser
{
    public:
    ChildNode parseToken(Tokens &&tokens);
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
    //ChildNode buildBackSlash_O();
    ChildNode buildString();
    ChildNode buildDot();
    
    Tokens tokens;
    TokenIterator tokeniterator;
    bool canStar = true;
};

void Node::addChildNode(ChildNode childNode){

    childnodes.push_back(std::move(childNode));
    
}

ChildNode Parser::parseToken(Tokens &&token){

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
    
    ChildNode parres = std::make_unique<NodeParens>();
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
    if(!findTokens(Type::BRACKETS)) {
        return nullptr;
    }
    ChildNode brack = std::make_unique<NodeBrack>();
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
#endif /* tokenTree2_0_h */
