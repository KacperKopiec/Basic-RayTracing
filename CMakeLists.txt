cmake_minimum_required(VERSION 3.31)
project(raytracing)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(raytracing main.cpp
        objects/vec3.h
        utilities/color.h
        objects/ray.h
        objects/hit_record.h
        objects/hittable.h
        objects/sphere.h
        objects/hittable_list.h
        utilities/base.h
        utilities/interval.h
        render/camera.h
        objects/materials/material.h
        objects/materials/lambertian.h
        objects/materials/metal.h
        objects/materials/dielectric.h)
