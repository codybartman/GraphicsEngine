#pragma pack_matrix( row_major )


//  Constant Buffers
Texture2D mainTexture : register(t0);

SamplerState aSampler : register(s0);

cbuffer CamData : register(b0)
{
	float4x4 View;
	float4x4 Projection;
};

cbuffer LightParameters : register (b1)
{
	float4 EyePosWorld;
};

cbuffer InstanceData : register(b2)
{
	float4x4 World;
	float4x4 WorldInv;
};

cbuffer FogData : register(b3)
{
	float4 FogColor;
	float FogStart;
	float FogRange;
}

// ******* Shader operations

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 PosMS : POSITION;
	float4 Norm : NORMAL;
	float2 Tex : TEXCOORD;
	float4 WorldPos : WPOSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float4 nor : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output;
	output.PosMS = Pos;  // We pass along the raw model space position 
	output.Norm = nor;	 // and the face normal

	output.WorldPos = mul(Pos, World);

	output.Pos = mul(Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = Tex;

	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	// Compute light values in model-space
	float4 msEyePos = mul(EyePosWorld, WorldInv);
	float4 msDirToEye = normalize(msEyePos - input.PosMS);

	float4 myTexture = mainTexture.Sample(aSampler, input.Tex);

	float4 litColor = myTexture;

	/****************************************************/
	// Fog Demo

	float distToEye = length(msEyePos - input.PosMS);
	float FogPercent = saturate((distToEye - FogStart) / FogRange);  // See https://msdn.microsoft.com/en-us/library/windows/desktop/bb509645(v=vs.85).aspx
	FogPercent *= FogColor.w;

	float SpecialFogColor = FogColor;
	float ColorPercent = 0;

	if (input.WorldPos.y < 0)
	{
		float Depth = input.PosMS.y;
		ColorPercent = Depth / 200;
		//ColorPercent = 1 + ColorPercent;
		//ColorPercent *= FogColor.w;
		//ColorPercent = 1.0f;
	}



	FogPercent *= ColorPercent;
	// blend litcolor with fog level
	// C_src = FogColor
	// C_dst = litColor
	// C = C_src * fogPercent + C_dst * (1 - fogPercent)
	litColor = lerp(litColor, SpecialFogColor, FogPercent);				// See https://msdn.microsoft.com/en-us/library/windows/desktop/bb509618(v=vs.85).aspx



	return litColor;
}
