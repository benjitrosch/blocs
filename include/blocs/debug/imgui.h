#pragma once

#ifdef DEBUG
#ifndef IMGUI_CHECKVERSION
    #define IMGUI_CHECKVERSION()
#endif

#ifndef BLOCS_DEBUG_IMGUI
    typedef int ImGuiConfigFlags;
    enum ImGuiConfigFlags_
    {
        ImGuiConfigFlags_DockingEnable   = 1 << 6,
        ImGuiConfigFlags_ViewportsEnable = 1 << 10,
    };

    struct ImGuiIO
    {
        ImGuiConfigFlags ConfigFlags;
    };

    struct ImVec4 { float x, y, z, w; };
    
    constexpr int ImGuiCol_COUNT = 55;

    enum ImGuiCol_
    {
        ImGuiCol_WindowBg,
    };

    struct ImGuiStyle
    {
        float WindowRounding;
        ImVec4 Colors[ImGuiCol_COUNT];
    };

    struct ImGuiContext;
    struct ImFontAtlas;
    struct ImDrawData;

    namespace ImGui
    {
        ImGuiIO& GetIO();
        ImGuiContext* CreateContext(ImFontAtlas* shared_font_atlas = nullptr);
        void SetCurrentContext(ImGuiContext* ctx);

        void NewFrame();
        void Render();
        void StyleColorsDark(ImGuiStyle* dst = nullptr);
        ImGuiStyle& GetStyle();
        ImDrawData* GetDrawData();

        void UpdatePlatformWindows();
        void RenderPlatformWindowsDefault(void* platform_render_arg = nullptr, void* renderer_render_arg = nullptr);
        void DestroyContext(ImGuiContext* ctx);
    }

    bool ImGui_ImplSDL2_InitForOpenGL(SDL_Window* window, void* sdl_gl_context);
    bool ImGui_ImplOpenGL3_Init(const char* glsl_version);
    void ImGui_ImplOpenGL3_NewFrame();
    void ImGui_ImplSDL2_NewFrame();
    void ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);
    bool ImGui_ImplSDL2_ProcessEvent(const SDL_Event* event);
    void ImGui_ImplOpenGL3_Shutdown();
    void ImGui_ImplSDL2_Shutdown();
#endif

namespace blocs
{
    namespace debug
    {
        namespace imgui
        {
            struct ImGUIConfig
            {
                ImGuiContext* ctx;
                ImGuiIO* io;
            };

            inline ImGuiContext* create()
            {
                IMGUI_CHECKVERSION();
                ImGuiContext* context = ImGui::CreateContext();
                ImGui::SetCurrentContext(context);
                return context;
            }

            inline void init(Window window, Renderer renderer)
            {
                ImGui_ImplSDL2_InitForOpenGL(window, renderer);
#ifdef __EMSCRIPTEN__
		    	ImGui_ImplOpenGL3_Init("#version 300 es");
#else
		    	ImGui_ImplOpenGL3_Init("#version 330");
#endif
            }

            inline ImGuiIO* get_io(bool docking_enabled = true, bool viewports_enabled = true, u32 flags = 0)
            {
                // Setup ImGUI input.
                ImGuiIO* io = &ImGui::GetIO();
                // Enable ImGUI Docking + Viewports (only available from "docking" git branch).
                if (docking_enabled)    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
                if (viewports_enabled)  io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
                                        io->ConfigFlags |= flags;

                // Apply dark theme by default.
                ImGui::StyleColorsDark();
                // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
                ImGuiStyle& style = ImGui::GetStyle();
                if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                {
                    style.WindowRounding = 0.0f;
                    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
                }
                
                return io;
            }

            inline void new_frame()
            {
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplSDL2_NewFrame();
                ImGui::NewFrame();
            }

            inline void draw(ImGuiIO* io, Window window, Renderer context)
            {
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#ifndef __EMSCRIPTEN__
                // Render additional platform windows created from IMGUI viewports.
                if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                {
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();

                    SDL_GL_MakeCurrent(window, context);
                }
#endif
            }

            inline void events(SDL_Event event)
            {
                ImGui_ImplSDL2_ProcessEvent(&event);
            }

            inline void shutdown(ImGuiContext* context)
            {
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplSDL2_Shutdown();
                ImGui::DestroyContext(context);
            }
        }
    }
}
#endif
