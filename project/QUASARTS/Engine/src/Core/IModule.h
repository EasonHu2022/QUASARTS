#pragma once
class IModule
{

public:

	//a flag if need to be pushed an update
	bool NeedUpdate = true;

public:
	/// <summary>
	/// initialize the module
	/// </summary>
	virtual void init() = 0;

	/// <summary>
	/// start the module
	/// </summary>
	/// <returns></returns>
	virtual int start() = 0;

	/// <summary>
	/// push update
	/// </summary>
	virtual void update() = 0;

	/// <summary>
	/// stop the module
	/// </summary>
	/// <returns></returns>
	virtual int stop() = 0;

	/// <summary>
	/// release the module
	/// </summary>
	virtual void release() = 0;

};