#ifndef REGEX_H
#define REGEX_H

enum Token_type{
	TNULL,
	ORIGIN,SCALE,ROT,IS,//保留字 
	TO,STEP,DRAW,FOR,FROM,T,//保留字 
	SEMICO,L_BRACKET,R_BRACKET,COMMA,//分隔符 
	PLUS,MINUS,MUL,DIV,POWER,//运算符 
	FUNC,//函数 
	CONST_ID,//常数 
	NONTOKEN,//空标记 
	ERRTOKEN,//错误标记 
	COMMENT
};

typedef struct Token{
	Token_type type;
	char* lexeme;
	double value;
	double (*FuncPtr)(double);
} Token;

typedef double(*FuncPtr)(double);
class ExprNode{
	public:
		Token_type OpCode;		//记号种类
		union{
			struct{
				ExprNode* left;
				ExprNode* right;
			}CaseOperator;
		
			struct{
				ExprNode* Child;
				FuncPtr MathFuncPtr;
			}CaseFunc;
		
			double CaseConst;			//常数，绑定右值 
			double* CaseParmPtr;		//参数T,绑定左值 
		}Content; 
		
};

typedef struct MYPOINT{
	double x;
	double y;
}Point;

#endif
