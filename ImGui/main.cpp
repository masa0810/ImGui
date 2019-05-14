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

#include <cstdio>
#include <iostream>

using namespace fmt::literals;

static void glfw_error_callback(const int error,
                                const char* const description) {
  std::cerr << "Glfw Error {:d}: {:s}"_format(error, description) << std::endl;
}

int main(int, char*[]) {
  std::ios::sync_with_stdio(false);

  //
  // OpenGL セットアップ
  //

  // エラーコールバックセット
  glfwSetErrorCallback(glfw_error_callback);
  // 初期化
  if (!glfwInit()) {
    return 1;
  }

  // OpenGL バージョン設定 ( GL 3.2 + GLSL 150 )
  constexpr auto glslVersion = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

  // 基本ウィンドウの作成
  constexpr auto windowsTitle = "Dear ImGui GLFW+OpenGL3 example";
  auto* window = glfwCreateWindow(1280, 720, windowsTitle, nullptr, nullptr);
  if (window == nullptr) {
    return 1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // Enable vsync

  // OpenGL ローダー
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
  if (gl3wInit() != 0)
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
  if (glewInit() != GLEW_OK)
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
  if (gladLoadGL() == 0)
#else
  // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to
  // requires some form of initialization.
#endif
  {
    std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
    return 1;
  }

  // Dear ImGui のセットアップ
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // IO の設定
  auto& io = ImGui::GetIO();
  // キーボードコントロールの有効化
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // ゲームパッドコントロール有効化
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  // カラーテーマ設定
  // ImGui::StyleColorsDark();
  ImGui::StyleColorsLight();
  // ImGui::StyleColorsClassic();

  // レンダリングウィンドウのバインディング
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  // GLSL バージョン設定
  ImGui_ImplOpenGL3_Init(glslVersion);

  // フォントのロード
  io.Fonts->AddFontDefault();
  io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  // ImFont* font =
  //    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  //                                 nullptr,
  //                                 io.Fonts->GetGlyphRangesJapanese());
  // IM_ASSERT(font != nullptr);

  // UI で操作する変数類
  auto showDemoWindow = true;
  auto showAnotherWindow = false;
  ImVec4 clearColor{0.45f, 0.55f, 0.60f, 1.00f};

  // メインループ
  while (!glfwWindowShouldClose(window)) {
    // OpenGL のポーリング処理
    glfwPollEvents();

    // Dear ImGui のフレーム処理
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1、デモウィンドウ表示（詳細は ShowDemoWindow のコード参照）
    if (showDemoWindow) {
      ImGui::ShowDemoWindow(&showDemoWindow);
    }

    // 2、シンプルウィンドウの作成
    {
      // UI用変数
      static float f = 0;
      static int counter = 0;
      // ウィンドウ追加開始
      ImGui::Begin("Hello, world!");
      // テキスト追加
      ImGui::Text("This is some useful text.");
      // デモウィンドウ表示チェックボックス追加
      ImGui::Checkbox("Demo Window", &showDemoWindow);
      // その他ウィンドウ表示チェックボックス追加
      ImGui::Checkbox("Another Window", &showAnotherWindow);
      // 浮動小数点スライダー追加
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      // カラー調整追加
      ImGui::ColorEdit3("clear color", (float*)&clearColor);
      // ボタン追加
      if (ImGui::Button("Button")) {
        counter++;
      }
      // 同一行へのテキスト追加
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);
      // フォーマットテキスト追加
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000 / io.Framerate, io.Framerate);
      // ウィンドウ追加終了
      ImGui::End();
    }

    // 3、その他ウィンドウ
    if (showAnotherWindow) {
      // ウィンドウ追加開始（ bool を渡すとウィンドウのクローズ状態が取れる）
      ImGui::Begin("Another Window", &showAnotherWindow);
      // テキスト追加
      ImGui::Text("Hello from another window!");
      // ボタン追加
      if (ImGui::Button("Close Me")) {
        showAnotherWindow = false;
      }
      // ウィンドウ追加終了
      ImGui::End();
    }

    // その他レンダリング処理
    ImGui::Render();
    auto displayW = 0, displayH = 0;
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &displayW, &displayH);
    glViewport(0, 0, displayW, displayH);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
  }

  // GUI 終了処理
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  // OpenGL 終了処理
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
