#include <Portals.hpp>
#include <Player.hpp>

GravityPortal::GravityPortal(Vec2D size, std::unordered_map<int, std::string>&& fields) : EffectObject(size, std::move(fields)), upsideDown(std::stoi(fields[1]) == 11) {}


void GravityPortal::collide(Player& p) const {
	EffectObject::collide(p);

	if (upsideDown != p.upsideDown) {
		p.velocity = -p.velocity / 2;
		p.upsideDown = upsideDown;
		p.gravityPortal = true;
	}
}
