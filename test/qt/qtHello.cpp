

//! [main program]
#include "QtWidgets/qapplication.h"
#include "QtWidgets/qlabel.h"
#include "QtWidgets/qlineedit.h"
#include "QtWidgets/qboxlayout.h" 
#include "QtWidgets/qtextedit.h"

#include "gtest/gtest.h"


TEST(QtHello, test_layout) {
//	int argc = 1;
//	char** argv = nullptr;
//	QApplication app(argc, argv);
//	QWidget window;
//	//! [create, lay out widgets and show]
//	QLabel* label = new QLabel(QApplication::translate("windowlayout", "Name:"));
//	QLineEdit* lineEdit = new QLineEdit();
//
//	QHBoxLayout* layout = new QHBoxLayout();
//	layout->addWidget(label);
//	layout->addWidget(lineEdit);
//	window.setLayout(layout);
//	//! [create, lay out widgets and show]
//	window.setWindowTitle(
//		QApplication::translate("windowlayout", "Window layout"));
//	window.show();
//	 app.exec();
}


TEST(QtHello, test_textEdit) {
	int argc = 1;
	char** argv = nullptr;
    QApplication app(argc, argv);

    QTextEdit textEdit;
    textEdit.show();
	app.exec();
}
