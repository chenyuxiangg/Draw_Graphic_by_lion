#ifndef CYUFA_H
#define CYUFA_H

#include "Struct.h"
#include "Lexical_analyzer.h"
#include "UI.h"
#include <stdarg.h>

class CYufa{
	public:
		CYufa();
		~CYufa();
		
		void Parser(char* scrFileprt);
		void Program();
		void Statement();
		void OriginStatement();
		void RotStatement();
		void ScaleStatement();
		void ForStatement();
		
		ExprNode* Expression();
		ExprNode* Term();
		ExprNode* Factor();
		ExprNode* Component();
		ExprNode* MakeExprNode(Token_type opcode,...);
		ExprNode* Atom();
		
		//¸¨Öúº¯Êý
		void FetchToken ();
		void MatchToken(Token_type token_t);
		void SyntaxError(int errornu);
		void PrintSyntaxTree(ExprNode* root,int indent);
		double& GetParameter();
		
	private:
		Clexical_Analyzer* m_lexical;
		Token m_token;
		double Parameter;
	
};

#endif
