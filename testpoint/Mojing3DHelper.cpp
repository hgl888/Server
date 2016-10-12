
#include "Mojing3DHelper.h"

float Distance(const Point3f &a, const Point3f &b)
{
	return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z -b.z));
}

Plane::Plane(Point3f topLeft, Point3f topRight, Point3f bottomLeft)
{
	//known three point��caculate RECT: ax+by+cz=d;   
	a = ((topRight.y - topLeft.y)*(bottomLeft.z - topLeft.z) - (topRight.z - topLeft.z)*(bottomLeft.y - topLeft.y));
	b = ((topRight.z - topLeft.z)*(bottomLeft.x - topLeft.x) - (topRight.x - topLeft.x)*(bottomLeft.z - topLeft.z));
	c = ((topRight.x - topLeft.x)*(bottomLeft.y - topLeft.y) - (topRight.y - topLeft.y)*(bottomLeft.x - topLeft.x));
	d = a*topLeft.x + b*topLeft.y + c*topLeft.z;

	float distance = sqrtf(a*a + b*b + c*c);
	a /= distance;
	b /= distance;
	c /= distance;
	d /= distance;

	m_xAxis = normalize(topRight - topLeft);
	m_yAxis = normalize(bottomLeft - topLeft);

	m_width = ::Distance(topLeft, topRight);
	m_height = ::Distance(topLeft, bottomLeft);

	m_topLeft = topLeft;
	m_topRight = topRight;
	m_bottomLeft = bottomLeft;
}

//�㵽ƽ�����   
float Plane::Distance(Point3f pt)
{
	return fabsf(a*pt.x + b*pt.y + c*pt.z + d) / sqrtf(a*a + b*b + c*c);
}

bool Plane::Intersection(Vector3f light, Point2f &position)
{
	float div = a * light.x + b * light.y + c * light.z;
	if (fabsf(div) < 1e-6)
	{
		// light is parallel with RECT
		return false;
	}

	float t = d / (a * light.x + b * light.y + c * light.z);
	if (t < 0.0f)
	{
		// light direction is reverse
		return false;
	}

	Vector3f vecInPlane = light * t - m_topLeft;

    float val = dot_product(m_xAxis, m_yAxis);
	if (fabs(dot_product(m_xAxis, m_yAxis)) < 1e-6)
	{
		//rectangle
		float width = dot_product(vecInPlane, m_xAxis);
		if ((width < 0) || (width > m_width))
			return false;

		float height = dot_product(vecInPlane, m_yAxis);
		if ((height < 0) || (height > m_height))
			return false;

		position.x = width / m_width;
		position.y = height / m_height;
		return true;
	}
	else
	{
		//Parallelogram
		float x1, x2, y1, y2, m, n;
		if ((div = m_xAxis.x * m_yAxis.y - m_yAxis.x * m_xAxis.y) != 0.0f)
		{
			// use x,y
			x1 = m_xAxis.x;
			y1 = m_xAxis.y;
			x2 = m_yAxis.x;
			y2 = m_yAxis.y;
			m = vecInPlane.x;
			n = vecInPlane.y;
		}
		else if ((div = m_xAxis.x * m_yAxis.z - m_yAxis.x * m_xAxis.z) != 0.0f)
		{
			// use x,z
			x1 = m_xAxis.x;
			y1 = m_xAxis.z;
			x2 = m_yAxis.x;
			y2 = m_yAxis.z;
			m = vecInPlane.x;
			n = vecInPlane.z;
		}
		else if ((div = m_xAxis.y * m_yAxis.z - m_yAxis.y * m_xAxis.z) != 0.0f)
		{
			// use y,z
			x1 = m_xAxis.y;
			y1 = m_xAxis.z;
			x2 = m_yAxis.y;
			y2 = m_yAxis.z;
			m = vecInPlane.y;
			n = vecInPlane.z;
		}
		else
		{
			// My Godness, this shall be impossible!
			return false;
		}

		float width = (m * y2 - n * x2) / div;
		if ((width < 0) || (width > m_width))
			return false;

		float height = (n * x1 - m * y1) / div;
		if ((height < 0) || (height > m_height))
			return false;

		position.x = width / m_width;
		position.y = height / m_height;
		return true;
	}
}