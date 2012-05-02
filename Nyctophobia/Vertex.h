//=============================================================================
// Vertex.h by Frank Luna (C) 2005 All Rights Reserved.
//
// This file contains all of our vertex structures.  We will add to it
// as needed throughout the book.
//=============================================================================

#ifndef VERTEX_H
#define VERTEX_H

#include <d3dx9.h>

// Call in constructor and destructor, respectively, of derived application class.
void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();

//==============================================================
struct RectVertex
{
	RectVertex():pos(0.0f, 0.0f, 0.0f), color(0xff000000){}
	RectVertex(float x, float y, float z):pos(x,y,z), color(0xff000000){}
	RectVertex(float x, float y, float z, D3DCOLOR c):pos(x,y,z), color(c){}
	RectVertex(const D3DXVECTOR3& v):pos(v), color(0xff000000){}
	RectVertex(const D3DXVECTOR3& v,  D3DCOLOR c):pos(v), color(c){}

	D3DXVECTOR3 pos;
	D3DCOLOR color;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct TextureVertex
{
	TextureVertex():pos(0.0f, 0.0f, 0.0f){}
	TextureVertex(float x, float y, float z):pos(x,y,z){}
	TextureVertex(const D3DXVECTOR3& v):pos(v){}
	
	D3DXVECTOR3 pos;
	D3DXVECTOR2 tex0;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct BlendVertex
{
	BlendVertex():pos(0.0f, 0.0f, 0.0f){}
	BlendVertex(float x, float y, float z):pos(x,y,z){}
	BlendVertex(const D3DXVECTOR3& v):pos(v){}
	
	D3DXVECTOR3 pos;
	D3DXVECTOR2 tex0;
	D3DCOLOR color;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct BlendTexVertex
{
	BlendTexVertex():pos(0.0f, 0.0f, 0.0f){}
	BlendTexVertex(float x, float y, float z):pos(x,y,z){}
	BlendTexVertex(const D3DXVECTOR3& v):pos(v){}
	
	D3DXVECTOR3 pos;
	D3DXVECTOR2 tex0;
	D3DXVECTOR2 tex1;
	static IDirect3DVertexDeclaration9* Decl;
};

#endif