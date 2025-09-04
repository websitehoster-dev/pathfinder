#include <Hazard.hpp>
#include <Player.hpp>
#include <cmath>

Hazard::Hazard(Vec2D size, std::unordered_map<int, std::string>&& fields) : Object(size, std::move(fields)) {
	// Hazards are processed last, after everything else
	prio = 2;
}

bool Sawblade::touching(Player const& p) const {
	// Hitbox detection for circle

	Vec2D corners[4] = {
		Vec2D(p.getLeft(), p.getBottom()).rotate(p.rotation, p.pos),
		Vec2D(p.getRight(), p.getBottom()).rotate(p.rotation, p.pos),
		Vec2D(p.getRight(), p.getTop()).rotate(p.rotation, p.pos),
		Vec2D(p.getLeft(), p.getTop()).rotate(p.rotation, p.pos)
	};

	float radius = size.x;

	// Check if any corner of the player is within the sawblade radius
	for (auto& c : corners) {
		auto diff = c - pos;
		if (std::sqrt(diff.x * diff.x + diff.y * diff.y) <= radius)
			return true;
	}

	// Check if sawblade center is inside the player hitbox using proper point-in-polygon test
	// Use the winding number algorithm for rotated rectangles
	Vec2D sawbladeCenter = pos;
	int windingNumber = 0;
	
	for (int i = 0; i < 4; i++) {
		Vec2D v1 = corners[i];
		Vec2D v2 = corners[(i + 1) % 4];
		
		if (v1.y <= sawbladeCenter.y) {
			if (v2.y > sawbladeCenter.y) { // upward crossing
				if (((v2.x - v1.x) * (sawbladeCenter.y - v1.y) - (v2.y - v1.y) * (sawbladeCenter.x - v1.x)) > 0) {
					windingNumber++;
				}
			}
		} else {
			if (v2.y <= sawbladeCenter.y) { // downward crossing
				if (((v2.x - v1.x) * (sawbladeCenter.y - v1.y) - (v2.y - v1.y) * (sawbladeCenter.x - v1.x)) < 0) {
					windingNumber--;
				}
			}
		}
	}
	
	if (windingNumber != 0) {
		return true;
	}

	// Check if sawblade intersects any edge of the player rectangle
	for (int i = 0; i < 4; i++) {
		Vec2D v1 = corners[i];
		Vec2D v2 = corners[(i + 1) % 4];
		
		// Distance from point to line segment
		Vec2D edge = v2 - v1;
		Vec2D toCenter = sawbladeCenter - v1;
		
		float edgeLength = std::sqrt(edge.x * edge.x + edge.y * edge.y);
		if (edgeLength == 0) continue;
		
		// Project point onto line
		float t = (toCenter.x * edge.x + toCenter.y * edge.y) / (edgeLength * edgeLength);
		t = std::max(0.0f, std::min(1.0f, t)); // Clamp to segment
		
		Vec2D closest = v1 + edge * t;
		Vec2D distVec = sawbladeCenter - closest;
		float distance = std::sqrt(distVec.x * distVec.x + distVec.y * distVec.y);
		
		if (distance <= radius) {
			return true;
		}
	}

	return false;
}

bool Hazard::touching(Player const& p) const {
	// For triangular spikes, use more precise collision detection
	// Get the rotated corners of the player hitbox
	Vec2D corners[4] = {
		Vec2D(p.getLeft(), p.getBottom()).rotate(p.rotation, p.pos),
		Vec2D(p.getRight(), p.getBottom()).rotate(p.rotation, p.pos),
		Vec2D(p.getRight(), p.getTop()).rotate(p.rotation, p.pos),
		Vec2D(p.getLeft(), p.getTop()).rotate(p.rotation, p.pos)
	};

	// Define triangle vertices based on hazard size and rotation
	Vec2D triangleVertices[3];
	float halfWidth = size.x / 2;
	float halfHeight = size.y / 2;
	
	// Default upward-pointing triangle
	triangleVertices[0] = Vec2D(0, halfHeight);      // top point
	triangleVertices[1] = Vec2D(-halfWidth, -halfHeight); // bottom left
	triangleVertices[2] = Vec2D(halfWidth, -halfHeight);  // bottom right
	
	// Rotate triangle vertices and translate to hazard position
	for (int i = 0; i < 3; i++) {
		triangleVertices[i] = triangleVertices[i].rotate(rotation, Vec2D(0, 0)) + pos;
	}
	
	// Check if any player corner is inside the triangle
	for (auto& corner : corners) {
		if (pointInTriangle(corner, triangleVertices[0], triangleVertices[1], triangleVertices[2])) {
			return true;
		}
	}
	
	// Check if any triangle edge intersects with player edges
	for (int i = 0; i < 4; i++) {
		Vec2D p1 = corners[i];
		Vec2D p2 = corners[(i + 1) % 4];
		
		for (int j = 0; j < 3; j++) {
			Vec2D t1 = triangleVertices[j];
			Vec2D t2 = triangleVertices[(j + 1) % 3];
			
			if (lineSegmentsIntersect(p1, p2, t1, t2)) {
				return true;
			}
		}
	}
	
	return false;
}

bool Hazard::pointInTriangle(Vec2D p, Vec2D a, Vec2D b, Vec2D c) const {
	// Use barycentric coordinates to check if point is inside triangle
	Vec2D v0 = c - a;
	Vec2D v1 = b - a;
	Vec2D v2 = p - a;
	
	float dot00 = v0.x * v0.x + v0.y * v0.y;
	float dot01 = v0.x * v1.x + v0.y * v1.y;
	float dot02 = v0.x * v2.x + v0.y * v2.y;
	float dot11 = v1.x * v1.x + v1.y * v1.y;
	float dot12 = v1.x * v2.x + v1.y * v2.y;
	
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
	
	return (u >= 0) && (v >= 0) && (u + v <= 1);
}

bool Hazard::lineSegmentsIntersect(Vec2D p1, Vec2D p2, Vec2D p3, Vec2D p4) const {
	float d1 = direction(p3, p4, p1);
	float d2 = direction(p3, p4, p2);
	float d3 = direction(p1, p2, p3);
	float d4 = direction(p1, p2, p4);
	
	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
		((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
		return true;
	}
	
	return false;
}

float Hazard::direction(Vec2D a, Vec2D b, Vec2D c) const {
	return (c.x - a.x) * (b.y - a.y) - (b.x - a.x) * (c.y - a.y);
}

void Hazard::collide(Player& p) const {
	p.dead = true;
}
