#include "PointLight_ToShare.h"

PointLight_ToShare::PointLight_ToShare()
{
	Vect zero(0, 0, 0, 0);
	Position = zero;
	Range = 50.0f;
	Attenuation = .2 * zero;
	Ambient = 0.0f * zero;
	Diffuse = 0.25f * zero;
}

PointLight_ToShare::PointLight_ToShare(Vect &pos, Vect &color)
{
	Position = pos;
	Range = 200.0f;
	Vect up(0, 1, 0);
	Attenuation = .2 * up;
	Ambient = 0.0f * color;
	Diffuse = 0.25f * color;
}

PointLight_ToShare::PointLight_ToShare(Vect &pos, float r, Vect &att, Vect &amb, Vect &dif)
{
	Position = pos;
	Range = r;
	Attenuation = att;
	Ambient = amb;
	Diffuse = dif;
}


PointLight_ToShare::~PointLight_ToShare()
{

}

Vect PointLight_ToShare::GetAmbient()
{
	return Ambient;
}

Vect PointLight_ToShare::GetDiffuse()
{
	return Diffuse;
}

Vect PointLight_ToShare::GetPosition()
{
	return Position;
}

Vect PointLight_ToShare::GetAttenuation()
{
	return Attenuation;
}

void PointLight_ToShare::UpdatePos(Vect &pos)
{
	Position = pos;
}

float PointLight_ToShare::GetRange()
{
	return Range;
}

void PointLight_ToShare::FlipPosition(Matrix &FlipMat)
{
	Position *= FlipMat;
}


