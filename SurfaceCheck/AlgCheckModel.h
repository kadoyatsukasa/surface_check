#pragma once

#include<HalconCpp.h>
#include<vector>
#include "AlgParameters.h"

using namespace HalconCpp;

class AlgCheckModel
{

public:
	AlgCheckModel();
	~AlgCheckModel();

	bool templateDetection(
		HObject ho_rImage,
		HObject ho_RegionROI,
		HTuple hv_ModelID,
		HTuple hv_VariationID,
		HTuple hv_RowModel,
		HTuple hv_ColumnModel,
		HTuple hv_BackgroundGVModel, 
		HTuple hv_ForegroundGVModel
	);

	void getGrayValueRange(
		HObject ho_Image, 
		HObject ho_RegionROI,
		HObject *ho_RegionForeground,
		HObject *ho_RegionBackground,
		HTuple *hv_BackgroundGVal,
		HTuple *hv_ForegroundGVal
	);

public:
	struct Ellipse_ROI
	{
		double x;
		double y;
		double radius;
		double phi;
		double LongRadius;
		double ShortRadius;
	};

	std::vector<Ellipse_ROI> Ellipses;
	Ellipse_ROI roi;

private:
	AlgParameters* m_params;
};

