#include <QWidget>
#include <QtWidgets>
#include <QMainWindow>
#include <QDebug>
#include <memory>
#include <QFile>
#include <fmt/core.h>
using namespace std;

class A
{
public:
    inline static int count = 0;
    A()
    {
        count += 1;
        fmt::print("object count: {}\n", count);
    };
    void print()
    {
        fmt::print("A used\n");
    };
    ~A()
    {
        count -= 1;
        fmt::print("object count: {}\n", count);
        fmt::print("A deleted\n");
    };
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow *parent = 0) : QMainWindow(parent)
    {
        auto topbar_w = new QWidget;
        auto topbar = new QHBoxLayout;
        topbar_w->setLayout(topbar);
        auto label = new QLabel(tr("Keyword:"));
        topbar->addWidget(label);
        topbar->addWidget(lineedit);

        topbar->addWidget(btn);
        auto full = new QWidget;
        auto l2 = new QVBoxLayout;
        full->setLayout(l2);
        l2->addWidget(topbar_w);
        l2->addWidget(textedit);
        a = new A;
        // a = new A;
        // a = new A;

        setCentralWidget(full);
        full->setMinimumSize(800, 600);
        setWindowTitle(tr("Connecting buttons to processes.."));
        connect(btn, &QPushButton::clicked, this, &MainWindow::on_findButton_clicked);

        loadTextFile();
    };
    ~MainWindow()
    {
        delete a;
    }
public slots:
    void on_findButton_clicked()
    {
        QString searchString = lineedit->text();
        fmt::print("1\n");
        {
            auto a = make_unique<A>();
            a.get()->print();
        }
        fmt::print("2\n");
        textedit->find(searchString, QTextDocument::FindWholeWords);
    };

private:
    QPushButton *btn = new QPushButton(tr("Find"));
    QTextEdit *textedit = new QTextEdit();
    QLineEdit *lineedit = new QLineEdit();
    A *a;
    void loadTextFile()
    {
        QFile inputFile(":/input.txt");
        inputFile.open(QIODevice::ReadOnly);

        QTextStream in(&inputFile);
        QString line = in.readAll();
        inputFile.close();

        textedit->setPlainText(line);
        QTextCursor cursor = textedit->textCursor();
        cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
    };
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show(); // Show main window
    return a.exec();
}
