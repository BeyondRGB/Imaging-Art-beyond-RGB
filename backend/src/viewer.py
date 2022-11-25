""" gui/main.py
Main window for the BeyondRGB testing application. The purpose of this
application is to assist in testing the BeyondRGB application backend. This
application provides a graphical way to select files and define the target area.
This is not meant to replace the frontend.

Classes:
    MainWindow

Authors:
    Brendan Grau <https://github.com/Victoriam7>

Licenses:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
# Python imports
import sys
from PySide6.QtWidgets import (QApplication, QMainWindow, QPushButton,
                               QPlainTextEdit, QVBoxLayout, QWidget,
                               QGraphicsScene, QGraphicsView)
from PySide6.QtCore import QProcess

# Local imports
from target_selector import target_selector


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.brgb = None

        self.start_btn = QPushButton("Execute")
        self.start_btn.pressed.connect(self.execute)
        self.text = QPlainTextEdit()
        self.text.setReadOnly(True)

        x = target_selector()

        self.target_img = QGraphicsScene()
        self.target_img.addPixmap(x)
        view = QGraphicsView(self.target_img)
        view.show()

        layout = QVBoxLayout()
        layout.addWidget(self.start_btn)
        layout.addWidget(self.text)
        layout.addWidget(view)

        window = QWidget()
        window.setLayout(layout)

        self.setCentralWidget(window)

    def message(self, s):
        self.text.appendPlainText(s)

    def execute(self):
        if self.brgb is None:  # No process running.
            self.message("Executing process")

            # Innit process and hook up outputs
            self.brgb = QProcess()  # Keep a reference to the process
            self.brgb.readyReadStandardOutput.connect(self.handle_stdout)
            self.brgb.readyReadStandardError.connect(self.handle_stderr)
            self.brgb.stateChanged.connect(self.handle_state)
            self.brgb.finished.connect(self.process_finished)  

            # Start process
            self.brgb.start("python3",
                            ['main.py', '../../../images/SampleImages',
                             '1', '2', '3', '4', '5', '6', '7', '8'])

    def handle_stderr(self):
        data = self.brgb.readAllStandardError()
        stderr = bytes(data).decode("utf8")
        self.message(stderr)

    def handle_stdout(self):
        data = self.brgb.readAllStandardOutput()
        stdout = bytes(data).decode("utf8")
        self.message(stdout)

    def handle_state(self, state):
        states = {
            QProcess.NotRunning: 'Not running',
            QProcess.Starting: 'Starting',
            QProcess.Running: 'Running',
        }
        state_name = states[state]
        self.message(f"State changed: {state_name}")

    def process_finished(self):
        self.message("Process finished.")
        self.brgb = None


if __name__ == "__main__":
    app = QApplication(sys.argv)

    w = MainWindow()
    w.show()

    app.exec()
