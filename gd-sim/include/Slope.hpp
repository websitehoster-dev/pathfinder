#pragma once
#include <Block.hpp>

struct Slope : public Block {
    int orientation;
	
    Slope(Vec2D size, std::unordered_map<int, std::string>&& fields);
    void collide(Player&) const override;
    bool touching(Player const&) const override;

    void calc(Player& p) const;
    int gravOrient(Player const& p) const;
    double angle() const;
    double expectedY(Player const& p) const;
};
