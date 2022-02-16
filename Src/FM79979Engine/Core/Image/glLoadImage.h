#ifndef glLoadImage_H
#define glLoadImage_H
#include "../SmartPointer.h"
namespace FATMING_CORE
{
	extern bool	g_bSupportNonPowerOfTwoTexture;
	//glCopyTexSubImage2D
	void	SaveCurrentBufferToImage(char*e_strFileName,int e_iWidth,int e_iHeight);
	void	SaveBufferToImage(char*e_strFileName,int e_iWidth,int e_iHeight,void*e_pPixel);

	float* UVToTriangleStrip(float*e_pfUV);
	float* UVToTwoTriangle(float*e_pfUV);
	//=======================
	//base image that contain UV and width and height data
	//also contain the pixel data if u want to fetch it.
	//please do not Inheritance it,it's a smart pointer!
	//=======================
		class	cSimpleGLTexture:public cSmartObject
		{
		protected:
			//texture original size.won't be effected by power of two
			int	m_iWidth;
			int	m_iHeight;
			//UV  left,top,right,button,it could possible not be 0,0,1,1 if image is not power of 2.
			float	m_fUV[4];														//left top right buttom.
			//pixels format,GL_RGB,GL_RGBA.
			GLint	m_iPixelFormat;
			//pixel data.
			GET_SET_DEC(unsigned char*,m_pPixels,GetPixels,SetPixels);
			//texture ID.
			GLuint	m_uiImageIndex;
		public:
		    //last image index has been used
            static  GLuint  m_suiLastUsingImageIndex;
			DEFINE_TYPE_INFO();
			//if e_bCopyPixels is true,u have to ensure the e_pPixels has delete or keep in somewhere u want keep!!
			//!!ensure u know how to use it!
			cSimpleGLTexture(NamedTypedObject*e_pOwner,unsigned char*e_pPixels,int e_iWidth,int e_iHeight,WCHAR*e_strName,bool e_bCopyPixels,bool e_bAllocateMemoryForPixelIfFetch,GLenum e_eImageType = GL_RGBA);
			cSimpleGLTexture(NamedTypedObject*e_pOwner,const char*e_strImageFileName,bool e_bFetchPixelData = false);
			virtual ~cSimpleGLTexture();
			void	Destroy();

			//it e_bFetchPixelData true m_pPixels will fill of pixel data.
			bool	LoadImage(const char*e_strImageFileName,bool e_bFetchPixelData = false);
			bool	ApplyImage();
			GLuint	GetImageIndex(){ return m_uiImageIndex;}
			//should't call this!!,instead u should call SetWidth and SetHeight.
			void	SetImageSize(int e_iWidth,int e_iHeight){ m_iWidth = e_iWidth; m_iHeight = e_iHeight; }
			int		GetWidth(){ return m_iWidth; }
			int		GetHeight(){ return m_iHeight; }
			GLint	GetPixelFormat(){ return m_iPixelFormat; }
			inline	bool	IsSameTextureID(cSimpleGLTexture*e_pSimpleGLTexture)
			{
				return e_pSimpleGLTexture->m_uiImageIndex == m_uiImageIndex?true:false;
			}
			virtual WCHAR*DescriptionInfo()
			{
			    static  WCHAR   l_sStrDescription[MAX_PATH];
				char   l_scStrDescription[MAX_PATH];
			    sprintf(l_scStrDescription,"Size:%d,%d\nUV:%.2f,%2.f,%.2f,%.2f",m_iWidth,m_iHeight,m_fUV[0],m_fUV[1],m_fUV[2],m_fUV[3]);
				CHAR_TO_WCHAR(l_scStrDescription,l_sStrDescription);
			    return l_sStrDescription;
			}
			float*	GetUV();
			std::string*		m_pstrFullFileName;
		};
		//sample to get pixel by texture ID
		////Use our texture
		//glBindTexture( GL_TEXTURE_2D, m_uiImageIndex );
		//   
		////Get the pixel data
		//glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pPixels );
		//m_iPixelFormat = GL_RGBA;
}
#endif