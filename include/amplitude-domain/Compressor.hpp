#ifndef GIMMEL_COMPRESSOR_HPP
#define GIMMEL_COMPRESSOR_HPP
namespace giml {
    template <typename T>
    class Compressor {
    private:
        float threshold = 1.f, ratio = 1.f;
        float attackIncrement = 0.f, releaseDecrement = 0.f;
        float ramp = 0.f; 
        bool hardKnee = true;
        int sampleRate;

    public:
        Compressor(int sampleRate) : sampleRate(sampleRate) {}
        ~Compressor() {}
        //Copy constructor
        Compressor(const Compressor& c) {
            this->sampleRate = c.sampleRate;
            this->threshold = c.threshold;
            this->ratio = c.ratio;
            this->attackIncrement = c.attackIncrement;
            this->releaseDecrement = c.releaseDecrement;
            this->ramp = c.ramp;
            this->hardKnee = c.hardKnee;
        }
        //Copy assignment constructor
        Compressor& operator=(const Compressor& c) {
            this->sampleRate = c.sampleRate;
            this->threshold = c.threshold;
            this->ratio = c.ratio;
            this->attackIncrement = c.attackIncrement;
            this->releaseDecrement = c.releaseDecrement;
            this->ramp = c.ramp;
            this->hardKnee = c.hardKnee;
            return *this;
        }

        T processValue(T in) {
            if (hardKnee) {

            }
        }

        void setAttack() {

        }
        void setRelease() {

        }
        void setHardKnee(bool knee) {
            this->hardKnee = knee;
        }
        
    };
}
#endif