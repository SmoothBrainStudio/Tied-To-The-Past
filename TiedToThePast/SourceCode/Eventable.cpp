#include "Definitions.hpp"
#include "Eventable.hpp"

Eventable::~Eventable()
{
}

void Eventable::CallOnClick(bool _isClick)
{
	if (_isClick)
	{
		clickCallBack();
	}
}

void Eventable::CallOnReleaseClick(bool _isRelease)
{
	if (_isRelease)
	{
		clickReleaseCallBack();
	}
}

void Eventable::CallWhileClick(bool _isClick)
{
	if (_isClick)
	{
		whileClickCallBack();
	}
}

void Eventable::CallOnOver(bool _isHover)
{
	if (_isHover)
	{
		overCallBack();
	}
}

void Eventable::CallOnOut(bool _isOut)
{
	if (_isOut)
	{
		outCallBack();
	}
}

void Eventable::CallOnChange(bool _isChange)
{
	if (_isChange)
	{
		changeCallBack();
	}
}
