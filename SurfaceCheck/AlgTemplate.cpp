#include "AlgTemplate.h"

AlogTemplate::AlogTemplate():
	m_params(nullptr)
{
	m_params = AlgParameters::instance();
}

AlogTemplate::~AlogTemplate()
{
}


void AlogTemplate::templateCreation(QString model,QString target)
{
	// Local iconic variables
	HObject  ho_ModelImage, ho_Rectangle, ho_ImageReduced;
	HObject  ho_RegionROI, ho_BrightRegion, ho_RegionDilation;
	HObject  ho_ConnectedRegions, ho_SelectedRegions, ho_LabelRegion;
	HObject  ho_Region, ho_LabelRectangle, ho_RegionIntersection;
	HObject  ho_ModelContours, ho_ModelContoursAffinTrans, ho_VarImage;

	HObject  ho_RegionForeground, ho_RegionBackground, ho_rImage;
	HObject  ho_reduceRImage;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_WindowHandle;
	HTuple  hv_UsedThreshold, hv_Row, hv_Column, hv_Radius;
	HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2, hv_Area;
	HTuple  hv_RowModel, hv_ColumnModel, hv_ModelID, hv_HomMat2DIdentity;
	HTuple  hv_HomMat2DTranslate, hv_VariationID, hv_BackgroundGVModel;
	HTuple  hv_ForegroundGVModel;

	HTuple	hv_model, hv_target;
	HTuple  hv_result, hv_result_path, hv_result_name;


	hv_model = HTuple(model.toStdString().c_str());
	hv_target = HTuple(target.toStdString().c_str());
	
	//
	//Read model image and init visualization
	//HalconCpp::HObject himg;
	ReadImage(&m_params->ho_Image, hv_model);
	GenRectangle1(&ho_Rectangle, 30, 300, 930, 930);
	ReduceDomain(m_params->ho_Image, ho_Rectangle, &ho_ImageReduced);
	GetImageSize(ho_ImageReduced, &hv_Width, &hv_Height);

	//
	//Segment model region

	//--------------------
	//get_model_region (ModelImage, RegionROI, ImageReduced)
	//
	//Segment label on the cap
	BinaryThreshold(ho_ImageReduced, &ho_BrightRegion, "max_separability", "dark",&hv_UsedThreshold);
	DilationCircle(ho_BrightRegion, &ho_RegionDilation, 1.5);
	Connection(ho_RegionDilation, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 1000, 9999999);
	Union1(ho_SelectedRegions, &ho_LabelRegion);
	SmallestCircle(ho_LabelRegion, &hv_Row, &hv_Column, &hv_Radius);
	SmallestRectangle1(ho_LabelRegion, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
	//
	//Define model region
	GenCircle(&ho_Region, hv_Row, hv_Column, hv_Radius);
	GenRectangle1(&ho_LabelRectangle, hv_Row1, hv_Column1, hv_Row2, hv_Column2);
	Intersection(ho_Region, ho_LabelRectangle, &ho_RegionIntersection);
	DilationCircle(ho_RegionIntersection, &m_params->ho_RegionROI, 5.5);
	ReduceDomain(m_params->ho_Image, m_params->ho_RegionROI, &ho_ImageReduced);

	//--------------------
	AreaCenter(m_params->ho_RegionROI, &hv_Area, &m_params->hv_RowModel, &m_params->hv_ColumnModel);

	CreateScaledShapeModel(ho_ImageReduced, "auto", 0, HTuple(360).TupleRad(), "auto",
		0.8, 1.2, "auto", "auto", "use_polarity", "auto", "auto", &m_params->hv_ModelID);
	GetShapeModelContours(&ho_ModelContours, m_params->hv_ModelID, 1);
	//Transform model contours from the original position for visualization
	HomMat2dIdentity(&hv_HomMat2DIdentity);
	HomMat2dTranslate(hv_HomMat2DIdentity, m_params->hv_RowModel,m_params->hv_ColumnModel, &hv_HomMat2DTranslate);
	AffineTransContourXld(ho_ModelContours, &ho_ModelContoursAffinTrans, hv_HomMat2DTranslate);
	//
	//Create variation model for print inspection
	CreateVariationModel(hv_Width, hv_Height, "byte", "direct", &m_params->hv_VariationID);
	//Generate variation image
	SobelAmp(m_params->ho_Image, &ho_VarImage, "sum_abs", 5);
	PrepareDirectVariationModel(m_params->ho_Image, ho_VarImage, m_params->hv_VariationID, (HTuple(20).Append(25)),
		(HTuple(1.6).Append(1.6)));

	ReadImage(&m_params->ho_rImage, hv_target);
	ReduceDomain(m_params->ho_rImage, ho_Rectangle, &m_params->ho_rImage);

}

void AlogTemplate::templateCleaning(HTuple hv_ModelID,HTuple hv_VariationID)
{
	ClearShapeModel(hv_ModelID);
	ClearVariationModel(hv_VariationID);
	
}