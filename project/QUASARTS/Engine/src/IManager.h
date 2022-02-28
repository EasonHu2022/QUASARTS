#pragma once
class IManager
{
public:
	/// <summary>
	/// initialize the manager 
	/// </summary>
	virtual void init() = 0;

	/// <summary>
	/// start the managers
	/// </summary>
	/// <returns></returns>
	virtual int start() = 0;

	/// <summary>
	/// push update to each manager
	/// </summary>
	virtual void update() = 0;

	/// <summary>
	/// stop the managers
	/// </summary>
	/// <returns></returns>
	virtual int stop() = 0;

	/// <summary>
	/// release the manager controller
	/// </summary>
	virtual void release() = 0;
};