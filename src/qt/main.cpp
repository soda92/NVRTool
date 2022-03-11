#include <QWidget>
#include <QtWidgets>
#include <QMainWindow>
#include <QDebug>

class QPushButton;
class QTextBrowser;

// This is the declaration of our MainWidget class
// The definition/implementation is in mainwidget.cpp
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow *parent = 0) : QMainWindow(parent)
    {
        button_ = new QPushButton(tr("Push Me!"));
        textBrowser_ = new QTextBrowser();

        QGridLayout *mainLayout = new QGridLayout;
        mainLayout->addWidget(button_, 0, 0);
        mainLayout->addWidget(textBrowser_, 1, 0);
        QWidget *widget = new QWidget();
        widget->setLayout(mainLayout);
        setCentralWidget(widget);
        setWindowTitle(tr("Connecting buttons to processes.."));

        connect(button_, &QPushButton::clicked, this, &MainWindow::clicked);
    }; // Constructor
    ~MainWindow()
    {
        delete button_;
        delete textBrowser_;
    }; // Destructor

public slots:
    void clicked()
    {
        qDebug() << "clicked";
    }

private:
    QPushButton *button_;
    QTextBrowser *textBrowser_;
};
#include "main.moc"
#include <QtWidgets>

int main(int argc, char *argv[])
{
    // Creates an instance of QApplication
    QApplication a(argc, argv);

    // This is our MainWidget class containing our GUI and functionality
    MainWindow w;
    w.show(); // Show main window

    // run the application and return execs() return value/code
    return a.exec();
}
