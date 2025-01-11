#include <Block.hpp>
#include <Player.hpp>

#ifndef DEMO
bool BreakableBlock::touching(Player const& p) const {
	if (Block::touching(p)) {
		return !p.usedEffects.contains(id);
	}

	return false;
}

void BreakableBlock::collide(Player& p) const {
	//Block::collide(p);

	if (p.gravTop(*this) > p.gravBottom(p) + p.grav(5) && p.grav(pos.y) < p.grav(p.pos.y)) {
		p.dead = true;
	} else if (p.gravBottom(*this) < p.gravTop(p) - p.grav(5) && p.grav(pos.y) > p.grav(p.pos.y)) {
		p.dead = true;
	} else {
		Block::collide(p);
	}

	if (p.grounded) {
		p.snapData.playerFrame = 0;
	}

	if (p.dead) {
		p.dead = false;
		p.usedEffects.insert(id);
	}
}
#endif