#ifndef _ShaderTextureN
#define _ShaderTextureN

#include "ShaderBase.h"
#include "Texture.h"
#include "assert.h"

class ShaderTexture : public ShaderBase
{
public:
	ShaderTexture(ID3D11Device* device);
	~ShaderTexture();

	virtual void SetToContext(ID3D11DeviceContext* devcon) override;

	void SetFogData(float FogStartIn, float FogRangeIn, Vect & FogColorIn);

	void SetTextureResourceAndSampler(Texture* tex);
	void SendWorld(const Matrix & world);
	void SendCamMatrices(const Matrix& view, const Matrix& proj);

	void SendLightParameters(const Vect & eyepos);

private:
	Matrix mView;
	Matrix mProjection;
	Matrix World;

	struct CamMatrices
	{
		Matrix View;
		Matrix Projection;
	};

	ID3D11Buffer*  mpBufferCamMatrices;

	struct ViewProjData
	{
		Matrix mView;
		Matrix mProjection;
	};

	struct Data_World
	{
		Matrix World;
		Matrix WorlInv;
	};

	struct Data_LightParams
	{
		Vect EyePosWorld;
	};

	ID3D11Buffer*  mpBufferLightParams;

	ID3D11Buffer*	mpBuffWord;

	struct FogData
	{
		Vect FogColor;
		float FogStart;
		float FogRange;

	};

	ID3D11Buffer*  mpBufferFogParams;
};



#endif _ShaderTextureN



