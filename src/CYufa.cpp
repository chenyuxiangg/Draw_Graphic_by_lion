#include "CYufa.h"
#include "Cyuyi.h"
#include <cmath>
#include <iostream>
#include <windows.h>
using namespace std;

extern Token tokentable[];
extern Cyuyi* g_yuyi;

CYufa::CYufa() :Parameter(0.0) {
	m_lexical = new Clexical_Analyzer();
}

CYufa::~CYufa(){
	delete m_lexical;
	m_lexical = NULL;
	double Parameter = 0;
}

//分析一句话 
void CYufa::Parser(char* srcFileptr){
	m_lexical->Analyze(srcFileptr);
	FetchToken();
	Program();
	m_lexical->Clear(); 
}

void CYufa::FetchToken(){
	m_token = m_lexical->GetToken();
}

void CYufa::Program(){
	while(m_token.type != TNULL){
		Statement();
		MatchToken(SEMICO);
	}
}

void CYufa::Statement(){
	switch(m_token.type){
		case FOR:
			ForStatement();
			break;
		case ORIGIN:
			OriginStatement();
			break;
		case ROT:
			RotStatement();
			break;
		case SCALE:
			ScaleStatement();
			break;
		default:
			SyntaxError(m_lexical->GetCount());
			break;
	}
}

void CYufa::ForStatement(){
	ExprNode* start_ptr = NULL;
	ExprNode* end_ptr = NULL;
	ExprNode* step_ptr = NULL;
	ExprNode* x_ptr = NULL;
	ExprNode* y_ptr = NULL;
	
	double start = 0.0;
	double end = 0.0;
	double step = 0.0;
	
	FetchToken();
	MatchToken(T);
	MatchToken(FROM);		start_ptr = Expression(); 
	start = g_yuyi->GetExprValue(start_ptr);
	MatchToken(TO);			end_ptr = Expression();
	end = g_yuyi->GetExprValue(end_ptr);
	MatchToken(STEP);		step_ptr = Expression(); 
	step = g_yuyi->GetExprValue(step_ptr);
	MatchToken(DRAW);
	MatchToken(L_BRACKET);	x_ptr = Expression(); 
	MatchToken(COMMA);		y_ptr = Expression(); 
	MatchToken(R_BRACKET); 
	UI::Getinstance()->DrawLoop(start,end,step,x_ptr,y_ptr);
} 

void CYufa::OriginStatement(){
	ExprNode* x_ptr = NULL;
	ExprNode* y_ptr = NULL;
	
	FetchToken();
	MatchToken(IS);
	MatchToken(L_BRACKET);	x_ptr = Expression();
	double x = g_yuyi->GetExprValue(x_ptr);
	MatchToken(COMMA);		y_ptr = Expression();
	double y = g_yuyi->GetExprValue(y_ptr);
	MatchToken(R_BRACKET);
	g_yuyi->SetOrigin(x,y);
}

void CYufa::ScaleStatement(){
	ExprNode* x_ptr = NULL;
	ExprNode* y_ptr = NULL;
	
	FetchToken();
	MatchToken(IS);
	MatchToken(L_BRACKET);	x_ptr = Expression();
	double x = g_yuyi->GetExprValue(x_ptr);
	MatchToken(COMMA);		y_ptr = Expression();
	double y = g_yuyi->GetExprValue(y_ptr);
	MatchToken(R_BRACKET);
	g_yuyi->SetScale(x,y);
}

void CYufa::RotStatement(){
	ExprNode* rot_ptr = NULL;

	FetchToken();
	MatchToken(IS);			rot_ptr = Expression();
	double r = g_yuyi->GetExprValue(rot_ptr);
	g_yuyi->SetRot(r);
}

void  CYufa::MatchToken(Token_type token_t){
	if(token_t != m_token.type){
		SyntaxError(m_lexical->GetCount());
	}
	FetchToken();
}

ExprNode* CYufa::Expression(){
	ExprNode* left = NULL;
	ExprNode* right = NULL;
	Token_type token_tmp;
	left = Term();
	while(m_token.type == PLUS || m_token.type == MINUS){
		token_tmp = m_token.type;
		MatchToken(token_tmp);
		right = Term();
		left = MakeExprNode(token_tmp,left,right);
	} 
	return left;
}

ExprNode* CYufa::MakeExprNode(Token_type opcode,...){
	ExprNode* ExprPtr = new ExprNode();
	va_list ArgPtr;
	ExprPtr->OpCode = opcode;
	va_start(ArgPtr,opcode);
	switch(opcode){
		case CONST_ID:
			ExprPtr->Content.CaseConst = (double)va_arg(ArgPtr,double);
			break;
		case T:
			ExprPtr->Content.CaseParmPtr = &Parameter;
			break;
		case FUNC:
			ExprPtr->Content.CaseFunc.Child = (ExprNode*)va_arg(ArgPtr,ExprNode*);
			ExprPtr->Content.CaseFunc.MathFuncPtr = (FuncPtr)va_arg(ArgPtr,FuncPtr);
			break;
		default:
			ExprPtr->Content.CaseOperator.left = (ExprNode*)va_arg(ArgPtr,ExprNode*);
			ExprPtr->Content.CaseOperator.right = (ExprNode*)va_arg(ArgPtr,ExprNode*);
			break;
	}
	va_end(ArgPtr);
	return ExprPtr;
}

void CYufa::SyntaxError(int errornu){
	char err[50];
	WCHAR cerr[50];
	sprintf_s(err, sizeof(err), "错误位置：%d:%d", UI::GetLine(),errornu);
	MultiByteToWideChar(CP_ACP, 0, err, strlen(err) + 1, cerr, sizeof(cerr)/sizeof(cerr[0]));
	MessageBox(NULL,cerr,TEXT("error"),MB_OK);
}

ExprNode* CYufa::Term(){
	ExprNode* left = NULL;
	ExprNode* right = NULL;
	Token_type token_tmp;
	left = Factor();
	while(m_token.type == MUL || m_token.type == DIV){
		token_tmp = m_token.type;
		MatchToken(token_tmp);
		right = Factor();
		left = MakeExprNode(token_tmp,left,right);
	}
	return left;
}

ExprNode* CYufa::Factor(){
	ExprNode* left = NULL;
	ExprNode* right = NULL;
	Token_type token_tmp;
	if(m_token.type == PLUS || m_token.type == MINUS){
		left = MakeExprNode(CONST_ID,0.0);
		token_tmp = m_token.type;
		MatchToken(token_tmp);
		right = Factor();
		left = MakeExprNode(token_tmp,left,right);
		return left;
	}
	else{
		left = Component();
	}
	return left;
}

ExprNode* CYufa::Component(){
	ExprNode* left = NULL;
	left = Atom();
	if(m_token.type == POWER){
		Token_type token_tmp = m_token.type;
		MatchToken(token_tmp);
		ExprNode* right = Component();
		left = MakeExprNode(token_tmp,left,right);
	}
	return left;
}

ExprNode* CYufa::Atom(){
	ExprNode* ret = NULL;
	Token_type token_tmp = m_token.type;
	Token tmp = m_token;
	MatchToken(token_tmp);
	switch(token_tmp){
		case CONST_ID:
			return MakeExprNode(token_tmp,tmp.value);
		case T:
			return MakeExprNode(token_tmp,tmp.value);
		case FUNC:
			MatchToken(L_BRACKET);
			ret = Expression();
			MatchToken(R_BRACKET); 
			return MakeExprNode(token_tmp,ret,tmp.FuncPtr);
		case L_BRACKET: 
			ret = Expression();
			MatchToken(R_BRACKET);
			return ret;
		default:
			SyntaxError(m_lexical->GetCount());
			return NULL;
	}
}

void CYufa::PrintSyntaxTree(ExprNode* root,int indent){
	switch(root->OpCode){
		case CONST_ID:
			for(int i = 0 ;i < indent;++i){
				cout << " ";
			} 
			cout << root->Content.CaseConst << endl;
			break;
		case T:
			for(int i = 0;i < indent;++i){
				cout << " ";
			}
			cout << "T" << endl;
		case FUNC:
			for(int i = 0;i < indent;++i){
				cout << " ";
			}
			for(int i = 0;i < 18;++i){
				if(root->Content.CaseFunc.MathFuncPtr == tokentable[i].FuncPtr)
				{
					cout << tokentable[i].lexeme << endl;
					break;
				}
			}
			PrintSyntaxTree(root->Content.CaseFunc.Child,indent+2);
			break;
		default:
			for(int i = 0;i < indent;++i){
				cout << " ";
			}
			switch(root->OpCode){
				case PLUS:
					cout << "+" << endl;
					break;
				case MINUS:
					cout << "-" << endl;
					break;
				case MUL:
					cout << "*" << endl;
					break;
				case DIV:
					cout << "/" << endl;
					break;
				case POWER:
					cout << "**" << endl;
					break; 
				default:
					return;
			}
			PrintSyntaxTree(root->Content.CaseOperator.left,indent+2);
			PrintSyntaxTree(root->Content.CaseOperator.right,indent+2);
			break;
	}
}

double& CYufa::GetParameter() {
	return Parameter;
}


