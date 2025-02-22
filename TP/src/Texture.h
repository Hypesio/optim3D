#ifndef TEXTURE_H
#define TEXTURE_H

#include <graphics.h>
#include <ImageFormat.h>

#include <glm/vec2.hpp>

#include <vector>
#include <memory>


namespace OM3D {

struct TextureData {
    std::unique_ptr<u8[]> data;
    glm::uvec2 size = {};
    ImageFormat format;

    static Result<TextureData> from_file(const std::string& file_name);
};

class Texture {

    public:
        Texture() = default;
        Texture(Texture&&) = default;
        Texture& operator=(Texture&&) = default;

        ~Texture();

        Texture(const TextureData& data);
        Texture(const glm::uvec2 &size, ImageFormat format);
        Texture(const glm::uvec2 &size, ImageFormat format, int value);
        Texture(const size_t buffer_size, ImageFormat format); // Texture Buffer

        void bind(u32 index) const;
        void bind_as_image(u32 index, AccessType access);
        void bind_as_buffer(u32 index) const;

        const glm::uvec2& size() const;
        const int buffer_size() const;

        const GLHandle &handle() const;

        static u32 mip_levels(glm::uvec2 size);

    private:
        friend class Framebuffer;

        GLHandle _handle;
        glm::uvec2 _size = {};
        size_t _buffer_size;
        GLHandle _buffer_handle;
        ImageFormat _format;
};

}

#endif // TEXTURE_H
