/*
                \I,*,+,.
                   ^
                   |
PROGRAM : Str, . , ( går att börja med.
        |--------|
             V 
            \I ,*,+,.,(
                 |-|
                  V
                  Str, . , (   

*/
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
    virtual Result eval(constIterator begin, constIterator end, Digger &digger) = 0;
    virtual void print() = 0;
    void addChildNode(ChildNode childnode);
    std::vector<ChildNode> childnodes;
};
class NodeBase : public Node
{
    Result eval(constIterator begin, constIterator end,Digger &runner) override{
        Result result;
        if(childnodes[runner.level] != 0)
             result = childnodes[runner.level++]->eval(begin,end,runner);
        if(runner.level == childnodes.size()) runner.level--;
        return result;

        Result total;
        return total;
    };
    void print() override{std::cout << "Glue" << std::endl;};
};
class NodeBrack : public Node
{
    Result eval(constIterator begin, constIterator end, Digger &runner) override{
        Result sum;
        return sum;
    };
    void print() override{std::cout << "Brack" << std::endl;}
};
class NodeStar : public Node
{
    Result eval(constIterator begin, constIterator end, Digger &runner) override{
        Result sum;
        return sum;
    };
    void print() override{std::cout << "Star" << std::endl;};
};
class NodePlus : public Node
{
    Result eval(constIterator begin, constIterator end, Digger &runner) override{
        Result sum;
        return sum;
    };
    void print() override{std::cout << "Plus" << std::endl;};
};

class NodeBackslash_I : public Node{

    public:
        Result eval(constIterator begin, constIterator end, Digger &digger) override{
//            if(childnodes[digger.level++] != nullptr)
//            Result result = childnodes[digger.level++]->eval(begin,end,digger);
            Result sum;
            return sum;
        };
        void print() override{std::cout << "\\I" << std::endl;};
};
class NodeBackslash_O : public Node{

    Result eval(constIterator begin, constIterator end, Digger &runner) override{
        Result sum;
        return sum;
    };
    void print() override{std::cout << "\\O" << std::endl;};
};

class NodeDot : public Node{

public:
    NodeDot(const std::string &str) : value(str){};
    Result eval(constIterator begin, constIterator end, Digger &runner) override{
        
        Result result;
        std::string test(begin,end);
        
        Result sum;
        return sum;
    };
    void print() override{std::cout << "Dot" << std::endl;};
    std::string value;
};

class NodeParens : public Node{

public:
    Result eval(constIterator begin, constIterator end, Digger &runner) override{
        Result sum;
        return sum;
    };
    void print() override{std::cout << "Parre" << std::endl;};
};
class NodeGlue : public Node{

public:
    Result eval(constIterator begin, constIterator end, Digger &runner) override{
        Result sum;
        return sum;
    };
    void print() override{std::cout << "Glue" << std::endl;};
};

class NodeString : public Node{

    public:
        NodeString(const std::string &str) : value(str){};
    
        Result eval(constIterator begin, constIterator end, Digger &digger) override{

            Result result;
            std::string test(begin, end);

//            for(auto it = begin; it <= end; it++){
//                test += *it;
//            }
            for(std::string::size_type pos = 0; pos<test.size(); pos+=value.size())
            {
                
                pos = test.find(value,pos);
                if(pos == std::string::npos)
                    break;
                Pair tmp;
                tmp.first = begin + pos;
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
    ChildNode buildBase();
    ChildNode buildTree();
    ChildNode buildPlus(ChildNode &childNode);
    ChildNode buildStar(ChildNode &childNode);
    ChildNode buildParres();
    ChildNode buildBrack(ChildNode &childNode);
    ChildNode buildBackSlash_I(ChildNode &childNode);
    ChildNode buildBackSlash_O();
    ChildNode buildString(const std::string &string);
    ChildNode buildGlue();
//    ChildNode buildDot();
    ChildNode buildDot(const std::string &string);
        Tokens tokens;
        TokenIterator tokeniterator;
};

void Node::addChildNode(ChildNode childNode){

    childnodes.push_back(std::move(childNode));
    
}

ChildNode Parser::parseToken(Tokens &&token){

    this->tokens = std::move(token);
    this->tokeniterator = this->tokens.begin();
    return buildBase();
}

Token *Parser::findTokens(Type token){

    if (tokeniterator == tokens.end() || tokeniterator->type != token){
        return nullptr;
    }
    return &*(tokeniterator++);
}
ChildNode Parser::buildBase(){

    ChildNode base = std::make_unique<NodeBase>();
    while (true){
        
        ChildNode block = buildTree();
        if (!block)
            return nullptr;
        
        base->addChildNode(std::move(block));
        
        if(findTokens(Type::BACKSLASH_O)) {
            block = buildBackSlash_O();
            for(auto &kid : base->childnodes) {
                block->addChildNode(std::move(kid));
            }
            base->childnodes.clear();
            base->addChildNode(std::move(block));
        } if (tokeniterator == tokens.end())
        break;
    }
    return base;
}

ChildNode Parser::buildTree(){
    Token *token;
    
    ChildNode childNode = nullptr;
    token = findTokens(Type::STRING);
    
    if(token){

        childNode = buildString(token->value);
    }
    
    if(!token) {
        token = findTokens(Type::DOT);
        if(token && !childNode)
            childNode = buildDot(token->value);
        
    }
    if(!token && findTokens(Type::LEFT_PAR)) {
        childNode = buildParres();
    }

    if(childNode){
        
        if (findTokens(Type::BACKSLASH_I))
        {
            ChildNode parentNode = buildBackSlash_I(childNode);
            return parentNode;
        }
        if (findTokens(Type::PLUS))
        {
            ChildNode parentNode = buildPlus(childNode);
            return parentNode;
        }
        if (findTokens(Type::STAR))
        {
            ChildNode parentNode = buildStar(childNode);
            return parentNode;
        }
        if (findTokens(Type::BRACKETS))
        {
            ChildNode parentNode = buildBrack(childNode);
            return parentNode;
        }
        
        return childNode;
    }
    
    
    return nullptr;
}

ChildNode Parser::buildBackSlash_I(ChildNode &childNode){

    ChildNode backSlash_I = std::make_unique<NodeBackslash_I>();
    backSlash_I->addChildNode(std::move(childNode));
    ChildNode parentNode = 0;
    
    if(findTokens(Type::STAR))
        parentNode = buildStar(backSlash_I);
    if(findTokens(Type::PLUS))
       parentNode = buildPlus(backSlash_I);
    if(parentNode)
        return parentNode;

    return backSlash_I;
}
/*ChildNode Parser::buildGlue(){
    
    ChildNode glueChild = buildTree();
    ChildNode glue = std::make_unique<NodeGlue>();
    glue ->addChildNode(std::move(glueChild));
    
    ChildNode rhsChild = buildTree();
    glue->addChildNode(std::move(rhsChild));
    
    ChildNode glueParentNode = 0;
    if(findTokens(Type::BACKSLASH_I))
        glueParentNode = buildBackSlash_I(glue);
    if(findTokens(Type::PLUS))
        glueParentNode = buildPlus(glue);
    if(findTokens(Type::STAR))
        glueParentNode = buildStar(glue);
    if(glueParentNode)
    {
        return glueParentNode;
    }

    return glue;
}*/

ChildNode Parser::buildParres(){

    ChildNode parreChild = buildTree();

    if(!findTokens(Type::RIGHT_PAR))
    {
        return nullptr;
    }

    ChildNode parre = std::make_unique<NodeParens>();
    parre ->addChildNode(std::move(parreChild));
    
    ChildNode parreParentNode = 0;

    if(findTokens(Type::BACKSLASH_I))
        parreParentNode = buildBackSlash_I(parre);
    if(findTokens(Type::PLUS))
        parreParentNode = buildPlus(parre);
    if(findTokens(Type::STAR))
        parreParentNode = buildStar(parre);
    
    if(parreParentNode)
    {
        return parreParentNode;
    }
    return parre;
}
/*ChildNode Parser::buildDot(){
    
    ChildNode dotChild = buildTree();
    if(tokens.size() == 1){
        ChildNode dot = std::make_unique<NodeDot>();
        return dot;
        
    }
    
    ChildNode dot = std::make_unique<NodeDot>();
    dot ->addChildNode(std::move(dotChild));
    
    ChildNode dotParentNode = 0;

    if(findTokens(Type::BACKSLASH_I))
        dotParentNode = buildBackSlash_I(dot);
    if(findTokens(Type::PLUS))
        dotParentNode = buildPlus(dot);
    if(findTokens(Type::STAR))
        dotParentNode = buildStar(dot);

    if(dotParentNode)
        return dotParentNode;
    
    return dot;
}*/

ChildNode Parser::buildPlus(ChildNode &childNode){

    ChildNode plus = std::make_unique<NodePlus>();
    plus->addChildNode(std::move(childNode));
    
    ChildNode rhs;
    /*if(findTokens(Type::STRING))
       rhs = buildGlue();*/
       rhs = buildTree();
    if(!rhs) return nullptr;
    plus->addChildNode(std::move(rhs));
    

    return plus;
}
ChildNode Parser::buildBrack(ChildNode &childNode)
{
    ChildNode brack = std::make_unique<NodeBrack>();
    brack->addChildNode(std::move(childNode));
    ChildNode parentNode;

    if(findTokens(Type::BACKSLASH_I))
        parentNode = buildBackSlash_I(brack);
    if(findTokens(Type::PLUS))
        parentNode = buildPlus(brack);
    if(parentNode)
        return parentNode;
    return brack;
}
ChildNode Parser::buildStar(ChildNode &childNode){


    ChildNode star = std::make_unique<NodeStar>();
    star->addChildNode(std::move(childNode));
    ChildNode parentNode = 0;
    if(findTokens(Type::BACKSLASH_I)) 
        parentNode = buildBackSlash_I(star);
    if(findTokens(Type::PLUS))
        parentNode = buildPlus(star);
    if(parentNode)
        return parentNode;

    return star;
}

ChildNode Parser::buildString(const std::string &string)
{
    return std::make_unique<NodeString>(string);
}
ChildNode Parser::buildDot(const std::string &string)
{
    return std::make_unique<NodeDot>(string);
}

ChildNode Parser::buildBackSlash_O(){
    
    ChildNode backSlash_O = std::make_unique<NodeBackslash_O>();
    return backSlash_O;
}
