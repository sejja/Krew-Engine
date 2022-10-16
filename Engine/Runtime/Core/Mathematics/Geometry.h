//
//	Geometry.h
//	Good Neighbours
//
//	Created by Fernando Uribe on 09/17/21
//	Copyright 2021 Diego Revilla. All Rights reserved
//

#ifndef _GEOMETRY__H_
#define _GEOMETRY__H_

namespace Engine {
    class GameObject;

    struct ENGINE_API Ray {
        Ray();
        Ray(const glm::vec3& _s, const glm::vec3& _e);
        Ray(const Ray& copy);
        void set_start(const glm::vec3& _s);
        void set_end(const glm::vec3& _e);
        void set_dir(const glm::vec3& _d);
        glm::vec3 start, end, dir;
    };

    struct ENGINE_API AABB
    {
        AABB();
        AABB(GameObject* obj);
        AABB(const glm::vec3& _min, const glm::vec3& _max);
        AABB(const AABB& copy);
        glm::vec3 min, max;
    };

    struct ENGINE_API Triangle
    {
        Triangle();
        Triangle(const glm::vec3& _a, const glm::vec3& _b, const glm::vec3& _c);
        Triangle(const Triangle& copy);
        glm::vec3 operator[](unsigned idx)const;
        glm::vec3& operator[](unsigned idx);
        glm::vec3 a, b, c;
    };

    struct ENGINE_API Sphere
    {
        Sphere();
        Sphere(const glm::vec3& _c, float _r);
        Sphere(const Sphere& copy);
        glm::vec3 c;
        float r;
    };

    struct ENGINE_API Plane
    {
        Plane();
        Plane(const glm::vec4& plane);
        Plane(const glm::vec3& _p, const glm::vec3& _n);
        Plane(const Plane& copy);
        Plane& operator=(const Plane& copy);
        float d;
        glm::vec3 n, p;
    };

    struct ENGINE_API Frustum
    {
        Frustum();
        Frustum(const glm::mat4& mtx);
        Frustum(const Frustum& copy);
        void Compute();
        glm::mat4 vp;
        Plane planes[6];
    };

    enum eClassify
    {
        eINSIDE = -1,
        eOVERLAPPING = 0,
        eOUTSIDE = 1
    };

    /// \brief Get closest point from [point vs plane]
    /// \param p point
    /// \param n plane normal
    /// \param d dot product between plane normal and a plane point
    glm::vec3 ENGINE_API closest_point_plane(const glm::vec3& p, const glm::vec3& n, float d);
    glm::vec3 ENGINE_API closest_point_plane(const glm::vec3& p, const Plane& plane);

    /// \brief Get closest points from [segment vs segment]
    /// \param sA start of segment A
    /// \param eA end of segment A
    /// \param eB start of segment B
    /// \param sB end of segment B
    /// \param rA closest point of segment A
    /// \param rB closest point of segment b
    void ENGINE_API closest_segment_segment(const glm::vec3& sA, const glm::vec3& eA, const glm::vec3& sB, const glm::vec3& eB, glm::vec3* rA, glm::vec3* rB);

    /// \brief Overlap [point vs aabb]
    /// \param p point
    /// \param min minimum coords of the aabb
    /// \param max maximum coords of the aabb
    bool ENGINE_API overlap_point_aabb(const glm::vec3& p, const glm::vec3& min, const glm::vec3& max);
    bool ENGINE_API overlap_point_aabb(const glm::vec3& p, const AABB& aabb);

    /// \brief Overlap [point vs sphere]
    /// \param p point
    /// \param c center of the sphere
    /// \param r radius of the sphere
    bool ENGINE_API overlap_point_sphere(const glm::vec3& p, const glm::vec3& c, float r);
    bool ENGINE_API overlap_point_sphere(const glm::vec3& p, const Sphere& sphere);

    /// \brief Classify point respect to a plane [plane vs point]
    /// \param n plane normal
    /// \param d dot product between plane normal and a plane point
    /// \param p point
    int ENGINE_API classify_plane_point(const glm::vec3& n, float d, const glm::vec3& p);
    int ENGINE_API classify_plane_point(const Plane& plane, const glm::vec3& p);

    /// \brief Classify triangle respect to a plane [plane vs triangle]
    /// \param n plane normal
    /// \param d dot product between plane normal and a plane point
    /// \param a triangle point
    /// \param b triangle point
    /// \param c triangle point
    int ENGINE_API classify_plane_triangle(const glm::vec3& n, float d, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    int ENGINE_API classify_plane_triangle(const Plane& plane, const Triangle& triangle);

    /// \brief Classify aabb respect to a plane [plane vs aabb]
    /// \param n plane normal
    /// \param d dot product between plane normal and a plane point
    /// \param min minimum coords of the aabb
    /// \param max maximum coords of the aabb
    int ENGINE_API classify_plane_aabb(const glm::vec3& n, float d, const glm::vec3& min, const glm::vec3& max);
    int ENGINE_API classify_plane_aabb(const Plane& plane, const AABB& aabb);

    /// \brief Classify sphere respect to a plane [plane vs sphere]
    /// \param n plane normal
    /// \param d dot product between plane normal and a plane point
    /// \param c center of the sphere
    /// \param r radius of the sphere
    int ENGINE_API classify_plane_sphere(const glm::vec3& n, float d, const glm::vec3& c, float r);
    int ENGINE_API classify_plane_sphere(const Plane& plane, const Sphere& sphere);

    /// \brief Overlap [aabb vs aabb]
    /// \param minA minimum coords of the aabb A
    /// \param maxA maximum coords of the aabb A
    /// \param minB minimum coords of the aabb B
    /// \param maxB maximum coords of the aabb B
    bool ENGINE_API overlap_aabb_aabb(const glm::vec3& minA, const glm::vec3& maxA, const glm::vec3& minB, const glm::vec3& maxB);
    bool ENGINE_API overlap_aabb_aabb(const AABB& aabb1, const AABB& aabb2);

    /// \brief Overlap [sphere vs sphere]
    /// \param c1 center of the sphere 1
    /// \param r1 radius of the sphere 1
    /// \param c2 center of the sphere 2
    /// \param r2 radius of the sphere 2
    bool ENGINE_API overlap_sphere_sphere(const glm::vec3& c1, float r1, const glm::vec3& c2, float r2);
    bool ENGINE_API overlap_sphere_sphere(const Sphere& sphere1, const Sphere& sphere2);

    /// \brief Intersection time [ray vs plane]
    /// \param s ray start point
    /// \param v ray direction
    /// \param n plane normal
    /// \param d dot product between plane normal and a plane point
    float ENGINE_API intersection_time_ray_plane(const glm::vec3& s, const glm::vec3& v, const glm::vec3& n, float d);
    float ENGINE_API intersection_time_ray_plane(const Ray& ray, const Plane& plane);

    /// \brief Intersection time [ray vs aabb]
    /// \param s ray start point
    /// \param v ray direction
    /// \param minP minimum coords of the aabb
    /// \param maxP maximum coords of the aabb
    float ENGINE_API intersection_time_ray_aabb(const glm::vec3& s, const glm::vec3& v, const glm::vec3& minP, const glm::vec3& maxP);
    float ENGINE_API intersection_time_ray_aabb(const Ray& ray, const AABB& aabb);

    /// \brief Intersection time [ray vs sphere]
    /// \param s ray start point
    /// \param v ray direction
    /// \param c center of the sphere
    /// \param r radius of the sphere
    float ENGINE_API intersection_time_ray_sphere(const glm::vec3& s, const glm::vec3& v, const glm::vec3& center, float r);
    float ENGINE_API intersection_time_ray_sphere(const Ray& ray, const Sphere& sphere);

    /// \brief Intersection time [ray vs triangle]
    /// \param s ray start point
    /// \param v ray direction
    /// \param a triangle point
    /// \param b triangle point
    /// \param c triangle point
    float ENGINE_API intersection_time_ray_triangle(const glm::vec3& s, const glm::vec3& v, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    float ENGINE_API intersection_time_ray_triangle(const Ray& ray, const Triangle& triangle);

    /// \brief Classify sphere respect to frustum planes [frustum vs sphere]
    /// \param n plane normals
    /// \param d dot products between plane normals and plane points
    /// \param c center of the sphere
    /// \param r radius of the sphere
    int ENGINE_API classify_frustum_sphere_naive(const glm::vec3* n, float* d, const glm::vec3& c, float r);
    int ENGINE_API classify_frustum_aabb_naive(const Frustum& f, const Sphere& sphere);

    /// \brief Classify aabb respect to frustum planes [frustum vs aabb]
    /// \param n plane normals
    /// \param d dot products between plane normals and plane points
    /// \param min minimum coords of the aabb
    /// \param max maximum coords of the aabb
    int ENGINE_API classify_frustum_aabb_naive(const glm::vec3* n, float* d, const glm::vec3& min, const glm::vec3& max);
    int ENGINE_API classify_frustum_aabb_naive(const Frustum& f, const AABB& aabb);
}

#endif // __GEOMETRY_HPP__
