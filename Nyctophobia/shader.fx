//=============================================================================
// Shader.fx
//=============================================================================

uniform extern float4x4 gWVP;
uniform extern texture	gTex;
uniform extern texture	gBlendMap;
uniform extern float	gRotation;
uniform extern float3	gShadowMod;

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR; //Anisotropic; //NOT ALWAYS SUPPORTED
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler BlendMapS = sampler_state
{
	Texture = <gBlendMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};
	

struct OutputVS
{
    float4 posH : POSITION0;
	float4 clr	: COLOR0;
};

struct Output2VS
{
    float4 posH		: POSITION0;
	float4 clr		: COLOR0;
	float2 tex		: TEXCOORD0;
	float2 blend	: TEXCOORD1;
};


//Gaussian Blur



//================================================================

OutputVS ShadowVS(float3 posL : POSITION0, float4 c : COLOR0)
{
    // Zero out our output.
	OutputVS outVS = (OutputVS)0;
	
	// Transform to homogeneous clip space.	
	outVS.posH = mul(float4(posL, 1.0f), gWVP);

	// Color
	outVS.clr = c;
	
	// Done--return the output.
    return outVS;
}


float4 ShadowPS(float4 c : COLOR0) : COLOR //ONLY PShader3
{
	return c;
}

technique ShadowTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 ShadowVS();
        pixelShader  = compile ps_2_0 ShadowPS();

		// Specify the render/device states associated with this pass.
		FillMode = Solid;
    }
}

//=========================================================================

Output2VS BlendVS(float3 posL : POSITION0, float4 c : COLOR0, float2 tex0 : TEXCOORD0) // float4 c : COLOR0,
{
    // Zero out our output.
	Output2VS outVS = (Output2VS)0;
	
	// Transform to homogeneous clip space.	
	outVS.posH = mul(float4(posL, 1.0f), gWVP);

	// Color
	outVS.clr = c;

	//rotation
	float2 r = tex0;
	r -= 0.5;
	float ci = cos(gRotation);
    float si = sin(gRotation);
	float4 rot = float4(ci, -si, si, ci);

	float2 tex = mul(r, float2x2(rot));
	tex += 0.5;
	outVS.tex = tex;
	
	// Done--return the output.
    return outVS;
}


float4 BlendPS(float4 c : COLOR0, float2 tex : TEXCOORD0) : COLOR //, float2 tex : TEXCOORD0
{
	float4 clr = c;
	c.r *= gShadowMod.r * c.a;
	c.g *= gShadowMod.g * c.a;
	c.b *= gShadowMod.b * c.a;

	float b = tex2D(BlendMapS, tex).r;
	if(b.r == 0) {
		clip(-1);
	}

	float alpha = (b.r);
	clr.a *= alpha;
	clr.r *= alpha;
	clr.g *= alpha;
	clr.b *= alpha;
    return clr;
}

technique BlendTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 BlendVS();
        pixelShader  = compile ps_2_0 BlendPS();

		// Specify the render/device states associated with this pass.
		FillMode = Solid;
    }
}

//=====================================================================

Output2VS BlendTexVS(float3 posL : POSITION0, float2 tex0 : TEXCOORD0, float2 tex1 : TEXCOORD1, float4 c : COLOR0) // float4 c : COLOR0,
{
    // Zero out our output.
	Output2VS outVS = (Output2VS)0;
	
	// Transform to homogeneous clip space.	
	outVS.posH = mul(float4(posL, 1.0f), gWVP);

	// Color
	outVS.clr = c;

	//rotation
	float2 r = tex0;
	r -= 0.5;
	float ci = cos(gRotation);
    float si = sin(gRotation);
	float4 rot = float4(ci, -si, si, ci);

	float2 tex = mul(r, float2x2(rot));
	tex += 0.5;
	outVS.tex = tex;

	outVS.blend = tex1;
	
	// Done--return the output.
    return outVS;
}


float4 BlendTexPS(float2 tex0 : TEXCOORD0, float2 tex1 : TEXCOORD1) : COLOR //, float2 tex : TEXCOORD0
{
	float4 clr = float4(tex2D(TexS, tex0).rgba);
	clr.r *= gShadowMod.r;
	clr.g *= gShadowMod.g;
	clr.b *= gShadowMod.b;

	float3 b = tex2D(BlendMapS, tex1).r;
	float alpha = (b.r);

	clr.a *= alpha;
    return clr;
}

technique BlendTexTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 BlendTexVS();
        pixelShader  = compile ps_2_0 BlendTexPS();

		// Specify the render/device states associated with this pass.
		FillMode = Solid;
    }
}
