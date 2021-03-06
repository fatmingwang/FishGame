#include "../stdafx.h"
#include "BaseImage.h"
#include "SimplePrimitive.h"
#include "BinaryToTexture.h"
#include "../GLSL/Shader.h"
#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{
	const wchar_t*     cBaseImage::TypeID( L"cBaseImage" );
	const wchar_t*     cUIImage::TypeID( L"cUIImage" );
	//====================
	//
	//====================
	cBaseImage::cBaseImage(const char*e_strImageName,bool e_bFetchPixels):cRenderObjectBehavior()
	{
		if( e_strImageName )
			this->SetName(UT::CharToWchar(UT::GetFileNameWithoutFullPath(e_strImageName)));
		m_pTexture = new cSimpleGLTexture(this,e_strImageName,e_bFetchPixels);
		memcpy(this->m_fUV,m_pTexture->GetUV(),sizeof(float)*4);
		m_bMirror = false;
		m_bVisible = true;
		m_iWidth = m_pTexture->GetWidth();
		m_iHeight = m_pTexture->GetHeight();
		if( !g_bSupportNonPowerOfTwoTexture )
		{
			if(m_pTexture->GetUV()[2] != 1.f || m_pTexture->GetUV()[3] == 1.f)
			{
				m_iWidth = (int)(m_pTexture->GetWidth()*m_pTexture->GetUV()[2]);
				m_iHeight = (int)(m_pTexture->GetHeight()*m_pTexture->GetUV()[3]);
			}
		}
		m_vColor = Vector4::One;
		m_OffsetPos.x = m_OffsetPos.y = 0;
	}

	cBaseImage::cBaseImage(cBaseImage*e_pBaseImage):cRenderObjectBehavior(e_pBaseImage)
	{
		SetTexBehaviorData(e_pBaseImage);
	}

	cBaseImage::~cBaseImage()
	{
		m_pTexture->Release(this);
	}
	//===============
	//
	//===============

	void	cBaseImage::SetPos(POINT e_Pos)
	{
		m_pvPos->x = (float)e_Pos.x;
		m_pvPos->y = (float)e_Pos.y;
		Frame::SetLocalPosition(*m_pvPos);
	}
	void	cBaseImage::SetPos(Vector2 e_vPos)
	{
		m_pvPos->x = e_vPos.x;
		m_pvPos->y = e_vPos.y;
		Frame::SetLocalPosition(*m_pvPos);
	}
	void	cBaseImage::SetPosByImageCenter(Vector3 e_vPos)
	{
		m_pvPos->x = e_vPos.x-m_iWidth/2.f;m_pvPos->y = e_vPos.y-m_iHeight/2.f;
		Frame::SetLocalPosition(*m_pvPos);
	}

    bool    cBaseImage::CollideTexture(int e_iX,int e_iY,bool e_bTestAlphaChannel,Vector4*e_pvCollidedColor)
	{
		e_iY = (int)(e_iY-this->m_pvPos->y);
        e_iX = (int)(e_iX-this->m_pvPos->x);
		if( e_iX<m_OffsetPos.x||e_iY<m_OffsetPos.y||m_OffsetPos.x+m_iWidth<e_iX||m_OffsetPos.y+m_iHeight<e_iY )
			return false;
		if( !e_bTestAlphaChannel || !this->m_pTexture->GetPixels() )
            return true;
        int l_iLocalPosX = e_iX-m_OffsetPos.x;
        int l_iLocalPosY = e_iY-m_OffsetPos.y;
		unsigned char*l_pPixels = m_pTexture->GetPixels();
        assert(m_pTexture->GetPixelFormat() == GL_RGBA);
        //m_iWidth to get original size
        int l_iPixelsID = (l_iLocalPosX+(l_iLocalPosY*m_pTexture->GetWidth()))*4;
        //+3 for alpha
        if(l_pPixels[l_iPixelsID+3] != 0)
		{
			unsigned char   l_Color[4] = { l_pPixels[l_iPixelsID],l_pPixels[l_iPixelsID+1],l_pPixels[l_iPixelsID+2],l_pPixels[l_iPixelsID+3] };
			Vector4 e_vColor(l_Color[0]/255.f,l_Color[1]/255.f,l_Color[2]/255.f,l_Color[3]/255.f);
			if( e_pvCollidedColor )
				*e_pvCollidedColor = e_vColor;
            return true;
		}
        return false;
	}
	//==================
	//move point to image's local position
	//now set image;s center as 0,0,so set localX and Localy tp correspond position(width for minus half image)
	//
	//==================
    bool    cBaseImage::CollideTextureWithTransform(int e_iX,int e_iY,bool e_bTestAlphaChannel)
    {
		//fuck result is wrong.
        int l_iLocalY = (int)(e_iY-this->m_pvPos->y);
        int l_iLocalX = (int)(e_iX-this->m_pvPos->x);
		cMatrix44	l_matrix = cMatrix44::Identity;
		Vector3 l_vPos = Vector3(l_iLocalX-m_pTexture->GetWidth()/2.f,l_iLocalY-m_pTexture->GetHeight()/2.f,0);
		l_matrix = l_matrix.ZAxisRotationMatrix(this->GetRotation().z);
		Vector3 l_vFinalPos = l_matrix.TransformVector(l_vPos);
		l_iLocalY = (int)l_vFinalPos.y+this->m_pTexture->GetHeight()/2;
		l_iLocalX = (int)l_vFinalPos.x+this->m_pTexture->GetWidth()/2;
		if( l_iLocalX<0||l_iLocalY<0 )
			return false;
		if( l_iLocalX>this->m_iWidth||l_iLocalY>this->m_iHeight )
			return false;
		//it might be a PuzzleImageUnit so get correct width size
		int l_iActulWidthSize = m_pTexture->GetWidth();
		//fuck.....
        //if( !g_bSupportNonPowerOfTwoTexture && this->GetOwner() )
        //{
          //  l_iActulWidthSize = UT::power_of_two(m_pTexture->GetWidth());
        //}
		int	l_iIndex = l_iLocalY*l_iActulWidthSize+l_iLocalX;
		if( l_iIndex<0 )
			return false;
		if( !e_bTestAlphaChannel )
        {
            RECT    l_rc = {this->m_OffsetPos.x,this->m_OffsetPos.y,this->m_iWidth,this->m_iHeight};
            if(UT::RectCollidePoint(l_rc,l_iLocalX,l_iLocalY))
                return true;
            return false;
        }
		unsigned char l_Alpha = m_pTexture->GetPixels()[l_iIndex*4];//argb
		//unsigned char l_Alpha = this->m_pPixels[l_iIndex*4+3];//rgba
		//UINT32	l_Alpha2 = *(UINT32*)&this->m_pPixels[l_iIndex*4];//argb
	//	unsigned char	l_cR = this->m_pPixels[l_iIndex*4+1];
	//	unsigned char	l_cG = this->m_pPixels[l_iIndex*4+2];
	//	unsigned char	l_cB = this->m_pPixels[l_iIndex*4+4];

		if( l_Alpha != 0 )
		{//there is alpha with color is default empty color,so it's not collide
			//if( l_cR==255&&
			//	l_cG==255&&
			//	l_cB==255)
			//	return false;
			//this->Destroy();
			////memset(m_pPixels,130,sizeof(m_pTexture->GetWidth()*m_iHeight)*4);
			//glGenTextures(1,&m_uiImageIndex);
			//glBindTexture( GL_TEXTURE_2D, m_uiImageIndex );
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//m_pPixels[l_iIndex*4] = 125;
			//m_pPixels[l_iIndex*4+1] = 125;
			//m_pPixels[l_iIndex*4+2] = 125;
			//m_pPixels[l_iIndex*4+3] = 125;
			//glTexImage2D(GL_TEXTURE_2D,
			//  0,
			//  GL_RGBA,
			//  m_pTexture->GetWidth(),
			//  m_iHeight,
			//  0,
			//  GL_RGBA ,
			//  GL_UNSIGNED_BYTE,
			//  m_pPixels);
			return true;
		}
		return false;
    }

	void	cBaseImage::RenderRect()
	{
		GLRender::RenderRectangle(this->GetWorldPosition(),(float)this->m_iWidth,(float)this->m_iHeight,Vector4::One,this->GetRotation());
	}

	void	cBaseImage::SetMirror(bool e_bMirror)
	{
		if(e_bMirror == m_bMirror)
			return;
		m_bMirror = e_bMirror;
		float	l_fX[2] = {m_fUV[0],m_fUV[2]};
		m_fUV[0] = l_fX[1];
		m_fUV[2] = l_fX[0];
	}

	void	cBaseImage::SetTexBehaviorData(cBaseImage*e_pBaseImage)
	{
		this->SetName(e_pBaseImage->GetName());
		m_pTexture = e_pBaseImage->m_pTexture;
		m_pTexture->AddRef(this);
		memcpy(this->m_fUV,e_pBaseImage->GetUV(),sizeof(float)*4);
		m_bMirror = e_pBaseImage->m_bMirror;
		m_bVisible = true;
		m_vColor = e_pBaseImage->m_vColor;
		m_OffsetPos = e_pBaseImage->m_OffsetPos;
		m_iWidth = e_pBaseImage->m_iWidth;
		m_iHeight = e_pBaseImage->m_iHeight;
	}

	void	cBaseImage::ApplyImage()
	{
		//if( m_pTexture )
			this->m_pTexture->ApplyImage();
	}

	void	cBaseImage::Render(int e_iX,int e_iY)
	{
		if( !m_bVisible )
			return;
		m_pTexture->ApplyImage();
		GLRender::DrawQuadWithTextureAndColorAndCoordinate((float)e_iX,(float)e_iY,this->m_pvPos->z,this->m_iWidth,this->m_iHeight,this->m_vColor,m_fUV,this->GetRotation().z);
	}

	void	cBaseImage::Render(Vector3 e_vPos)
	{
		if( !m_bVisible )
			return;
		m_pTexture->ApplyImage();
		GLRender::DrawQuadWithTextureAndColorAndCoordinate(e_vPos.x,e_vPos.y,e_vPos.z,this->m_iWidth,this->m_iHeight,this->m_vColor,m_fUV,this->GetRotation().z);
	}
	//====================
	//
	//====================	
    void	cBaseImage::RenderWithOffset()
    {
		if( !m_bVisible )
			return;
		m_pTexture->ApplyImage();
		Vector3	l_vPos = this->GetWorldPosition();
		DrawQuadWithTextureAndColorAndCoordinate(l_vPos.x+this->m_OffsetPos.x,l_vPos.y+this->m_OffsetPos.y,l_vPos.z,this->m_iWidth,this->m_iHeight,m_vColor,this->m_fUV,this->GetRotation().z);
    }
	//====================
	//
	//====================
	void	cBaseImage::Render()
	{
		if( !m_bVisible )
			return;
		Vector3	l_vPos = this->GetWorldPosition();
		if(m_pTexture->ApplyImage())
		{
			DrawQuadWithTextureAndColorAndCoordinate(l_vPos.x,l_vPos.y,l_vPos.z,this->m_iWidth,this->m_iHeight,m_vColor,this->m_fUV,this->GetRotation().z);
		}
		else
			RenderRectangle(l_vPos,(float)m_iWidth,(float)m_iHeight,Vector4::One);
	}
	
    void	cBaseImage::Render(cMatrix44 e_Mat)
    {
		if( !m_bVisible )
			return;    
        m_pTexture->ApplyImage();
		float l_fWidth = m_iWidth/2.f;
		float l_fHeight = m_iHeight/2.f;
		float	l_Vertices[] = { -l_fWidth,-l_fHeight,
								 l_fWidth, -l_fHeight,
								 -l_fWidth, l_fHeight,
								 l_fWidth,l_fHeight};
/*        float l_fWidth = m_iWidth;
		float l_fHeight = m_iHeight;
		float	l_Vertices[] = { 0,0,
								 l_fWidth,0,
								 0, l_fHeight,
								 l_fWidth,l_fHeight};	*/							 
		float	l_fTexPointer[] = {  m_fUV[0],m_fUV[1],
								   m_fUV[2],m_fUV[1],
								   m_fUV[0],m_fUV[3],
								   m_fUV[2],m_fUV[3],};
        DrawQuadWithMatrix(l_Vertices,l_fTexPointer,this->m_vColor,e_Mat);
    }

	void	cBaseImage::RenderBySpecificPos(Vector3 e_vStartPos,Vector3 e_vEndPos)
	{
		Vector3	l_vUpDirection = e_vStartPos-e_vEndPos;
		if( l_vUpDirection.Length()<=0.1f )
			return;
		Vector3	l_vRightDirection = l_vUpDirection.Normalize()^Vector3(0,0,-1);
		int	l_iWidth =	this->m_iHeight;
		Vector3	l_v2 = l_vRightDirection*(float)l_iWidth+e_vStartPos;
		Vector3	l_v3 = l_vRightDirection*(float)-l_iWidth+e_vStartPos;
		Vector3	l_v1 = l_vRightDirection*(float)l_iWidth+e_vEndPos;;
		Vector3	l_v0 = l_vRightDirection*(float)-l_iWidth+e_vEndPos;
		m_pTexture->ApplyImage();
		float*l_pfTex = this->GetUV();
		float	l_pfTexPointer[8] = { l_pfTex[2],l_pfTex[1],
									l_pfTex[2],l_pfTex[3],
									l_pfTex[0],l_pfTex[1],
									l_pfTex[0],l_pfTex[3] };

		float	l_pVertices[8] = { l_v3.x,l_v3.y,l_v2.x,l_v2.y,l_v0.x,l_v0.y,l_v1.x,l_v1.y};
		myGlUVPointer(2,l_pfTexPointer);
		myGlVertexPointer(2,l_pfTexPointer);
		ASSIGN_2D_COLOR(m_vColor);
		MY_GLDRAW_ARRAYS(GL_TRIANGLE_STRIP, 0, 4);
	}

	void	cBaseImage::RenderWithOffset(Vector3 e_vPos)
	{
		if( !m_bVisible )
			return;
		Render(Vector3(e_vPos.x+this->m_OffsetPos.x,e_vPos.y+this->m_OffsetPos.y,0.f));
	}
	void	cBaseImage::RenderWithOffset(int e_iX,int e_iY)
	{
		if( !m_bVisible )
			return;
		Render(e_iX+this->m_OffsetPos.x,e_iY+this->m_OffsetPos.y);
	}
	//====================
	//
	//====================
	cUIImage::cUIImage(char*e_strImageName):cBaseImage(e_strImageName,true)
	{
		m_pvPos->x = 0;m_pvPos->y = 0;
		m_OriginalImageSize.x = -1;
		m_OriginalImageSize.y = -1;
		m_RightDownStripOffPos.x = this->m_iWidth-1;
		m_RightDownStripOffPos.y = this->m_iHeight-1;
		m_iCollisionImageWidth = m_iWidth;
		m_iCollisionImageHeight = m_iHeight;
		m_pEditorAttachParent = 0;
		m_vEditorAttachParentRelativePos = Vector3::Zero;
	}

	cUIImage::cUIImage(WCHAR*e_strName,char*e_pPixelsData,int e_iWidth,int e_iHeight,GLint e_iDataFormat):cBaseImage(0,false)
	{
		this->SetName(e_strName);
		m_OriginalImageSize.x = -1;
		m_OriginalImageSize.y = -1;
		m_fUV[0] = 0.f; m_fUV[1] = 0.f; m_fUV[2] = 1.f; m_fUV[3] = 1.f;
		m_pvPos->x = 0;m_pvPos->y = 0; 
		m_RightDownStripOffPos.x = e_iWidth-1;
		m_RightDownStripOffPos.y = e_iHeight-1;
		m_iCollisionImageWidth = e_iWidth;
		m_iCollisionImageHeight = e_iHeight;
		this->m_iHeight = e_iHeight;
		this->m_iWidth = e_iWidth;
		int	l_iWidthPO2 = power_of_two(m_iHeight);
		int	l_iHeightPO2 = power_of_two(m_iWidth);
		GLint texSize; glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
		int	l_iChannel = e_iDataFormat == GL_RGB?3:4;
		m_pTexture->Release(this);
		//make sure power of 2,because not every fukcing graphic card support it
		//but if u exactly sure it do support power of 2 u could mark this.
		if( !g_bSupportNonPowerOfTwoTexture&&(l_iWidthPO2!=e_iWidth||l_iHeightPO2!=e_iHeight) )
		{
			assert((l_iWidthPO2<=texSize||l_iHeightPO2<=texSize)&&"texture size is too big then card support");
	#ifdef WIN32
	#ifdef DEBUG
			cGameApp::OutputDebugInfoString(e_strName);
			cGameApp::OutputDebugInfoString(L"  image has not correct respond width and height,because none power of 2\n");
			if( e_iDataFormat == GL_RGB )
				cGameApp::OutputDebugInfoString(L"UV is changed,because image size is not to become power of 2");
	#endif
	#endif
			bool e_balpah = false;
#ifdef ANDROID
			if( e_iDataFormat == GL_RGBA )//android not support GL_BGRA?or just nvidia ignore it?
#else
			if( e_iDataFormat == GL_RGBA || e_iDataFormat == GL_BGRA )
#endif
				e_balpah = true;
			char*l_pNewPixelData = TextureToPowOfTwo((char*)e_pPixelsData,e_iWidth,l_iHeightPO2,e_balpah);
			delete e_pPixelsData;
			this->m_iWidth = l_iWidthPO2; this->m_iHeight = l_iHeightPO2;
			m_pTexture = new cSimpleGLTexture(this,(unsigned char*)l_pNewPixelData,l_iWidthPO2,l_iHeightPO2,e_strName,true,false,e_iDataFormat);
			//assign new UV
			m_fUV[2] = (float)m_iWidth/l_iWidthPO2;
			m_fUV[3] =  (float)m_iHeight/l_iHeightPO2;
		}
		else
		{
			m_pTexture = new cSimpleGLTexture(this,(unsigned char*)e_pPixelsData,e_iWidth,e_iHeight,e_strName,true,false,e_iDataFormat);

		}
		m_pEditorAttachParent = 0;
		m_vEditorAttachParentRelativePos = Vector3::Zero;
	}

	cUIImage::cUIImage(cUIImage*e_pUIImage):cBaseImage(e_pUIImage)
	{
		m_pvPos->x = 0;m_pvPos->y = 0;
		m_OriginalImageSize.x = -1;
		m_OriginalImageSize.y = -1;
		m_RightDownStripOffPos = e_pUIImage->m_RightDownStripOffPos;
		m_iCollisionImageWidth = e_pUIImage->m_iCollisionImageWidth;
		m_iCollisionImageHeight = e_pUIImage->m_iCollisionImageHeight;
		m_pEditorAttachParent = e_pUIImage;
		m_vEditorAttachParentRelativePos = Vector3::Zero;
	}

	cUIImage::~cUIImage()
	{
	}

	void	cUIImage::RenderWithOffset()
	{
		if( !m_bVisible )
			return;
		Vector3	l_vPos = this->GetWorldPosition();
		cBaseImage::Render((int)l_vPos.x-this->m_OffsetPos.x,(int)l_vPos.y-this->m_OffsetPos.y);
	}


	bool	cUIImage::Collide(int e_iLocalX,int e_iLocalY)
	{
		if( this->m_pTexture->GetPixelFormat()  == GL_RGB)//there is no alpha so we can't find any info
		{
			if( e_iLocalX<this->m_iCollisionImageWidth&&e_iLocalX>=0 )
				if( e_iLocalY<this->m_iCollisionImageHeight&&e_iLocalY>=0 )
					return true;
			return false;
		}
		cMatrix44	l_matrix;
		Vector3 l_vPos = Vector3(m_pTexture->GetHeight()/2.f-e_iLocalY,e_iLocalX-m_pTexture->GetWidth()/2.f,0);
		l_matrix = l_matrix.Identity;
		l_matrix = l_matrix.ZAxisRotationMatrix(MyMath::DegToRad(360-this->GetRotation().z));
		Vector3 l_vFinalPos = l_matrix.TransformVector(l_vPos);
		e_iLocalY = (int)(-l_vFinalPos.x)+this->m_pTexture->GetHeight()/2;
		e_iLocalX = (int)l_vFinalPos.y+this->m_pTexture->GetWidth()/2;
		if( e_iLocalX<0||e_iLocalY<0 )
			return false;
		if( e_iLocalX>m_pTexture->GetWidth()||e_iLocalY>m_pTexture->GetHeight() )
			return false;
		int	l_iIndex = e_iLocalY*this->m_iCollisionImageWidth+e_iLocalX;
		if( l_iIndex<0 )
			return false;
        //+4!!!!!!!!!?FUCK...
		unsigned char l_Alpha = this->m_pTexture->GetPixels()[l_iIndex*4];//argb
		//UINT32	l_Alpha2 = *(UINT32*)&this->m_pPixels[l_iIndex*4];//argb
	//	unsigned char	l_cR = this->m_pPixels[l_iIndex*4+1];
	//	unsigned char	l_cG = this->m_pPixels[l_iIndex*4+2];
	//	unsigned char	l_cB = this->m_pPixels[l_iIndex*4+4];

		if( l_Alpha != 0 )
		{//there is alpha with color is default empty color,so it's not collide
			//if( l_cR==255&&
			//	l_cG==255&&
			//	l_cB==255)
			//	return false;
			//this->Destroy();
			////memset(m_pPixels,130,sizeof(m_pTexture->GetWidth()*m_iHeight)*4);
			//glGenTextures(1,&m_uiImageIndex);
			//glBindTexture( GL_TEXTURE_2D, m_uiImageIndex );
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//m_pPixels[l_iIndex*4] = 125;
			//m_pPixels[l_iIndex*4+1] = 125;
			//m_pPixels[l_iIndex*4+2] = 125;
			//m_pPixels[l_iIndex*4+3] = 125;
			//glTexImage2D(GL_TEXTURE_2D,
			//  0,
			//  GL_RGBA,
			//  m_pTexture->GetWidth(),
			//  m_iHeight,
			//  0,
			//  GL_RGBA ,
			//  GL_UNSIGNED_BYTE,
			//  m_pPixels);
			return true;
		}
		return false;
	}

	bool	cUIImage::CollideWithOutAlphaTest(int e_iLocalX,int e_iLocalY)
	{
		if( this->m_pTexture->GetPixelFormat() == GL_RGB)//there is no alpha so we can't find any info
		{
			if( e_iLocalX<this->m_iCollisionImageWidth&&e_iLocalX>=0 )
				if( e_iLocalY<this->m_iCollisionImageHeight&&e_iLocalY>=0 )
					return true;
			return false;
		}
		cMatrix44	l_matrix;
		Vector3 l_vPos = Vector3(this->m_iHeight/2.f-e_iLocalY,e_iLocalX-this->m_pTexture->GetWidth()/2.f,0);
		l_matrix = l_matrix.Identity;
		l_matrix = l_matrix.ZAxisRotationMatrix(MyMath::DegToRad(360-this->GetRotation().z));
		Vector3 l_vFinalPos = l_matrix.TransformVector(l_vPos);
		e_iLocalY = (int)(-l_vFinalPos.x)+this->m_iHeight/2;
		e_iLocalX = (int)l_vFinalPos.y+this->m_pTexture->GetWidth()/2;
		if( e_iLocalX<0||e_iLocalY<0 )
			return false;
		if( e_iLocalX>m_pTexture->GetWidth()||e_iLocalY>m_iHeight )
			return false;
			
		if( e_iLocalX>(this->m_RightDownStripOffPos.x-this->m_OffsetPos.x)||e_iLocalY>this->m_RightDownStripOffPos.y-this->m_OffsetPos.y )
			return false;			
		int	l_iIndex = e_iLocalY*this->m_iCollisionImageWidth+e_iLocalX;
		if( l_iIndex<0 )
			return false;
		return true;
	}
}