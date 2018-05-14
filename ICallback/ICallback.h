#pragma once
#include<string>
#include "ICallBack_global.h"
class ICALLBACK_EXPORT ICallBack
{
public:
	ICallBack();
	virtual ~ICallBack();
	virtual void call(std::string msg) = 0;
};

