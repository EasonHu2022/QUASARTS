#include "QuasartsEngine.h"
class SceneCameraController
{
private:
	//store the entity id of the scene camera
	unsigned int camera;
	float MouseSensitivity = 0.1f;



public:
	SceneCameraController() {};
	~SceneCameraController() {};

	void on_update();
	void on_init();
	void on_stop();
	char   buffer[MAX_PATH];

};