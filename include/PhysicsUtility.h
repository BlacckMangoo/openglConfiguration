#pragma once

#include <glm/glm.hpp>

#include <vector>


class PhysicsUtility {
public : 
	// 
	glm::vec2 LineVectorFromPoints(const glm::vec2& start, const glm::vec2& end);

	//Projection 
	glm::vec2 ProjectPointOnLine(const glm::vec2& point, const glm::vec2& lineStart, const glm::vec2& lineEnd);
};