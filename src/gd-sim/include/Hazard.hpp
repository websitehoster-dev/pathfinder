#pragma once
#include <Object.hpp>

struct Hazard : public Object {
    Hazard(Vec2D size, std::unordered_map<int, std::string>&& fields);
    void collide(Player&) const override;
};

#ifndef DEMO
struct Sawblade : public Hazard {
    Sawblade(Vec2D size, std::unordered_map<int, std::string>&& fields);
    bool touching(Player const&) const override;
};
#endif