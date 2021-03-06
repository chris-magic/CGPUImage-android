/**
 * Created by lvHiei on 17-4-12.
 * This is a project of GPUImage implemented with c++, you can
 * use it free. if you find some bug please send me a email.
 * My Email is majun_1523@163.com.
 */

#include "GPUImagePixellatePositionFilter.h"


#ifdef __GLSL_SUPPORT_HIGHP__

// 片元着色器
extern const char _pixelLatePosition_fragment_shader[]=
"varying highp vec2 textureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"uniform highp float fractionalWidthOfPixel;\n"
"uniform highp float aspectRatio;\n"
"uniform lowp vec2 pixelateCenter;\n"
"uniform highp float pixelateRadius;\n"
"\n"
"void main()\n"
"{\n"
"    highp vec2 textureCoordinateToUse = vec2(textureCoordinate.x, (textureCoordinate.y * aspectRatio + 0.5 - 0.5 * aspectRatio));\n"
"    highp float dist = distance(pixelateCenter, textureCoordinateToUse);\n"
"\n"
"    if (dist < pixelateRadius)\n"
"    {\n"
"        highp vec2 sampleDivisor = vec2(fractionalWidthOfPixel, fractionalWidthOfPixel / aspectRatio);\n"
"        highp vec2 samplePos = textureCoordinate - mod(textureCoordinate, sampleDivisor) + 0.5 * sampleDivisor;\n"
"        gl_FragColor = texture2D(inputImageTexture, samplePos );\n"
"    }\n"
"    else\n"
"    {\n"
"        gl_FragColor = texture2D(inputImageTexture, textureCoordinate );\n"
"    }\n"
"}"
;

#else

// 片元着色器
extern const char _pixelLatePosition_fragment_shader[]=
"precision mediump float;\n"
"varying vec2 textureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"uniform float fractionalWidthOfPixel;\n"
"uniform float aspectRatio;\n"
"uniform vec2 pixelateCenter;\n"
"uniform float pixelateRadius;\n"
"\n"
"void main()\n"
"{\n"
"    vec2 textureCoordinateToUse = vec2(textureCoordinate.x, (textureCoordinate.y * aspectRatio + 0.5 - 0.5 * aspectRatio));\n"
"    float dist = distance(pixelateCenter, textureCoordinateToUse);\n"
"\n"
"    if (dist < pixelateRadius)\n"
"    {\n"
"        vec2 sampleDivisor = vec2(fractionalWidthOfPixel, fractionalWidthOfPixel / aspectRatio);\n"
"        vec2 samplePos = textureCoordinate - mod(textureCoordinate, sampleDivisor) + 0.5 * sampleDivisor;\n"
"        gl_FragColor = texture2D(inputImageTexture, samplePos );\n"
"    }\n"
"    else\n"
"    {\n"
"        gl_FragColor = texture2D(inputImageTexture, textureCoordinate );\n"
"    }\n"
"}"
;

#endif





GPUImagePixellatePositionFilter::GPUImagePixellatePositionFilter()
    : GPUImageFilter(_pixelLatePosition_fragment_shader)
{
    updateAspectRatio();

    m_fFractionalWidthOfAPixel = 0.05f;
    m_fRadius = 0.25f;
    m_pCenter[0] = 0.5f;
    m_pCenter[1] = 0.5f;

    m_iRadiusUniformLocation = -1;
    m_iFractionalUniformLocation = -1;
    m_iCenterUniformLocation = -1;
    m_iAspectRatioUniformLocation = -1;
}

GPUImagePixellatePositionFilter::~GPUImagePixellatePositionFilter()
{

}

void GPUImagePixellatePositionFilter::setCenter(float x, float y)
{
    m_pCenter[0] = x;
    m_pCenter[1] = y;
}

void GPUImagePixellatePositionFilter::setRadius(float radius)
{
    m_fRadius = radius;
}


void GPUImagePixellatePositionFilter::setRadius(int percent)
{
    float max = 1.0f;
    float min = 0.0f;

    if(percent < 0){
        percent = 0;
    }

    if(percent > 100){
        percent = 100;
    }

    float incremental = (max - min) / 100;

    float value = min + incremental * percent;
    setRadius(value);
}


void GPUImagePixellatePositionFilter::setFractionalWidthOfAPixel(float fractional)
{
    GLfloat min = 1.0 / 1280.0;

    if(m_iTextureWidth != 0){
        min = 1.0 / m_iTextureWidth;
    }

    if(fractional < min){
        fractional = min;
    }

    m_fFractionalWidthOfAPixel = fractional;
}

void GPUImagePixellatePositionFilter::setAspectRatio(float aspectRatio)
{
    m_fAspectRatio = aspectRatio;
}

bool GPUImagePixellatePositionFilter::createProgramExtra()
{
    m_iFractionalUniformLocation = glGetUniformLocation(m_uProgram, "fractionalWidthOfPixel");
    m_iAspectRatioUniformLocation = glGetUniformLocation(m_uProgram, "aspectRatio");
    m_iCenterUniformLocation = glGetUniformLocation(m_uProgram, "pixelateCenter");
    m_iRadiusUniformLocation = glGetUniformLocation(m_uProgram, "pixelateRadius");
    return GPUImageFilter::createProgramExtra();
}

bool GPUImagePixellatePositionFilter::beforeDrawExtra()
{
    glUniform1f(m_iFractionalUniformLocation, m_fFractionalWidthOfAPixel);
    glUniform1f(m_iAspectRatioUniformLocation, m_fAspectRatio);
    glUniform1f(m_iRadiusUniformLocation, m_fRadius);
    glUniform2fv(m_iCenterUniformLocation, 1, m_pCenter);
    return GPUImageFilter::beforeDrawExtra();
}


void GPUImagePixellatePositionFilter::setTextureSize(int width, int height)
{
    GPUImageFilter::setTextureSize(width, height);
    updateAspectRatio();
}

void GPUImagePixellatePositionFilter::setTextureRotation(Rotation rotation)
{
    GPUImageFilter::setTextureRotation(rotation);
    updateAspectRatio();
}

void GPUImagePixellatePositionFilter::updateAspectRatio()
{
    if(0 == m_iTextureWidth){
        m_iTextureWidth = 1280;
    }

    if(0 == m_iTextureHeight){
        m_iTextureHeight = 720;
    }

    m_fAspectRatio = isRotationSwapWidthAndHeight() ?
                     1.0 * m_iTextureHeight / m_iTextureWidth :
                     1.0 * m_iTextureWidth / m_iTextureHeight;
}


