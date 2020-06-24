#pragma once

#include <vector>
#include<HalconCpp.h>
using namespace HalconCpp;

class AlgParameters
{
public:

	static AlgParameters* instance();

public:

	HObject ho_Image;
	HObject ho_rImage;
	HObject ho_RegionROI;
	HObject ho_RegionForeground;
	HObject ho_RegionBackground;
	
	HTuple hv_ModelID;
	HTuple hv_VariationID;
	HTuple hv_RowModel;
	HTuple hv_ColumnModel;
	HTuple hv_BackgroundGVModel;
	HTuple hv_ForegroundGVModel;

private:
	explicit AlgParameters();
	~AlgParameters();

};