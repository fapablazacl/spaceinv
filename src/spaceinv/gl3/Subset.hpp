
#pragma once

#ifndef __gl3_subset__
#define __gl3_subset__

#include "xe/DataFormat.hpp"
#include "Buffer.hpp"

namespace gl3 {

    struct SubsetAttrib : public xe::Attrib {
        int bufferIndex = 0;

        SubsetAttrib() {}

        SubsetAttrib(const std::string &name_, std::size_t count_, xe::DataType type_, int bufferIndex_) {
            name = name_;
            count = count_;
            type = type_;
            bufferIndex = bufferIndex_;
        }
    };

    typedef xe::DataFormat<SubsetAttrib> SubsetFormat;

    GLenum conv_type(xe::DataType type) {
        switch (type) {
            case xe::DataType::Float32: return GL_FLOAT;
            case xe::DataType::Int32:   return GL_INT;
            default: assert(false);
        }
    }

    class Subset {
    public:
        Subset() {}

        Subset(const SubsetFormat &format, const std::vector<Buffer> &buffers, const Buffer &ibuffer) {
            glGenVertexArrays(1, &id);
            assert(glGetError() == GL_NO_ERROR);

            glBindVertexArray(id);
            assert(glGetError() == GL_NO_ERROR);

            GLuint vertexAttrib = 0;

            for (const SubsetAttrib &attrib : format.attribs) {
                if (attrib.type == xe::DataType::Unknown) {
                    break;
                }

                const Buffer &buffer = buffers[attrib.bufferIndex];

                glBindBuffer(buffer.getTarget(), buffer.getId());
                assert(glGetError() == GL_NO_ERROR);
                
                glEnableVertexAttribArray(vertexAttrib);
                assert(glGetError() == GL_NO_ERROR);

                glVertexAttribPointer(vertexAttrib, attrib.count, conv_type(attrib.type), GL_FALSE, 0, nullptr);
                assert(glGetError() == GL_NO_ERROR);

                ++vertexAttrib;
            }

            if (ibuffer) {
                glBindBuffer(ibuffer.getTarget(), ibuffer.getId());
                glBindVertexArray(0);
                _indexed = true;

                assert(glGetError() == GL_NO_ERROR);
            }   
        }

        ~Subset() {
            if (id) {
                glDeleteVertexArrays(1, &id);
                id = 0;
            }

            assert(glGetError() == GL_NO_ERROR);
        }

        GLuint getId() const {
            return id;
        }

        bool indexed() const {
            return _indexed;
        }

    private:
        GLuint id = 0;
        bool _indexed = false;
    };
}

#endif