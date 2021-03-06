#include "../stdafx.h"
#include "glLoadImage.h"
#include "SimplePrimitive.h"
#include "../Utility.h"
#include "BinaryToTexture.h"
#include "../GameplayUT/GameApp.h"
#include "../GameplayUT/BinaryFile.h"
#include "pngLoader.h"
#if defined(WIN32)
#include "../../../Include/IL/il.h"
//if u like to link under windows copy and paste to the main.cpp
//#pragma comment(lib, "../../lib/unicode/Devil.lib")
#elif defined(IOS)
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIImage.h>
#import <UIKit/UIImagePickerController.h>
#elif defined(ANDROID)
#include "../Android/nv_images.h"
#elif defined(LINUX)
#include "IL/il.h"//linux
#endif
#ifdef IOS
//saving an image 
UIImage * glToUIImage()
{
    //int l_iWidth  = (int)cGameApp::m_svViewPortSize.x;
    //int l_iHeight  = (int)cGameApp::m_svViewPortSize.y;
    int l_iWidth  = (int)FATMING_CORE::cGameApp::m_svViewPortSize.w;
    int l_iHeight  = (int)FATMING_CORE::cGameApp::m_svViewPortSize.z;

    NSInteger myDataLength =  l_iWidth * l_iHeight * 4;
 
    // allocate array and read pixels into it.
    GLubyte *buffer = (GLubyte *) malloc(myDataLength);
    glReadPixels(0, 0, l_iWidth, l_iHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
 
    // gl renders "upside down" so swap top to bottom into new array.
    // there's gotta be a better way, but this works.
    GLubyte *buffer2 = (GLubyte *) malloc(myDataLength);
    for(int y = 0; y < l_iHeight; y++)
    {
        for(int x = 0; x < l_iWidth * 4; x++)
        {
            buffer2[(l_iHeight-1 - y) * l_iWidth * 4 + x] = buffer[y * 4 * l_iWidth + x];
        }
    }
 
    // make data provider with data.
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, buffer2, myDataLength, NULL);
 
    // prep the ingredients
    int bitsPerComponent = 8;
    int bitsPerPixel = 32;
    int bytesPerRow = 4 * l_iWidth;
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
 
    // make the cgimage
    CGImageRef imageRef = CGImageCreate(l_iWidth, l_iHeight, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
 
    // then make the uiimage from that
    UIImage *myImage = [UIImage imageWithCGImage:imageRef];
    return myImage;
}
 
void    captureToPhotoAlbum()
{
    UIImage *image = glToUIImage();
    UIImageWriteToSavedPhotosAlbum(image, nil, nil, nil);
    //[image release];
} 
#endif 
namespace FATMING_CORE
{
	//if graphic card does not support non power of two,set as false
#ifdef OPENGLES_2_X
	bool	g_bSupportNonPowerOfTwoTexture = true;
	//bool	g_bSupportNonPowerOfTwoTexture = false;
#else
	bool	g_bSupportNonPowerOfTwoTexture = false;
#endif
	UINT	g_iAteVideoMomory = 0;//in KB
	//intel chip,return MB
	int GetVideoMemorySize()
	{
		DWORD	i[5] = { 0, 0, 0x27, 0, 0 };
		DWORD	o[5] = { 0, 0, 0, 0, 0 };
	 
		HDC hdc = CreateDC(L"DISPLAY", 0, 0, 0);
	 
		if (hdc == NULL) 
		{
			return 0;
		}
	 
		int s = ExtEscape(hdc, 0x7032, 0x14, (LPCSTR)i, 0x14, (LPSTR)o);
	 
		DeleteDC(hdc);
	 
		if (s <= 0) 
		{
			return 0;
		}
	 
		return o[3];
	}
	//check video ram,return by kb
	int	CheckRestVRam()
	{
		//const GLubyte*l_str1 = glGetString(GL_VERSION);//const GLubyte*l_str3 = glGetString(GL_RENDERER);
		const GLubyte*l_str2 = glGetString(GL_VENDOR);
		#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
		#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
		char str[] ="NVIDIA";char * pch = strstr ((char*)l_str2,str);
		if( pch )//this is nvidia
		{
			//GLint total_mem_kb = 0;
			//glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,&total_mem_kb);
			//int	l_iTotalMB = total_mem_kb/1024;
			//int	l_iRestTotalKB = total_mem_kb%1024;
			GLint cur_avail_mem_kb = 0;
			glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,&cur_avail_mem_kb);
			return cur_avail_mem_kb;
		}
		else
		{//ATI
			if (strstr((char*)glGetString(GL_EXTENSIONS), "GL_ATI_meminfo"))
			{//http://www.geeks3d.com/20100531/programming-tips-how-to-know-the-graphics-memory-size-and-usage-in-opengl/
				#ifndef GL_VBO_FREE_MEMORY_ATI
				#define GL_VBO_FREE_MEMORY_ATI 0x87FB
				#endif
				#ifndef GL_TEXTURE_FREE_MEMORY_ATI
				#define GL_TEXTURE_FREE_MEMORY_ATI 0x87FC
				#endif
				#ifndef GL_RENDERBUFFER_FREE_MEMORY_ATI
				#define GL_RENDERBUFFER_FREE_MEMORY_ATI 0x87FD
				#endif
				//returns in kB, so if you want MB,divide by 1024
				int free_mem = -1;
				glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI,&free_mem);
				return free_mem;
				
			}
			else
			{//intel?
				return GetVideoMemorySize();
			}
		}
		//have no idea what the graphic card is it
		return 0;
	}

	void	DeleteTexture(cSimpleGLTexture*e_pSimpleGLTexture)
	{
		int	l_iBeforeAlivableVRamKB = CheckRestVRam();
		GLuint	l_iImageIndex = e_pSimpleGLTexture->GetImageIndex();
		glDeleteTextures(1,&l_iImageIndex);
#ifdef WIN32
#ifdef DEBUG
		int	l_iAfterAlivableVRamKB = CheckRestVRam();
		int	l_iDiffErentKB = l_iBeforeAlivableVRamKB-l_iAfterAlivableVRamKB;
		int	l_iAteVRamKB = e_pSimpleGLTexture->GetWidth()*e_pSimpleGLTexture->GetHeight()*(e_pSimpleGLTexture->GetPixelFormat() == GL_RGBA?4:3)/1024;
		g_iAteVideoMomory -= l_iAteVRamKB;

		//g_iAteVideoMomory += l_iValue;
		float	l_fMB = l_iAteVRamKB/1024.f;
		float	l_fTotalMB = g_iAteVideoMomory/1024.f;
		std::wstring	l_str = UT::ComposeMsgByFormat(L"DeleteTexture-------------\n:%ls:\t\t\t\t\t\t%.2fMB,\nVideoMomory Use:%.2fMB\nRestVRam:%.2f\n-------------\n",e_pSimpleGLTexture->GetName(),l_fMB,l_fTotalMB,l_iAfterAlivableVRamKB/1024.f);
	//	if(cGameApp::m_spLogFile)
		//{
		//	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
		//}
		//else
		{
			cGameApp::OutputDebugInfoString(l_str.c_str());
		}
#endif
#endif
	}
	void MyTextureGenerate(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels,const WCHAR*e_strFileName)
	{
		int	l_iBeforeAlivableVRamKB = CheckRestVRam();
		glTexImage2D(target,level,internalformat, width,height,border,format,type,pixels);
#ifdef WIN32
#ifdef DEBUG
		int	l_iAfterAlivableVRamKB = CheckRestVRam();
		int	l_iDiffErentKB = l_iBeforeAlivableVRamKB-l_iAfterAlivableVRamKB;
		int	l_iAteVRamKB = (width*height*(format==GL_RGBA?4:3))/1024;
		g_iAteVideoMomory += l_iAteVRamKB;

		//g_iAteVideoMomory += l_iValue;
		float	l_fMB = l_iAteVRamKB/1024.f;
		float	l_fTotalMB = g_iAteVideoMomory/1024.f;
		std::wstring	l_str = UT::ComposeMsgByFormat(L"GenerateTexture-------------\n%ls:\t\t\t\t\t\t%.2fMB,\nVideoMomory Use:%.2fMB\nRestVRam:%.2f\n-------------\n",e_strFileName,l_fMB,l_fTotalMB,l_iAfterAlivableVRamKB/1024.f);
	//	if(cGameApp::m_spLogFile)
		//{
		//	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
		//}
		//else
		{
			cGameApp::OutputDebugInfoString(l_str.c_str());
		}
#endif
#endif

	}
	//GLfloat	g_fMAG_FILTERValue = GL_NEAREST;
	//GLfloat	g_fMIN_FILTERValue = GL_NEAREST;
	GLfloat	g_fMAG_FILTERValue = GL_LINEAR;
	GLfloat	g_fMIN_FILTERValue = GL_LINEAR;
    GLuint  cSimpleGLTexture::m_suiLastUsingImageIndex = -1;

	float*	UVToTriangleStrip(float*e_pfUV)
	{
		static	float l_sUV[TWO_TRIANGLE_STRIP_UV_TO_QUAD_UV_COUNT];
		//0
		l_sUV[0] = e_pfUV[0];l_sUV[1] = e_pfUV[1];
		//1
		l_sUV[2] = e_pfUV[2];l_sUV[3] = e_pfUV[1];
		//2
		l_sUV[4] = e_pfUV[0];l_sUV[5] = e_pfUV[3];
		//3
		l_sUV[6] = e_pfUV[2];l_sUV[7] = e_pfUV[3];
		return l_sUV;
	}
	float*	UVToTwoTriangle(float*e_pfUV)
	{
		static	float l_sUV[TWO_TRIANGLE_VERTICES_UV_TO_QUAD_UV_COUNT];
		//2,3   5
		//0     1,4
		//0
		l_sUV[0] = e_pfUV[0];l_sUV[1] = e_pfUV[3];
		//1
		l_sUV[2] = e_pfUV[2];l_sUV[3] = e_pfUV[3];
		//2
		l_sUV[4] = e_pfUV[0];l_sUV[5] = e_pfUV[1];
		//3
		l_sUV[6] = e_pfUV[0];l_sUV[7] = e_pfUV[1];
		//4
		l_sUV[8] = e_pfUV[2];l_sUV[9] = e_pfUV[3];
		//5
		l_sUV[10] = e_pfUV[2];l_sUV[11] = e_pfUV[1];
		return l_sUV;
	}

	//IL_NO_ERROR
	//No detectable error has occured.
	//IL_INVALID_ENUM, ILU_INVALID_ENUM, ILUT_INVALID_ENUM
	//An unacceptable enumerated value was passed to a function.
	//IL_OUT_OF_MEMORY, ILU_OUT_OF_MEMORY, ILUT_OUT_OF_MEMORY
	//Could not allocate enough memory in an operation.
	//IL_FORMAT_NOT_SUPPORTED
	//The format a function tried to use was not able to be used by that function.
	//IL_INTERNAL_ERROR, ILU_INTERNAL_ERROR
	//A serious error has occurred. Please e-mail DooMWiz with the conditions leading up to this error being reported.
	//IL_INVALID_VALUE, ILU_INVALID_VALUE, ILUT_INVALID_VALUE
	//An invalid value was passed to a function or was in a file.
	//IL_ILLEGAL_OPERATION, ILU_ILLEGAL_OPERATION, ILUT_ILLEGAL_OPERATION
	//The operation attempted is not allowable in the current state. The function returns with no ill side effects.
	//IL_ILLEGAL_FILE_VALUE
	//An illegal value was found in a file trying to be loaded.
	//IL_INVALID_FILE_HEADER
	//A file's header was incorrect.
	//IL_INVALID_PARAM, ILU_INVALID_PARAM, ILUT_INVALID_PARAM
	//An invalid parameter was passed to a function, such as a NULL pointer.
	//IL_COULD_NOT_OPEN_FILE, ILUT_COULD_NOT_OPEN_FILE
	//Could not open the file specified. The file may already be open by another app or may not exist.
	//IL_INVALID_EXTENSION
	//The extension of the specified filename was not correct for the type of image-loading function.
	//IL_FILE_ALREADY_EXISTS
	//The filename specified already belongs to another file. To overwrite files by default, call ilEnable with the IL_FILE_OVERWRITE parameter.
	//IL_OUT_FORMAT_SAME
	//Tried to convert an image from its format to the same format.
	//IL_STACK_OVERFLOW, ILUT_STACK_OVERFLOW
	//One of the internal stacks was already filled, and the user tried to add on to the full stack.
	//IL_STACK_UNDERFLOW, ILUT_STACK_UNDERFLOW
	//One of the internal stacks was empty, and the user tried to empty the already empty stack.
	//IL_INVALID_CONVERSION
	//An invalid conversion attempt was tried.
	//IL_LIB_JPEG_ERROR
	//An error occurred in the libjpeg library.
	//IL_LIB_PNG_ERROR
	//An error occurred in the libpng library.
	//IL_UNKNOWN_ERROR
	//No function sets this yet, but it is possible (not probable) it may be used in the future.
	//ILUT_NOT_SUPPORTED
	//A type is valid but not supported in the current build.
	//here si the idea for store screen buffer to 4 block
	//http://www.gamedev.net/community/forums/topic.asp?topic_id=489893
	//here we divided screen to 4 parts
	//       
	//  |---|---|
	//  | 1 | 2 |
	//  |---|---|
	//  | 3 | 4 |
	//  |---|---|



	//Lord_Evil, you talk about GL_PACK as if the alignment was its only parameter. There are many parameters you can adjust,
	//and alignment is indeed one of them, but I'm talking about adjusting row skip, pixel skip and row length.
	//
	//GLubyte *img = new GLubyte[1280*1024*4];
	//
	//glPixelStorei(GL_PACK_ROW_LENGTH, 1280);
	//
	//// Block 1
	//glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_PACK_SKIP_ROWS, 512);
	//glReadPixels(0, 512, 640, 512, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//
	//// Block 2
	//glPixelStorei(GL_PACK_SKIP_PIXELS, 640);
	//glPixelStorei(GL_PACK_SKIP_ROWS, 512);
	//glReadPixels(640, 512, 640, 512, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//
	//// Block 3
	//glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	//glReadPixels(0, 0, 640, 512, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//
	//// Block 4
	//glPixelStorei(GL_PACK_SKIP_PIXELS, 640);
	//glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	//glReadPixels(640, 0, 640, 512, GL_RGBA, GL_UNSIGNED_BYTE, img);


	//=====================
	//simple method to store the specific area to buffer
	//x, y, width, and height indicate the area of the screen you want to capture,
	//where x and y are the lower left of the rectangle, 
	//and width and height are the dimensions. 
	//If you want to read the whole screen, just set both x and y to 0, and width and height to the dimensions of the screen.

	//def readScreen(x, y, width, height):
	//    """ Read in the screen information in the area specified """
	//    glFinish()
	//    glPixelStorei(GL_PACK_ALIGNMENT, 4)
	//    glPixelStorei(GL_PACK_ROW_LENGTH, 0)
	//    glPixelStorei(GL_PACK_SKIP_ROWS, 0)
	//    glPixelStorei(GL_PACK_SKIP_PIXELS, 0)
	//
	//    data = glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE)
	//
	//    return data
	//

	//=====================






	//==============
	//http://ubuntuforums.org/showthread.php?t=396281
	//wybiral said:
	//when u loading a image with alpha u 'd call below beofre apply drawing
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.5);


	//http://www.gamedev.net/community/forums/topic.asp?topic_id=452780&forum_id=25&gforum_id=0
	//gluBuild2DMipmaps is very slow (+ buggy)
	//
	//if a texture is not power of 2 sized then gluBuild2DMipmaps will first resize it so it is, i assume thats where the problem lies somehow.
	//
	//to artifically see if mipmapping is working draw the texture with a high texture_lod_bias
	//perhaps draw the texture on a fullscreen sized polygon
	//+ then with a keypress cycle through the mipmap levels


	//If you want to use the GL_GENERATE_MIPMAP you don't use gluBuild2DMipmaps() you use glTexImage2D() and call the correct glTexParameteri() with true in it.
	//
	//The above method works, and will automatically generate mipmaps for your dynamic textures for all levels above base level.
	//
	//With GenerateMipmapEXT() it will generate all mipmaps above base also but when you choose to not the driver.
	//==============
	//glCopyTexSubImage2D
	
//- ()saveImage:(UIImage*)image:(NSString*)imageName
//{ 
//    NSData *imageData = UIImagePNGRepresentation(image); //convert image into .png format.     
//    NSFileManager *fileManager = [NSFileManager defaultManager];//create instance of NSFileManager     
//    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); //create an array and store result of our search for the documents directory in it     
//    NSString *documentsDirectory = [paths objectAtIndex:0]; //create NSString object, that holds our exact path to the documents directory     
//    NSString *fullPath = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.png", imageName]]; //add our image to the path     
//    [fileManager createFileAtPath:fullPath contents:imageData attributes:nil]; //finally save the path (image)
//    NSLog(@"image saved");
//}
// 
////removing an image
// 
//- ()removeImage:(NSString*)fileName {
// 
//NSFileManager *fileManager = [NSFileManager defaultManager];
// 
//NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
// 
//NSString *documentsDirectory = [paths objectAtIndex:0];
// 
//NSString *fullPath = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.png", fileName]];
// 
//[fileManager removeItemAtPath: fullPath error:NULL];
// 
//NSLog(@"image removed");
// 
//}
// 
////loading an image
// 
//- (UIImage*)loadImage:(NSString*)imageName {
// 
//NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
// 
//NSString *documentsDirectory = [paths objectAtIndex:0];
// 
//NSString *fullPath = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.png", imageName]];
// 
//return [UIImage imageWithContentsOfFile:fullPath];
// 
//Now, you can easily save an image like:
//
//1
//[self saveImage: myUIImage: @"myUIImageName"];
//or load it like:
//
//1
//myUIImage = [self loadImage: @"myUIImageName"];
//or remove it like:
//
//1
//[self removeImage: @"myUIImageName"

	void	SaveCurrentBufferToImage(char*e_strFileName,int e_iWidth,int e_iHeight)
	{
#if defined(WIN32)
		//if it not working check ILInit is called?
		ilEnable(IL_FILE_OVERWRITE);
		unsigned char *l_pPixelData = new unsigned char[e_iWidth*e_iHeight*3];
		glReadPixels( 0, 0, e_iWidth, e_iHeight, GL_RGB, GL_UNSIGNED_BYTE, l_pPixelData );
		ILuint uiID;
		ilGenImages( 1, &uiID );
		ilBindImage( uiID );
		ilTexImage( e_iWidth, e_iHeight, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, l_pPixelData);
		ilSave( IL_PNG, (WCHAR*)e_strFileName );
		ilDeleteImages(1, &uiID); // Because we have already copied image data into texture data
		delete l_pPixelData;
#elif defined(IOS)//for iphone,save into album
        captureToPhotoAlbum();
#elif defined(ANDROID)

#endif
	}

    void	SaveBufferToImage(char*e_strFileName,int e_iWidth,int e_iHeight,void*e_pPixel)
    {
#if defined(WIN32)
		ILuint uiID;
		ilGenImages( 1, &uiID );
		ilBindImage( uiID );
		ilTexImage( e_iWidth, e_iHeight, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, e_pPixel);
		ilSave( IL_PNG, (WCHAR*)e_strFileName );
		ilDeleteImages(1, &uiID); // Because we have already copied image data into texture data
#elif defined(IOS)//for iphone,save into album
        captureToPhotoAlbum();
#elif defined(ANDROID)
#endif
    }

	const wchar_t*     cSimpleGLTexture::TypeID( L"cSimpleGLTexture" );
	//===============
	//
	//===============
	cSimpleGLTexture::cSimpleGLTexture(NamedTypedObject*e_pOwner,unsigned char*e_pPixels,int e_iWidth,int e_iHeight,WCHAR*e_strName,bool e_bCopyPixels,bool e_bAllocateMemoryForPixelIfFetch,GLenum e_eImageType)
	{
		m_pstrFullFileName = 0;
		this->AddRef(e_pOwner);
	    m_pPixels = 0;
		m_fUV[0] = 0.f; m_fUV[1] = 0.f; m_fUV[2] = 1.f; m_fUV[3] = 1.f;
		m_uiImageIndex = -1;
		this->m_iHeight = e_iHeight;
		this->m_iWidth = e_iWidth;
		int	l_iNumChannel = 4;
		if( e_bCopyPixels )
		{
			if( e_bAllocateMemoryForPixelIfFetch )
			{
				int	l_iSize = m_iHeight*m_iWidth;
				if( e_eImageType == GL_RGBA )
				{
					m_pPixels = new unsigned char[l_iSize*4];
					memcpy(m_pPixels,e_pPixels,sizeof(unsigned char)*l_iSize*4);
				}
				else
				if( e_eImageType == GL_RGB )
				{
					m_pPixels = new unsigned char[l_iSize*3];
					memcpy(m_pPixels,e_pPixels,sizeof(unsigned char)*l_iSize*3);
					l_iNumChannel = 3;
				}
			}
			else
				m_pPixels = e_pPixels;
		}
		else
			m_pPixels = 0;
		m_iPixelFormat = e_eImageType;

		glGenTextures(1, &m_uiImageIndex); /* Texture name generation */
		assert(m_uiImageIndex!=(UINT)-1&&"opengl init???");
		GLenum	l_GLenum = glGetError();
#ifdef 	OPENGLES_2_X
		glActiveTexture( GL_TEXTURE0 );
#endif
		m_suiLastUsingImageIndex = -1;
		this->ApplyImage();/* Binding of texture name */
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear  interpolation for magnification filter */
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear  interpolation for minifying filter */
		//quick way to find out power of two.
		//if (width & (width - 1)) or (height & (height - 1)):
		int	l_iWidthPO2 = power_of_two(m_iWidth);
		int	l_iHeightPO2 = power_of_two(m_iHeight);
		GLint texSize; glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
		//make sure power of 2,because not every fukcing graphic card support it
		//but if u exactly sure it do support power of 2 u could mark this.
		if( !g_bSupportNonPowerOfTwoTexture&&(l_iWidthPO2!=m_iWidth||l_iHeightPO2!=m_iHeight) )
		{
			assert((l_iWidthPO2<=texSize||l_iHeightPO2<=texSize)&&"texture size is too big then card support");
	#ifdef WIN32
	#ifdef DEBUG
			cGameApp::OutputDebugInfoString(e_strName);
			cGameApp::OutputDebugInfoString(L"  image has not correct respond width and height,because none power of 2\n");
			if( m_iPixelFormat == GL_RGB )
				cGameApp::OutputDebugInfoString(L"UV is changed,because image size is not to become power of 2");
	#endif
	#endif
			char*l_pNewPixelData = TextureToPowOfTwo((char*)e_pPixels,m_iWidth,m_iHeight,m_iPixelFormat==GL_RGBA?true:false);
			MyTextureGenerate(GL_TEXTURE_2D, 0, l_iNumChannel, l_iWidthPO2,l_iHeightPO2, 0,l_iNumChannel==4?GL_RGBA:GL_RGB, GL_UNSIGNED_BYTE,l_pNewPixelData,e_strName); // Texture specification.
			GLenum	l_error = glGetError();
			//assign new UV
			m_fUV[2] = (float)m_iWidth/l_iWidthPO2;
			m_fUV[3] =  (float)m_iHeight/l_iHeightPO2;
			delete l_pNewPixelData;
		}
		else
		{
			assert((m_iWidth<=texSize||m_iHeight<=texSize)&&"texture size is too big then card support");
			MyTextureGenerate(GL_TEXTURE_2D, 0, l_iNumChannel, m_iWidth,m_iHeight, 0,m_iPixelFormat, GL_UNSIGNED_BYTE,e_pPixels,e_strName); // Texture specification.
		}
	}
	//===============
	//
	//===============
	cSimpleGLTexture::cSimpleGLTexture(NamedTypedObject*e_pOwner,const char*e_strImageFileName,bool e_bFetchPixelData)
	{
		m_pstrFullFileName = 0;
		this->AddRef(e_pOwner);
	    m_pPixels = 0;
		m_uiImageIndex = -1;
		m_fUV[0] = 0.f; m_fUV[1] = 0.f; m_fUV[2] = 1.f; m_fUV[3] = 1.f;
		this->m_iHeight = 0;
		this->m_iWidth = 0;
		if( !e_strImageFileName )
		{//if image resource does't need to read,the texture resource is come from external,so we do not to delete it
			return;
		}
		LoadImage(e_strImageFileName,e_bFetchPixelData);
	}
	//===============
	//
	//===============
	cSimpleGLTexture::~cSimpleGLTexture()
	{
		Destroy();
	}
	//===============
	//
	//===============
	bool	cSimpleGLTexture::LoadImage(const char*e_strImageFileName,bool e_bFetchPixelData)
	{
		assert(e_strImageFileName);
		//delete old one if existed
		Destroy();
		char*	l_strImageName = UT::GetFileNameWithoutFullPath(e_strImageFileName);
		std::wstring	l_ssss=  CharToWchar(l_strImageName);
		const wchar_t*	l_strImageNameW = l_ssss.c_str();
		//is loading the same image again?
		if( this->IsSameName(l_strImageNameW) )
			return false;
		//asign new name
		this->SetName(l_strImageNameW);
		m_pstrFullFileName = new std::string;
		*m_pstrFullFileName = e_strImageFileName;
		std::vector<unsigned char> image;
		unsigned l_uWidth, l_uHeight;
		unsigned error = lodepng::decode(image, l_uWidth, l_uHeight, e_strImageFileName);

		m_iWidth = l_uWidth;
		m_iHeight = l_uHeight;
		// If there's an error, display it.
		if(error != 0)
		{
			//UT::ErrorMsg(e_strImageFileName,lodepng_error_text(error));
			return false;
		}
		else
		{
			m_iPixelFormat = GL_RGBA;
			glGenTextures(1, &m_uiImageIndex); /* Texture name generation */
			//		GLenum	l_GLenum = glGetError();
			//		assert(l_GLenum);
#ifdef 	OPENGLES_2_X
			glActiveTexture( GL_TEXTURE0  );
#endif
			m_suiLastUsingImageIndex = -1;
			this->ApplyImage();
			// Set the texture parameters to use a minifying filter and a linear filer (weighted average)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);				
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear  interpolation for magnification filter */
		    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			int	l_iWidthPO2 = power_of_two(m_iWidth);
			int	l_iHeightPO2 = power_of_two(m_iHeight);
			if(  !g_bSupportNonPowerOfTwoTexture && (l_iWidthPO2!=m_iWidth||l_iHeightPO2!=m_iHeight) )//make it power of two
			{
				char*l_pNewPixelData = TextureToPowOfTwo((char*)&image[0],m_iWidth,m_iHeight,true);
				MyTextureGenerate(GL_TEXTURE_2D, 0, m_iPixelFormat, l_iWidthPO2,l_iHeightPO2, 0,GL_RGBA, GL_UNSIGNED_BYTE,l_pNewPixelData,UT::CharToWchar(e_strImageFileName).c_str()); // Texture specification.	
				//assign new UV
				m_fUV[2] = (float)m_iWidth/(float)l_iWidthPO2;
				m_fUV[3] =  (float)m_iHeight/(float)l_iHeightPO2;		
				if( e_bFetchPixelData )
				{//because we wanna to detected the pixels alpha for collide so we have to resize
					m_pPixels = (unsigned char*)l_pNewPixelData;
				}
				else
				{//we do not need the alpha so modify UV to show the part we needed
					delete l_pNewPixelData;
				}		
			}
			else
			{
				MyTextureGenerate(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth,m_iHeight, 0,m_iPixelFormat, GL_UNSIGNED_BYTE,&image[0],UT::CharToWchar(e_strImageFileName).c_str()); // Texture specification.
				if( e_bFetchPixelData )
				{
					int	l_iDataSize = m_iWidth*m_iHeight*4;
					m_pPixels = (unsigned char*)malloc(l_iDataSize);
					memcpy(m_pPixels,&image[0],sizeof(char)*l_iDataSize);
				}		
			}
		}
//#endif
		//if loading failed show alert
		if(!m_uiImageIndex)
		{
			ErrorMsg(L"opengl process Image Failed",UT::CharToWchar(e_strImageFileName));
			return false;
		}
		return true;
	}	
	bool	cSimpleGLTexture::ApplyImage()
	{
	    if( m_suiLastUsingImageIndex != m_uiImageIndex )
	    {
			if( m_uiImageIndex == 0 )
				return false;
		    glBindTexture( GL_TEXTURE_2D, m_uiImageIndex);
		    m_suiLastUsingImageIndex = m_uiImageIndex;
			return true;
		}
		return true;
	}
	//===============
	//
	//===============
	void	cSimpleGLTexture::Destroy()
	{
		SAFE_DELETE(m_pstrFullFileName);
		if( m_uiImageIndex )
		{
			if( m_uiImageIndex != -1 )
				DeleteTexture(this);
			m_uiImageIndex = -1;
			if(m_pPixels)//if the pixels data is not from resource,free it
				free(m_pPixels);
		}
	}

	float*	cSimpleGLTexture::GetUV()
	{
		return m_fUV;
	}

}