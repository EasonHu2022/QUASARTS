#include "QuasartsEngine.h"
#include "EditorRuntime.h"
class QEditor : public Engine::Application
{
public:
	QEditor()
	{
		//push a editor runtime into  application
		auto runtime = new EditorRuntime();
		push_runtime(runtime);
	
	};
	~QEditor() {};

private:

};

Engine::Application* Engine::create_application()
{
	return new QEditor();
}