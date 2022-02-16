#ifndef _SHADER_H_
#define _SHADER_H_

//Flexible Vertex Format Constants, or FVF codes,
//are used to describe the contents of vertices interleaved in a single data
//stream that will be processed by the fixed-function pipeline.
#define	FVF_POS					0//xyz  3*4byte
#define	FVF_NORMAL				1//xyz  3*4byte
#define	FVF_DIFFUSE				2//RGBA 4*4byte
#define	FVF_TAGENT				3//xyz  3*4byte
#define	FVF_BITAGENT			4//xyz  3*4byte
#define	FVF_SKINNING_WEIGHT		5//4 inlunce bones 4*4byte
#define	FVF_SKINNING_BONE_INDEX	6//the data for bone id usually is 4 unsigned byte to become 1 float.if the bone ID is bigger than 512,u have to ensure data swap problem
								 //1*4 byte one float
#define	FVF_TEX0				7//UV	2*4byte
#define	FVF_TEX1				8//UV	2*4byte
//#define	FVF_TEX2				9//UV	2*4byte
//#define	FVF_TEX3				10//UV	2*4byte
//#define	FVF_TEX4				11//UV	2*4byte
//#define	FVF_TEX5				12//UV	2*4byte
//#define	FVF_TEX6				13//UV	2*4byte
//#define	FVF_TEX7				14//UV	2*4byte

//last one
#define	TOTAL_FVF				9

extern char*	g_strShaderAttribution[TOTAL_FVF];
extern int		g_iFVF_DataStride[TOTAL_FVF];
extern GLenum	g_iFVF_DataType[TOTAL_FVF];
extern GLenum	g_iDrawindiceType;

namespace FATMING_CORE
{
	//for shader data
	extern bool	g_bCommonVSClientState[TOTAL_FVF];
	extern bool	g_bCommonVSNoTextureClientState[TOTAL_FVF];
	extern bool	g_bMyMeshVSClientState[TOTAL_FVF];
	extern bool	g_bMySkinningMeshVSClientState[TOTAL_FVF];
	extern char*g_strCommonVS;
	extern char*g_strCommonFS;
	extern char*g_strCommonVSNoTexture;
	extern char*g_strCommonFSNoTexture;
	extern char*g_strMyMeshVS;
	extern char*g_strMyMeshFS;
	extern char*g_strMySkinningMeshVS;
	extern char*g_strMySkinningMeshFS;

	extern WCHAR*DEFAULT_SHADER;
	extern WCHAR*NO_TEXTURE_SHADER;
	extern WCHAR*STATIC_MESH_SHADER;
	extern WCHAR*SKINNING_MESH_SHADER;

	class	cBaseShader:public NamedTypedObject
	{
		GLuint	m_uiVS;
		GLuint	m_uiFS;
		GLuint	m_uiProgram;
		bool	CreateVS(char*e_strVS);
		bool	CreateFS(char*e_strPS);
		bool	CreateProgram(char*e_strVS,char*e_strPS,bool e_bTexture);
		bool	m_bTexture;
	public:
		GLuint	m_uiMatrixVPLoacation;
		GLuint	m_uiMatrixWLoacation;
		GLuint	m_uiTexLoacation;
		GLuint	m_uiAttribArray[TOTAL_FVF];
		//for non texture,directly setup pixel shader's color
		GLuint	m_uiColorLoacation;
		//for skinning
		GLuint	m_uiBonesLocation;
		//if e_bTexture is true it texture coordinate attribute and texture UV will be assign.
		cBaseShader(WCHAR*e_strName,bool e_bTexture = true);
		cBaseShader(char*e_strVS,char*e_strPS,WCHAR*e_strName,bool e_bTexture = true);
		//for opengl es1
		cBaseShader(WCHAR*e_strName,bool *e_pbClientState);
		virtual	NamedTypedObject*	Clone(){ return 0; }
		virtual ~cBaseShader();
		void	Use(bool e_bUseLastWVPMatrix = true);
		void	Disable();
		GLuint	GetUniFormLocationByName(const char*e_strName);
	};

	//e_pbClientState to setup opengl es client state
	cBaseShader*	CreateShader(bool *e_pbClientState,WCHAR*e_strName = DEFAULT_SHADER);
	//e_pbClientState to setup opengl es client state
	cBaseShader*	CreateShader(bool *e_pbClientState,char*e_strVS,char*e_strPS,WCHAR*e_strName = DEFAULT_SHADER);
	cBaseShader*	GetShader(WCHAR*e_strName);
	cBaseShader*	GetCurrentShader();
	//
	bool	DeleteShader(WCHAR*e_strName = DEFAULT_SHADER);
	//only call once before draw any 2D image
	//if e_bUseLastWVPMatrix is true it will use the WVP matrix last has been called
	void	UseShaderProgram(const WCHAR*e_strName = DEFAULT_SHADER,bool e_bUseLastWVPMatrix = true);
	void	UseShaderProgram(cBaseShader*e_p2DShader);
	//only call once if u wanna to disable it.
	void	DisableShaderProgram(WCHAR*e_strName = DEFAULT_SHADER);
	//void	SetupWorldViewProjectionMatrix(float*e_pfWVPMatrix);
	//while shader is applied,not set value by camera,ignore rotation,because it has beenn rotated
	void	SetupShaderViewProjectionMatrix(float*e_pfVPMatrix,bool e_bOrientation);
	float*	GetCurrentViewProjectionMatrix();
	//void	SetupPojectionMatrix(float*e_pfPMatrix);
	//void	SetupViewMatrix(float*e_pfVMatrix);
	void	SetupShaderWorldMatrix(float*e_pfWMatrix);
	//ensure current program is not support texture
	void	SetupShaderColor(Vector4 e_vcolor);
	//ensure current program is not support texture
	void	SetupShaderBonesData(cMatrix44*e_pBoneMatrices,int e_iCount);
	bool CheckProgram(int Object, int Type,WCHAR*e_strMessage);
	//CheckShader(e_uiShaderHandle, GL_COMPILE_STATUS,L"Compile\n");
	bool CheckShader(int Object, int Type, WCHAR*e_strMessage);
#ifndef DEBUG
#define	MY_GLDRAW_ARRAYS	glDrawArrays
#define	MY_GLDRAW_ELEMENTS	glDrawElements
#else
	void	MY_GLDRAW_ARRAYS(GLenum mode, GLint first, GLsizei count);
	void	MY_GLDRAW_ELEMENTS(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
#endif
	
}

#endif//end _2DSHADER_H