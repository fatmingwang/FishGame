#ifndef _UV_COMBINE_H_
#define _UV_COMBINE_H_

//=============================
//restriction:
//the images should from same texture or occur error
//how
//create a buffer to contain necessary vertex and UV buffer,
//assign new uv in it and setup vrtex data
//=============================

class	cImagesCombineToOneImage
{
	//
	int					m_iNumImage;
	float*				m_pfVertexBuffer;
	float*				m_pfUVBuffer;
	Vector2				m_vImageSize;
	//
	GET_SET_DEC(Vector3,m_vPos,GetPos,SetPos);
	//
public:
	cImagesCombineToOneImage();
	~cImagesCombineToOneImage();
	void	SetupData(int e_iNumImages,Vector2 e_vSize,Vector3 e_vPos);
	void	UpdateUVAndVertexBuffer(int e_iIndex,Vector2 e_vSize,Vector4 e_vUV);
	void	Init();
	void	Update(float e_fElpaseTime);
	void	Render();
	void	DebugRender();
};


#endif