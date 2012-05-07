#include "d3dUtil.h"
#include "Effects.h"
#include "Graphics.h"
#include "Game.h"
#include "Camera.h"
#include "Defines.h"

Effects::Effects() 
{
	mFX = 0;
	mhWVP = 0;
	mhBlendMap = 0;
	mhTexture = 0;
	mhBlendTexTech = 0;
	dbgErrors = 0;
	float rgb[] = {1,1,1};

	D3DXCreateBuffer(8 *sizeof(int), &dbgErrors);

	HR(D3DXCreateEffectFromFile(gd3dDevice, "shader.fx", 0,0, D3DXSHADER_DEBUG, 0, &mFX, &dbgErrors));
	if(dbgErrors)
		MessageBox(0, (char*)dbgErrors->GetBufferPointer(),0,0);

	mhShadowTech = mFX->GetTechniqueByName("ShadowTech");
	mhBlendTech = mFX->GetTechniqueByName("BlendTech");
	mhBlendTexTech = mFX->GetTechniqueByName("BlendTexTech");
	mhBlendStencilTech = mFX->GetTechniqueByName("BlendStencilTech");

	mhWVP = mFX->GetParameterByName(0, "gWVP");
	mhBlendMap = mFX->GetParameterByName(0, "gBlendMap");
	mhTexture = mFX->GetParameterByName(0, "gTex");
	mhRotation = mFX->GetParameterByName(0, "gRotation");
	mhShadowColorMod = mFX->GetParameterByName(0, "gShadowMod");
	mFX->SetFloatArray(mhShadowColorMod, rgb, 3);

	mhTerrainBlendMap = mFX->GetParameterByName(0, "gBlendMap2");
	mhUseTerrainBlend = mFX->GetParameterByName(0, "gUseSecondBlend");
	HR(mFX->SetBool(mhUseTerrainBlend, false));
}

void Effects::setRotation(float v)
{
	HR(mFX->SetFloat(mhRotation, v));
}
void Effects::setShadowColorMod(float *mod)
{
	mFX->SetFloatArray(mhShadowColorMod, mod, 3);
}

void Effects::resetShadowColorMod()
{
	float rgb[] = {1,1,1};
	mFX->SetFloatArray(mhShadowColorMod, rgb, 3);
}

void Effects::updateWVP()
{
	D3DXMATRIX M = gGraphics->getCamera()->getView()*gGraphics->getCamera()->getProj();
	HR(mFX->SetMatrix(mhWVP, &M));
}


void Effects::setTexture(IDirect3DTexture9* texture)
{
	HR(mFX->SetTexture(mhTexture, texture));
}

void Effects::setBlendMap(IDirect3DTexture9* texture)
{
	HR(mFX->SetTexture(mhBlendMap, texture));
}

void Effects::setTerrainBlendMap(IDirect3DTexture9* texture)
{
	if(texture!=NULL) {
		HR(mFX->SetBool(mhUseTerrainBlend, true));
		HR(mFX->SetTexture(mhTerrainBlendMap, texture));
	}
	else
		HR(mFX->SetBool(mhUseTerrainBlend, false));
}

Effects::~Effects()
{
	ReleaseCOM(mFX);
	delete dbgErrors;
}