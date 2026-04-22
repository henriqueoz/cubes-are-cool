#ifndef SHADER_H_
#define SHADER_H_

#include <array>
#include <cstdint>

#include "raylib.h"

enum MATERIAL_LOC : std::uint8_t
{
    MATERIAL_LOC_AMBIENT,
    MATERIAL_LOC_DIFFUSE,
    MATERIAL_LOC_SPECULAR,
    MATERIAL_LOC_SHINENESS
};

enum LIGHT_LOC : std::uint8_t
{
    LIGHT_LOC_POSITION,
    LIGHT_LOC_AMBIENT,
    LIGHT_LOC_DIFFUSE,
    LIGHT_LOC_SPECULAR
};

struct PhongShaderLocations
{
    Shader shader;

    std::array<std::uint32_t, 4> materialLocs;
    std::array<std::uint32_t, 4> lightLocs;
};

struct ShaderMaterial
{
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
    float shineness;
};

struct ShaderLight
{
    Vector3 position;
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
};

struct ShaderValues
{
    ShaderMaterial material;
    ShaderLight light;
};

inline PhongShaderLocations GetPhongShaderLocs(const Shader shader);

inline void SetPhongShaderValues(const PhongShaderLocations locs, const ShaderValues values);

inline void SetPhongShaderValuesEx(const PhongShaderLocations locs, const ShaderMaterial material,
                                   const ShaderLight light);

inline PhongShaderLocations GetPhongShaderLocs(const Shader shader)
{
    PhongShaderLocations result{.shader = shader};

    result.materialLocs[MATERIAL_LOC_AMBIENT] = GetShaderLocation(shader, "material.ambient");
    result.materialLocs[MATERIAL_LOC_DIFFUSE] = GetShaderLocation(shader, "material.diffuse");
    result.materialLocs[MATERIAL_LOC_SPECULAR] = GetShaderLocation(shader, "material.specular");
    result.materialLocs[MATERIAL_LOC_SHINENESS] = GetShaderLocation(shader, "material.shineness");

    result.lightLocs[LIGHT_LOC_POSITION] = GetShaderLocation(shader, "light.position");
    result.lightLocs[LIGHT_LOC_AMBIENT] = GetShaderLocation(shader, "light.ambient");
    result.lightLocs[LIGHT_LOC_DIFFUSE] = GetShaderLocation(shader, "light.diffuse");
    result.lightLocs[LIGHT_LOC_SPECULAR] = GetShaderLocation(shader, "light.specular");

    return result;
}

inline void SetPhongShaderValues(const PhongShaderLocations locs, const ShaderValues values)
{
    SetPhongShaderValuesEx(locs, values.material, values.light);
}

inline void SetPhongShaderValuesEx(const PhongShaderLocations locs, const ShaderMaterial material,
                                   const ShaderLight light)
{
    const auto &materialLocs = locs.materialLocs;
    SetShaderValue(locs.shader, materialLocs[MATERIAL_LOC_AMBIENT], &material.ambient, SHADER_UNIFORM_VEC3);
    SetShaderValue(locs.shader, materialLocs[MATERIAL_LOC_DIFFUSE], &material.diffuse, SHADER_UNIFORM_VEC3);
    SetShaderValue(locs.shader, materialLocs[MATERIAL_LOC_SPECULAR], &material.specular, SHADER_UNIFORM_VEC3);
    SetShaderValue(locs.shader, materialLocs[MATERIAL_LOC_SHINENESS], &material.shineness, SHADER_UNIFORM_FLOAT);

    const auto &lightLocs = locs.lightLocs;
    SetShaderValue(locs.shader, lightLocs[LIGHT_LOC_POSITION], &light.position, SHADER_UNIFORM_VEC3);
    SetShaderValue(locs.shader, lightLocs[LIGHT_LOC_AMBIENT], &light.ambient, SHADER_UNIFORM_VEC3);
    SetShaderValue(locs.shader, lightLocs[LIGHT_LOC_DIFFUSE], &light.diffuse, SHADER_UNIFORM_VEC3);
    SetShaderValue(locs.shader, lightLocs[LIGHT_LOC_SPECULAR], &light.specular, SHADER_UNIFORM_VEC3);
}

#endif