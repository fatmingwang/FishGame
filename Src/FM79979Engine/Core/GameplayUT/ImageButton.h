#ifndef _IMAGE_BUTTON_H_
#define _IMAGE_BUTTON_H_
#include "../AllImageInclude.h"
namespace FATMING_CORE
{
	//cPuzzleImageUnit*l_pPuzzleImageUnit;
	//m_pCancelButton = new cImageButton(l_pPuzzleImageUnit,Vector3(0,0,0));
	//m_pCancelButton = new cImageButton(e_pMouse_TouchWithCancelButton->m_pCancelButton);
	//m_pCancelButton->SetName(CANCEL_BUTTON_NAME);
	//m_pCancelButton->Setm_pCancelButton(m_pDoButtonGoal_Callback);
	//
	//must call update !! or condition won't be satisfied.
	extern Vector4	g_vImageButtonClickedColor;
	class	cImageButton:public cClickMouseBehavior,public cBaseImage
	{
	protected:
		virtual	void				SetTransformInternalData();
		//while mouse is ip set a little offset let player c it again to avoid change phase too fast
		UT::sTimeCounter			m_TC;
        virtual void				InternalMouseDown(int e_iPosX,int e_iPosY);
        virtual void				InternalMouseUp(int e_iPosX,int e_iPosY);
        virtual void				InternalMouseLeave(int e_iPosX,int e_iPosY);
		virtual	bool				InternalCollide(int e_iPosX,int e_iPosY);
		//virtual bool    InternalCollide(int e_iPosX,int e_iPosY);
		//if mouse pressed render this image
		cRenderObjectBehavior*		m_pPressedImage;
		cRenderObjectBehavior*		m_pFinishImage;
		bool						ProcessFinishAndPressedImage(TiXmlElement*e_pTiXmlelement);
	public:
		DEFINE_TYPE_INFO();
		cImageButton();
		cImageButton(cPuzzleImageUnit*e_pImage,Vector3 e_vPos);
		cImageButton(cBaseImage*e_pImage,Vector3 e_vPos);
		cImageButton(cSubMPDI*e_pSubMPDI,DoButtonGoal_Callback e_pDoButtonGoal_Callback = 0);
		//fetch first SubMPDI
		cImageButton(cMPDI*e_pMPDI,DoButtonGoal_Callback e_pDoButtonGoal_Callback = 0);
		cImageButton(cImageButton*e_pImageButton);
		static	cImageButton*		GetMe(TiXmlElement*e_pElement);
		CLONE_MYSELF(cImageButton);
		virtual ~cImageButton();
		virtual bool				IsSatisfiedCondition();
		virtual void				Init();
		virtual void				InitRepeat();
		virtual void				Update(float e_fElpaseTime);
		virtual	void				Render();
		//virtual	void	SetPosByImageCenter(Vector3 e_vPos);
		cRenderObjectBehavior*		GetPressedImage(){return m_pPressedImage;}
		void						SetPressedImage(cRenderObjectBehavior*e_pImage);
		cRenderObjectBehavior*		GetFinishImage(){return m_pFinishImage;}
		void						SetFinishImage(cRenderObjectBehavior*e_pImage);
	};

	enum	eYesNoDialogResult
	{
		eYNDR_YES = 0,
		eYNDR_NO,
		eYNDR_NONE,
	};

	class	cYesNoDialog:public cClickMouseBehavior
	{
		Vector3				m_vPos;
		cImageButton*		m_pYesImageButton;
		cImageButton*		m_pNoImageButton;
		cBaseImage*			m_pBK;
		Vector3				m_vFontPos;
		std::wstring		m_strDescription;
		GET_SET_DEC(eYesNoDialogResult,m_eYesNoDialogResult,GetYesNoDialogResult,SetYesNoDialogResult);
	public:
		cYesNoDialog(cMPDI*e_pMPDI,WCHAR*e_strDescription);
		cYesNoDialog(cBaseImage*e_pYesImageButton,cBaseImage*e_pNoImageButton,cBaseImage*e_pBK,WCHAR*e_strDescription,Vector3 e_vYesPos,Vector3 e_vNoPos,Vector3 e_vBKPos,Vector3 e_vFontPos);
		cYesNoDialog(cYesNoDialog*e_pYesNoDialog);
		CLONE_MYSELF(cYesNoDialog);
		virtual ~cYesNoDialog();
		void	SetPos();
		virtual void    Init();
		virtual void    Update(float e_fElpaseTime);
		virtual	void	Render();
        virtual void    MouseMove(int e_iPosX,int e_iPosY);
        //first time into
        virtual void    MouseDown(int e_iPosX,int e_iPosY);
        //
        virtual void    MouseUp(int e_iPosX,int e_iPosY);
		//
		virtual	bool	IsSatisfiedCondition(){ return m_eYesNoDialogResult != eYNDR_NONE; }
		eYesNoDialogResult	GetResult(){ return m_eYesNoDialogResult; }
	};

	cClickMouseBehaviorList<cImageButton>*	GetClickMouseBehaviorList(TiXmlElement*e_pElement);
//end namespace namespace FATMING_CORE
}

#endif