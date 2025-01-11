#include <Hazard.hpp>
#include <Player.hpp>
#include <cmath>

Hazard::Hazard(Vec2D size, std::unordered_map<int, std::string>&& fields) : Object(size, std::move(fields)) {
	prio = 2;
}

Sawblade::Sawblade(Vec2D size, std::unordered_map<int, std::string>&& fields) : Hazard(size, std::move(fields)) {}



bool Sawblade::touching(Player const& p) const {
	// check if p is inside a circle with radius size.x / 2 and position as pos

	Vec2D corners[4] = {
		Vec2D(p.getLeft(), p.getBottom()).rotate(p.rotation, p.pos),
		Vec2D(p.getRight(), p.getBottom()).rotate(p.rotation, p.pos),
		Vec2D(p.getRight(), p.getTop()).rotate(p.rotation, p.pos),
		Vec2D(p.getLeft(), p.getTop()).rotate(p.rotation, p.pos)
	};

	float radius = size.x;
	for (auto& c : corners) {
		auto diff = c - pos;
		if (std::sqrt(diff.x * diff.x + diff.y * diff.y) <= radius)
			return true;
		c = diff;
	}

	if (std::signbit(corners[2].x) != std::signbit(corners[0].x) && std::signbit(corners[2].y) != std::signbit(corners[0].y)) {
		return true;
	}

	return false;
}

void Hazard::collide(Player& p) const {
	p.dead = true;
}
