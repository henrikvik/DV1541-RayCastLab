#include "MyShapes.h"

MyPlane::MyPlane(Vec normal, float distance, Color color)
{
	this->n/*ormal*/ = normal;
	this->p/*oint*/ = normal * distance;
	this->k/*onstant*/ = -n.Dot(p);

	this->d/*istance*/ = distance;
	this->c/*olor*/ = color;

	// n.x + n.y + n.z + k = 0;
}

void MyPlane::test(Ray & r, HitData & hit)
{
	float dot = n.Dot(r.d);

	if (dot != 0)
	{
		/*
			 | x = r.o.x + r.d.x * t
		ray: | y = r.o.y + r.d.y * t
		     | z = r.o.z + r.d.z * t

		plane: n.x + n.y + n.z + k = 0
		 
		n.x * (r.o.x + r.d.x * t) + n.y * (r.o.y + r.d.y * t) + n.z * (r.o.z + r.d.z * t) + k = 0
		t * (n.x * r.d.x + n.y * r.d.y + n.z * r.d.z) + (n.x * r.o.x +  n.y * r.o.y + n.z * r.o.z) + k = 0
		t * n.Dot(r.d) + n.Dot(r.o) + k = 0
		t * n.Dot(r.d) = -(n.Dot(r.o) + k)
		t = -(n.Dot(r.o) + k) / n.Dot(r.d)
		*/

		float t = -(n.Dot(r.o) + k) / dot;

		if (t < hit.t || hit.t < 0)
		{
			hit.t = t;
			hit.color = c;
			hit.lastNormal = n;
			hit.lastShape = this;
		}
	}
}

Vec MyPlane::normal(Vec & point)
{
	return n;
}

MySphere::MySphere(Vec center, float radius, Color color)
{
	this->c = color;
	this->p = center;
	this->r = radius;
	this->r2 = radius * radius;
}

void MySphere::test(Ray & r, HitData & hit)
{
	/*
		f(x) = ||x - p|| - r = 0
		f(ray) = ||o + td - p|| - r = 0
		f(ray) = (o + td - p) * (o + td - p) = r2
		f(ray) = (o + td - p) * (o + td - p) = r2
		f(ray) = (td + (o - p)) * (td + (o - p)) = r2
		f(ray) = td * td + td * (o - p) + td * (o - p) + (o - p) * (o - p) = r2
		f(ray) = t2 + 2t(d * (o - p)) + (o - p) * (o - p) - r2 = 0
		a = (d * (o - p))
		b = (o - p) * (o - p) - r2

		t2 + 2ta + b = 0
		t = -a +- sqr(a2 - b)
	*/
	Vec po = r.o - p;
	float a = r.d.Dot(po);
	float b = po.Dot(po) - r2;
	float c = a * a - b;

	if (c > 0)
	{
		float sqrc = sqrt(c);
		float t1 = -a + sqrc;
		float t2 = -a - sqrc;

		if (t1 > t2) std::swap(t1, t2);

		if (t1 > 0 && (t1 < hit.t || hit.t < 0))
		{
			hit.t = t1;
			hit.color = this->c;
			hit.lastNormal = normal(r.o);
			hit.lastShape = this;
		}
		else if (t2 > 0 && (t2 < hit.t || hit.t < 0))
		{
			hit.t = t2;
			hit.color = this->c;
			hit.lastNormal = normal(r.o);
			hit.lastShape = this;
		}
	}
	else if (c == 0)
	{
		float t = -a + sqrt(c);

		if (t > 0 && (t < hit.t || hit.t < 0))
		{
			hit.t = t;
			hit.color = this->c;
			hit.lastNormal = normal(r.o);
			hit.lastShape = this;
		}
	}
}

Vec MySphere::normal(Vec & o)
{
	Vec n = o - p;
	n.Normalize();
	return n;
}

MyTriangle::MyTriangle(Vec p1, Vec p2, Vec p3, Color color)
{
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->n = (p2 - p1).Cross(p3 - p1);
	this->d = -n.Dot(p1);
	this->c = color;
}

void MyTriangle::test(Ray & ray, HitData & hit)
{
	float ndotd = n.Dot(ray.d);
	if (ndotd != 0)
	{
		float t = -(n.Dot(ray.o) + d) / ndotd;	

		if (t > 0)
		{
			Vec p = ray(t);
		}
	}
}

Vec MyTriangle::normal(Vec & point)
{
	return n;
}
