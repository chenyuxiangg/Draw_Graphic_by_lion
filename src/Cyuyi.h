#ifndef YUYI_H
#define YUYI_H

#include "Struct.h"

class Cyuyi{
	public:
		Cyuyi():m_origin_x(0.0),m_origin_y(0.0),m_rot_ang(0.0),
		m_scale_x(1),m_scale_y(1),m_current_x(0.0),m_current_y(0.0){
			
		}
		void CalcCoord(ExprNode* x_nptr,ExprNode* y_nprt);
		void SetOrigin(const double& x,const double& y);
		void SetRot(const double& r);
		void SetScale(const double& x,const double& y);
		Point GetCurrentPoint();
		double GetExprValue(ExprNode* root);
		
	private:
		double m_origin_x;
		double m_origin_y;
		double m_rot_ang;
		double m_scale_x;
		double m_scale_y;
		double m_current_x;
		double m_current_y;
};

#endif 
