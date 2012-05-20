#include <tchar.h>

#include "Graphics.h"
#include "Vertex.h"
#include "Vector.h"
#include "cPolygon.h"
#include "Triangulate.h"
#include "Defines.h"
#include "Effects.h"
#include "Camera.h"

#include "DrawableTex2D.h"

/*
http://www.flipcode.com/archives/Efficient_Polygon_Triangulation.shtml read!
*/

//! Constructor that sets up the neccessary things.
/**
Inits the vertex declarations, builds the vertex buffers, loads the fonts, creates the sprite, checks caps.
*/
Graphics::Graphics()
{
	// Makes all vertexes ready to use.
	InitAllVertexDeclarations();

	mVB_texture = NULL;
	mVB_Rect	= NULL;
	mVB_blend	= NULL;
	//mVB_triangle= NULL;

	// Maximum is 16 vertices.
	if(FAILED(gd3dDevice->CreateVertexBuffer(sizeof(TextureVertex)*16, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &mVB_texture, NULL)))
		MessageBox(0, "Error creating TextureVertex buffer", 0, 0);

	if(FAILED(gd3dDevice->CreateVertexBuffer(sizeof(RectVertex) *32, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &mVB_Rect, NULL)))
		MessageBox(0, "Error creating RectVertex buffer", 0, 0);

	if(FAILED(gd3dDevice->CreateVertexBuffer(sizeof(BlendVertex) *16, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &mVB_blend, NULL)))
		MessageBox(0, "Error creating BlendVertex buffer", 0, 0);

	if(FAILED(gd3dDevice->CreateVertexBuffer(sizeof(BlendTexVertex) *16, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &mVB_blendTex, NULL)))
		MessageBox(0, "Error creating BlendTexVertex buffer", 0, 0);

	mCam = new Camera(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,SCREEN_WIDTH,SCREEN_HEIGHT);
	mCam->buildProjectionMatrix();
	mCam->buildViewMatrix();
	mCam->buildViewport();

	mFX = new Effects();

	mAlphaTexture = new DrawableTex2D(SCREEN_WIDTH,SCREEN_HEIGHT, 0, D3DFMT_A8R8G8B8, true, D3DFMT_D24S8, true);
	HR(D3DXCreateTextureFromFile(gd3dDevice, "Data\\Images\\Alphacircle.png", &mCircle));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "Data\\Images\\Alphasquare.png", &mSquare));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "Data\\Images\\Shadowcircle.png", &mShadow));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "Data\\Images\\test.png", &mTest));
	mStencilCircle = loadTexture("Data\\Images\\Alphacircle.png", D3DCOLOR_XRGB(0,0,0));
	// Load the font.
	loadDxFont();

	// Create the sprite handler.
	HRESULT result = D3DXCreateSprite(gd3dDevice, &mSpriteHandler);

	buildFastBuffer();

	//mCustomFont = new Font();
	HR(gd3dDevice->GetDeviceCaps(&mCaps));
	checkDeviceCaps();
}

//! Destructor that for cleanin up.
/**
Releases the vertex buffers and the fonts.
*/
Graphics::~Graphics()
{
	delete mCam;
	delete mFX;
	delete mAlphaTexture;

	ReleaseCOM(mTest);

	ReleaseCOM(mCircle);
	ReleaseCOM(mStencilCircle);
	ReleaseCOM(mShadow);
	ReleaseCOM(mVB_texture);
	ReleaseCOM(mVB_Rect);
	ReleaseCOM(mVB_fast);
	ReleaseCOM(mVB_blend);
	ReleaseCOM(mVB_blendTex);
	ReleaseCOM(mFont);
	ReleaseCOM(mBigFont);
	//delete mCustomFont;
}

//! Loads and returns a texture from the specified source.
/**
Tries create a texture with D3DXCreateTextureFromFileEx(). Prompts a MessageBox if it fails.
@param filename The source file.
@param colorKey The color that represent transparency.
@return The created texture.
*/
IDirect3DTexture9* Graphics::loadTexture(string filename, DWORD colorKey)
{
	IDirect3DTexture9 *texture;		// Texture to return.
	D3DXIMAGE_INFO SrcInfo;			// Optional.

	// Load texture from file image.
	if (FAILED(D3DXCreateTextureFromFileEx(gd3dDevice, filename.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, 
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
        colorKey, &SrcInfo, NULL, &texture)))
	{
		char buffer[256];
		sprintf(buffer, "Error loading texture: %s", filename);
		MessageBox(0, buffer, 0, 0);
		return NULL;
	}

	return texture;
}

//! Builds the static vertex buffer.
/**
@note The vertex buffer is 100x100 and should get scaled, rotated and transformed before drawTexture() is called.
*/
void Graphics::buildFastBuffer()
{
	HR(gd3dDevice->CreateVertexBuffer(4 * sizeof(TextureVertex), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &mVB_fast, 0));

	TextureVertex* v = NULL;
	HR(mVB_fast->Lock(0, 0, (void**)&v, 0));

	int width = 100;
	int height = 100;

	v[0] = TextureVertex(-width/2, -height/2, 0);
	v[1] = TextureVertex(-width/2, height/2, 0);
	v[2] = TextureVertex(width/2, height/2, 0);
	v[3] = TextureVertex(width/2, -height/2, 0);
	v[0].tex0.x = 0;
	v[0].tex0.y = 0;
	v[1].tex0.x = 0;
	v[1].tex0.y = 1;
	v[2].tex0.x = 1;
	v[2].tex0.y = 1;
	v[3].tex0.x = 1;
	v[3].tex0.y = 0;

	HR(mVB_fast->Unlock());
}

//! Loads the ID3DXFont member.
void Graphics::loadDxFont(void)
{
	D3DXFONT_DESC fontDesc;
	fontDesc.Height          = 18;
    fontDesc.Width           = 8;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = 10;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
	 _tcscpy(fontDesc.FaceName, _T("Bitstream Vera Sans Mono"));
    //_tcscpy(fontDesc.FaceName, _T("Arial")); Bitstream Vera Sans Mono

	HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &mFont));

	fontDesc.Width = 13;
	fontDesc.Height = 26;

	HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &mBigFont));
}

//! Checks device caps.
/**
@return False if lack of vertex or pixel shader support.
*/
bool Graphics::checkDeviceCaps()
{
	D3DCAPS9 caps;
	HR(gd3dDevice->GetDeviceCaps(&caps));

	// Check for vertex shader support.
	if(caps.VertexShaderVersion < D3DVS_VERSION(2, 0))
		return false;
	
	// Check pixel shader support.
	if(caps.PixelShaderVersion < D3DPS_VERSION(2, 0))
		return false;

	return true;
}


//! Draw text with either DX font or cusom font.
/**
@param text The text to be drawn.
@param x The X coordinate.
@param y The Y coordinate.
@param size The dimension. Width = height = size.
@param color Text color.
*/
void Graphics::drawText(string text, int x, int y, TextType type, int size, DWORD color)
{
	if(type == CUSTOM)
		drawCustomFont(text, x, y, size, color);
	else if(type == SMALL_DX)
		drawSmallDxFont(text, x, y, color);
	else
		drawBigDxFont(text, x, y, color);
}

//! Draws a custom font.
/**
@param text The text to be drawn.
@param x The X coordinate.
@param y The Y coordinate.
@param size The dimension. Width = height = size.
@param color Text color.
*/
void Graphics::drawCustomFont(string text, int x, int y, int size, DWORD color)
{
	//mCustomFont->draw(text, x, y, size, color);
}

//! Draws text using ID3DXFont.
/**
@param text The text to be drawn.
@param x The X coordinate.
@param y The Y coordinate.
@textColor The text color.
*/
void Graphics::drawSmallDxFont(string text, int x, int y, D3DCOLOR textColor)
{
	HR(gd3dDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE));
	RECT pos = {x, y, 0, 0};
	HR(mFont->DrawText(0, text.c_str(), -1, &pos, DT_NOCLIP, textColor));
	HR(gd3dDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE));
}


//! Draws text using ID3DXFont.
/**
@param text The text to be drawn.
@param x The X coordinate.
@param y The Y coordinate.
@textColor The text color.
*/
void Graphics::drawBigDxFont(string text, int x, int y, D3DCOLOR textColor)
{
	HR(gd3dDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE));
	RECT pos = {x, y, 0, 0};
	HR(mBigFont->DrawText(0, text.c_str(), -1, &pos, DT_NOCLIP, textColor));
	HR(gd3dDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE));
}

//! Returns the sprite handler.
/**
@note Used to draw objects with screen coordinates.
@return Pointer to the sprite handler.
*/
LPD3DXSPRITE Graphics::getSpriteHandler()
{
	return mSpriteHandler;
}

//! Draws a textured polygon, supports animation.
/**
@param shape The shape to be drawn.
@param texture The texture to apply to the shape.
@param srcRect Part of the texture to draw.
@param flipped True if the texture should be flipped.
*/
bool Graphics::drawTexturedPolygon(cPolygon shape, IDirect3DTexture9 *texture, Rect* srcRect, bool flipped)
{
	// Can't go outside buffer size
	if(shape.mVertices.size() > 16)
		return false;

	// Set vertex declaration and stream source
	gd3dDevice->SetVertexDeclaration(TextureVertex::Decl);
	gd3dDevice->SetStreamSource(0, mVB_texture, 0, sizeof(TextureVertex));	

	Rect sourceRect;
	if(srcRect != NULL)
		 sourceRect = *srcRect;

	// Normalize to image coordinates
	if(srcRect != NULL)	{
		D3DXVECTOR3 center;
		D3DSURFACE_DESC desc;
		texture->GetLevelDesc(NULL, &desc);
		sourceRect.left /= desc.Width;
		sourceRect.right /= desc.Width;
		sourceRect.top /= desc.Height;
		sourceRect.bottom /= desc.Height;
	}
	else	{
		sourceRect.left = 0;
		sourceRect.right = 1;
		sourceRect.top = 0;
		sourceRect.bottom = 1;
	}

	// Should the image be horizontally flipped?
	if(flipped)	{
		if(sourceRect.left != 0)
			sourceRect.bottom = sourceRect.bottom;

		float tmp = sourceRect.left;
		sourceRect.left = sourceRect.right;
		sourceRect.right = tmp;
	}

	// Lock and set the vertices position and texture coordinates
	TextureVertex *vertices = 0;
	mVB_texture->Lock(0, 0, (void**)&vertices, 0);

	vertices[0].pos.x = shape.mVertices[0].x + shape.getPos().x;
	vertices[0].pos.y = shape.mVertices[0].y + shape.getPos().y;
	vertices[0].pos.z = 0.0f;
	vertices[0].tex0.x = sourceRect.left;
	vertices[0].tex0.y = sourceRect.top;

	vertices[1].pos.x = shape.mVertices[1].x + shape.getPos().x;
	vertices[1].pos.y = shape.mVertices[1].y + shape.getPos().y;
	vertices[1].pos.z = 0.0f;
	vertices[1].tex0.x = sourceRect.left;
	vertices[1].tex0.y = sourceRect.bottom;

	vertices[2].pos.x = shape.mVertices[2].x + shape.getPos().x;
	vertices[2].pos.y = shape.mVertices[2].y + shape.getPos().y;
	vertices[2].pos.z = 0.0f;
	vertices[2].tex0.x = sourceRect.right;
	vertices[2].tex0.y = sourceRect.bottom;

	vertices[3].pos.x = shape.mVertices[3].x + shape.getPos().x;
	vertices[3].pos.y = shape.mVertices[3].y + shape.getPos().y;
	vertices[3].pos.z = 0.0f;
	vertices[3].tex0.x = sourceRect.right;
	vertices[3].tex0.y = sourceRect.top;

	// Unlock the vertex buffer
	mVB_texture->Unlock();

	// Set texture
	gd3dDevice->SetTexture (0, texture);

	// Draw image
	gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	gd3dDevice->SetTexture (0, NULL);
}

//! Draws a solid or wireframed rectangle.
/**
@param x The X coordinate.
@param y The Y coordinate.
@param width The width.
@param height The height.
@param solid Draw the rect solid or not.
@note If solid is false then the rect gets drawn in wireframe mode.
*/
void Graphics::drawRect(float x, float y, int width, int height, D3DCOLOR fillColor, bool solid, bool blend, float z)
{  
	Rect drawRect;
	drawRect.left = x-(width/2);
	drawRect.right = x+(width/2);
	drawRect.top = y-(height/2);
	drawRect.bottom = y+(height/2);

	Graphics::drawRect(drawRect, fillColor, solid, blend, z);
}

//! Draws a solid or wireframed rectangle.
/**
@param rect The Rect that should be drawn.
@param solid Draw the rect solid or not.
@param fillColor The color to draw the rect in.
@note If solid is false then the rect gets drawn in wireframe mode.
*/
void Graphics::drawRect(Rect rect, D3DCOLOR fillColor, bool solid, bool blend, float z)
{
	// Setup the vertexbuffer and its declaration
	gd3dDevice->SetStreamSource(0, mVB_blend, 0, sizeof(BlendVertex));
	gd3dDevice->SetVertexDeclaration(BlendVertex::Decl);

	mAlphaRect = rect;

	D3DSURFACE_DESC desc;
	mAlphaTexture->d3dTex()->GetLevelDesc(NULL, &desc);
	mAlphaRect.left /= desc.Width;
	mAlphaRect.right /= desc.Width;
	mAlphaRect.top /= desc.Height;
	mAlphaRect.bottom /= desc.Height;

	mFX->getFX()->SetTechnique(mFX->mhBlendTech);

	if(blend)
		mFX->setBlendMap(mAlphaTexture->d3dTex());
	else
		mFX->setBlendMap(mSquare);

	BlendVertex *vertices = 0;

	HR(mVB_blend->Lock(0, 0, (void**)&vertices, 0));

	vertices[0].color = fillColor;
	vertices[0].pos.x = (float) rect.left;
	vertices[0].pos.y = (float) rect.top;
	vertices[0].pos.z = z;
	vertices[0].tex0.x = mAlphaRect.left;
	vertices[0].tex0.y = mAlphaRect.top;

	vertices[1].color = fillColor;
	vertices[1].pos.x = (float) rect.right;
	vertices[1].pos.y = (float) rect.top;
	vertices[1].pos.z = z;
	vertices[1].tex0.x = mAlphaRect.right;
	vertices[1].tex0.y = mAlphaRect.top;

	vertices[2].color = fillColor;
	vertices[2].pos.x = (float) rect.right;
	vertices[2].pos.y = (float) rect.bottom;
	vertices[2].pos.z = z;
	vertices[2].tex0.x = mAlphaRect.right;
	vertices[2].tex0.y = mAlphaRect.bottom;

	vertices[3].color = fillColor;
	vertices[3].pos.x = (float) rect.left;
	vertices[3].pos.y = (float) rect.bottom;
	vertices[3].pos.z = z;
	vertices[3].tex0.x = mAlphaRect.left;
	vertices[3].tex0.y = mAlphaRect.bottom;

	// Unlock the vertex buffer
	HR(mVB_blend->Unlock());

	// Draw content in buffer
	if(solid)
		gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	else
		gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		  

	UINT numPasses = 0;
	HR(mFX->getFX()->Begin(&numPasses,0));
	for(int i = 0; i < numPasses; ++i)
	{
		HR(mFX->getFX()->BeginPass(i));
		gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		HR(mFX->getFX()->EndPass());
	}
	HR(mFX->getFX()->End());

	gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//Reset
	mFX->setBlendMap(0);
}

//! Draws a textured rectangle.
/**
@param texture The texture to draw.
@param rect The rect to draw.
@param rotation Rotation of the rect.
@note The rotation is measured in radians.
*/
void Graphics::drawTexture(IDirect3DTexture9 *texture, Rect rect, float rotation, bool stenciling)
{
	// Transform to x, y, width and height representation.
	float width = rect.right - rect.left;
	float height = rect.bottom - rect.top;
	float x = rect.left + width/2;
	float y = rect.top + height/2;
	
	// Call the other drawTexture() function.
	drawTexture(texture, x, y, width, height, rotation, stenciling);
}

//! Draws a textured rectangle.
/**
@param texture The texture to draw.
@param x The X coordinate.
@param y The Y coordinate.
@param width The width.
@param height The height.
@param rotation Rotation of the texture.
@param sourceRect The part of the texture to draw.
@param flipped Draw the texture flipped or not.
@note The rotation is measured in radians.
@note The source rect is in image coordinates. Not in 0-1 range.
*/
void Graphics::drawTexture(IDirect3DTexture9 *texture, float x, float y, int width, int height, float rotation, bool stenciled, Rect* sourceRect)
{
	gd3dDevice->SetStreamSource(0, mVB_blendTex, 0, sizeof(BlendTexVertex));
	gd3dDevice->SetVertexDeclaration(BlendTexVertex::Decl);

	if(stenciled) 
	{
		HR(mFX->getFX()->SetTechnique(mFX->mhBlendStencilTech));
	}
	else 
	{
		HR(mFX->getFX()->SetTechnique(mFX->mhBlendTexTech));
	}

	mFX->setBlendMap(mSquare);
	mFX->setTexture(texture);
	mFX->setRotation(rotation);

	mDrawRect.left = x-(width/2);
	mDrawRect.right = x+(width/2);
	mDrawRect.top = y-(height/2);
	mDrawRect.bottom = y+(height/2);

	BlendTexVertex *vertices = 0;

	HR(mVB_blendTex->Lock(0, 0, (void**)&vertices, 0));

	vertices[0].pos.x = (float) mDrawRect.left;
	vertices[0].pos.y = (float) mDrawRect.top;
	vertices[0].pos.z = 0;
	vertices[0].tex0.x = 0;
	vertices[0].tex0.y = 0;
	vertices[0].tex1.x = 0;
	vertices[0].tex1.y = 0;

	vertices[1].pos.x = (float) mDrawRect.right;
	vertices[1].pos.y = (float) mDrawRect.top;
	vertices[1].pos.z = 0;
	vertices[1].tex0.x = 1;
	vertices[1].tex0.y = 0;
	vertices[1].tex1.x = 1;
	vertices[1].tex1.y = 0;

	vertices[2].pos.x = (float) mDrawRect.right;
	vertices[2].pos.y = (float) mDrawRect.bottom;
	vertices[2].pos.z = 0;
	vertices[2].tex0.x = 1;
	vertices[2].tex0.y = 1;
	vertices[2].tex1.x = 1;
	vertices[2].tex1.y = 1;

	vertices[3].pos.x = (float) mDrawRect.left;
	vertices[3].pos.y = (float) mDrawRect.bottom;
	vertices[3].pos.z = 0;
	vertices[3].tex0.x = 0;
	vertices[3].tex0.y = 1;
	vertices[3].tex1.x = 0;
	vertices[3].tex1.y = 1;

	// Unlock the vertex buffer
	HR(mVB_blendTex->Unlock());

	// Draw content in buffer
	UINT numPasses = 0;
	HR(mFX->getFX()->Begin(&numPasses,0));
	for(int i = 0; i < numPasses; ++i)
	{
		HR(mFX->getFX()->BeginPass(i));
		gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);	
		HR(mFX->getFX()->EndPass());
	}
	HR(mFX->getFX()->End());

	//Reset
	mFX->setBlendMap(0);
	mFX->setTexture(0);
	mFX->setRotation(0);
}

//! Draws a part from a texture atlas.
/**
@param texture The texture to draw.
@param x The X coordinate.
@param y The Y coordinate.
@param width The width.
@param height The height.
@param srcRect Part of the texture to draw.
@param flipped Draw the texture flipped or not.
@note sourceRect is in image pixels.
*/
void Graphics::drawAnimation(IDirect3DTexture9* texture, float x, float y, int width, int height, Rect* srcRect, bool flipped)
{
	// Rect size as it weill be drawn on screen
	Rect drawRect;
	drawRect.left = x - width/2;
	drawRect.right = x + width/2;
	drawRect.top = y - height/2;
	drawRect.bottom = y + height/2;
	
	// Normalize to image coordinates
	if(srcRect != NULL)	{
		D3DXVECTOR3 center;
		D3DSURFACE_DESC desc;
		texture->GetLevelDesc(NULL, &desc);
		srcRect->left /= desc.Width;
		srcRect->right /= desc.Width;
		srcRect->top /= desc.Height;
		srcRect->bottom /= desc.Height;
	}
	else	{
		srcRect = new Rect();
		srcRect->left = 0;
		srcRect->right = 1;
		srcRect->top = 0;
		srcRect->bottom = 1;
	}

	// Flip texture?
	if(flipped)	{
		int tmp = srcRect->left;
		srcRect->left = srcRect->right;
		srcRect->right = tmp;
	}

	gd3dDevice->SetVertexDeclaration(TextureVertex::Decl);
	gd3dDevice->SetStreamSource(0, mVB_texture, 0, sizeof(TextureVertex));	

	TextureVertex *vertices = 0;
	mVB_texture->Lock(0, 0, (void**)&vertices, 0);

	vertices[0].pos.x = (float) drawRect.left;
	vertices[0].pos.y = (float) drawRect.top;
	vertices[0].pos.z = 0;
	vertices[0].tex0.x = srcRect->left;
	vertices[0].tex0.y = srcRect->top;

	vertices[1].pos.x = (float) drawRect.right;
	vertices[1].pos.y = (float) drawRect.top;
	vertices[1].pos.z = 0;
	vertices[1].tex0.x = srcRect->right;
	vertices[1].tex0.y = srcRect->top;

	vertices[2].pos.x = (float) drawRect.right;
	vertices[2].pos.y = (float) drawRect.bottom;
	vertices[2].pos.z = 0;
	vertices[2].tex0.x = srcRect->right;
	vertices[2].tex0.y = srcRect->bottom;

	vertices[3].pos.x = (float) drawRect.left;
	vertices[3].pos.y = (float) drawRect.bottom;
	vertices[3].pos.z = 0;
	vertices[3].tex0.x = srcRect->left;
	vertices[3].tex0.y = srcRect->bottom;

	// Unlock the vertex buffer
	mVB_texture->Unlock();

	// Set texture
	gd3dDevice->SetTexture (0, texture);

	// Draw content in buffer
	gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	gd3dDevice->SetTexture (0, NULL);
}

//! Draws a thin line.
/**
@param start The start point.
@param end The end point.
@param color Color to draw the line in.
*/
void Graphics::drawLine(Vector start, Vector end, float thickness, D3DCOLOR color, bool blend)
{
	if(start.x > end.x) {
		float t = start.x;
		start.x = end.x;
		end.x = t;
	}
	if(start.y > end.y) {
		float t = start.y;
		start.y = end.y;
		end.y = t;
	}
	// Setup the vertexbuffer and its declaration
	gd3dDevice->SetStreamSource(0, mVB_blend, 0, sizeof(BlendVertex));
	gd3dDevice->SetVertexDeclaration(BlendVertex::Decl);	 

	BlendVertex *vertices = 0;

	mFX->getFX()->SetTechnique(mFX->mhBlendTech);

	if(blend)
		mFX->setBlendMap(mAlphaTexture->d3dTex());
	else
		mFX->setBlendMap(mSquare);

	Vector diff = start - end;
	float angle = atan2f(diff.y, diff.x);
	angle -= PI/2;

	mDrawRect.left = start.x;
	mDrawRect.right = end.x;
	mDrawRect.top = start.y;
	mDrawRect.bottom = end.y;

	mAlphaRect = mDrawRect;

	D3DSURFACE_DESC desc;
	mAlphaTexture->d3dTex()->GetLevelDesc(NULL, &desc);
	mAlphaRect.left /= desc.Width;
	mAlphaRect.right /= desc.Width;
	mAlphaRect.top /= desc.Height;
	mAlphaRect.bottom /= desc.Height;

	HR(mVB_blend->Lock(0, 0, (void**)&vertices, 0));

	vertices[0].color = color;
	vertices[0].pos.x = start.x;
	vertices[0].pos.y = start.y;
	vertices[0].pos.z = 0.0f;
	vertices[0].tex0.x = start.x / desc.Width;
	vertices[0].tex0.y = start.y / desc.Height;

	vertices[1].color = color;
	vertices[1].pos.x = start.x - cosf(angle) * thickness;
	vertices[1].pos.y = start.y - sinf(angle) * thickness;
	vertices[1].pos.z = 0.0f; 
	vertices[1].tex0.x = (start.x - cosf(angle) * thickness) / desc.Width; //right
	vertices[1].tex0.y = (start.y - sinf(angle) * thickness) / desc.Height; //top

	vertices[2].color = color;
	vertices[2].pos.x = end.x - cosf(angle) * thickness;
	vertices[2].pos.y = end.y - sinf(angle) * thickness;
	vertices[2].pos.z = 0.0f; 
	vertices[2].tex0.x = (end.x - cosf(angle) * thickness) / desc.Width; //right
	vertices[2].tex0.y = (end.y - sinf(angle) * thickness) / desc.Height; //bottom

	vertices[3].color = color;
	vertices[3].pos.x = end.x;
	vertices[3].pos.y = end.y;
	vertices[3].pos.z = 0.0f; 
	vertices[3].tex0.x = end.x / desc.Width;
	vertices[3].tex0.y = end.y / desc.Height;

	// Unlock the vertex buffer
	HR(mVB_blend->Unlock());

	// Draw content in buffer
	gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
	UINT numPasses = 0;
	HR(mFX->getFX()->Begin(&numPasses,0));
	for(int i = 0; i < numPasses; ++i)
	{
		HR(mFX->getFX()->BeginPass(i));
		gd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		HR(mFX->getFX()->EndPass());
	}
	HR(mFX->getFX()->End());

	//Reset
	mFX->setBlendMap(0);
}

void Graphics::drawTexturedLine(IDirect3DTexture9* texture, Vector start, Vector end, float thickness)
{
	// Setup the vertexbuffer and its declaration
	gd3dDevice->SetStreamSource(0, mVB_texture, 0, sizeof(TextureVertex));
	gd3dDevice->SetVertexDeclaration(TextureVertex::Decl);	  

	TextureVertex* vertices = 0;

	HR(mVB_texture->Lock(0, 0, (void**)&vertices, 0));

	Vector diff = start - end;
	float angle = atan2f(diff.y, diff.x);
	angle -= 3.14/2;

	vertices[0].pos.x = start.x + cosf(angle) * thickness/2;
	vertices[0].pos.y = start.y + sinf(angle) * thickness/2;
	vertices[0].pos.z = 0;
	vertices[0].tex0.x = 0;
	vertices[0].tex0.y = 0;

	vertices[1].pos.x = start.x - cosf(angle) * thickness/2;
	vertices[1].pos.y = start.y - sinf(angle) * thickness/2;
	vertices[1].pos.z = 0; 
	vertices[1].tex0.x = 0;
	vertices[1].tex0.y = 1;

	vertices[2].pos.x = end.x + cosf(angle) * thickness/2;
	vertices[2].pos.y = end.y + sinf(angle) * thickness/2;
	vertices[2].pos.z = 0; 
	vertices[2].tex0.x = 1;
	vertices[2].tex0.y = 1;

	vertices[3].pos.x = end.x - cosf(angle) * thickness/2;
	vertices[3].pos.y = end.y - sinf(angle) * thickness/2;
	vertices[3].pos.z = 0; 
	vertices[3].tex0.x = 1;
	vertices[3].tex0.y = 0;

	// Unlock the vertex buffer
	HR(mVB_texture->Unlock());

	// Set texture
	gd3dDevice->SetTexture (0, texture);

	// Draw content in buffer
	//gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		
	gd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void Graphics::onLostDevice()
{
	mFont->OnLostDevice();
	mBigFont->OnLostDevice();
	mSpriteHandler->OnLostDevice();
	mAlphaTexture->onLostDevice();
	//mCustomFont->onLostDevice();
}

void Graphics::onResetDevice()
{
	mFont->OnResetDevice();
	mBigFont->OnResetDevice();
	mSpriteHandler->OnResetDevice();
	mAlphaTexture->onResetDevice();
	//mCustomFont->onResetDevice();
}

void Graphics::drawPolygon(cPolygon* shape, D3DCOLOR color, bool filled, float z)
{
	if(shape->requireTriangulation())
		shape->triangulate();
	if(shape->mVertices.size() > 32)
		return;

	// Set vertex declaration and stream source
	gd3dDevice->SetVertexDeclaration(RectVertex::Decl);
	gd3dDevice->SetStreamSource(0, mVB_Rect, 0, sizeof(RectVertex));

	HR(mFX->getFX()->SetTechnique(mFX->mhShadowTech));

	// Lock and set the vertices position and texture coordinates
	RectVertex *vertices = 0;
	mVB_Rect->Lock(0, 0, (void**)&vertices, 0);

	for(int i = 0; i<shape->getTriangles().size()/3; i++) 
	{
		vertices[i*3+0].color = color;
		vertices[i*3+0].pos.x = shape->getTriangles()[i*3+0].x;// + shape->getPos().x;
		vertices[i*3+0].pos.y = shape->getTriangles()[i*3+0].y;// + shape->getPos().y;
		vertices[i*3+0].pos.z = z;

		vertices[i*3+1].color = color;
		vertices[i*3+1].pos.x = shape->getTriangles()[i*3+1].x;// + shape->getPos().x;
		vertices[i*3+1].pos.y = shape->getTriangles()[i*3+1].y;// + shape->getPos().y;
		vertices[i*3+1].pos.z = z;

		vertices[i*3+2].color = color;
		vertices[i*3+2].pos.x = shape->getTriangles()[i*3+2].x;// + shape->getPos().x;
		vertices[i*3+2].pos.y = shape->getTriangles()[i*3+2].y;// + shape->getPos().y;
		vertices[i*3+2].pos.z = z;
	}
	// Unlock the vertex buffer
	mVB_Rect->Unlock();

	if(filled)
		gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	else
		gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	UINT numPasses = 0;
	HR(mFX->getFX()->Begin(&numPasses,0));
	for(int i = 0; i < numPasses; ++i)
	{
		HR(mFX->getFX()->BeginPass(i));
		// Draw image
		gd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, shape->getTriangles().size()/3);
		HR(mFX->getFX()->EndPass());
	}
	HR(mFX->getFX()->End());

	//Reset
	//mFX->setBlendMap(0);
}

void Graphics::drawCircle(Vector position, float radius, D3DCOLOR color, bool filled, bool smudged)
{
	// Setup the vertexbuffer and its declaration
	gd3dDevice->SetStreamSource(0, mVB_blend, 0, sizeof(BlendVertex));
	gd3dDevice->SetVertexDeclaration(BlendVertex::Decl);	  

	HR(mFX->getFX()->SetTechnique(mFX->mhBlendTech));

	float rgb[] = {1,0,0};

	if(smudged) 
	{
		mFX->setBlendMap(mShadow);
		mFX->setRotation(0 - (int) PI/10 * sinf(2*PI*(float)(gMath->random(1,100)/100)));
		mFX->setShadowColorMod(rgb);
	}
	else
		mFX->setBlendMap(mCircle);

	mDrawRect.left = position.x-(radius/2);
	mDrawRect.right = position.x+(radius/2);
	mDrawRect.top = position.y-(radius/2);
	mDrawRect.bottom = position.y+(radius/2);

	BlendVertex *vertices = 0;

	HR(mVB_blend->Lock(0, 0, (void**)&vertices, 0));

	vertices[0].color = color;
	vertices[0].pos.x = (float) mDrawRect.left;
	vertices[0].pos.y = (float) mDrawRect.top;
	vertices[0].pos.z = 0;
	vertices[0].tex0.x = 0;
	vertices[0].tex0.y = 0;

	vertices[1].color = color;
	vertices[1].pos.x = (float) mDrawRect.right;
	vertices[1].pos.y = (float) mDrawRect.top;
	vertices[1].pos.z = 0;
	vertices[1].tex0.x = 0;
	vertices[1].tex0.y = 1;

	vertices[2].color = color;
	vertices[2].pos.x = (float) mDrawRect.right;
	vertices[2].pos.y = (float) mDrawRect.bottom;
	vertices[2].pos.z = 0;
	vertices[2].tex0.x = 1;
	vertices[2].tex0.y = 1;

	vertices[3].color = color;
	vertices[3].pos.x = (float) mDrawRect.left;
	vertices[3].pos.y = (float) mDrawRect.bottom;
	vertices[3].pos.z = 0;
	vertices[3].tex0.x = 1;
	vertices[3].tex0.y = 0;

	// Unlock the vertex buffer
	HR(mVB_blend->Unlock());

	// Draw content in buffer
	UINT numPasses = 0;
	HR(mFX->getFX()->Begin(&numPasses,0));
	for(int i = 0; i < numPasses; ++i)
	{
		HR(mFX->getFX()->BeginPass(i));
		gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);	
		HR(mFX->getFX()->EndPass());
	}
	HR(mFX->getFX()->End());

	//Reset
	mFX->setRotation(0);
	mFX->setBlendMap(0);
	mFX->resetShadowColorMod();
}

void Graphics::drawBlendedTexture(IDirect3DTexture9* texture, float x, float y, int width, int height, float rotation, Rect* sourceRect, IDirect3DTexture9* blend, int repeats)
{
	// Setup the vertexbuffer and its declaration
	gd3dDevice->SetStreamSource(0, mVB_blendTex, 0, sizeof(BlendTexVertex));
	gd3dDevice->SetVertexDeclaration(BlendTexVertex::Decl);

	HR(mFX->getFX()->SetTechnique(mFX->mhBlendTexTech));

	if(blend == NULL)
		mFX->setBlendMap(mAlphaTexture->d3dTex());
	else
		mFX->setBlendMap(blend);
	mFX->setTexture(texture);
	mFX->setRotation(rotation);

	mDrawRect.left = x-(width/2);
	mDrawRect.right = x+(width/2);
	mDrawRect.top = y-(height/2);
	mDrawRect.bottom = y+(height/2);

	mAlphaRect = mDrawRect;

	D3DSURFACE_DESC desc;
	mAlphaTexture->d3dTex()->GetLevelDesc(NULL, &desc);
	mAlphaRect.left /= desc.Width;
	mAlphaRect.right /= desc.Width;
	mAlphaRect.top /= desc.Height;
	mAlphaRect.bottom /= desc.Height;

	BlendTexVertex *vertices = 0;

	HR(mVB_blendTex->Lock(0, 0, (void**)&vertices, 0));

	vertices[0].pos.x = mDrawRect.left;//mDrawRect.left * cos(rotation) - mDrawRect.top * sin(rotation);
	vertices[0].pos.y = mDrawRect.top;//mDrawRect.left * sin(rotation) + mDrawRect.top * cos(rotation);
	vertices[0].pos.z = 0;
	vertices[0].tex0.x = 0;
	vertices[0].tex0.y = 0;
	vertices[0].tex1.x = mAlphaRect.left;
	vertices[0].tex1.y = mAlphaRect.top;

	vertices[1].pos.x = mDrawRect.right;//mDrawRect.right * cos(rotation) - mDrawRect.top * sin(rotation);
	vertices[1].pos.y = mDrawRect.top;//mDrawRect.right * sin(rotation) + mDrawRect.top * cos(rotation);
	vertices[1].pos.z = 0;
	vertices[1].tex0.x = 1*repeats;
	vertices[1].tex0.y = 0;
	vertices[1].tex1.x = mAlphaRect.right;
	vertices[1].tex1.y = mAlphaRect.top;

	vertices[2].pos.x = mDrawRect.right;//mDrawRect.right * cos(rotation) - mDrawRect.bottom * sin(rotation);
	vertices[2].pos.y = mDrawRect.bottom;//mDrawRect.right * sin(rotation) + mDrawRect.bottom * cos(rotation);
	vertices[2].pos.z = 0;
	vertices[2].tex0.x = 1*repeats;
	vertices[2].tex0.y = 1*repeats;
	vertices[2].tex1.x = mAlphaRect.right;
	vertices[2].tex1.y = mAlphaRect.bottom;

	vertices[3].pos.x = mDrawRect.left;//mDrawRect.left * cos(rotation) - mDrawRect.bottom * sin(rotation);
	vertices[3].pos.y = mDrawRect.bottom;//mDrawRect.left * sin(rotation) + mDrawRect.bottom * cos(rotation);
	vertices[3].pos.z = 0;
	vertices[3].tex0.x = 0;
	vertices[3].tex0.y = 1*repeats;
	vertices[3].tex1.x = mAlphaRect.left;
	vertices[3].tex1.y = mAlphaRect.bottom;

	// Unlock the vertex buffer
	HR(mVB_blendTex->Unlock());

	// Draw content in buffer
	UINT numPasses = 0;
	HR(mFX->getFX()->Begin(&numPasses,0));
	for(int i = 0; i < numPasses; ++i)
	{
		HR(mFX->getFX()->BeginPass(i));
		gd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);	
		HR(mFX->getFX()->EndPass());
	}
	HR(mFX->getFX()->End());

	//Reset
	mFX->setRotation(0);
	mFX->setBlendMap(0);
	mFX->setTexture(0);
}