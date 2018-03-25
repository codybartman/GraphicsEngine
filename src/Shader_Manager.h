#ifndef  _Shader_Manager
#define  _Shader_Manager


#include <d3d11.h>
#include "d3dUtil.h"
#include "Align16.h"

// New includes for demo
#include "Vect.h"
#include "Matrix.h"
#include "Camera.h"
#include "ShaderTextureLight.h"
#include "ShaderColorLight.h"
#include "GraphicObject_TextureLight.h"
#include "GraphicObject_ColorLight.h"
#include "GraphicObject_Color.h"
#include "GraphicObject_Texture.h"
#include "PointLight_ToShare.h"

class Model;
class TerrainModel;
class Texture;

class Shader_Manager : public Align16
{
public:
	static const int PLCount = LightCount::PLCount;
	Shader_Manager(ID3D11Device * md3dDeviceIn, ID3D11DeviceContext* md3dImmediateContextIn);
	~Shader_Manager();

	void SetMirror();
	void SetSkyBox();
	void SetShaderTex();
	void SetShaderCol();
	void SetLights(PointLight_ToShare LightIn[]);
	void UpdateLightPos();
	void SetFog(Vect &FogColor);
	void SendCam(Camera &mCam);
	void RenderSkyBox();
	void RenderTextLight();

	void RenderTextLight_Reflection();

	void RenderColorLight_Reflection();

	void RenderColorLight();

	void RenderSkyBox_Reflection();

	void RenderMirror();

	void RenderBehindMirror();



private:
	ID3D11Device * md3dDevice;					// Connects to the graphics card
	ID3D11DeviceContext* md3dImmediateContext;	// Settings for the GPU to use

	Matrix mirrorMat;
	Matrix ReflectionMat;

	ShaderTexture *pShader_Texture;
	ShaderTextureLight *pShader_TextureLight;
	ShaderColorLight *pShader_ColorLight;


	PointLight_ToShare Light[PLCount];
	Vect eyepos;
	Vect LightPos[PLCount];
	Vect LightPos_Reflected[PLCount];
	Vect SLpos;

	Matrix mWorld_SkyBox;
	Matrix mWorld_SkyBox_Reflection;
	Matrix mWorld_Ship;
	Matrix mWorld_Sphere;
	Matrix mWorld_Sphere_C;
	Matrix mWorld_Pyramid;
	Matrix mWorld_Crate;
	Matrix mWorld_Ground;

	Model *pModel_Skybox;
	Model *pModel_Ship;
	Model *pModel_Sphere;
	Model *pModel_Sphere_C;
	Model *pModel_Pyramid;
	Model *pModel_Crate;
	//Model *pModel_Ground;
	Model *pModel_Mirror;

	GraphicObject_Texture *SkyBox;
	GraphicObject_TextureLight *Ship;
	GraphicObject_TextureLight *Sphere_texture;
	GraphicObject_ColorLight *Sphere_Color;
	GraphicObject_ColorLight *Pyramid;
	GraphicObject_TextureLight *Crate;
	//GraphicObject_ColorLight *Ground;
	GraphicObject_ColorLight *Mirror;

	Texture *mText_SkyBox;
	Texture *mText_Ship01;
	Texture *mText_Crate;
	Texture *mText_Concrete;
	Texture *mText_Test512;

};




#endif // ! _Shader_Manager





