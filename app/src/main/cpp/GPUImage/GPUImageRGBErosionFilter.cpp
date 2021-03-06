/**
* Created by lvHiei on 17-5-3.
* This is a project of GPUImage implemented with c++, you can
* use it free. if you find some bug please send me a email.
* My Email is majun_1523@163.com.
*/

#include "GPUImageRGBErosionFilter.h"


extern const char _dilationRadiusOne_vertex_shader[];
extern const char _dilationRadiusTwo_vertex_shader[];
extern const char _dilationRadiusThree_vertex_shader[];
extern const char _dilationRadiusFour_vertex_shader[];

#ifdef __GLSL_SUPPORT_HIGHP__


// 片元着色器
extern const char _rgbErosionRadiusOne_fragment_shader[]=
"precision highp float;\n"
"\n"
"varying vec2 centerTextureCoordinate;\n"
"varying vec2 oneStepPositiveTextureCoordinate;\n"
"varying vec2 oneStepNegativeTextureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"void main()\n"
"{\n"
"    lowp vec4 centerIntensity = texture2D(inputImageTexture, centerTextureCoordinate);\n"
"    lowp vec4 oneStepPositiveIntensity = texture2D(inputImageTexture, oneStepPositiveTextureCoordinate);\n"
"    lowp vec4 oneStepNegativeIntensity = texture2D(inputImageTexture, oneStepNegativeTextureCoordinate);\n"
"\n"
"    lowp vec4 minValue = min(centerIntensity, oneStepPositiveIntensity);\n"
"\n"
"    gl_FragColor = min(minValue, oneStepNegativeIntensity);\n"
"}"
;


// 片元着色器
extern const char _rgbErosionRadiusTwo_fragment_shader[]=
"precision highp float;\n"
"\n"
"varying vec2 centerTextureCoordinate;\n"
"varying vec2 oneStepPositiveTextureCoordinate;\n"
"varying vec2 oneStepNegativeTextureCoordinate;\n"
"varying vec2 twoStepsPositiveTextureCoordinate;\n"
"varying vec2 twoStepsNegativeTextureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"void main()\n"
"{\n"
"    lowp vec4 centerIntensity = texture2D(inputImageTexture, centerTextureCoordinate);\n"
"    lowp vec4 oneStepPositiveIntensity = texture2D(inputImageTexture, oneStepPositiveTextureCoordinate);\n"
"    lowp vec4 oneStepNegativeIntensity = texture2D(inputImageTexture, oneStepNegativeTextureCoordinate);\n"
"    lowp vec4 twoStepsPositiveIntensity = texture2D(inputImageTexture, twoStepsPositiveTextureCoordinate);\n"
"    lowp vec4 twoStepsNegativeIntensity = texture2D(inputImageTexture, twoStepsNegativeTextureCoordinate);\n"
"\n"
"    lowp vec4 minValue = min(centerIntensity, oneStepPositiveIntensity);\n"
"    minValue = min(minValue, oneStepNegativeIntensity);\n"
"    minValue = min(minValue, twoStepsPositiveIntensity);\n"
"\n"
"    gl_FragColor = min(minValue, twoStepsNegativeIntensity);\n"
"}"
;


// 片元着色器
extern const char _rgbErosionRadiusThree_fragment_shader[]=
"precision highp float;\n"
"\n"
"varying vec2 centerTextureCoordinate;\n"
"varying vec2 oneStepPositiveTextureCoordinate;\n"
"varying vec2 oneStepNegativeTextureCoordinate;\n"
"varying vec2 twoStepsPositiveTextureCoordinate;\n"
"varying vec2 twoStepsNegativeTextureCoordinate;\n"
"varying vec2 threeStepsPositiveTextureCoordinate;\n"
"varying vec2 threeStepsNegativeTextureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"void main()\n"
"{\n"
"    lowp vec4 centerIntensity = texture2D(inputImageTexture, centerTextureCoordinate);\n"
"    lowp vec4 oneStepPositiveIntensity = texture2D(inputImageTexture, oneStepPositiveTextureCoordinate);\n"
"    lowp vec4 oneStepNegativeIntensity = texture2D(inputImageTexture, oneStepNegativeTextureCoordinate);\n"
"    lowp vec4 twoStepsPositiveIntensity = texture2D(inputImageTexture, twoStepsPositiveTextureCoordinate);\n"
"    lowp vec4 twoStepsNegativeIntensity = texture2D(inputImageTexture, twoStepsNegativeTextureCoordinate);\n"
"    lowp vec4 threeStepsPositiveIntensity = texture2D(inputImageTexture, threeStepsPositiveTextureCoordinate);\n"
"    lowp vec4 threeStepsNegativeIntensity = texture2D(inputImageTexture, threeStepsNegativeTextureCoordinate);\n"
"\n"
"    lowp vec4 minValue = min(centerIntensity, oneStepPositiveIntensity);\n"
"    minValue = min(minValue, oneStepNegativeIntensity);\n"
"    minValue = min(minValue, twoStepsPositiveIntensity);\n"
"    minValue = min(minValue, twoStepsNegativeIntensity);\n"
"    minValue = min(minValue, threeStepsPositiveIntensity);\n"
"\n"
"    gl_FragColor = min(minValue, threeStepsNegativeIntensity);\n"
"}"
;


// 片元着色器
extern const char _rgbErosionRadiusFour_fragment_shader[]=
"precision highp float;\n"
"\n"
"varying vec2 centerTextureCoordinate;\n"
"varying vec2 oneStepPositiveTextureCoordinate;\n"
"varying vec2 oneStepNegativeTextureCoordinate;\n"
"varying vec2 twoStepsPositiveTextureCoordinate;\n"
"varying vec2 twoStepsNegativeTextureCoordinate;\n"
"varying vec2 threeStepsPositiveTextureCoordinate;\n"
"varying vec2 threeStepsNegativeTextureCoordinate;\n"
"varying vec2 fourStepsPositiveTextureCoordinate;\n"
"varying vec2 fourStepsNegativeTextureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"void main()\n"
"{\n"
"    lowp vec4 centerIntensity = texture2D(inputImageTexture, centerTextureCoordinate);\n"
"    lowp vec4 oneStepPositiveIntensity = texture2D(inputImageTexture, oneStepPositiveTextureCoordinate);\n"
"    lowp vec4 oneStepNegativeIntensity = texture2D(inputImageTexture, oneStepNegativeTextureCoordinate);\n"
"    lowp vec4 twoStepsPositiveIntensity = texture2D(inputImageTexture, twoStepsPositiveTextureCoordinate);\n"
"    lowp vec4 twoStepsNegativeIntensity = texture2D(inputImageTexture, twoStepsNegativeTextureCoordinate);\n"
"    lowp vec4 threeStepsPositiveIntensity = texture2D(inputImageTexture, threeStepsPositiveTextureCoordinate);\n"
"    lowp vec4 threeStepsNegativeIntensity = texture2D(inputImageTexture, threeStepsNegativeTextureCoordinate);\n"
"    lowp vec4 fourStepsPositiveIntensity = texture2D(inputImageTexture, fourStepsPositiveTextureCoordinate);\n"
"    lowp vec4 fourStepsNegativeIntensity = texture2D(inputImageTexture, fourStepsNegativeTextureCoordinate);\n"
"\n"
"    lowp vec4 minValue = min(centerIntensity, oneStepPositiveIntensity);\n"
"    minValue = min(minValue, oneStepNegativeIntensity);\n"
"    minValue = min(minValue, twoStepsPositiveIntensity);\n"
"    minValue = min(minValue, twoStepsNegativeIntensity);\n"
"    minValue = min(minValue, threeStepsPositiveIntensity);\n"
"    minValue = min(minValue, threeStepsNegativeIntensity);\n"
"    minValue = min(minValue, fourStepsPositiveIntensity);\n"
"\n"
"    gl_FragColor = min(minValue, fourStepsNegativeIntensity);\n"
"}"
;


#else


// 片元着色器
extern const char _rgbErosionRadiusOne_fragment_shader[]=
"precision mediump float;\n"
"varying vec2 centerTextureCoordinate;\n"
"varying vec2 oneStepPositiveTextureCoordinate;\n"
"varying vec2 oneStepNegativeTextureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 centerIntensity = texture2D(inputImageTexture, centerTextureCoordinate);\n"
"    vec4 oneStepPositiveIntensity = texture2D(inputImageTexture, oneStepPositiveTextureCoordinate);\n"
"    vec4 oneStepNegativeIntensity = texture2D(inputImageTexture, oneStepNegativeTextureCoordinate);\n"
"\n"
"    vec4 minValue = min(centerIntensity, oneStepPositiveIntensity);\n"
"\n"
"    gl_FragColor = min(minValue, oneStepNegativeIntensity);\n"
"}"
;


// 片元着色器
extern const char _rgbErosionRadiusTwo_fragment_shader[]=
"precision mediump float;\n"
"varying vec2 centerTextureCoordinate;\n"
"varying vec2 oneStepPositiveTextureCoordinate;\n"
"varying vec2 oneStepNegativeTextureCoordinate;\n"
"varying vec2 twoStepsPositiveTextureCoordinate;\n"
"varying vec2 twoStepsNegativeTextureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 centerIntensity = texture2D(inputImageTexture, centerTextureCoordinate);\n"
"    vec4 oneStepPositiveIntensity = texture2D(inputImageTexture, oneStepPositiveTextureCoordinate);\n"
"    vec4 oneStepNegativeIntensity = texture2D(inputImageTexture, oneStepNegativeTextureCoordinate);\n"
"    vec4 twoStepsPositiveIntensity = texture2D(inputImageTexture, twoStepsPositiveTextureCoordinate);\n"
"    vec4 twoStepsNegativeIntensity = texture2D(inputImageTexture, twoStepsNegativeTextureCoordinate);\n"
"\n"
"    vec4 minValue = min(centerIntensity, oneStepPositiveIntensity);\n"
"    minValue = min(minValue, oneStepNegativeIntensity);\n"
"    minValue = min(minValue, twoStepsPositiveIntensity);\n"
"\n"
"    gl_FragColor = min(minValue, twoStepsNegativeIntensity);\n"
"}"
;


// 片元着色器
extern const char _rgbErosionRadiusThree_fragment_shader[]=
"precision mediump float;\n"
"varying vec2 centerTextureCoordinate;\n"
"varying vec2 oneStepPositiveTextureCoordinate;\n"
"varying vec2 oneStepNegativeTextureCoordinate;\n"
"varying vec2 twoStepsPositiveTextureCoordinate;\n"
"varying vec2 twoStepsNegativeTextureCoordinate;\n"
"varying vec2 threeStepsPositiveTextureCoordinate;\n"
"varying vec2 threeStepsNegativeTextureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 centerIntensity = texture2D(inputImageTexture, centerTextureCoordinate);\n"
"    vec4 oneStepPositiveIntensity = texture2D(inputImageTexture, oneStepPositiveTextureCoordinate);\n"
"    vec4 oneStepNegativeIntensity = texture2D(inputImageTexture, oneStepNegativeTextureCoordinate);\n"
"    vec4 twoStepsPositiveIntensity = texture2D(inputImageTexture, twoStepsPositiveTextureCoordinate);\n"
"    vec4 twoStepsNegativeIntensity = texture2D(inputImageTexture, twoStepsNegativeTextureCoordinate);\n"
"    vec4 threeStepsPositiveIntensity = texture2D(inputImageTexture, threeStepsPositiveTextureCoordinate);\n"
"    vec4 threeStepsNegativeIntensity = texture2D(inputImageTexture, threeStepsNegativeTextureCoordinate);\n"
"\n"
"    vec4 minValue = min(centerIntensity, oneStepPositiveIntensity);\n"
"    minValue = min(minValue, oneStepNegativeIntensity);\n"
"    minValue = min(minValue, twoStepsPositiveIntensity);\n"
"    minValue = min(minValue, twoStepsNegativeIntensity);\n"
"    minValue = min(minValue, threeStepsPositiveIntensity);\n"
"\n"
"    gl_FragColor = min(minValue, threeStepsNegativeIntensity);\n"
"}"
;


// 片元着色器
extern const char _rgbErosionRadiusFour_fragment_shader[]=
"precision mediump float;\n"
"varying vec2 centerTextureCoordinate;\n"
"varying vec2 oneStepPositiveTextureCoordinate;\n"
"varying vec2 oneStepNegativeTextureCoordinate;\n"
"varying vec2 twoStepsPositiveTextureCoordinate;\n"
"varying vec2 twoStepsNegativeTextureCoordinate;\n"
"varying vec2 threeStepsPositiveTextureCoordinate;\n"
"varying vec2 threeStepsNegativeTextureCoordinate;\n"
"varying vec2 fourStepsPositiveTextureCoordinate;\n"
"varying vec2 fourStepsNegativeTextureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 centerIntensity = texture2D(inputImageTexture, centerTextureCoordinate);\n"
"    vec4 oneStepPositiveIntensity = texture2D(inputImageTexture, oneStepPositiveTextureCoordinate);\n"
"    vec4 oneStepNegativeIntensity = texture2D(inputImageTexture, oneStepNegativeTextureCoordinate);\n"
"    vec4 twoStepsPositiveIntensity = texture2D(inputImageTexture, twoStepsPositiveTextureCoordinate);\n"
"    vec4 twoStepsNegativeIntensity = texture2D(inputImageTexture, twoStepsNegativeTextureCoordinate);\n"
"    vec4 threeStepsPositiveIntensity = texture2D(inputImageTexture, threeStepsPositiveTextureCoordinate);\n"
"    vec4 threeStepsNegativeIntensity = texture2D(inputImageTexture, threeStepsNegativeTextureCoordinate);\n"
"    vec4 fourStepsPositiveIntensity = texture2D(inputImageTexture, fourStepsPositiveTextureCoordinate);\n"
"    vec4 fourStepsNegativeIntensity = texture2D(inputImageTexture, fourStepsNegativeTextureCoordinate);\n"
"\n"
"    vec4 minValue = min(centerIntensity, oneStepPositiveIntensity);\n"
"    minValue = min(minValue, oneStepNegativeIntensity);\n"
"    minValue = min(minValue, twoStepsPositiveIntensity);\n"
"    minValue = min(minValue, twoStepsNegativeIntensity);\n"
"    minValue = min(minValue, threeStepsPositiveIntensity);\n"
"    minValue = min(minValue, threeStepsNegativeIntensity);\n"
"    minValue = min(minValue, fourStepsPositiveIntensity);\n"
"\n"
"    gl_FragColor = min(minValue, fourStepsNegativeIntensity);\n"
"}"
;


#endif



GPUImageRGBErosionFilter::GPUImageRGBErosionFilter()
    : GPUImageTwoPassTextureSamplingFilter(_dilationRadiusOne_vertex_shader, _rgbErosionRadiusOne_fragment_shader, _dilationRadiusOne_vertex_shader, _rgbErosionRadiusOne_fragment_shader)
{

}

GPUImageRGBErosionFilter::GPUImageRGBErosionFilter(int radius)
    : GPUImageTwoPassTextureSamplingFilter(_dilationRadiusOne_vertex_shader, _rgbErosionRadiusOne_fragment_shader, _dilationRadiusOne_vertex_shader, _rgbErosionRadiusOne_fragment_shader)
{
    const char* vertexShaderForThisRadius = NULL;
    const char* fragmentShaderForThisRadius = NULL;
    switch (radius)
    {
        case 0:
        case 1:
        {
            vertexShaderForThisRadius = _dilationRadiusOne_vertex_shader;
            fragmentShaderForThisRadius = _rgbErosionRadiusOne_fragment_shader;
            break;
        }
        case 2:
        {
            vertexShaderForThisRadius = _dilationRadiusTwo_vertex_shader;
            fragmentShaderForThisRadius = _rgbErosionRadiusTwo_fragment_shader;
            break;
        }
        case 3:
        {
            vertexShaderForThisRadius = _dilationRadiusThree_vertex_shader;
            fragmentShaderForThisRadius = _rgbErosionRadiusThree_fragment_shader;
            break;
        }
        case 4:
        {
            vertexShaderForThisRadius = _dilationRadiusFour_vertex_shader;
            fragmentShaderForThisRadius = _rgbErosionRadiusFour_fragment_shader;
            break;
        }
        default:
        {
            vertexShaderForThisRadius = _dilationRadiusOne_vertex_shader;
            fragmentShaderForThisRadius = _rgbErosionRadiusOne_fragment_shader;
            break;
        }
    }

    resetShader(vertexShaderForThisRadius, fragmentShaderForThisRadius, vertexShaderForThisRadius, fragmentShaderForThisRadius);
}

GPUImageRGBErosionFilter::~GPUImageRGBErosionFilter() {

}

