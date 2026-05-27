#include "UIManager.h"
#include <iostream>
#include <string>

// ImGui headers
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// GLFW
#include <GLFW/glfw3.h>

UIManager::UIManager() : window(nullptr) {}
UIManager::~UIManager() {}

bool UIManager::init() {
    if (!glfwInit()) return false;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "Automobile Insurance System", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // UX/UI Improvement: Increase global font scale for readability
    io.FontGlobalScale = 1.3f;

    // Setup Dear ImGui style
    renderStyles();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return true;
}

void UIManager::renderStyles() {
    // ── Iron Man HUD Theme ────────────────────────────────────────────────────
    // Start from the dark base so any unset slots get sane defaults
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    // ── Geometry / Shape ─────────────────────────────────────────────────────
    // Rigid tech-interface look: sharp windows, smooth frames
    style.WindowRounding    = 2.0f;   // nearly square — feels like a HUD panel
    style.ChildRounding     = 3.0f;
    style.FrameRounding     = 5.0f;   // smooth input fields & checkboxes
    style.GrabRounding      = 4.0f;
    style.TabRounding       = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.PopupRounding     = 2.0f;

    // ── Spacing ───────────────────────────────────────────────────────────────
    style.WindowPadding     = ImVec2(14.0f, 14.0f);
    style.FramePadding      = ImVec2(8.0f,  6.0f);
    style.ItemSpacing       = ImVec2(8.0f,  10.0f);
    style.ItemInnerSpacing  = ImVec2(6.0f,  6.0f);
    style.WindowBorderSize  = 1.0f;
    style.FrameBorderSize   = 1.0f;   // subtle border on every input box

    // ─────────────────────────────────────────────────────────────────────────
    // COLOUR PALETTE
    //  Charcoal blacks  : #111111 → (0.067, 0.067, 0.067)
    //                     #1A1A1A → (0.102, 0.102, 0.102)
    //                     #222222 → (0.133, 0.133, 0.133)
    //  Crimson red      : #8B0000 → (0.545, 0.000, 0.000)   deep armour
    //                     #C0392B → (0.753, 0.224, 0.169)   lighter accent
    //  Arc-reactor cyan : #00D4FF → (0.000, 0.831, 1.000)   full glow
    //                     #00A8CC → (0.000, 0.659, 0.800)   dimmer hover
    //  Gold/brass       : #C9A84C → (0.788, 0.659, 0.298)   accent/border
    //                     #8B6914 → (0.545, 0.412, 0.078)   dark gold
    //  Text             : #E8E8E8 → (0.910, 0.910, 0.910)   crisp white
    // ─────────────────────────────────────────────────────────────────────────

    ImVec4* c = style.Colors;

    // ── Backgrounds ──────────────────────────────────────────────────────────
    c[ImGuiCol_WindowBg]           = ImVec4(0.067f, 0.067f, 0.067f, 1.000f); // #111111
    c[ImGuiCol_ChildBg]            = ImVec4(0.102f, 0.102f, 0.102f, 1.000f); // #1A1A1A
    c[ImGuiCol_PopupBg]            = ImVec4(0.102f, 0.102f, 0.102f, 0.980f);
    c[ImGuiCol_MenuBarBg]          = ImVec4(0.067f, 0.067f, 0.067f, 1.000f);

    // ── Borders ───────────────────────────────────────────────────────────────
    // Gold/brass borders give the HUD its iconic edging
    c[ImGuiCol_Border]             = ImVec4(0.545f, 0.412f, 0.078f, 0.600f); // dark gold
    c[ImGuiCol_BorderShadow]       = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);

    // ── Input / Frame ─────────────────────────────────────────────────────────
    c[ImGuiCol_FrameBg]            = ImVec4(0.133f, 0.133f, 0.133f, 1.000f); // #222222
    c[ImGuiCol_FrameBgHovered]     = ImVec4(0.000f, 0.290f, 0.360f, 0.700f); // dim cyan glow
    c[ImGuiCol_FrameBgActive]      = ImVec4(0.000f, 0.659f, 0.800f, 0.800f); // arc reactor cyan

    // ── Title bar (Crimson armor) ─────────────────────────────────────────────
    c[ImGuiCol_TitleBg]            = ImVec4(0.320f, 0.000f, 0.000f, 1.000f); // deep crimson
    c[ImGuiCol_TitleBgActive]      = ImVec4(0.545f, 0.000f, 0.000f, 1.000f); // brighter crimson
    c[ImGuiCol_TitleBgCollapsed]   = ImVec4(0.200f, 0.000f, 0.000f, 0.800f);

    // ── Scrollbar ────────────────────────────────────────────────────────────
    c[ImGuiCol_ScrollbarBg]        = ImVec4(0.067f, 0.067f, 0.067f, 1.000f);
    c[ImGuiCol_ScrollbarGrab]      = ImVec4(0.545f, 0.412f, 0.078f, 0.800f); // gold grab
    c[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.788f, 0.659f, 0.298f, 1.000f);
    c[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.000f, 0.831f, 1.000f, 1.000f); // cyan snap

    // ── Check mark ───────────────────────────────────────────────────────────
    c[ImGuiCol_CheckMark]          = ImVec4(0.000f, 0.831f, 1.000f, 1.000f); // arc-reactor cyan

    // ── Slider ───────────────────────────────────────────────────────────────
    c[ImGuiCol_SliderGrab]         = ImVec4(0.000f, 0.659f, 0.800f, 1.000f);
    c[ImGuiCol_SliderGrabActive]   = ImVec4(0.000f, 0.831f, 1.000f, 1.000f);

    // ── Buttons (Arc-reactor cyan) ────────────────────────────────────────────
    c[ImGuiCol_Button]             = ImVec4(0.000f, 0.380f, 0.500f, 1.000f); // muted cyan
    c[ImGuiCol_ButtonHovered]      = ImVec4(0.000f, 0.659f, 0.800f, 1.000f); // bright cyan
    c[ImGuiCol_ButtonActive]       = ImVec4(0.000f, 0.831f, 1.000f, 1.000f); // full arc-reactor glow

    // ── Collapsing headers / tree nodes (Crimson) ────────────────────────────
    c[ImGuiCol_Header]             = ImVec4(0.420f, 0.000f, 0.000f, 0.900f); // crimson panel
    c[ImGuiCol_HeaderHovered]      = ImVec4(0.640f, 0.000f, 0.000f, 1.000f); // lighter crimson
    c[ImGuiCol_HeaderActive]       = ImVec4(0.753f, 0.224f, 0.169f, 1.000f); // warm crimson

    // ── Separator / resize ───────────────────────────────────────────────────
    c[ImGuiCol_Separator]          = ImVec4(0.545f, 0.412f, 0.078f, 0.700f); // gold divider
    c[ImGuiCol_SeparatorHovered]   = ImVec4(0.788f, 0.659f, 0.298f, 1.000f);
    c[ImGuiCol_SeparatorActive]    = ImVec4(0.000f, 0.831f, 1.000f, 1.000f);
    c[ImGuiCol_ResizeGrip]         = ImVec4(0.545f, 0.412f, 0.078f, 0.400f);
    c[ImGuiCol_ResizeGripHovered]  = ImVec4(0.788f, 0.659f, 0.298f, 0.800f);
    c[ImGuiCol_ResizeGripActive]   = ImVec4(0.000f, 0.831f, 1.000f, 1.000f);

    // ── Tabs (Crimson inactive, cyan active) ──────────────────────────────────
    c[ImGuiCol_Tab]                = ImVec4(0.280f, 0.000f, 0.000f, 0.900f); // dark crimson
    c[ImGuiCol_TabHovered]         = ImVec4(0.000f, 0.659f, 0.800f, 1.000f); // cyan hover
    c[ImGuiCol_TabActive]          = ImVec4(0.000f, 0.480f, 0.640f, 1.000f); // mid cyan selected
    c[ImGuiCol_TabUnfocused]       = ImVec4(0.180f, 0.000f, 0.000f, 0.900f);
    c[ImGuiCol_TabUnfocusedActive] = ImVec4(0.000f, 0.320f, 0.440f, 1.000f);

    // ── Plot / data lines (gold) ──────────────────────────────────────────────
    c[ImGuiCol_PlotLines]          = ImVec4(0.788f, 0.659f, 0.298f, 1.000f);
    c[ImGuiCol_PlotLinesHovered]   = ImVec4(0.000f, 0.831f, 1.000f, 1.000f);
    c[ImGuiCol_PlotHistogram]      = ImVec4(0.788f, 0.659f, 0.298f, 1.000f);
    c[ImGuiCol_PlotHistogramHovered]= ImVec4(0.000f, 0.831f, 1.000f, 1.000f);

    // ── Text & selection ─────────────────────────────────────────────────────
    c[ImGuiCol_Text]               = ImVec4(0.910f, 0.910f, 0.910f, 1.000f); // crisp white
    c[ImGuiCol_TextDisabled]       = ImVec4(0.400f, 0.400f, 0.400f, 1.000f);
    c[ImGuiCol_TextSelectedBg]     = ImVec4(0.000f, 0.659f, 0.800f, 0.400f); // cyan selection

    // ── Misc ─────────────────────────────────────────────────────────────────
    c[ImGuiCol_DragDropTarget]     = ImVec4(0.000f, 0.831f, 1.000f, 0.900f);
    c[ImGuiCol_NavHighlight]       = ImVec4(0.000f, 0.831f, 1.000f, 1.000f);
    c[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 1.000f, 1.000f, 0.700f);
    c[ImGuiCol_NavWindowingDimBg]  = ImVec4(0.200f, 0.200f, 0.200f, 0.200f);
    c[ImGuiCol_ModalWindowDimBg]   = ImVec4(0.067f, 0.000f, 0.000f, 0.600f); // crimson dim
}


void UIManager::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void UIManager::run() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main Dashboard Window (FullScreen)
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | 
                                        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | 
                                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | 
                                        ImGuiWindowFlags_NoNavFocus;

        // Push some extra padding for the main window content
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
        ImGui::Begin("Dashboard", nullptr, window_flags);
        ImGui::PopStyleVar();
        
        ImGui::TextUnformatted("AUTOMOBILE INSURANCE SYSTEM DASHBOARD");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        if (ImGui::BeginTabBar("ActorTabs", ImGuiTabBarFlags_None)) {
            if (ImGui::BeginTabItem("   Manager   ")) {
                ImGui::Spacing();
                renderManagerTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("   Salesman   ")) {
                ImGui::Spacing();
                renderSalesmanTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("   Surveyor   ")) {
                ImGui::Spacing();
                renderSurveyorTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("   Customer   ")) {
                ImGui::Spacing();
                renderCustomerTab();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

// Helper to render a form row cleanly
static void RenderFormRowString(const char* label, const char* id, char* buf, size_t buf_size) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label);
    
    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(300.0f);
    ImGui::InputText(id, buf, buf_size);
}

static void RenderFormRowDouble(const char* label, const char* id, double* val) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label);
    
    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(300.0f);
    ImGui::InputDouble(id, val);
}

static void RenderFormRowBool(const char* label, const char* id, bool* val) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label);
    
    ImGui::TableSetColumnIndex(1);
    ImGui::Checkbox(id, val);
}

void UIManager::renderManagerTab() {
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Manager Actions");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ── Quick-Fetch Row ───────────────────────────────────────────────────────
    if (ImGui::Button("Fetch New Customers", ImVec2(220, 38))) {
        reportOutput = userCtrl.fetchNewCustomersReport();
        systemMessage = reportOutput.empty() ? "No customers found." : "Fetched New Customers.";
    }
    ImGui::SameLine(0, 12.0f);
    if (ImGui::Button("Fetch Pending Claims", ImVec2(220, 38))) {
        reportOutput = claimCtrl.fetchPendingClaims();
        systemMessage = reportOutput.empty() ? "No pending claims found." : "Fetched Pending Claims.";
    }
    ImGui::SameLine(0, 12.0f);
    if (ImGui::Button("Fetch Inspection Reports", ImVec2(240, 38))) {
        inspReportsOutput = claimCtrl.fetchAllInspectionReports();
        reportOutput = inspReportsOutput;
        systemMessage = inspReportsOutput.empty() ? "No reports on file." :
            "Fetched " + std::to_string(inspReportsOutput.size()) + " report(s).";
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ── Approve / Reject Claim ────────────────────────────────────────────────
    ImGui::Text("Approve / Reject Claim:");
    ImGui::Spacing();
    if (ImGui::BeginTable("MgrApproveTable", 2, ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 180.0f);
        ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
        RenderFormRowString("Approve — Claim ID", "##mgrApproveId", mgrApproveId, IM_ARRAYSIZE(mgrApproveId));
        RenderFormRowString("Reject  — Claim ID", "##mgrRejectId",  mgrRejectId,  IM_ARRAYSIZE(mgrRejectId));
        ImGui::EndTable();
    }
    ImGui::Spacing();
    // Approve button
    if (strlen(mgrApproveId) == 0) {
        ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "Enter Claim ID above to approve.");
    } else {
        if (ImGui::Button("Approve Claim", ImVec2(160, 35))) {
            if (claimCtrl.approveClaim(mgrApproveId))
                systemMessage = "[Approved] Claim '" + std::string(mgrApproveId) + "' approved.";
            else
                systemMessage = "[Error] No inspection report found or claim not pending.";
        }
    }
    ImGui::SameLine(0, 20.0f);
    // Reject button (gold warning color)
    if (strlen(mgrRejectId) == 0) {
        ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "Enter Claim ID above to reject.");
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.42f, 0.00f, 0.00f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.64f, 0.00f, 0.00f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.75f, 0.22f, 0.17f, 1.0f));
        if (ImGui::Button("Reject Claim", ImVec2(160, 35))) {
            if (claimCtrl.rejectClaim(mgrRejectId))
                systemMessage = "[Rejected] Claim '" + std::string(mgrRejectId) + "' rejected.";
            else
                systemMessage = "[Error] Claim not found or not in Pending state.";
        }
        ImGui::PopStyleColor(3);
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ── Register Workshop ─────────────────────────────────────────────────────
    if (ImGui::CollapsingHeader("Register Workshop")) {
        ImGui::Spacing();
        if (ImGui::BeginTable("WshpRegTable", 2, ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 180.0f);
            ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
            RenderFormRowString("Workshop ID *",   "##wshpId",   wshpRegId,   IM_ARRAYSIZE(wshpRegId));
            RenderFormRowString("Workshop Name *", "##wshpName", wshpRegName, IM_ARRAYSIZE(wshpRegName));
            ImGui::EndTable();
        }
        ImGui::Spacing();
        bool wshpBlank = (strlen(wshpRegId) == 0 || strlen(wshpRegName) == 0);
        if (wshpBlank)
            ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "ID and Name are required.");
        else if (ImGui::Button("Register Workshop##btn", ImVec2(200, 35))) {
            if (claimCtrl.registerWorkshop(wshpRegId, wshpRegName))
                systemMessage = "SUCCESS - Generated ID: " + std::string(wshpRegId);
            else
                systemMessage = "ERROR - Failed to save.";
        }
        ImGui::Spacing();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ── Customer Claim History ────────────────────────────────────────────────
    ImGui::Text("Customer Claim History:");
    ImGui::Spacing();
    if (ImGui::BeginTable("MgrHistoryTable", 2, ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 180.0f);
        ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
        RenderFormRowString("Customer ID", "##mgrHistCust", mgrHistoryCustId, IM_ARRAYSIZE(mgrHistoryCustId));
        ImGui::EndTable();
    }
    ImGui::Spacing();
    if (strlen(mgrHistoryCustId) == 0)
        ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "Customer ID cannot be blank.");
    else if (ImGui::Button("Fetch Claim History", ImVec2(200, 35))) {
        claimHistoryOutput = claimCtrl.fetchCustomerClaimHistory(mgrHistoryCustId);
        reportOutput = claimHistoryOutput;
        systemMessage = claimHistoryOutput.empty()
            ? "No claims found for customer '" + std::string(mgrHistoryCustId) + "'."
            : "Fetched " + std::to_string(claimHistoryOutput.size()) + " claim(s) for customer.";
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ── Output Panel ──────────────────────────────────────────────────────────
    ImGui::Text("Output:");
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);
    ImGui::BeginChild("MgrOutput", ImVec2(0, 220), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    for (const auto& line : reportOutput)
        ImGui::TextUnformatted(line.c_str());
    ImGui::EndChild();
    ImGui::PopStyleVar();

    // ── System Message ────────────────────────────────────────────────────────
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.0f, 0.831f, 1.0f, 1.0f), "System Message: %s", systemMessage.c_str());
    if (!systemMessage.empty()) {
        ImGui::SameLine();
        if (ImGui::Button("Copy##mgrCpy")) {
            std::string toCopy = systemMessage;
            size_t pos = systemMessage.find("ID: ");
            if (pos != std::string::npos) toCopy = systemMessage.substr(pos + 4);
            ImGui::SetClipboardText(toCopy.c_str());
        }
    }
}

void UIManager::renderSalesmanTab() {
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Salesman Actions");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ── Register Customer ──────────────────────────────────────────────────
    if (ImGui::CollapsingHeader("Register Customer", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Spacing();
        if (ImGui::BeginTable("SalesRegCustTable", 2, ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 180.0f);
            ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
            RenderFormRowString("Name *", "##regName", regName, IM_ARRAYSIZE(regName));
            RenderFormRowString("Password *", "##regPass", regPass, IM_ARRAYSIZE(regPass));
            RenderFormRowString("Address", "##regAddr", regAddr, IM_ARRAYSIZE(regAddr));
            RenderFormRowString("Date (YYYY-MM-DD)", "##regDate", regDate, IM_ARRAYSIZE(regDate));
            ImGui::EndTable();
        }
        ImGui::Spacing();
        bool custBlank = (strlen(regName) == 0 || strlen(regPass) == 0);
        if (custBlank)
            ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "Name and Password are required.");
        else if (ImGui::Button("Register Customer##btn", ImVec2(200, 35))) {
            std::string newID = userCtrl.registerCustomer(regName, regPass, regAddr, regDate);
            if (!newID.empty()) systemMessage = "SUCCESS - Generated ID: " + newID;
            else systemMessage = "ERROR - Failed to save.";
        }
        ImGui::Spacing();
    }

    ImGui::Spacing();

    // ── Register Vehicle ───────────────────────────────────────────────────
    if (ImGui::CollapsingHeader("Register Vehicle")) {
        ImGui::Spacing();
        if (ImGui::BeginTable("SalesRegVehTable", 2, ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 180.0f);
            ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
            RenderFormRowString("Owner (Cust ID) *", "##vehOwner", vehOwner, IM_ARRAYSIZE(vehOwner));
            RenderFormRowString("Make *", "##vehMake", vehMake, IM_ARRAYSIZE(vehMake));
            RenderFormRowString("License Plate *", "##vehPlate", vehPlate, IM_ARRAYSIZE(vehPlate));
            ImGui::EndTable();
        }
        ImGui::Spacing();
        bool vehBlank = (strlen(vehOwner) == 0 || strlen(vehMake) == 0 || strlen(vehPlate) == 0);
        if (vehBlank)
            ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "Owner ID, Make, and Plate are required.");
        else if (ImGui::Button("Register Vehicle##btn", ImVec2(200, 35))) {
            std::string newID = userCtrl.registerVehicle(vehOwner, vehMake, vehPlate);
            if (!newID.empty()) systemMessage = "SUCCESS - Generated ID: " + newID;
            else systemMessage = "ERROR - Failed to save.";
        }
        ImGui::Spacing();
    }

    ImGui::Spacing();

    // ── Issue Policy ───────────────────────────────────────────────────────
    if (ImGui::CollapsingHeader("Issue Policy")) {
        ImGui::Spacing();
        if (ImGui::BeginTable("SalesIssuePolTable", 2, ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 180.0f);
            ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
            RenderFormRowString("Vehicle ID *", "##polVeh", polVeh, IM_ARRAYSIZE(polVeh));
            RenderFormRowDouble("Premium Amount *", "##polPrem", &polPremium);
            ImGui::EndTable();
        }
        ImGui::Spacing();
        bool polBlank = (strlen(polVeh) == 0);
        if (polBlank)
            ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "Vehicle ID is required.");
        else if (ImGui::Button("Issue Policy##btn", ImVec2(200, 35))) {
            std::string newID = userCtrl.issuePolicy(polVeh, polPremium);
            if (!newID.empty()) systemMessage = "SUCCESS - Generated ID: " + newID;
            else systemMessage = "ERROR - Failed to save.";
        }
        ImGui::Spacing();
    }

    // ── System Message ────────────────────────────────────────────────────────
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.0f, 0.831f, 1.0f, 1.0f), "System Message: %s", systemMessage.c_str());
    if (!systemMessage.empty()) {
        ImGui::SameLine();
        if (ImGui::Button("Copy##slsCpy")) {
            std::string toCopy = systemMessage;
            size_t pos = systemMessage.find("ID: ");
            if (pos != std::string::npos) toCopy = systemMessage.substr(pos + 4);
            ImGui::SetClipboardText(toCopy.c_str());
        }
    }
}

void UIManager::renderCustomerTab() {
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Customer Actions");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ── File Claim ───────────────────────────────────────────────────────
    if (ImGui::CollapsingHeader("File Claim", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Spacing();
        if (ImGui::BeginTable("CustFileClaimTable", 2, ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 180.0f);
            ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
            RenderFormRowString("Policy ID *", "##claimPol", claimPol, IM_ARRAYSIZE(claimPol));
            RenderFormRowString("Description *", "##claimDesc", claimDesc, IM_ARRAYSIZE(claimDesc));
            ImGui::EndTable();
        }
        ImGui::Spacing();
        bool claimBlank = (strlen(claimPol) == 0 || strlen(claimDesc) == 0);
        if (claimBlank)
            ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "Policy ID and Description are required.");
        else if (ImGui::Button("File Claim##btn", ImVec2(200, 35))) {
            std::string newID = claimCtrl.fileClaim(claimPol, claimDesc);
            if (!newID.empty()) systemMessage = "SUCCESS - Generated ID: " + newID;
            else systemMessage = "ERROR - Failed to save.";
        }
        ImGui::Spacing();
    }

    ImGui::Spacing();

    // ── View Policies ───────────────────────────────────────────────────────
    if (ImGui::CollapsingHeader("View Policies")) {
        ImGui::Spacing();
        if (ImGui::BeginTable("CustViewPolTable", 2, ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 180.0f);
            ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
            RenderFormRowString("Customer ID *", "##custView", custViewId, IM_ARRAYSIZE(custViewId));
            ImGui::EndTable();
        }
        ImGui::Spacing();
        bool viewBlank = (strlen(custViewId) == 0);
        if (viewBlank)
            ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "Customer ID is required.");
        else if (ImGui::Button("Fetch Policies", ImVec2(200, 35))) {
            custPolicies = userCtrl.fetchCustomerPolicies(custViewId);
            systemMessage = custPolicies.empty()
                ? "No active policies for ID: " + std::string(custViewId)
                : "Fetched " + std::to_string(custPolicies.size()) + " policy/policies.";
        }
        ImGui::Spacing();
        ImGui::Text("Policies Output:");
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);
        ImGui::BeginChild("CustPolOutput", ImVec2(0, 200), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        for (const auto& p : custPolicies)
            ImGui::TextUnformatted(p.c_str());
        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::Spacing();
    }

    // ── System Message ────────────────────────────────────────────────────────
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.0f, 0.831f, 1.0f, 1.0f), "System Message: %s", systemMessage.c_str());
    if (!systemMessage.empty()) {
        ImGui::SameLine();
        if (ImGui::Button("Copy##cstCpy")) {
            std::string toCopy = systemMessage;
            size_t pos = systemMessage.find("ID: ");
            if (pos != std::string::npos) toCopy = systemMessage.substr(pos + 4);
            ImGui::SetClipboardText(toCopy.c_str());
        }
    }
}


void UIManager::renderSurveyorTab() {
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Surveyor Actions");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ── Submit Inspection Report ─────────────────────────────────────────────
    if (ImGui::CollapsingHeader("Submit Inspection Report", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Spacing();
        if (ImGui::BeginTable("SurvSubRepTable", 2, ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 180.0f);
            ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
            RenderFormRowString("Associated Claim ID *", "##repClaim", repClaim, IM_ARRAYSIZE(repClaim));
            RenderFormRowString("Workshop ID *", "##repWork", repWork, IM_ARRAYSIZE(repWork));
            RenderFormRowBool("Total Loss?", "##repLoss", &repTotalLoss);
            RenderFormRowDouble("Estimated Cost", "##repCost", &repCost);
            ImGui::EndTable();
        }
        ImGui::Spacing();
        bool repBlank = (strlen(repClaim) == 0 || strlen(repWork) == 0);
        if (repBlank)
            ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "Claim ID and Workshop ID are required.");
        else if (ImGui::Button("Submit Report", ImVec2(200, 35))) {
            std::string newID = claimCtrl.submitInspectionReport(repClaim, repWork, repTotalLoss, repCost);
            if (!newID.empty())
                systemMessage = "SUCCESS - Generated ID: " + newID;
            else
                systemMessage = "ERROR - Failed to save.";
        }
        ImGui::Spacing();
    }

    // ── System Message ────────────────────────────────────────────────────────
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.0f, 0.831f, 1.0f, 1.0f), "System Message: %s", systemMessage.c_str());
    if (!systemMessage.empty()) {
        ImGui::SameLine();
        if (ImGui::Button("Copy##srvCpy")) {
            std::string toCopy = systemMessage;
            size_t pos = systemMessage.find("ID: ");
            if (pos != std::string::npos) toCopy = systemMessage.substr(pos + 4);
            ImGui::SetClipboardText(toCopy.c_str());
        }
    }
}
