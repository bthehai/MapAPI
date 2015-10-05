#include "stdafx.h"
#include "AreaStyle.h"


AreaStyle::AreaStyle( int color )
	: RenderStyle()
{

}

RenderStyle* AreaStyle::current()
{
	throw std::exception("The method or operation is not implemented.");
}
