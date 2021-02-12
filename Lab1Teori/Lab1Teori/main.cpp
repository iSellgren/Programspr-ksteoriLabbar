#include <iostream>
#include <vector>
#include <string>
#include "token.hpp"
#include <cstdio>
#include <cstdlib>
//#include "tokenTree.hpp"
//#include "tokenTree2.0.h"
//#include "tokenTree3.0.h"
#include "tokenTree4.0.h"

#include <fstream>
/*
Prio lista
0 Program
1 O\{3}
2 \I
3 ()
4 +
5 * , {3}
6 str , .
*/
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
struct VisitTree
{
    VisitTree()
    {
        level++;
    }
    ~VisitTree()
    {
        level--;
    }
    static int level;
};
std::string ToggleColor(std::string &color)
{
    if(color == YELLOW)
        color = BLUE;
    else
        color = YELLOW;
    return color;
}
/*
void ColorPrintMatch(const std::string &input, Result result)
{
    std::string MatchColor;
    auto begin = input.begin();
    for(auto res : result) {
        if(res.first - begin != 0) {
            std::cout << std::string(begin, res.first);
        }
        if(Color == MAGENTA)
            Color = BLUE;
        else
            Color = MAGENTA;
        std::cout << ToggleColor(MatchColor) << std::string(res.first, res.last) << RESET;
        begin = res.last;
    }
}
*/

void ColorPrintMatch(std::string &color, Pair pair) {
    
    if(pair.first < pair.last){
        std::cout << color << std::string(pair.first, pair.last) << RESET;
        ToggleColor(color);
    }
}
int VisitTree::level = 0;

void visitNode(Node *node)
{
    VisitTree visittree;
    
    for(int i = 0; i < visittree.level - 1; i++) std::cout << '\t';
    node->print();

    for(auto &children : node->childnodes)
        visitNode(children.get() );


}
std::vector<Token> parse(const std::string &str)
{
    Tokenizer tokenizer;
    Tokens tokens =  tokenizer.tokenizer(str);
    return tokens;
}

int main(int argc, char* argv[])
{
   /* std::string input;
    std::string tokens;
    if(std::cin)
    if(argc > 1){
        tokens = argv[1];
        for (std::string line; std::getline(std::cin, line);) {

            input = line;
        }
    }*/
    
    //std::string tokens (".+.");
    //std::string tokens ("def.*");
//    std::string tokens ("Waterloo (.*)facing\\O{1}");
    //std::string tokens("(wa*)"); // Fungerar
    //std::string tokens("could.{105}");
    //std::string tokens("lo* could.{3}");
//    std::string tokens("lo.* could");
//   std::string tokens("Love\\I");
    //std::string tokens("w.*fin+at+(fa.).{5}");
    //std::string tokens ("I.*Water");
//    std::string tokens ("the war(.*) Waterloo");
//    std::string tokens ("promise to (Love+Hate) you\\O{1}"); //Fungerar
    //std::string tokens ("Waterloo (.*) Waterloo\\O{1}");
//    std::string tokens("I (.*) the war (.*) Waterloo \\O{2}");
    std::string tokens ("((love+hate)+Water)");
    //std::string tokens ("lo+wo");
    //std::string tokens("wa.");
    //std::string tokens(".*wa");
//   std::string tokens("wa.*Wa");
    //std::string tokens("war (Waterloo)");

    //Martins Xmpl
//    std::string tokens("lo* could.{3}"); //Fungerar
//    std::string tokens(".*"); //Fungerar
    //std::string tokens(".* "); //Fungerar
    //std::string tokens("promise to (Love+Hate) you\\O{1}"); //Fungerar ej, Ska inte fungera
    //std::string tokens("promise to (Love+Hate)\\I you\\O{1}"); //Fungerar
//    std::string tokens("promise to (love+hate) you\\O{1}"); //Fungerar
//      std::string tokens("(Love+Hate)\\I"); //Fungerar
    std::string input("Waterloo I was defeated, you won the war Waterloo promise to love you for ever more Waterloo couldn't escape if I wanted to Waterloo knowing my fate is to be with you Waterloo finally facing my Waterloo");
    //std::string tokens ("love*");
    //std::string tokens ("Waterloo+war");
    //std::string tokens (".*");

    std::vector<Token> test = parse(tokens);
    for(auto i : test)
        std::cout << i << std::endl;
    Parser parser;
    Digger digger;
    //digger.input = input;
    digger.inputFirst = digger.resultFirst = digger.resultLast = input.begin();
    digger.inputLast = input.end();
//    for(auto i = digger.inputFirst; i != digger.inputLast; i++)
//        std::cout << *i << std::endl;
    auto base = parser.parseToken(std::move(test), &digger);
    if(base.get()) {
        visitNode(base.get() );
    } else {
        std::cout << "Bad tree\n";
        std::cout << parser.tokeniterator->value << " : " << parser.tokeniterator - parser.tokens.begin() << std::endl;
        return EXIT_FAILURE;
    }
    auto it = input.cbegin();
//    auto result = base->eval(digger);
    std::string color = YELLOW;
    while(digger.resultLast < digger.inputLast && base->eval(digger)) {
        std::cout << std::string(it, digger.resultFirst);
        ColorPrintMatch(color, Pair{digger.resultFirst, digger.resultLast});
        it = digger.resultLast;
        //std::cout << std::string(digger.resultFirst, digger.resultLast);
        digger.resultFirst = digger.resultLast;
    }
    if(it < input.cend()) {
        std::cout << std::string(it, input.cend()) << '\n';
    } else {
        std::cout << '\n';
    }
    //auto prevLast = digger.result[0].last;
    
    
    
    /*
    
    if(result)
        std::cout << std::string(digger.inputFirst, digger.result[0].first);
    std::string color = YELLOW;
      while(digger.result[0].last != digger.inputLast) {
//          ColorPrintMatch(color, digger.result[0]);
          prevLast = digger.result[0].last;
          result = base->eval(digger);
          
          std::cout << std::string(prevLast,  digger.result[0].first);
      }
*/
        

    
    /*if(base.get()) {
        visitNode(base.get() );
    } else {
        std::cout << "Bad tree\n";
        std::cout << parser.tokeniterator->value << " : " << parser.tokeniterator - parser.tokens.begin() << std::endl;
        return EXIT_FAILURE;
    }

    auto result = base->eval(input.begin(),input.end(),digger);
   auto prevLast = result.last;
//    std::cout << *result.first << "first" <<  std::endl;
//    std::cout << *digger.parrenVec[0].first << "parrenFirst" << std::endl;
    std::cout << std::string(input.cbegin(), result.first);
    
    std::string color = YELLOW;
    while(result.last != input.end()) {
        ColorPrintMatch(color, result);
        prevLast = result.last;
        result = base->eval(result.last, input.end(),digger);
        
        std::cout << std::string(prevLast, result.first);
    }
    
    if(std::distance(result.first, result.last) == input.size() ) {
//        ColorPrintMatch(color, result);
        if(digger.ParentIsStar == true)
            ColorPrintMatch(color, result);
    }
//    if(digger.ParentIsStar == false && digger.ChildIsDot == false)
        ColorPrintMatch(color, result);
    if(digger.ParentIsStar == true)
        ColorPrintMatch(color, result);

    std::cout << std::endl << std::endl;
    
//    std::string test1 = input.substr(result[0]);
    
    */
    return EXIT_SUCCESS;
}
