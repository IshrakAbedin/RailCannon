#pragma once

#include <functional>

class Possessable
{
public:
	std::function<void()> OnPossessCallback;
	std::function<void()> OnDepossessCallback;

	bool IsPossessed;
	bool IsControllerOn;

	Possessable() : OnPossessCallback(nullptr), OnDepossessCallback(nullptr), IsPossessed(false), IsControllerOn(false) { }

	virtual void OnPossess() = 0;
	virtual void OnDepossess() = 0;
};