/**
 * Created by lvHiei on 17-4-1.
 * This is a project of GPUImage implemented with c++, you can
 * use it free. if you find some bug please send me a email.
 * My Email is majun_1523@163.com.
 */


#include "GPUImageSaturationFilter.h"


#ifdef __GLSL_SUPPORT_HIGHP__

// 片元着色器
extern const char _saturation_fragment_shader[]=
"varying highp vec2 textureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"uniform lowp float saturation;\n"
"\n"
"// Values from \"Graphics Shaders: Theory and Practice\" by Bailey and Cunningham\n"
"const mediump vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);\n"
"\n"
"void main()\n"
"{\n"
"    lowp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
"    lowp float luminance = dot(textureColor.rgb, luminanceWeighting);\n"
"    lowp vec3 greyScaleColor = vec3(luminance);\n"
"\n"
"    gl_FragColor = vec4(mix(greyScaleColor, textureColor.rgb, saturation), textureColor.w);\n"
"\n"
"}"
;

#else

// 片元着色器
extern const char _saturation_fragment_shader[]=
"precision mediump float;\n"
"uniform sampler2D inputImageTexture;\n"
"varying vec2 textureCoordinate;\n"
"uniform lowp float saturation;\n"

"void main()\n"
"{\n"
"	vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n "
"   // saturation filter\n"
"   const mediump vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);\n"
"   lowp float luminance = dot(textureColor.rgb, luminanceWeighting);\n"
"   lowp vec3 greyScaleColor = vec3(luminance);\n"
"   vec4 saturationColor = vec4(mix(greyScaleColor, textureColor.rgb, saturation), textureColor.a);\n"
"   gl_FragColor = saturationColor;\n"
"}\n"
;

#endif




GPUImageSaturationFilter::GPUImageSaturationFilter()
        : GPUImageFilter(_saturation_fragment_shader)
{
    m_fSaturation = 1.0f;
}

GPUImageSaturationFilter::~GPUImageSaturationFilter()
{

}

void GPUImageSaturationFilter::setSaturation(int percent)
{
    float max = 2.0f;
    float min = 0.0f;

    if(percent < 0){
        percent = 0;
    }

    if(percent > 100){
        percent = 100;
    }

    float incremental = (max - min) / 100;

    float value = min + incremental * percent;
    setSaturation(value);
}

bool GPUImageSaturationFilter::createProgramExtra()
{
    m_iSaturationLocation = glGetUniformLocation(m_uProgram, "saturation");
    return true;
}

bool GPUImageSaturationFilter::beforeDrawExtra()
{
    glUniform1f(m_iSaturationLocation, m_fSaturation);
    return true;
}

bool GPUImageSaturationFilter::onDrawExtra()
{
    return true;
}


void GPUImageSaturationFilter::setSaturation(float saturation)
{
    m_fSaturation = saturation;
}

