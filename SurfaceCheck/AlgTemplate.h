#pragma once

#include <QObject>
#include <HalconCpp.h>
#include "AlgParameters.h"
using namespace HalconCpp;

class AlogTemplate
{

public:
	explicit AlogTemplate();
	~AlogTemplate();

	void templateCreation(QString model,QString target);
	void templateCleaning(HTuple hv_ModelID, HTuple hv_VariationID);

private:
	AlgParameters* m_params;

};

