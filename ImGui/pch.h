#ifndef PCH_H
#define PCH_H

#ifdef _MSC_VER
// Windows.hで"min"と"max"をdefineしない(C++標準ライブラリの"min"と"max"を使う)
#define NOMINMAX
// 頻繁に使用されないAPIを使用しない
#define VC_EXTRALEAN
// 頻繁に使用されないAPIを使用しない
//#define WIN32_LEAN_AND_MEAN
// GDI系APIをOFF
#define NOGDI
#endif


// C4996警告対策
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#if defined(_DEBUG) && !defined(_SCL_SECURE_NO_WARNINGS)
#define _SCL_SECURE_NO_WARNINGS
#endif
#include <cstdio>
#ifdef _SCL_SECURE_NO_WARNINGS
#undef _SCL_SECURE_NO_WARNINGS
#endif
#undef _CRT_SECURE_NO_WARNINGS
#endif

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
// Initialize with gl3wInit()
#include <GL/gl3w.h>
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
// Initialize with glewInit()
#include <GL/glew.h>
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
// Initialize with gladLoadGL()
#include <glad/glad.h>
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif
#include <GLFW/glfw3.h>
#include <fmt/format.h>

#include <iostream>

#endif
