
#ifndef _GraphicObject_Texture
#define _GraphicObject_Texture

#include "GraphicObject_Base.h"
#include "Vect.h"
#include "Texture.h"
#include "ShaderTexture.h"

class GraphicObject_Texture : public GraphicObject_Base
{
	friend class ShaderTextureLight;

public:
	GraphicObject_Texture() = delete;
	GraphicObject_Texture(ShaderTexture* shader, Model* mod);
	GraphicObject_Texture(const GraphicObject_Texture&) = delete;				   // Copy constructor
	GraphicObject_Texture(GraphicObject_Texture&&) = default;					   // Move constructor
	GraphicObject_Texture& operator= (const GraphicObject_Texture&) & = default;   // Copy assignment operator
	GraphicObject_Texture& operator= (GraphicObject_Texture&&) & = default;		   // Move assignment operator
	~GraphicObject_Texture();


	void SetTexture( Texture* textIn);
	void SetWorld(const Matrix & m, const Matrix &mReflected);
	virtual void Render() override;

	void RenderReflection();

private:
	ShaderTexture *pShader;
	Texture *texture;
	Matrix world;
	Matrix world_Reflected;
};



#endif  _GraphicObject_Texture




