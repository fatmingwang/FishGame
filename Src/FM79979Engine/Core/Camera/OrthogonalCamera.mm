#include "../stdafx.h"
#include "OrthogonalCamera.h"
#include "../GLSL/Shader.h"
#include "SimpleCamera.h"
#include "../Image/SimplePrimitive.h"
namespace FATMING_CORE
{
	const wchar_t*         cOrthogonalCamera::TypeID( L"cOrthogonalCamera" );

	cOrthogonalCamera::cOrthogonalCamera(Vector2 e_vResolution)
	{
		m_vCameraAngle = Vector3::Zero;
		m_vResolution = e_vResolution;
		m_fGridWidth = 100.f;
		m_fGridHeight = 100.f;
		Reset();
		glhOrthof2(m_ProjectionMatrix,m_vViewRect.x,m_vViewRect.z,m_vViewRect.w,m_vViewRect.y, -10000, 10000);
	}

	cOrthogonalCamera::cOrthogonalCamera(cOrthogonalCamera*e_pOrthogonalCamera):Frame(e_pOrthogonalCamera)
	{
		m_vCameraAngle = e_pOrthogonalCamera->m_vCameraAngle;
		m_vResolution = e_pOrthogonalCamera->m_vResolution;
		m_fGridWidth = e_pOrthogonalCamera->m_fGridWidth;
		m_fGridHeight = e_pOrthogonalCamera->m_fGridHeight;
		glhOrthof2(m_ProjectionMatrix,m_vViewRect.x,m_vViewRect.z,m_vViewRect.w,m_vViewRect.y, -10000, 10000);	
	}

	Vector2	cOrthogonalCamera::ConvertMousePositionToWorldPosition(Vector2 e_MousePosition,Vector2 e_ViewportSize)
	{
			float	l_fScaleX = e_MousePosition.x/e_ViewportSize.x;
			float	l_fScaleY = e_MousePosition.y/e_ViewportSize.y;
			//transform mouse positio into game's resolution
			e_MousePosition.x = l_fScaleX*(m_vViewRect.z-m_vViewRect.x)+m_vViewRect.x;
			e_MousePosition.y = l_fScaleY*(m_vViewRect.w-m_vViewRect.y)+m_vViewRect.y;
			return e_MousePosition;
	}
    void    cOrthogonalCamera::MoveViewRect(Vector2 e_vDirection)
    {
        this->m_vViewRect.x+=e_vDirection.x;
        this->m_vViewRect.y+=e_vDirection.y;
        this->m_vViewRect.z+=e_vDirection.x;
        this->m_vViewRect.w+=e_vDirection.y;
    }

	void	cOrthogonalCamera::Render(Vector2 e_vCameraPos)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glhOrthof2(m_ProjectionMatrix,m_vViewRect.x+e_vCameraPos.x,m_vViewRect.z+e_vCameraPos.x,m_vViewRect.w+e_vCameraPos.y,m_vViewRect.y+e_vCameraPos.y, -10000, 10000);
		m_ProjectionMatrix *= cMatrix44::RotationMatrix(m_vCameraAngle);
		FATMING_CORE::SetupShaderViewProjectionMatrix( m_ProjectionMatrix,true );
	}

	void	cOrthogonalCamera::Render(bool e_bYInvert)
	{
		//push attribute
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		if( e_bYInvert )
			glhOrthof2(m_ProjectionMatrix,m_vViewRect.x,m_vViewRect.z,m_vViewRect.y,m_vViewRect.w, -10000, 10000);
		else
			glhOrthof2(m_ProjectionMatrix,m_vViewRect.x,m_vViewRect.z,m_vViewRect.w,m_vViewRect.y, -10000, 10000);
		//because I invert Y.
		//cMatrix44	l_mat = this->GetWorldTransform();
		//l_mat.SetTranslation(-l_mat.GetTranslation());
		m_ProjectionMatrix *= cMatrix44::RotationMatrix(m_vCameraAngle);
		FATMING_CORE::SetupShaderViewProjectionMatrix(m_ProjectionMatrix,true);
	}

	void	cOrthogonalCamera::DrawGrid(float e_fXOffset,float e_fYOffset,Vector4 e_vColor,float e_fLineWidth)
	{
		//just want a interge value for 100
		//int		l_iLeft = (int)m_vViewRect.x/100*100;
		//int		l_iRight = (int)m_vViewRect.z/100*100;
		//int		l_iUp = (int)m_vViewRect.y/100*100;
		//int		l_iDown = (int)m_vViewRect.w/100*100;

		float	l_fResolutionWidth = m_vViewRect.z-m_vViewRect.x;
		float	l_fResolutionHeight = m_vViewRect.w-m_vViewRect.y;
		float	l_fRow = l_fResolutionWidth/m_fGridWidth;
		float	l_fColumn = l_fResolutionHeight/m_fGridHeight;
		int	l_iRow = (int)l_fRow;
		int	l_iColumn = (int)l_fColumn;
		l_iRow = l_fRow-l_iRow>0.f?l_iRow+1:(l_iRow);
		l_iColumn = l_fColumn-l_iColumn>0.f?l_iColumn+1:(l_iColumn);
		float	l_fPosX = (m_vViewRect.x)/m_fGridWidth;
		float	l_fPosY = (m_vViewRect.y)/m_fGridHeight;
		float	l_fXOffset = (l_fPosX-(int)(l_fPosX))*m_fGridWidth;
		float	l_fYOffset = (l_fPosY-(int)(l_fPosY))*m_fGridHeight;
		int	l_iTotalCount = (l_iRow+2+l_iColumn+2)*2;
		if( l_iRow<=0||l_iColumn<=0 )
			return;
		//glTranslatef(-l_fXOffset,-l_fYOffset,0.f);
		glLineWidth(e_fLineWidth);
		Vector2*l_vAllVertices = new Vector2[l_iTotalCount];
		float	l_fStartUp = m_vViewRect.y-m_fGridHeight;
		float	l_fEndDown = m_vViewRect.w+m_fGridHeight;
		float	l_fStartLeft = m_vViewRect.x-m_fGridWidth;
		float	l_fEndRight = m_vViewRect.z+m_fGridWidth;
		//+2 from l_iTotalCount
		for( int i=0;i<l_iRow+2;++i )
		{
			l_vAllVertices[i*2] = Vector2(l_fStartLeft+i*m_fGridWidth,l_fStartUp);
			l_vAllVertices[i*2+1] = Vector2(l_fStartLeft+i*m_fGridWidth,l_fEndDown);
			l_vAllVertices[i*2].x+=e_fXOffset;
			l_vAllVertices[i*2+1].x+=e_fXOffset;
		}
		//+2 from l_iTotalCount
		int	l_iTotalRow = (l_iRow+2)*2;
		for( int j=0;j<l_iColumn+2;++j )
		{
			l_vAllVertices[l_iTotalRow+j*2]		=	Vector2(l_fStartLeft,l_fStartUp+j*m_fGridHeight);
			l_vAllVertices[l_iTotalRow+j*2+1]	=	Vector2(l_fEndRight,l_fStartUp+j*m_fGridHeight);
			l_vAllVertices[l_iTotalRow+j*2].y	+=	e_fYOffset;
			l_vAllVertices[l_iTotalRow+j*2+1].y	+=	e_fYOffset;
		}
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		cMatrix44	l_mat = cMatrix44::TranslationMatrix(Vector3(-l_fXOffset,-l_fYOffset,0.f));
		SetupShaderWorldMatrix(l_mat);
		myGlVertexPointer(2,&l_vAllVertices[0]);
		SetupShaderColor(e_vColor);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, l_iTotalCount);
		UseShaderProgram(l_p2DShader);
		delete l_vAllVertices;
	}

	void	cOrthogonalCamera::DrawSelectFrame()
	{
		if( m_bDrawSelectFrame )
			m_DrawSelectRectangleByMouse.Render();
	}

	void	cOrthogonalCamera::Reset()
	{
		m_CameraPos.x = m_vResolution.x/2.f;m_CameraPos.y = m_vResolution.y/2.f; m_fScale = 1.f;
		m_vViewRect.x = m_vViewRect.z = m_vViewRect.y = m_vViewRect.w = 0.f;
		m_bDrawSelectFrame = true;
		m_vViewRect.x = m_CameraPos.x-m_vResolution.x/2.f;
		m_vViewRect.z = m_vViewRect.x+m_vResolution.x;
		m_vViewRect.w = m_CameraPos.y+m_vResolution.y/2.f;
		m_vViewRect.y = m_vViewRect.w-m_vResolution.y;
		this->SetLocalTransform(cMatrix44::Identity);
	}

	void	cOrthogonalCamera::CameraUpdateByMouse(bool e_bLeftButton,bool e_bRightButton,int e_iWheelDelta,int e_iMouseX,int e_iMouseY,Vector2 e_vViewport)
	{
		 sMouseState	l_sMouseState;
		 memset(&l_sMouseState,0,sizeof(sMouseState));
		 l_sMouseState.iMoveX = e_iMouseX - m_MouseBehave.GetMouseState().iCurrentX;
		 l_sMouseState.iMoveY = e_iMouseY - m_MouseBehave.GetMouseState().iCurrentY;
		 l_sMouseState.iCurrentX = e_iMouseX;
		 l_sMouseState.iCurrentY = e_iMouseY;
		 if(e_iWheelDelta != 0 )
		 //if(e->Button == System::Windows::Forms::MouseButtons::Middle)
		 {
			 l_sMouseState.iMoveZ = e_iWheelDelta/120;
			 l_sMouseState.bMidButton = true;
		 }
		 else
		 if ( e_bLeftButton )
			 l_sMouseState.bLefButton = true;
		 else
		 if ( e_bRightButton )
			 l_sMouseState.bRigButton = true;
		 m_MouseBehave.Update(l_sMouseState);
		 //POINT l_ViewportSize = { (int)e_vViewport.x,(int)e_vViewport.y };
		//if scale is active we have to know old position and caompare to new mouse position.
		Vector2	l_PerviousWorldMousePosition;
		//to get old width and height,so we know the percentage
		float	l_fOldWidth;
		float	l_fOldHeight;
		if(this->m_MouseBehave.GetMouseState().bMidButton)//go ahead with camera direction
		{
			l_PerviousWorldMousePosition = m_MouseWorldPosition;
			m_fScale+=(m_MouseBehave.GetMouseState().iMoveZ/100.f);
			l_fOldWidth = m_vViewRect.z-m_vViewRect.x;
			l_fOldHeight = m_vViewRect.w-m_vViewRect.y;
		}
		else
		if(this->m_MouseBehave.GetMouseState().bRigButton)
		{		
			m_CameraPos.x += m_MouseBehave.GetMouseState().iMoveX;
			m_CameraPos.y += m_MouseBehave.GetMouseState().iMoveY;
		}
		float	l_fNewWidth = m_vResolution.x*m_fScale;
		float	l_fNewHeight = m_vResolution.y*m_fScale;
		m_vViewRect.x = m_CameraPos.x-(l_fNewWidth/2.f);
		m_vViewRect.z = m_vViewRect.x+l_fNewWidth;
		m_vViewRect.y = m_CameraPos.y-(l_fNewHeight/2.f);
		m_vViewRect.w = m_vViewRect.y+l_fNewHeight;
		m_MouseWorldPosition = ConvertMousePositionToWorldPosition(Vector2((float)m_MouseBehave.GetMouseState().iCurrentX,(float)m_MouseBehave.GetMouseState().iCurrentY),Vector2((float)e_vViewport.x,(float)e_vViewport.y));
		if(this->m_MouseBehave.GetMouseState().bMidButton)
		{
			m_CameraPos.x -= ((m_MouseWorldPosition.x-l_PerviousWorldMousePosition.x))*(l_fOldWidth/l_fNewWidth);//old postion minus new position and plus the rate we have changed
			m_CameraPos.y -= ((m_MouseWorldPosition.y-l_PerviousWorldMousePosition.y))*(l_fOldHeight/l_fNewHeight);
		}
		//below is not necessary,but it's helpful while we want to draw rectange to show position info on the screen
		POINT	l_MouseWorldPosition = {(int)m_MouseWorldPosition.x,(int)m_MouseWorldPosition.y};
		m_DrawSelectRectangleByMouse.Update(this->m_MouseBehave.GetMouseState().bLefButton,l_MouseWorldPosition);
		 l_sMouseState.bLefButton = false;
		 l_sMouseState.bMidButton = false;;
		 l_sMouseState.bRigButton = false;;
		 m_MouseBehave.Update(l_sMouseState);
	}

	void	cOrthogonalCamera::CameraUpdateByMouse(MyMath::AXIS e_Axis,bool e_bLeftButton,bool e_bRightButton,int e_iWheelDelta,int e_iMouseX,int e_iMouseY,Vector2 e_vViewport)
	{
		cOrthogonalCamera	l_OrthogonalCamera;
		l_OrthogonalCamera.CameraUpdateByMouse(e_bLeftButton,e_bRightButton,e_iWheelDelta,e_iMouseX,e_iMouseY,e_vViewport);
		if( e_Axis == MyMath::W )
			return;
		switch(  e_Axis )
		{
			case MyMath::X:
				l_OrthogonalCamera.SetLocalTransform(*cMatrix44::X_TO_Z*l_OrthogonalCamera.GetLocalTransform());
				break;
			case MyMath::Y:
				l_OrthogonalCamera.SetLocalTransform(*cMatrix44::Y_TO_Z*l_OrthogonalCamera.GetLocalTransform());
				break;
			case MyMath::Z:
				break;
		}
		this->SetLocalTransform(l_OrthogonalCamera.GetLocalTransform());
	}

	bool		cOrthogonalCamera::ViewportConvert(Vector4 e_vShowRect,Vector2 e_vRealViewPort,Vector4*e_pResult)
	{
		if(!IsVisible(e_vShowRect))
			return false;
		if( e_vShowRect.Width() == 0 || e_vShowRect.Height() == 0 )
			return false;
		if( e_pResult )
		{
			Vector4	l_vLocalSpaceShowRect(e_vShowRect.x-m_vViewRect.x,e_vShowRect.y-m_vViewRect.y,
				e_vShowRect.z-m_vViewRect.x,e_vShowRect.w-m_vViewRect.y);
			if( e_pResult )
				*e_pResult = ViewRectToOpenGLScissor(l_vLocalSpaceShowRect);
		}
		return true;
	}

	bool		cOrthogonalCamera::IsVisible(Vector4 e_vShowRect)
	{
		return this->m_vViewRect.Collide(e_vShowRect);
	}
}