#ifndef VERTEX_OBJECT_BUFFER
#define VERTEX_OBJECT_BUFFER

#include "../GLSL/Shader.h"
namespace FATMING_CORE
{
	//==================================
	//ensure u have setup data order as 0,position 1,normal 2,color 3,tangent 4,bitangent 5texteure
	//==================================
	class	cVBOBuffer
	{
		GLuint	m_uiAllBufferID[TOTAL_FVF];
		int		m_iAllBufferStride[TOTAL_FVF];
		GLuint	m_uiDrawIndicesBufferID;
		UINT	m_uiIndicesCount;//for draw indices
	public:
		cVBOBuffer();
		virtual ~cVBOBuffer();
		//enusre the count is calculate by float
		void	SetupVerticesBuffer(float*e_pfData,int e_iDataLocation,int e_iStride,UINT e_uiCount,int e_iDataSize = 4);//sizeof(float) = 4
		void	SetupIndicesBuffer(float*e_pfData,int e_iStride,UINT e_uiCount);
		void	Render();
		void	RenderByShader();
		UINT	GetIndicexCount(){ return m_uiIndicesCount; }
		float*	GetData(int e_iDataLocation);
	};
	//check HGLRC	InitOpenGL(HWND e_pHwnd,bool e_bInitGlewInit)
	extern bool g_bVBOSupported;
}
#endif