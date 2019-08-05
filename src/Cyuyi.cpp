#include "Cyuyi.h"
#include <cmath>
#include <iostream>
using namespace std;

double Cyuyi::GetExprValue(ExprNode* root){
	if(root == nullptr){
		return 0.0;
	}
	switch(root->OpCode){
		case PLUS:
			return GetExprValue(root->Content.CaseOperator.left)
			+ GetExprValue(root->Content.CaseOperator.right);
		case MINUS:
			return GetExprValue(root->Content.CaseOperator.left)
			- GetExprValue(root->Content.CaseOperator.right); 
		case MUL:
			return GetExprValue(root->Content.CaseOperator.left)
			* GetExprValue(root->Content.CaseOperator.right);
		case DIV:
			{
				double right = GetExprValue(root->Content.CaseOperator.right);
				if(fabs(right) > 0.0 )
				{
					return GetExprValue(root->Content.CaseOperator.left)
					/ right;
				}else{
					cout << "³ýÊýÎª0" << endl; 
				}
			}
			return 0.0;
		case POWER:
			{
				double left = GetExprValue(root->Content.CaseOperator.left);
				double right = GetExprValue(root->Content.CaseOperator.right);
				return pow(left,right);
			}
		case FUNC:
			return (*(root->Content.CaseFunc.MathFuncPtr))(GetExprValue(root->Content.CaseFunc.Child));
		case CONST_ID:
			return root->Content.CaseConst;
		case T:
			return *(root->Content.CaseParmPtr);
		default:
			cout << "½âÎö´íÎó" << endl;
			return 0.0;
	}
}

void Cyuyi::CalcCoord(ExprNode* x_nptr,ExprNode* y_nptr){
	m_current_x = GetExprValue(x_nptr);
	m_current_y =GetExprValue(y_nptr);
	
	m_current_x *= m_scale_x;
	m_current_y *= m_scale_y;
	
	m_current_x = m_current_x*cos(m_rot_ang)+m_current_y*sin(m_rot_ang);
	m_current_y = m_current_y*cos(m_rot_ang)-m_current_x*sin(m_rot_ang);
	
	m_current_x += m_origin_x;
	m_current_y += m_origin_y;
}


void Cyuyi::SetOrigin(const double& x,const double& y){
	m_origin_x = x;
	m_origin_y = y;
}

void Cyuyi::SetRot(const double& r){
	m_rot_ang = r; 
}

void Cyuyi::SetScale(const double& x,const double& y){
	m_scale_x = x;
	m_scale_y = y;
}

Point Cyuyi::GetCurrentPoint(){
	Point p;
	p.x = m_current_x;
	p.y = m_current_y;
	return p;
}
