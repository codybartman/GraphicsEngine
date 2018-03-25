

#ifndef _GraphicObject_ColorLight
#define _GraphicObject_ColorLight

#include "GraphicObject_Base.h"
#include "Vect.h"
#include "Texture.h"
//#include "ShaderColor.h"
#include "ShaderColorLight.h"

class GraphicObject_ColorLight : public GraphicObject_Base
{
	friend class ShaderColorLight;

public:
	GraphicObject_ColorLight() = delete;
	GraphicObject_ColorLight(ShaderColorLight* shader, Model* mod);
	GraphicObject_ColorLight(const GraphicObject_ColorLight&) = delete;				   // Copy constructor
	GraphicObject_ColorLight(GraphicObject_ColorLight&&) = default;					   // Move constructor
	GraphicObject_ColorLight& operator= (const GraphicObject_ColorLight&) & = default;   // Copy assignment operator
	GraphicObject_ColorLight& operator= (GraphicObject_ColorLight&&) & = default;		   // Move assignment operator
	~GraphicObject_ColorLight();


	void SetColor( Vect &textIn);
	void SetWorld(const Matrix & worldIn, const Matrix &world_ReflectedIn);
	virtual void Render() override;
	
	void RenderReflection();


private:
	ShaderColorLight *pShader;
	Vect color;
	Matrix world;
	Matrix world_Reflected;
};



#endif  _GraphicObject_ColorLight




