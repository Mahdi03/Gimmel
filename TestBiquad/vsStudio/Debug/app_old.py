import numpy
from numba import jit
import matplotlib.pyplot as plot
import scipy.signal
from matplotlib.widgets import Slider, Button, RadioButtons

from biquadWrapper import Biquad

durationSecs = 1

sampleRate = 48000
blockSize = 1024
numBlocks = 5
numSamples = int(sampleRate * durationSecs)
biquad = Biquad(sampleRate)

def getSignalOut(signalIn):
    return numpy.array([biquad.processSample(sample) for sample in signalIn])

def freqResponse(signalIn, signalOut, sampleRate, blockSize):

    # window = scipy.signal.get_window("hamming", blockSize)

    # magnitudeResponse = numpy.zeros(blockSize//2)
    # phaseResponse = numpy.zeros(blockSize//2)

    # for _ in range(numBlocks):
    #     noise = numpy.random.uniform(-1, 1, blockSize)
    #     noise *= window
    #     out = getSignalOut(noise)

    #     IN = numpy.fft.fft(noise)[:blockSize // 2]
    #     OUT = numpy.fft.fft(out)[:blockSize // 2]

    #     H = OUT/IN

    #     magnitudeResponse += 20 * numpy.log10(numpy.abs(H))
    #     phaseResponse += numpy.angle(H, deg=True)

    X = numpy.fft.fft(signalIn)[:blockSize // 2]
    Y = numpy.fft.fft(signalOut)[:blockSize // 2]

    H = Y / X

    magnitude = 20 * numpy.log10(numpy.abs(H))
    phase = numpy.angle(H, deg=True)


    # frequencies, inputPowerSpectralDensities = welch(signalIn, fs=sampleRate, nperseg=blockSize)
    # _, outputPowerSpectralDensities = welch(signalOut, fs=sampleRate, nperseg=blockSize)
    # gain = 10*numpy.log10(signalOut/signalIn)
    # phaseDiff = numpy.angle(numpy.fft.fft(signalOut)) - numpy.angle(numpy.fft.fft(signalIn))
    # phase = numpy.unwrap(phaseDiff)[:len(frequencies)]

    return magnitude, phase

def liveBodePlot():
    biquad.setType(biquad.LPF_1st)
    print(biquad.getType())
    biquad.setParams(1000, 0, 0)
    biquad.enable()
    plot.ion()
    figure, (axesMagnitude, axesPhase) = plot.subplots(2, 1, figsize=(5, 4))
    
    # Define sliders and radio buttons
    ax_freq = plot.axes([0.1, 0.25, 0.65, 0.03], facecolor='lightgoldenrodyellow')
    ax_q = plot.axes([0.1, 0.2, 0.65, 0.03], facecolor='lightgoldenrodyellow')
    ax_gain = plot.axes([0.1, 0.15, 0.65, 0.03], facecolor='lightgoldenrodyellow')
    ax_toggle = plot.axes([0.1, 0.05, 0.1, 0.04])
    ax_type = plot.axes([0.1, 0.01, 0.2, 0.04])

    slider_Frequency = Slider(ax_freq, 'f', 20, sampleRate / 2, valinit=1000)
    slider_Q = Slider(ax_q, 'Q', 0.1, 10.0, valinit=0.707)
    slider_Gain = Slider(ax_gain, 'Gain', -24, 24, valinit=0)
    toggle_button = Button(ax_toggle, 'Toggle Filter')
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
    radio = RadioButtons(ax_type, filterTypes)

    biquadEnabled = True
    def toggle_filter(event):
        if biquadEnabled:
            biquad.disable()
        else:
            biquad.enable()
    
    def updateBiquadType(label):
        match label:
            case "LPF_1st":
                biquad.setType(biquad.LPF_1st)
            case "HPF_1st":
                biquad.setType(biquad.HPF_1st)
            case "LPF_2nd":
                biquad.setType(biquad.LPF_2nd)
            case "HPF_2nd":
                biquad.setType(biquad.HPF_2nd)
            case "BPF":
                biquad.setType(biquad.BPF)
            case "BSF":
                biquad.setType(biquad.BSF)
            case "LPF_Butterworth":
                biquad.setType(biquad.LPF_Butterworth)
            case "HPF_Butterworth":
                biquad.setType(biquad.HPF_Butterworth)
            case "BPF_Butterworth":
                biquad.setType(biquad.BPF_Butterworth)
            case "BSF_Butterworth":
                biquad.setType(biquad.BSF_Butterworth)
            case "LPF_LR":
                biquad.setType(biquad.LPF_LR)
            case "HPF_LR":
                biquad.setType(biquad.HPF_LR)
            case "APF_1st":
                biquad.setType(biquad.APF_1st)
            case "APF_2nd":
                biquad.setType(biquad.APF_2nd)
            case "LSF":
                biquad.setType(biquad.LSF)
            case "HSF":
                biquad.setType(biquad.HSF)
            case "PEQ":
                biquad.setType(biquad.PEQ)
            case "PEQ_constQ":
                biquad.setType(biquad.PEQ_constQ)

    def updateBiquadParams(val):
        # Update Biquad parameters
        biquad.setParams(slider_Frequency.val, slider_Q.val, slider_Gain.val)

    slider_Frequency.on_changed(updateBiquadParams)
    slider_Q.on_changed(updateBiquadParams)
    slider_Gain.on_changed(updateBiquadParams)

    toggle_button.on_clicked(toggle_filter)
    radio.on_clicked(updateBiquadType)
    # Generate white noise input
    signalIn = numpy.random.uniform(-1, 1, numSamples).astype(numpy.float32)
    signalOut = getSignalOut(signalIn)

    magnitude_response, phase_response = freqResponse(signalIn, signalOut, sampleRate, blockSize)
    freqs = numpy.fft.fftfreq(blockSize, 1/sampleRate)[:blockSize //2]
    # Initialize plots
    lineMagnitude, = axesMagnitude.plot(freqs, magnitude_response, 'b')
    linePhase, = axesPhase.plot(freqs, phase_response, 'r')

    def updatePlot():
        signalIn = numpy.random.uniform(-1, 1, blockSize).astype(numpy.float32)
        signalOut = getSignalOut(signalIn)
        magnitude_response, phase_response = freqResponse(signalIn, signalOut, sampleRate, blockSize)
        lineMagnitude.set_ydata(magnitude_response)
        linePhase.set_ydata(phase_response)
        figure.canvas.draw()
        figure.canvas.flush_events()
        # print("Did we do something?")

    axesMagnitude.set_title('Magnitude Response')
    axesMagnitude.set_ylabel('Magnitude (dB)')
    axesMagnitude.set_xscale('log')
    axesMagnitude.set_xlim(20, sampleRate / 2)
    axesMagnitude.set_ylim(-40, 10)
    axesMagnitude.grid(True)

    axesPhase.set_title('Phase Response')
    axesPhase.set_xlabel('Frequency (Hz)')
    axesPhase.set_ylabel('Phase (degrees)')
    axesPhase.set_xscale('log')
    axesPhase.set_xlim(20, sampleRate / 2)
    axesPhase.set_ylim(-180, 180)
    axesPhase.grid(True)

    while True:
        updatePlot()
        plot.pause(0.0167)

biquad.setParams__LPF_1st(500)
biquad.setType(biquad.LPF_1st)
biquad.processSample(5)


liveBodePlot()