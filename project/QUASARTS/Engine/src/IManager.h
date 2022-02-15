#pragma once
class ManagerCtrl
{
public:
	/// <summary>
	/// initialize the manager controller
	/// </summary>
	void init();

	/// <summary>
	/// start the managers
	/// </summary>
	/// <returns></returns>
	int start();

	/// <summary>
	/// push update to each manager
	/// </summary>
	void update();

	/// <summary>
	/// stop the managers
	/// </summary>
	/// <returns></returns>
	int stop();

	/// <summary>
	/// release the manager controller
	/// </summary>
	void release();
};