#include "GraphicObject_Texture.h"
#include "Model.h"
#include "ShaderTexture.h"
#include <assert.h>




GraphicObject_Texture::GraphicObject_Texture(ShaderTexture * shader, Model * mod)
{
	SetModel(mod);
	pShader = shader;

	world = Matrix(IDENTITY);
}

GraphicObject_Texture::~GraphicObject_Texture()
{

}

void GraphicObject_Texture::SetTexture(Texture *textIn)
{
	texture = textIn;
}

void GraphicObject_Texture::SetWorld(const Matrix & m, const Matrix &mReflected)
{
	world = m;
	world_Reflected = mReflected;
}

void GraphicObject_Texture::Render()
{
	pShader->SetToContext(pShader->GetContext());
	pModel->BindVertexIndexBuffers(pShader->GetContext());
	pShader->SendWorld(world);
	pShader->SetTextureResourceAndSampler(texture);
	pModel->Render(pShader->GetContext());
}

void GraphicObject_Texture::RenderReflection()
{
	pShader->SetToContext(pShader->GetContext());
	pModel->BindVertexIndexBuffers(pShader->GetContext());
	pShader->SendWorld(world_Reflected);
	pShader->SetTextureResourceAndSampler(texture);
	pModel->Render(pShader->GetContext());
}
