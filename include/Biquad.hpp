#ifndef GIML_BIQUAD_HPP
#define GIML_BIQUAD_HPP

#import <math.h>

namespace giml {
    template <typename T>
    class Biquad : Effect<T> {
    public:
        enum class BiquadUseCase {
            PassThroughDefault, //Default type until parameters are set
            //Basic RC
            LPF_1st,        // First-Order Lowpass Filter (LPF)
            HPF_1st,        // First-Order Highpass Filter (HPF)
            
            //Second-order filters
            LPF_2nd,        // Second-Order LPF
            HPF_2nd,        // Second-Order HPF
            BPF,            // Bandpass Filter (BPF)
            BSF,            // Bandstop Filter (BSF)
            
            //Butterworth Filters
            LPF_Butterworth,// Butterworth LPF
            HPF_Butterworth,// Butterworth HPF
            BPF_Butterworth,// Butterworth BPF
            BSF_Butterworth,// Butterworth BSF

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
            //PEQ,            //(non-const Q)
            PEQ_constQ      // Parametric EQ Filter (const Q)
        };

        Biquad() = delete;
        Biquad(int sampleRate) : sampleRate(sampleRate) {

        }
        //TODO: Copy constructor + Copy assignment constructor
        ~Biquad() {}

        void setParams__LPF_1st(float cutoffFrequency) {
            //Set type to low-pass if not already
            if (this->useCase != BiquadUseCase::LPF_1st) {
                this->useCase = BiquadUseCase::LPF_1st;
            }
            float cutoffAngle = GIML_TWO_PI * cutoffFrequency / this->sampleRate;
            float gamma = ::cosf(cutoffAngle) / (1 + ::sinf(cutOffAngle));
            this->a0 = (1 - gamma) / 2;
            this->a1 = this->a0;
            this->a2 = 0;
            this->b1 = -gamma;
            this->b2 = 0;

            this->wet = 1;
            this->dry = 0;
        }

        void setParams__HPF_1st(float cutoffFrequency) {
            //Set type to low-pass if not already
            if (this->useCase != BiquadUseCase::HPF_1st) {
                this->useCase = BiquadUseCase::HPF_1st;
            }
            float cutoffAngle = GIML_TWO_PI * cutoffFrequency / this->sampleRate;
            float gamma = ::cosf(cutoffAngle) / (1 + ::sinf(cutOffAngle));
            this->a0 = (1 + gamma) / 2;
            this->a1 = -this->a0;
            this->a2 = 0;
            this->b1 = -gamma;
            this->b2 = 0;

            this->wet = 1;
            this->dry = 0;
        }

        void setParams__LPF_2nd(float cutoffFrequency, float Q) {
            //Set type to low-pass if not already
            if (this->useCase != BiquadUseCase::LPF_2nd) {
                this->useCase = BiquadUseCase::LPF_2nd;
            }
            float cutoffAngle = GIML_TWO_PI * cutoffFrequency / this->sampleRate;
            float d = ::sinf(cutoffAngle) / (2 * Q);
            float Beta = (1 + (1 - d) / (1 + d)) / 2;
            float gamma = Beta * ::cosf(cutoffAngle);


            this->a0 = (Beta - gamma) / 2;
            this->a1 = Beta - gamma;
            this->a2 = this->a0;
            this->b1 = -2*gamma;
            this->b2 = 2*Beta -1;

            this->wet = 1;
            this->dry = 0;
        }

        void setParams__HPF_2nd(float cutoffFrequency, float Q) {
            //Set type to high-pass if not already
            if (this->useCase != BiquadUseCase::HPF_2nd) {
                this->useCase = BiquadUseCase::HPF_2nd;
            }
            float cutoffAngle = GIML_TWO_PI * cutoffFrequency / this->sampleRate;
            float d = ::sinf(cutoffAngle) / (2 * Q);
            float Beta = (1 + (1 - d) / (1 + d)) / 2;
            float gamma = Beta * ::cosf(cutoffAngle);


            this->a0 = (Beta + gamma) / 2;
            this->a1 = -(Beta + gamma);
            this->a2 = this->a0;
            this->b1 = -2 * gamma;
            this->b2 = 2 * Beta - 1;

            this->wet = 1;
            this->dry = 0;
        }



        T processSample(T in) {
            T returnVal;
            switch (useCase) {
            case giml::Biquad::BiquadUseCase::LPF_1st:
            case giml::Biquad::BiquadUseCase::HPF_1st:
                returnVal = a0 * in + a1 * prevX1 - b1 * prevY1;
                break;
            case giml::Biquad::BiquadUseCase::LPF_2nd:
            case giml::Biquad::BiquadUseCase::HPF_2nd:
                returnVal = a0 * in + a1 * prevX1 + a2*prevX2 - b1 * prevY1 - b2*prevY2;
                break;
            case giml::Biquad::BiquadUseCase::BPF:
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

            //Back propagate inputs and outputs
            prevX2 = prevX1;
            prevX1 = in;

            prevY2 = prevY1;
            prevY1 = returnVal;

            return returnVal;
        }
    private:
        BiquadUseCase useCase = BiquadUseCase::PassThroughDefault;

        int sampleRate;

        T a0=1, a1=0, a2=0,   //Numeratror coefficients (set a0 to 1 for default passthrough)
            b1=0, b2=0;     //Denominator coefficients
        //Past 2 x,y values
        T prevX1, prevX2,
            prevY1, prevY2;

        float wet = 1.f, dry = 0.f; //Percentages of wet and dry mix to add

        enum transferFunctionCoefficientsIndices {
            a0=0, a1, a2,
            b1, b2
            c0, d0,
            numCoefficients //This will be 1 more and thus be the length of all we need to store
        };
        T transferFunctionCoefficients[transferFunctionCoefficientsIndices::numCoefficients];
        
        enum prevValuesIndices {
            X_z1 = 0, X_z2,
            Y_z1, Y_z2,
            numValues //This will be 1 more and thus be the length of all we need to store
        };
        T previousValuesRegisters[prevValuesIndices::numValues];

        inline void setParams(Biquad::BiquadUseCase useCase) {
            switch (useCase) {
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
                case giml::Biquad::BiquadUseCase::BSF:
                    break;
                case giml::Biquad::BiquadUseCase::LPF_Butterworth:
                    break;
                case giml::Biquad::BiquadUseCase::HPF_Butterworth:
                    break;
                case giml::Biquad::BiquadUseCase::BPF_Butterworth:
                    break;
                case giml::Biquad::BiquadUseCase::BSF_Butterworth:
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