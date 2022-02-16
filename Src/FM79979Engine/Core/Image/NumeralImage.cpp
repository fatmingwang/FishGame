#include "../stdafx.h"
#include "NumeralImage.h"
#include "SimplePrimitive.h"

#include "../GLSL/Shader.h"
#include "PuzzleImage.h"
namespace FATMING_CORE
{
	const wchar_t*     cNumeralImage::TypeID( L"cNumeralImage" );
	cNumeralImage::cNumeralImage(char*e_strImageName):cBaseImage(e_strImageName)
	{
		m_i64Value = 0;
		this->m_iSingleImageHeight = m_iHeight;	
		this->m_iSingleImageWidth = m_iWidth/10;
		m_iHeight = m_iSingleImageHeight;
		m_iWidth =  m_iSingleImageWidth;
		//this->m_iImageSpace = e_iImageSpace;
		this->m_iNumIndex = 10;
		m_pfTexCoordinate = new float[4*m_iNumIndex];
		float	l_fStep = this->m_fUV[2]/(float)m_iNumIndex;
		for( int i=0;i<m_iNumIndex;++i )
		{
			m_pfTexCoordinate[i*4  ] = i*l_fStep;
			m_pfTexCoordinate[i*4+1] = m_fUV[1];
			m_pfTexCoordinate[i*4+2] = i*l_fStep+l_fStep;
			m_pfTexCoordinate[i*4+3] = m_fUV[3];
		}
		m_eDirection = eD_LEFT;
	}

	cNumeralImage::cNumeralImage(cBaseImage*e_pImage,float*e_pftexCoordinate):cBaseImage(e_pImage)
	{
		m_i64Value = 0;
		m_eDirection = eD_LEFT;
		//this->m_iImageSpace = e_iImageSpace;
		this->m_iNumIndex = 10;
		memcpy(m_fUV,e_pftexCoordinate,sizeof(float)*4);
		m_pfTexCoordinate = new float[4*m_iNumIndex];
		float	l_fStep = (m_fUV[2]-m_fUV[0])/(float)m_iNumIndex;
		for( int i=0;i<m_iNumIndex;++i )
		{
			m_pfTexCoordinate[i*4  ] = i*l_fStep+m_fUV[0];
			m_pfTexCoordinate[i*4+1] = m_fUV[1];
			m_pfTexCoordinate[i*4+2] = i*l_fStep+l_fStep+m_fUV[0];
			m_pfTexCoordinate[i*4+3] = m_fUV[3];
		}		
		m_iSingleImageHeight = m_iHeight;
		m_iSingleImageWidth = m_iWidth/10;
	}

	cNumeralImage::cNumeralImage(cBaseImage*e_pImage0,cBaseImage*e_pImage9):cBaseImage(e_pImage0)
	{
		m_i64Value = 0;
		m_eDirection = eD_LEFT;
		//this->m_iImageSpace = e_iImageSpace;
		this->m_iNumIndex = 10;
		m_fUV[0] = e_pImage0->GetUV()[0];m_fUV[1] = e_pImage0->GetUV()[1];
		m_fUV[2] = e_pImage9->GetUV()[2];m_fUV[3] = e_pImage9->GetUV()[3];
		m_pfTexCoordinate = new float[4*m_iNumIndex];
		float	l_fStep = (m_fUV[2]-m_fUV[0])/(float)m_iNumIndex;
		cPuzzleImageUnit*l_pPIUnit = (cPuzzleImageUnit*)e_pImage0;
		for( int i=0;i<m_iNumIndex;++i )
		{
			float*l_pUV = l_pPIUnit->GetUV();
			memcpy(&m_pfTexCoordinate[i*4],l_pUV,sizeof(float)*4);
			l_pPIUnit = l_pPIUnit->GetNext();
			//m_pfTexCoordinate[i*4  ] = ;
			//m_pfTexCoordinate[i*4+1] = m_fUV[1];
			//m_pfTexCoordinate[i*4+2] = i*l_fStep+l_fStep+m_fUV[0];
			//m_pfTexCoordinate[i*4+3] = m_fUV[3];
		}		
		m_iSingleImageHeight = m_iHeight;
		m_iSingleImageWidth = m_iWidth;
	}

	cNumeralImage::cNumeralImage(cNumeralImage*e_pNumerialImage):cBaseImage(e_pNumerialImage)
	{
		m_i64Value = 0;
		m_eDirection = e_pNumerialImage->m_eDirection;
		//this->m_iImageSpace = e_iImageSpace;
		this->m_iNumIndex = e_pNumerialImage->m_iNumIndex;
		memcpy(m_fUV,e_pNumerialImage->m_fUV,sizeof(float)*4);
		int	l_iSize = 4*m_iNumIndex;
		m_pfTexCoordinate = new float[l_iSize];
		memcpy(m_pfTexCoordinate,e_pNumerialImage->m_pfTexCoordinate,sizeof(float)*l_iSize);
		m_iSingleImageHeight = m_iHeight = e_pNumerialImage->m_iSingleImageHeight;
		m_iSingleImageWidth = m_iWidth = e_pNumerialImage->m_iSingleImageWidth;
	}

	cNumeralImage::~cNumeralImage()
	{
		SAFE_DELETE(m_pfTexCoordinate);
	}

	void	cNumeralImage::Draw(const char*e_strData,int e_iCount,int e_iPosX,int e_iPosY,float*e_pmat,bool e_bCenter)
	{
		if( !m_bVisible )
			return;
		std::string	l_str = e_strData;
		int	l_iNum = e_iCount;
		if( l_iNum )
		{
			float	l_fHalfWidth = 0.f;
			float	l_fHalfHeight = 0.f;
			float	l_fOffsetPosX = 0.f;
			float	l_fOffsetPosY = 0.f;
			switch(m_eDirection)
			{
				case eD_LEFT:
					l_fOffsetPosX = (float)m_iSingleImageWidth;
					l_fHalfWidth = (m_iSingleImageWidth*l_iNum/2.f);
					l_fHalfHeight = m_iSingleImageHeight/2.f;
					break;
				case eD_DOWN:
					l_fOffsetPosY = (float)m_iSingleImageHeight;
					l_fHalfWidth = m_iSingleImageWidth/2.f;
					l_fHalfHeight = m_iSingleImageHeight*l_iNum/2.f;
					break;
			}
			float	l_fImageHalfWidth = m_iSingleImageWidth/2.f;
			float	l_fImageHalfHeight = m_iSingleImageHeight/2.f;
			float	l_fPosX = -l_fHalfWidth;
			float	l_fPosY = -l_fHalfHeight;
			//from small to big
			for( int i=0;i<l_iNum;++i )
			{
				float	*l_pfVertices	=	&g_fMPDIOptmizeRenderVertices[i*3*6];
				float	*l_pfUV			=	&g_fMPDIOptmizeRenderUV[i*2*6];
				float	*l_pfColor		=	&g_fMPDIOptmizeRenderColor[i*4*6];

				AssignUVDataTo2Triangles(&m_pfTexCoordinate[(l_str[i]-'0')*4],l_pfUV,false);
				for( int i=0;i<6;++i )
					memcpy(&l_pfColor[i*4],&m_vColor,sizeof(Vector4));
				Vector3	l_vPos[4];
				l_vPos[0] = Vector3(l_fPosX						,l_fPosY,0.f);
				l_vPos[1] = Vector3(l_fPosX+m_iSingleImageWidth	,l_fPosY,0.f);
				l_vPos[2] = Vector3(l_fPosX						,l_fPosY+m_iSingleImageHeight,0.f);
				l_vPos[3] = Vector3(l_fPosX+m_iSingleImageWidth	,l_fPosY+m_iSingleImageHeight,0.f);
				Assign4VerticesDataTo2Triangles((float*)l_vPos,l_pfVertices,3);
				l_fPosX += l_fOffsetPosX;
				l_fPosY += l_fOffsetPosY;
			}
			cBaseShader*l_p2DShader = GetCurrentShader();
			this->ApplyImage();
			UseShaderProgram(DEFAULT_SHADER);
			cMatrix44 l_mat;
			if( e_bCenter )
			{
				l_mat = cMatrix44::TranslationMatrix((float)e_iPosX,(float)e_iPosY,this->GetPos().z)*	cMatrix44::ZAxisRotationMatrix(this->GetAngle());
			}
			else
			{
				int	l_iMinusCount = l_iNum-1;
				l_mat = cMatrix44::TranslationMatrix((float)e_iPosX,(float)e_iPosY,GetPos().z)*
					cMatrix44::ZAxisRotationMatrix(this->GetAngle())*cMatrix44::TranslationMatrix(l_fHalfWidth-l_iMinusCount*m_iSingleImageWidth,l_fHalfHeight,0.f);
			}
			if( e_pmat )
			{
				l_mat = l_mat*cMatrix44(e_pmat);
			}
			SetupShaderWorldMatrix(l_mat);
			myGlVertexPointer(3,g_fMPDIOptmizeRenderVertices);
			myGlUVPointer(2,g_fMPDIOptmizeRenderUV);
			myGlColorPointer(4,g_fMPDIOptmizeRenderColor);
			MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, l_iNum*6);
			UseShaderProgram(l_p2DShader);
		}
	}
	void	cNumeralImage::Draw(int64	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat,bool e_bCenter)
	{
		if( !m_bVisible )
			return;
		std::string	l_str = ValueToString(e_iValue);
		int	l_iNum = (int)strlen(l_str.c_str());
		Draw(l_str.c_str(),l_iNum,e_iPosX,e_iPosY,e_pmat,e_bCenter);
	}
	void	cNumeralImage::Draw(int	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat,bool e_bCenter)
	{
		if( !m_bVisible )
			return;
		char	l_str[TEMP_SIZE];
		itoa(e_iValue,l_str,10);
		int	l_iNum = (int)strlen(l_str);
		Draw(l_str,l_iNum,e_iPosX,e_iPosY,e_pmat,e_bCenter);
	}

	void	cNumeralImage::DrawOnCenter(int64	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat)
	{
		Draw(e_iValue,e_iPosX,e_iPosY,e_pmat,true);
	}

	void	cNumeralImage::DrawOnCenter(int	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat)
	{
		Draw(e_iValue,e_iPosX,e_iPosY,e_pmat,true);
	}

	void	cNumeralImage::Render()
	{
		Vector3	l_vPos = this->GetPos();
		Draw(m_i64Value,(int)l_vPos.x,(int)l_vPos.y);
	}

	void	cNumeralImage::SetValue(int64 e_i64Value)
	{
		m_i64Value = e_i64Value;
	}
}