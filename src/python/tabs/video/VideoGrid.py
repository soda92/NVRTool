from PyQt5 import QtWidgets

class VideoGrid(QtWidgets.QGridLayout):
    def __init__(self, max_count) -> None:
        super(VideoGrid, self).__init__()
        self.max_count = max_count
        # self.count = 0
        self.current_row = 0
        self.current_column = 0

    def addWidget(self, widget):
        # print(self.current_row,self.current_column)
        super().addWidget(widget, self.current_row, self.current_column)
        self.current_column += 1
        if self.current_column >= self.max_count:
            self.current_row += 1
            self.current_column = 0
