//
// Created by vojta on 4.3.17.
//

#ifndef COMPILER_CMAKE_PARSER_H
#define COMPILER_CMAKE_PARSER_H

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <cassert>
#include "scanner.h"
#include "SymbolTable.h"
#include "tab_keywords_flex.h"


struct Parser {
    int TOKEN;
    int LINENO;
    char * YYTEXT;

    std::vector<std::string> rodataSectionAssembly;
    std::vector<std::string> textSectionAssembly;

    std::vector<std::string> callingConventionReg = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

    SymbolTable * symbolTable;
    size_t labelConstant=0;
    size_t labelJumps=0;
    size_t elementsOnStack = 0;
    bool sign=false;

    size_t argNoDouble=0;
    size_t argObycejny=0;

    size_t labelFUN=0;

    std::ofstream* outputAssFile;

    std::string labelElseBranch;
    std::string labelEndIF;

    std::string binaryOutputname="a.out";

    Parser(){
//        rodataSectionAssembly.push_back("\.text");
//        textSectionAssembly.push_back("\t.section .text");

//        bssSectionAssembly.push_back("\t.bss");
//        dataSectionAssembly.push_back("\t.data");
//        textSectionAssembly.push_back("\t.text");
        openScope();
        symbolTable->insert(new SymbolTable::Symbol("printf",FUNCTION,INT));
//        symbolTable->insert(new SymbolTable::Symbol("pow",FUNCTION,DOUBLE));
////        symbolTable->insert(new SymbolTable::Symbol("acos",FUNCTION,DOUBLE));
//        symbolTable->insert(new SymbolTable::Symbol("asin",FUNCTION,DOUBLE));
//        symbolTable->insert(new SymbolTable::Symbol("sin",FUNCTION,DOUBLE));
//        symbolTable->insert(new SymbolTable::Symbol("cos",FUNCTION,DOUBLE));
////        symbolTable->insert(new SymbolTable::Symbol("sqrt",FUNCTION,DOUBLE));
//        symbolTable->insert(new SymbolTable::Symbol("fmin",FUNCTION,DOUBLE));
//        symbolTable->insert(new SymbolTable::Symbol("fmax",FUNCTION,DOUBLE));
//        symbolTable->insert(new SymbolTable::Symbol("exp",FUNCTION,DOUBLE));
//        symbolTable->insert(new SymbolTable::Symbol("hypot",FUNCTION,DOUBLE));
//        symbolTable->insert(new SymbolTable::Symbol("malloc",FUNCTION,INT));
//        symbolTable->insert(new SymbolTable::Symbol("free",FUNCTION,INT));

    }
    ~Parser(){
        closeScope();
    }

    std::string getLabelConstant(){
        return ".LC" + std::to_string(labelConstant++);
    }
    std::string getLabelJumps(){
        return ".L" + std::to_string(labelJumps++);
    }

    void openScope(){
        SymbolTable * tmp = new SymbolTable();
        tmp->parent=this->symbolTable;
        symbolTable=tmp;
    }
    void closeScope(){
        SymbolTable * tmp = symbolTable->parent;
        delete(symbolTable);
        symbolTable=tmp;
    }

    void run(const std::string& f ){
        std::string nameAss = std::tmpnam(nullptr);
        std::cout << nameAss + ".s"+"\n";
        outputAssFile = new std::ofstream(nameAss + ".s");

        std::string name1 = std::tmpnam(nullptr);
        std::string cmd = std::string("gcc -fpreprocessed -E ") + f + " | sed '/^#/ d' > " + name1;
        std::cout << cmd + "\n";
        std::system(cmd.c_str());

        setYYINfile(name1.c_str());
        advance();
        std::cout << "starting parser ...\n";
        openScope();
        translation_unit();
        std::cout << "parser ended ...\n";
        closeScope();


        outputAssFile->close();
        delete(outputAssFile);
        std::system(("cat " + nameAss + ".s").c_str());
        //ld -e main hello.o -o hello -lc --dynamic-linker /lib64/ld-linux-x86-64.so.2
        std::system(("gcc -O0 -gstabs+ " + nameAss + ".s" +" -o " + binaryOutputname + " -lm").c_str());
//        std::system(("./"+binaryOutputname).c_str());
        std::cout << nameAss + ".s" << std::endl;

    }

    void setYYINfile(const char * f){
        yyin = std::fopen(f,"r");
        if(!yyin) {
            std::perror("File opening failed");
            exit(-1);
        }
    }
    void advance(){
        TOKEN = yylex();
        LINENO = yyget_lineno();
        YYTEXT = yytext;

    }
    void eat(int t){
        if (TOKEN==t) {
//            printf("%s\n",YYTEXT);
            advance();

        } else{
            printf("ERROR LINE: %d expected %c but is %d %s\n",LINENO,t,TOKEN,YYTEXT);
            assert(0);
        }
    }
    void test() {
        for (int i = 0; i < 10; i++) {
            yylex();
            std::cout << yytext << std::endl;
        }
    }
    std::string name_specifier(){
        std::string a = YYTEXT;
        eat(NAME_CONSTANT);
        return a;
    }

    void Factor(float& value,SymbolType& st);
    void Expression_prime(float &inherited_value, float &synthesized_value,SymbolType& st);
    void Term_prime(float& inherited_value, float& synthesized_value,SymbolType& st);
    void Term(float& Tval,SymbolType& st);
    void arithmetic_expression(float& value,SymbolType& st);


    void function_arguments_list(std::vector<int>::iterator &ArgORder,int&,std::vector<SymbolType>&,std::string&);
    void function_argument(std::vector<int>::iterator &ArgOrder,int&,std::vector<SymbolType>&,std::string&);
    void ID(SymbolType&);
    void relation_op();
    void local_declaration();
    void relation_expression(bool a);
    void logical_and_expressionPrime(bool a);
    void logical_and_expression(bool a);
    void logical_or_expressionPrime();
    void logical_or_expression();
    void conditional_expression();
    void if_statement();
    void for_statement();
    void while_statement();
    void return_statement();
    void break_statement();
    void continue_statement();
    void expression_statement();
    void block_item();
    void block_item_list();
    void compound_statement();
    void argument_declaration(std::string name);
    void external_declarationX(SymbolType type, std::string name);
    void external_declaration();
    void translation_unitX();
    void translation_unit();

    void type_specifier(SymbolType& type);

    void printfStatement();

    static std::string getStringSymbolTypeStatic(SymbolType t) {
        switch(t){
            case INT:
                return "int";
            case DOUBLE:
                return "double";
            case VOID:
                return "void";
            case CHAR:
                return "char";
            case UNSIGNED:
                return "unsigned";
            case VOIDPOINTER:
                return "void*";
            case INTPOINTER:
                return "int *";
            case DOUBLEPOINTER:
                return "double *";
            default:
                return "FUCK";
        }
    }
};


#endif //COMPILER_CMAKE_PARSER_H
