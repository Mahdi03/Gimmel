#ifndef GIMMEL_BIQUAD_HPP
#define GIMMEL_BIQUAD_HPP

namespace giml {
    class Biquad {
    public:
        enum class BiquadUseCase {
            //Basic RC
            LPF_1st,        // First-Order Lowpass Filter (LPF)
            HPF_1st,        // First-Order Highpass Filter (HPF)
            
            //Second-order filters
            LPF_2nd,        // Second-Order LPF
            HPF_2nd,        // Second-Order HPF
            BPF,            // Bandpass Filter (BPF)
            
            //Butterworth Filters
            LPF_Butterworth,// Butterworth LPF
            HPF_Butterworth,// Butterworth HPF
            BPF_Butterworth,// Butterworth BPF

            //Linkwitz-Riley - steeper than Butterworth
            LPF_LR,         // Linkwitz-Riley LPF
            HPF_LR,         // Linkwitz-Riley HPF

            //All-pass filters (no frequency changes, only phase shift)
            APF_1st,        // First-Order Allpass Filter (APF)
            APF_2nd,        // Second-Order APF -> 2nd-Order APF has double the phase shift

            //Shelf filters
            LSF_1st,        // First-Order Low Shelf Filter (LSF)
            HSF_1st,        // First-Order High Shelf Filter (HSF)

            //Parametric EQ Filters
            PEQ,            //(non-const Q)
            PEQ_constQ      // Parametric EQ Filter (const Q)
        };

        Biquad() = delete;
        Biquad(Biquad::BiquadUseCase biquadUseCase) : useCase(biquadUseCase), setParams() {

        }
    private:
        BiquadUseCase useCase;


        inline void setParams() {
            switch (this->useCase) {
                case giml::Biquad::BiquadUseCase::LPF_1st:
                    break;
                case giml::Biquad::BiquadUseCase::HPF_1st:
                    break;
                case giml::Biquad::BiquadUseCase::LPF_2nd:
                    break;
                case giml::Biquad::BiquadUseCase::HPF_2nd:
                    break;
                case giml::Biquad::BiquadUseCase::BPF:
                    break;
                case giml::Biquad::BiquadUseCase::LPF_Butterworth:
                    break;
                case giml::Biquad::BiquadUseCase::HPF_Butterworth:
                    break;
                case giml::Biquad::BiquadUseCase::BPF_Butterworth:
                    break;
                case giml::Biquad::BiquadUseCase::LPF_LR:
                    break;
                case giml::Biquad::BiquadUseCase::HPF_LR:
                    break;
                case giml::Biquad::BiquadUseCase::APF_1st:
                    break;
                case giml::Biquad::BiquadUseCase::APF_2nd:
                    break;
                case giml::Biquad::BiquadUseCase::LSF_1st:
                    break;
                case giml::Biquad::BiquadUseCase::HSF_1st:
                    break;
                case giml::Biquad::BiquadUseCase::PEQ:
                    break;
                case giml::Biquad::BiquadUseCase::PEQ_constQ:
                    break;
                default:
                    break;
            }
            
        }
    };
}

#endif