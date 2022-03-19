//#include "EditorUI.h"
//
//Editor::Editor()
//{
//    new_project = false;
//    folder_path = "";
//}
//
//void Editor::init_GUI(GLFWwindow* window) {
//
//
//    // GL 3.0 + GLSL 130
//    const char* glsl_version = "#version 130";
//
//
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//
//    // Setup Dear ImGui style
//    ImGui::StyleColorsDark();
//    //ImGui::StyleColorsClassic();
//
//    // Setup Platform/Renderer backends
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init(glsl_version);
//
//}
//
//void Editor::file_input() {
//
//    ImGui::Begin("New Window");
//
//    if (ImGui::TreeNode("Assets"))
//    {
//
//        static const char* items[]{ "Objects","Textures","Animations" };
//        static const char* leaves[]{ "cool_object.obj","sphere.obj","another_object.obj" };
//        static bool is_selected = true;
//        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
//        static bool align_label_with_current_x_position = false;
//        static bool test_drag_and_drop = false;
//        if (align_label_with_current_x_position)
//            ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
//
//        static int selection_mask = (1 << 2);
//        int node_clicked = -1;
//        for (int i = 0; i < 3; i++)
//        {
//
//            ImGuiTreeNodeFlags node_flags = base_flags;
//
//            const bool is_selected = (selection_mask & (1 << i)) != 0;
//            if (is_selected)
//                node_flags |= ImGuiTreeNodeFlags_Selected;
//            // Items 0..2 are Tree Node
//            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, items[i]);
//            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
//                node_clicked = i;
//            if (node_open) {
//                static int selected = -1;
//                for (int n = 0; n < 3; n++)
//                {
//
//
//                    if (ImGui::Selectable(leaves[n], selected == n, ImGuiSelectableFlags_AllowDoubleClick)) {
//                        selected = n;
//                        if (ImGui::IsMouseDoubleClicked(0))
//                            std::cout << leaves[n] << "\n";
//                    }
//
//                }
//                ImGui::TreePop();
//            }
//
//        }
//        if (node_clicked != -1)
//        {
//            // Update selection state
//            // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
//            if (ImGui::GetIO().KeyCtrl)
//                selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
//            else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
//                selection_mask = (1 << node_clicked);           // Click to single-select
//        }
//        ImGui::TreePop();
//    }
//
//    ImGui::End();
//}
//#if defined(_WIN32)
//std::string Editor::OpenFileDialogue() {
//    OPENFILENAME ofn;
//    wchar_t fileName[MAX_PATH] = L"";
//    ZeroMemory(&ofn, sizeof(ofn));
//
//    ofn.lStructSize = sizeof(OPENFILENAME);
//    ofn.hwndOwner = NULL;
//    ofn.lpstrFilter = L"All Files (*.*)\0*.cpp\0";
//    ofn.lpstrFile = fileName;
//    ofn.nMaxFile = MAX_PATH;
//    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
//    ofn.lpstrDefExt = L"";
//
//    if (GetSaveFileName(&ofn)) {
//        std::wstring ws(fileName);
//        // your new String
//        std::string fileNameStr(ws.begin(), ws.end());
//        return fileNameStr;
//    }
//    else
//        return "N/A";
//}
//
//std::string Editor::OpenFolderDialogue() {
//    wchar_t path[MAX_PATH] = L"";
//    BROWSEINFO bi;
//
//    bi.hwndOwner = NULL;
//    bi.pidlRoot = NULL;
//    bi.pszDisplayName = path;	// This is just for display: not useful
//    bi.lpszTitle = L"Choose Client Directory";
//    bi.ulFlags = BIF_RETURNONLYFSDIRS;
//    bi.lpfn = NULL;
//    bi.lParam = 0;
//    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
//    if (SHGetPathFromIDList(pidl, path)) {
//        std::wstring ws(path);
//        std::string pathstr(ws.begin(), ws.end());
//        return pathstr;
//    }
//    else
//        return "N/A";
//    
//}
//#else
//std::string Editor::OpenFileDialogue() {
//    char filename[1024];
//    FILE* f = popen("zenity --file-selection --file-filter=*.cpp","r");
//    if (f == NULL)
//        return "N/A";
//    else{
//        fgets(filename, 1024, f);
//        std::string fileNameStr;
//        fileNameStr = filename;
//        return fileNameStr;
//    }
//    
//}
//
//std::string Editor::OpenFolderDialogue() {
//    char foldername[1024];
//    FILE* f = popen("zenity  --file-selection --directory", "r");
//    if (f == NULL)
//        return "N/A";
//    else {
//        fgets(foldername, 1024, f);
//        std::string folderNameStr;
//        folderNameStr = foldername;
//        return folderNameStr;
//    }
//
//}
//#endif
//
//void Editor::newProject() {
//
//    ImGui::SetNextWindowSize(ImVec2(300, 100));
//    ImGui::Begin("Choose new porject directory", &new_project, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
//    static char buf1[64] = ""; 
//    static char buf2[MAX_PATH] = "";
//    for (int i = 0; i < folder_path.length(); i++) {
//        buf2[i] = folder_path[i];
//    }
//    std::string check = "pokemon";
//    ImGui::PushItemWidth(-1);
//    ImGui::InputTextWithHint("##pname", "Project Name", buf1, 64);
//    ImGui::PopItemWidth();
//    if (ImGui::InputTextWithHint("##ppath", "Project Directory", buf2, 64)) {
//        folder_path = buf2;
//    }
//    ImGui::SameLine();
//    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2);
//    if (ImGui::Button("  Browse  ")) {
//        std::string temp_path = OpenFolderDialogue();
//        if (temp_path.compare("N/A")!=0)
//            folder_path = temp_path;
//            
//    }
//    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
//    if (ImGui::Button("Confirm")) {
//        if (strlen(buf1) != 0 && strlen(buf2) != 0) {
//            FileModule newModule;
//            newModule.create_workdir(buf2, buf1);
//            new_project = false;
//        }
//
//    }
//    ImGui::SameLine(ImGui::GetWindowWidth() - 59);
//    if (ImGui::Button("Cancel")) {
//            new_project = false;
//    }
//    
//
//    ImGui::End();
//
//}
//
//void Editor::menubar() {
//
//    if (ImGui::BeginMainMenuBar())
//    {
//        
//        if (ImGui::BeginMenu("File"))
//        {
//            ImGui::MenuItem("New Project", NULL, &new_project);
//            if (ImGui::MenuItem("Open Project", "Ctrl+O")) {
//
//                std::cout << OpenFileDialogue().c_str() << std::endl;
//
//            }
//            if (ImGui::MenuItem("Save Project", "Ctrl+S")) {
//
//                
//
//            }
//            ImGui::Separator();
//            if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
//
//                
//
//            }
//            if (ImGui::MenuItem("Open Scene", "Ctrl+Shift+O")) {
//
//                
//
//            }
//            if (ImGui::MenuItem("Save Scene", "Ctrl+Shift+S")) {
//
//               
//
//            }
//            ImGui::EndMenu();
//        }
//
//        if (ImGui::BeginMenu("Edit"))
//        {
//            if (ImGui::MenuItem("Select All", "Ctrl+A")) {
//
//            }
//            if (ImGui::MenuItem("Deselect All", "Ctrl+D")) {
//
//            }
//            if (ImGui::MenuItem("Select Children", "Shift+C")) {
//
//            }
//            if (ImGui::MenuItem("Invert Selection", "Ctrl+I")) {
//
//            }
//            ImGui::Separator();
//            if (ImGui::MenuItem("Cut", "Ctrl+X")) {
//
//            }
//            if (ImGui::MenuItem("Copy", "Ctrl+C")) {
//
//            }
//            if (ImGui::MenuItem("Paste", "Ctrl+V")) {
//
//            }
//            ImGui::Separator();
//            if (ImGui::MenuItem("Play", "Ctrl+P")) {
//
//            }
//            if (ImGui::MenuItem("Pause", "Ctrl+Shift+P")) {
//
//            }
//            ImGui::EndMenu();
//        }
//
//        if (ImGui::BeginMenu("Assets"))
//        {
//            if (ImGui::MenuItem("Add Path")) {
//
//            }
//            if (ImGui::MenuItem("Add Script")) {
//
//            }
//            if (ImGui::MenuItem("Delete Script")) {
//
//            }
//            if (ImGui::MenuItem("Add Attribute")) {
//
//            }
//            if (ImGui::MenuItem("Delete Attribute")) {
//
//            }
//            ImGui::EndMenu();
//        }
//
//        if (ImGui::BeginMenu("Object"))
//        {
//            ImGui::EndMenu();
//        }
//
//        if (ImGui::BeginMenu("Help"))
//        {
//            ImGui::EndMenu();
//        }
//        ImGui::EndMainMenuBar();
//    }
//    
//}