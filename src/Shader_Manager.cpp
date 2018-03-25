#include "Shader_Manager.h"


#include "Model.h"
#include "ModelTools.h"
#include "TerrainModel.h"
#include "DirectXTex.h"
#include "Texture.h"

Shader_Manager::Shader_Manager(ID3D11Device * md3dDeviceIn, ID3D11DeviceContext* md3dImmediateContextIn)
{
	md3dDevice = md3dDeviceIn;
	md3dImmediateContext = md3dImmediateContextIn;
	SetMirror();
	SetShaderCol();
	SetShaderTex();
	SetSkyBox();
	Vect fog(0.4f, 0.2f, 0.2f, 1.0f);
	SetFog(fog);
}



Shader_Manager::~Shader_Manager()
{
	delete pShader_Texture;
	delete pShader_TextureLight;
	delete pShader_ColorLight;

	delete pModel_Skybox;
	delete pModel_Ship;
	delete pModel_Sphere;
	delete pModel_Sphere_C;
	delete pModel_Pyramid;
	delete pModel_Crate;
	delete pModel_Mirror;

	delete SkyBox;
	delete Ship;
	delete Sphere_texture;
	delete Sphere_Color;
	delete Pyramid;
	delete Crate;
	delete Mirror;

	delete mText_SkyBox;
	delete mText_Ship01;
	delete mText_Crate;
	delete mText_Concrete;
	delete mText_Test512;
}

void Shader_Manager::SetMirror()
{
	// Initialize the world matrix
	mWorld_Ground = Matrix(SCALE, 1.0f, 1.0f, 1.0f) ;
	mWorld_Ground *= Matrix(TRANS, 0, -10.0f, 0.0f);
	mirrorMat = Matrix(IDENTITY); // Slight offset from the 'wall' to avoid z-fighting

	// Create Reflection matrix for mirror plane.
	// Uses the mirror's normal N and position
	Vect N = mirrorMat.get(ROW_1);
	Vect p = mirrorMat.get(ROW_3);
	float d = -p.dot(N);
	Matrix NTN(N[x] * N, N[y] * N, N[z] * N, d * N);  // Set the 4 rows
	NTN[m3] = 0;	// Zero out the #$#@$! 1 that's defaulted in the w position
	NTN[m7] = 0;
	NTN[m11] = 0;
	ReflectionMat = Matrix(IDENTITY) - 2 * NTN;
	ReflectionMat[m15] = 1; // correcting the corner entry
							// See also: https://en.wikipedia.org/wiki/Transformation_matrix
							// and http://ami.ektf.hu/uploads/papers/finalpdf/AMI_40_from175to186.pdf
}

void Shader_Manager::SetSkyBox()
{
	pShader_Texture = new ShaderTexture(md3dDevice);
	pShader_Texture->SetToContext(md3dImmediateContext);

	mWorld_SkyBox = Matrix(IDENTITY);
	mWorld_SkyBox_Reflection = mWorld_SkyBox * ReflectionMat;
	mWorld_SkyBox_Reflection *= Matrix(SCALE, 0.9f, 0.9f, 0.9f);
	pModel_Skybox = new Model(md3dDevice, Model::PreMadedeModels::SkyBox, 500.0f);
	SkyBox = new GraphicObject_Texture(pShader_Texture, pModel_Skybox);
	mText_SkyBox = new Texture(md3dDevice, L"../Assets/Textures/SkyBox.tga");

	SkyBox->SetWorld(mWorld_SkyBox, mWorld_SkyBox_Reflection);
	SkyBox->SetTexture(mText_SkyBox);
}

void Shader_Manager::SetShaderTex()
{
	pShader_TextureLight = new ShaderTextureLight(md3dDevice);
	pShader_TextureLight->SetToContext(md3dImmediateContext);

	mWorld_Ship = Matrix(SCALE, 1.0f, 1.0f, 1.0f);
	mWorld_Sphere = Matrix(SCALE, 1.0f, 3.0f, 1.0f) *Matrix(TRANS, 5.0f, -10.0f, 5.0f);
	mWorld_Crate = Matrix(SCALE, 1.0f, 1.0f, 1.0f) *	Matrix(TRANS, 0, +2.50f, 0.0f);
	mWorld_Crate *= Matrix(ROT_Y, 1.0f);

	pModel_Ship = new Model(md3dDevice, "../Assets/Models/Ship.azul");
	pModel_Sphere = new Model(md3dDevice, Model::PreMadedeModels::UnitSphere);
	pModel_Crate = new Model(md3dDevice, Model::PreMadedeModels::UnitBoxRepeatedTexture, 0.3f);

	Ship = new GraphicObject_TextureLight(pShader_TextureLight, pModel_Ship);
	Sphere_texture = new GraphicObject_TextureLight(pShader_TextureLight, pModel_Sphere);
	Crate = new GraphicObject_TextureLight(pShader_TextureLight, pModel_Crate);

	Ship->SetWorld(mWorld_Ship, mWorld_Ship * ReflectionMat);
	Sphere_texture->SetWorld(mWorld_Sphere, mWorld_Sphere * ReflectionMat);
	Crate->SetWorld(mWorld_Crate, mWorld_Crate * ReflectionMat);

	mText_Ship01 = new Texture(md3dDevice, L"../Assets/Textures/Ship_F.tga");
	mText_Crate = new Texture(md3dDevice, L"../Assets/Textures/CrateFace.tga");
	mText_Concrete = new Texture(md3dDevice, L"../Assets/Textures/Concrete.tga");
	mText_Test512 = new Texture(md3dDevice, L"../Assets/Textures/SkyBox.tga");

	Ship->SetTexture(mText_Ship01);
	Sphere_texture->SetTexture(mText_Concrete);
	Crate->SetTexture(mText_Crate);


}

void Shader_Manager::SetShaderCol()
{
	pShader_ColorLight = new ShaderColorLight(md3dDevice);
	pShader_ColorLight->SetToContext(md3dImmediateContext);

	pModel_Sphere_C = new Model(md3dDevice, Model::PreMadedeModels::UnitSphere);
	pModel_Pyramid = new Model(md3dDevice, Model::PreMadedeModels::BigPyramid);

	//pModel_Ground = new Model(md3dDevice, Model::PreMadedeModels::BigPlane);
	pModel_Mirror = new Model(md3dDevice, Model::PreMadedeModels::BigPlane, 1000.0f);

	Sphere_Color = new GraphicObject_ColorLight(pShader_ColorLight, pModel_Sphere_C);
	Pyramid = new GraphicObject_ColorLight(pShader_ColorLight, pModel_Pyramid);

	//Ground = new GraphicObject_ColorLight(pShader_ColorLight, pModel_Ground);
	Mirror = new GraphicObject_ColorLight(pShader_ColorLight, pModel_Mirror);

	mWorld_Sphere_C = Matrix(SCALE, 1.0f, 1.0f, 1.0f) *		Matrix(TRANS, 10.0f, -10.0f, -5.0f);
	mWorld_Pyramid = Matrix(SCALE, 1.0f, 1.0f, 1.0f) *		Matrix(TRANS, 6, -16, 15);
	mWorld_Pyramid *= Matrix(ROT_Y, 24.0f);

	Vect color = Colors::Brown;
	Sphere_Color->SetColor(color);
	color = Colors::DarkGreen;
	Pyramid->SetColor(color);
	color = Colors::DarkOrange;
	//Ground->SetColor(color);
	color = Vect(0, .5, 1, .2f);
	Mirror->SetColor(color);

	Sphere_Color->SetWorld(mWorld_Sphere_C, mWorld_Sphere_C * ReflectionMat);
	Pyramid->SetWorld(mWorld_Pyramid, mWorld_Pyramid * ReflectionMat);
	//Ground->SetWorld(mWorld_Ground, mWorld_Ground * ReflectionMat);
	Mirror->SetWorld(mirrorMat, mirrorMat * ReflectionMat);

}

void Shader_Manager::SetLights(PointLight_ToShare LightIn[])
{
	for (int i = 0; i < PLCount; i++)
	{
		Light[i] = LightIn[i];
		LightPos[i] = LightIn[i].GetPosition();

		LightIn[i].FlipPosition(ReflectionMat);
		LightPos_Reflected[i] = LightIn[i].GetPosition();
	}

	pShader_TextureLight->SetPointLightParameters(LightIn);
	pShader_ColorLight->SetPointLightParameters(LightIn);


	Vect LightDirection(0.0f, -0.4f, 1.0f);
	Vect Amb(0.2f, 0.2f, 0.2f, 0.2f);
	Vect Dif(0.2f, 0.2f, 0.2f, 0.2f);

	pShader_TextureLight->SetDirectionalLightParameters(LightDirection.getNorm(), Amb, Dif);
	pShader_ColorLight->SetDirectionalLightParameters(LightDirection.getNorm(), Amb, Dif);


	SLpos = Vect(0.0f, 10.0f, 0.0f);
	Vect att = .5 * Vect(1, 0, 0);
	Vect dir = -SLpos.getNorm();
	float spotExp = 50;
	Vect amb = Vect(0.0f, 0.0f, 0.0f);
	Vect dif = 1.0 * Vect(1, 1, 1);


	pShader_TextureLight->SetSpotLightParameters(SLpos, 10, att, dir, spotExp, amb, dif);
	pShader_ColorLight->SetSpotLightParameters(SLpos, 10, att, dir, spotExp, amb, dif);

}

void Shader_Manager::UpdateLightPos()
{
	Matrix transform(TRANS, 0.0f, 00.0f, -0.01f);
	mWorld_Ship *= transform;
	mWorld_Crate *= transform;
	Ship->SetWorld(mWorld_Ship, mWorld_Ship * ReflectionMat);
	Crate->SetWorld(mWorld_Crate, mWorld_Crate * ReflectionMat);

	Vect SLMove( 0.0f, 00.0f, -0.01f);
	SLpos += SLMove;
	pShader_TextureLight->SetSpotLightPos(SLpos);
	pShader_ColorLight->SetSpotLightPos(SLpos);

	//Matrix Identity(IDENTITY);
	//Matrix movement(IDENTITY);
	Vect movement(0, -0.1, 0, 0);
	for (int i = 0; i < PLCount; i++)
	{
		Vect currentPos = Light[i].GetPosition();
		Light[i].UpdatePos( currentPos + movement);
		LightPos[i] = Light[i].GetPosition() ;

		LightPos_Reflected[i] = LightPos[i] * ReflectionMat;
	}
}

void Shader_Manager::SetFog(Vect &FogColor)
{
	pShader_Texture->SetFogData(5.0f, 25.0f, FogColor);
	pShader_TextureLight->SetFogData(5.0f, 25.0f, FogColor);
	pShader_ColorLight->SetFogData(5.0f, 25.0f, FogColor);
}

void Shader_Manager::SendCam(Camera &mCam)
{
	pShader_Texture->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	pShader_TextureLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	pShader_ColorLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());

	//Vect eyepos;
	mCam.getPos(eyepos);
}

void Shader_Manager::RenderSkyBox()
{
	//pShader_Texture->
	SkyBox->Render();
}

void Shader_Manager::RenderTextLight()
{
	//pShader_TextureLight->SetToContext(md3dImmediateContext);
	for (int i = 0; i < PLCount; i++)
	{
		pShader_TextureLight->SetPointLightPos(LightPos[i], i);
	}
	pShader_TextureLight->SendLightParameters(eyepos);
	
	Crate->Render();
	Ship->Render();
}

void Shader_Manager::RenderColorLight()
{
	//pShader_ColorLight->SetToContext(md3dImmediateContext);
	for (int i = 0; i < PLCount; i++)
	{
		pShader_ColorLight->SetPointLightPos(i, LightPos[i]);
	}
	pShader_ColorLight->SendLightParameters(eyepos);

	Pyramid->Render();
	Sphere_Color->Render();
}

void Shader_Manager::RenderSkyBox_Reflection()
{
	//pShader_Texture->
	//SkyBox->RenderReflection();
}

void Shader_Manager::RenderTextLight_Reflection()
{
	//pShader_TextureLight->SetToContext(md3dImmediateContext);	for (int i = 0; i < PLCount; i++)
	for (int i = 0; i < PLCount; i++)
	{
		pShader_TextureLight->SetPointLightPos(LightPos_Reflected[i], i);
	}
	pShader_TextureLight->SendLightParameters(eyepos);

	Crate->RenderReflection();
	Ship->RenderReflection();
}

void Shader_Manager::RenderColorLight_Reflection()
{
	//pShader_ColorLight->SetToContext(md3dImmediateContext);
	for (int i = 0; i < PLCount; i++)
	{
		pShader_ColorLight->SetPointLightPos(i, LightPos_Reflected[i]);
	}
	pShader_ColorLight->SendLightParameters(eyepos);

	Crate->RenderReflection();
	//Floor->RenderReflection();
}



void Shader_Manager::RenderMirror()
{
	Mirror->Render();
}

void Shader_Manager::RenderBehindMirror()
{
	pShader_ColorLight->SetToContext(md3dImmediateContext);

	//Pyramid->Render();
	//Ground->Render();
	//Sphere_texture->Render();
}