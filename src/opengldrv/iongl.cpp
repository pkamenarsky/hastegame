#include "iongl.hh"
/*
#ifdef ION_IMPLEMENT_GL_VERSION_1_2
glDrawRangeElementsPROC glDrawRangeElements;
glTexImage3DPROC glTexImage3D = 0;
glTexSubImage3DPROC glTexSubImage3D = 0;
glCopyTexSubImage3DPROC glCopyTexSubImage3D = 0;
#endif // ION_IMPLEMENT_GL_VERSION_1_2

#ifdef ION_IMPLEMENT_GL_ARB_IMAGING
glBlendColorPROC glBlendColor = 0;
glBlendEquationPROC glBlendEquation = 0;
glColorTablePROC glColorTable = 0;
glColorTableParameterfvPROC glColorTableParameterfv = 0;
glColorTableParameterivPROC glColorTableParameteriv = 0;
glCopyColorTablePROC glCopyColorTable = 0;
glGetColorTablePROC glGetColorTable = 0;
glGetColorTableParameterfvPROC glGetColorTableParameterfv = 0;
glGetColorTableParameterivPROC glGetColorTableParameteriv = 0;
glColorSubTablePROC glColorSubTable = 0;
glCopyColorSubTablePROC glCopyColorSubTable = 0;
glConvolutionFilter1DPROC glConvolutionFilter1D = 0;
glConvolutionFilter2DPROC glConvolutionFilter2D = 0;
glConvolutionParameterfPROC glConvolutionParameterf = 0;
glConvolutionParameterfvPROC glConvolutionParameterfv = 0;
glConvolutionParameteriPROC glConvolutionParameteri = 0;
glConvolutionParameterivPROC glConvolutionParameteriv = 0;
glCopyConvolutionFilter1DPROC glCopyConvolutionFilter1D = 0;
glCopyConvolutionFilter2DPROC glCopyConvolutionFilter2D = 0;
glGetConvolutionFilterPROC glGetConvolutionFilter = 0;
glGetConvolutionParameterfvPROC glGetConvolutionParameterfv = 0;
glGetConvolutionParameterivPROC glGetConvolutionParameteriv = 0;
glGetSeparableFilterPROC glGetSeparableFilter = 0;
glSeparableFilter2DPROC glSeparableFilter2D = 0;
glGetHistogramPROC glGetHistogram = 0;
glGetHistogramParameterfvPROC glGetHistogramParameterfv = 0;
glGetHistogramParameterivPROC glGetHistogramParameteriv = 0;
glGetMinmaxPROC glGetMinmax = 0;
glGetMinmaxParameterfvPROC glGetMinmaxParameterfv = 0;
glGetMinmaxParameterivPROC glGetMinmaxParameteriv = 0;
glHistogramPROC glHistogram = 0;
glMinmaxPROC glMinmax = 0;
glResetHistogramPROC glResetHistogram = 0;
glResetMinmaxPROC glResetMinmax = 0;
#endif // ION_IMPLEMENT_GL_ARB_IMAGING

#ifdef ION_IMPLEMENT_GL_ARB_TEXTURE_COMPRESSION
glCompressedTexImage3DARBPROC glCompressedTexImage3DARB = 0;
glCompressedTexImage2DARBPROC glCompressedTexImage2DARB = 0;
glCompressedTexImage1DARBPROC glCompressedTexImage1DARB = 0; 
glCompressedTexSubImage3DARBPROC glCompressedTexSubImage3DARB = 0;
glCompressedTexSubImage2DARBPROC glCompressedTexSubImage2DARB = 0;
glCompressedTexSubImage1DARBPROC glCompressedTexSubImage1DARB = 0;
glGetCompressedTexImageARBPROC glGetCompressedTexImageARB = 0;
#endif // ION_IMPLEMENT_GL_ARB_TEXTURE_COMPRESSION

#ifdef ION_IMPLEMENT_GL_ARB_MULTITEXTURE
glActiveTextureARBPROC glActiveTextureARB = 0;
glClientActiveTextureARBPROC glClientActiveTextureARB = 0;
glMultiTexCoord1dARBPROC glMultiTexCoord1dARB = 0;
glMultiTexCoord1dvARBPROC glMultiTexCoord1dvARB = 0;
glMultiTexCoord1fARBPROC glMultiTexCoord1fARB = 0;
glMultiTexCoord1fvARBPROC glMultiTexCoord1fvARB = 0;
glMultiTexCoord1iARBPROC glMultiTexCoord1iARB = 0;
glMultiTexCoord1ivARBPROC glMultiTexCoord1ivARB = 0;
glMultiTexCoord1sARBPROC glMultiTexCoord1sARB = 0;
glMultiTexCoord1svARBPROC glMultiTexCoord1svARB = 0;
glMultiTexCoord2dARBPROC glMultiTexCoord2dARB = 0;
glMultiTexCoord2dvARBPROC glMultiTexCoord2dvARB = 0;
glMultiTexCoord2fARBPROC glMultiTexCoord2fARB = 0;
glMultiTexCoord2fvARBPROC glMultiTexCoord2fvARB = 0;
glMultiTexCoord2iARBPROC glMultiTexCoord2iARB = 0;
glMultiTexCoord2ivARBPROC glMultiTexCoord2ivARB = 0;
glMultiTexCoord2sARBPROC glMultiTexCoord2sARB = 0;
glMultiTexCoord2svARBPROC glMultiTexCoord2svARB = 0;
glMultiTexCoord3dARBPROC glMultiTexCoord3dARB = 0;
glMultiTexCoord3dvARBPROC glMultiTexCoord3dvARB = 0;
glMultiTexCoord3fARBPROC glMultiTexCoord3fARB = 0;
glMultiTexCoord3fvARBPROC glMultiTexCoord3fvARB = 0;
glMultiTexCoord3iARBPROC glMultiTexCoord3iARB = 0;
glMultiTexCoord3ivARBPROC glMultiTexCoord3ivARB = 0;
glMultiTexCoord3sARBPROC glMultiTexCoord3sARB = 0;
glMultiTexCoord3svARBPROC glMultiTexCoord3svARB = 0;
glMultiTexCoord4dARBPROC glMultiTexCoord4dARB = 0;
glMultiTexCoord4dvARBPROC glMultiTexCoord4dvARB = 0;
glMultiTexCoord4fARBPROC glMultiTexCoord4fARB = 0;
glMultiTexCoord4fvARBPROC glMultiTexCoord4fvARB = 0;
glMultiTexCoord4iARBPROC glMultiTexCoord4iARB = 0;
glMultiTexCoord4ivARBPROC glMultiTexCoord4ivARB = 0;
glMultiTexCoord4sARBPROC glMultiTexCoord4sARB = 0;
glMultiTexCoord4svARBPROC glMultiTexCoord4svARB = 0;
#endif // ION_IMPLEMENT_GL_ARB_MULTITEXTURE

#ifdef ION_IMPLEMENT_GL_ARB_VERTEX_PROGRAM
glVertexAttrib1sARBPROC glVertexAttrib1sARB = 0;
glVertexAttrib1fARBPROC glVertexAttrib1fARB = 0;
glVertexAttrib1dARBPROC glVertexAttrib1dARB = 0;
glVertexAttrib2sARBPROC glVertexAttrib2sARB = 0;
glVertexAttrib2fARBPROC glVertexAttrib2fARB = 0;
glVertexAttrib2dARBPROC glVertexAttrib2dARB = 0;
glVertexAttrib3sARBPROC glVertexAttrib3sARB = 0;
glVertexAttrib3fARBPROC glVertexAttrib3fARB = 0;
glVertexAttrib3dARBPROC glVertexAttrib3dARB = 0;
glVertexAttrib4sARBPROC glVertexAttrib4sARB = 0;
glVertexAttrib4fARBPROC glVertexAttrib4fARB = 0;
glVertexAttrib4dARBPROC glVertexAttrib4dARB = 0;
glVertexAttrib4NubARBPROC glVertexAttrib4NubARB = 0;
glVertexAttrib1svARBPROC glVertexAttrib1svARB = 0;
glVertexAttrib1fvARBPROC glVertexAttrib1fvARB = 0;
glVertexAttrib1dvARBPROC glVertexAttrib1dvARB = 0;
glVertexAttrib2svARBPROC glVertexAttrib2svARB = 0;
glVertexAttrib2fvARBPROC glVertexAttrib2fvARB = 0;
glVertexAttrib2dvARBPROC glVertexAttrib2dvARB = 0;
glVertexAttrib3svARBPROC glVertexAttrib3svARB = 0;
glVertexAttrib3fvARBPROC glVertexAttrib3fvARB = 0;
glVertexAttrib3dvARBPROC glVertexAttrib3dvARB = 0;
glVertexAttrib4bvARBPROC glVertexAttrib4bvARB = 0;
glVertexAttrib4svARBPROC glVertexAttrib4svARB = 0;
glVertexAttrib4ivARBPROC glVertexAttrib4ivARB = 0;
glVertexAttrib4ubvARBPROC glVertexAttrib4ubvARB = 0;
glVertexAttrib4usvARBPROC glVertexAttrib4usvARB = 0;
glVertexAttrib4uivARBPROC glVertexAttrib4uivARB = 0;
glVertexAttrib4fvARBPROC glVertexAttrib4fvARB = 0;
glVertexAttrib4dvARBPROC glVertexAttrib4dvARB = 0;
glVertexAttrib4NbvARBPROC glVertexAttrib4NbvARB = 0;
glVertexAttrib4NsvARBPROC glVertexAttrib4NsvARB = 0;
glVertexAttrib4NivARBPROC glVertexAttrib4NivARB = 0;
glVertexAttrib4NubvARBPROC glVertexAttrib4NubvARB = 0;
glVertexAttrib4NusvARBPROC glVertexAttrib4NusvARB = 0;
glVertexAttrib4NuivARBPROC glVertexAttrib4NuivARB = 0;
glVertexAttribPointerARBPROC glVertexAttribPointerARB = 0;
glEnableVertexAttribArrayARBPROC glEnableVertexAttribArrayARB = 0;
glDisableVertexAttribArrayARBPROC glDisableVertexAttribArrayARB = 0;
glProgramStringARBPROC glProgramStringARB = 0;
glBindProgramARBPROC glBindProgramARB = 0;
glDeleteProgramsARBPROC glDeleteProgramsARB = 0;
glGenProgramsARBPROC glGenProgramsARB = 0;
glProgramEnvParameter4dARBPROC glProgramEnvParameter4dARB = 0;
glProgramEnvParameter4dvARBPROC glProgramEnvParameter4dvARB = 0;
glProgramEnvParameter4fARBPROC glProgramEnvParameter4fARB = 0;
glProgramEnvParameter4fvARBPROC glProgramEnvParameter4fvARB = 0;
glProgramLocalParameter4dARBPROC glProgramLocalParameter4dARB = 0;
glProgramLocalParameter4dvARBPROC glProgramLocalParameter4dvARB = 0;
glProgramLocalParameter4fARBPROC glProgramLocalParameter4fARB = 0;
glProgramLocalParameter4fvARBPROC glProgramLocalParameter4fvARB = 0;
glGetProgramEnvParameterdvARBPROC glGetProgramEnvParameterdvARB = 0;
glGetProgramEnvParameterfvARBPROC glGetProgramEnvParameterfvARB = 0;
glGetProgramLocalParameterdvARBPROC glGetProgramLocalParameterdvARB = 0;
glGetProgramLocalParameterfvARBPROC glGetProgramLocalParameterfvARB = 0;
glGetProgramivARBPROC glGetProgramivARB = 0;
glGetProgramStringARBPROC glGetProgramStringARB = 0;
glGetVertexAttribdvARBPROC glGetVertexAttribdvARB = 0;
glGetVertexAttribfvARBPROC glGetVertexAttribfvARB = 0;
glGetVertexAttribivARBPROC glGetVertexAttribivARB = 0;
glGetVertexAttribPointervARBPROC glGetVertexAttribPointervARB = 0;
glIsProgramARBPROC glIsProgramARB = 0;
#endif // ION_IMPLEMENT_GL_ARB_VERTEX_PROGRAM

#ifdef ION_IMPLEMENT_GL_ARB_VERTEX_BUFFER_OBJECT
glBindBufferARBPROC glBindBufferARB = 0;
glDeleteBuffersARBPROC glDeleteBuffersARB = 0;
glGenBuffersARBPROC glGenBuffersARB = 0;
glIsBufferARBPROC glIsBufferARB = 0;
glBufferDataARBPROC glBufferDataARB = 0;
glBufferSubDataARBPROC glBufferSubDataARB = 0;
glGetBufferSubDataARBPROC glGetBufferSubDataARB = 0;
glMapBufferARBPROC glMapBufferARB = 0;
glUnmapBufferARBPROC glUnmapBufferARB = 0;
glGetBufferParameterivARBPROC glGetBufferParameterivARB = 0;
glGetBufferPointervARBPROC glGetBufferPointervARB = 0;
#endif // ION_IMPLEMENT_GL_ARB_VERTEX_BUFFER_OBJECT

#ifdef ION_IMPLEMENT_GL_ARB_OCCLUSION_QUERY
glGenQueriesARBPROC glGenQueriesARB = 0;
glDeleteQueriesARBPROC glDeleteQueriesARB = 0;
glIsQueryARBPROC glIsQueryARB = 0;
glBeginQueryARBPROC glBeginQueryARB = 0;
glEndQueryARBPROC glEndQueryARB = 0;
glGetQueryivARBPROC glGetQueryivARB = 0;
glGetQueryObjectivARBPROC glGetQueryObjectivARB = 0;
glGetQueryObjectuivARBPROC glGetQueryObjectuivARB = 0;
#endif // ION_IMPLEMENT_GL_ARB_OCCLUSION_QUERY

#ifdef ION_IMPLEMENT_GL_NV_OCCLUSION_QUERY
glGenOcclusionQueriesNVPROC glGenOcclusionQueriesNV = NULL;
glDeleteOcclusionQueriesNVPROC glDeleteOcclusionQueriesNV = NULL;
glIsOcclusionQueryNVPROC glIsOcclusionQueryNV = NULL;
glBeginOcclusionQueryNVPROC glBeginOcclusionQueryNV = NULL;
glEndOcclusionQueryNVPROC glEndOcclusionQueryNV = NULL;
glGetOcclusionQueryivNVPROC glGetOcclusionQueryivNV = NULL;
glGetOcclusionQueryuivNVPROC glGetOcclusionQueryuivNV = NULL;
#endif // ION_IMPLEMENT_GL_NV_OCCLUSION_QUERY

#ifdef ION_IMPLEMENT_GL_EXT_FRAMEBUFFER_OBJECT
glIsRenderbufferEXTPROC glIsRenderbufferEXT = NULL;
glBindRenderbufferEXTPROC glBindRenderbufferEXT = NULL;
glDeleteRenderbuffersEXTPROC glDeleteRenderbuffersEXT = NULL;
glGenRenderbuffersEXTPROC glGenRenderbuffersEXT = NULL;
glRenderbufferStorageEXTPROC glRenderbufferStorageEXT = NULL;
glGetRenderbufferParameterivEXTPROC glGetRenderbufferParameterivEXT = NULL;
glIsFramebufferEXTPROC glIsFramebufferEXT = NULL;
glBindFramebufferEXTPROC glBindFramebufferEXT = NULL;
glDeleteFramebuffersEXTPROC glDeleteFramebuffersEXT = NULL;
glGenFramebuffersEXTPROC glGenFramebuffersEXT = NULL;
glCheckFramebufferStatusEXTPROC glCheckFramebufferStatusEXT = NULL;
glFramebufferTexture1DEXTPROC glFramebufferTexture1DEXT = NULL;
glFramebufferTexture2DEXTPROC glFramebufferTexture2DEXT = NULL;
glFramebufferTexture3DEXTPROC glFramebufferTexture3DEXT = NULL;
glFramebufferRenderbufferEXTPROC glFramebufferRenderbufferEXT = NULL;
glGetFramebufferAttachmentParameterivEXTPROC glGetFramebufferAttachmentParameterivEXT = NULL;
glGenerateMipmapEXTPROC glGenerateMipmapEXT = NULL;
#endif // ION_IMPLEMENT_GL_EXT_FRAMEBUFFER_OBJECT
*/

