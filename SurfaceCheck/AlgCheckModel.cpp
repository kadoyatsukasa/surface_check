#include "AlgCheckModel.h"
#include <QDebug>

AlgCheckModel::AlgCheckModel()
{
	m_params = AlgParameters::instance();
}

AlgCheckModel::~AlgCheckModel()
{
}
/**
 * Detection
 * 
 * Compare difference between template and sample
 *
 */
bool AlgCheckModel::templateDetection(
	HObject ho_rImage,
	HObject ho_RegionROI,
	HTuple hv_ModelID,
	HTuple hv_VariationID,
	HTuple hv_RowModel,
	HTuple hv_ColumnModel,
	HTuple hv_BackgroundGVModel, 
	HTuple hv_ForegroundGVModel
)
{
	// Local iconic variables
	HObject  ho_ModelContours, ho_ImageAffinTrans;
	HObject  ho_RegionForegroundImage, ho_RegionBackgroundImage;
	HObject  ho_ModelContoursTrans, ho_ImageReduced, ho_ImageScaled;
	HObject  ho_RegionDiff, ho_RegionDiffSelected, ho_RegionOpening;
	HObject  ho_ConnectedRegions, ho_DefectRegions, ho_DefectRegionsTrans;
	HObject  ho_DefectRegionsClosing, ho_RegionUnion, ho_RegionClosing;
	HObject  ho_DefectRegionEnlarged, ho_Ellipse;

	// Local control variables
	HTuple  hv_StartSeconds, hv_Row, hv_Column, hv_Angle;
	HTuple  hv_Scale, hv_Score, hv_EndSeconds, hv_TimeObjSearch;
	HTuple  hv_HomMat2DImage, hv_BackgroundImage, hv_ForegroundImage;
	HTuple  hv_Mult, hv_Add, hv_HomMat2DContour, hv_NumberRegionDiff;
	HTuple  hv_i, hv_HomMat2DInvert, hv_Ra, hv_Rb, hv_Phi;
	HTuple  hv_TimePrintInspect;

	HTuple  hv_Area;
	HTuple  hv_RowEllipse, hv_ColumnEllipse;

	Ellipses.clear();

	FindScaledShapeModel(ho_rImage, hv_ModelID, 0, HTuple(360).TupleRad(), 0.98, 1.02,
		0.4, 1, 1, "least_squares_high", (HTuple(5).Append(3)), 0.9, &hv_Row, &hv_Column,
		&hv_Angle, &hv_Scale, &hv_Score);
	//
	GetShapeModelContours(&ho_ModelContours, hv_ModelID, 1);
	//
	try
	{
			if (0 != ((hv_Row.TupleLength()) > 0))
			{
				//
				//Align the model region and the found label region
				VectorAngleToRigid(HTuple(hv_Row[0]), HTuple(hv_Column[0]), hv_Angle, HTuple(hv_RowModel[0]),
					HTuple(hv_ColumnModel[0]), 0, &hv_HomMat2DImage);
				AffineTransImage(ho_rImage, &ho_ImageAffinTrans, hv_HomMat2DImage, "constant",
					"false");
				getGrayValueRange(ho_ImageAffinTrans, ho_RegionROI, &ho_RegionForegroundImage,
					&ho_RegionBackgroundImage, &hv_BackgroundImage, &hv_ForegroundImage);
				//
				//Scale image to the model's gray value range
				hv_Mult = (hv_ForegroundGVModel - hv_BackgroundGVModel) / (hv_ForegroundImage - hv_BackgroundImage);
				hv_Add = hv_ForegroundGVModel - (hv_Mult*hv_ForegroundImage);
				//Transform the contours of the models for the visualization
				VectorAngleToRigid(0, 0, 0, HTuple(hv_Row[0]), HTuple(hv_Column[0]), hv_Angle,
					&hv_HomMat2DContour);
				AffineTransContourXld(ho_ModelContours, &ho_ModelContoursTrans, hv_HomMat2DContour);
				ReduceDomain(ho_ImageAffinTrans, ho_RegionROI, &ho_ImageReduced);
				ScaleImage(ho_ImageReduced, &ho_ImageScaled, hv_Mult, hv_Add);
				//
				//Print Inspection);
	
				CompareExtVariationModel(ho_ImageScaled, &ho_RegionDiff, hv_VariationID, "light_dark");
				//Analyze bright and dark defect regions
				CountObj(ho_RegionDiff, &hv_NumberRegionDiff);
				{
					HTuple end_val32 = hv_NumberRegionDiff;
					HTuple step_val32 = 1;
					for (hv_i = 1; hv_i.Continue(end_val32, step_val32); hv_i += step_val32)
					{
						SelectObj(ho_RegionDiff, &ho_RegionDiffSelected, hv_i);
						OpeningCircle(ho_RegionDiffSelected, &ho_RegionOpening, 2.5);
						Connection(ho_RegionOpening, &ho_ConnectedRegions);
						SelectShape(ho_ConnectedRegions, &ho_DefectRegions, "height", "and", 10, 99999);
						//
						//Transform regions to the original position in the input image
						HomMat2dInvert(hv_HomMat2DImage, &hv_HomMat2DInvert);
						AffineTransRegion(ho_DefectRegions, &ho_DefectRegionsTrans, hv_HomMat2DInvert,"nearest_neighbor");
						ClosingCircle(ho_DefectRegionsTrans, &ho_DefectRegionsClosing, 1.5);
	
						Union1(ho_DefectRegionsClosing, &ho_RegionUnion);
						ClosingCircle(ho_RegionUnion, &ho_RegionClosing, 10);
						Connection(ho_RegionClosing, &ho_DefectRegionEnlarged);
						EllipticAxis(ho_DefectRegionEnlarged, &hv_Ra, &hv_Rb, &hv_Phi);
						
						if (0 != (hv_Phi.TupleLength()))
						{
							AreaCenter(ho_DefectRegionEnlarged, &hv_Area, &hv_RowEllipse, &hv_ColumnEllipse);
	
	
							roi.x = hv_RowEllipse.D();
							roi.y = hv_ColumnEllipse.D();
							roi.radius = sqrt((hv_Area/3.14).D());
							roi.phi = hv_Phi.D();
							roi.LongRadius = (hv_Ra * 2).D();
							roi.ShortRadius = (hv_Rb * 2).D();

							Ellipses.push_back(roi);
							
						}
					}
				}

				return true;
			}
			else
			{
				return false;
			}
	}
	catch (HException &e)
	{
        qDebug() << e.ErrorMessage().Text();
	}
}

void AlgCheckModel::getGrayValueRange(
	HObject ho_Image,
	HObject ho_RegionROI,
	HObject *ho_RegionForeground,
	HObject *ho_RegionBackground,
	HTuple *hv_BackgroundGVal,
	HTuple *hv_ForegroundGVal
)
{

	// Local iconic variables
	HObject  ho_ImageReduced;

	// Local control variables
	HTuple  hv_UsedThreshold, hv_DeviationFG, hv_DeviationBG;

	ReduceDomain(ho_Image, ho_RegionROI, &ho_ImageReduced);
	BinaryThreshold(ho_ImageReduced, &(*ho_RegionBackground), "max_separability", "dark",
		&hv_UsedThreshold);
	Difference(ho_RegionROI, (*ho_RegionBackground), &(*ho_RegionForeground));
	Intensity((*ho_RegionForeground), ho_Image, &(*hv_ForegroundGVal), &hv_DeviationFG);
	Intensity((*ho_RegionBackground), ho_Image, &(*hv_BackgroundGVal), &hv_DeviationBG);
}
