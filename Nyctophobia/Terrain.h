#pragma once

#include "Graphics.h"
#include "d3dUtil.h"

class Terrain
{
public:
	Terrain(){};
	Terrain(IDirect3DTexture9* tex, IDirect3DTexture9* blend)
	{
		mTexture = tex;
		mBlend = blend;
		mRelease = true;
	};
	~Terrain()
	{
		ReleaseCOM(mTexture);
		if(mRelease)
			ReleaseCOM(mBlend);
	}

	void setTexture(IDirect3DTexture9* tex)		{mTexture = tex;}
	void setBlendTexture(IDirect3DTexture9* tex){mBlend = tex;}

	IDirect3DTexture9* getTexture()				{return mTexture;}
	IDirect3DTexture9* getBlendTexture()		{return mBlend;}

	void releaseBlend()							{ReleaseCOM(mBlend); mRelease = false;}
private:
	IDirect3DTexture9* mTexture;
	IDirect3DTexture9* mBlend;

	//
	bool mRelease;
};