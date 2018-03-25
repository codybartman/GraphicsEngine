

#ifndef _GraphicObject_TextureLight
#define _GraphicObject_TextureLight

#include "GraphicObject_Base.h"
#include "Vect.h"
#include "Texture.h"
//#include "ShaderColor.h"
#include "ShaderTexture.h"
#include "ShaderTextureLight.h"

class GraphicObject_TextureLight : public GraphicObject_Base
{
	friend class ShaderTextureLight;

public:
	GraphicObject_TextureLight() = delete;
	GraphicObject_TextureLight(ShaderTextureLight* shader, Model* mod);
	GraphicObject_TextureLight(const GraphicObject_TextureLight&) = delete;				   // Copy constructor
	GraphicObject_TextureLight(GraphicObject_TextureLight&&) = default;					   // Move constructor
	GraphicObject_TextureLight& operator= (const GraphicObject_TextureLight&) & = default;   // Copy assignment operator
	GraphicObject_TextureLight& operator= (GraphicObject_TextureLight&&) & = default;		   // Move assignment operator
	~GraphicObject_TextureLight();

	void SetWorld(const Matrix & worldIn, const Matrix &world_ReflectedIn);
	void SetTexture(Texture * texIn);
	void SetTexture(Texture * texIn, int meshNum);
	virtual void Render() override;

	void RenderReflection();


private:
	ShaderTextureLight *pShader;

	Texture **MeshTextures;
	Matrix world;
	Matrix world_Reflected;
};



#endif  _GraphicObject_TextureLight




