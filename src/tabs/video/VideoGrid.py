import typing
from PyQt6 import QtWidgets, QtCore
from .VideoFrame import VideoFrame


class VideoGrid(QtWidgets.QGridLayout):
    widgets: typing.List[VideoFrame]
    """子窗口"""

    def __init__(self, max_count) -> None:
        super(VideoGrid, self).__init__()
        self.setSpacing(0)
        self.max_count = max_count
        self.numbers = 0
        self.current_row = 0
        self.current_column = 0
        self.widgets: typing.List[VideoFrame] = []
        self.maxmized = False

    def addWidget(self, widget: VideoFrame):
        """添加子窗口"""
        self.widgets.append(widget)
        super().addWidget(widget, self.current_row, self.current_column)
        widget.number = self.numbers
        widget.double_click_signal.connect(self.maxmize)

        self.numbers += 1
        self.current_column += 1
        if self.current_column >= self.max_count:
            self.current_row += 1
            self.current_column = 0

    @QtCore.pyqtSlot(int)
    def maxmize(self, number: int) -> None:
        """子窗口最大最小化"""
        if self.maxmized:
            for w in self.widgets:
                w.show()
            self.maxmized = False
        else:
            self.maxmized = True
            for w in self.widgets:
                if w.number != number:
                    w.hide()
