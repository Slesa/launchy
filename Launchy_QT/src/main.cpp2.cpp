// main.cpp
#include <qapplication.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>

class myApp : public QWidget
{
	Q_OBJECT

private:
	QWidget* window;
	QPushButton* close;

public:
	myApp(QWidget * parent = 0) {
		window = new QWidget(0,Qt::FramelessWindowHint);
		close = new QPushButton("Close", &window);
		window.show();
	 }

public slots:

};

int
main( int argc, char* argv[] )
  {
    QApplication app(argc, argv);

	myApp* mya = new myApp();

    return app.exec();
  }