#include "GraphicObject_ColorLight.h"
#include "Model.h"
#include "ShaderTexture.h"
#include <assert.h>




GraphicObject_ColorLight::GraphicObject_ColorLight(ShaderColorLight * shader, Model * mod)
{
	SetModel(mod);
	pShader = shader;

	world = Matrix(IDENTITY);
}

GraphicObject_ColorLight::~GraphicObject_ColorLight()
{

}

void GraphicObject_ColorLight::SetColor(Vect &colorIn)
{
	color = colorIn;
}

void GraphicObject_ColorLight::SetWorld(const Matrix & worldIn, const Matrix &world_ReflectedIn)
{
	world = worldIn;
	world_Reflected = world_ReflectedIn;
}

void GraphicObject_ColorLight::Render()
{
	pShader->SetToContext(pShader->GetContext());
	pShader->SendWorldAndMaterial(world, color, color, Vect(1,1,1,100));
	pModel->Render(pShader->GetContext());
}

void GraphicObject_ColorLight::RenderReflection()
{

	pShader->SetToContext(pShader->GetContext());
	pShader->SendWorldAndMaterial(world_Reflected, color, color, Vect(1, 1, 1, 100));
	pModel->Render(pShader->GetContext());
}
