#ifndef HIMAGEWIDGET_H
#define HIMAGEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QWheelEvent>
#include <QFileDialog>
#include <QDebug>
#include <QMenu>

#include <HalconCpp.h>
#include <Halcon.h>

class HImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HImageWidget(QWidget *parent = 0);
    void showImage(HalconCpp::HObject& imageObj);
    void showImage(const char* imagePath);
	void showText(const char* text, const char* colorStr = nullptr);
	void displayLine(int beginX, int beginY, int endX, int endY, const char* color = "green");
	void displayRect(int beginX, int beginY, int endX, int endY);

	void displayCicle(float centerX, float centerY, float radius, const char* color = "green");
	void displayCrossPoint(float centerX, float centerY, const char* color);
	void displayEllipse(float centerX,float centerY,float phi,float radiusA,float radiusB,const char* color="green");
	void displayZoomImage(double mode,double mouseRow,double mouseCol);

	void moveImage(double motionX,double motionY,int* beginX,int* beginY,int* endX,int* endY);

	HalconCpp::HTuple* getWindowHand();

	inline QString getImageName() { return ImageName; }
	inline QString getImagePath() { return ImagePath; }
	inline QString getImagePathName() { return ImagePathName; }
	inline QString getImageNickName() { return ImageNickName; }

	inline void setImageName(QString name) { ImageName = name; }
	inline void setImagePath(QString path) { ImagePath = path; }
	inline void setImagePathName(QString pathName) { ImagePathName = pathName; }
	inline void setImageNickName(QString nickname) { ImageNickName = nickname; }

protected:
	void resizeEvent(QResizeEvent* event);					
	void wheelEvent(QWheelEvent *event)override;		
	void mousePressEvent(QMouseEvent *ev)override;		
	void mouseMoveEvent(QMouseEvent *ev)override;		
	void mouseReleaseEvent(QMouseEvent *ev)override;
	
signals:

private slots :
	void OnSelectImage();
	void OnZoomInImage();
	void OnZoomOutImage();
	void OnPressImage();

private:
    HalconCpp::HTuple m_WindowHandle;
    HalconCpp::HObject hImageM;
	HalconCpp::HObject m_imageObj, m_imageZoom;
	HalconCpp::HTuple w, h;
	HalconCpp::HTuple m_row, m_col,m_btn;
	HalconCpp::HTuple m_mouseX, m_mouseY, m_mousePressed;

	double m_oldX = 0;
	double m_oldY = 0;
	double m_newX = 0; 
	double m_newY = 0;
	double m_motionX = 0; 
	double m_motionY = 0;
	int m_moveBeginX = 0;
	int m_moveBeginY = 0;
	int m_moveEndX = 0;
	int m_moveEndY = 0;

	qreal ZoomValue = 1.0;
	int XPInterval = 0;
	int YPInterVal = 0;
	QPoint old;
	bool m_pressed=false;

	QString ImageName;
	QString ImagePath;
	QString ImagePathName;
	QString ImageNickName;
};

#endif // HIMAGEWIDGET_H
