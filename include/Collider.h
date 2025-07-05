#pragma once 
#include <vector>
#include <glm/glm.hpp>

class Collider {
	
public :
	Collider() = default;
	Collider(const std::vector<glm::vec2>& points) : originalOffsets(points), points(points) { EdgesFromPoints(points);  };

	std::vector<glm::vec2> points = {};
	std::vector<glm::vec2> edges = {}; 
	std::vector<glm::vec2> originalOffsets = {}; // Store original relative positions

	void PlacePointsAroundCentre(const glm::vec2& centre);
	void EdgesFromPoints(const std::vector<glm::vec2>& points);
	void Move(glm::vec2 offset);

	bool CheckCollision(Collider& other);
	
	
};