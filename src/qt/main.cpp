#include <QWidget>
#include <QtWidgets>
#include <QMainWindow>
#include <QDebug>

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
    };
    ~MainWindow()
    {
        delete button_;
        delete textBrowser_;
    };

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
    QApplication a(argc, argv);
    MainWindow w;
    w.show(); // Show main window
    return a.exec();
}
