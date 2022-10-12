/*  blocs_imgui

    To add to your project, include this file in *one* C++ file
    to create the implementation:

    #define BLOCS_IMGUI
    #include "blocs_imgui.h"
*/

#pragma once
#ifdef BLOCS_IMGUI

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <blocs.h>

namespace blocs
{
    namespace imgui
    {
        struct ImGUIConfig
        {
            ImGuiContext* ctx;
            ImGuiIO*      io;
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

        inline ImGuiIO* get_io(
            bool docking_enabled = true, bool viewports_enabled = true,
            u32 flags = 0
        )
        {
            // Setup ImGUI input.
            ImGuiIO* io = &ImGui::GetIO();
            // Enable ImGUI Docking + Viewports (only available from
            // "docking" git branch).
            if (docking_enabled)
                io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            if (viewports_enabled)
                io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            io->ConfigFlags |= flags;

            // Apply dark theme by default.
            ImGui::StyleColorsDark();
            // When viewports are enabled we tweak WindowRounding/WindowBg
            // so platform windows can look identical to regular ones.
            ImGuiStyle& style = ImGui::GetStyle();
            if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                style.WindowRounding              = 0.0f;
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
            // Render additional platform windows created from IMGUI
            // viewports.
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
#endif
