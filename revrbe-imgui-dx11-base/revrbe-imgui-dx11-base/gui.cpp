#include "gui.h"
#include "xenforo.h"

void GUI::Draw(HWND hwnd)
{
    if (!isLoggedIn)
    {
        if (drawLogin)
        {
            DrawLoginInterface(hwnd);
        }
    }
    else
    {
        DrawInterface(hwnd);
    }
}

void GUI::DrawLoginInterface(HWND hwnd)
{
    if (!GUI::isLoggedIn) 
    {
        ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
        ImGui::Begin((magiXOR("Login Interface").decrypt()), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
        {
            float imageWidth = 100.0f;
            float imageHeight = 100.0f;
            float windowWidth = ImGui::GetWindowWidth();
            float posX = (windowWidth - imageWidth) * 0.5f;
            ImGui::SetCursorPosX(posX);
            ImGui::Image((PVOID)logoImage, ImVec2(imageWidth, imageHeight));
            ImGui::Dummy(ImVec2(0, 5.0f));

            {
                float inputWidth = windowWidth * 0.6f;

                float textWidth = ImGui::CalcTextSize(magiXOR("Username").decrypt()).x;
                ImGui::SetCursorPosX((windowWidth - textWidth) * 0.24f);
                ImGui::Text(magiXOR("Username").decrypt());
                ImGui::SetCursorPosX((windowWidth - inputWidth) * 0.5f);
                ImGui::PushItemWidth(inputWidth);
                ImGui::InputText(magiXOR("##username").decrypt(), GUI::user_name, IM_ARRAYSIZE(GUI::user_name));
                ImGui::PopItemWidth();

                ImGui::Dummy(ImVec2(0, 0.5f));

                textWidth = ImGui::CalcTextSize(magiXOR("Password").decrypt()).x;
                ImGui::SetCursorPosX((windowWidth - textWidth) * 0.24f);
                ImGui::Text(magiXOR("Password").decrypt());
                ImGui::SetCursorPosX((windowWidth - inputWidth) * 0.5f);
                ImGui::PushItemWidth(inputWidth);
                ImGui::InputText(magiXOR("##password").decrypt(), GUI::pass_word, IM_ARRAYSIZE(GUI::pass_word), ImGuiInputTextFlags_Password);
                ImGui::PopItemWidth();

                ImGui::Dummy(ImVec2(0, 2.0f));

                float buttonWidth = 240.0f;
                float buttonPosX = (windowWidth - buttonWidth) * 0.5f;
                ImGui::SetCursorPosX(buttonPosX);
                if (ImGui::Button(magiXOR("Log In").decrypt(), ImVec2(buttonWidth, 25))) {
                    bool login = checkLoginCredentials(GUI::user_name, GUI::pass_word);

                    if (login) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3000 + 1000));
                        isLoggedIn = true;
                    }
                }

                ImGui::Dummy(ImVec2(0, 1.5f));

                float disabledTextWidth = ImGui::CalcTextSize(magiXOR("Don't have an account?").decrypt()).x;
                ImGui::SetCursorPosX((windowWidth - disabledTextWidth) * 0.5f);
                ImGui::TextDisabled(magiXOR("Don't have an account?").decrypt());
                if (ImGui::IsItemClicked()) {
                    ShellExecute(NULL, "open", magiXOR("http://google.com/").decrypt(), NULL, NULL, SW_SHOWNORMAL);
                }

                static int currentColorScheme = 0;
                float colorTextWidth = ImGui::CalcTextSize(magiXOR("Change colors").decrypt()).x;
                ImGui::SetCursorPosX((windowWidth - colorTextWidth) * 0.49f);
                if (ImGui::Button(magiXOR("Change colors").decrypt())) {
                    currentColorScheme = (currentColorScheme + 1) % 14;

                    TCHAR tempPath[MAX_PATH];
                    GetTempPath(MAX_PATH, tempPath);
                    std::string colorSchemeFile = std::string(tempPath) + magiXOR("color_scheme.txt").decrypt();

                    std::ofstream outputFile(colorSchemeFile);
                    if (outputFile.is_open()) {
                        outputFile << currentColorScheme;
                        outputFile.close();
                    }

                    ApplyColorScheme(colorSchemes[currentColorScheme]);
                }
            }
        }

        ImGui::PopStyleVar();
        ImGui::End();
    }
}

// Declare the state variable outside the function
int state = 0;

void GUI::DrawInterface(HWND hwnd)
{
    ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
    ImGui::Begin((magiXOR("Interface").decrypt()), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
    {
        float imageWidth = 80.0f;
        float imageHeight = 80.0f;
        float windowWidth = ImGui::GetWindowWidth();

        // Left-hand side child window
        ImVec2 childPos(ImGui::GetWindowPos().x + 15.0f, ImGui::GetWindowPos().y + 15.0f);
        ImVec2 childSize(imageWidth + 3.0f, imageHeight + 3.0f);

        ImGui::BeginChild("ChildWindow", childSize, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        {
            ImVec2 imagePos((childSize.x - imageWidth) * 0.5f, (childSize.y - imageHeight) * 0.5f - 5);
            ImGui::SetCursorPos(imagePos);
            ImGui::Image((PVOID)logoImage, ImVec2(imageWidth, imageHeight));
        }
        ImGui::EndChild();

        // Right-hand side child window
        ImGui::SameLine();
        float rightChildWidth = ImGui::GetContentRegionAvail().x;
        float rightChildHeight = ImGui::GetContentRegionAvail().y;

        ImGui::BeginChild("RightChildWindow", ImVec2(rightChildWidth, rightChildHeight), true);
        {
            if (state == 1)
            {
                ImGui::Text(magiXOR("Spoofer").decrypt());
            }
            else if (state == 2)
            {
                ImGui::Text(magiXOR("Injector").decrypt());
            }
            else if (state == 3)
            {
                ImGui::Text(magiXOR("Info").decrypt());
            }
            else {
                ImGui::Text(magiXOR("Hi, %s!").decrypt(), GUI::user_name);
                ImGui::Text(magiXOR("Rank: %s").decrypt(), user_rank);
                
                // I will add licenses(product id, license id and expiry date) later
            }
        }
        ImGui::EndChild();

        float buttonWidth = childSize.y;
        float buttonHeight = 27;
        ImGui::SetCursorPos(ImVec2(9, imageHeight + 20));
        if (ImGui::Button("Spoofer", ImVec2(buttonWidth, buttonHeight)))
        {
            state = 1;
        }
        ImGui::SetCursorPos(ImVec2(9, imageHeight + 55));
        if (ImGui::Button("Injector", ImVec2(buttonWidth, buttonHeight)))
        {
            state = 2;
        }
        ImGui::SetCursorPos(ImVec2(9, imageHeight + 90));
        if (ImGui::Button("Info", ImVec2(buttonWidth, buttonHeight)))
        {
            state = 3;
        }
    }

    ImGui::End();
}

void GUI::ApplyColorScheme(const char* colorScheme) {
    ImGuiStyle& style = ImGui::GetStyle();

    if (strcmp(colorScheme, "Standard") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.22f, 0.22f, 0.22f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.40f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.90f, 0.90f, 0.90f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.70f, 0.50f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.70f, 0.50f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.9f);
    }
    else if (strcmp(colorScheme, "Cinder") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
    }
    else if (strcmp(colorScheme, "Green colorway") == 0) {
        ImGui::StyleColorsClassic();
        style.Colors[ImGuiCol_Text] = ImVec4(0.76f, 0.93f, 0.78f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.76f, 0.93f, 0.78f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.18f, 0.92f, 0.29f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.76f, 0.93f, 0.78f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.76f, 0.93f, 0.78f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.18f, 0.92f, 0.29f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
    }
    else if (strcmp(colorScheme, "Blue colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.85f, 0.92f, 0.95f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.85f, 0.92f, 0.95f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.18f, 0.23f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.32f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.18f, 0.23f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.18f, 0.23f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.18f, 0.23f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.18f, 0.23f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.48f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.31f, 0.48f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.63f, 0.82f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.35f, 0.63f, 0.82f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.44f, 0.60f, 0.75f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.22f, 0.38f, 0.51f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.63f, 0.82f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.85f, 0.92f, 0.95f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.85f, 0.92f, 0.95f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.44f, 0.60f, 0.75f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.18f, 0.23f, 0.9f);
    }
    else if (strcmp(colorScheme, "Purple/orange colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.88f, 0.84f, 0.96f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.88f, 0.84f, 0.96f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.17f, 0.12f, 0.24f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.30f, 0.20f, 0.40f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.30f, 0.20f, 0.40f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.30f, 0.20f, 0.40f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.30f, 0.20f, 0.40f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.30f, 0.20f, 0.40f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.91f, 0.55f, 0.23f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.91f, 0.55f, 0.23f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.93f, 0.52f, 0.17f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.57f, 0.35f, 0.85f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.84f, 0.36f, 0.14f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.45f, 0.25f, 0.58f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.57f, 0.35f, 0.85f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.88f, 0.84f, 0.96f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.88f, 0.84f, 0.96f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.84f, 0.36f, 0.14f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.22f, 0.16f, 0.28f, 0.9f);
    }
    else if (strcmp(colorScheme, "Dark mode") == 0) {
        ImGui::StyleColorsClassic();
        style.Colors[ImGuiCol_Text] = ImVec4(0.85f, 0.85f, 0.85f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.22f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.45f, 0.45f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.29f, 0.29f, 0.29f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.55f, 0.55f, 0.55f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.29f, 0.29f, 0.29f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.85f, 0.85f, 0.85f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.85f, 0.85f, 0.85f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.55f, 0.55f, 0.55f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.9f);
    }
    else if (strcmp(colorScheme, "Pink/green/blue colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.58f, 0.70f, 0.39f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.25f, 0.25f, 0.25f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.25f, 0.25f, 0.25f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.71f, 0.24f, 0.53f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.58f, 0.70f, 0.39f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.77f, 0.44f, 0.90f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.12f, 0.89f, 0.91f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.81f, 0.28f, 0.60f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.81f, 0.28f, 0.60f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.81f, 0.28f, 0.60f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.81f, 0.28f, 0.60f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.06f, 0.53f, 0.55f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.28f, 0.60f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.12f, 0.89f, 0.91f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.12f, 0.89f, 0.91f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.81f, 0.28f, 0.60f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.9f);
    }
    else if (strcmp(colorScheme, "Blue and grey colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.30f, 0.40f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.35f, 0.45f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.50f, 0.60f, 0.70f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.45f, 0.55f, 0.65f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.30f, 0.40f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.30f, 0.40f, 0.50f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.35f, 0.45f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.28f, 0.36f, 0.46f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.35f, 0.45f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.60f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.65f, 0.75f, 0.85f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.75f, 0.85f, 0.95f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.80f, 0.90f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.95f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.60f, 0.70f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.85f, 0.95f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.50f, 0.60f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.55f, 0.65f, 0.75f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.77f, 0.87f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.60f, 0.70f, 0.50f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.65f, 0.75f, 0.85f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.85f, 0.95f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.90f, 1.00f, 0.20f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.90f, 0.95f, 1.00f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.85f, 0.95f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.50f, 0.70f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.30f, 0.50f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.50f, 0.70f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.30f, 0.50f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.50f, 0.60f, 0.70f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.30f, 0.40f, 0.50f, 0.9f);
    }
    else if (strcmp(colorScheme, "Boring colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.80f, 0.80f, 0.80f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.90f, 0.20f, 0.20f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.80f, 0.80f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.70f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.60f, 0.90f, 0.90f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.80f, 1.00f, 1.00f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.90f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.80f, 0.80f, 0.50f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 1.00f, 1.00f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.90f, 0.20f, 0.20f, 0.20f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.35f, 0.35f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.90f, 0.30f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.80f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.90f, 0.30f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.90f, 0.80f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.50f, 0.80f, 0.80f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.30f, 0.50f, 0.50f, 0.9f);
    }
    else if (strcmp(colorScheme, "Blue/green colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.85f, 0.92f, 0.88f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.85f, 0.92f, 0.88f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.11f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f); 
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.24f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.73f, 0.69f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.73f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.24f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.24f, 0.27f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.44f, 0.73f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.24f, 0.27f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.24f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.23f, 0.50f, 0.46f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.44f, 0.73f, 0.69f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.44f, 0.73f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.23f, 0.50f, 0.46f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.21f, 0.47f, 0.44f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.44f, 0.73f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.32f, 0.55f, 0.51f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.05f, 0.38f, 0.42f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.06f, 0.37f, 0.41f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.05f, 0.38f, 0.42f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.11f, 0.32f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.05f, 0.38f, 0.42f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.16f, 0.47f, 0.51f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.05f, 0.38f, 0.42f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.88f, 0.96f, 0.84f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.88f, 0.96f, 0.84f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.05f, 0.38f, 0.42f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.29f, 0.32f, 0.9f);
    }
    else if (strcmp(colorScheme, "Green/yellow colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.88f, 0.84f, 0.96f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.88f, 0.84f, 0.96f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.18f, 0.12f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.40f, 0.30f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.96f, 0.84f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.96f, 0.84f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.40f, 0.30f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.30f, 0.50f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.96f, 0.84f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.30f, 0.50f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.30f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.84f, 0.89f, 0.36f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.96f, 0.84f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.96f, 0.84f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.84f, 0.89f, 0.36f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.89f, 0.93f, 0.55f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.96f, 0.84f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.28f, 0.41f, 0.14f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.84f, 0.36f, 0.14f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.45f, 0.25f, 0.58f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.57f, 0.35f, 0.85f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.88f, 0.84f, 0.96f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.88f, 0.84f, 0.96f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.84f, 0.36f, 0.14f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.22f, 0.16f, 0.28f, 0.9f);
    }
    else if (strcmp(colorScheme, "Orange/grey colorway") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.48f, 0.48f, 0.48f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.24f, 0.24f, 0.24f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.64f, 0.32f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.30f, 0.30f, 0.30f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.50f, 0.64f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45f, 0.65f, 0.85f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.55f, 0.75f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.64f, 0.32f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.50f, 0.64f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.55f, 0.75f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.40f, 0.40f, 0.40f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.93f, 0.75f, 0.18f, 0.78f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.93f, 0.75f, 0.18f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.72f, 0.79f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.93f, 0.75f, 0.18f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.75f, 0.18f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.54f, 0.90f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.93f, 0.75f, 0.18f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.93f, 0.75f, 0.18f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.54f, 0.90f, 0.70f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.93f, 0.75f, 0.18f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.93f, 0.75f, 0.18f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.93f, 0.75f, 0.18f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.23f, 0.16f, 0.42f, 0.9f);
    }
    else if (strcmp(colorScheme, "Whitey") == 0) {
        ImGui::StyleColorsDark();
        style.Colors[ImGuiCol_Text] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    }
    else if (strcmp(colorScheme, "Colorful") == 0) {
        ImGui::StyleColorsLight();
        style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.15f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.90f, 0.35f, 0.29f, 0.75f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.60f, 0.38f, 0.80f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.80f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.35f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.35f, 0.80f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.65f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.45f, 0.80f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.35f, 0.80f, 0.53f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00f, 0.80f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.60f, 0.38f, 0.80f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.90f, 0.35f, 0.29f, 0.75f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.80f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.60f, 0.38f, 0.70f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.90f, 0.35f, 0.29f, 0.75f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.35f, 0.80f, 0.70f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.45f, 1.00f, 0.80f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.25f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.15f, 0.35f, 0.80f, 0.45f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.15f, 0.45f, 1.00f, 0.70f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.25f, 0.60f, 0.80f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.15f, 0.35f, 0.80f, 0.20f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.15f, 0.45f, 1.00f, 0.45f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.15f, 0.25f, 0.60f, 0.70f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.15f, 0.35f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.15f, 0.45f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.15f, 0.35f, 0.80f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.15f, 0.45f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.15f, 0.45f, 1.00f, 0.35f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    }
}
