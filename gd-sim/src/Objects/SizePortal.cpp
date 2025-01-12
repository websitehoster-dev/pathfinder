#include <Portals.hpp>
#include <Player.hpp>

SizePortal::SizePortal(Vec2D size, std::unordered_map<int, std::string>&& fields) : EffectObject(size, std::move(fields)), small(std::stoi(fields[1]) == 101) {}


void SizePortal::collide(Player& p) const {
	EffectObject::collide(p);

	p.small = small;
}
