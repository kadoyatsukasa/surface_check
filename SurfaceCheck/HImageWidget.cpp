#include "HImageWidget.h"

HImageWidget::HImageWidget(QWidget *parent)
    : QWidget(parent)
{
	HalconCpp::OpenWindow(0, 0, (Hlong)this->width(), (Hlong)this->height(), (Hlong)this->winId(), "visible", "", &m_WindowHandle);
	HalconCpp::HDevWindowStack::Push(m_WindowHandle);
	HalconCpp::HDevWindowStack::SetActive(m_WindowHandle);
}

void HImageWidget::resizeEvent(QResizeEvent* event)
{
	int w = this->width();
	int h = this->height();
	if (w > 0 && h > 0)
	{
		HalconCpp::CloseWindow(m_WindowHandle);
		HalconCpp::OpenWindow(0, 0, (Hlong)w, (Hlong)h, (Hlong)this->winId(), "visible", "", &m_WindowHandle);
		HalconCpp::HDevWindowStack::Push(m_WindowHandle);
		HalconCpp::HDevWindowStack::SetActive(m_WindowHandle);
	}

    return QWidget::resizeEvent(event);
}

void HImageWidget::showImage(HalconCpp::HObject &imageObj)
{
    try
    {
        /*HalconCpp::HTuple w, h;*/
        HalconCpp::GetImagePointer1(imageObj, NULL, NULL, &w, &h);
        HalconCpp::SetPart(m_WindowHandle, NULL, NULL, h, w);
        HalconCpp::DispImage(imageObj, m_WindowHandle);
    }
    catch (HalconCpp::HException &except)
    {
		ClearWindow(m_WindowHandle);

        HalconCpp::SetTposition(m_WindowHandle, 0, 2);
        //HalconCpp::WriteString(windowHandM, except.ErrorMessage());
		showText("Error....", "red");
    }
}

void HImageWidget::showImage(const char* imagePath)
{
    if (!imagePath)
        return;
	try {
		HalconCpp::ReadImage(&m_imageObj, imagePath);
		showImage(m_imageObj);
	}
	catch(Herror error){
		throw(error);
	}
}

void HImageWidget::showText(const char* text, const char* colorStr)
{
	if (!text)
		return;

	if (!colorStr)
		colorStr = "green";

	HalconCpp::SetColor(m_WindowHandle, colorStr);
	HalconCpp::SetTposition(m_WindowHandle, 10, 10);
	HalconCpp::WriteString(m_WindowHandle, text);
}

void HImageWidget::displayLine(int beginX, int beginY, int endX, int endY, const char* color)
{
	HalconCpp::SetColor(m_WindowHandle, color);
	HalconCpp::SetLineWidth(m_WindowHandle, 2);
	HalconCpp::DispLine(m_WindowHandle, beginY, beginX, endY, endX);
}

void HImageWidget::displayRect(int beginX, int beginY, int endX, int endY)
{
// 	HalconCpp::SetColor(windowHandM, "green");
// 	HalconCpp::SetLineWidth(windowHandM, 2);
// 	HalconCpp::DispRectangle1(windowHandM, beginY, beginX, endY, endX);
	displayLine(beginX, beginY, endX, beginY);
	displayLine(endX, beginY, endX, endY);
	displayLine(endX, endY, beginX, endY);
	displayLine(beginX, endY, beginX, beginY);
}

HalconCpp::HTuple* HImageWidget::getWindowHand()
{
	return &m_WindowHandle;
}

void HImageWidget::displayCicle(float centerX, float centerY, float radius, const char* color)
{
	HalconCpp::SetColor(m_WindowHandle, color);
	HalconCpp::SetLineWidth(m_WindowHandle, 2);
	HalconCpp::SetDraw(m_WindowHandle, "margin");
	HalconCpp::DispCircle(m_WindowHandle, centerY, centerX, radius);
}

void HImageWidget::displayCrossPoint(float centerX, float centerY, const char* color)
{
	HalconCpp::SetColor(m_WindowHandle, color);
	HalconCpp::SetLineWidth(m_WindowHandle, 1);

	float beginY = centerY - 80;
	float endY = centerY + 80;
	HalconCpp::DispLine(m_WindowHandle, beginY, centerX, endY, centerX);

	float beginX = centerX - 80;
	float endX = centerX + 80;
	HalconCpp::DispLine(m_WindowHandle, centerY, beginX, centerY, endX);
}


void HImageWidget::displayEllipse(float centerX, float centerY,float phi ,float radiusA, float radiusB, const char* color)
{
	HalconCpp::SetColor(m_WindowHandle, color);
	HalconCpp::SetLineWidth(m_WindowHandle, 2);
	HalconCpp::SetDraw(m_WindowHandle, "margin");
	HalconCpp::DispEllipse(m_WindowHandle, centerX, centerY, phi, radiusA, radiusB);
}

//////////////////////////////////////////////////////////////////////////
///******Renderer************////
//////////////////////////////////////////////////////////////////////////


void HImageWidget::wheelEvent(QWheelEvent *event)
{
	HalconCpp::HTuple mouseRow, mouseCol, mouseBtn;

	
	try 
	{
		GetMpositionSubPix(m_WindowHandle,&mouseRow,&mouseCol,&mouseBtn);

	}
	catch(HalconCpp::HException e)
	{
		qDebug() << "Exception is "<<e.ErrorMessage()<<'\n';
		return;
	}
	
	double mode = 1;

	if (event->delta() > 0)
		mode = 1;
	else
		mode = -1;

	displayZoomImage(mode, mouseRow.D(), mouseCol.D());
	
	
}

void HImageWidget::mousePressEvent(QMouseEvent *event)
{
	m_pressed = true;
	HalconCpp::HTuple mouseX, mouseY, mousePressed;
	try 
	{
		GetMpositionSubPix(m_WindowHandle, &mouseX, &mouseY, &mousePressed);
		m_oldX = mouseX.D();
		m_oldY = mouseY.D();
		update();
	}
	catch (HalconCpp::HException exception)
	{
		qDebug() << "Exception is " << exception.ErrorMessage();
		return;
	}
}

void HImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
	m_pressed = false;

	
	update();

}

void HImageWidget::mouseMoveEvent(QMouseEvent *event)
{
	HalconCpp::HTuple mouseX, mouseY, mousePressed;
	HalconCpp::HTuple width, height;
	HalconCpp::GetMpositionSubPix(m_WindowHandle,&mouseX,&mouseY,&mousePressed);

	m_newX = mouseX.D();
	m_newY = mouseY.D();
	m_motionX = m_newX - m_oldX;
	m_motionY = m_newY - m_oldY;

	moveImage(m_motionX, m_motionY,&m_moveBeginX,&m_moveBeginY,&m_moveEndX,&m_moveEndY);

	

	HalconCpp::SetPart(m_WindowHandle, m_moveBeginX, m_moveBeginY, m_moveEndX, m_moveEndY);
	HalconCpp::FlushBuffer(m_WindowHandle);
	HalconCpp::ClearWindow(m_WindowHandle);
	HalconCpp::DispImage(m_imageObj, m_WindowHandle);

	update();
}

void HImageWidget::OnSelectImage()
{
	ImageName = QFileDialog::getOpenFileName(this, "Select Image", "./", tr("Images (*.jpg *.bmp *.png *.tiff)"));
	QFile file(ImageName);
	QFileInfo fileinfo(ImageName);
	showImage(ImageName.toStdString().c_str());

	if (!file.exists())
		return;

	ImagePath = fileinfo.absolutePath();
	ImagePathName = fileinfo.absoluteFilePath();
	ImageNickName = fileinfo.fileName();

	ZoomValue = 1.0;
	XPInterval = 0;
	YPInterVal = 0;
	update();

}


void HImageWidget::OnZoomInImage()
{
	ZoomValue += 0.05;

	HalconCpp::ZoomImageFactor(m_imageObj,&m_imageZoom,ZoomValue,ZoomValue,"nearest_neighbor");
	
	update();
}

void HImageWidget::OnZoomOutImage()
{
	ZoomValue -= 0.05;
	
	if (ZoomValue <= 0)
	{
		ZoomValue = 0.05;	
		return;
	}
	HalconCpp::ZoomImageFactor(m_imageObj, &m_imageZoom, ZoomValue, ZoomValue, "nearest_neighbor");
	update();
}

void HImageWidget::OnPressImage()
{
	ZoomValue = 1.0;
	XPInterval = 0.0;
	YPInterVal = 0.0;
	update();
}



void HImageWidget::displayZoomImage(double mode, double mouseRow, double mouseCol)
{
	int zoomBeginRow, zoomEndRow, zoomBeginCol, zoomEndCol;
	double widthZoomValue = 1.0;
	double heightZoomValue = 1.0;
	HalconCpp::HTuple currentBeginRow, currentEndRow, currentBeginCol, currentEndCol;

	try 
	{
		GetPart(m_WindowHandle,&currentBeginRow, &currentBeginCol, &currentEndRow, &currentEndCol);
	}
	catch (Herror error) {
		return;
	}

	// zoom in
	if (mode > 0)
	{
		zoomBeginRow = static_cast<wint_t>(currentBeginRow.D() + (mouseRow - currentBeginRow.D())*0.03);
		zoomBeginCol = static_cast<wint_t>(currentBeginCol.D() + (mouseCol - currentBeginCol.D())*0.03);
		zoomEndRow = static_cast<wint_t>(currentEndRow.D() - (currentEndRow.D() - mouseRow)*0.03);
		zoomEndCol = static_cast<wint_t>(currentEndCol.D() - (currentEndCol.D() - mouseCol)*0.03);
		widthZoomValue += (static_cast<double>(zoomEndRow) - static_cast<double>(zoomBeginRow)) / (currentEndRow.D() - currentBeginRow.D())*0.01;
		heightZoomValue += (static_cast<double>(zoomEndCol) - static_cast<double>(zoomBeginCol)) / (currentEndCol.D() - currentBeginCol.D())*0.01;
		ZoomImageFactor(m_imageObj, &m_imageZoom, widthZoomValue, heightZoomValue, "nearest_neighbor");
		SetPart(m_WindowHandle,zoomBeginRow,zoomBeginCol,zoomEndRow,zoomEndCol);
	}
	//zoom out
	else
	{
		zoomBeginRow = static_cast<int>(mouseRow - (mouseRow - currentBeginRow.D()) / 0.7);
		zoomBeginCol = static_cast<int>(mouseCol - (mouseCol - currentBeginCol.D()) / 0.7);
		zoomEndRow = static_cast<int>(mouseRow - (currentEndRow.D() - mouseRow) / 0.7);
		zoomEndCol = static_cast<int>(mouseCol - (currentEndCol.D() - mouseCol) / 0.7);
		widthZoomValue -= (static_cast<double>(zoomEndRow) - static_cast<double>(zoomBeginRow))/(currentEndRow.D() - currentBeginRow.D())*0.01;
		heightZoomValue -= (static_cast<double>(zoomEndRow) - static_cast<double>(zoomBeginRow))/(currentEndRow.D() - currentBeginRow.D())*0.01;

		if (widthZoomValue <= 0 || heightZoomValue <= 0)
		{
			widthZoomValue = static_cast<double>((zoomEndRow - zoomBeginRow) / (currentEndRow.D() - currentBeginRow.D()))*0.01;
			heightZoomValue = static_cast<double>((zoomEndRow - zoomBeginRow) / (currentEndRow.D() - currentBeginRow.D()))*0.01;
			return;
		}

		ZoomImageFactor(m_imageObj, &m_imageZoom, widthZoomValue, heightZoomValue, "nearest_neighbor");
	}
	ClearWindow(m_WindowHandle);
	DispImage(m_imageZoom, m_WindowHandle);
}

/// <summary>
/// Get the movement of mouse pointer
/// </summary>
/// <param name="motionX">motion of mouse pointer on the X direction</param>
/// <param name="motionY">motion of mouse pointer on the Y direction</param>
/// <param name="beginX">starting X axis of region after mouse drag</param>
/// <param name="beginY">starting Y axis of region after mouse drag</param>
/// <param name="endX">ending X axis of region after mouse drag</param>
/// <param name="endY">ending Y axis of region after mouse drag</param>
void HImageWidget::moveImage(double motionX, double motionY, int* beginX, int* beginY, int* endX, int* endY)
{
	HalconCpp::HTuple currentBeginX, currentBeginY, currentEndX, currentEndY;
	HalconCpp::GetPart(m_WindowHandle,&currentBeginX, &currentBeginY, &currentEndX, &currentEndY);
	
	*beginX = static_cast<int>(currentBeginX.D()-motionX);
	*beginY = static_cast<int>(currentBeginY.D()-motionY);
	*endX = static_cast<int>(currentEndX.D()-motionX);
	*endY = static_cast<int>(currentEndY.D()-motionY);

	return;
}