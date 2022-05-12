#include "QuasartsEngine.h"

/*
	Entry point
*/
#include "Core/EntryPoint.h"
#include <memory>
#include <map>
#include <unordered_map>
#include "Controller/SceneCameraController.h"


class GuiView;
class QEditor : public Engine::Application
{
public:
	QEditor();
	~QEditor();

	void init() override;
	void on_update() override;
	void on_gui() override;

	template <class T>
	void add_gui_view();
	template <class T>
	inline auto getGuiView()
	{
		return std::static_pointer_cast<T>(guiViews[typeid(T).hash_code()]);
	}

	/// <summary>
	/// test code in init()
	/// </summary>
	//void test_in_init();

	void test_in_update();

private:
	std::unordered_map<size_t, std::shared_ptr<GuiView>> guiViews;

	void poll_input();
	SceneCameraController* cameraController;
};
template<class T>
inline void QEditor::add_gui_view()
{
	guiViews.emplace(typeid(T).hash_code(), std::make_shared<T>());
	guiViews[typeid(T).hash_code()]->on_add();
}


Engine::Application* Engine::create_application()
{
	return new QEditor();
}

