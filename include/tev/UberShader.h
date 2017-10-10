// This file was developed by Thomas Müller <thomas94@gmx.net>.
// It is published under the BSD 3-Clause License within the LICENSE file.

#pragma once

#include <tev/GlTexture.h>

#include <nanogui/glutil.h>

TEV_NAMESPACE_BEGIN

class UberShader {
public:
    UberShader();
    virtual ~UberShader();

    // Draws just a checkerboard.
    void draw(const Eigen::Vector2f& pixelSize, const Eigen::Vector2f& checkerSize);

    // Draws an image.
    void draw(
        const Eigen::Vector2f& pixelSize,
        const Eigen::Vector2f& checkerSize,
        const GlTexture* textureImage,
        const Eigen::Matrix3f& transformImage,
        float exposure,
        float offset,
        ETonemap tonemap
    );

    // Draws a difference between a reference and an image.
    void draw(
        const Eigen::Vector2f& pixelSize,
        const Eigen::Vector2f& checkerSize,
        const GlTexture* textureImage,
        const Eigen::Matrix3f& transformImage,
        const GlTexture* textureReference,
        const Eigen::Matrix3f& transformReference,
        float exposure,
        float offset,
        ETonemap tonemap,
        EMetric metric
    );

private:
    void bindCheckerboardData(const Eigen::Vector2f& pixelSize, const Eigen::Vector2f& checkerSize);

    void bindImageData(
        const GlTexture* textureImage,
        const Eigen::Matrix3f& transformImage,
        float exposure,
        float offset,
        ETonemap tonemap
    );

    void bindReferenceData(
        const GlTexture* textureReference,
        const Eigen::Matrix3f& transformReference,
        EMetric metric
    );

    nanogui::GLShader mShader;
    GlTexture mColorMap;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

TEV_NAMESPACE_END
