#pragma once
#include <string>
#include <vector>
#include "Fbx.h"

namespace Model
{
	struct ModelData
	{
		Fbx* pfbx_;
	    Transform transform_;
		std::string filename_;
	};
	int Load(std::string filename);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel);
	void DrawPseudoNormal(int hModel);
	void Release();

	void Raycast(int hModel, RayCastData& rayData);
}