#include "Material.h"

#include <glad/glad.h>

#include <algorithm>

namespace OM3D
{

    Material::Material()
    {
    }

    void Material::set_program(std::shared_ptr<Program> prog)
    {
        _program = std::move(prog);
    }

    void Material::set_blend_mode(BlendMode blend)
    {
        _blend_mode = blend;
    }

    void Material::set_cull_mode(CullMode cull)
    {
        _culling_mode = cull;
    }

    void Material::set_depth_test_mode(DepthTestMode depth)
    {
        _depth_test_mode = depth;
    }

    void Material::set_depth_mask(GLboolean mask)
    {
        _depth_mask = mask;
    }

    void Material::set_texture(u32 slot, std::shared_ptr<Texture> tex)
    {
        if (const auto it = std::find_if(_textures.begin(), _textures.end(), [&](const auto &t)
                                         { return t.second == tex; });
            it != _textures.end())
        {
            it->second = std::move(tex);
        }
        else
        {
            _textures.emplace_back(slot, std::move(tex));
        }
    }

    bool Material::is_transparent()
    {
        return _blend_mode != BlendMode::None;
    }

    void Material::bind(CullMode force_cullmode) const
    {
        switch (_blend_mode)
        {
        case BlendMode::None:
            glDisable(GL_BLEND);
            break;

        case BlendMode::Alpha:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;

        case BlendMode::Additive:
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            break;
        }

        switch (_depth_test_mode)
        {
        case DepthTestMode::None:
            glDisable(GL_DEPTH_TEST);
            break;

        case DepthTestMode::Equal:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_EQUAL);
            break;

        case DepthTestMode::Standard:
            glEnable(GL_DEPTH_TEST);
            // We are using reverse-Z
            glDepthFunc(GL_GEQUAL);
            break;

        case DepthTestMode::Reversed:
            glEnable(GL_DEPTH_TEST);
            // We are using reverse-Z
            glDepthFunc(GL_LEQUAL);
            break;
        }

        glDepthMask(_depth_mask);

        CullMode cull_to_apply = _culling_mode;
        if (force_cullmode != CullMode::None)
        {
            cull_to_apply = force_cullmode;
        }
        switch (cull_to_apply)
        {
        case CullMode::None:
            glDisable(GL_CULL_FACE);
            break;
        case CullMode::Backface:
            glCullFace(GL_BACK);
            glEnable(GL_CULL_FACE);
            break;
        case CullMode::Frontface:
            glCullFace(GL_FRONT);
            glEnable(GL_CULL_FACE);
            break;
        }

        for (const auto &texture : _textures)
        {
            texture.second->bind(texture.first);
        }
        _program->bind();
    }

    std::shared_ptr<Material> Material::empty_material()
    {
        static std::weak_ptr<Material> weak_material;
        auto material = weak_material.lock();
        if (!material)
        {
            material = std::make_shared<Material>();
            material->_program = Program::from_files("g_buffer.frag", "basic.vert");
            weak_material = material;
        }
        return material;
    }

    Material Material::textured_material()
    {
        Material material;
        material._program = Program::from_files("g_buffer.frag", "basic.vert", {"TEXTURED"});
        return material;
    }

    Material Material::textured_normal_mapped_material()
    {
        Material material;
        material._program = Program::from_files("g_buffer.frag", "basic.vert", std::array<std::string, 2>{"TEXTURED", "NORMAL_MAPPED"});
        return material;
    }

    std::shared_ptr<Material> Material::copy_material()
    {
        std::shared_ptr<Material> copy = std::make_shared<Material>(Material::textured_normal_mapped_material());
        copy->set_blend_mode(_blend_mode); 
        copy->set_cull_mode(_culling_mode);
        copy->set_depth_mask(_depth_mask);
        copy->set_depth_test_mode(_depth_test_mode);
        copy->set_program(_program);
        for (auto tex : _textures)
            copy->set_texture(tex.first, tex.second);

        return copy;
    }

}
