#ifndef	_IAB_PRODUCT_BUTTON_H_
#define	_IAB_PRODUCT_BUTTON_H_

//an image with a button
//
//

class	cIABProductButton:public cImageButton
{
	cBaseImage*				m_pItemImage;
public:
	cIABProductButton();
	~cIABProductButton();
	virtual	void			Render();
};

#endif