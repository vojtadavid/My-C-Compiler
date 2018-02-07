//
// Created by vojta on 4.3.17.
//

#include <cassert>
#include "Parser.h"
#include "SymbolTable.h"

void Parser::relation_op(){
    switch(TOKEN){
        case AND_OP:
            eat(AND_OP);
            break;
        case OR_OP:
            eat(OR_OP);
            break;
        case LE_OP:
            eat(LE_OP);
            break;
        case GE_OP:
            eat(GE_OP);
            break;
        case EQ_OP:
            eat(EQ_OP);
            break;
        case NE_OP:
            eat(NE_OP);
            break;
        case '<':
            eat('<');
            break;
        case '>':
            eat('>');
            break;
        default:
            printf("relational_op ERROR %d\n",TOKEN);
    }

}

void Parser::Factor(float &value,SymbolType& st) {
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    if (TOKEN == NAME_CONSTANT ) {
        ID(st);
        return;
    }
    if(TOKEN=='-'){
        eat('-');
        Factor(value,st);
        if(st==INT){
            textSectionAssembly.push_back("\timulq\t$-1,%rax");
        }
        if(st==DOUBLE){
            textSectionAssembly.push_back("\tmovq\t$-1, %rax");
            textSectionAssembly.push_back("\tcvtsi2sd\t%rax, %xmm3");
            textSectionAssembly.push_back("\tmulsd\t%xmm3, %xmm0");
        }
        return;
    }
    if (TOKEN == NUMBER_FLOAT) {
//        value = std::stof(YYTEXT);
//        std::cout << value << '\n';
        st=DOUBLE;
        std::string label=getLabelConstant();
        rodataSectionAssembly.push_back(label+":\n\t.double\t" + std::string(YYTEXT));
        textSectionAssembly.push_back("\tmovsd\t"+label+"(%rip), %xmm0");
//        textSectionAssembly.push_back("\tsubq\t$8, %rsp");
//        textSectionAssembly.push_back("\tmovsd\t%xmm0, -8(%rsp)");
        eat(NUMBER_FLOAT);
        return;
    }
    if (TOKEN== NUMBER_INTEGER){
//        value = std::stoi(YYTEXT);
//        std::cout << value << '\n';
        st=INT;
        textSectionAssembly.push_back("\tmovq\t$" + std::string(YYTEXT) + ", %rax");

        eat(NUMBER_INTEGER);
        return;
    }

    if (TOKEN == LEFT_PARENTHESIS) {
        eat(LEFT_PARENTHESIS);
        float val;

        textSectionAssembly.push_back("\tpushq\t%rbx");
        textSectionAssembly.push_back("\tpushq\t%r12");

        textSectionAssembly.push_back("\tsubq\t$8, %rsp");
        textSectionAssembly.push_back("\tmovsd\t%xmm1, (%rsp)");

        textSectionAssembly.push_back("\tsubq\t$8, %rsp");
        textSectionAssembly.push_back("\tmovsd\t%xmm2, (%rsp)");

        arithmetic_expression(val,st);

        textSectionAssembly.push_back("\tmovsd\t(%rsp), %xmm2");
        textSectionAssembly.push_back("\taddq\t$8, %rsp");

        textSectionAssembly.push_back("\tmovsd\t(%rsp), %xmm1");
        textSectionAssembly.push_back("\taddq\t$8, %rsp");

        textSectionAssembly.push_back("\tpopq\t%r12");
        textSectionAssembly.push_back("\tpopq\t%rbx");
        value=val;

        eat(RIGHT_PARENTHESIS);
        return;
    }
    std::cout << "Bad factor\n";
    exit(EXIT_FAILURE);
}

void Parser::Expression_prime(float &inherited_value, float &synthesized_value,SymbolType& st) {
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    float TermValue;
    float E1inh;
    float E1syn;

    if (TOKEN == '+') {
        eat('+');
        if(st==DOUBLE){
            textSectionAssembly.push_back("\tmovsd\t%xmm0, %xmm1");
        }
        if(st==INT){
            textSectionAssembly.push_back("\tmovq\t%rax, %rbx");
        }
        SymbolType st2;
        Term(TermValue,st2);

        if(st==INT && st2==INT){
            textSectionAssembly.push_back("\taddq\t%rbx, %rax");
            st=INT;
//            goto label;
        }
        if(st==DOUBLE && st2==DOUBLE){
            textSectionAssembly.push_back("\taddsd\t%xmm1, %xmm0");
            st=DOUBLE;
//            goto label;
        }
        if(st==DOUBLE && st2==INT){
            textSectionAssembly.push_back("\tcvtsi2sd\t%rax, %xmm0");
            textSectionAssembly.push_back("\taddsd\t%xmm1, %xmm0");
            st=DOUBLE;
//            goto label;
        }
        if(st==INT && st2==DOUBLE){
            textSectionAssembly.push_back("\tcvtsi2sd\t%rbx, %xmm1");
            textSectionAssembly.push_back("\taddsd\t%xmm1, %xmm0");
            st=DOUBLE;
//            goto label;

        }
        //TODO bez goto
        label:
        E1inh = inherited_value + TermValue;

        Expression_prime(E1inh, E1syn,st);
        synthesized_value = E1syn;
        return;
    }
    if (TOKEN == '-') {
        eat('-');
        if(st==DOUBLE){
            textSectionAssembly.push_back("\tmovsd\t%xmm0, %xmm1");
        }
        if(st==INT){
            textSectionAssembly.push_back("\tmovq\t%rax, %rbx");
        }
        SymbolType st2;
        Term(TermValue,st2);
        if(st==INT && st2==INT){
            textSectionAssembly.push_back("\tsubq\t%rax, %rbx");
            textSectionAssembly.push_back("\tmovq\t%rbx, %rax");
            st=INT;
//            goto label2;
        }
        if(st==DOUBLE && st2==DOUBLE){
            textSectionAssembly.push_back("\tsubsd\t%xmm0, %xmm1");
            textSectionAssembly.push_back("\tmovsd\t%xmm1, %xmm0");
            st=DOUBLE;
//            goto label2;
        }
        if(st==DOUBLE && st2==INT){
            textSectionAssembly.push_back("\tcvtsi2sd\t%rax, %xmm0");
            textSectionAssembly.push_back("\tsubsd\t%xmm0, %xmm1");
            textSectionAssembly.push_back("\tmovsd\t%xmm1, %xmm0");
            st=DOUBLE;
//            goto label2;
        }
        if(st==INT && st2==DOUBLE){
            textSectionAssembly.push_back("\tcvtsi2sd\t%rbx, %xmm1");
            textSectionAssembly.push_back("\tsubsd\t%xmm0, %xmm1");
            textSectionAssembly.push_back("\tmovsd\t%xmm1, %xmm0");
            st=DOUBLE;
//            goto label2;
        }

        //TODO bez goto
        label2:
        E1inh = inherited_value - TermValue;

        Expression_prime(E1inh, E1syn,st);
        synthesized_value = E1syn;
        return;

    }

    synthesized_value= inherited_value;
}

void Parser::Term_prime(float &inherited_value, float &synthesized_value,SymbolType& st) {
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    float Fval;
    float T1inh;
    float T1syn;
    if (TOKEN == '*') {
        eat('*');
        if(st==DOUBLE){
            textSectionAssembly.push_back("\tmovsd\t%xmm0, %xmm2");
        }
        if(st==INT){
            textSectionAssembly.push_back("\tmovq\t%rax, %r12");
        }
        SymbolType st2;
        Factor(Fval,st2);

        if(st==INT && st2==INT){
            textSectionAssembly.push_back("\timulq\t%r12, %rax");
            st=INT;
//            goto label;
        }
        if(st==DOUBLE && st2==DOUBLE){
            textSectionAssembly.push_back("\tmulsd\t%xmm2, %xmm0");
            st=DOUBLE;
//            goto label;
        }
        if(st==DOUBLE && st2==INT){
            textSectionAssembly.push_back("\tcvtsi2sd\t%rax, %xmm0");
            textSectionAssembly.push_back("\tmulsd\t%xmm2, %xmm0");
            st=DOUBLE;
//            goto label;
        }
        if(st==INT && st2==DOUBLE){
            textSectionAssembly.push_back("\tcvtsi2sd\t%r12, %xmm2");
            textSectionAssembly.push_back("\tmulsd\t%xmm2, %xmm0");
            st=DOUBLE;
//            goto label;

        }
        T1inh = inherited_value*Fval;

        Term_prime(T1inh, T1syn,st);
        synthesized_value = T1syn;

        return;
    }
    if (TOKEN == '/') {
        eat('/');
        if(st==DOUBLE){
            textSectionAssembly.push_back("\tmovsd\t%xmm0, %xmm2");
        }
        if(st==INT){
            textSectionAssembly.push_back("\tmovq\t%rax, %rcx");
        }
        SymbolType st2;
        Factor(Fval,st2);
        if(st==INT && st2==INT){
            textSectionAssembly.push_back("\tmovl\t%eax, %edi");
            textSectionAssembly.push_back("\tmovl\t%ecx, %eax");
//            textSectionAssembly.push_back("\tmovq\t$0, %rdx");
//            textSectionAssembly.push_back("\tcdq");
            textSectionAssembly.push_back("\tcltd");
            textSectionAssembly.push_back("\tidivl\t%edi");
            st=INT;
        }
        if(st==DOUBLE && st2==DOUBLE){
            textSectionAssembly.push_back("\tdivsd\t%xmm0, %xmm2");
            textSectionAssembly.push_back("\tmovsd\t%xmm2, %xmm0");
            st=DOUBLE;
        }
        if(st==DOUBLE && st2==INT){
            textSectionAssembly.push_back("\tcvtsi2sd\t%rax, %xmm0");
            textSectionAssembly.push_back("\tdivsd\t%xmm0, %xmm2");
            textSectionAssembly.push_back("\tmovsd\t%xmm2, %xmm0");
            st=DOUBLE;
        }
        if(st==INT && st2==DOUBLE){
            textSectionAssembly.push_back("\tcvtsi2sd\t%rcx, %xmm2");
            textSectionAssembly.push_back("\tdivsd\t%xmm0, %xmm2");
            textSectionAssembly.push_back("\tmovsd\t%xmm2, %xmm0");
            st=DOUBLE;
        }

        T1inh = inherited_value*Fval;
        Term_prime(T1inh, T1syn,st);
        synthesized_value = T1syn;

        return;
    }
    if(TOKEN== '%'){
        eat('%');
        assert(st==INT);
        textSectionAssembly.push_back("\tmovq\t%rax, %rcx");

        SymbolType st2;
        Factor(Fval,st2);
        assert(st2==INT);
        textSectionAssembly.push_back("\tmovl\t%eax, %edi");
        textSectionAssembly.push_back("\tmovl\t%ecx, %eax");
//            textSectionAssembly.push_back("\tmovq\t$0, %rdx");
//            textSectionAssembly.push_back("\tcdq");
        textSectionAssembly.push_back("\tcltd");
        textSectionAssembly.push_back("\tidivl\t%edi");
        textSectionAssembly.push_back("\tmovl\t%edx, %eax");
        st=INT;

        return;
    }

}

void Parser::Term(float &Tval,SymbolType& st) {
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    float Fval;
    float Tinh;
    float Tsyn;


    Factor(Fval,st);
    Tinh = Fval;

    Term_prime(Tinh,Tsyn,st);
    Tval = Tsyn;
}

void Parser::arithmetic_expression(float &value,SymbolType& st) {
    float TermValue;
    float synthetized_value;
    float inherited_value;
//    textSectionAssembly.push_back("\txorq\t%rax, %rax");
//    textSectionAssembly.push_back("\tpxor\t%xmm0, %xmm0");
    Term(TermValue,st);
    Expression_prime(inherited_value,synthetized_value,st);

//    printf("%s term value: %f %f %f\n",__func__,TermValue,synthetized_value,inherited_value);
//    printf("%s arthm exp value: %f \n",__func__,TermValue+synthetized_value);

    value= TermValue+synthetized_value;
}

void Parser::printfStatement() {

}

void Parser::ID(SymbolType& st) {
    std::printf("ID %s\n",YYTEXT);
    std::string name = YYTEXT;
    float Evalue;

    auto s = symbolTable->lookup(name);
    st=s->type;
    eat(NAME_CONSTANT);
    if( (st==INTPOINTER || st==DOUBLEPOINTER) && TOKEN=='[' ){

        eat('[');
        float f;
        SymbolType symbolType;
        arithmetic_expression(f,symbolType);
        assert(symbolType==INT);
        textSectionAssembly.push_back("\tleaq\t0(,%rax,8), %rdx");
        textSectionAssembly.push_back("\tmov\t-" + std::to_string(s->locationOnstack) + "(%rbp), %rax");
        textSectionAssembly.push_back("\taddq\t%rdx, %rax");
        if(st==DOUBLEPOINTER) {
            textSectionAssembly.push_back("\tmovsd\t(%rax), %xmm0");
            st=DOUBLE;
        }
        if(st==INTPOINTER){
            textSectionAssembly.push_back("\tmovq\t(%rax), %rax");
            st=INT;
        }
        eat(']');
        return;
    }



    if (TOKEN == LEFT_PARENTHESIS) {

        textSectionAssembly.push_back("#saving register before calling function");
        textSectionAssembly.push_back("\tpushq\t%rbx");
        textSectionAssembly.push_back("\tpushq\t%r12");
//
        textSectionAssembly.push_back("\tsubq\t$8, %rsp");
        textSectionAssembly.push_back("\tmovsd\t%xmm1, (%rsp)");

        textSectionAssembly.push_back("\tsubq\t$8, %rsp");
        textSectionAssembly.push_back("\tmovsd\t%xmm2, (%rsp)");
        textSectionAssembly.push_back("#saving register before calling function");

        eat(LEFT_PARENTHESIS);
        std::vector<int> args;
        args.resize(6);
        auto it = args.end();
        it--;
        int tmp=0;
        function_arguments_list(it,tmp,s->argumentTypes,name);
        int i=0;
        int j=0;
        int noArg=0;
        int noFloatArg=0;
        for(auto& el : args){
            if(el==1) noArg++;
            if(el==2) noFloatArg++;
        }
        for(auto& el : args){
//            std::cout << el << " ";
            if(el==1){
                textSectionAssembly.push_back("\tpopq\t" + callingConventionReg[noArg-j-1]);
                j++;
            }
            if(el==2){
                std::string x = "%xmm" + std::to_string(noFloatArg-i-1);
                textSectionAssembly.push_back("\tmovsd\t(%rsp), %xmm"+std::to_string(noFloatArg-i-1));
                textSectionAssembly.push_back("\taddq\t$8, %rsp");
                i++;
            }
        }
        textSectionAssembly.push_back("\tmovl\t$"+std::to_string(noFloatArg)+", %eax");

//        if( (symbolTable->stackVariables)%16!=0){
//            textSectionAssembly.push_back("\tsubq\t$8, %rsp");
//        }
        textSectionAssembly.push_back("\tmovq\t %rsp, %r14");
        textSectionAssembly.push_back("\tandq\t$~0xF, %rsp");
        textSectionAssembly.push_back("\tcallq\t"+name+"@plt");
        textSectionAssembly.push_back("\tmovq\t%r14, %rsp");

//        if( (symbolTable->stackVariables)%16!=0){
//            textSectionAssembly.push_back("\taddq\t$8, %rsp");
//        }
        std::cout << '\n';
        eat(RIGHT_PARENTHESIS);

        textSectionAssembly.push_back("#restoring register after calling function");
        textSectionAssembly.push_back("\tmovsd\t(%rsp), %xmm2");
        textSectionAssembly.push_back("\taddq\t$8, %rsp");

        textSectionAssembly.push_back("\tmovsd\t(%rsp), %xmm1");
        textSectionAssembly.push_back("\taddq\t$8, %rsp");
//
        textSectionAssembly.push_back("\tpopq\t%r12");
        textSectionAssembly.push_back("\tpopq\t%rbx");
        textSectionAssembly.push_back("#restoring register after calling function");


        return;
    }
    std::string suffix;
    std::string reg;
    switch (s->size){
        case 2:
            suffix="b";
            reg="ax";
            break;
        case 4:
            suffix="l";
            reg="eax";
            break;
        case 8:
            suffix="q";
            reg="rax";
            break;
    }
    if(s->type==DOUBLE){
        textSectionAssembly.push_back("\tmovsd\t\-" + std::to_string(s->locationOnstack) + "(%rbp), %xmm0");
    } else {
//        textSectionAssembly.push_back(
//                "\tmov" + suffix + "\t-" + std::to_string(s->locationOnstack) + "(%rbp), %" + reg);
        textSectionAssembly.push_back("\tmov\t-" + std::to_string(s->locationOnstack) + "(%rbp), %rax");
    }


}

void Parser::local_declaration() {
    SymbolType t;
    type_specifier(t);
    std::string name = name_specifier();
    symbolTable->insert(new SymbolTable::Symbol(name,VARIABLE,t));
    auto s = symbolTable->lookup(name);

//    symbolTable->stackVariables+=s->size;
    symbolTable->stackVariables+=8;
    s->locationOnstack=symbolTable->stackVariables;

    std::printf("locdecl %d %d\n",symbolTable->stackVariables,s->size);

    if (TOKEN==';'){
        eat(';');
        //TODO
        textSectionAssembly.push_back("\tsubq\t$"+std::to_string(s->size)+", %rsp");
        return;
    }
    eat('=');
    if(TOKEN=='\''){
        eat('\'');

        textSectionAssembly.push_back("\tpush\t$" + std::to_string(int(YYTEXT[0])));
        eat(TOKEN);
        eat('\'');
        eat(';');
        return;
    }


    float result;
    SymbolType st;
    arithmetic_expression(result,st);

    if(st==DOUBLE) {
        textSectionAssembly.push_back("\tsubq\t$8, %rsp");
        textSectionAssembly.push_back("\tmovsd\t%xmm0, (%rsp)");

    } else {
        std::string suffix;
        std::string reg;
        switch (s->size){
            case 2:
                suffix="b";
                reg="ax";
                break;
            case 4:
                suffix="l";
                reg="eax";
                break;
            case 8:
                suffix="q";
                reg="rax";
                break;
        }
//        textSectionAssembly.push_back("\tsubq\t$"+std::to_string(s->size)+", %rsp");
//        textSectionAssembly.push_back("\tmov" + suffix + "\t-" + std::to_string(s->locationOnstack) + "%"+reg+", %rsp");
        textSectionAssembly.push_back("\tpushq\t%rax");
    }
    eat(';');
}

void Parser::relation_expression(bool AND) {
    float Evalue;
    int type;
    if (TOKEN == LEFT_PARENTHESIS) {
        eat(LEFT_PARENTHESIS);
        logical_or_expression();
        eat(RIGHT_PARENTHESIS);
        return;
    }
    //NON ZERO TRUE
    //ZERO FALSE
    std::string op;
    std::string opAND;
    std::string opOR;
    SymbolType st1;
    arithmetic_expression(Evalue, st1);
    if (st1 == INT) textSectionAssembly.push_back("\tmovq\t%rax, %r13");
    if (st1 == DOUBLE) {
        textSectionAssembly.push_back("\tsubq\t$8, %rsp\n"
                                              "\tmovsd\t%xmm0, (%rsp)");
        if (TOKEN == '<' || TOKEN == '>' || (TOKEN >= 218 && TOKEN <= 221)) {
            switch (TOKEN) {
                case LE_OP:
                    eat(LE_OP);
                    opAND = "ja";
                    opOR = "jbe";
                    break;
                case GE_OP:
                    eat(GE_OP);
                    opAND = "jb";
                    opOR = "jae";
                    break;
                case EQ_OP:
                    eat(EQ_OP);
                    opAND = "jnz";
                    opOR = "jz";
                    break;
                case NE_OP:
                    eat(NE_OP);
                    opAND = "jz";
                    opOR = "jnz";
                    break;
                case '<':
                    eat('<');
                    opAND = "jae";
                    opOR = "jb";
                    break;
                case '>':
                    eat('>');
                    opAND = "jbe";
                    opOR = "ja";
                    break;
                default:
                    printf("relational_op ERROR %d\n", TOKEN);
            }
        }
        SymbolType st2;
        arithmetic_expression(Evalue, st2);
        assert(st1 == st2);

        textSectionAssembly.push_back("\tmovsd\t(%rsp), %xmm1");
        textSectionAssembly.push_back("\taddq\t$8, %rsp");
        textSectionAssembly.push_back("\tucomisd\t%xmm0, %xmm1");

        if (TOKEN == OR_OP) {
            textSectionAssembly.push_back("\t" + opOR + " " + labelElseBranch);
        } else {
            textSectionAssembly.push_back("\t" + opAND + " " + labelElseBranch);
        }

        return;
    }
    if (TOKEN == '<' || TOKEN == '>' || (TOKEN >= 218 && TOKEN <= 221)) {
        switch (TOKEN) {
            case LE_OP:
                eat(LE_OP);
                opAND = "jg";
                opOR = "jle";
                break;
            case GE_OP:
                eat(GE_OP);
                opAND = "jl";
                opOR = "jge";
                break;
            case EQ_OP:
                eat(EQ_OP);
                opAND = "jne";
                opOR = "je";
                break;
            case NE_OP:
                eat(NE_OP);
                opAND = "je";
                opOR = "jne";
                break;
            case '<':
                eat('<');
                opAND = "jge";
                opOR = "jl";
                break;
            case '>':
                eat('>');
                opAND = "jle";
                opOR = "jg";
                break;
            default:
                printf("relational_op ERROR %d\n", TOKEN);
        }
    }
    SymbolType st2;
    arithmetic_expression(Evalue, st2);
    assert(st1 == st2);
    textSectionAssembly.push_back("\tcmpq\t%rax, %r13");

    if (TOKEN == OR_OP) {
        textSectionAssembly.push_back("\t" + opOR + " " + labelElseBranch);
    } else {
        textSectionAssembly.push_back("\t" + opAND + " " + labelElseBranch);
    }

    return;

}

void Parser::logical_and_expressionPrime(bool a) {
    if (TOKEN==AND_OP) {
        eat(AND_OP);
        relation_expression(a);
        logical_and_expressionPrime(true);
    }
}

void Parser::logical_and_expression(bool a) {
    relation_expression(a);
    logical_and_expressionPrime(a);
}

void Parser::logical_or_expressionPrime() {
    if (TOKEN==OR_OP) {
        eat(OR_OP);
        logical_and_expression(false);
        logical_or_expressionPrime();
    }
}

void Parser::logical_or_expression() {
    logical_and_expression(true);
    logical_or_expressionPrime();
}

void Parser::conditional_expression() {
    logical_or_expression();
}

void Parser::if_statement() {
    textSectionAssembly.push_back("#IF STATEMENT BEGIN");
    std::string tmpLabelElseBranch=labelElseBranch=getLabelJumps();
    std::string tmpLabelEndIF=labelEndIF=getLabelJumps();
    eat(FLEX_KEYWORD_IF);
    eat(LEFT_PARENTHESIS);
    conditional_expression();
    eat(RIGHT_PARENTHESIS);

    size_t tmpStackSize = symbolTable->stackVariables;
    openScope();
    symbolTable->stackVariables=tmpStackSize;
    compound_statement();
    textSectionAssembly.push_back("\taddq\t$"+std::to_string(symbolTable->stackVariables-tmpStackSize)+", %rsp");
    closeScope();
    textSectionAssembly.push_back("\tjmp\t"+tmpLabelEndIF);

    textSectionAssembly.push_back(tmpLabelElseBranch+":");
    if (TOKEN == FLEX_KEYWORD_ELSE) {

        eat(FLEX_KEYWORD_ELSE);
        size_t tmpStackSize = symbolTable->stackVariables;
        openScope();
        symbolTable->stackVariables=tmpStackSize;
        compound_statement();
        textSectionAssembly.push_back("\taddq\t$"+std::to_string(symbolTable->stackVariables-tmpStackSize)+", %rsp");
        closeScope();
    }
    textSectionAssembly.push_back(tmpLabelEndIF+":");
    textSectionAssembly.push_back("#IF STATEMENT END");

//    eat(FLEX_KEYWORD_IF);
//    eat(LEFT_PARENTHESIS);
//    std::string elseBranch = getLabelJumps();
//    std::string endIf = getLabelJumps();
//    std::string op;
////    conditional_expression();
//    float f;
//    SymbolType st;
//    arithmetic_expression(f,st);
//    assert(st==INT);
////    textSectionAssembly.push_back("\tmovq\t%rax, %rbx");
//    textSectionAssembly.push_back("\tpushq\t%rax");
//    switch(TOKEN) {
//        case LE_OP:
//            eat(LE_OP);
//            op = "jg";
//            break;
//        case GE_OP:
//            eat(GE_OP);
//            op = "jl";
//            break;
//        case EQ_OP:
//            eat(EQ_OP);
//            op = "jne";
//            break;
//        case NE_OP:
//            eat(NE_OP);
//            op = "je";
//            break;
//        case '<':
//            eat('<');
//            op = "jge";
//            break;
//        case '>':
//            eat('>');
//            op = "jle";
//            break;
//        default:
//            printf("relational_op ERROR %d\n", TOKEN);
//    }
//    arithmetic_expression(f,st);
//    assert(st==INT);
//    textSectionAssembly.push_back("\tcmpq\t%rax, (%rsp)");
//    textSectionAssembly.push_back("\tpopq\t%rax");
//    textSectionAssembly.push_back("\t"+op+" " + elseBranch);
//
//    eat(RIGHT_PARENTHESIS);
//    size_t tmpStackSize = symbolTable->stackVariables;
//    openScope();
//    symbolTable->stackVariables=tmpStackSize;
//    compound_statement();
//    textSectionAssembly.push_back("\taddq\t$"+std::to_string(symbolTable->stackVariables-tmpStackSize)+", %rsp");
//    closeScope();
//
//    textSectionAssembly.push_back("\tjmp "+endIf);
//    textSectionAssembly.push_back(elseBranch+":");
//
//    if (TOKEN == FLEX_KEYWORD_ELSE) {
//        eat(FLEX_KEYWORD_ELSE);
//        size_t tmpStackSize = symbolTable->stackVariables;
//        openScope();
//        symbolTable->stackVariables=tmpStackSize;
//        compound_statement();
//        textSectionAssembly.push_back("\taddq\t$"+std::to_string(symbolTable->stackVariables-tmpStackSize)+", %rsp");
//        closeScope();
//    }
//    textSectionAssembly.push_back(endIf+":");
}

void Parser::for_statement() {
    SymbolType st;
    eat(FLEX_KEYWORD_FOR);
    eat(LEFT_PARENTHESIS);
    type_specifier(st);
    name_specifier();
    eat('=');
    float exp;
    arithmetic_expression(exp,st);
    eat(';');

    arithmetic_expression(exp,st);
    relation_op();
    arithmetic_expression(exp,st);

    eat(';');
    name_specifier();
    eat('=');
    arithmetic_expression(exp,st);
    eat(RIGHT_PARENTHESIS);

    compound_statement();
}

void Parser::while_statement() {
    textSectionAssembly.push_back("#WHILE STATEMENT BEGIN");
    std::string tmplabelEndWhile=labelElseBranch=getLabelJumps();
    std::string condWhile=getLabelJumps();
    textSectionAssembly.push_back(condWhile+":");
    eat(FLEX_KEYWORD_WHILE);
    eat(LEFT_PARENTHESIS);
    conditional_expression();
    eat(RIGHT_PARENTHESIS);

    size_t tmpStackSize = symbolTable->stackVariables;
    openScope();
    symbolTable->stackVariables=tmpStackSize;
    compound_statement();
    textSectionAssembly.push_back("\taddq\t$"+std::to_string(symbolTable->stackVariables-tmpStackSize)+", %rsp");
    closeScope();
    textSectionAssembly.push_back("\tjmp " + condWhile);
    textSectionAssembly.push_back(tmplabelEndWhile+":");

    textSectionAssembly.push_back("#WHILE STATEMENT END");


//    eat(FLEX_KEYWORD_WHILE);
//    eat(LEFT_PARENTHESIS);
//    std::string op;
//    std::string condWhile=getLabelJumps();
//    std::string endWhile=getLabelJumps();
////    conditional_expression();
//
//    textSectionAssembly.push_back(condWhile+":");
//    float f;
//    SymbolType st;
//    arithmetic_expression(f,st);
//    assert(st==INT);
////    textSectionAssembly.push_back("\tmovq\t%rax, %rbx");
//    textSectionAssembly.push_back("\tmovq\t%rax, %r13");
//    switch(TOKEN) {
//        case LE_OP:
//            eat(LE_OP);
//            op = "jg";
//            break;
//        case GE_OP:
//            eat(GE_OP);
//            op = "jl";
//            break;
//        case EQ_OP:
//            eat(EQ_OP);
//            op = "jne";
//            break;
//        case NE_OP:
//            eat(NE_OP);
//            op = "je";
//            break;
//        case '<':
//            eat('<');
//            op = "jge";
//            break;
//        case '>':
//            eat('>');
//            op = "jle";
//            break;
//        default:
//            printf("relational_op ERROR %d\n", TOKEN);
//    }
//    arithmetic_expression(f,st);
//    assert(st==INT);
//    textSectionAssembly.push_back("\tcmpq\t%rax, %r13");
//    textSectionAssembly.push_back("\t"+op+" " + endWhile);
//    eat(RIGHT_PARENTHESIS);
//    size_t tmpStackSize = symbolTable->stackVariables;
//    openScope();
//    symbolTable->stackVariables=tmpStackSize;
//    compound_statement();
//    textSectionAssembly.push_back("\taddq\t$"+std::to_string(symbolTable->stackVariables-tmpStackSize)+", %rsp");
//    closeScope();
//    textSectionAssembly.push_back("\tjmp " + condWhile);
//    textSectionAssembly.push_back(endWhile+":");
}

void Parser::return_statement() {
    eat(FLEX_KEYWORD_RETURN);
    if(TOKEN==';'){
        eat(';');
    }else {
        float Evalue;
        SymbolType st;
        arithmetic_expression(Evalue, st);
        eat(';');
    }
    textSectionAssembly.push_back("\tmovq\t-32(%rbp), %r14\n"
            "\tmovq\t-24(%rbp), %r13\n"
            "\tmovq\t-16(%rbp), %r12\n"
            "\tmovq\t-8(%rbp), %rbx\n"
            "\tmovq\t%rbp, %rsp\n"
            "\tpopq\t%rbp\n"
            "\tret");
}

void Parser::break_statement() {
    eat(FLEX_KEYWORD_BREAK);
    eat(';');
}

void Parser::continue_statement() {
    eat(FLEX_KEYWORD_CONTINUE);
    eat(';');
}

void Parser::expression_statement() {
    SymbolType st;
    float Evalue;
//    arithmetic_expression(Evalue,st);
    std::string name=YYTEXT;
    auto s = symbolTable->lookup(name);
    if(s->kind==FUNCTION){
        ID(st);
        eat(';');
        return;
    }

    eat(NAME_CONSTANT);
    if(TOKEN=='['){
        assert(s->type==INTPOINTER || s->type==DOUBLEPOINTER);
        eat('[');
        arithmetic_expression(Evalue,st);
        eat(']');
        assert(st==INT);
        textSectionAssembly.push_back("\tleaq\t0(,%rax,8), %rdx\n"
                                    "\tmovq\t-" + std::to_string(s->locationOnstack) + "(%rbp), %rax\n"
                                    "\taddq\t%rdx, %rax\n"
                                    "\tpushq\t%rax\n");

        eat('=');
        if (s->kind == ARGUMENT || s->kind == VARIABLE) {

            arithmetic_expression(Evalue,st);

            textSectionAssembly.push_back("\tpopq\t%rbx");
            if (s->type == DOUBLEPOINTER) {
                textSectionAssembly.push_back("\tmovsd\t%xmm0, (%rbx)");
            } else {
                textSectionAssembly.push_back("\tmov\t%rax, (%rbx)");
            }

        }
        eat(';');
        return;

    }
    if (TOKEN == '=') {
        eat('=');
        if (s->kind == ARGUMENT || s->kind == VARIABLE) {

            arithmetic_expression(Evalue,st);
            assert(st==s->type);
            if (s->type == DOUBLE) {
                textSectionAssembly.push_back("\tmovsd\t%xmm0, -" + std::to_string(s->locationOnstack) + "(%rbp)");
            } else {
                textSectionAssembly.push_back("\tmov\t%rax, -" + std::to_string(s->locationOnstack) + "(%rbp)");
            }

        }
        eat(';');
    }

//    if (TOKEN == '='){
//        eat('=');
//        arithmetic_expression(Evalue,st);
//    }
//    eat(';');
}

void Parser::block_item() {
    if (TOKEN >= 107 && TOKEN <= 113) {
        local_declaration();
        return;
    }
    if (TOKEN == FLEX_KEYWORD_IF) {
        if_statement();
        return;
    }
    if (TOKEN == FLEX_KEYWORD_FOR) {
        for_statement();
        return;
    }
    if (TOKEN == FLEX_KEYWORD_WHILE) {
        while_statement();
        return;
    }
    if (TOKEN == FLEX_KEYWORD_BREAK) {
        break_statement();
        return;
    }
    if (TOKEN == FLEX_KEYWORD_CONTINUE) {
        continue_statement();
        return;
    }
    if (TOKEN == FLEX_KEYWORD_RETURN) {
        return_statement();
        return;
    }

    expression_statement();
}

void Parser::block_item_list() {
    if (TOKEN != RIGHT_CURLY_BRACKET) {
        block_item();
        block_item_list();
    }
}

void Parser::compound_statement() {
    eat(LEFT_CURLY_BRACKET);
    block_item_list();
    eat(RIGHT_CURLY_BRACKET);
}

void Parser::argument_declaration(std::string nameOfFun) {
    if (TOKEN!=RIGHT_PARENTHESIS){
        SymbolType t;
        type_specifier(t);
        auto fun = symbolTable->lookup(nameOfFun);
        fun->argumentTypes.push_back(t);
        std::string name = name_specifier();
        symbolTable->insert(new SymbolTable::Symbol(name,ARGUMENT,t));
        auto s  = symbolTable->lookup(name);
        symbolTable->stackVariables+=8;
        s->locationOnstack=symbolTable->stackVariables;
        if(t==DOUBLE) {
            textSectionAssembly.push_back("\tsubq\t$8, %rsp");
            textSectionAssembly.push_back("\tmovsd\t%xmm"+ std::to_string(argNoDouble++) +", (%rsp)");
        }else{
            textSectionAssembly.push_back("\tpushq\t"+ callingConventionReg[argObycejny++]);
        }

        if (TOKEN==','){
            eat(',');
            argument_declaration(nameOfFun);
        }
    }
}

void Parser::external_declarationX(SymbolType type, std::string name ) {

    if (TOKEN==LEFT_PARENTHESIS) {
        argObycejny=0;
        argNoDouble=0;

        textSectionAssembly.push_back(
                "\t.globl\t"+name+"\n"
                "\t.type\t"+name+", @function"
        );
        textSectionAssembly.push_back(name+":\n"
//                                              ".LFB"+std::to_string(labelFUN)+":\n"
                                              "\tpushq\t%rbp\n"
                                              "\tmovq\t%rsp, %rbp\n"
                                              "\tpushq\t %rbx\n"
                                              "\tpushq\t %r12\n"
                                              "\tpushq\t %r13\n"
                                              "\tpushq\t %r14\n"
        );

        symbolTable->insert(new SymbolTable::Symbol(name,FUNCTION,type));
        openScope();
        symbolTable->nameOfScopeOfFunction=name;
        symbolTable->returnType=type;
        symbolTable->calleeSavedRegSize+=4*8;
        symbolTable->stackVariables=symbolTable->calleeSavedRegSize;

        eat(LEFT_PARENTHESIS);
        argument_declaration(name);
        eat(RIGHT_PARENTHESIS);
        if(TOKEN==';'){
            eat(';');
            rodataSectionAssembly.clear();
            textSectionAssembly.clear();
            closeScope();
            return;
        }

        compound_statement();

        closeScope();

//        textSectionAssembly.push_back(".LFE"+std::to_string(labelFUN)+":");
        textSectionAssembly.push_back("\t.size "+name+", .-"+name);
        textSectionAssembly.push_back("\n");
        labelFUN++;


        for(auto &el : rodataSectionAssembly){
            *outputAssFile << el << '\n';
        }
        for(auto &el : textSectionAssembly){
            *outputAssFile << el << '\n';
        }
        rodataSectionAssembly.clear();
        textSectionAssembly.clear();
//        rodataSectionAssembly.push_back("\t.text");
//        textSectionAssembly.push_back("\t.section .text");
        return;
    }
    if (TOKEN==SEMICOLON){
        symbolTable->insert(new SymbolTable::Symbol(name,VARIABLE,type));
        eat(SEMICOLON);
        return;
    }
}

void Parser::external_declaration() {
    SymbolType t;
    type_specifier(t);
    std::string name= name_specifier();
    external_declarationX(t,name);


}

void Parser::translation_unitX() {
    if (TOKEN != 0) {
        external_declaration();
        translation_unitX();
    }
}

void Parser::translation_unit() {
    external_declaration();
    translation_unitX();
}

void Parser::type_specifier(SymbolType& type) {
    switch(TOKEN){
        case FLEX_KEYWORD_VOID:
            type=VOID;
            eat(FLEX_KEYWORD_VOID); break;
        case FLEX_KEYWORD_INT:
            type=INT;
            eat(FLEX_KEYWORD_INT); break;
        case FLEX_KEYWORD_DOUBLE:
            type=DOUBLE;
            eat(FLEX_KEYWORD_DOUBLE); break;
        case FLEX_KEYWORD_CHAR:
            type=CHAR;
            eat(FLEX_KEYWORD_CHAR); break;
        case FLEX_KEYWORD_UNSIGNED:
            type=UNSIGNED;
            eat(FLEX_KEYWORD_UNSIGNED); break;
        default:
        std::cout << "Unknown type\n";
            exit(EXIT_FAILURE);
    }
    if(TOKEN=='*'){
        if(type==INT) type=INTPOINTER;
        if(type==DOUBLE) type=DOUBLEPOINTER;
        if(type==VOID) type=VOIDPOINTER;
        eat('*');
    }
}

void Parser::function_arguments_list(std::vector<int>::iterator &ArgORder,int& noArg,std::vector<SymbolType>& argsTypes,std::string& nameOffun) {

    if (TOKEN != RIGHT_PARENTHESIS) {
        function_argument(ArgORder,noArg,argsTypes,nameOffun);
        if (TOKEN == ',') {
            eat(',');
            function_arguments_list(ArgORder,noArg,argsTypes,nameOffun);
        }
    }
}

void Parser::function_argument(std::vector<int>::iterator &ArgOrder,int& noArg,std::vector<SymbolType>& argsTypes,std::string& nameOffun) {
    noArg++;
    if (TOKEN == STRING_LITERAL) {
        std::string label=getLabelConstant();
        rodataSectionAssembly.push_back(label+":\n\t.string\t" + std::string(YYTEXT));
        textSectionAssembly.push_back("\tleaq\t" + label + "(%rip), %rax");
        textSectionAssembly.push_back("\tpushq\t%rax");
        eat(STRING_LITERAL);
        *ArgOrder=1;
        ArgOrder--;
        return;
    }
    float Evalue;
    SymbolType st;
    arithmetic_expression(Evalue,st);
//    if(nameOffun!="printf") {
//        std::cout << nameOffun <<" " << getStringSymbolTypeStatic(argsTypes[noArg]) << " " << getStringSymbolTypeStatic(argsTypes[noArg]) << '\n';
//        if(argsTypes[noArg] == VOIDPOINTER) {
//            assert(argsTypes[noArg] == VOIDPOINTER && (st == DOUBLEPOINTER || st == INTPOINTER || VOIDPOINTER));
//        } else{
//            assert(argsTypes[noArg] == st);
//        }
//    }

    if(st==DOUBLE){
        *ArgOrder=2;
        ArgOrder--;
        textSectionAssembly.push_back("\tsubq\t$8, %rsp");
        textSectionAssembly.push_back("\tmovsd\t%xmm0, (%rsp)");
//        assert()
    }
    else{
        *ArgOrder=1;
        ArgOrder--;
        textSectionAssembly.push_back("\tpushq %rax");
    }

}

//https://www3.nd.edu/~dthain/courses/cse40243/fall2015/intel-intro.html
//https://en.wikibooks.org/wiki/X86_Assembly