// This file was developed by Thomas Müller <thomas94@gmx.net>.
// It is published under the BSD 3-Clause License within the LICENSE file.

#pragma once

#include <tev/Common.h>

#include <nanogui/opengl.h>

TEV_NAMESPACE_BEGIN

class GlTexture {
public:
    GlTexture(GLint clamping = GL_CLAMP_TO_EDGE, GLint filtering = GL_NEAREST, bool mipmap = true);

    GlTexture(const GlTexture& other) = delete;
    GlTexture(GlTexture&& other) noexcept
    : mId{other.mId}, mClamping{other.mClamping}, mFiltering{other.mFiltering}, mMipmap{other.mMipmap} {
        other.mId = 0;
    }

    GlTexture& operator=(const GlTexture& other) = delete;
    GlTexture& operator=(GlTexture&& other) noexcept {
        std::swap(mId, other.mId);
        std::swap(mClamping, other.mClamping);
        std::swap(mFiltering, other.mFiltering);
        std::swap(mMipmap, other.mMipmap);
        return *this;
    }

    ~GlTexture() noexcept {
        if (mId) {
            glDeleteTextures(1, &mId);
        }
    }

    GLuint id() const { return mId; }
    const std::vector<float>& data() const { return mData; }
    const Eigen::Vector2i& size() const { return mSize; }

    void setData(const std::vector<float>& data, const Eigen::Vector2i& size, int numChannels);

private:
    Eigen::Vector2i mSize = Eigen::Vector2i::Constant(0);
    std::vector<float> mData;
    int mNumChannels = 0;

    GLuint mId = 0;
    GLint mClamping;
    GLint mFiltering;
    bool mMipmap;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

TEV_NAMESPACE_END
