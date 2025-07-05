#include "PhysicsUtility.h"
#include "glm/glm.hpp"

glm::vec2 PhysicsUtility::LineVectorFromPoints(const glm::vec2& start, const glm::vec2& end) {
	return glm::normalize(end - start);
}
glm::vec2 PhysicsUtility::ProjectPointOnLine(const glm::vec2& point, const glm::vec2& lineStart, const glm::vec2& lineEnd) {
	glm::vec2 lineVector = LineVectorFromPoints(lineStart, lineEnd);
	float t = glm::dot(point - lineStart, lineVector);
	return lineStart + t * lineVector;
}