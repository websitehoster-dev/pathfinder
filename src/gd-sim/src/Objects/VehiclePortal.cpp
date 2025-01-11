#include <Portals.hpp>
#include <Player.hpp>

VehiclePortal::VehiclePortal(Vec2D size, std::unordered_map<int, std::string>&& fields) : EffectObject(size, std::move(fields)) {
	switch (atoi(fields[1].c_str())) {
		case 12:
			type = VehicleType::Cube;
			break;
		case 13:
			type = VehicleType::Ship;
			break;
		case 47:
			type = VehicleType::Ball;
			break;

		#ifndef DEMO
		case 111:
			type = VehicleType::Ufo;
			break;
		case 660:
			type = VehicleType::Wave;
			break;
		#endif

		default:
			type = VehicleType::Cube;
			break;
		}
}

void VehiclePortal::collide(Player& player) const {
	EffectObject::collide(player);
	bool n = false;

	if (player.vehicle.type != type) {
		#ifndef DEMO
		if (player.vehicle.type == VehicleType::Wave)
			player.velocity *= 0.9;
		#endif

		player.vehicle = Vehicle::from(type);
		n = true;
	}

	player.size = Vec2D(30, 30) * (DEMO_NONE(player.small ? 0.6 :) 1);
	player.vehicle.enter(player, this, n);
}