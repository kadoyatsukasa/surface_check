#include "SurfaceCheck.h"

SurfaceCheck::SurfaceCheck(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::SurfaceCheckClass)
{
	ui->setupUi(this);

	m_TemplateAlogrithm = new AlogTemplate();
	m_CheckAlogrithm = new AlgCheckModel();
	m_params = AlgParameters::instance();

	connect(ui->BtnCheck, SIGNAL(clicked()), this, SLOT(resultDetection()));
	connect(ui->btnSelectModel, SIGNAL(clicked()),ui->ModelImage,SLOT(OnSelectImage()));
	connect(ui->btnSelectTarget, SIGNAL(clicked()), ui->TargetImage, SLOT(OnSelectImage()));
}


void SurfaceCheck::resultDetection() 
{


	QString Model = ui->ModelImage->getImagePathName();
	QString Target = ui->TargetImage->getImagePathName();

	m_TemplateAlogrithm->templateCreation(Model, Target);

	m_CheckAlogrithm->getGrayValueRange(
		m_params->ho_Image,
		m_params->ho_RegionROI,
		&m_params->ho_RegionForeground,
		&m_params->ho_RegionBackground,
		&m_params->hv_BackgroundGVModel,
		&m_params->hv_ForegroundGVModel
	);

	m_CheckAlogrithm->templateDetection(
		m_params->ho_rImage,
		m_params->ho_RegionROI,
		m_params->hv_ModelID,
		m_params->hv_VariationID,
		m_params->hv_RowModel,
		m_params->hv_ColumnModel,
		m_params->hv_BackgroundGVModel,
		m_params->hv_ForegroundGVModel
	);

	
	m_ImageDisplayer = ui->CheckResult;
	m_ImageDisplayer->showImage(Target.toStdString().c_str());
	
	std::vector<AlgCheckModel::Ellipse_ROI> roi = m_CheckAlogrithm->Ellipses;
	std::vector<AlgCheckModel::Ellipse_ROI>::iterator it;

	for (it = roi.begin(); it != roi.end(); it++)
	{
		m_ImageDisplayer->displayEllipse(it->x,it->y,it->phi,it->LongRadius,it->ShortRadius,"red");
	}

	//roi.clear();
	m_TemplateAlogrithm->templateCleaning(m_params->hv_ModelID,m_params->hv_VariationID);
}