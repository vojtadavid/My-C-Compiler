%option noyywrap
%option yylineno

%{
#include "tab_keywords_flex.h"

%}

DIGIT    [0-9]
ID	    [a-z][a-z0-9]*
L	[a-zA-Z_]
%%

%"printf"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_PRINTF; }
%"scanf"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_SCANF;}
"typedef"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_TYPEDEF;}
"extern"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_EXTERN;}
"static"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_STATIC;}
"auto"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_AUTO;}
"register"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_REGISTER;}
"void"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_VOID; }
"char"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_CHAR;}
"short"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_SHORT;}
"int"			{/*printf("keyword %s\n",yytext);*/	return FLEX_KEYWORD_INT; }
"long"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_LONG;}
"float"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_FLOAT;}
"double"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_DOUBLE;}
"signed"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_SIGNED;}
"unsigned"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_UNSIGNED;}
"_Bool"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_BOOL;}
"_Complex"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_COMPLEX;}
"struct"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_STRUCT;}
"union"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_UNION;}
"const"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_CONST;}
"restrict"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_RESTRICT;}
"volatile"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_VOLATILE;}
"sizeof"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_SIZEOF;}
"enum"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_ENUM;}
"inline"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_INLINE;}
"default"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_DEFAULT;}
"if"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_IF;}
"else"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_ELSE;}
"switch"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_SWITCH;}
"while"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_WHILE;}
"do"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_DO;}
"for"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_FOR;}
"goto"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_GOTO;}
"continue"		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_CONTINUE;}
"case"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_CASE;}
"break"			{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_BREAK;}
"return" 		{/*printf("keyword %s\n",yytext);*/ return FLEX_KEYWORD_RETURN;}


{L}({L}|{DIGIT})*		{ /*printf("IDENTIFIER: %s\n",yytext );*/ return NAME_CONSTANT;}

{DIGIT}+   			{ /*printf( "An integer: %s (%d)\n", yytext, atoi( yytext ) );*/ return NUMBER_INTEGER;}
{DIGIT}+"."{DIGIT}*      { /*printf( "An float: %s (%d)\n", yytext, atoi( yytext ) );*/   return NUMBER_FLOAT;}


L?\"(\\.|[^\\"])*\"	{ return(STRING_LITERAL); }




"..."			{ /*printf("sign: %s\n",yytext);*/ return(ELLIPSIS); }
">>="			{ /*printf("sign: %s\n",yytext);*/ return(RIGHT_ASSIGN); }
"<<="			{ /*printf("sign: %s\n",yytext);*/ return(LEFT_ASSIGN); }
"+="			{ /*printf("sign: %s\n",yytext);*/ return(ADD_ASSIGN); }
"-="			{ /*printf("sign: %s\n",yytext);*/ return(SUB_ASSIGN); }
"*="			{ /*printf("sign: %s\n",yytext);*/ return(MUL_ASSIGN); }
"/="			{ /*printf("sign: %s\n",yytext);*/ return(DIV_ASSIGN); }
"%="			{ /*printf("sign: %s\n",yytext);*/ return(MOD_ASSIGN); }
"&="			{ /*printf("sign: %s\n",yytext);*/ return(AND_ASSIGN); }
"^="			{ /*printf("sign: %s\n",yytext);*/ return(XOR_ASSIGN); }
"|="			{ /*printf("sign: %s\n",yytext);*/ return(OR_ASSIGN); }
">>"			{ /*printf("sign: %s\n",yytext);*/ return(RIGHT_OP); }
"<<"			{ /*printf("sign: %s\n",yytext);*/ return(LEFT_OP); }
"++"			{ /*printf("sign: %s\n",yytext);*/ return(INC_OP); }
"--"			{ /*printf("sign: %s\n",yytext);*/ return(DEC_OP); }
"->"			{ /*printf("sign: %s\n",yytext);*/ return(PTR_OP); }
"&&"			{ /*printf("sign: %s\n",yytext);*/ return(AND_OP); }
"||"			{ /*printf("sign: %s\n",yytext);*/ return(OR_OP); }
"<="			{ /*printf("sign: %s\n",yytext);*/ return(LE_OP); }
">="			{ /*printf("sign: %s\n",yytext);*/ return(GE_OP); }
"=="			{ /*printf("sign: %s\n",yytext);*/ return(EQ_OP); }
"!="			{ /*printf("sign: %s\n",yytext);*/ return(NE_OP); }
";"				{ /*printf("sign: %s\n",yytext);*/ return(SEMICOLON); }
("{"|"<%")		{ /*printf("sign: %s\n",yytext);*/ return(LEFT_CURLY_BRACKET); }
("}"|"%>")		{ /*printf("sign: %s\n",yytext);*/ return(RIGHT_CURLY_BRACKET); }
","				{ /*printf("sign: %s\n",yytext);*/ return(','); }
":"				{ /*printf("sign: %s\n",yytext);*/ return(':'); }
"="				{ /*printf("sign: %s\n",yytext);*/ return(EQUALS_SIGN); }
"("				{ /*printf("sign: %s\n",yytext);*/ return(LEFT_PARENTHESIS); }
")"				{ /*printf("sign: %s\n",yytext);*/ return(RIGHT_PARENTHESIS); }
("["|"<:")		{ /*printf("sign: %s\n",yytext);*/ return('['); }
("]"|":>")		{ /*printf("sign: %s\n",yytext);*/ return(']'); }
"."				{ /*printf("sign: %s\n",yytext);*/ return('.'); }
"&"				{ /*printf("sign: %s\n",yytext);*/ return('&'); }
"!"				{ /*printf("sign: %s\n",yytext);*/ return('!'); }
"~"				{ /*printf("sign: %s\n",yytext);*/ return('~'); }
"-"				{ /*printf("sign: %s\n",yytext);*/ return('-'); }
"+"				{ /*printf("sign: %s\n",yytext);*/ return('+'); }
"*"				{ /*printf("sign: %s\n",yytext);*/ return('*'); }
"/"				{ /*printf("sign: %s\n",yytext);*/ return('/'); }
"%"				{ /*printf("sign: %s\n",yytext);*/ return('%'); }
"<"				{ /*printf("sign: %s\n",yytext);*/ return('<'); }
">"				{ /*printf("sign: %s\n",yytext);*/ return('>'); }
"^"				{ /*printf("sign: %s\n",yytext);*/ return('^'); }
"|"				{ /*printf("sign: %s\n",yytext);*/ return('|'); }
"?"				{ /*printf("sign: %s\n",yytext);*/ return('?'); }

"'"				{ /*printf("sign: %s\n",yytext);*/ return('\''); }
"'"				{ /*printf("sign: %s\n",yytext);*/ return('\''); }



[ \t\v\n\f]		{ }
.			{ puts("ignore bad character"); printf("%s\n",yytext ); }

%%
