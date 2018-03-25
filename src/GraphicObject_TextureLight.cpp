#include "GraphicObject_TextureLight.h"
#include "Model.h"
#include <assert.h>



GraphicObject_TextureLight::GraphicObject_TextureLight(ShaderTextureLight * shader, Model * mod)
{
	SetModel(mod);
	pShader = shader;
	world = Matrix(IDENTITY);


	int n = mod->GetMeshCount();
	MeshTextures = new Texture*[n];
	for (int i = 0; i < n; i++)
		MeshTextures[i] = 0;
}



GraphicObject_TextureLight::~GraphicObject_TextureLight()
{
	delete[] MeshTextures;
}

void GraphicObject_TextureLight::SetWorld(const Matrix & worldIn, const Matrix &world_ReflectedIn)
{
	world = worldIn;
	world_Reflected = world_ReflectedIn;
}

void GraphicObject_TextureLight::SetTexture(Texture * texIn)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
		MeshTextures[i] = texIn;
}

void GraphicObject_TextureLight::SetTexture(Texture * texIn, int meshNum)
{
	assert(pModel->ValidMeshNum(meshNum));
	MeshTextures[meshNum] = texIn;
}



void GraphicObject_TextureLight::Render()
{

	pShader->SetToContext(pShader->GetContext());
	pModel->BindVertexIndexBuffers(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldAndSpec(world, Vect(1, 1, 1, 100));
		pShader->SetTextureResourceAndSampler(MeshTextures[i]);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}

void GraphicObject_TextureLight::RenderReflection()
{
	pShader->SetToContext(pShader->GetContext());
	pModel->BindVertexIndexBuffers(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldAndSpec(world_Reflected, Vect(1, 1, 1, 10));
		pShader->SetTextureResourceAndSampler(MeshTextures[i]);
		pModel->RenderMesh(pShader->GetContext(), i);

	}
}