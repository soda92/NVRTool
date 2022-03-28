import vlc
import pathlib

CURRENT = pathlib.Path(__file__).resolve().parent
VIDEO = CURRENT.joinpath("test-video.mkv")


class Client:
    def __init__(self, hwnd, ip):
        self.hwnd = hwnd
        self.ip = ip
        self.instance = vlc.Instance()
        self.player = self.instance.media_player_new()
        self.player.set_hwnd(self.hwnd)
        self.media = self.instance.media_new(str(VIDEO))
        self.player.set_media(self.media)

    def stream(self):
        self.player.play()
