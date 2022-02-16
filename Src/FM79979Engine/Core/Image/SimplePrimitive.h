#ifndef SimplePrimitive_H
#define SimplePrimitive_H
#include "glLoadImage.h"
using namespace FATMING_CORE;
namespace FATMING_CORE
{
	//from 2DShader.cpp
	extern GLuint	g_uiAttribArray[9];//TOTAL_FVF
}

static float		g_f2DTextureOriginalCoordinateBuffer[8] = { 0,0,1,0,0,1,1,1 };//texture coordinate
static float		g_f2DTextureOriginalCoordinateMirrorBuffer[8] = {1,0,0,0,1,1,0,1};
static float		g_f2DColorOne[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
static float		g_f2DTextureCoordinate[8] = {1,1,1,1,1,1,1,1};
static Vector3		g_v2DVertexBuffer[4]={ Vector3::Zero,Vector3::Zero,Vector3::Zero,Vector3::Zero };
static Vector4		g_f2DColor[4]={Vector4::Zero,Vector4::Zero,Vector4::Zero,Vector4::Zero};
static cMatrix44	g_mat2DTransform = cMatrix44::Identity;
//for MPDI
static float		g_fMPDIOptmizeRenderVertices[64*2*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];//0.75k
static float		g_fMPDIOptmizeRenderUV[64*2*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];//0.75k
static float		g_fMPDIOptmizeRenderColor[64*4*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];//1.5k

#ifdef OPENGLES_2_X
#define	ASSIGN_2D_COLOR(Color)	memcpy(g_f2DColorOne,Color,sizeof(float)*4);					\
								memcpy(&g_f2DColorOne[4],Color,sizeof(float)*4);				\
								memcpy(&g_f2DColorOne[8],Color,sizeof(float)*4);				\
								memcpy(&g_f2DColorOne[12],Color,sizeof(float)*4);				\
								glVertexAttribPointer( g_uiAttribArray[FVF_DIFFUSE], 4, GL_FLOAT, 0, 0, g_f2DColorOne );

#define	ASSIGN_2D_VerticesBySize(Width,Height,Depth)															\
	g_v2DVertexBuffer[0].x = (float)-Width;	g_v2DVertexBuffer[0].y = (float)-Height;							\
	g_v2DVertexBuffer[1].x = (float)Width;	g_v2DVertexBuffer[1].y = (float)-Height;							\
	g_v2DVertexBuffer[2].x = (float)-Width;	g_v2DVertexBuffer[2].y = (float)Height;								\
	g_v2DVertexBuffer[3].x = (float)Width;	g_v2DVertexBuffer[3].y = (float)Height;								\
	g_v2DVertexBuffer[0].z = g_v2DVertexBuffer[1].z = g_v2DVertexBuffer[2].z = g_v2DVertexBuffer[3].z = Depth;	\
	glVertexAttribPointer( g_uiAttribArray[FVF_POS], 3, GL_FLOAT, 0, 0, g_v2DVertexBuffer );

#define	ASSIGN_2D_UV(UV)													\
	g_f2DTextureCoordinate[0] = UV[0]; g_f2DTextureCoordinate[1] = UV[1];	\
	g_f2DTextureCoordinate[2] = UV[2]; g_f2DTextureCoordinate[3] = UV[1];	\
	g_f2DTextureCoordinate[4] = UV[0]; g_f2DTextureCoordinate[5] = UV[3];	\
	g_f2DTextureCoordinate[6] = UV[2]; g_f2DTextureCoordinate[7] = UV[3];	\
	glVertexAttribPointer( g_uiAttribArray[FVF_TEX0], 2, GL_FLOAT, 0, 0, g_f2DTextureCoordinate );

#define	ASSIGN_2D_MIRROR_UV(UV)												\
	g_f2DTextureCoordinate[0] = UV[2]; g_f2DTextureCoordinate[1] = UV[1];	\
	g_f2DTextureCoordinate[2] = UV[0]; g_f2DTextureCoordinate[3] = UV[1];	\
	g_f2DTextureCoordinate[4] = UV[2]; g_f2DTextureCoordinate[5] = UV[3];	\
	g_f2DTextureCoordinate[6] = UV[0]; g_f2DTextureCoordinate[7] = UV[3];	\
	glVertexAttribPointer( g_uiAttribArray[FVF_TEX0], 2, GL_FLOAT, 0, 0, g_f2DTextureCoordinate );


#define	myGlVertexPointer(Stride,pData)glVertexAttribPointer(g_uiAttribArray[FVF_POS],Stride, GL_FLOAT,0, 0, pData);
#define	myGlUVPointer(Stride,pData)	glVertexAttribPointer(g_uiAttribArray[FVF_TEX0],Stride, GL_FLOAT,0, 0, pData);
#define	myGlColorPointer(Stride,pData)	glVertexAttribPointer(g_uiAttribArray[FVF_DIFFUSE],Stride, GL_FLOAT,0, 0, pData);
#define	myGlNormalPointer(Stride,pData)	glVertexAttribPointer(g_uiAttribArray[FVF_NORMAL],Stride, GL_FLOAT,0, 0, pData);

#else
#define	ASSIGN_2D_COLOR(Color)	memcpy(g_f2DColorOne,Color,sizeof(float)*4);					\
								memcpy(&g_f2DColorOne[4],Color,sizeof(float)*4);				\
								memcpy(&g_f2DColorOne[8],Color,sizeof(float)*4);				\
								memcpy(&g_f2DColorOne[12],Color,sizeof(float)*4);				\
								glColorPointer( 4, GL_FLOAT, 0, g_f2DColorOne );
#define	ASSIGN_2D_VerticesBySize(Width,Height,Depth)															\
	g_v2DVertexBuffer[0].x = (float)-Width;	g_v2DVertexBuffer[0].y = (float)-Height;							\
	g_v2DVertexBuffer[1].x = (float)Width;	g_v2DVertexBuffer[1].y = (float)-Height;							\
	g_v2DVertexBuffer[2].x = (float)-Width;	g_v2DVertexBuffer[2].y = (float)Height;								\
	g_v2DVertexBuffer[3].x = (float)Width;	g_v2DVertexBuffer[3].y = (float)Height;								\
	g_v2DVertexBuffer[0].z = g_v2DVertexBuffer[1].z = g_v2DVertexBuffer[2].z = g_v2DVertexBuffer[3].z = Depth;	\
	glVertexPointer( 3, GL_FLOAT, 0, g_v2DVertexBuffer );

#define	ASSIGN_2D_UV(UV)													\
	g_f2DTextureCoordinate[0] = UV[0]; g_f2DTextureCoordinate[1] = UV[1];	\
	g_f2DTextureCoordinate[2] = UV[2]; g_f2DTextureCoordinate[3] = UV[1];	\
	g_f2DTextureCoordinate[4] = UV[0]; g_f2DTextureCoordinate[5] = UV[3];	\
	g_f2DTextureCoordinate[6] = UV[2]; g_f2DTextureCoordinate[7] = UV[3];	\
	glTexCoordPointer(2, GL_FLOAT, 0, g_f2DTextureCoordinate );

#define	ASSIGN_2D_MIRROR_UV(UV)												\
	g_f2DTextureCoordinate[0] = UV[2]; g_f2DTextureCoordinate[1] = UV[1];	\
	g_f2DTextureCoordinate[2] = UV[0]; g_f2DTextureCoordinate[3] = UV[1];	\
	g_f2DTextureCoordinate[4] = UV[2]; g_f2DTextureCoordinate[5] = UV[3];	\
	g_f2DTextureCoordinate[6] = UV[0]; g_f2DTextureCoordinate[7] = UV[3];	\
	glTexCoordPointer(2, GL_FLOAT, 0, g_f2DTextureCoordinate );

#define	myGlVertexPointer(Stride,pData)glVertexPointer		(Stride,	GL_FLOAT,0, pData);
#define	myGlUVPointer(Stride,pData)	glTexCoordPointer		(Stride,	GL_FLOAT,0, pData);
#define	myGlColorPointer(Stride,pData)	glColorPointer		(Stride,	GL_FLOAT,0, pData);
#define	myGlNormalPointer(Stride,pData)glNormalPointer		(GL_FLOAT,	Stride,    pData);
#endif

namespace GLRender
{
	//before call any 2d image function must betwnne glEnable2D glDisable2D,if GLSL here need a Matrix to setup ViewProjection matrix,fixed pipeline won't return any value.
	//2D y axis down is positive up is negative.
	void glEnable2D(float e_fResolutionWidth,float e_fResolutionHeight,float*e_pfMatrix = 0,bool e_bInvertYAxis = false);
	void glDisable2D();
	//
	void	DrawQuadWithTextureAndColorAndCoordinate(GLfloat e_iX,GLfloat e_iY,GLfloat e_fDepth,GLint e_iWidth,GLint e_iHeight,Vector4 e_vColor,float *e_pfTexCoordinate,float e_fRotationAngle = 0.f);
	void	DrawMirrorQuadWithTextureAndColorAndCoordinate(GLfloat e_iX,GLfloat e_iY,GLfloat e_fDepth,GLint e_iWidth,GLint e_iHeight,Vector4 e_vColor,float *e_pfTexCoordinate,float e_fRotationAngle = 0.f);
    //current it only works for 2D(for MPDI)
	void    DrawQuadWithMatrix(float*e_pfVertices,float*e_pfTextureUV,Vector4 e_vColor,float*e_pfMatrix);
	//
	void    GetDrawQuadFVFDataByTriangleStrip(GLint e_iWidth,GLint e_iHeight,float *e_pfTexCoordinate,Vector4 e_vColor,float*e_pfVertices,float*e_pfUV,float*e_pColor,cMatrix44*e_pmat);
	void    GetDrawQuadFVFDataByTwoTriangles(GLint e_iWidth,GLint e_iHeight,float *e_pfTexCoordinate,Vector4 e_vColor,float*e_pfVertices,float*e_pfUV,float*e_pColor,cMatrix44*e_pmat);
	//2point,P for start position,D for destination,
	//Vector3	l_vPoint;
	//l_vPoint.x = 0;
	//l_vPoint.y = 0;
	//l_vPoint.z = 0;
	//Vector3	l_vDirection;
	//l_vDirection.x = 1920;
	//l_vDirection.y = 1080;
	//l_vDirection.z = 0;
	//RenderArrow(l_vPoint,l_vDirection,30,0xffffffff);
	//u have to disable client state or it may occor crash
	void	RenderArrow(Vector3 P,Vector3 D, float radius,Vector4 e_vColor);
	void	Render3DArrow(Vector3 P,Vector3 D,Vector4 e_vColor = Vector4::One,float e_fWidth = 8);
	void	RenderPoint(Vector2	e_vPos,float e_fSize = 15,Vector4 e_vColor = Vector4::One);
	void	RenderPoint(Vector3	e_vPos,float e_fSize = 15,Vector4 e_vColor = Vector4::One);
	void	RenderPoints(Vector3*e_vPos,int e_iNum,float e_fSize = 15,Vector4 e_vColor = Vector4::One,float*e_pfMatrix = cMatrix44::Identity);
	void	RenderPoints(Vector2*e_vPos,int e_iNum,float e_fSize = 15,Vector4 e_vColor = Vector4::One,float*e_pfMatrix = cMatrix44::Identity);
	void	RenderLine(float*e_pPosition,int e_iNum,Vector4 e_vColor,int e_iStride,float*e_pfMatrix = cMatrix44::Identity,float e_fLineSize = 2);
	void	RenderLine(std::vector<Vector2>* e_pPositionPoint,Vector4 e_vColor,float*e_pfMatrix = cMatrix44::Identity,bool e_bRenderPoints = false);
	void	RenderLine(std::vector<Vector3>* e_pPositionPoint,Vector4 e_vColor,float*e_pfMatrix = cMatrix44::Identity,bool e_bRenderPoints = false);
	//e_fDensityAngle get smaller get smooth
	//u have to multiply matrx as u want
	//bad performance,here should instead by a circle class
	void	RenderCircle(Vector2 e_vPos,float e_fRadius,Vector4 e_vColor = Vector4(0.f,1.f,0.f,1.f),bool e_bDrawPoint = false,float e_fDensityAngle = 5.f);
	void	RenderCircle(cMatrix44 e_mat,float e_fRadius,Vector4 e_vColor = Vector4(0.f,1.f,0.f,1.f),bool e_bDrawPoint = false,float e_fDensityAngle = 5.f);
	//draw by center orientation.
	//a 50,50 RECT left up is -25,-25
	void	RenderRectangle(POINT e_Pos,int e_iWidth,int e_iHeight,Vector4 e_vColor,float e_fAngle = 0.f,float e_fLineWidth = 3);
	void	RenderRectangle(Vector2 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,float e_fAngle,float e_fLineWidth = 3);
	void	RenderRectangle(Vector2 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,Vector3 e_vAngle = Vector3::Zero,float e_fLineWidth = 3);
	void	RenderRectangle(Vector3 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,Vector3 e_vAngle = Vector3::Zero,float e_fLineWidth = 3);
	void	RenderRectangle(float e_fWidth,float e_fHeight,cMatrix44 e_Mat,Vector4 e_vColor,float e_fLineWidth = 3);
	void	RenderFilledRectangle(Vector2 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,float e_fAngle);

	void	DrawCube(Vector3 e_vSize,cMatrix44 e_mat,Vector4 e_vColor);
	void	DrawPlane(cMatrix44 e_mat,float	e_fWidth = 100,float	e_fHeight = 100,float e_fGridWidthDistance = 10,float e_fGridHeightDistance = 10,float e_fLineWidth = 7.f,Vector4 e_vColor = Vector4(1,1,1,1));
	//
	void	DrawVectorProject(Vector3 e_v1,Vector3 e_v2,cMatrix44 e_mat = cMatrix44::TranslationMatrix(Vector3(300,-300,0)));
	//frome UV(left up right buttom) to 2 triangles vertex
	void	AssignUVDataTo2Triangles(float*e_pSrc,float*e_pDest,bool e_bMirror);
	//frome UV(left up right buttom) to  triangle strip vertex
	void	AssignUVDataToTriangleStrip(float*e_pSrc,float*e_pDest,bool e_bMirror);
	void	Assign4VerticesDataTo2Triangles(float*e_pSrc,float*e_pDest,int e_iStrip);
};
using namespace GLRender;
//end namespace GLRender
#endif