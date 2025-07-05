#include "Collider.h"
#include <cfloat>

void Collider::EdgesFromPoints(const std::vector<glm::vec2>& points) {
	std::vector<glm::vec2> edges;
	size_t numPoints = points.size();
	for (size_t i = 0; i < numPoints; ++i) {
		glm::vec2 start = points[i];
		glm::vec2 end = points[(i + 1) % numPoints]; // wrap around to the first point
		edges.push_back(end - start);
	}
	this->edges = edges;
}

void Collider::Move(glm::vec2 offset) {
	for (auto& point : this->points) {
		point += offset;
	}
	EdgesFromPoints(points);
}

void Collider::PlacePointsAroundCentre(const glm::vec2& centre)
{
	// Apply the original offsets to the new center position
	for (size_t i = 0; i < points.size(); ++i) {
		points[i] = centre + originalOffsets[i];
	}
	EdgesFromPoints(points);
}

bool Collider::CheckCollision(Collider& other)
{
	// SAT collision - check projections along all edges of both colliders
	
	// Check projections along edges of this collider
	for (const auto& edge : this->edges) {
		glm::vec2 axis = glm::vec2(-edge.y, edge.x); // perpendicular to edge
		float minA = FLT_MAX, maxA = -FLT_MAX;
		float minB = FLT_MAX, maxB = -FLT_MAX;
		
		// Project this collider's points onto the axis
		for (const auto& point : this->points) {
			float projection = glm::dot(point, axis);
			minA = std::min(minA, projection);
			maxA = std::max(maxA, projection);
		}
		
		// Project other collider's points onto the axis
		for (const auto& point : other.points) {
			float projection = glm::dot(point, axis);
			minB = std::min(minB, projection);
			maxB = std::max(maxB, projection);
		}
		
		// Check for separation
		if (maxA < minB || maxB < minA) {
			return false; // no collision
		}
	}
	
	// Check projections along edges of the other collider
	for (const auto& edge : other.edges) {
		glm::vec2 axis = glm::vec2(-edge.y, edge.x); // perpendicular to edge
		float minA = FLT_MAX, maxA = -FLT_MAX;
		float minB = FLT_MAX, maxB = -FLT_MAX;
		
		// Project this collider's points onto the axis
		for (const auto& point : this->points) {
			float projection = glm::dot(point, axis);
			minA = std::min(minA, projection);
			maxA = std::max(maxA, projection);
		}
		
		// Project other collider's points onto the axis
		for (const auto& point : other.points) {
			float projection = glm::dot(point, axis);
			minB = std::min(minB, projection);
			maxB = std::max(maxB, projection);
		}
		
		// Check for separation
		if (maxA < minB || maxB < minA) {
			return false; // no collision
		}
	}
	
	// If no separating axis found, collision detected
	return true;
}