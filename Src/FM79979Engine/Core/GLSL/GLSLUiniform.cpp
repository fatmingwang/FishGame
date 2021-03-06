#include "../stdafx.h"
//#include "../XML/XMLLoader.h"
#include "GLSLUiniform.h"

#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <assert.h>
#include "../GameplayUT/GameApp.h"
	//cUniformManager*g_p2DUniformManager = 0;
	////using a default shader for 2D only
	//bool	Init2DShader(char*e_strFileName)
	//{
	//	assert(g_p2DUniformManager==0);
	//	SAFE_DELETE(g_p2DUniformManager);
	//	g_p2DUniformManager = new cUniformManager();
	//	bool	l_b = false;
	//#ifdef __IPHONE
	//	char	l_strValue[MAX_PATH];
	//	GetAppleBundelResourcePathByObjectPath(e_strFileName,l_strValue);
	//	l_b = g_p2DUniformManager->Parse(l_strValue);
	//#else
	//#ifdef __ANDROID__
	//	l_b = g_p2DUniformManager->Parse(e_strFileName);
	//#else//wind32,linux
	//	l_b = g_p2DUniformManager->Parse(e_strFileName);
	//#endif
	//#endif
	//	return l_b;
	//}
	////delete the data above
	//void	Destroy2DShader()
	//{
	//	SAFE_DELETE(g_p2DUniformManager);
	//}
using namespace std;
namespace FATMING_CORE
{
	cGLSLProgram*g_pCurrentUsingGLSLProgram = 0;
	const wchar_t*         cUniformManager::TypeID( L"cUniformManager" );
	//=================================================================================================================================
	///
	/// Constructor
	///
	/// \param none
	///
	/// \return null
	//=================================================================================================================================
	cUniformData::cUniformData() :
	   m_iLocation(-1),
	   m_strName(NULL),
	   m_eDatatype(0),
	   m_uiTextureHandle(0),
	   m_uiTextureUnit(0),
	   m_bDepthTex(false),
	   m_iWidth(0),
	   m_iHeight(0),
	   m_uiFBOHandle(0),
	   m_uiColorRbHandle(0),
	   m_uiVertShaderHandle(0),
	   m_uiFragShaderHandle(0)
	{
	}

	//=================================================================================================================================
	///
	/// Destructor
	///
	/// \param none
	///
	/// \return null
	//=================================================================================================================================
	cUniformData::~cUniformData()   
	{
	   if ( m_strName != NULL )
	   {
		  delete [] m_strName;
		  m_strName = NULL;
	   }
	}


	cUniformManager::cUniformManager():ISAXCallback(true)
	{
		m_uiRecentProgramHandle = 0;
	}

	cUniformManager::~cUniformManager()
	{

	}

	//<GLSL ProgramName="mainProg" VS="data/vertex.glsl" FS="data/fragment.glsl" >
	//	<!-- sample 1D,2D,3D -->
	//	<Texture Name="tex0" samplerDimesion="2" />
	//	<!-- attribute enusude u call the vertexpointer and setup index to cooresponded-->
	//	<Attribute Name="position" Index="0">
	//	<Attribute Name="texture" Index="1">
	//	<Attribute Name="normal" Index="2">
	//	<!-- uniform -->
	//	<Uniform Name="MatrixVP" Type="float44" Data="1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,"/>
	//	<Uniform Name="LightPos" Type="float3" Data="0.f,0.f,0.f"/>	
	//<HLSL/>
	void	cUniformManager::ProcessGLSLData()
	{
		//char	l_strShaderFileName[MAX_PATH];
		cGLSLProgram*l_pGLSLProgram = new cGLSLProgram();
		m_pCurrentGLSLProgram = l_pGLSLProgram;
		m_pCurrentUniformData->m_eDatatype = SAMPLE_PROGRAM;
		// Create the one program we are going to use.//ensure glewInit is called
		m_uiRecentProgramHandle = glCreateProgram();
		m_pCurrentGLSLProgram->SetProgramHandle(m_uiRecentProgramHandle);
		PARSE_NAME_VALUE_START
			COMPARE_NAME("ProgramName")
			{
				int	l_iLength = (int)wcslen(l_strValue);
				m_pCurrentUniformData->m_strName = new char [l_iLength+1];
				sprintf(m_pCurrentUniformData->m_strName,"%s\0",UT::WcharToChar(l_strValue));
				m_pCurrentGLSLProgram->SetName(UT::CharToWchar(m_pCurrentUniformData->m_strName));
			}
			else
			COMPARE_NAME("VS")
			{
				m_pCurrentUniformData->m_uiVertShaderHandle = glCreateShader( GL_VERTEX_SHADER );
				LoadShaderObject( UT::WcharToChar(l_strValue).c_str(), m_pCurrentUniformData->m_uiVertShaderHandle );
			}
			else
			COMPARE_NAME("FS")
			{
				m_pCurrentUniformData->m_uiFragShaderHandle = glCreateShader( GL_FRAGMENT_SHADER );
				LoadShaderObject( UT::WcharToChar(l_strValue).c_str(), m_pCurrentUniformData->m_uiFragShaderHandle );
			}
		PARSE_NAME_VALUE_END
		// Attach them to the program.
		glAttachShader( m_uiRecentProgramHandle, m_pCurrentUniformData->m_uiVertShaderHandle );
		glAttachShader( m_uiRecentProgramHandle, m_pCurrentUniformData->m_uiFragShaderHandle );
		// Link the whole program together.
		glLinkProgram( m_uiRecentProgramHandle );
		// Check for link success
		glUseProgram( m_uiRecentProgramHandle );
		this->m_pCurrentGLSLProgram->UpdateAllUniforms();
		CheckProgram(m_uiRecentProgramHandle,GL_LINK_STATUS,L"link");
	}

	void	cUniformManager::ProcessTextureData()
	{
	//	<Texture Name="tex0" SamplerDimesion="2" Unit="0" />
		PARSE_NAME_VALUE_START
			COMPARE_NAME("Name")
			{
				int	l_iLength = (int)wcslen(l_strValue);
				m_pCurrentUniformData->m_strName = new char [l_iLength+1];
				sprintf(m_pCurrentUniformData->m_strName,"%s\0",UT::WcharToChar(l_strValue));
			}
			else
			COMPARE_NAME("SamplerDimesion")//1D,2D,3D
			{
		
			}
			else
			COMPARE_NAME("Unit")
			{
				m_pCurrentUniformData->m_uiTextureUnit = (UINT)_wtoi(l_strValue);
			}
		PARSE_NAME_VALUE_END
		m_pCurrentUniformData->m_eDatatype = SAMPLE_INT;
		//u have to assign texture ID
		m_pCurrentUniformData->m_uiTextureHandle = -1;
		glUseProgram(m_uiRecentProgramHandle);
		m_pCurrentUniformData->m_iLocation = glGetUniformLocation( m_uiRecentProgramHandle,"tex0" );
		assert(m_pCurrentUniformData->m_iLocation!=-1);
		//http://www.gamedev.net/community/forums/topic.asp?topic_id=516840
		//Because half of your shader doesn't do anything, and is optimized
		//away by the compiler. The uniforms don't exist anymore after compilation 
		//(because they aren't used) and will therefore not be found by glGetUniformLocation.
		//Unless you are accessing them by a pixel shader. But since you didn't post any, my 
		//first assumption would be this.
		glUniform1i( m_pCurrentUniformData->m_iLocation, m_pCurrentUniformData->m_uiTextureUnit );
	}

	void	cUniformManager::ProcessAttributeData()
	{
		std::wstring	l_strAttributeName;
		int	l_iIndex = -1;
		PARSE_NAME_VALUE_START
			COMPARE_NAME("Name")
			{
				l_strAttributeName = l_strValue;
			}
			else
			COMPARE_NAME("Type")
			{
				COMPARE_VALUE_WITH_DEFINE( VERTEX_ATTRIBUTE_POSITION )
				{
					l_iIndex = 0;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(VERTEX_ATTRIBUTE_NORMAL)
				{
					l_iIndex = 1;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(VERTEX_ATTRIBUTE_TEXTURE)
				{
					l_iIndex = 2;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(VERTEX_ATTRIBUTE_COLOR)
				{
					l_iIndex = 3;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(VERTEX_ATTRIBUTE_TANGENT)
				{
					l_iIndex = 4;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(VERTEX_ATTRIBUTE_BITANGENT)
				{
					l_iIndex = 5;
				}
			}
		PARSE_NAME_VALUE_END
		assert(l_iIndex!=-1);
		this->m_pCurrentGLSLProgram->AddAttributeData(UT::WcharToChar(l_strAttributeName.c_str()),l_iIndex);
		SAFE_DELETE(m_pCurrentUniformData);
	}

	void	cUniformManager::ProcessUniformData()
	{
		PARSE_NAME_VALUE_START
			COMPARE_NAME("Name")
			{
				int	l_iLength = (int)wcslen(l_strValue);
				m_pCurrentUniformData->m_strName = new char [l_iLength+1];
				sprintf(m_pCurrentUniformData->m_strName,"%s\0",UT::WcharToChar(l_strValue));
				this->m_pCurrentUniformData->m_iLocation = glGetUniformLocation( m_uiRecentProgramHandle, UT::WcharToChar(l_strValue).c_str() );
				//http://www.gamedev.net/community/forums/topic.asp?topic_id=516840
				//Because half of your shader doesn't do anything, and is optimized
				//away by the compiler. The uniforms don't exist anymore after compilation 
				//(because they aren't used) and will therefore not be found by glGetUniformLocation.
				//Unless you are accessing them by a pixel shader. But since you didn't post any, my 
				//first assumption would be this.
				assert(this->m_pCurrentUniformData->m_iLocation != -1);
			}
			else
			COMPARE_NAME_WITH_DEFINE(DATA_TYPE)
			{
				COMPARE_VALUE_WITH_DEFINE(UNIFORM_DATA_TYPE_cMatrix44)
				{
					m_pCurrentUniformData->m_eDatatype = SAMPLE_FLOAT_MAT4;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(UNIFORM_DATA_TYPE_FLOAT1)
				{
					m_pCurrentUniformData->m_eDatatype = SAMPLE_FLOAT;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(UNIFORM_DATA_TYPE_FLOAT2)
				{
					m_pCurrentUniformData->m_eDatatype = SAMPLE_FLOAT_VEC2;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(UNIFORM_DATA_TYPE_FLOAT3)
				{
					m_pCurrentUniformData->m_eDatatype = SAMPLE_FLOAT_VEC3;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(UNIFORM_DATA_TYPE_FLOAT4)
				{
					m_pCurrentUniformData->m_eDatatype = SAMPLE_FLOAT_VEC4;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(UNIFORM_DATA_TYPE_INT)
				{
					m_pCurrentUniformData->m_eDatatype = SAMPLE_INT;
				}
				else
				{
					UT::ErrorMsg((WCHAR*)l_strValue,L"there is no such type");
				}
			}
			else
			COMPARE_NAME("Data")
			{
				switch(m_pCurrentUniformData->m_eDatatype)
				{
					case SAMPLE_FLOAT:
						this->m_pCurrentUniformData->m_fData[0] = (float)_wtof(l_strValue);
						glUniform1f( m_pCurrentUniformData->m_iLocation, this->m_pCurrentUniformData->m_fData[0] );
						break;
					case SAMPLE_FLOAT_VEC2:
					{
						Vector2	l_v2 = GetVector2((char*)l_strValue);
						memcpy(this->m_pCurrentUniformData->m_fData,&l_v2,sizeof(float)*2);
						glUniform2fv( m_pCurrentUniformData->m_iLocation,1, &this->m_pCurrentUniformData->m_fData[0] );
					}
						break;
					case SAMPLE_FLOAT_VEC3:
					{
						Vector3	l_v3 = GetVector3((char*)l_strValue);
						memcpy(this->m_pCurrentUniformData->m_fData,&l_v3,sizeof(float)*3);
						glUniform3fv( m_pCurrentUniformData->m_iLocation,1, &this->m_pCurrentUniformData->m_fData[0] );
					}
						break;
					case SAMPLE_FLOAT_VEC4:
					{
						Vector4	l_v4 = GetVector4((char*)l_strValue);
						memcpy(this->m_pCurrentUniformData->m_fData,&l_v4,sizeof(float)*4);
						glUniform4fv( m_pCurrentUniformData->m_iLocation,1, &this->m_pCurrentUniformData->m_fData[0] );
					}
						break;
					case SAMPLE_INT:
						this->m_pCurrentUniformData->m_iData[0] = _wtoi(l_strValue);
						glUniform1i( m_pCurrentUniformData->m_iLocation, this->m_pCurrentUniformData->m_iData[0] );
						break;
					case SAMPLE_FLOAT_MAT4:
					{
						cMatrix44	l_mat = GetMatrix((char*)l_strValue);
						memcpy(this->m_pCurrentUniformData->m_fData,&l_mat,sizeof(float)*16);
						glUniformMatrix4fv( m_pCurrentUniformData->m_iLocation, 1, GL_FALSE, &m_pCurrentUniformData->m_fData[0] );
					}
						break;
				}
			}
		PARSE_NAME_VALUE_END
	}

	void	cUniformManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		if( !e_pTiXmlElement->m_bDone )
		{
			cUniformData *l_pNewUniform = new cUniformData;
			memset( l_pNewUniform, 0, sizeof(cUniformData) );
			m_pCurrentUniformData = l_pNewUniform;
			const WCHAR*	l_strName = e_pTiXmlElement->Value();
			COMPARE_NAME("GLSL")
			{
				ProcessGLSLData();
			}
			else
			COMPARE_NAME("Texture")
			{
				ProcessTextureData();
			}
			else
			COMPARE_NAME("Attribute")
			{
				ProcessAttributeData();
			}
			else
			COMPARE_NAME("Uniform")
			{
				ProcessUniformData();
			}
			else
			{
				assert(0);
			}
			assert( m_uiRecentProgramHandle != 0);
			if( m_pCurrentUniformData )
			{
				assert(m_pCurrentUniformData->m_strName);
				m_pCurrentGLSLProgram->AddUniform(m_pCurrentUniformData);
			}
		}
		else
		{
			const WCHAR*	l_strName = e_pTiXmlElement->Value();
			COMPARE_NAME("GLSL")
			{
				glLinkProgram( m_uiRecentProgramHandle );
				CheckProgram(m_uiRecentProgramHandle,GL_LINK_STATUS,L"link");
				m_pCurrentGLSLProgram->UpdateAllUniforms();
				this->AddObject(m_pCurrentGLSLProgram);
				m_uiRecentProgramHandle = 0;
			}
		}
	}
	//=================================================================================================================================
	///
	/// Loads all the shader objects.
	///
	/// \param fileName is the name for the file where we get objects
	/// \param stateHandle is the ES handle to different types of shaders
	///
	/// \return bool saying whether we passed or failed
	//=================================================================================================================================
	bool LoadShaderObject( const char* e_strFileName, GLuint e_uiShaderHandle )
	{
		char* source = NULL;

		{
		  // Use file io to load the code of the shader.
		  std::ifstream fp( e_strFileName , ios_base::binary );
		  if( fp.fail() ) 
		  {
	#ifdef WIN32
			 cGameApp::OutputDebugInfoString(L"Failed to open shader file:");
			 cGameApp::OutputDebugInfoString(UT::CharToWchar(e_strFileName));
			 cGameApp::OutputDebugInfoString(L"\n");
	#endif
			 assert(0);
			 return false;
		  }

		  fp.seekg( 0, std::ios_base::end );
		  const long len = fp.tellg();
		  fp.seekg( 0, std::ios_base::beg );
		  // The +1 is optional, depending on how you call glShaderSourceARB
		  source = new char[len+1];
		  fp.read(source, sizeof(char)*len);
		  source[len] = 0;
		}

		const char* gls[1] = { source };

		// Pass our sources to OpenGL. Our sources are NULL terminated, so pass NULL for the lenghts.
		glShaderSource( e_uiShaderHandle, 1, gls, NULL );
		// OpenGL made a copy. Do not need the source anymore.
		delete[] source;
		source = 0;

		// Compile that one object.
		glCompileShader(e_uiShaderHandle);

		// Check for compilation success
		GLint compilationResult = 0;
		glGetShaderiv( e_uiShaderHandle, GL_COMPILE_STATUS, &compilationResult );
		CheckShader(e_uiShaderHandle, GL_COMPILE_STATUS,L"Compile\n");
	   // current implementation always succeeds.
	   // The error will happen at link time.
	   if ( compilationResult == 0 )
	   {
	#ifdef WIN32
		  cGameApp::OutputDebugInfoString(L"Failed to compile shader:");
		  cGameApp::OutputDebugInfoString(UT::CharToWchar(e_strFileName));
		  cGameApp::OutputDebugInfoString(L"\n");
	#endif
		  return false;
	   }
	   return true;
	}

	cUniformData*	cGLSLProgram::GetUniform(char*e_strName)
	{
	   for ( cUniformDataIterator index = m_Uniforms.begin(); index != m_Uniforms.end() ; ++index)
	   {
		  cUniformData* current = *index;
		  if ( strcmp( e_strName, current->m_strName ) == 0 )
			  return current;
	   }
	   return 0;
	}
	//=================================================================================================================================
	///
	/// Updates the value of a uniform
	///
	/// \param name - The name we gave to the uniform
	/// \param vals - An array of values we want to to replace the current uniform values with
	///
	/// \return true=pass ... false=fail
	//=================================================================================================================================
	bool cGLSLProgram::UpdateOneUniform( const char* name, float* vals )
	{
	   bool updated = false;
	   cUniformDataIterator index;

	   for ( index = m_Uniforms.begin(); index != m_Uniforms.end() ; ++index)
	   {
		  cUniformData* current = *index;

		  if ( strcmp( name, current->m_strName ) == 0 )
		  {
			 updated = true;

			 switch( current->m_eDatatype )
			 {
			 case SAMPLE_FLOAT:
				memcpy( current->m_fData, vals, sizeof(float) * 1 );
				glUniform1f( current->m_iLocation, current->m_fData[0] );
				break;
			 case SAMPLE_FLOAT_VEC2:
				memcpy( current->m_fData, vals, sizeof(float) * 2 );
				glUniform2f( current->m_iLocation, current->m_fData[0], current->m_fData[1] );
				break;
			 case SAMPLE_FLOAT_VEC3:
				memcpy( current->m_fData, vals, sizeof(float) * 3 );
				glUniform3f( current->m_iLocation, current->m_fData[0], current->m_fData[1], current->m_fData[2] );
				break;
			 case SAMPLE_FLOAT_VEC4:
				memcpy( current->m_fData, vals, sizeof(float) * 4 );
				glUniform4f( current->m_iLocation, current->m_fData[0], current->m_fData[1], current->m_fData[2], current->m_fData[3] );
				break;
			 case SAMPLE_FLOAT_MAT4:
				memcpy( current->m_fData, vals, sizeof(float) * 16 );
				glUniformMatrix4fv( current->m_iLocation, 1, GL_FALSE, &current->m_fData[0] );
				break;
			 default:
				assert(0);
				break;
			 }
		  }

		  if ( updated )
		  {
			 break;
		  }
	   }
	   
	   assert( updated == true ); // They probably passed in un unsupported type or invalid name
	   return updated;
	}

	//=================================================================================================================================
	///
	/// Binds a texture in GL
	///
	/// \param name - The name we gave to the texture
	/// \param width - The width of the texture
	/// \param height - The height of the texture
	///
	/// \return true=pass ... false=fail
	//=================================================================================================================================
	bool cGLSLProgram::BindTexture( const char* name, int width, int height )
	{
	   bool updated = false;
	   cUniformDataIterator index;

	   for ( index = m_Uniforms.begin(); index != m_Uniforms.end() ; ++index)
	   {
		  cUniformData* current = *index;

		  if ( strcmp( name, current->m_strName ) == 0 )
		  {
			 if ( current->m_eDatatype == SAMPLE_INT && current->m_uiTextureHandle )
			 {
				updated = true;
			 }
			 break;
		  }
	   }
	   
	   if ( updated )
	   {
		  GLint l_iUniformLocation;
		  l_iUniformLocation = glGetUniformLocation( m_uiProgramHandle, (*index)->m_strName );
		  glUniform1i( l_iUniformLocation, (*index)->m_uiTextureUnit );
		  glActiveTexture( GL_TEXTURE0 + (*index)->m_uiTextureUnit );
		  glBindTexture( GL_TEXTURE_2D, (*index)->m_uiTextureHandle );

		  if ((*index)->m_iWidth != width || (*index)->m_iHeight != height )
		  {
			 if ( width != 0 && height != 0 ) // If these are 0 then we just want to keep the current size
			 {
				(*index)->m_iWidth = width;
				(*index)->m_iHeight = height;
				if ( (*index)->m_bDepthTex )
				{
				   glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
				}
				else
				{
				   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
				}
			 }
		  }
	   }
	   else
	   {
		  assert( 0 ); // They probably passed in an a non-texture type or an invalid name
	   }
	   return updated;
	}

	//=================================================================================================================================
	///
	/// Binds an FBO in GL
	///
	/// \param name - The name of the fbo
	/// \param width - The width of the fbo
	/// \param height - The height of the fbo
	///
	/// \return true=pass ... false=fail
	//=================================================================================================================================
	bool cGLSLProgram::BindFbo( const char* e_strName, int e_iWidth, int e_iHeight )
	{
	   bool updated = false;
	   cUniformDataIterator index;

	   for ( index = m_Uniforms.begin(); index != m_Uniforms.end() ; ++index)
	   {
		  cUniformData* current = *index;

		  if ( strcmp( e_strName, current->m_strName ) == 0 )
		  {
			 if ( current->m_eDatatype == SAMPLE_INT && current->m_uiTextureHandle && current->m_uiFBOHandle )
			 {
				updated = true;
			 }
			 break;
		  }
	   }
	   
	   if ( updated )
	   {
		  glBindFramebuffer( GL_FRAMEBUFFER, (*index)->m_uiFBOHandle );

		  if ((*index)->m_iWidth != e_iWidth || (*index)->m_iHeight != e_iHeight )
		  {
			 GLint curTexture;
			 glGetIntegerv( GL_TEXTURE_BINDING_2D, &curTexture );

			 (*index)->m_iWidth = e_iWidth;
			 (*index)->m_iHeight = e_iHeight;
			 glBindTexture( GL_TEXTURE_2D, (*index)->m_uiTextureHandle );
			 if ( (*index)->m_bDepthTex )
			 {
				glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, e_iWidth, e_iHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
			 }
			 else
			 {
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, e_iWidth, e_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
			 }

			 glBindTexture( GL_TEXTURE_2D, curTexture );

			 if ( (*index)->m_bDepthTex )
			 {
				if(!(*index)->m_uiColorRbHandle)
				{
				   glGenRenderbuffers( 1, &(*index)->m_uiColorRbHandle );
				}
				glBindRenderbuffer( GL_RENDERBUFFER, (*index)->m_uiColorRbHandle );
				glRenderbufferStorage( GL_RENDERBUFFER, GL_RGB, e_iWidth, e_iHeight );
				glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, (*index)->m_uiColorRbHandle );
				glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, (*index)->m_uiTextureHandle, 0 );
			 }
			 else
			 {
				glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (*index)->m_uiTextureHandle, 0 );
			 }
		  }
	   }
	   else
	   {
		  assert( 0 ); // They probably passed in an a non-texture type or an invalid name
	   }
	   return updated;
	}


	//=================================================================================================================================
	///
	/// Deletes all the GL resources that we have loaded
	///
	/// \param name - The name we gave to the program
	///
	/// \return true=pass ... false=fail
	//=================================================================================================================================
	void cGLSLProgram::FreeAllData()
	{
		cUniformDataIterator index;
		for ( index = m_Uniforms.begin(); index != m_Uniforms.end() ; ++index)
		{
		  if ( (*index)->m_uiTextureHandle )
		  {
			 glDeleteTextures(1,&((*index)->m_uiTextureHandle)); 
		  }
		  if ( (*index)->m_uiVertShaderHandle )
		  {
			 glDeleteShader( (*index)->m_uiVertShaderHandle );
		  }
		  if ( (*index)->m_uiFragShaderHandle )
		  {
			 glDeleteShader( (*index)->m_uiFragShaderHandle );
		  }
		  if ( (*index)->m_uiFBOHandle )
		  {
			 glDeleteFramebuffers(1, &(*index)->m_uiFBOHandle );
		  }
		  if ( (*index)->m_uiColorRbHandle )
		  {
			 glDeleteRenderbuffers(1, &(*index)->m_uiColorRbHandle );
		  }
		}

		for ( UINT i = 0; i < m_Uniforms.size() ; i++ )
		{
		  if ( m_Uniforms[i] != NULL )
		  {
			 delete m_Uniforms[i];
		  }
		}
		m_Uniforms.clear();
		glDeleteProgram( m_uiProgramHandle );
	}

	//=================================================================================================================================
	///
	/// Updates all the uniform data after a link
	///
	/// \param void
	///
	/// \return void
	//=================================================================================================================================
	void cGLSLProgram::UpdateAllUniforms()
	{
	   cUniformDataIterator index;

	   for ( index = m_Uniforms.begin(); index != m_Uniforms.end() ; ++index)
	   {
		 //GLenum	l_en = (*index)->m_eDatatype;
		 switch( (*index)->m_eDatatype )
		 {
		 case SAMPLE_FLOAT:
			glUniform1f( (*index)->m_iLocation, (*index)->m_fData[0] );
			break;
		 case SAMPLE_FLOAT_VEC2:
			glUniform2f( (*index)->m_iLocation, (*index)->m_fData[0], (*index)->m_fData[1] );
			break;
		 case SAMPLE_FLOAT_VEC3:
			glUniform3f( (*index)->m_iLocation, (*index)->m_fData[0], (*index)->m_fData[1], (*index)->m_fData[2] );
			break;
		 case SAMPLE_FLOAT_VEC4:
			glUniform4f( (*index)->m_iLocation, (*index)->m_fData[0], (*index)->m_fData[1], (*index)->m_fData[2], (*index)->m_fData[3] );
			break;
		 case SAMPLE_FLOAT_MAT4:
			glUniformMatrix4fv( (*index)->m_iLocation, 1, GL_FALSE, &(*index)->m_fData[0] );
			break;
		 case SAMPLE_INT:
			if ( (*index)->m_iLocation != 0 ) // Prevents error on nullTexture which has not yet been given a location (done at bind time)
			{
			   glUniform1i( (*index)->m_iLocation, (*index)->m_uiTextureUnit );
			}
			break;
		 case SAMPLE_PROGRAM:
			break;
		 default:
			assert(0);
			break;
		 }
	   }
	}

	void	cGLSLProgram::AddAttributeData(std::string e_strName,int e_iLocation)
	{
		AddAttributeData(e_strName.c_str(),e_iLocation);
	}

	void	cGLSLProgram::AddAttributeData(const char*e_strName,int e_iLocation)
	{
		this->m_AttributeDataVector.insert(std::make_pair(e_iLocation,std::string(e_strName)));
	}

	void	cGLSLProgram::UsingProgram()
	{
		if( g_pCurrentUsingGLSLProgram != this )
		{
			g_pCurrentUsingGLSLProgram = this;
			glUseProgram(this->m_uiProgramHandle);
			std::map<int,std::string>::iterator	l_iterator = m_AttributeDataVector.begin();
			for( ;l_iterator != m_AttributeDataVector.end();++l_iterator )
			{
				int	l_iIndex = l_iterator->first;
				glEnableVertexAttribArray(l_iIndex);
				//const char*l_str = l_iterator->second.c_str();
				glBindAttribLocation( m_uiProgramHandle, l_iterator->first, l_iterator->second.c_str() );
				//const char*l_str = (const char*)glGetString(glGetError());
			}
		}
	}

	void	cGLSLProgram::DisableProgram()
	{
		assert(g_pCurrentUsingGLSLProgram == this);
		glUseProgram(0);
		std::map<int,std::string>::iterator	l_iterator = m_AttributeDataVector.begin();
		for( ;l_iterator != m_AttributeDataVector.end();++l_iterator )
		{
			int	l_iIndex = l_iterator->first;
			glDisableVertexAttribArray(l_iIndex);
			//const char*l_str = (const char*)glGetString(glGetError());
		}
	}
	//========================
	//
	//========================
}