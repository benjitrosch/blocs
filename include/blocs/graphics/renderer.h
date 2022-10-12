#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#define APIENTRY
#endif

#include <fstream>
#include <memory>
#include <sstream>
#include <stddef.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <stb_image.h>

#include <blocs/common.h>
#include <blocs/math/calc.h>
#include <blocs/math/vec.h>
#include <blocs/math/matrix.h>
#include <blocs/graphics/image.h>

#ifdef DEBUG
#define LOG_GL_DEBUG(msg)              \
    do                                 \
    {                                  \
        LOG_DEBUG("(OPENGL) " << msg); \
    } while (0)
#define LOG_GL_ERR(msg, fn, line)                                            \
    do                                                                       \
    {                                                                        \
        std::cerr << RED("(OPENGL) [" << fn << ", " << line << "]: " << msg) \
                  << "\n";                                                   \
        abort();                                                             \
    } while (0)
#else
#define LOG_GL_DEBUG(msg) \
    do                    \
    {                     \
    } while (0)
#define LOG_GL_ERR(msg) \
    do                  \
    {                   \
    } while (0)
#endif

#define GL_CALL(x)                     \
    do                                 \
    {                                  \
        gl_clear_errors();             \
        x;                             \
        gl_check_errors(#x, __LINE__); \
    } while (0)

// clang-format off
typedef ptrdiff_t                       GLintptr;
typedef ptrdiff_t                       GLsizeiptr;
typedef unsigned int                    GLenum;
typedef unsigned char                   GLboolean;
typedef unsigned int                    GLbitfield;
typedef void                            GLvoid;
typedef char                            GLchar;
typedef signed char                     GLbyte;
typedef unsigned char                   GLubyte;
typedef short                           GLshort;
typedef unsigned short                  GLushort;
typedef int                             GLint;
typedef unsigned int                    GLuint;
typedef int                             GLsizei;
typedef float                           GLfloat;
typedef float                           GLclampf;
typedef double                          GLdouble;
typedef double                          GLclampd;

#define GL_TRUE                         1
#define GL_FALSE                        0
#define GL_NONE                         0
#define GL_NO_ERROR                     0
#define GL_DONT_CARE                    0x1100
#define GL_ZERO                         0x0000
#define GL_ONE                          0x0001
#define GL_BYTE                         0x1400
#define GL_UNSIGNED_BYTE                0x1401
#define GL_SHORT                        0x1402
#define GL_UNSIGNED_SHORT               0x1403
#define GL_INT                          0x1404
#define GL_UNSIGNED_INT                 0x1405
#define GL_FLOAT                        0x1406
#define GL_HALF_FLOAT                   0x140B
#define GL_UNSIGNED_SHORT_4_4_4_4_REV   0x8365
#define GL_UNSIGNED_SHORT_5_5_5_1_REV   0x8366
#define GL_UNSIGNED_INT_2_10_10_10_REV  0x8368
#define GL_UNSIGNED_SHORT_5_6_5         0x8363
#define GL_UNSIGNED_INT_24_8            0x84FA
#define GL_BOOL                         0x8B56
#define GL_COMPILE_STATUS               0x8B81
#define GL_VENDOR                       0x1F00
#define GL_RENDERER                     0x1F01
#define GL_VERSION                      0x1F02
#define GL_EXTENSIONS                   0x1F03
#define GL_COLOR_BUFFER_BIT             0x4000
#define GL_DEPTH_BUFFER_BIT             0x0100
#define GL_STENCIL_BUFFER_BIT           0x0400
#define GL_SCISSOR_BOX                  0x0C10
#define GL_SCISSOR_TEST                 0x0C11
#define GL_DEPTH_TEST                   0x0B71
#define GL_STENCIL_TEST                 0x0B90
#define GL_LINE                         0x1B01
#define GL_FILL                         0x1B02
#define GL_CW                           0x0900
#define GL_CCW                          0x0901
#define GL_FRONT                        0x0404
#define GL_BACK                         0x0405
#define GL_FRONT_AND_BACK               0x0408
#define GL_CULL_FACE                    0x0B44
#define GL_TEXTURE_2D                   0x0DE1
#define GL_BLEND                        0x0BE2
#define GL_SRC_COLOR                    0x0300
#define GL_ONE_MINUS_SRC_COLOR          0x0301
#define GL_SRC_ALPHA                    0x0302
#define GL_ONE_MINUS_SRC_ALPHA          0x0303
#define GL_DST_ALPHA                    0x0304
#define GL_ONE_MINUS_DST_ALPHA          0x0305
#define GL_DST_COLOR                    0x0306
#define GL_ONE_MINUS_DST_COLOR          0x0307
#define GL_SRC_ALPHA_SATURATE           0x0308
#define GL_CONSTANT_COLOR               0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR     0x8002
#define GL_CONSTANT_ALPHA               0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA     0x8004
#define GL_SRC1_ALPHA                   0x8589
#define GL_SRC1_COLOR                   0x88F9
#define GL_ONE_MINUS_SRC1_COLOR         0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA         0x88FB
#define GL_MIN                          0x8007
#define GL_MAX                          0x8008
#define GL_FUNC_ADD                     0x8006
#define GL_FUNC_SUBTRACT                0x800A
#define GL_FUNC_REVERSE_SUBTRACT        0x800B
#define GL_NEVER                        0x0200
#define GL_LESS                         0x0201
#define GL_EQUAL                        0x0202
#define GL_LEQUAL                       0x0203
#define GL_GREATER                      0x0204
#define GL_NOTEQUAL                     0x0205
#define GL_GEQUAL                       0x0206
#define GL_ALWAYS                       0x0207
#define GL_INVERT                       0x150A
#define GL_KEEP                         0x1E00
#define GL_REPLACE                      0x1E01
#define GL_INCR                         0x1E02
#define GL_DECR                         0x1E03
#define GL_INCR_WRAP                    0x8507
#define GL_DECR_WRAP                    0x8508
#define GL_REPEAT                       0x2901
#define GL_CLAMP_TO_EDGE                0x812F
#define GL_MIRRORED_REPEAT              0x8370
#define GL_NEAREST                      0x2600
#define GL_LINEAR                       0x2601
#define GL_NEAREST_MIPMAP_NEAREST       0x2700
#define GL_NEAREST_MIPMAP_LINEAR        0x2702
#define GL_LINEAR_MIPMAP_NEAREST        0x2701
#define GL_LINEAR_MIPMAP_LINEAR         0x2703
#define GL_COLOR_ATTACHMENT0            0x8CE0
#define GL_DEPTH_ATTACHMENT             0x8D00
#define GL_STENCIL_ATTACHMENT           0x8D20
#define GL_DEPTH_STENCIL_ATTACHMENT     0x821A
#define GL_RED                          0x1903
#define GL_ALPHA                        0x1906
#define GL_RGB                          0x1907
#define GL_RGBA                         0x1908
#define GL_LUMINANCE                    0x1909
#define GL_RGB8                         0x8051
#define GL_RGBA8                        0x8058
#define GL_RGBA4                        0x8056
#define GL_RGB5_A1                      0x8057
#define GL_RGB10_A2_EXT                 0x8059
#define GL_RGBA16                       0x805B
#define GL_BGRA                         0x80E1
#define GL_RG                           0x8227
#define GL_RG8                          0x822B
#define GL_RG16                         0x822C
#define GL_R16F                         0x822D
#define GL_R32F                         0x822E
#define GL_RG16F                        0x822F
#define GL_RG32F                        0x8230
#define GL_RGBA32F                      0x8814
#define GL_RGBA16F                      0x881A
#define GL_TEXTURE_WRAP_S               0x2802
#define GL_TEXTURE_WRAP_T               0x2803
#define GL_TEXTURE_WRAP_R               0x8072
#define GL_TEXTURE_MAG_FILTER           0x2800
#define GL_TEXTURE_MIN_FILTER           0x2801
#define GL_PACK_ALIGNMENT               0x0D05
#define GL_UNPACK_ALIGNMENT             0x0CF5
#define GL_TEXTURE0                     0x84C0
#define GL_ARRAY_BUFFER                 0x8892
#define GL_ELEMENT_ARRAY_BUFFER         0x8893
#define GL_DYNAMIC_DRAW                 0x88E8
#define GL_STATIC_DRAW                  0x88E4
#define GL_FRAMEBUFFER                  0x8D40
#define GL_READ_FRAMEBUFFER             0x8CA8
#define GL_DRAW_FRAMEBUFFER             0x8CA9
#define GL_FRAMEBUFFER_BINDING          0x8CA6
#define GL_RENDERBUFFER                 0x8D41
#define GL_MAX_DRAW_BUFFERS             0x8824
#define GL_POINTS                       0x0000
#define GL_LINES                        0x0001
#define GL_LINE_STRIP                   0x0003
#define GL_TRIANGLES                    0x0004
#define GL_FRAGMENT_SHADER              0x8B30
#define GL_VERTEX_SHADER                0x8B31
#define GL_ACTIVE_UNIFORMS              0x8B86
#define GL_ACTIVE_ATTRIBUTES            0x8B89
#define GL_FLOAT_VEC2                   0x8B50
#define GL_FLOAT_VEC3                   0x8B51
#define GL_FLOAT_VEC4                   0x8B52
#define GL_SAMPLER_2D                   0x8B5E
#define GL_FLOAT_MAT3x2                 0x8B67
#define GL_FLOAT_MAT4                   0x8B5C
#define GL_UNSIGNED_INT_8_8_8_8_REV     0x8367
#define GL_FRAMEBUFFER_COMPLETE         0x8CD5

typedef void*           (*GLLoadFunc)(const char *name);
typedef void            (*DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
typedef const GLubyte*  (*GLGETSTRINGPROC)(GLenum name);
typedef const GLenum    (*GLGETERRORPROC)();
typedef void            (*GLFLUSHPROC)();
typedef void            (*GLENABLEPROC)(GLenum mode);
typedef void            (*GLDISABLEPROC)(GLenum mode);
typedef void            (*GLCLEARPROC)(GLenum mask);
typedef void            (*GLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
#ifndef __EMSCRIPTEN__
typedef void            (*GLCLEARDEPTHPROC)(GLdouble depth);
typedef void            (*GLDEPTHRANGEPROC)(GLdouble n, GLdouble f);
#endif
typedef void            (*GLCLEARSTENCILPROC)(GLint stencil);
typedef void            (*GLDEPTHMASKPROC)(GLboolean enabled);
typedef void            (*GLDEPTHFUNCPROC)(GLenum func);
typedef void            (*GLVIEWPORTPROC)(GLint x, GLint y, GLint width, GLint height);
typedef void            (*GLSCISSORPROC)(GLint x, GLint y, GLint width, GLint height);
typedef void            (*GLCULLFACEPROC)(GLenum mode);
typedef void            (*GLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void            (*GLBLENDEQUATIONPROC)(GLenum mode);
typedef void            (*GLBLENDFUNCSEPARATEPROC)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void            (*GLBLENDEQUATIONSEPARATEPROC)(GLenum modeRGB, GLenum modeAlpha);
typedef void            (*GLBLENDCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void            (*GLCOLORMASKPROC)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void            (*GLGETINTEGERVPROC)(GLenum name, GLint* data);
typedef void            (*GLGENTEXTURESPROC)(GLint n, void* textures);
typedef void            (*GLGENRENDERBUFFERSPROC)(GLint n, void* textures);
typedef void            (*GLGENFRAMEBUFFERSPROC)(GLint n, void* textures);
typedef const GLenum    (*GLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
typedef void            (*GLACTIVETEXTUREPROC)(GLuint id);
typedef void            (*GLBINDTEXTUREPROC)(GLenum target, GLuint id);
typedef void            (*GLBINDRENDERBUFFERPROC)(GLenum target, GLuint id);
typedef void            (*GLBINDFRAMEBUFFERPROC)(GLenum target, GLuint id);
typedef void            (*GLTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalFormat, GLint width, GLint height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void            (*GLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void            (*GLFRAMEBUFFERRENDERBUFFERPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void            (*GLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void            (*GLTEXPARAMETERIPROC)(GLenum target, GLenum name, GLint param);
typedef void            (*GLRENDERBUFFERSTORAGEPROC)(GLenum target, GLenum internalformat, GLint width, GLint height);
#ifndef __EMSCRIPTEN__
typedef void            (*GLGETTEXIMAGEPROC)(GLenum target, GLint level, GLenum format, GLenum type, void* data);
#endif
typedef void            (*GLDRAWELEMENTSPROC)(GLenum mode, GLint count, GLenum type, void* indices);
typedef void            (*GLDRAWELEMENTSINSTANCEDPROC)(GLenum mode, GLint count, GLenum type, void* indices, GLint amount);
typedef void            (*GLDELETETEXTURESPROC)(GLint n, GLuint* textures);
typedef void            (*GLDELETERENDERBUFFERSPROC)(GLint n, GLuint* renderbuffers);
typedef void            (*GLDELETEFRAMEBUFFERSPROC)(GLint n, GLuint* textures);
typedef void            (*GLGENVERTEXARRAYSPROC)(GLint n, GLuint* arrays);
typedef void            (*GLBINDVERTEXARRAYPROC)(GLuint id);
typedef void            (*GLGENBUFFERSPROC)(GLint n, GLuint* arrays);
typedef void            (*GLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void            (*GLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void            (*GLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
typedef void            (*GLDELETEBUFFERSPROC)(GLint n, GLuint* buffers);
typedef void            (*GLDELETEVERTEXARRAYSPROC)(GLint n, GLuint* arrays);
typedef void            (*GLENABLEVERTEXATTRIBARRAYPROC)(GLuint location);
typedef void            (*GLDISABLEVERTEXATTRIBARRAYPROC)(GLuint location);
typedef void            (*GLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLint stride, const void* pointer);
typedef void            (*GLVERTEXATTRIBDIVISORPROC)(GLuint index, GLuint divisor);
typedef GLuint          (*GLCREATESHADERPROC)(GLenum type);
typedef void            (*GLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void            (*GLDETACHSHADERPROC)(GLuint program, GLuint shader);
typedef void            (*GLDELETESHADERPROC)(GLuint shader);
typedef void            (*GLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
typedef void            (*GLCOMPILESHADERPROC)(GLuint shader);
typedef void            (*GLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* result);
typedef GLuint          (*GLCREATEPROGRAMPROC)();
typedef void            (*GLDELETEPROGRAMPROC)(GLuint program);
typedef void            (*GLLINKPROGRAMPROC)(GLuint program);
typedef void            (*GLVALIDATEPROGRAMPROC)(GLuint pipeline);
typedef void            (*GLUSEPROGRAMPROC)(GLuint program);
typedef GLint           (*GLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar* name);
typedef GLint           (*GLGETATTRIBLOCATIONPROC)(GLuint program, const GLchar* name);
typedef void            (*GLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void            (*GLUNIFORM2IPROC)(GLint location, GLint v0, GLint v1);
typedef void            (*GLUNIFORM3IPROC)(GLint location, GLint v0, GLint v1, GLint v2);
typedef void            (*GLUNIFORM4IPROC)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void            (*GLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void            (*GLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
typedef void            (*GLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void            (*GLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void            (*GLUNIFORM1IVPROC)(GLint location, GLsizei count, const GLint *value);
typedef void            (*GLUNIFORM2IVPROC)(GLint location, GLsizei count, const GLint *value);
typedef void            (*GLUNIFORM3IVPROC)(GLint location, GLsizei count, const GLint *value);
typedef void            (*GLUNIFORM4IVPROC)(GLint location, GLsizei count, const GLint *value);
typedef void            (*GLUNIFORM1FVPROC)(GLint location, GLsizei count, const GLfloat *value);
typedef void            (*GLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat *value);
typedef void            (*GLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat *value);
typedef void            (*GLUNIFORM4FVPROC)(GLint location, GLsizei count, const GLfloat *value);
typedef void            (*GLUNIFORMMATRIX4FVPROC)(GLint location, GLint count, GLboolean transpose, const GLfloat* value);

inline                  GLGETSTRINGPROC                 gl_glGetString;
inline                  GLGETERRORPROC                  gl_glGetError;
inline                  GLFLUSHPROC                     gl_glFlush;
inline                  GLENABLEPROC                    gl_glEnable;
inline                  GLDISABLEPROC                   gl_glDisable;
inline                  GLCLEARPROC                     gl_glClear;
inline                  GLCLEARCOLORPROC                gl_glClearColor;
#ifndef __EMSCRIPTEN__
inline                  GLCLEARDEPTHPROC                gl_glClearDepth;
inline                  GLDEPTHRANGEPROC                gl_glDepthRange;
#endif
inline                  GLCLEARSTENCILPROC              gl_glClearStencil;
inline                  GLDEPTHMASKPROC                 gl_glDepthMask;
inline                  GLDEPTHFUNCPROC                 gl_glDepthFunc;
inline                  GLVIEWPORTPROC                  gl_glViewport;
inline                  GLSCISSORPROC                   gl_glScissor;
inline                  GLCULLFACEPROC                  gl_glCullFace;
inline                  GLBLENDFUNCPROC                 gl_glBlendFunc;
inline                  GLBLENDEQUATIONPROC             gl_glBlendEquation;
inline                  GLBLENDFUNCSEPARATEPROC         gl_glBlendFuncSeparate;
inline                  GLBLENDEQUATIONSEPARATEPROC     gl_glBlendEquationSeparate;
inline                  GLBLENDCOLORPROC                gl_glBlendColor;
inline                  GLCOLORMASKPROC                 gl_glColorMask;
inline                  GLGETINTEGERVPROC               gl_glGetIntegerv;
inline                  GLGENTEXTURESPROC               gl_glGenTextures;
inline                  GLGENRENDERBUFFERSPROC          gl_glGenRenderbuffers;
inline                  GLGENFRAMEBUFFERSPROC           gl_glGenFramebuffers;
inline                  GLCHECKFRAMEBUFFERSTATUSPROC    gl_glCheckFramebufferStatus;
inline                  GLACTIVETEXTUREPROC             gl_glActiveTexture;
inline                  GLBINDTEXTUREPROC               gl_glBindTexture;
inline                  GLBINDRENDERBUFFERPROC          gl_glBindRenderbuffer;
inline                  GLBINDFRAMEBUFFERPROC           gl_glBindFramebuffer;
inline                  GLTEXIMAGE2DPROC                gl_glTexImage2D;
inline                  GLTEXSUBIMAGE2DPROC             gl_glTexSubImage2D;
inline                  GLFRAMEBUFFERRENDERBUFFERPROC   gl_glFramebufferRenderbuffer;
inline                  GLFRAMEBUFFERTEXTURE2DPROC      gl_glFramebufferTexture2D;
inline                  GLTEXPARAMETERIPROC             gl_glTexParameteri;
inline                  GLRENDERBUFFERSTORAGEPROC       gl_glRenderbufferStorage;
#ifndef __EMSCRIPTEN__
inline                  GLGETTEXIMAGEPROC               gl_glGetTexImage;
#endif
inline                  GLDRAWELEMENTSPROC              gl_glDrawElements;
inline                  GLDRAWELEMENTSINSTANCEDPROC     gl_glDrawElementsInstanced;
inline                  GLDELETETEXTURESPROC            gl_glDeleteTextures;
inline                  GLDELETERENDERBUFFERSPROC       gl_glDeleteRenderbuffers;
inline                  GLDELETEFRAMEBUFFERSPROC        gl_glDeleteFramebuffers;
inline                  GLGENVERTEXARRAYSPROC           gl_glGenVertexArrays;
inline                  GLBINDVERTEXARRAYPROC           gl_glBindVertexArray;
inline                  GLGENBUFFERSPROC                gl_glGenBuffers;
inline                  GLBINDBUFFERPROC                gl_glBindBuffer;
inline                  GLBUFFERDATAPROC                gl_glBufferData;
inline                  GLBUFFERSUBDATAPROC             gl_glBufferSubData;
inline                  GLDELETEBUFFERSPROC             gl_glDeleteBuffers;
inline                  GLDELETEVERTEXARRAYSPROC        gl_glDeleteVertexArrays;
inline                  GLENABLEVERTEXATTRIBARRAYPROC   gl_glEnableVertexAttribArray;
inline                  GLDISABLEVERTEXATTRIBARRAYPROC  gl_glDisableVertexAttribArray;
inline                  GLVERTEXATTRIBPOINTERPROC       gl_glVertexAttribPointer;
inline                  GLVERTEXATTRIBDIVISORPROC       gl_glVertexAttribDivisor;
inline                  GLCREATESHADERPROC              gl_glCreateShader;
inline                  GLATTACHSHADERPROC              gl_glAttachShader;
inline                  GLDETACHSHADERPROC              gl_glDetachShader;
inline                  GLDELETESHADERPROC              gl_glDeleteShader;
inline                  GLSHADERSOURCEPROC              gl_glShaderSource;
inline                  GLCOMPILESHADERPROC             gl_glCompileShader;
inline                  GLGETSHADERIVPROC               gl_glGetShaderiv;
inline                  GLCREATEPROGRAMPROC             gl_glCreateProgram;
inline                  GLDELETEPROGRAMPROC             gl_glDeleteProgram;
inline                  GLLINKPROGRAMPROC               gl_glLinkProgram;
inline                  GLVALIDATEPROGRAMPROC           gl_glValidateProgram;
inline                  GLUSEPROGRAMPROC                gl_glUseProgram;
inline                  GLGETUNIFORMLOCATIONPROC        gl_glGetUniformLocation;
inline                  GLGETATTRIBLOCATIONPROC         gl_glGetAttribLocation;
inline                  GLUNIFORM1IPROC                 gl_glUniform1i;
inline                  GLUNIFORM2IPROC                 gl_glUniform2i;
inline                  GLUNIFORM3IPROC                 gl_glUniform3i;
inline                  GLUNIFORM4IPROC                 gl_glUniform4i;
inline                  GLUNIFORM1FPROC                 gl_glUniform1f;
inline                  GLUNIFORM2FPROC                 gl_glUniform2f;
inline                  GLUNIFORM3FPROC                 gl_glUniform3f;
inline                  GLUNIFORM4FPROC                 gl_glUniform4f;
inline                  GLUNIFORM1IVPROC                gl_glUniform1iv;
inline                  GLUNIFORM2IVPROC                gl_glUniform2iv;
inline                  GLUNIFORM3IVPROC                gl_glUniform3iv;
inline                  GLUNIFORM4IVPROC                gl_glUniform4iv;
inline                  GLUNIFORM1FVPROC                gl_glUniform1fv;
inline                  GLUNIFORM2FVPROC                gl_glUniform2fv;
inline                  GLUNIFORM3FVPROC                gl_glUniform3fv;
inline                  GLUNIFORM4FVPROC                gl_glUniform4fv;
inline                  GLUNIFORMMATRIX4FVPROC          gl_glUniformMatrix4fv;
    
#define                 glGetString                     gl_glGetString
#define                 glGetError                      gl_glGetError
#define                 glFlush                         gl_glFlush
#define                 glEnable                        gl_glEnable
#define                 glDisable                       gl_glDisable
#define                 glClear                         gl_glClear
#define                 glClearColor                    gl_glClearColor
#ifndef __EMSCRIPTEN__
#define                 glClearDepth                    gl_glClearDepth
#define                 glDepthRange                    gl_glDepthRange
#endif
#define                 glClearStencil                  gl_glClearStencil
#define                 glDepthMask                     gl_glDepthMask
#define                 glDepthFunc                     gl_glDepthFunc
#define                 glViewport                      gl_glViewport
#define                 glScissor                       gl_glScissor
#define                 glCullFace                      gl_glCullFace
#define                 glBlendFunc                     gl_glBlendFunc
#define                 glBlendEquation                 gl_glBlendEquation
#define                 glBlendFuncSeparate             gl_glBlendFuncSeparate
#define                 glBlendEquationSeparate         gl_glBlendEquationSeparate
#define                 glBlendColor                    gl_glBlendColor
#define                 glColorMask                     gl_glColorMask
#define                 glGetIntegerv                   gl_glGetIntegerv
#define                 glGenTextures                   gl_glGenTextures
#define                 glGenRenderbuffers              gl_glGenRenderbuffers
#define                 glGenFramebuffers               gl_glGenFramebuffers
#define                 glCheckFramebufferStatus        gl_glCheckFramebufferStatus
#define                 glActiveTexture                 gl_glActiveTexture
#define                 glBindTexture                   gl_glBindTexture
#define                 glBindRenderbuffer              gl_glBindRenderbuffer
#define                 glBindFramebuffer               gl_glBindFramebuffer
#define                 glTexImage2D                    gl_glTexImage2D
#define                 glTexSubImage2D                 gl_glTexSubImage2D
#define                 glFramebufferRenderbuffer       gl_glFramebufferRenderbuffer
#define                 glFramebufferTexture2D          gl_glFramebufferTexture2D
#define                 glTexParameteri                 gl_glTexParameteri
#define                 glRenderbufferStorage           gl_glRenderbufferStorage
#ifndef __EMSCRIPTEN__
#define                 glGetTexImage                   gl_glGetTexImage
#endif
#define                 glDrawElements                  gl_glDrawElements
#define                 glDrawElementsInstanced         gl_glDrawElementsInstanced
#define                 glDeleteTextures                gl_glDeleteTextures
#define                 glDeleteRenderbuffers           gl_glDeleteRenderbuffers
#define                 glDeleteFramebuffers            gl_glDeleteFramebuffers
#define                 glGenVertexArrays               gl_glGenVertexArrays
#define                 glBindVertexArray               gl_glBindVertexArray
#define                 glGenBuffers                    gl_glGenBuffers
#define                 glBindBuffer                    gl_glBindBuffer
#define                 glBufferData                    gl_glBufferData
#define                 glBufferSubData                 gl_glBufferSubData
#define                 glDeleteBuffers                 gl_glDeleteBuffers
#define                 glDeleteVertexArrays            gl_glDeleteVertexArrays
#define                 glEnableVertexAttribArray       gl_glEnableVertexAttribArray
#define                 glDisableVertexAttribArray      gl_glDisableVertexAttribArray
#define                 glVertexAttribPointer           gl_glVertexAttribPointer
#define                 glVertexAttribDivisor           gl_glVertexAttribDivisor
#define                 glCreateShader                  gl_glCreateShader
#define                 glAttachShader                  gl_glAttachShader
#define                 glDetachShader                  gl_glDetachShader
#define                 glDeleteShader                  gl_glDeleteShader
#define                 glShaderSource                  gl_glShaderSource
#define                 glCompileShader                 gl_glCompileShader
#define                 glGetShaderiv                   gl_glGetShaderiv
#define                 glCreateProgram                 gl_glCreateProgram
#define                 glDeleteProgram                 gl_glDeleteProgram
#define                 glLinkProgram                   gl_glLinkProgram
#define                 glValidateProgram               gl_glValidateProgram
#define                 glUseProgram                    gl_glUseProgram
#define                 glGetUniformLocation            gl_glGetUniformLocation
#define                 glGetAttribLocation             gl_glGetAttribLocation
#define                 glUniform1i                     gl_glUniform1i
#define                 glUniform2i                     gl_glUniform2i
#define                 glUniform3i                     gl_glUniform3i
#define                 glUniform4i                     gl_glUniform4i
#define                 glUniform1f                     gl_glUniform1f
#define                 glUniform2f                     gl_glUniform2f
#define                 glUniform3f                     gl_glUniform3f
#define                 glUniform4f                     gl_glUniform4f
#define                 glUniform1iv                    gl_glUniform1iv
#define                 glUniform2iv                    gl_glUniform2iv
#define                 glUniform3iv                    gl_glUniform3iv
#define                 glUniform4iv                    gl_glUniform4iv
#define                 glUniform1fv                    gl_glUniform1fv
#define                 glUniform2fv                    gl_glUniform2fv
#define                 glUniform3fv                    gl_glUniform3fv
#define                 glUniform4fv                    gl_glUniform4fv
#define                 glUniformMatrix4fv              gl_glUniformMatrix4fv
// clang-format on

namespace blocs
{
    namespace renderer
    {
        inline void bind(GLLoadFunc func)
        {
            glGetString = (GLGETSTRINGPROC)func("glGetString");
            if (glGetString == NULL)
            {
                LOG_ERR("failed to bind openGL");
                return;
            }

            // clang-format off
            gl_glGetString                  = (GLGETSTRINGPROC)func("glGetString");
            gl_glGetError                   = (GLGETERRORPROC)func("glGetError");
            gl_glFlush                      = (GLFLUSHPROC)func("glFlush");
            gl_glEnable                     = (GLENABLEPROC)func("glEnable");
            gl_glDisable                    = (GLDISABLEPROC)func("glDisable");
            gl_glClear                      = (GLCLEARPROC)func("glClear");
            gl_glClearColor                 = (GLCLEARCOLORPROC)func("glClearColor");
#ifndef __EMSCRIPTEN__
            gl_glClearDepth                 = (GLCLEARDEPTHPROC)func("glClearDepth");
            gl_glDepthRange                 = (GLDEPTHRANGEPROC)func("glDepthRange");
#endif
            gl_glClearStencil               = (GLCLEARSTENCILPROC)func("glClearStencil");
            gl_glDepthMask                  = (GLDEPTHMASKPROC)func("glDepthMask");
            gl_glDepthFunc                  = (GLDEPTHFUNCPROC)func("glDepthFunc");
            gl_glViewport                   = (GLVIEWPORTPROC)func("glViewport");
            gl_glScissor                    = (GLSCISSORPROC)func("glScissor");
            gl_glCullFace                   = (GLCULLFACEPROC)func("glCullFace");
            gl_glBlendFunc                  = (GLBLENDFUNCPROC)func("glBlendFunc");
            gl_glBlendEquation              = (GLBLENDEQUATIONPROC)func("glBlendEquation");
            gl_glBlendFuncSeparate          = (GLBLENDFUNCSEPARATEPROC)func("glBlendFuncSeparate");
            gl_glBlendEquationSeparate      = (GLBLENDEQUATIONSEPARATEPROC)func("glBlendEquationSeparate");
            gl_glBlendColor                 = (GLBLENDCOLORPROC)func("glBlendColor");
            gl_glColorMask                  = (GLCOLORMASKPROC)func("glColorMask");
            gl_glGetIntegerv                = (GLGETINTEGERVPROC)func("glGetIntegerv");
            gl_glGenTextures                = (GLGENTEXTURESPROC)func("glGenTextures");
            gl_glGenRenderbuffers           = (GLGENRENDERBUFFERSPROC)func("glGenRenderbuffers");
            gl_glGenFramebuffers            = (GLGENFRAMEBUFFERSPROC)func("glGenFramebuffers");
            gl_glCheckFramebufferStatus     = (GLCHECKFRAMEBUFFERSTATUSPROC)func("glCheckFramebufferStatus");
            gl_glActiveTexture              = (GLACTIVETEXTUREPROC)func("glActiveTexture");
            gl_glBindTexture                = (GLBINDTEXTUREPROC)func("glBindTexture");
            gl_glBindRenderbuffer           = (GLBINDRENDERBUFFERPROC)func("glBindRenderbuffer");
            gl_glBindFramebuffer            = (GLBINDFRAMEBUFFERPROC)func("glBindFramebuffer");
            gl_glTexImage2D                 = (GLTEXIMAGE2DPROC)func("glTexImage2D");
            gl_glTexSubImage2D              = (GLTEXSUBIMAGE2DPROC)func("glTexSubImage2D");
            gl_glFramebufferRenderbuffer    = (GLFRAMEBUFFERRENDERBUFFERPROC)func("glFramebufferRenderbuffer");
            gl_glFramebufferTexture2D       = (GLFRAMEBUFFERTEXTURE2DPROC)func("glFramebufferTexture2D");
            gl_glTexParameteri              = (GLTEXPARAMETERIPROC)func("glTexParameteri");
            gl_glRenderbufferStorage        = (GLRENDERBUFFERSTORAGEPROC)func("glRenderbufferStorage");
#ifndef __EMSCRIPTEN__
            gl_glGetTexImage                = (GLGETTEXIMAGEPROC)func("glGetTexImage");
#endif
            gl_glDrawElements               = (GLDRAWELEMENTSPROC)func("glDrawElements");
            gl_glDrawElementsInstanced      = (GLDRAWELEMENTSINSTANCEDPROC)func("glDrawElementsInstanced");
            gl_glDeleteTextures             = (GLDELETETEXTURESPROC)func("glDeleteTextures");
            gl_glDeleteRenderbuffers        = (GLDELETERENDERBUFFERSPROC)func("glDeleteRenderbuffers");
            gl_glDeleteFramebuffers         = (GLDELETEFRAMEBUFFERSPROC)func("glDeleteFramebuffers");
            gl_glGenVertexArrays            = (GLGENVERTEXARRAYSPROC)func("glGenVertexArrays");
            gl_glBindVertexArray            = (GLBINDVERTEXARRAYPROC)func("glBindVertexArray");
            gl_glGenBuffers                 = (GLGENBUFFERSPROC)func("glGenBuffers");
            gl_glBindBuffer                 = (GLBINDBUFFERPROC)func("glBindBuffer");
            gl_glBufferData                 = (GLBUFFERDATAPROC)func("glBufferData");
            gl_glBufferSubData              = (GLBUFFERSUBDATAPROC)func("glBufferSubData");
            gl_glDeleteBuffers              = (GLDELETEBUFFERSPROC)func("glDeleteBuffers");
            gl_glDeleteVertexArrays         = (GLDELETEVERTEXARRAYSPROC)func("glDeleteVertexArrays");
            gl_glEnableVertexAttribArray    = (GLENABLEVERTEXATTRIBARRAYPROC)func("glEnableVertexAttribArray");
            gl_glDisableVertexAttribArray   = (GLDISABLEVERTEXATTRIBARRAYPROC)func("glDisableVertexAttribArray");
            gl_glVertexAttribPointer        = (GLVERTEXATTRIBPOINTERPROC)func("glVertexAttribPointer");
            gl_glVertexAttribDivisor        = (GLVERTEXATTRIBDIVISORPROC)func("glVertexAttribDivisor");
            gl_glCreateShader               = (GLCREATESHADERPROC)func("glCreateShader");
            gl_glAttachShader               = (GLATTACHSHADERPROC)func("glAttachShader");
            gl_glDetachShader               = (GLDETACHSHADERPROC)func("glDetachShader");
            gl_glDeleteShader               = (GLDELETESHADERPROC)func("glDeleteShader");
            gl_glShaderSource               = (GLSHADERSOURCEPROC)func("glShaderSource");
            gl_glCompileShader              = (GLCOMPILESHADERPROC)func("glCompileShader");
            gl_glGetShaderiv                = (GLGETSHADERIVPROC)func("glGetShaderiv");
            gl_glCreateProgram              = (GLCREATEPROGRAMPROC)func("glCreateProgram");
            gl_glDeleteProgram              = (GLDELETEPROGRAMPROC)func("glDeleteProgram");
            gl_glLinkProgram                = (GLLINKPROGRAMPROC)func("glLinkProgram");
            gl_glValidateProgram            = (GLVALIDATEPROGRAMPROC)func("glValidateProgram");
            gl_glUseProgram                 = (GLUSEPROGRAMPROC)func("glUseProgram");
            gl_glGetUniformLocation         = (GLGETUNIFORMLOCATIONPROC)func("glGetUniformLocation");
            gl_glGetAttribLocation          = (GLGETATTRIBLOCATIONPROC)func("glGetAttribLocation");
            gl_glUniform1i                  = (GLUNIFORM1IPROC)func("glUniform1i");
            gl_glUniform2i                  = (GLUNIFORM2IPROC)func("glUniform2i");
            gl_glUniform3i                  = (GLUNIFORM3IPROC)func("glUniform3i");
            gl_glUniform4i                  = (GLUNIFORM4IPROC)func("glUniform4i");
            gl_glUniform1f                  = (GLUNIFORM1FPROC)func("glUniform1f");
            gl_glUniform2f                  = (GLUNIFORM2FPROC)func("glUniform2f");
            gl_glUniform3f                  = (GLUNIFORM3FPROC)func("glUniform3f");
            gl_glUniform4f                  = (GLUNIFORM4FPROC)func("glUniform4f");
            gl_glUniform1iv                 = (GLUNIFORM1IVPROC)func("glUniform1iv");
            gl_glUniform2iv                 = (GLUNIFORM2IVPROC)func("glUniform2iv");
            gl_glUniform3iv                 = (GLUNIFORM3IVPROC)func("glUniform3iv");
            gl_glUniform4iv                 = (GLUNIFORM4IVPROC)func("glUniform4iv");
            gl_glUniform1fv                 = (GLUNIFORM1FVPROC)func("glUniform1fv");
            gl_glUniform2fv                 = (GLUNIFORM2FVPROC)func("glUniform2fv");
            gl_glUniform3fv                 = (GLUNIFORM3FVPROC)func("glUniform3fv");
            gl_glUniform4fv                 = (GLUNIFORM4FVPROC)func("glUniform4fv");
            gl_glUniformMatrix4fv           = (GLUNIFORMMATRIX4FVPROC)func("glUniformMatrix4fv");

#ifndef __EMSCRIPTEN__
            if (glClearDepth                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glClearDepth");
            if (glDepthRange                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDepthRange");
            if (glGetTexImage               == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glGetTexImage");
#endif
            if (glGetString                 == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glGetString");
            if (glGetError                  == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glGetError");
            if (glFlush                     == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glFlush");
            if (glEnable                    == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glEnable");
            if (glDisable                   == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDisable");
            if (glClear                     == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glClear");
            if (glClearColor                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glClearColor");
            if (glClearStencil              == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glClearStencil");
            if (glDepthMask                 == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDepthMask");
            if (glDepthFunc                 == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDepthFunc");
            if (glViewport                  == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glViewport");
            if (glScissor                   == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glScissor");
            if (glCullFace                  == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glCullFace");
            if (glBlendFunc                 == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glBlendFunc");
            if (glBlendEquation             == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glBlendEquation");
            if (glBlendFuncSeparate         == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glBlendFuncSeparate");
            if (glBlendEquationSeparate     == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glBlendEquationSeparate");
            if (glBlendColor                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glBlendColor");
            if (glColorMask                 == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glColorMask");
            if (glGetIntegerv               == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glGetIntegerv");
            if (glGenTextures               == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glGenTextures");
            if (glGenRenderbuffers          == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glGenRenderbuffers");
            if (glGenFramebuffers           == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glGenFramebuffers");
            if (glCheckFramebufferStatus    == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glCheckFramebufferStatus");
            if (glActiveTexture             == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glActiveTexture");
            if (glBindTexture               == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glBindTexture");
            if (glBindRenderbuffer          == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glBindRenderbuffer");
            if (glBindFramebuffer           == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glBindFramebuffer");
            if (glTexImage2D                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glTexImage2D");
            if (glTexSubImage2D             == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glTexSubImage2D");
            if (glFramebufferRenderbuffer   == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glFramebufferRenderbuffer");
            if (glFramebufferTexture2D      == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glFramebufferTexture2D");
            if (glTexParameteri             == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glTexParameteri");
            if (glRenderbufferStorage       == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glRenderbufferStorage ");
            if (glDrawElements              == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDrawElements");
            if (glDrawElementsInstanced     == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDrawElementsInstanced");
            if (glDeleteTextures            == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDeleteTextures");
            if (glDeleteRenderbuffers       == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDeleteRenderbuffers");
            if (glDeleteFramebuffers        == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDeleteFramebuffers");
            if (glGenVertexArrays           == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glGenVertexArrays");
            if (glBindVertexArray           == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glBindVertexArray");
            if (glGenBuffers                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glGenBuffers");
            if (glBindBuffer                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glBindBuffer");
            if (glBufferData                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glBufferData");
            if (glBufferSubData             == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glBufferSubData");
            if (glDeleteBuffers             == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDeleteBuffers");
            if (glDeleteVertexArrays        == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDeleteVertexArrays");
            if (glEnableVertexAttribArray   == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glEnableVertexAttribArray");
            if (glDisableVertexAttribArray  == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDisableVertexAttribArray");
            if (glVertexAttribPointer       == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glVertexAttribPointer");
            if (glVertexAttribDivisor       == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glVertexAttribDivisor");
            if (glCreateShader              == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glCreateShader");
            if (glAttachShader              == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glAttachShader");
            if (glDetachShader              == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDetachShader");
            if (glDeleteShader              == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDeleteShader");
            if (glShaderSource              == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glShaderSource");
            if (glCompileShader             == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glCompileShader");
            if (glGetShaderiv               == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glGetShaderiv");
            if (glCreateProgram             == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glCreateProgram");
            if (glDeleteProgram             == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glDeleteProgram");
            if (glLinkProgram               == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glLinkProgram");
            if (glValidateProgram           == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glValidateProgram");
            if (glUseProgram                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUseProgram");
            if (glGetUniformLocation        == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glGetUniformLocation");
            if (glGetAttribLocation         == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glGetAttribLocation");
            if (glUniform1i                 == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform1i");
            if (glUniform2i                 == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform2i");
            if (glUniform3i                 == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform3i");
            if (glUniform4i                 == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform4i");
            if (glUniform1f                 == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform1f");
            if (glUniform2f                 == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform2f");
            if (glUniform3f                 == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform3f");
            if (glUniform4f                 == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform4f");
            if (glUniform1iv                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform1iv");
            if (glUniform2iv                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform2iv");
            if (glUniform3iv                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform3iv");
            if (glUniform4iv                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform4iv");
            if (glUniform1fv                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform1fv");
            if (glUniform2fv                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform2fv");
            if (glUniform3fv                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform3fv");
            if (glUniform4fv                == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniform4fv");
            if (glUniformMatrix4fv          == nullptr) LOG_ERR("SDL_GL_GetProcAddress FAILED TO LOAD PROC ADDRESS: glUniformMatrix4fv");
            // clang-format on
        }

        inline void gl_clear_errors()
        {
            while (glGetError() != GL_NO_ERROR)
                ;
        }

        inline bool gl_check_errors(cstr fn, u32 line)
        {
            while (GLenum error = glGetError())
            {
#ifdef DEBUG
                LOG_GL_ERR(error, fn, line);
#endif
                return false;
            }
            return true;
        }

        using gl_id = u32;

        enum class AttributeType : u8
        {
            NONE = 0,
            BYTE,
            BOOL,
            INT,
            FLOAT,
            VEC2,
            VEC3,
            VEC4,
            MAT3,
            MAT4,
        };

        struct BufferAttribute
        {
            AttributeType type;
            str           name;
            bool          normalized;

            BufferAttribute()
                : type(AttributeType::NONE), name(""), normalized(false)
            {
            }

            BufferAttribute(
                AttributeType type, const str& name, bool normalized = false
            )
                : type(type), name(name), normalized(normalized)
            {
            }
        };

        using buffer_layout = std::vector<BufferAttribute>;

        namespace attributes
        {
            inline u32 get_size_of_type(AttributeType type)
            {
                switch (type)
                {
                    case AttributeType::NONE:
                        return 0;
                    case AttributeType::BYTE:
                        return 1;
                    case AttributeType::BOOL:
                        return 4;
                    case AttributeType::INT:
                        return 4;
                    case AttributeType::FLOAT:
                        return 4;
                    case AttributeType::VEC2:
                        return 4 * 2;
                    case AttributeType::VEC3:
                        return 4 * 3;
                    case AttributeType::VEC4:
                        return 4 * 4;
                    case AttributeType::MAT3:
                        return 4 * 3 * 3;
                    case AttributeType::MAT4:
                        return 4 * 4 * 4;
                }

                LOG_ERR("couldn't get size of unknown buffer type");
                return 0;
            }

            inline u32 get_type_count(AttributeType type)
            {
                switch (type)
                {
                    case AttributeType::NONE:
                        return 0;
                    case AttributeType::BYTE:
                        return 1;
                    case AttributeType::BOOL:
                        return 1;
                    case AttributeType::INT:
                        return 1;
                    case AttributeType::FLOAT:
                        return 1;
                    case AttributeType::VEC2:
                        return 2;
                    case AttributeType::VEC3:
                        return 3;
                    case AttributeType::VEC4:
                        return 4;
                    case AttributeType::MAT3:
                        return 3 * 3;
                    case AttributeType::MAT4:
                        return 4 * 4;
                }

                LOG_ERR("couldn't get count of unknown buffer type");
                return 0;
            }

            inline GLenum get_GL_type(AttributeType type)
            {
                switch (type)
                {
                    case AttributeType::NONE:
                        return GL_NONE;
                    case AttributeType::BYTE:
                        return GL_UNSIGNED_BYTE;
                    case AttributeType::BOOL:
                        return GL_BOOL;
                    case AttributeType::INT:
                        return GL_INT;
                    case AttributeType::FLOAT:
                        return GL_FLOAT;
                    case AttributeType::VEC2:
                        return GL_FLOAT;
                    case AttributeType::VEC3:
                        return GL_FLOAT;
                    case AttributeType::VEC4:
                        return GL_FLOAT;
                    case AttributeType::MAT3:
                        return GL_FLOAT;
                    case AttributeType::MAT4:
                        return GL_FLOAT;
                }

                LOG_ERR("couldn't get gl type of unknown buffer type");
                return 0;
            }

            inline u32 get_stride(const buffer_layout& layout)
            {
                u32 stride = 0;
                for (auto& attribute : layout)
                {
                    stride += attributes::get_size_of_type(attribute.type);
                }
                return stride;
            }

            inline size get_layout_count(const buffer_layout& layout)
            {
                size count = 0;
                for (u32 i = 0; i < layout.size(); i++)
                {
                    count += attributes::get_type_count(layout[i].type);
                }
                return count;
            }
        }

        namespace vertex_buffer
        {
            inline gl_id generate()
            {
                gl_id id;
                GL_CALL(glGenBuffers(1, &id));
                return id;
            }

            inline gl_id generate(const void* data, size size)
            {
                gl_id id;
                GL_CALL(glGenBuffers(1, &id));
                GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
                GL_CALL(
                    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)
                );
                return id;
            }

            inline gl_id generate(size max_size)
            {
                gl_id id;
                GL_CALL(glGenBuffers(1, &id));
                GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
                GL_CALL(glBufferData(
                    GL_ARRAY_BUFFER, max_size, nullptr, GL_DYNAMIC_DRAW
                ));
                return id;
            }

            inline void remove(gl_id id) { GL_CALL(glDeleteBuffers(1, &id)); }

            inline void bind(gl_id id)
            {
                GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
            }

            inline void unbind() { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

            inline void subdata(size size, void* data)
            {
                GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
            }

            inline void buffer_data(size max_size)
            {
                GL_CALL(glBufferData(
                    GL_ARRAY_BUFFER, max_size, nullptr, GL_DYNAMIC_DRAW
                ));
            }
        }

        namespace vertex_array
        {
            inline gl_id generate()
            {
                gl_id id = 0;
                GL_CALL(glGenVertexArrays(1, &id));
                return id;
            }

            inline void remove(gl_id id)
            {
                GL_CALL(glDeleteVertexArrays(1, &id));
            }

            inline void bind(gl_id id) { GL_CALL(glBindVertexArray(id)); }

            inline void unbind() { GL_CALL(glBindVertexArray(0)); }

            inline void add_buffer(
                const gl_id id, const gl_id vb, const buffer_layout& layout,
                i32 divisor = 0
            )
            {
                vertex_array::bind(id);
                vertex_buffer::bind(vb);

                uintptr_t offset = 0;
                for (u32 i = 0; i < layout.size(); i++)
                {
                    const auto& attribute = layout[i];

                    GL_CALL(glEnableVertexAttribArray(i));
                    GL_CALL(glVertexAttribPointer(
                        i, attributes::get_type_count(attribute.type),
                        attributes::get_GL_type(attribute.type),
                        attribute.normalized, attributes::get_stride(layout),
                        (const void*)offset
                    ));
                    GL_CALL(glVertexAttribDivisor(i, divisor));

                    offset += attributes::get_size_of_type(attribute.type);
                }
            }
        }

        namespace index_buffer
        {
            inline gl_id generate(const uint* data, size count)
            {
                gl_id id = 0;
                GL_CALL(glGenBuffers(1, &id));
                GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
                GL_CALL(glBufferData(
                    GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), data,
                    GL_STATIC_DRAW
                ));
                return id;
            }

            inline gl_id generate(size count)
            {
                gl_id id = 0;
                GL_CALL(glGenBuffers(1, &id));
                GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
                GL_CALL(glBufferData(
                    GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), nullptr,
                    GL_DYNAMIC_DRAW
                ));
                return id;
            }

            inline void remove(gl_id id) { GL_CALL(glDeleteBuffers(1, &id)); }

            inline void bind(gl_id id)
            {
                GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
            }

            inline void unbind()
            {
                GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            }

            inline void subdata(size size, void* data)
            {
                GL_CALL(glBufferSubData(
                    GL_ELEMENT_ARRAY_BUFFER, 0, size * sizeof(u32), data
                ));
            }

            inline void buffer_data(size max_size)
            {
                GL_CALL(glBufferData(
                    GL_ELEMENT_ARRAY_BUFFER, max_size * sizeof(u32), nullptr,
                    GL_DYNAMIC_DRAW
                ));
            }
        }

        namespace shaders
        {
            constexpr cstr DEFAULT_2D_VERTEX_SHADER =
#ifdef __EMSCRIPTEN__
                "#version 300 es\n"
#else
                "#version 330 core\n"
#endif
                "layout(location = 0) in vec3 a_pos;\n"
                "layout(location = 1) in vec2 a_tex;\n"
                "layout(location = 2) in vec4 a_color;\n"
                "layout(location = 3) in vec4 a_fill;\n"
                "out vec2 v_tex;\n"
                "out vec4 v_color;\n"
                "out vec4 v_fill;\n"
                "uniform mat4 u_world;\n"
                "uniform float u_flip;\n"
                "void main()\n"
                "{\n"
                "   v_tex = a_tex;\n"
                "   v_color = a_color;\n"
                "   v_fill = a_fill;\n"
                "   gl_Position = u_world * vec4(a_pos.xyz, 1.0);\n"
                "   gl_Position.y *= u_flip;\n"
                "}\n";

            constexpr cstr DEFAULT_2D_FRAGMENT_SHADER =
#ifdef __EMSCRIPTEN__
                "#version 300 es\n"
                "precision mediump float;\n"
#else
                "#version 330 core\n"
#endif
                "in vec2 v_tex;\n"
                "in vec4 v_color;\n"
                "in vec4 v_fill;\n"
                "out vec4 color;\n"
                "uniform sampler2D u_texture;\n"
                "void main()\n"
                "{\n"
                "   color = texture(u_texture, v_tex) * v_color + v_fill;\n"
                "}\n";

            namespace
            {
                struct ShaderProgramSource
                {
                    str vert;
                    str frag;
                };

                inline str parse(const str& filepath)
                {
                    std::ifstream stream(filepath);
                    if (stream)
                    {
                        str line;
                        ss  ss;

                        while (getline(stream, line))
                        {
#ifdef __EMSCRIPTEN__
                            if (line.find("#version 330") != str::npos)
                            {
                                ss << "#version 300 es\nprecision mediump "
                                      "float;\n";
                            }
                            else
                            {
                                ss << line << '\n';
                            }
#else
                            ss << line << '\n';
#endif
                        }

                        return ss.str();
                    }
                    else
                    {
                        LOG_ERR("could not open fragment shader file");
                    }

                    return "";
                }

                inline u32 compile(u32 type, const str& source)
                {
                    u32  id  = glCreateShader(type);
                    cstr src = source.c_str();
                    GL_CALL(glShaderSource(id, 1, &src, nullptr));
                    GL_CALL(glCompileShader(id));

                    return id;
                }
            }

            inline gl_id create_shader(const str& vert, const str& frag)
            {
                u32 compiled_vert = compile(GL_VERTEX_SHADER, vert);
                u32 compiled_frag = compile(GL_FRAGMENT_SHADER, frag);

                assert(
                    compiled_vert != -1 && "could not compile vertex shader"
                );
                assert(
                    compiled_frag != -1 && "could not compile fragment shader"
                );

                u32 program = glCreateProgram();

                GL_CALL(glAttachShader(program, compiled_vert));
                GL_CALL(glAttachShader(program, compiled_frag));
                GL_CALL(glLinkProgram(program));
                GL_CALL(glValidateProgram(program));

                GL_CALL(glDeleteShader(compiled_vert));
                GL_CALL(glDeleteShader(compiled_frag));

                return program;
            }

            inline gl_id generate(const str& vert, const str& frag)
            {
                return create_shader(vert, frag);
            }

            inline void remove(gl_id id) { GL_CALL(glDeleteProgram(id)); }

            inline void bind(gl_id id) { GL_CALL(glUseProgram(id)); }

            inline void unbind() { GL_CALL(glUseProgram(0)); }

            inline i32 get_uniform_location(gl_id id, const str& name)
            {
                GL_CALL(return glGetUniformLocation(id, name.c_str()));
            }

            inline void set_uniform(i32 location, i32 val)
            {
                GL_CALL(glUniform1i(location, val));
            }

            inline void set_uniform(i32 location, f32 val)
            {
                GL_CALL(glUniform1f(location, val));
            }

            inline void set_uniform(i32 location, vec2f vec)
            {
                GL_CALL(glUniform2f(location, vec.x, vec.y));
            }

            inline void set_uniform(i32 location, vec3f vec)
            {
                GL_CALL(glUniform3f(location, vec.x, vec.y, vec.z));
            }

            inline void set_uniform(i32 location, vec4f vec)
            {
                GL_CALL(glUniform4f(location, vec.x, vec.y, vec.z, vec.w));
            }

            inline void set_uniform(
                i32 location, const size count, const i32 vals[]
            )
            {
                GL_CALL(glUniform1iv(location, count, vals));
            }

            inline void set_uniform(
                i32 location, const size count, const vec2<i32> vals[]
            )
            {
                i32* flat_arr = new i32[count * 2];
                i32  i        = 0;
                for (i32 j = 0; j < count * 2; j += 2)
                {
                    flat_arr[j]     = vals[i].x;
                    flat_arr[j + 1] = vals[i].y;
                    i++;
                }

                GL_CALL(glUniform2iv(location, count, flat_arr));
            }

            inline void set_uniform(
                i32 location, const size count, const vec3<i32> vals[]
            )
            {
                i32* flat_arr = new i32[count * 3];
                i32  i        = 0;
                for (i32 j = 0; j < count * 2; j += 3)
                {
                    flat_arr[j]     = vals[i].x;
                    flat_arr[j + 1] = vals[i].y;
                    flat_arr[j + 2] = vals[i].z;
                    i++;
                }

                GL_CALL(glUniform3iv(location, count, flat_arr));
            }

            inline void set_uniform(
                i32 location, const size count, const vec4<i32> vals[]
            )
            {
                i32* flat_arr = new i32[count * 4];
                i32  i        = 0;
                for (i32 j = 0; j < count * 2; j += 4)
                {
                    flat_arr[j]     = vals[i].x;
                    flat_arr[j + 1] = vals[i].y;
                    flat_arr[j + 2] = vals[i].z;
                    flat_arr[j + 3] = vals[i].w;
                    i++;
                }

                GL_CALL(glUniform4iv(location, count, flat_arr));
            }

            inline void set_uniform(
                i32 location, const size count, const f32 vals[]
            )
            {
                GL_CALL(glUniform1fv(location, count, vals));
            }

            inline void set_uniform(
                i32 location, const size count, const vec2<f32> vals[]
            )
            {
                f32* flat_arr = new f32[count * 2];
                i32  i        = 0;
                for (i32 j = 0; j < count * 2; j += 2)
                {
                    flat_arr[j]     = vals[i].x;
                    flat_arr[j + 1] = vals[i].y;
                    i++;
                }

                GL_CALL(glUniform2fv(location, count, flat_arr));
            }

            inline void set_uniform(
                i32 location, const size count, const vec3<f32> vals[]
            )
            {
                f32* flat_arr = new f32[count * 3];
                i32  i        = 0;
                for (i32 j = 0; j < count * 2; j += 3)
                {
                    flat_arr[j]     = vals[i].x;
                    flat_arr[j + 1] = vals[i].y;
                    flat_arr[j + 2] = vals[i].z;
                    i++;
                }

                GL_CALL(glUniform3fv(location, count, flat_arr));
            }

            inline void set_uniform(
                i32 location, const size count, const vec4<f32> vals[]
            )
            {
                f32* flat_arr = new f32[count * 4];
                i32  i        = 0;
                for (i32 j = 0; j < count * 2; j += 4)
                {
                    flat_arr[j]     = vals[i].x;
                    flat_arr[j + 1] = vals[i].y;
                    flat_arr[j + 2] = vals[i].z;
                    flat_arr[j + 3] = vals[i].w;
                    i++;
                }

                GL_CALL(glUniform4fv(location, count, flat_arr));
            }

            inline void set_uniform(i32 location, mat4x4f mat)
            {
                GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]));
            }
        }

        namespace textures
        {
            inline gl_id generate(
                const str& filepath, i32& w, i32& h, bool aliased = false
            )
            {
                gl_id id;

                Image img = Image(filepath);
                w         = img.w;
                h         = img.h;

                GL_CALL(glGenTextures(1, &id));
                GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

                if (aliased)
                {
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR
                    ));
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR
                    ));
                }
                else
                {
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST
                    ));
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST
                    ));
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE
                    ));
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE
                    ));
                }

#ifdef __EMSCRIPTEN__
                GL_CALL(glTexImage2D(
                    GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                    GL_UNSIGNED_BYTE, img.pixels
                ));
#else
                GL_CALL(glTexImage2D(
                    GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                    GL_UNSIGNED_INT_8_8_8_8_REV, img.pixels
                ));
#endif

                return id;
            }

            inline gl_id generate(const str& filepath, bool aliased = false)
            {
                i32 w, h;
                return textures::generate(filepath, w, h, aliased);
            }

            inline gl_id generate(
                const void* pixels, const i32 w, const i32 h,
                bool aliased = false
            )
            {
                gl_id id;

                GL_CALL(glGenTextures(1, &id));
                GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

                if (aliased)
                {
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR
                    ));
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR
                    ));
                }
                else
                {
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST
                    ));
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST
                    ));
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE
                    ));
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE
                    ));
                }

                GL_CALL(glTexImage2D(
                    GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                    GL_UNSIGNED_BYTE, pixels
                ));

                return id;
            }

            inline void sub_data(
                const void* pixels, const i32 x, const i32 y, const i32 w,
                const i32 h
            )
            {
                GL_CALL(glTexSubImage2D(
                    GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE,
                    pixels
                ));
            }

            inline void remove(gl_id id) { GL_CALL(glDeleteTextures(1, &id)); }

            inline void bind(gl_id id, u32 slot = 0)
            {
                GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
                GL_CALL(glBindTexture(GL_TEXTURE_2D, id));
            }

            inline void unbind() { GL_CALL(glBindTexture(GL_TEXTURE_2D, 0)); }
        }

        namespace frame_buffer
        {
            inline gl_id generate(i32 w, i32 h)
            {
                gl_id id;
                GL_CALL(glGenFramebuffers(1, &id));

                if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
                    GL_FRAMEBUFFER_COMPLETE)
                {
                    LOG_ERR("failed to create frame buffer");
                }

                return id;
            }

            inline gl_id generate_render_texture(gl_id id, i32 w, i32 h)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, id);

                gl_id tex;

                GL_CALL(glGenTextures(1, &tex));
                GL_CALL(glBindTexture(GL_TEXTURE_2D, tex));

                GL_CALL(glTexParameteri(
                    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST
                ));
                GL_CALL(glTexParameteri(
                    GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST
                ));
                GL_CALL(glTexParameteri(
                    GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE
                ));
                GL_CALL(glTexParameteri(
                    GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE
                ));

                GL_CALL(glTexImage2D(
                    GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                    GL_UNSIGNED_BYTE, nullptr
                ));

                glBindTexture(GL_TEXTURE_2D, 0);
                GL_CALL(glFramebufferTexture2D(
                    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0
                ));

                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                return tex;
            }

            inline void generate_render_textures(
                gl_id id, const size count, gl_id* buffers, i32 w, i32 h
            )
            {
                glBindFramebuffer(GL_FRAMEBUFFER, id);

                GL_CALL(glGenTextures(count, &buffers));
                for (uint i = 0; i < count; i++)
                {
                    auto tex = buffers[i];

                    GL_CALL(glBindTexture(GL_TEXTURE_2D, tex));

                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST
                    ));
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST
                    ));
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE
                    ));
                    GL_CALL(glTexParameteri(
                        GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE
                    ));

                    GL_CALL(glTexImage2D(
                        GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, nullptr
                    ));

                    glBindTexture(GL_TEXTURE_2D, 0);
                    GL_CALL(glFramebufferTexture2D(
                        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                        tex, 0
                    ));
                }

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

            inline void resize_render_texture(gl_id id, i32 w, i32 h)
            {
                glBindTexture(GL_TEXTURE_2D, id);
                glTexImage2D(
                    GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                    GL_UNSIGNED_BYTE, nullptr
                );
            }

            inline void bind(gl_id id)
            {
                GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));
            }

            inline void unbind()
            {
                GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
            }

            inline bool is_bound()
            {
                GLint has_frame_buffer;
                GL_CALL(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &has_frame_buffer)
                );
                return has_frame_buffer != 0;
            }

            inline void remove(gl_id id) { glDeleteFramebuffers(1, &id); }
        }

        inline void clear(const color& color = color::black, bool depth = false)
        {
            const vec4 color_vec = color.to_vec4();
            glClearColor(color_vec.x, color_vec.y, color_vec.z, color_vec.w);
#ifndef __EMSCRIPTEN__
            if (bool depth = false) glClearDepth(1.0f);
#endif
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        }

        inline vec4i get_scissorbox()
        {
            GLint size[4] = {0};
            GL_CALL(glGetIntegerv(GL_SCISSOR_BOX, size));

            return {size[0], size[1], size[2], size[3]};
        }

        inline vec4i get_viewport(
            i32 game_w, i32 game_h, i32 screen_w, i32 screen_h
        )
        {
            f32 ratio = game_w / (f32)game_h;

            i32 w = screen_w;
            i32 h = (i32)(screen_w / ratio + 0.5f);

            if (h > screen_h)
            {
                h = screen_h;
                w = (i32)(screen_h * ratio + 0.5f);
            }

            i32 x = (screen_w / 2) - (w / 2);
            i32 y = (screen_h / 2) - (h / 2);

            return {x, y, w, h};
        }

        inline void viewport(i32 x, i32 y, i32 w, i32 h)
        {
            glViewport(x, y, w, h);
        }

        enum class BlendFunction : GLenum
        {
            ZERO                     = GL_ZERO,
            ONE                      = GL_ONE,
            SRC_COLOR                = GL_SRC_COLOR,
            ONE_MINUS_SRC_COLOR      = GL_ONE_MINUS_SRC_COLOR,
            DST_COLOR                = GL_DST_COLOR,
            ONE_MINUS_DST_COLOR      = GL_ONE_MINUS_DST_COLOR,
            SRC_ALPHA                = GL_SRC_ALPHA,
            ONE_MINUS_SRC_ALPHA      = GL_ONE_MINUS_SRC_ALPHA,
            DST_ALPHA                = GL_DST_ALPHA,
            ONE_MINUS_DST_ALPHA      = GL_ONE_MINUS_DST_ALPHA,
            CONSTANT_COLOR           = GL_CONSTANT_COLOR,
            ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
            CONSTANT_ALPHA           = GL_CONSTANT_ALPHA,
            ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
            SRC_ALPHA_SATURATE       = GL_SRC_ALPHA_SATURATE,
            SRC1_COLOR               = GL_SRC1_COLOR,
            ONE_MINUS_SRC1_COLOR     = GL_ONE_MINUS_SRC1_COLOR,
            SRC1_ALPHA               = GL_SRC1_ALPHA,
            ONE_MINUS_SRC1_ALPHA     = GL_ONE_MINUS_SRC1_ALPHA
        };

        enum class BlendEquation : GLenum
        {
            ADD              = GL_FUNC_ADD,
            SUBTRACT         = GL_FUNC_SUBTRACT,
            REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
            MIN              = GL_MIN,
            MAX              = GL_MAX,
        };

        enum class BlendMask : uchar
        {
            NONE  = 0,
            RED   = 1,
            GREEN = 2,
            BLUE  = 4,
            ALPHA = 8,
            RGB   = RED | GREEN | BLUE,
            RGBA  = RED | GREEN | BLUE | ALPHA,
        };

        struct BlendMode
        {
            static const BlendMode Normal;
            static const BlendMode NonPremultiplied;
            static const BlendMode Subtract;
            static const BlendMode Additive;

            BlendEquation color_eq;
            BlendFunction color_src;
            BlendFunction color_dst;
            BlendEquation alpha_eq;
            BlendFunction alpha_src;
            BlendFunction alpha_dst;

            BlendMask mask;
        };

        inline const BlendMode BlendMode::Normal = {
            BlendEquation::ADD,
            BlendFunction::ONE,
            BlendFunction::ONE_MINUS_SRC_ALPHA,
            BlendEquation::ADD,
            BlendFunction::ONE,
            BlendFunction::ONE_MINUS_SRC_ALPHA,

            BlendMask::RGBA};

        inline const BlendMode BlendMode::NonPremultiplied = {
            BlendEquation::ADD,
            BlendFunction::SRC_ALPHA,
            BlendFunction::ONE_MINUS_SRC_ALPHA,
            BlendEquation::ADD,
            BlendFunction::SRC_ALPHA,
            BlendFunction::ONE_MINUS_SRC_ALPHA,

            BlendMask::RGBA};

        inline const BlendMode BlendMode::Subtract = {
            BlendEquation::REVERSE_SUBTRACT,
            BlendFunction::ONE,
            BlendFunction::ONE,
            BlendEquation::ADD,
            BlendFunction::ONE,
            BlendFunction::ONE,

            BlendMask::RGBA};

        inline const BlendMode BlendMode::Additive = {
            BlendEquation::ADD, BlendFunction::SRC_ALPHA, BlendFunction::ONE,
            BlendEquation::ADD, BlendFunction::SRC_ALPHA, BlendFunction::ONE,

            BlendMask::RGBA};

        inline void blend(GLenum src, GLenum dst)
        {
            if (((src != GL_ZERO && src != GL_ONE) &&
                 (src < GL_SRC_COLOR || src > GL_ONE_MINUS_SRC1_ALPHA)) ||
                ((dst != GL_ZERO && dst != GL_ONE) &&
                 (dst < GL_SRC_COLOR || dst > GL_ONE_MINUS_SRC1_ALPHA)))
            {
                LOG_ERR("invalid blend enum");
                return;
            }

            GL_CALL(glEnable(GL_BLEND));
            GL_CALL(glBlendFunc(src, dst));
        }

        inline void blend(BlendFunction src, BlendFunction dst)
        {
            blend((GLenum)src, (GLenum)dst);
        }

        inline void blend(GLenum eq)
        {
            if (eq < GL_MIN || eq > GL_FUNC_REVERSE_SUBTRACT)
            {
                LOG_ERR("invalid blend mode");
                return;
            }

            GL_CALL(glEnable(GL_BLEND));
            GL_CALL(glBlendEquation(eq));
        }

        inline void blend(BlendEquation eq) { blend((GLenum)eq); }

        inline void blend(
            BlendFunction color_src, BlendFunction color_dst,
            BlendFunction alpha_src, BlendFunction alpha_dst
        )
        {
            GL_CALL(glBlendFuncSeparate(
                (GLenum)color_src, (GLenum)color_dst, (GLenum)alpha_src,
                (GLenum)alpha_dst
            ));
        }

        inline void blend(BlendEquation color, BlendEquation alpha)
        {
            GL_CALL(glBlendEquationSeparate((GLenum)color, (GLenum)alpha));
        }

        inline void blend(f32 r, f32 g, f32 b, f32 a)
        {
            GL_CALL(glBlendColor(r, g, b, a));
        }

        inline void blend(color col)
        {
            const vec4f color_vec = col.to_vec4();
            GL_CALL(
                glBlendColor(color_vec.x, color_vec.y, color_vec.z, color_vec.w)
            );
        }

        inline void enable_blend() { GL_CALL(glEnable(GL_BLEND)); }

        inline void disable_blend() { GL_CALL(glDisable(GL_BLEND)); }

        inline void mask(bool r, bool g, bool b, bool a)
        {
            GL_CALL(glColorMask(r, g, b, a));
        }

        inline void mask(BlendMask mask)
        {
            GL_CALL(glColorMask(
                ((i32)mask & (i32)BlendMask::RED),
                ((i32)mask & (i32)BlendMask::GREEN),
                ((i32)mask & (i32)BlendMask::BLUE),
                ((i32)mask & (i32)BlendMask::ALPHA)
            ));
        }

        enum class Cull : GLenum
        {
            NONE           = GL_ZERO,
            FRONT          = GL_FRONT,
            BACK           = GL_BACK,
            FRONT_AND_BACK = GL_FRONT_AND_BACK,
        };

        inline void cull(GLenum face)
        {
            if (face < GL_FRONT || face > GL_FRONT_AND_BACK)
            {
                LOG_ERR("cannot cull invalid face enum");
                return;
            }

            GL_CALL(glEnable(GL_CULL_FACE));
            GL_CALL(glCullFace(face));
        }

        inline void disable_cull() { GL_CALL(glDisable(GL_CULL_FACE)); }

        inline void cull(Cull face)
        {
            if (face == Cull::NONE)
                disable_cull();
            else
                cull((GLenum)face);
        }

        inline void enable_depth(GLenum func = GL_LESS)
        {
            if (func < GL_NEVER || func > GL_ALWAYS)
            {
                LOG_ERR("cannot use invalid depth function");
                return;
            }

            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
            glDepthFunc(func);
        }

        inline void disable_depth() { glDisable(GL_DEPTH_TEST); }

        inline void enable_scissor() { GL_CALL(glEnable(GL_SCISSOR_TEST)); }

        inline void disable_scissor() { GL_CALL(glDisable(GL_SCISSOR_TEST)); }

        inline void scissor(const rectf& scissor)
        {
            GL_CALL(glEnable(GL_SCISSOR_TEST));
            GL_CALL(glScissor(
                        (GLint)scissor.x, (GLint)scissor.y, (GLint)scissor.w,
                        (GLint)scissor.h
            ););
        }

        inline const uchar* get_renderer() { return glGetString(GL_RENDERER); }

        inline const uchar* get_version() { return glGetString(GL_VERSION); }

        inline void draw(size i = 6)
        {
            GL_CALL(glDrawElements(
                GL_TRIANGLES, static_cast<GLsizei>(i), GL_UNSIGNED_INT, nullptr
            ));
        }

        inline void draw_instanced(size indices = 6, size instances = 1)
        {
            GL_CALL(glDrawElementsInstanced(
                GL_TRIANGLES, static_cast<GLsizei>(indices), GL_UNSIGNED_INT,
                nullptr, static_cast<GLint>(instances)
            ));
        }
    }
}
