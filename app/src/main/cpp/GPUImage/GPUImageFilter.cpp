/**
 * Created by lvHiei on 17-4-1.
 * This is a project of GPUImage implemented with c++, you can
 * use it free. if you find some bug please send me a email.
 * My Email is majun_1523@163.com.
 */


#include "GPUImageFilter.h"


// 顶点着色器
extern const char _base_vertex_shader[]=
//"precision mediump float;\n"
"attribute vec4 position;\n"
"attribute vec4 inputTextureCoordinate;\n"

"varying vec2 textureCoordinate;\n"

"void main()\n"
"{\n"
"	gl_Position = position;\n"
"	textureCoordinate = inputTextureCoordinate.xy;\n"
"}\n"
;

#ifdef __GLSL_SUPPORT_HIGHP__

// 片元着色器
extern const char _base_fragment_shader[]=
"varying highp vec2 textureCoordinate;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"void main()\n"
"{\n"
"    gl_FragColor = texture2D(inputImageTexture, textureCoordinate);\n"
"}"
;

#else

// 片元着色器
extern const char _base_fragment_shader[]=
"precision mediump float;\n"
"uniform sampler2D inputImageTexture;\n"
"varying vec2 textureCoordinate;\n"

"void main()\n"
"{\n"
"	vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n "
"   gl_FragColor = textureColor;\n"
"}\n"
;

#endif





GPUImageFilter::GPUImageFilter()
{
    init(_base_vertex_shader, _base_fragment_shader);
}

GPUImageFilter::GPUImageFilter(const char *fragment)
{
    init(_base_vertex_shader, fragment);
}

GPUImageFilter::GPUImageFilter(const char *vertex, const char *fragment)
{
    init(vertex, fragment);
}


GPUImageFilter::~GPUImageFilter()
{
    this->release();
    m_uProgram = 0;
    m_uPositionLocation = 0;
    m_uTextureCoordLocation = 0;
}

bool GPUImageFilter::init(const char *vertex, const char *fragment)
{
    m_uProgram = 0;
    m_uPositionLocation = 0;
    m_uTextureCoordLocation = 0;

    m_uVertexShader = 0;
    m_uFragmentShader = 0;

    m_uTextureId = 0;
    m_iTextureWidth = 0;
    m_iTextureHeight = 0;

    m_pVertexCoordinate = (float *) malloc(sizeof(float) * VERTEX_COORDINATE_SIZE);
    m_pTextureCoordinate = (float *) malloc(sizeof(float) * TEXTURE_COORDINATE_SIZE);

    m_pVertexShader = NULL;
    m_pFragmnetShader = NULL;

    uint32_t vertex_len = strlen(vertex) + 1;
    uint32_t fragment_len = strlen(fragment) + 1;

    m_pVertexShader = (char *) malloc(vertex_len);
    m_pFragmnetShader = (char *) malloc(fragment_len);

    strcpy(m_pVertexShader, vertex);
    strcpy(m_pFragmnetShader, fragment);

    return true;
}

bool GPUImageFilter::createProgram()
{
    int vertexLen = 2048;
    int fragmentLen = 8196;
    char vertex[2048];
    char fragment[8196];

    this->createVertexShader(vertex, vertexLen);
    this->createFragmentShader(fragment, fragmentLen);

    const char*vertex_shader = vertex;
    const char*fragment_shader = fragment;

    //    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    // 清除颜色
    glClearColor(0.0, 0.0, 0.0, 0.0);

    //编译顶点着色器
    m_uVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_uVertexShader,1,&vertex_shader,NULL);
    glCompileShader(m_uVertexShader);
    GLint compile_status = -20;
    glGetShaderiv( m_uVertexShader, GL_COMPILE_STATUS,  &compile_status);
    if(GL_TRUE != compile_status )
    {
        GLsizei length = 0;
        GLchar infoLog[1024];
        glGetShaderInfoLog(m_uVertexShader,
                           1024,
                           &length,
                           infoLog);
        LOGE("GPUImageFilter : compile vertexShader failed : error msg = %s", infoLog);

        return false;
    }

    // 编译片元着色器
    m_uFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_uFragmentShader,1,&fragment_shader,NULL);
    glCompileShader(m_uFragmentShader);
    glGetShaderiv( m_uFragmentShader, GL_COMPILE_STATUS,  &compile_status);
    if(GL_TRUE != compile_status )
    {
        GLsizei length = 0;
        GLchar infoLog[1024];
        glGetShaderInfoLog(m_uFragmentShader,
                           1024,
                           &length,
                           infoLog);
        LOGE("GPUImageFilter : compile fragmentShader failed : error msg = %s", infoLog);
        return false;
    }

    // 创建应用程序，链接着色器
    m_uProgram = glCreateProgram();
    glAttachShader(m_uProgram, m_uVertexShader);
    glAttachShader(m_uProgram, m_uFragmentShader);
    glLinkProgram(m_uProgram);
//    glUseProgram(m_uProgram);

    m_uPositionLocation = glGetAttribLocation(m_uProgram, "position");
    m_uTextureCoordLocation = glGetAttribLocation(m_uProgram, "inputTextureCoordinate");
    m_iInputTextureUniformLocation = glGetUniformLocation(m_uProgram, "inputImageTexture");
    this->createProgramExtra();

    return !checkGLError("createProgram");
}

bool GPUImageFilter::createProgramExtra()
{
    return true;
}

void GPUImageFilter::setVertexCoordinate(float *vertexCoordinate)
{
    if(!vertexCoordinate){
        return;
    }

    memcpy(m_pVertexCoordinate, vertexCoordinate, sizeof(float)*VERTEX_COORDINATE_SIZE);
}

void GPUImageFilter::setTextureCoordinate(float *textureCoordinate)
{
    if(!textureCoordinate){
        return;
    }

    memcpy(m_pTextureCoordinate, textureCoordinate, sizeof(float)*TEXTURE_COORDINATE_SIZE);
}

bool GPUImageFilter::createVertexShader(char *vertex, int &length)
{
    int expLen = strlen(m_pVertexShader);

    if(!vertex || length < expLen){
        LOGE("createVertexShader failed! vertex:%p,length:$d,expLen:%d", vertex, length, expLen);
        return false;
    }

    strcpy(vertex, m_pVertexShader);
    length = expLen;

    return true;
}

bool GPUImageFilter::createFragmentShader(char *fragment, int &length)
{
    int expLen = strlen(m_pFragmnetShader);

    if(!fragment || length < expLen){
        LOGE("createFragmentShader failed! fragment:%p,length:$d,expLen:%d", fragment, length, expLen);
        return false;
    }

    strcpy(fragment, m_pFragmnetShader);
    length = expLen;
    return true;
}

bool GPUImageFilter::draw(GLuint textureId, int viewWidth, int viewHeight, GLuint frameBufferId)
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glUseProgram(m_uProgram);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    this->beforeDraw();

    glActiveTexture(GL_TEXTURE0);
    glViewport(0 ,0 , viewWidth, viewHeight);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(m_iInputTextureUniformLocation, 0);


    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();

    this->onDraw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return !checkGLError("draw");
}

bool GPUImageFilter::beforeDraw()
{
    glVertexAttribPointer(m_uPositionLocation, 2, GL_FLOAT, GL_FALSE, 0, m_pVertexCoordinate);
    glEnableVertexAttribArray(m_uPositionLocation);

    glVertexAttribPointer(m_uTextureCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, m_pTextureCoordinate);
    glEnableVertexAttribArray(m_uTextureCoordLocation);

    this->beforeDrawExtra();

    return true;
}

bool GPUImageFilter::onDraw()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(m_uPositionLocation);
    glDisableVertexAttribArray(m_uTextureCoordLocation);

    this->onDrawExtra();

    return true;
}

bool GPUImageFilter::beforeDrawExtra()
{
    return true;
}

bool GPUImageFilter::onDrawExtra()
{
    return true;
}

bool GPUImageFilter::release()
{
    if(NULL != m_pVertexCoordinate){
        free(m_pVertexCoordinate);
        m_pVertexCoordinate = NULL;
    }

    if(NULL != m_pTextureCoordinate){
        free(m_pTextureCoordinate);
        m_pTextureCoordinate = NULL;
    }


    if(m_uVertexShader != 0){
        glDetachShader(m_uProgram, m_uVertexShader);
        glDeleteShader(m_uVertexShader);
        m_uVertexShader = 0;
    }

    if(m_uFragmentShader != 0){
        glDetachShader(m_uProgram, m_uFragmentShader);
        glDeleteShader(m_uFragmentShader);
        m_uFragmentShader = 0;
    }

    if(m_uProgram != 0){
        glDeleteProgram(m_uProgram);
        m_uProgram = 0;
    }

    if(m_uTextureId != 0){
        glDeleteTextures(1, &m_uTextureId);
        m_uTextureId = 0;
    }

    if(NULL != m_pVertexShader){
        free(m_pVertexShader);
        m_pVertexShader = NULL;
    }

    if(NULL != m_pFragmnetShader){
        free(m_pFragmnetShader);
        m_pFragmnetShader = NULL;
    }

    LOGI("GPUImageFilter::released");
    return true;
}

bool GPUImageFilter::createTexture(int textureWidth, int textureHeight)
{
    m_iTextureWidth = textureWidth;
    m_iTextureHeight = textureHeight;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_uTextureId);
    glBindTexture(GL_TEXTURE_2D, m_uTextureId);
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iTextureWidth, m_iTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);

    return checkGLError("createTexture");
}

bool GPUImageFilter::draw(GLubyte *rgbaData, int viewWidth, int viewHeight)
{
    glUseProgram(m_uProgram);

    glViewport(0 ,0 , viewWidth, viewHeight);

    this->beforeDraw();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uTextureId);


    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    0,
                    0,
                    m_iTextureWidth,
                    m_iTextureHeight,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    rgbaData);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();

    this->onDraw();

    return checkGLError("draw");
}

bool GPUImageFilter::checkGLError(const char* funName)
{
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGE("GL error after %s(): 0x%08x\n", funName, err);
        return true;
    }
    return false;
}


void GPUImageFilter::setTextureSize(int width, int height)
{
    m_iTextureWidth = width;
    m_iTextureHeight = height;
}

void GPUImageFilter::setTextureRotation(Rotation rotation)
{
    m_eRotation = rotation;
}

bool GPUImageFilter::isRotationSwapWidthAndHeight()
{
    return ROTATION_90 == m_eRotation || ROTATION_270 == m_eRotation;
}

