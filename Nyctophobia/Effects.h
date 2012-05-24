#pragma once

#include "d3dUtil.h"

class Effects
{
public:
	Effects();
	~Effects();

	ID3DXEffect* getFX() {return mFX;}

	//FX Handles
	D3DXHANDLE mhShadowTech;
	D3DXHANDLE mhBlendTech;
	D3DXHANDLE mhBlendTexTech;
	D3DXHANDLE mhBlendStencilTech;

	void updateWVP();
	void setTexture(IDirect3DTexture9* texture);
	void setBlendMap(IDirect3DTexture9* texture);
	void setRotation(float v);
	void setShadowColorMod(float* mod);
	void setShadowColorMod(float r, float g, float b);
	void resetShadowColorMod();
	void setTerrainBlendMap(IDirect3DTexture9* texture);

private:
	ID3DXEffect* mFX;
	ID3DXBuffer* dbgErrors;

	D3DXHANDLE mhWVP;
	D3DXHANDLE mhTexture;
	D3DXHANDLE mhBlendMap;
	D3DXHANDLE mhTerrainBlendMap;
	D3DXHANDLE mhUseTerrainBlend;
	D3DXHANDLE mhRotation;
	D3DXHANDLE mhScreenSize;
	D3DXHANDLE mhShadowColorMod;
};