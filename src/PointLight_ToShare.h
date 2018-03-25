#ifndef _PointLight_ToShare
#define _PointLight_ToShare

#include "Matrix.h"
class PointLight_ToShare : public Align16
{
public:
	PointLight_ToShare();
	PointLight_ToShare(Vect &pos, Vect &color);
	PointLight_ToShare(Vect &pos, float r, Vect &att, Vect &amb, Vect &dif);
	~PointLight_ToShare();

	Vect GetAmbient();
	Vect GetDiffuse();
	Vect GetPosition();
	Vect GetAttenuation();
	void UpdatePos(Vect &pos);
	float GetRange();

	void FlipPosition(Matrix &FlipMat);

private:

	Vect Ambient;
	Vect Diffuse;
	Vect Position;
	Vect Attenuation;
	float Range;


};


#endif _PointLight_ToShare