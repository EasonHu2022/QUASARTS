#include "QuasartsEngine.h"


/*
	Entry point
*/
#include "Core/EntryPoint.h"
#include <memory>
#include <map>
#include <unordered_map>


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

private:
	std::unordered_map<size_t, std::shared_ptr<GuiView>> guiViews;

};

Engine::Application* Engine::create_application()
{
	return new QEditor();
}

template<class T>
inline void QEditor::add_gui_view()
{
	guiViews.emplace(typeid(T).hash_code(), std::make_shared<T>());
}
