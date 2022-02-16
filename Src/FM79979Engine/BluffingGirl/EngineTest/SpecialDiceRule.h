#ifndef _SPECIAL_DICE_RILE_H_
#define _SPECIAL_DICE_RILE_H_

class	cSpecialDiceRule
{
	cMPDI*		m_pRuleWorking;
	cMPDI*		m_pRuleNotWorking;
public:
	int			m_iSpecialDicePoint;//-1 for not working
	bool		m_bWorking;
	cSpecialDiceRule(TiXmlElement*e_pElement);
	~cSpecialDiceRule();
	void		Render();
};

#endif