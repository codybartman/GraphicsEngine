
#ifndef _ShaderTextureLight
#define _ShaderTextureLight

#include "ShaderBase.h"
#include "Texture.h"
#include "Matrix.h"
#include "PointLight_ToShare.h"
#include "LightCount.h"

struct ID3D11Buffer;
struct ID3D11Device;

class ShaderTextureLight : public ShaderBase
{

public:
	ShaderTextureLight(const ShaderTextureLight&) = delete;				 // Copy constructor
	ShaderTextureLight(ShaderTextureLight&&) = default;                    // Move constructor
	ShaderTextureLight& operator=(const ShaderTextureLight&) & = default;  // Copy assignment operator
	ShaderTextureLight& operator=(ShaderTextureLight&&) & = default;       // Move assignment operator
	~ShaderTextureLight();		  							         // Destructor

	ShaderTextureLight(ID3D11Device* device);

	virtual void SetToContext(ID3D11DeviceContext* devcon) override;

	void SetDirectionalLightParameters(const Vect& dir, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetPointLightParameters(PointLight_ToShare PLIn[]);
	void SetPointLightPos(const Vect & pos, int i);
	void SetPointLightParameters(int i, const Vect& pos, float r, const Vect& att, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetSpotLightPos(const Vect & pos);
	void SetSpotLightParameters(const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));

	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void SendLightParameters(const Vect& eyepos);
	void SetTextureResourceAndSampler(Texture* tex);
	void SendWorldAndSpec(const Matrix& world, const Vect& sp = Vect(.5f, .5f, .5f));
	void SetFogData(float FogStartIn, float FogRangeIn, Vect &FogColorIn);

private:
	static const int PLCount = LightCount::PLCount;
	struct MaterialSpec
	{
		Vect Specular;
	};

	struct PhongADS
	{
		Vect Ambient;
		Vect Diffuse;
		Vect Specular;
	};

	struct DirectionalLight
	{
		PhongADS Light;
		Vect Direction;
	};

	DirectionalLight DirLightData;

	struct PointLight
	{
		PhongADS Light;
		Vect Position;
		Vect Attenuation;
		float Range;
	};

	PointLight PointLightData[PLCount];

	struct SpotLight
	{
		PhongADS Light;
		Vect Position;
		Vect Attenuation;
		Vect Direction;
		float SpotExp;
		float Range;
	};

	SpotLight SpotLightData;

	struct CamMatrices
	{
		Matrix View;
		Matrix Projection;
	};

	ID3D11Buffer*  mpBufferCamMatrices;

	struct Data_WorldAndSpec
	{
		Matrix World;
		Matrix WorlInv;
		MaterialSpec Mat;
	};

	ID3D11Buffer*	mpBuffWordAndSpec;

	struct Data_LightParams
	{
		DirectionalLight DirLight;
		PointLight PntLight[PLCount];
		SpotLight SptLight;
		Vect EyePosWorld;
	};

	ID3D11Buffer*  mpBufferLightParams;

	struct FogData
	{
		Vect FogColor;
		float FogStart;
		float FogRange;
						
	};

	ID3D11Buffer*  mpBufferFogParams;

};

#endif _ShaderTextureLight

