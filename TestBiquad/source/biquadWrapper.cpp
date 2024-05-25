#include <pybind11/pybind11.h>

#include "../../include/Biquad.hpp"
#include <iostream>


PYBIND11_MODULE(biquadWrapper, mod) {
	//Expose the Biquad class to Python
	pybind11::class_<giml::Biquad<float>> biquad(mod, "Biquad");
		//And all the method functions we want exposed
    biquad.def(pybind11::init<int>(), "Constructor", pybind11::arg("sampleRate"))
        .def("enable", &giml::Biquad<float>::enable)
        .def("disable", &giml::Biquad<float>::disable)
        .def("processSample", &giml::Biquad<float>::processSample)
        .def("setType", &giml::Biquad<float>::setType)
        .def("getType", &giml::Biquad<float>::getType)
        .def("setParams", &giml::Biquad<float>::setParams);


    ////Export types
    pybind11::enum_<giml::Biquad<float>::BiquadUseCase>(biquad, "BiquadUseCase")
        .value("LPF_1st", giml::Biquad<float>::BiquadUseCase::LPF_1st)
        .value("HPF_1st", giml::Biquad<float>::BiquadUseCase::HPF_1st)
        .value("LPF_2nd", giml::Biquad<float>::BiquadUseCase::LPF_2nd)
        .value("HPF_2nd", giml::Biquad<float>::BiquadUseCase::HPF_2nd)
        .value("BPF", giml::Biquad<float>::BiquadUseCase::BPF)
        .value("BSF", giml::Biquad<float>::BiquadUseCase::BSF)
        .value("LPF_Butterworth", giml::Biquad<float>::BiquadUseCase::LPF_Butterworth)
        .value("HPF_Butterworth", giml::Biquad<float>::BiquadUseCase::HPF_Butterworth)
        .value("BPF_Butterworth", giml::Biquad<float>::BiquadUseCase::BPF_Butterworth)
        .value("BSF_Butterworth", giml::Biquad<float>::BiquadUseCase::BSF_Butterworth)
        .value("LPF_LR", giml::Biquad<float>::BiquadUseCase::LPF_LR)
        .value("HPF_LR", giml::Biquad<float>::BiquadUseCase::HPF_LR)
        .value("APF_1st", giml::Biquad<float>::BiquadUseCase::APF_1st)
        .value("APF_2nd", giml::Biquad<float>::BiquadUseCase::APF_2nd)
        .value("LSF", giml::Biquad<float>::BiquadUseCase::LSF)
        .value("HSF", giml::Biquad<float>::BiquadUseCase::HSF)
        .value("PEQ", giml::Biquad<float>::BiquadUseCase::PEQ)
        .value("PEQ_constQ", giml::Biquad<float>::BiquadUseCase::PEQ_constQ)
        .export_values();	
}