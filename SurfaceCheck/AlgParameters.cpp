#include "AlgParameters.h"

AlgParameters::AlgParameters()
{
	
}
AlgParameters::~AlgParameters()
{

}

AlgParameters* AlgParameters::instance()
{
	static AlgParameters param;
	return &param;
}