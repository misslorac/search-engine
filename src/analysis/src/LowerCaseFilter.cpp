#include <stdlib.h>
#include "LowerCaseFilter.h"
#include "Token.h"
#include "StringUtility.h"

using namespace utility;

namespace analysis {

/* ������һ��token */
CToken *CLowerCaseFilter::next()
{	
	if (!m_pInput)
		return NULL;

	CToken *pToken = m_pInput->next();

	if (NULL == pToken)
		return NULL;

	string_utility::toLower(pToken->m_szText);

	return pToken;
}

}
