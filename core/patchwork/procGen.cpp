#include "procGen.h"

namespace patchwork
{
	ew::MeshData createSphere(float radius, int numSegments)
	{
		ew::MeshData sphere;
		ew::Vertex v;
		//Verts
		float thetaStep = (2*ew::PI) / numSegments;
		float phiStep = ew::PI / numSegments;

		for (int row = 0; row <= numSegments; row++)
		{
			float phi = row * phiStep;
			for (int col = 0; col <= numSegments; col++)
			{
				float theta = thetaStep * col;
				
				v.normal.x = cosf(theta) * sinf(phi);
				v.normal.y = cosf(phi);
				v.normal.z = sinf(theta) * sinf(phi);

				v.pos = v.normal * radius;

				v.uv.x = float(col) / numSegments;
				v.uv.y = 1.0 - (float(row) / numSegments);

				sphere.vertices.push_back(v);
			}
		}

		//Inds
		unsigned int poleStart = 0;
		unsigned int sideStart = numSegments + 1;

		for (int i = 0; i < numSegments; i++)
		{
			sphere.indices.push_back(sideStart + i);
			sphere.indices.push_back(poleStart + i); //Pole
			sphere.indices.push_back(sideStart + i + 1);
		}

		unsigned int columns = numSegments + 1;

		for (int row = 1; row < numSegments - 1; row++)
		{
			for (int col = 0; col < numSegments; col++)
			{
				int start = row * columns + col;
				//Triangle 1
				sphere.indices.push_back(start);
				sphere.indices.push_back(start + 1);
				sphere.indices.push_back(start + columns);
				//Triangle 2
				sphere.indices.push_back(start + columns);
				sphere.indices.push_back(start + 1);
				sphere.indices.push_back(start + columns + 1);
			}
		}
		poleStart = (columns * columns) - columns;
		sideStart = poleStart - columns;
		for (int i = 0; i < numSegments; i++)
		{
			sphere.indices.push_back(sideStart + i);
			sphere.indices.push_back(sideStart + i + 1);
			sphere.indices.push_back(poleStart + i);
		}
		return sphere;
	}
	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		ew::MeshData cylinder;
		ew::Vertex topVert;
		ew::Vertex botVert;
		ew::Vertex v;

		//Verts
		float topY = height / 2; //y=0 is centered
		float botY = -topY;

		topVert.pos = ew::Vec3(0, topY, 0);
		topVert.normal = ew::Vec3(0, 1, 0);
		topVert.uv = ew::Vec2(0.5);
		cylinder.vertices.push_back(topVert);

		float thetaStep = (2 * ew::PI) / numSegments;
		for (int k = 1; k <= 2; k++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int i = 0; i <= numSegments; i++)
				{
					float theta = i * thetaStep;

					v.pos.x = cos(theta) * radius;
					v.pos.z = sin(theta) * radius;
					if (k % 2 != 0)
					{
						v.pos.y = topY;
						if (j == 0)
						{
							v.normal = ew::Vec3(cosf(theta), 0, sinf(theta));
							v.uv = ew::Vec2((float)i / numSegments, topY > 0 ? 1 : 0);
						}
						else
						{
							v.normal = ew::Vec3(0, topY, 0);
							v.uv = ew::Vec2(cosf(theta) * 0.5 + 0.5, sinf(theta) * 0.5 + 0.5);
						}
					}
					else
					{
						v.pos.y = botY;
						if (j == 0)
						{
							v.normal = ew::Vec3(cosf(theta), 0, sinf(theta));
							v.uv = ew::Vec2((float)i / numSegments, botY > 0 ? 1 : 0);
						}
						else
						{
							v.normal = ew::Vec3(0, botY, 0);
							v.uv = ew::Vec2(cosf(theta) * 0.5 + 0.5, sinf(theta) * 0.5 + 0.5);
						}
					}

					cylinder.vertices.push_back(v);
				}
			}
		}

		botVert.pos = ew::Vec3(0, botY, 0);
		botVert.normal = ew::Vec3(0, -1, 0);
		botVert.uv = ew::Vec2(0.5);
		cylinder.vertices.push_back(botVert);

		//Inds
		int columns = numSegments + 1;
		for (int i = 0; i < columns; i++)
		{
			cylinder.indices.push_back(0);
			cylinder.indices.push_back(i + 1);
			cylinder.indices.push_back(i);
		}
		int sideStart = columns;
		for (int i = 0; i < columns; i++)
		{
			int start = sideStart + i;
			cylinder.indices.push_back(start);
			cylinder.indices.push_back(start + 1);
			cylinder.indices.push_back(start + columns);
			cylinder.indices.push_back(start + columns);
			cylinder.indices.push_back(start + 1);
			cylinder.indices.push_back(start + columns + 1);
		}
		int bottomIndex = cylinder.vertices.size() - 1;
		sideStart = bottomIndex - columns;
		for (int i = 0; i < columns; i++)
		{
			cylinder.indices.push_back(bottomIndex);
			cylinder.indices.push_back(sideStart + i);
			cylinder.indices.push_back(sideStart + i + 1);
		}

		return cylinder;
	}
	ew::MeshData createPlane(float width, float height, int subdivisions)
	{
		ew::MeshData plane;
		ew::Vertex v;

		for (int row = 0; row <= subdivisions; row++)
		{
			for (int col = 0; col <= subdivisions; col++)
			{
				v.uv.x = (float(col) / subdivisions);
				v.uv.y = (float(row) / subdivisions);

				v.pos.x = width * (col / subdivisions);
				v.pos.y = 0;
				v.pos.z = -height * (row / subdivisions);
				
				v.normal = ew::Vec3(0, 1, 0);
				plane.vertices.push_back(v);
			}
		}

		int columns = subdivisions + 1;

		for (int row = 0; row < subdivisions; row++)
		{
			for (int col = 0; col < subdivisions; col++)
			{
				int start = row * columns + col;
				//Bottom right triangle
				plane.indices.push_back(start);
				plane.indices.push_back(start + 1);
				plane.indices.push_back(start + columns + 1);
				//Top left triangle
				plane.indices.push_back(start);
				plane.indices.push_back(start + columns);
				plane.indices.push_back(start + columns + 1);
			}
		}
		return plane;

		//Used https://www.youtube.com/watch?v=FKLbihqDLsg to help with initial uv stuffs.
	};
}