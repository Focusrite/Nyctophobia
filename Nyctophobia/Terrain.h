#pragma once

#include "Graphics.h"
#include "d3dUtil.h"

class Terrain
{
public:
	Terrain(){};
	Terrain(IDirect3DTexture9* tex, IDirect3DTexture9* blend);
	~Terrain()
	{
		ReleaseCOM(mTexture);
		ReleaseCOM(mBlend);
	}

	void setTexture(IDirect3DTexture9* tex)		{mTexture = tex;}
	void setBlendTexture(IDirect3DTexture9* tex){mBlend = tex;}

	IDirect3DTexture9* getTexture()				{return mTexture;}
	IDirect3DTexture9* getBlendTexture()		{return mBlend;}
private:
	IDirect3DTexture9* mTexture;
	IDirect3DTexture9* mBlend;


};