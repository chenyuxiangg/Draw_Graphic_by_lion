#ifndef LEXICAL_ANALYZER_h
#define LEXICAL_ANALYZER_H
#include <vector>
#include "Struct.h"

class Clexical_Analyzer{
	public:
		Clexical_Analyzer();
		~Clexical_Analyzer();
		void Analyze(char* oneline);//analyze one word's type
		Token GetToken();
		void Show();
		void Clear();
		int GetCount();
		
	private:
		std::vector<Token> m_vectok;
		static int m_count;
};

#endif
