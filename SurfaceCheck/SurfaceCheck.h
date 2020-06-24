#pragma once

#include <QtWidgets/QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QtWidgets>
#include "HImageWidget.h"
#include "AlgTemplate.h"
#include "AlgCheckModel.h"
#include "AlgParameters.h"

#include "ui_SurfaceCheck.h"


class HImageWidget;

class SurfaceCheck : public QMainWindow
{
	Q_OBJECT

public:
	explicit SurfaceCheck(QWidget *parent = Q_NULLPTR);
	
public slots:
	void resultDetection();
	
private:
	Ui::SurfaceCheckClass *ui;
	
	HImageWidget *m_ImageDisplayer;
	AlogTemplate *m_TemplateAlogrithm;
	AlgCheckModel *m_CheckAlogrithm;
	AlgParameters *m_params;
};
