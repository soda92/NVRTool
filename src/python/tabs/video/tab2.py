from PyQt6 import QtWidgets
import sys


def main():

    app = QtWidgets.QApplication(sys.argv)
    tabW = QtWidgets.QTabWidget()
    tabW.setStyleSheet(
        """
        QTabBar::tab:disabled {
            width: 300px;
            color: transparent;
            background: transparent;
        }
        QTabBar{
            top: 30px;
        }
        """
    )
    pushButton1 = QtWidgets.QPushButton("QPushButton 1")
    pushButton2 = QtWidgets.QPushButton("QPushButton 2")

    tab1 = QtWidgets.QWidget()
    tab2 = QtWidgets.QWidget()
    emptySpace = QtWidgets.QWidget()
    tab3 = QtWidgets.QWidget()

    vBoxlayout = QtWidgets.QVBoxLayout()
    vBoxlayout.addWidget(pushButton1)
    vBoxlayout.addWidget(pushButton2)

    # Resize width and height
    tabW.resize(500, 500)

    # Set Layout for Third Tab Page
    tab3.setLayout(vBoxlayout)

    tabW.addTab(tab1, "Tab 1")
    tabW.addTab(tab2, "Tab 2")
    tabW.addTab(emptySpace, "ES")
    tabW.setTabEnabled(2, False)
    tabW.addTab(tab3, "Tab 3")

    tabW.setWindowTitle("PyQt QTabWidget Add Tabs and Widgets Inside Tab")
    tabW.show()

    sys.exit(app.exec())


if __name__ == "__main__":
    main()
