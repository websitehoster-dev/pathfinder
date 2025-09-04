#pragma once
#include <Object.hpp>

struct Hazard : public Object {
    Hazard(Vec2D size, std::unordered_map<int, std::string>&& fields);
    bool touching(Player const&) const override;
    void collide(Player&) const override;
    
private:
    bool pointInTriangle(Vec2D p, Vec2D a, Vec2D b, Vec2D c) const;
    bool lineSegmentsIntersect(Vec2D p1, Vec2D p2, Vec2D p3, Vec2D p4) const;
    float direction(Vec2D a, Vec2D b, Vec2D c) const;
};

struct Sawblade : public Hazard {
    using Hazard::Hazard;
    bool touching(Player const&) const override;
};
