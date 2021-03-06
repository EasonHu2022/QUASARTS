#include "MenuBarView.h"
#include <iostream>
// Decide OS
#if defined(_WIN32)
#include <windows.h>
#include <commdlg.h>
#include <direct.h>
#include <ShlObj_core.h>
#endif

// engine
#include "ECS/System/CollisionSystem.h"
#include "ECS/Showcase-Video.h"

void MenuBarView::on_add()
{
    new_project = false;
    new_scene = false;
    new_entity = false;
    new_child = false;
    new_script = false;
    folder_path = "";
    project_name = "";
    QDEBUG("on add view : MenuBar");
}

void MenuBarView::on_gui()
{
    //QDEBUG("hi - {0}", new_project);
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New Project", NULL, &new_project);
            if (ImGui::MenuItem("Open Project", "Ctrl+O")) {

                //std::cout << OpenFileDialogue().c_str() << std::endl;
                std::string proj_file;
                #ifdef QS_WINDOWS
                    proj_file = OpenFileDialogue(L"All Files (*.*)\0*.q\0");
                #else
                    proj_file = OpenFileDialogue("\"\"*.q\"\"");
                #endif
                if(proj_file.compare("N/A")!=0) {
                    // Get project name and folder path:
                    int count = 0;
                    for (int i = (int)(proj_file.size()) - 1; i >= 0; i--) {
                        if (proj_file[i] == '/' || proj_file[i] == '\\') {
                            count++;
                            if (count == 1) {
                                project_name = proj_file.substr(i + 1);
                            } else if (count == 2) {
                                folder_path = proj_file.substr(0, i);
                                break;
                            }
                            // Take the extension off project name:
                            for (int j = (int)project_name.size() - 1; j >= 0; j--) {
                                if (project_name[j] == '.') {
                                    project_name.erase(project_name.begin() + j, project_name.end());
                                    break;
                                }
                            }
                        }
                    }
                    FileModule::Instance()->open_root(proj_file);
                }
            }
            if (ImGui::MenuItem("Save Project", "Ctrl+S")) {
                
            }
            ImGui::Separator();
            ImGui::MenuItem("New Scene", "Ctrl+N", &new_scene);
            if (ImGui::MenuItem("Open Scene", "Ctrl+Shift+O")) {
                #ifdef QS_WINDOWS
                    std::string file_name = OpenFileDialogue(L"All Files (*.*)\0*.q\0");
                #else
                    std::string file_name = OpenFileDialogue("\"\"*.scn\"\"");
                #endif
                if(file_name.compare("N/A")!=0) {
                    Engine::CollisionSystem::Instance()->reset();
                    Engine::EventModule::Instance()->clear_queue();
                    Engine::ECSManager::Instance()->load_scene((char*)file_name.c_str());
                }
            }
            if (ImGui::MenuItem("Save Scene", "Ctrl+Shift+S")) {
                std::string scene_name = Engine::ECSManager::Instance()->get_scene_name();
                std::string file_name = folder_path + "/" + project_name +
                                            "/ProjectSetting/" + scene_name + ".scn";
                Engine::ECSManager::Instance()->save_scene((char*)file_name.c_str());
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Select All", "Ctrl+A")) {

            }
            if (ImGui::MenuItem("Deselect All", "Ctrl+Shift+D")) {

            }
            if (ImGui::MenuItem("Select Children", "Shift+C")) {

            }
            if (ImGui::MenuItem("Invert Selection", "Ctrl+I")) {

            }
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {

            }
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {

            }
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {

            }
            ImGui::Separator();
            if (ImGui::MenuItem("Play", "Ctrl+P")) {               
                Engine::ScriptSystem::Instance()->loadScripts();
            }
            if (ImGui::MenuItem("Stop", "Ctrl+Shift+P")) {
                Engine::ScriptSystem::Instance()->refreshScript();

            }
            ImGui::Separator();
            if (ImGui::MenuItem("Delete", "Ctrl+D")) {
                if (Engine::ECSManager::Instance()->get_current_entity() != TOO_MANY_ENTITIES) {
                    Engine::ECSManager::Instance()->destroy_entity(Engine::ECSManager::Instance()->get_current_entity());
                    Engine::ECSManager::Instance()->set_current_entity(TOO_MANY_ENTITIES);
                }
            }
            if (ImGui::MenuItem("Delete All")) {
                std::vector<unsigned int> entities = Engine::ECSManager::Instance()->get_entity_ID_match();
                unsigned int cameraID = Engine::ECSManager::Instance()->get_camera();
                for (int i = 1; i < entities.size(); i++) {
                    if (entities[i] == cameraID)
                        continue;
                    Engine::ECSManager::Instance()->destroy_entity(entities[i]);
                }
                Engine::ECSManager::Instance()->set_current_entity(TOO_MANY_ENTITIES);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Assets"))
        {
            ImGui::MenuItem("New Script", NULL, &new_script);
            if (ImGui::MenuItem("Add Script Component")) {
                
                unsigned int entityID = Engine::ECSManager::Instance()->get_current_entity();

                if (entityID == TOO_MANY_ENTITIES)
                {
                    QERROR("Please select an entity first :)");
                }
                else
                {

                    Engine::ECSManager::Instance()->create_component<Engine::ScriptComponent>(entityID, COMPONENT_SCRIPT);

                    auto script = Engine::ECSManager::Instance()->get_component<Engine::ScriptComponent>(entityID, COMPONENT_SCRIPT);

                    std::string script_file;
                    #ifdef QS_WINDOWS
                        script_file = OpenFileDialogue(L"All Files (*.*)\0*.lua\0");
                    #else
                        script_file = OpenFileDialogue("\"\"*.lua\"\"");
                    #endif
                    //std::cout << script_file << std::endl;
                    QDEBUG("Successfully added script component to entity:{0}, path: {1}", entityID, script_file);

                    //init
                    Engine::ScriptSystem::Instance()->initComponent(script, script_file, entityID);
                    Engine::ScriptSystem::Instance()->loadScript(script);
                }

            }
            if (ImGui::MenuItem("Delete Script")) {

                //Engine::ScriptSystem::Instance()->deleteScript();
            }
            if (ImGui::BeginMenu("Add Attribute"))
            {
                if (ImGui::MenuItem("Mesh")) {
                    newAttribute(COMPONENT_MESH);
                }
                if (ImGui::MenuItem("Collision Sphere")) {
                    newAttribute(COMPONENT_COLLISION_SPHERE);
                }
                if (ImGui::MenuItem("Material")) {
                    newAttribute(COMPONENT_MATERIAL);
                }
                if (ImGui::MenuItem("Lighting")) {
                    newAttribute(COMPONENT_LIGHTING);
                }
                if (ImGui::MenuItem("Script")) {
                    newAttribute(COMPONENT_SCRIPT);
                }
                if (ImGui::MenuItem("Camera")) {
                    newAttribute(COMPONENT_CAMERA);
                }
                if (ImGui::MenuItem("Orbit")) {
                    newAttribute(COMPONENT_ORBIT);
                }
                if (ImGui::MenuItem("Health")) {
                    newAttribute(COMPONENT_HEALTH);
                }
                if (ImGui::MenuItem("Weapon")) {
                    newAttribute(COMPONENT_WEAPON);
                }
                if (ImGui::MenuItem("Particle")) {
                    newAttribute(COMPONENT_PARTICLE);
                }
                if (ImGui::MenuItem("Enemy Spawner")) {
                    newAttribute(COMPONENT_ENEMY_SPAWNER);
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Delete Attribute")) {

            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Object"))
        {
            ImGui::MenuItem("Blank Entity", NULL, &new_entity);
            if (ImGui::MenuItem("Child Entity")) {
                if (Engine::ECSManager::Instance()->get_current_entity() != TOO_MANY_ENTITIES)
                    new_child = true;
            }
            if (ImGui::BeginMenu("Insert Basic Object")) {
                if (ImGui::MenuItem("Triangle")) {
                    load_object("Triangle", "/triangle.obj");
                }
                if (ImGui::MenuItem("Plane")) {
                    load_object("Plane", "/triangle_groundplane.obj");
                }
                if (ImGui::MenuItem("Pyramid")) {
                    load_object("Pyramid", "/tetrahedron_smooth.obj");
                }
                if (ImGui::MenuItem("Cube")) {
                    load_object("Cube", "/cube_smooth.obj");
                }
                if (ImGui::MenuItem("Cone")) {
                    load_object("Cone", "/Cone.obj");
                }
                if (ImGui::MenuItem("Sphere")) {
                    load_object("Sphere", "/sphere20x20.obj");
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Camera")) {

            }
            if (ImGui::MenuItem("Light")) {

                unsigned int entityID = Engine::ECSManager::Instance()->create_entity();
                Engine::ECSManager::Instance()->set_entityName(entityID, "Light");
                Engine::ECSManager::Instance()->create_component<Engine::LightComponent>(entityID, COMPONENT_LIGHTING);
                Engine::LightComponent *light = Engine::ECSManager::Instance()->get_component<Engine::LightComponent>(entityID, COMPONENT_LIGHTING);
                light->ambient = { 1.0f, 1.0f, 1.0f };
                light->type = Engine::LightType::parallel;

                Engine::ECSManager::Instance()->create_component<Engine::TransformComponent>(entityID, COMPONENT_TRANSFORM);
                Engine::TransformComponent *transform = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(entityID, COMPONENT_TRANSFORM);
                transform->position = { 1.0f,2.5f, 0.0f };
            }
            if (ImGui::MenuItem("Particle Emitter")) {
                unsigned int entityID = Engine::ECSManager::Instance()->create_entity();
                Engine::ECSManager::Instance()->set_entityName(entityID, "Emitter");
                Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(entityID, COMPONENT_TRANSFORM)->position = { 0.0f,0.0f,-1.0f };
                Engine::ParticleComponent particle;
                Engine::ECSManager::Instance()->create_component<Engine::ParticleComponent>(entityID, COMPONENT_PARTICLE, particle);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Game"))
        {
            if (ImGui::MenuItem("Construct Solar System")) {
		        Engine::construct_solar_system();
            }
            if (ImGui::MenuItem("Place Four Enemy Spawners")) {
                Engine::entity_stress_test();
            }
            if (ImGui::MenuItem("Start Showcase Intro")) {
                Engine::ShowcaseVideo::Instance()->create_video_intro();
            }
            if (ImGui::MenuItem("Start Script Test")) {
                Engine::ShowcaseVideo::Instance()->create_script_test(0);
            }
            if (ImGui::MenuItem("Start Script Test Closeup")) {
                Engine::ShowcaseVideo::Instance()->create_script_test(1);
            }
            if (ImGui::MenuItem("Give the Player a Weapon")) {
                Engine::ShowcaseVideo::Instance()->give_player_weapon();
            }
            if (ImGui::MenuItem("Start Level Demo"))
            {
                Engine::ShowcaseVideo::Instance()->start_level_demo();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (new_project)
        newProject();
    if (new_scene)
        newScene();
    if (new_script)
        newScript();
    if (new_entity)
        newEntity();
    if (new_child)
        newChild();
}

void MenuBarView::on_remove()
{
    QDEBUG("on remove view : MenuBar");
}


#ifdef QS_WINDOWS
std::string MenuBarView::OpenFileDialogue(const wchar_t* filter) {
    OPENFILENAME ofn;
    wchar_t fileName[260] = L"";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = 260;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"";

    if (GetOpenFileName(&ofn)) {

        std::wstring ws(fileName);
        // your new String
        std::string fileNameStr(ws.begin(), ws.end());
        return fileNameStr;
    }
    else
        return "N/A";
}
std::string MenuBarView::OpenFolderDialogue() {
    wchar_t path[260] = L"";
    BROWSEINFO bi;

    bi.hwndOwner = NULL;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = path;	// This is just for display: not useful
    bi.lpszTitle = L"Choose Client Directory";
    bi.ulFlags = BIF_RETURNONLYFSDIRS;
    bi.lpfn = NULL;
    bi.lParam = 0;
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (SHGetPathFromIDList(pidl, path)) {
        std::wstring ws(path);
        std::string pathstr(ws.begin(), ws.end());
        //Engine::Application::Instance->loaderFactory->load(pathstr, pathstr);
        return pathstr;
    }
    else
        return "N/A";
}
#else
std::string MenuBarView::OpenFileDialogue(std::string filter) {
    std::string command = "zenity --file-selection --file-filter=";
    command += filter;
    char filename[1024];
    FILE* f = popen(command.c_str(), "r");
    if (f == nullptr)
        return "N/A";
    else {
        if (fgets(filename, 1024, f) == nullptr) {
            return "N/A";
        } else {
            std::string fileNameStr;
            fileNameStr = filename;
            // Remove newline that appears on Linux:
            std::size_t position = fileNameStr.find('\n');
            if (position != std::string::npos) {
                fileNameStr.erase(fileNameStr.begin() + position, fileNameStr.end());
            }
            return fileNameStr;
        }
    }

}
std::string MenuBarView::OpenFolderDialogue() {
    char foldername[1024];
    FILE* f = popen("zenity --file-selection --directory", "r");
    if (f == NULL)
        return "N/A";
    else {
        if (fgets(foldername, 1024, f) == nullptr) {
            return "N/A";
        } else {
            std::string folderNameStr;
            folderNameStr = foldername;
            return folderNameStr;
        }
    }

}
#endif

void MenuBarView::newProject() {

    ImGui::SetWindowFocus("Choose new project directory");

    ImGui::SetNextWindowSize(ImVec2(300, 100));
    ImGui::Begin("Choose new project directory", &new_project, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    static char buf1[64] = "";
    static char buf2[260] = "";
    for (int i = 0; i < folder_path.length(); i++) {
        buf2[i] = folder_path[i];
    }

    ImGui::PushItemWidth(-1);
    if (ImGui::InputTextWithHint("##pname", "Project Name", buf1, 64))
    {
        project_name = buf1;
    }
    ImGui::PopItemWidth();
    if (ImGui::InputTextWithHint("##ppath", "Project Directory", buf2, 260)) {
        folder_path = buf2;
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2);
    if (ImGui::Button("  Browse  ")) {
        std::string temp_path = OpenFolderDialogue();
        if (temp_path.compare("N/A") != 0) {
            folder_path = temp_path;
            #ifndef QS_WINDOWS
                // Remove newline that appears on Linux:
                std::size_t position = folder_path.find('\n');
                if (position != std::string::npos) {
                    folder_path.erase(folder_path.begin() + position, folder_path.end());
                }
            #endif
        }
    }
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Confirm")) {
        if (strlen(buf1) != 0 && strlen(buf2) != 0) {

            FileModule::Instance()->create_workdir(buf2, buf1);
            FileModule::Instance()->save_root(buf2, buf1);

            new_project = false;
            show_window = true;
           
        }
    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 59);
    if (ImGui::Button("Cancel")) {
        new_project = false;
    }

    ImGui::End();
}

void MenuBarView::newScene() {

    ImGui::SetWindowFocus("Choose Scene Name");

    ImGui::SetNextWindowSize(ImVec2(300, 100));
    ImGui::Begin("Choose Scene Name", &new_scene, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    static char buf1[64] = "";

    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##pname", "Scene Name", buf1, 64);
    ImGui::PopItemWidth();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Confirm")) {
        if (strlen(buf1) != 0) {
            Engine::CollisionSystem::Instance()->reset();
            Engine::EventModule::Instance()->clear_queue();
            Engine::ECSManager::Instance()->new_scene(std::string(buf1));
            new_scene = false;
            show_window = true;
        }

    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 59);
    if (ImGui::Button("Cancel")) {
        new_scene = false;
    }

    ImGui::End();
}

void MenuBarView::newScript() {

    ImGui::SetWindowFocus("Choose Script Name");

    ImGui::SetNextWindowSize(ImVec2(250, 80));
    ImGui::Begin("Choose Script Name", &new_script, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    static char buf1[64] = "";

    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##pname", "Script Name", buf1, 64);
    ImGui::PopItemWidth();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Confirm")) {
        if (strlen(buf1) != 0) {

            if (FileModule::Instance()->get_root() != nullptr) 
            {
                std::string file_path = (std::string)FileModule::Instance()->get_root()->path + "/Scripts";
                Engine::ScriptSystem::Instance()->createScript(buf1, file_path);
            }
            else 
            {
                QWARN("Failed to create the script, Please create a project first");
            }
                
            new_script = false;
            show_window = true;
        }
    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 59);
    if (ImGui::Button("Cancel")) {
        new_script = false;
    }

    ImGui::End();
}

void MenuBarView::newEntity() {

    ImGui::SetWindowFocus("Choose Entity Name");

    ImGui::SetNextWindowSize(ImVec2(300, 100));
    ImGui::Begin("Choose Entity Name", &new_entity, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    static char buf1[64] = "";

    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##pname", "Entity Name", buf1, 64);
    ImGui::PopItemWidth();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Confirm")) {
        if (strlen(buf1) != 0) {

            unsigned int entityID = Engine::ECSManager::Instance()->create_entity();
            Engine::ECSManager::Instance()->set_entityName(entityID, buf1);
            new_entity = false;
            buf1[0] = '\0';
        }

    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 59);
    if (ImGui::Button("Cancel")) {
        new_entity = false;
    }

    ImGui::End();
}

void MenuBarView::newChild() {

    ImGui::SetWindowFocus("Choose Child Name");

    ImGui::SetNextWindowSize(ImVec2(300, 100));
    ImGui::Begin("Choose Child Name", &new_child, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    static char buf1[64] = "";

    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##pname", "Entity Name", buf1, 64);
    ImGui::PopItemWidth();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Confirm")) {
        if (strlen(buf1) != 0) {

            unsigned int entityID = Engine::ECSManager::Instance()->create_entity();

            Engine::ECSManager::Instance()->set_entityName(entityID, buf1);
            Engine::ECSManager::Instance()->add_child(Engine::ECSManager::Instance()->get_current_entity(), entityID);
            new_child = false;
            buf1[0] = '\0';

        }
    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 59);
    if (ImGui::Button("Cancel")) {
        new_child = false;
    }

    ImGui::End();
}

void MenuBarView::newAttribute(const int componentType)
{
    unsigned int entityId = Engine::ECSManager::Instance()->get_current_entity();

    if (entityId == TOO_MANY_ENTITIES)
    {
        WARN("Could not add attribute: selected entity is invalid.");
        return;
    }

    if (componentType == COMPONENT_MESH)
    {
        createMesh(entityId);
        createMaterial(entityId);
    }
    else if (componentType == COMPONENT_COLLISION_SPHERE)
    {
        Engine::ECSManager::Instance()->create_component<Engine::CollisionSphereComponent>(entityId, componentType);
        Engine::CollisionSystem::Instance()->init_collision_component(entityId, COMPONENT_COLLISION_SPHERE);
    }
    else if (componentType == COMPONENT_MATERIAL)
    {
		createMaterial(entityId);
    }
    else if (componentType == COMPONENT_LIGHTING)
    {
        WARN("Component creator not implemented in newAttribute(): COMPONENT_LIGHTING");
    }
    else if (componentType == COMPONENT_SCRIPT)
    {
        WARN("Component creator not implemented in newAttribute(): COMPONENT_SCRIPT");
    }
    else if (componentType == COMPONENT_CAMERA)
    {
        WARN("Component creator not implemented in newAttribute(): COMPONENT_CAMERA");
    }
    else if (componentType == COMPONENT_ORBIT)
    {
        Engine::ECSManager::Instance()->create_component<Engine::OrbitComponent>(entityId, componentType);
    }
    else if (componentType == COMPONENT_HEALTH)
    {
        Engine::HealthComponent health{100.f, 100.f};
        Engine::ECSManager::Instance()->create_component(entityId, COMPONENT_HEALTH, health);
    }
    else if (componentType == COMPONENT_WEAPON)
    {
        Engine::WeaponComponent weapon{10.f, 7.5, 0.25, 0.f};
        Engine::ECSManager::Instance()->create_component(entityId, COMPONENT_WEAPON, weapon);
    }
    else if (componentType == COMPONENT_PARTICLE)
    {
        Engine::ECSManager::Instance()->create_component<Engine::ParticleComponent>(entityId, COMPONENT_PARTICLE);
    }
    else if (componentType == COMPONENT_ENEMY_SPAWNER)
    {
        Engine::ECSManager::Instance()->create_component<Engine::EnemySpawnComponent>(entityId, COMPONENT_ENEMY_SPAWNER);
    }
} // newAttribute()

void MenuBarView::load_object(std::string name, std::string file) {
    
    unsigned int entityID = Engine::ECSManager::Instance()->create_entity();
    Engine::ECSManager::Instance()->set_entityName(entityID, name);
    Engine::TransformComponent transform;
    transform.position = { 0.0f,0.0f, 0.0f };
    transform.rotation = { 0.0f,0.0f, 0.0f };
    transform.scale = { 1.0f,1.0f, 1.0f };
    Engine::ECSManager::Instance()->create_component<Engine::TransformComponent>(entityID, COMPONENT_TRANSFORM, transform);

    Engine::MeshComponent mesh;
    auto path = FileModule::Instance()->get_internal_assets_path();
    mesh.path = path + "DefaultObjects" + file;
    Engine::ECSManager::Instance()->create_component<Engine::MeshComponent>(entityID, COMPONENT_MESH, mesh);

    Engine::MaterialComponent material;
    //get default engine assets path
    std::string vshPath = path + "Shader/DefaultShader.vsh";
    std::string fshPath = path + "Shader/DefaultShader.fsh";
    std::string gshPth = "";
    std::string texturePath = path + "Texture/white.png";
    material.material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);
    Engine::ECSManager::Instance()->create_component<Engine::MaterialComponent>(entityID, COMPONENT_MATERIAL, material);

    //temp 
    //Engine::ECSManager::Instance()->create_component<Engine::CollisionSphereComponent>(entityID, COMPONENT_COLLISION_SPHERE);
    //Engine::CollisionSystem::Instance()->init_collision_component(entityID, COMPONENT_COLLISION_SPHERE);

    //Engine::ECSManager::Instance()->create_component<Engine::HealthComponent>(entityID, COMPONENT_HEALTH);
    //Engine::ECSManager::Instance()->create_component<Engine::WeaponComponent>(entityID, COMPONENT_WEAPON);
}

void MenuBarView::createMaterial(unsigned int entityID)
{
if (Engine::ECSManager::Instance()->has_component(entityID, COMPONENT_MATERIAL)) { return; }
    Engine::MaterialComponent material;
    auto path = FileModule::Instance()->get_internal_assets_path();
    std::string vshPath = path + "Shader/DefaultShader.vsh";
    std::string fshPath = path + "Shader/DefaultShader.fsh";
    std::string gshPth = "";
    std::string texturePath = path + "Texture/white.png";
    material.material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);
    Engine::ECSManager::Instance()->create_component<Engine::MaterialComponent>
                                        (entityID, COMPONENT_MATERIAL, material);
}

void MenuBarView::createMesh(unsigned int entityID)
{
    if (Engine::ECSManager::Instance()->has_component(entityID, COMPONENT_MESH)) { return; }
    Engine::MeshComponent mesh;
    auto path = FileModule::Instance()->get_internal_assets_path();
    mesh.path = path + "DefaultObjects/triangle_groundplane.obj";
    Engine::ECSManager::Instance()->create_component<Engine::MeshComponent>(entityID, COMPONENT_MESH, mesh);
}