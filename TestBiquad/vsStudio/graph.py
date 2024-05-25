import numpy

from PyQt5.QtWidgets import (QApplication, QMainWindow, QGridLayout,
                             QWidget, QSlider, QPushButton, QComboBox, QLabel,
                             QHBoxLayout, QVBoxLayout)
from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QColor, QPen
import pyqtgraph as pg
# import sys
# sys.path.append("./Debug/")
from Debug.biquadWrapper import Biquad
#from biquadWrapper import Biquad

class UIControlsWidget(QWidget):
    def __init__(self, sampleRate):
        super().__init__()
        self.layout = QVBoxLayout()
        self.setLayout(self.layout)
        self.freq_slider = self.createSlider("Frequency", 1000, 1, sampleRate // 2, 1)
        self.q_slider = self.createSlider("Q", 0.707, 0.1, 10, 0.1, 10)
        self.gain_slider = self.createSlider("Gain", 0, -24, 24, 0.1, 10)


    def connectSliderCallbacks(self, callback):
        self.freq_slider.valueChanged.connect(callback)
        self.q_slider.valueChanged.connect(callback)
        self.gain_slider.valueChanged.connect(callback)


    def createSlider(self, name, defaultVal, minVal, maxVal, tickInterval, divider=1):
        label = QLabel(f"{name}:")
        slider = QSlider()
        slider.setRange(int(minVal*divider), int(maxVal*divider))
        slider.setOrientation(1)  # Set to horizontal orientation
        slider.setValue(int(defaultVal*divider))
        slider.setTickInterval(int(tickInterval*divider))
        valueLabel = QLabel(f"{slider.value()/divider:.3f}") # Up to 3 decimal places
            
        slider.valueChanged.connect(lambda value: valueLabel.setText(f"{value/divider}"))

        hbox = QHBoxLayout()
        hbox.addWidget(label)
        hbox.addWidget(slider)
        hbox.addWidget(valueLabel)

        self.layout.addLayout(hbox)
        return slider


class RealTimePlotWidget(QWidget):
    def initPlots(self):
        self.magnitudePlotWidget = pg.PlotWidget()
        self.phasePlotWidget = pg.PlotWidget()

        self.layout.addWidget(self.magnitudePlotWidget, 0, 0)
        self.layout.addWidget(self.phasePlotWidget, 0, 1)
        self.magnitudePlotItem = self.magnitudePlotWidget.plotItem
        self.phasePlotItem = self.phasePlotWidget.plotItem

        self.magnitudePlotItem.setLabel("left", "Magnitude (dB)")
        self.magnitudePlotItem.setLabel('bottom', 'Frequency (Hz)')
        self.magnitudePlotItem.setTitle('Magnitude Response')

        self.phasePlotItem.setLabel('left', 'Phase (degrees)')
        self.phasePlotItem.setLabel('bottom', 'Frequency (Hz)')
        self.phasePlotItem.setTitle('Phase Response')

        # Set log scale for x-axis
        self.magnitudePlotItem.setLogMode(x=True)
        self.phasePlotItem.setLogMode(x=True)

        #self.magnitudePlotItem.setXRange(self.freqs[0], self.freqs[-1])
        self.magnitudePlotItem.setYRange(-40, 20)
        #self.phasePlotItem.setXRange(min(self.freqs), max(self.freqs))
        self.phasePlotItem.setYRange(-180, 180)

        # Add grids
        self.magnitudePlotItem.showGrid(True, True)
        self.phasePlotItem.showGrid(True, True)

        # Create PlotDataItems for magnitude and phase curves
        pen1 = QPen(QColor(4, 217, 255), 1)
        pen1.setCosmetic(True)
        pen2 = QPen(QColor(255, 87, 51), 1)
        pen2.setCosmetic(True)
        self.mag_curve = self.magnitudePlotItem.plot(pen=pen1)
        self.phase_curve = self.phasePlotItem.plot(pen=pen2)
        

    def initUI(self):
        self.layout = QGridLayout()
        self.setLayout(self.layout)
    
    def initUIControls(self):
        self.uiControlsWidget = UIControlsWidget(self.sampleRate)
        self.uiControlsWidget.connectSliderCallbacks(self.updateBiquadParams)
        self.layout.addWidget(self.uiControlsWidget)

        # self.freq_label = QLabel('Frequency')
        # self.layout.addWidget(self.freq_label, 1, 0)
        # self.freq_slider = QSlider()
        # self.freq_slider.setOrientation(1)  # Vertical orientation
        # self.freq_slider.setMinimum(1)
        # self.freq_slider.setMaximum(self.sampleRate // 2)
        # self.freq_slider.setTickInterval(100)
        # self.freq_slider.setValue(1000) # Default initial cutoff frequency
        # self.freq_slider.valueChanged.connect(self.updateBiquadParams)
        # self.layout.addWidget(self.freq_slider, 1, 1)

        # self.q_label = QLabel('Q')
        # self.layout.addWidget(self.q_label, 2, 0)
        # self.q_slider = QSlider()
        # self.q_slider.setOrientation(1)  # Vertical orientation
        # # use x10 values
        # self.q_slider.setMinimum(1)
        # self.q_slider.setMaximum(100)
        # self.q_slider.setTickInterval(1)
        # self.q_slider.setValue(7)
        # self.q_slider.valueChanged.connect(self.updateBiquadParams)
        # self.layout.addWidget(self.q_slider, 2, 1)

        # self.gain_label = QLabel('Gain')
        # self.layout.addWidget(self.gain_label, 3, 0)
        # self.gain_slider = QSlider()
        # self.gain_slider.setOrientation(1)  # Vertical orientation
        # self.gain_slider.setMinimum(-240)
        # self.gain_slider.setMaximum(240)
        # self.gain_slider.setTickInterval(1)
        # self.gain_slider.setValue(0)
        # self.gain_slider.valueChanged.connect(self.updateBiquadParams)
        # self.layout.addWidget(self.gain_slider, 3, 1)

        self.toggle_button = QPushButton('Toggle Filter')
        self.toggle_button.clicked.connect(self.toggleBiquad)
        self.layout.addWidget(self.toggle_button, 4, 0)

        self.type_label = QLabel('Filter Type')
        self.layout.addWidget(self.type_label, 5, 0)
        self.type_combo = QComboBox()
        filterTypes = [
            "LPF_1st",
            "HPF_1st",
            "LPF_2nd",
            "HPF_2nd",
            "BPF",
            "BSF",
            "LPF_Butterworth",
            "HPF_Butterworth",
            "BPF_Butterworth",
            "BSF_Butterworth",
            "LPF_LR",
            "HPF_LR",
            "APF_1st",
            "APF_2nd",
            "LSF",
            "HSF",
            "PEQ",
            "PEQ_constQ"
        ]
        self.type_combo.addItems(filterTypes)
        self.type_combo.currentIndexChanged.connect(self.updateBiquadType)
        self.layout.addWidget(self.type_combo, 5, 1)


    def __init__(self, sampleRate, blockSize):
        super().__init__()

        self.sampleRate = sampleRate
        self.blockSize = blockSize
        self.biquad = Biquad(self.sampleRate)
        self.biquad.setType(Biquad.LPF_1st)
        self.biquadEnabled = False
        self.freqs = numpy.fft.fftfreq(self.blockSize, 1/self.sampleRate)[:self.blockSize //2]

        
        self.initUI()
        self.initPlots()
        self.initUIControls()

        self.show()
        # Set render loop
        self.timer = QTimer()
        self.timer.timeout.connect(self.renderPlot)
        self.timer.setInterval(17) # milliseconds
        self.timer.start()

    def toggleBiquad(self):
        if self.biquadEnabled:
            self.biquad.disable()
        else:
            self.biquad.enable()
    
    biquadTypes = [
        Biquad.LPF_1st,
        Biquad.HPF_1st,
        Biquad.LPF_2nd,
        Biquad.HPF_2nd,
        Biquad.BPF,
        Biquad.BSF,
        Biquad.LPF_Butterworth,
        Biquad.HPF_Butterworth,
        Biquad.BPF_Butterworth,
        Biquad.BSF_Butterworth,
        Biquad.LPF_LR,
        Biquad.HPF_LR,
        Biquad.APF_1st,
        Biquad.APF_2nd,
        Biquad.LSF,
        Biquad.HSF,
        Biquad.PEQ,
        Biquad.PEQ_constQ
    ]

    def updateBiquadType(self, index):
        self.biquad.setType(self.biquadTypes[index])

    def updateBiquadParams(self):
        self.biquad.setParams(self.uiControlsWidget.freq_slider.value(),
                              self.uiControlsWidget.q_slider.value()/10,
                              self.uiControlsWidget.gain_slider.value()/10)

    def getSignalOut(self, signalIn):
        return numpy.array([self.biquad.processSample(sample) for sample in signalIn])

    def renderPlot(self):
        signalIn = numpy.random.uniform(-1, 1, self.blockSize).astype(numpy.float32)
        signalOut = self.getSignalOut(signalIn)
        X = numpy.fft.fft(signalIn)[:self.blockSize // 2]
        Y = numpy.fft.fft(signalOut)[:self.blockSize // 2]

        H = Y / X

        magnitude = 20 * numpy.log10(numpy.abs(H))
        phase = numpy.angle(H, deg=True)

        self.mag_curve.setData(self.freqs, magnitude)
        self.phase_curve.setData(self.freqs, phase)


class MainWindow(QWidget):
    def __init__(self):
        super().__init__()

        self.layout = QHBoxLayout()
        self.setLayout(self.layout)

        self.plot_widget = RealTimePlotWidget()
        self.control_panel = UIControlsWidget()

        

        self.layout.addWidget(self.control_panel)
        self.layout.addWidget(self.plot_widget)
if __name__ == "__main__":
    app = QApplication([])
    win = QMainWindow()
    widget = RealTimePlotWidget(48000, 1024)
    win.setCentralWidget(widget)
    win.show()
    app.exec_()