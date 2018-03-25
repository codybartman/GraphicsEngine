#pragma pack_matrix( row_major )

// Material properties
struct Material
{
	float4 Specular;  // Hack: w holds the specular power
};

// Light properties
struct PhongADS
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
};

// Basic Phong Illumination computation
void PhongModel(
	Material mat,
	float4 myMaterial,
	PhongADS lightint,
	float3 L,
	float3 normal,
	float3 DirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
	// every light source adds to the ambient light total
	ambient = myMaterial * lightint.Ambient;

	float diffuseFactor = dot(L, normal);

	// Diffuse and specular contribution only if not facing away from light source
	if (diffuseFactor > 0.0f)
	{
		diffuse = diffuseFactor * myMaterial * lightint.Diffuse;

		float3 r = reflect(-L, normal);		// Compute the reflected ray of light
		float spec_val = max(dot(r, DirToEye.xyz), 0);
		float specFactor = pow(spec_val, mat.Specular.w);
		spec = specFactor * mat.Specular * lightint.Specular;
	}
	else
	{
		diffuse = float4(0, 0, 0, 0);
		spec = float4(0, 0, 0, 0);
	}
}

// Directional Light Data
struct DirectionalLight
{
	PhongADS LightInt;
	float4 Direction;
};

// Point Light data
struct PointLight
{
	PhongADS LightInt;
	float4 Position;
	float4 Attenuation;
	float Range;
};

// Spot light data
struct SpotLight
{
	PhongADS LightInt;
	float4 Position;
	float4 Attenuation;
	float4 Direction;
	float SpotExp;
	float Range;
};

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
	DirectionalLight DirLight;
	PointLight PntLight01;
	PointLight PntLight02;
	PointLight PntLight03;
	PointLight PntLight04;
	SpotLight SpLight;
	float4 EyePosWorld;
};

cbuffer InstanceData : register(b2)
{
	float4x4 World;
	float4x4 WorldInv;
	Material Mater;
};

cbuffer FogData : register(b3)
{
	float4 FogColor;
	float FogStart;
	float FogRange;
					
}

// Directional Light Computation
void ComputeDirectionalLight(
	Material mat,
	float4 myTexture,
	DirectionalLight DirLit,
	float4 normal,
	float4 DirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
	// Zeroing out the initial ADS contributions
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	spec = float4(0, 0, 0, 0);

	// we compute ligth in the model-space 
	// For directional light, the source is infinitely far, so translation have no effect
	float3 L = normalize(mul(-DirLit.Direction.xyz, (float3x3)WorldInv));

	PhongModel(mat, myTexture, DirLight.LightInt, L, normal.xyz, DirToEye.xyz, ambient, diffuse, spec);
}

// Point Light Computation
void ComputePointLight(
	Material mat,
	float4 myTexture,
	PointLight PLight,
	float4 posms,
	float4 normal,
	float4 DirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
	// Zeroing out the ADS contributions
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	spec = float4(0, 0, 0, 0);

	// we compute the model-space position
	float3 litPosMS = mul(PLight.Position, WorldInv).xyz;
	float3 L = litPosMS - posms.xyz;

	// Early out if out of range
	float d = length(L);
	if (d > PLight.Range)
		return;

	L /= d;	// normalize 

	PhongModel(mat, myTexture, PLight.LightInt, L, normal.xyz, DirToEye.xyz, ambient, diffuse, spec);

	// Now we attenuate based on range
	float att = 1 / dot(PLight.Attenuation.xyz, float3(1, d, d*d));
	// Ambient not attenuated
	diffuse *= att;
	spec *= att;
}

// Spot Light Computation
void ComputeSpotLight(
	Material mat,
	float4 myTexture,
	SpotLight SptLight,
	float4 posms,
	float4 normal,
	float4 DirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
	// Zeroing out the ADS contributions
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	spec = float4(0, 0, 0, 0);

	// ****** Same calculations as Point Light  ****

	// we compute the model-space position
	float3 litPosMS = mul(SptLight.Position, WorldInv).xyz;
	float3 L = litPosMS - posms.xyz;

	float d = length(L);
	if (d > SptLight.Range)
		return;

	L /= d;  // normalize 

	PhongModel(mat, myTexture, SptLight.LightInt, L, normal.xyz, DirToEye.xyz, ambient, diffuse, spec);

	// Now we attenuate based on range
	float att = 1 / dot(SptLight.Attenuation.xyz, float3(1, d, d*d));

	// Ambient not attenuated
	diffuse *= att;
	spec *= att;

	// *****************************************

	// now we compute the spotlight factor
	float4 msLitDir = SptLight.Direction;
	msLitDir.w = 0; // making sure we have a vector and not a point
	msLitDir = normalize(mul(msLitDir, WorldInv));
	float spot = pow(max(dot(-L, msLitDir.xyz), 0), SptLight.SpotExp);

	ambient *= spot;
	diffuse *= spot;
	spec *= spot;
}



// ******* Shader operations

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 PosMS : POSITION;
	float4 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float4 nor : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output;
	output.PosMS = Pos;  // We pass along the raw model space position 
	output.Norm = nor;	 // and the face normal

	output.Pos = mul(Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex =  Tex;

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

	float4 ambient = float4(0, 0, 0, 0);
	float4 diffuse = float4(0, 0, 0, 0);
	float4 spec = float4(0, 0, 0, 0);

	float4 A, D, S;
	float4 myTexture = mainTexture.Sample(aSampler, input.Tex);
	

	ComputeDirectionalLight(Mater, myTexture, DirLight, normalize(input.Norm), msDirToEye, A, D, S);

	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(Mater, myTexture, PntLight01, input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);

	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(Mater, myTexture, PntLight02, input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);

	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(Mater, myTexture, PntLight03, input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);

	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(Mater, myTexture, PntLight04, input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);

	ambient += A;
	diffuse += D;
	spec += S;

	ComputeSpotLight(Mater, myTexture, SpLight, input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);

	ambient += A;
	diffuse += D;
	spec += S;

	float4 litColor = ambient + diffuse + spec;

	/****************************************************/
	// Fog Demo

	//float distToEye = length(msEyePos - input.PosMS);
	//float FogPercent = saturate((distToEye - FogStart) / FogRange);  // See https://msdn.microsoft.com/en-us/library/windows/desktop/bb509645(v=vs.85).aspx
	//
	//float ColorPercent = 0;

	//if (input.PosMS.y < 8 && input.PosMS.y > 0)
	//{
	//	ColorPercent = input.PosMS.y / 8;
	//	ColorPercent = 1 - ColorPercent;
	//	ColorPercent *= FogColor.w;
	//}

	//FogPercent *= ColorPercent;

	//litColor = lerp(litColor, FogColor, FogPercent);				// See https://msdn.microsoft.com/en-us/library/windows/desktop/bb509618(v=vs.85).aspx
	//

	/****************************************************/

	return litColor;
	//return myText;
}