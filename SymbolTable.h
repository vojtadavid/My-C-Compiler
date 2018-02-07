//
// Created by vojta on 4.3.17.
//

#ifndef COMPILER_CMAKE_SYMBOLTABLE_H
#define COMPILER_CMAKE_SYMBOLTABLE_H


#include <string>
#include <list>
#include <map>
#include <ostream>
#include <experimental/optional>

enum SymbolKind  {FUNCTION,VARIABLE,ARGUMENT};
enum SymbolType {INT,DOUBLE,VOID,CHAR,UNSIGNED,INTPOINTER,DOUBLEPOINTER,VOIDPOINTER};



struct SymbolTable {
    struct Symbol{
        std::string name;
        SymbolKind kind;
        SymbolType type;
        size_t locationOnstack=0;
        size_t size=0;

        std::vector<SymbolType> argumentTypes;

        std::string getStringSymbolKind() const {
            switch(kind)
            {
                case FUNCTION: return "Function";
                case VARIABLE: return "Variable";
                case ARGUMENT: return "Argument";
                default:
                    return "FUCK";
            }
        }

        std::string getStringSymbolType() const{
            switch(type){
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


        Symbol(std::string n,SymbolKind symbolKind, SymbolType symbolType) : name(n),kind(symbolKind),type(symbolType){
            switch(symbolType){
                case INT:
                    size=4;
                    break;
                case DOUBLE:
                    size=8;
                    break;
                case VOID:
                    size=8;
                    break;
                case CHAR:
                    size=2;
                    break;
                case UNSIGNED:
                    size=8;
                    break;
                default:
                    size=0;

            }
        }

        Symbol(Symbol const& o){
            name=o.name;
            kind=o.kind;
            type=o.type;
            locationOnstack=o.locationOnstack;
            size=o.size;
        }
        Symbol(Symbol&& o){
            name=o.name;
            kind=o.kind;
            type=o.type;
            locationOnstack=o.locationOnstack;
            size=o.size;
        }

        friend std::ostream &operator<<(std::ostream &os, const SymbolTable::Symbol &symbol) {
            os << "name: " << symbol.name << " kind: " << symbol.getStringSymbolKind() << " type: " << symbol.getStringSymbolType();
            return os;
        }

    };
    SymbolTable(){

    }



    std::map<std::string,Symbol*> table;
    SymbolTable* parent= nullptr;
    size_t stackVariables=0;
    size_t calleeSavedRegSize=0;
    std::string nameOfScopeOfFunction;
    SymbolType returnType;

    Symbol* lookup(std::string name){
        auto foundSymbol = table.find(name);
        if(foundSymbol != table.end()) {
            return table[name];
        }
        else {
            if(parent!= nullptr){
                return parent->lookup(name);
            }
            std::printf("%s not found \n",name.c_str());
            exit(0);
        }
    }
    bool insert(Symbol* s){
        auto found = table.find(s->name);
        if (found !=table.end()){
            std::cout << "insertig symbol, ktery uz je v tabulce\n";
            exit(EXIT_FAILURE);
        } else {
            std::cout << "inserting symbol: " << s->name << '\n';
            table.insert(std::make_pair(s->name, s));
            return true;
        }
    }


    void print(){
        for(auto sym : table){
            std::cout << sym.first << " " << sym.second->getStringSymbolKind() << " " << sym.second->getStringSymbolType() << '\n';
        }

    }



};




#endif //COMPILER_CMAKE_SYMBOLTABLE_H
