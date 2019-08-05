#ifndef REGEX_H
#define REGEX_H

enum Token_type{
	TNULL,
	ORIGIN,SCALE,ROT,IS,//������ 
	TO,STEP,DRAW,FOR,FROM,T,//������ 
	SEMICO,L_BRACKET,R_BRACKET,COMMA,//�ָ��� 
	PLUS,MINUS,MUL,DIV,POWER,//����� 
	FUNC,//���� 
	CONST_ID,//���� 
	NONTOKEN,//�ձ�� 
	ERRTOKEN,//������ 
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
		Token_type OpCode;		//�Ǻ�����
		union{
			struct{
				ExprNode* left;
				ExprNode* right;
			}CaseOperator;
		
			struct{
				ExprNode* Child;
				FuncPtr MathFuncPtr;
			}CaseFunc;
		
			double CaseConst;			//����������ֵ 
			double* CaseParmPtr;		//����T,����ֵ 
		}Content; 
		
};

typedef struct MYPOINT{
	double x;
	double y;
}Point;

#endif
