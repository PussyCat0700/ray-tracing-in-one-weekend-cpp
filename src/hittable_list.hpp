#ifndef RAY_TRACING_HITTABLE_LIST_HPP
#define RAY_TRACING_HITTABLE_LIST_HPP

#include "hittable.hpp"

#include <memory>
#include <utility>
#include <vector>

class hittable_list final : public hittable
{
public:
    hittable_list() = default;
    hittable_list(std::shared_ptr<hittable> object)
    {
        add(std::move(object));
    }

    void clear()
    {
        objects.clear();
    }

    void add(std::shared_ptr<hittable>&& object)
    {
        objects.emplace_back(object);
    }

    bool hit(const ray& r, double t_min, double t_max,
        hit_record& rec) const override;
    bool bounding_box(double t0, double t1, aabb& output_box) const override;

    std::vector<std::shared_ptr<hittable>> objects;
};

inline bool hittable_list::hit(const ray& r, double t_min, double t_max,
    hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects)
    {
        if (object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

inline bool hittable_list::bounding_box(double t0, double t1,
    aabb& output_box) const
{
    if (objects.empty())
    {
        return false;
    }

    const aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects)
    {
        if (!object->bounding_box(t0, t1, output_box))
        {
            return false;
        }

        output_box =
            first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

#endif