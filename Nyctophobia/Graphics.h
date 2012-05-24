#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <crtdbg.h>
#include <string>
#include <list>

#include "d3dUtil.h"

#include "Rect.h"
#include "cPolygon.h"
#include "DrawableTex2D.h"


class Effects;
class Camera;

using namespace std;

//! Determines which type of text to draw.
enum TextType
{
	CUSTOM,
	SMALL_DX,
	BIG_DX
};

//! Wrapper for drawing with DirectX.
class Graphics
{
public:
	//! Constructor that sets up the neccessary members.
	Graphics();

	//! Destructor that for cleanin up.
	~Graphics();

	//! Loads and returns a texture from the specified source.
	IDirect3DTexture9* loadTexture(string filename, DWORD colorKey = 0);

	//! Loads the ID3DXFont member.
	void loadDxFont(void);

	//! Builds the static vertex buffer.
	void buildFastBuffer();

	//! Draws a solid or wireframed rectangle.
	void drawRect(float x, float y, int width, int height, D3DCOLOR fillColor = D3DCOLOR_ARGB( 255, 255 , 000, 000 ), bool solid = true, bool blend = true, float z = 0.0f);	//:TODO: Fix z and solid!

	//! Draws a solid or wireframed rectangle.
	void drawRect(Rect rect, D3DCOLOR fillColor = D3DCOLOR_ARGB( 155, 155, 000, 000 ), bool solid = true, bool blend = true, float z = 0.0f);	
	
	//! Draws a textured rectangle.
	void drawTexture(IDirect3DTexture9 *texture, Rect rect, float rotation = 0.0f, bool stenciled = false);

	//! Draws a textured rectangle.
	void drawTexture(IDirect3DTexture9 *texture, float x, float y, int width, int height, float rotation = 0.0f, bool stenciled = false, Rect* sourceRect = NULL);

	//! Draw texture blended with the world-texture
	void drawBlendedTexture(IDirect3DTexture9* texture, float x, float y, int width, int height, float rotation = 0.0f, Rect* sourceRect = NULL, IDirect3DTexture9* blend = NULL, int repeats = 1);

	//! Draws a part from a texture atlas.
	void drawAnimation(IDirect3DTexture9* texture, float x, float y, int width, int height, Rect *srcRect = NULL, bool flipped = false);

	//! Draw a polygon
	void drawPolygon(cPolygon* shape, D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 0, 0), bool filled = true, float z = 0.0f);

	//! Draws a textured polygon, supports animation.
	bool drawTexturedPolygon(cPolygon shape, IDirect3DTexture9 *texture, Rect *srcRect = NULL, bool flipped = false);

	//! Draws a thin line.
	void drawLine(Vector start, Vector end, float thickness = 20.0f, D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 0, 0), bool blend = true);

	void drawTexturedLine(IDirect3DTexture9* texture, Vector start, Vector end, float thickness = 20.0f);

	void drawCircle(Vector position, float radius, D3DCOLOR color = D3DCOLOR_XRGB(255,255,255), bool filled = true, bool smudged = false);

	//! Returns the sprite handler.
	LPD3DXSPRITE getSpriteHandler();

	//! Checks device caps.
	bool checkDeviceCaps();

	//! Draw text with either DX font or cusom font.
	void drawText(string text, int x, int y, TextType type = CUSTOM, int size = 8, DWORD color = 0xff000000);

	//! Draws text using ID3DXFont.
	void drawSmallDxFont(string text, int x, int y, DWORD color = 0xff000000);
	void drawBigDxFont(string text, int x, int y, DWORD color = 0xff000000);

	//! Draws a custom font.
	void drawCustomFont(string text, int x, int y, int size = 8, DWORD color = 0xff000000);

	//! Draw shadow shape from list
	void drawShadowShape(std::list<Vector> *pointList, D3DCOLOR color = 0xff000000);

	void onLostDevice();
	void onResetDevice();

	Camera* getCamera()				{return mCam;}
	void setCamera(Camera* cam)		{mCam = cam;}

	Effects* getFXhandler()			{return mFX;};

	DrawableTex2D* getAlphaTexture(){return mAlphaTexture;}

	//Temp
	IDirect3DTexture9* mTest;
	IDirect3DTexture9* mStencilCircle;
private:
	IDirect3DVertexBuffer9* mVB_texture;
	IDirect3DVertexBuffer9* mVB_Rect;
	IDirect3DVertexBuffer9* mVB_fast;
	IDirect3DVertexBuffer9* mVB_blend;
	IDirect3DVertexBuffer9* mVB_blendTex;
	ID3DXFont* mFont;
	ID3DXFont* mBigFont;

	Effects *mFX;

	Camera* mCam;

	LPD3DXSPRITE mSpriteHandler;
	DrawableTex2D *mAlphaTexture;

	//Drawing alphas
	IDirect3DTexture9* mCircle;
	IDirect3DTexture9* mShadow;
	IDirect3DTexture9* mSquare;
	Rect mDrawRect;
	Rect mAlphaRect;
	

	//Font* mCustomFont;
	D3DCAPS9 mCaps;

	// Matrices for drawing
	D3DXMATRIX T, S, R;
};	// Class

extern Graphics* gGraphics;

#endif