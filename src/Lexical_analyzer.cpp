#include "Lexical_analyzer.h"
#include <ctype.h>
#include <cstring>
#include <cmath>
#include <iostream>
using namespace std;

extern Token tokentable[] = {
{CONST_ID,"PI",3.1415926,nullptr},
{CONST_ID,"E",2.71828,nullptr},
{T,"T",0.0,nullptr},
{FUNC,"SIN",0.0,sin},
{FUNC,"COS",0.0,cos},
{FUNC,"TAN",0.0,tan},
{FUNC,"LN",0.0,log},
{FUNC,"EXP",0.0,exp},
{FUNC,"SQRT",0.0,sqrt},
{ORIGIN,"ORIGIN",0.0,nullptr},
{SCALE,"SCALE",0.0,nullptr},
{ROT,"ROT",0.0,nullptr},
{IS,"IS",0.0,nullptr},
{FOR,"FOR",0.0,nullptr},
{FROM,"FROM",0.0,nullptr},
{TO,"TO",0.0,nullptr},
{STEP,"STEP",0.0,nullptr},
{DRAW,"DRAW",0.0,nullptr}
};


int Clexical_Analyzer::m_count = 0;

Clexical_Analyzer::Clexical_Analyzer()
{
}

int Clexical_Analyzer::GetCount(){
	return m_count;
} 

Clexical_Analyzer::~Clexical_Analyzer()
{
	if(!m_vectok.empty())
	{
		for(auto v : m_vectok)
		{
			if(strcmp(v.lexeme,"PI") != 0 && strcmp(v.lexeme,"E"))
			{
				delete[] v.lexeme;
				v.lexeme = NULL;
			}
		}
	}
}

void Clexical_Analyzer::Analyze(char* oneline)
{
	char* pc = oneline;
	char* pl = pc;
	Token token;
	
	while(*pc)
	{
		//È¥µô¿Õ¸ñ 
		if(*pc == ' ')
		{
			pl = ++pc;
			continue;
		}
		if(isalpha(*pc))//ID
		{
			while(isalpha(*(++pc)));
			bool flag = false;
			char* tmp = new char[20];
			strncpy_s(tmp,20,pl,pc - pl);
			tmp[pc - pl] = '\0';
			for(int i = 0;i < 18;++i)
			{
				_strupr_s(tmp,strlen(tmp)+1);
				if(strcmp(tmp,tokentable[i].lexeme) == 0)
				{
					token = tokentable[i];
					token.lexeme = tmp;
					tmp = NULL;
					flag = true;
					break;
				}
			}
			if(!flag)
			{
				token.type = ERRTOKEN;
				token.lexeme = tmp;
				token.value = 0.0;
				token.FuncPtr = nullptr;
				tmp = NULL;
			}
			m_vectok.push_back(token);
			pl = pc;
		}
		else if(isdigit(*pc))//CONST_ID
		{
			char* tmp = new char[20];
			while(isdigit(*(++pc)));
			if(*pc == '.')
				while(isdigit(*(++pc)));
			strncpy_s(tmp,20,pl,pc - pl);
			tmp[pc - pl] = '\0';
			token.value = atof(tmp);
			token.type = CONST_ID;
			token.FuncPtr = nullptr;
			token.lexeme = tmp;
			m_vectok.push_back(token);
			pl = pc;
			tmp = NULL;
		}
		else if(*pc == '*')
		{ 
			char* tmp = new char[5];
			if(*(++pc) == '*')
			{
				++pc;
				strncpy_s(tmp,5,pl,pc - pl);
				tmp[pc - pl] = '\0';
				token.type = POWER;
				token.lexeme = tmp;
				token.value = 0.0;
				token.FuncPtr = nullptr;
			}
			else
			{
				strncpy_s(tmp,5,pl,pc - pl);
				tmp[pc - pl] = '\0';
				token.type = MUL;
				token.lexeme = tmp;
				token.value = 0.0;
				token.FuncPtr = nullptr;
			}
			m_vectok.push_back(token);
			tmp = NULL;
			pl = pc;
		}
		else if(*pc == '/')
		{
			char* tmp = new char[5];
			if(*(++pc) == '/')
			{
				strncpy_s(tmp,5,pl,pc - pl);
				tmp[pc - pl] = '\0';
				token.type = COMMENT;
				token.lexeme = tmp;
				token.value = 0.0;
				token.FuncPtr = nullptr;
			}
			else
			{
				strncpy_s(tmp,5,pl,pc - pl);
				tmp[pc - pl] = '\0';
				token.type = DIV;
				token.lexeme = tmp;
				token.value = 0.0;
				token.FuncPtr = nullptr;
			}
			m_vectok.push_back(token);
			tmp = NULL;
			pl = pc;
		}
		else if(*pc == '-')
		{
			char* tmp = new char[5];
			if(*(++pc) == '-')
			{
				strncpy_s(tmp,5,pl,pc - pl);
				tmp[pc - pl] = '\0';
				token.type = COMMENT;
				token.lexeme = tmp;
				token.value = 0.0;
				token.FuncPtr = nullptr;
			}
			else
			{
				strncpy_s(tmp,5,pl,pc - pl);
				tmp[pc - pl] = '\0';
				token.type = MINUS;
				token.lexeme = tmp;
				token.value = 0.0;
				token.FuncPtr = nullptr;
			}
			m_vectok.push_back(token);
			tmp = NULL;
			pl = pc;
		}
		else
		{
			char* tmp = new char[5];
			switch(*pc)
			{
				case '+':
					strncpy_s(tmp,5,pl,pc - pl + 1);
					tmp[pc - pl+1] = '\0';
					token.type = PLUS;
					token.lexeme = tmp;
					token.value = 0.0;
					token.FuncPtr = nullptr;
					m_vectok.push_back(token);
					tmp = NULL;
					pl = ++pc;
					break;
				case ',':
					strncpy_s(tmp,5,pl,pc - pl+1);
					tmp[pc - pl+1] = '\0';
					token.type = COMMA;
					token.lexeme = tmp;
					token.value = 0.0;
					token.FuncPtr = nullptr;
					m_vectok.push_back(token);
					tmp = NULL;
					pl = ++pc;
					break;
				case ';':
					strncpy_s(tmp,5,pl,pc - pl+1);
					tmp[pc - pl+1] = '\0';
					token.type = SEMICO;
					token.lexeme = tmp;
					token.value = 0.0;
					token.FuncPtr = nullptr;
					m_vectok.push_back(token);
					tmp = NULL;
					pl = ++pc;
					break;
				case '(':
					strncpy_s(tmp,5,pl,pc - pl+1);
					tmp[pc - pl+1] = '\0';
					token.type = L_BRACKET;
					token.lexeme = tmp;
					token.value = 0.0;
					token.FuncPtr = nullptr;
					m_vectok.push_back(token);
					tmp = NULL;
					pl = ++pc;
					break;
				case ')':
					strncpy_s(tmp,5,pl,pc - pl+1);
					tmp[pc - pl+1] = '\0';
					token.type = R_BRACKET;
					token.lexeme = tmp;
					token.value = 0.0;
					token.FuncPtr = nullptr;
					m_vectok.push_back(token);
					tmp = NULL;
					pl = ++pc;
					break;
				default:
					strncpy_s(tmp,5,pl,pc - pl+1);
					tmp[pc - pl+1] = '\0';
					token.type = ERRTOKEN;
					token.lexeme = tmp;
					token.value = 0.0;
					token.FuncPtr = nullptr;
					m_vectok.push_back(token);
					tmp = NULL;
					pl = ++pc;
					break;
					
					//token.type = ERRTOKEN;
					//token.lexeme = " ";
					//token.value = 0.0;
					//token.FuncPtr = nullptr;
					//m_vectok.push_back(token);
					//pl = ++pc;
					//if(*(pc+1) == '\0' && tmp)
						//delete[] tmp;
					//break;
			}
		}
		if(*pc == ' ')
		{
			pl = ++pc;
		}
	}
	pc = nullptr;
	pl = nullptr;
}

void Clexical_Analyzer::Show()
{
	if(m_vectok.empty())
	{
		cout << "m_vectokÎª¿Õ." << endl;
		return;
	}
	cout << "     type     lexeme     value     Func" << endl;
	cout << "----------------------------------------" << endl;
	for(auto v : m_vectok)
	{
		cout << "    " << v.type;
		cout << "    " << v.lexeme;
		cout << "    " << v.value;
		cout << "    " << v.FuncPtr;
		cout << endl;
	}
}

void Clexical_Analyzer::Clear()
{
	if(m_vectok.empty())
	{
		return;
	}
	for(auto v : m_vectok)
	{
		if(strcmp(v.lexeme,"PI") != 0 && strcmp(v.lexeme,"E") != 0)
		{
			delete[] v.lexeme; 
			v.lexeme = NULL;
		}
	}
	m_vectok.clear();
}

Token Clexical_Analyzer::GetToken(){
	int length = m_vectok.size();
	if(m_count < length)
		return m_vectok[m_count++];
	m_count = 0;
	Token t;
	t.FuncPtr = NULL;
	t.lexeme = NULL;
	t.type = TNULL;
	t.value = 0.0;
	return t;
}

