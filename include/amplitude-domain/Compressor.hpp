#ifndef GIMMEL_COMPRESSOR_HPP
#define GIMMEL_COMPRESSOR_HPP
#include <math.h>
#include "../utility.hpp"

// See: https://isocpp.org/wiki/faq/pointers-to-members
#define CALL_PTR_TO_MEMBER_FUNCTION(overarchingClass, ptrToMember) ((overarchingClass).*(ptrToMember))

namespace giml {

    /**
     * @brief This class models the Compressor effect
     * 
     * 
     * This effect allows the setting of the following parameters (see ______ for more details on the parameters):
     * - threshold: the cutoff amplitude at which the compressor will attenuate
     * - ratio: how much the 
     * - attack time (ms):
     * - hold time (ms):
     * - release time (ms):
     * TODO: add `knee` parameter and functionality
     * See ___________________ for the link to 
     * 
     * @tparam T floating-point type for input and output sample data such as `float`, `double`, or `long double`,
     * up to user what precision they are looking for (float is more performant)
     */
    template <typename T>
    class Compressor {
    private:
        float threshold = 1.f, ratio = 1.f;
        float attackIncrement = 0.f, releaseDecrement = 0.f;
        int holdTimeSamples = 0, numHoldTimeSamplesLeft = -1;
        float ramp = 0.f; 
        bool hardKnee = false; //TODO: Later will become a numeric value for how much knee they want to apply
        int sampleRate;

        //State machine internals:
        //  using C style function pointers for compatibility with different architectures with fewer changes required
        typedef T (Compressor::*StateHandler)(T);
        StateHandler pCurrentStateFunc = nullptr; //Current state tracker (function pointer of type `void func(T in)`)
        
        //Cache function pointers to avoid reference address calls every sample
        StateHandler pIdleState, pAttackState, pHoldState, pReleaseState;
        
        inline void transitionToAttackState() {
            this->pCurrentStateFunc = this->pAttackState;
        }
        inline T state_Attack(T in) {
            /**
             * In the attack state, we are slowly ramping up our grasp on the signal
             * If sample > threshold - continue ramping and hold state if we are done ramping
             * if sample < threshold - then we go into the hold state to hold at whatever ramp we are at
             * 
             */
            //Check to see the current sample's value
            float magnitude = ::fabs(in);
            float diff = magnitude - this->threshold;
            float gain;
            if (magnitude > this->threshold) { //If sample > threshold
                //Then the sample is over the threshold, we continue attacking unless we've reached the top
                if (this->ramp >= 1) { // and ramp is max...
                    //Then we need to transition into the hold state for the next incoming samples
                    this->transitionToHoldState();
                    //Also do the correct signal processing for this 
                    gain = (1 - diff * this->ratio);
                }
                else { //ramp still has a ways to go up...
                    //Then we remain in the Attack state and increment our ramp value
                    this->ramp += attackIncrement;
                    gain = (1 - (ramp * diff * this->ratio));
                }
            }
            else {
                //Then sample < threshold and we need to go into the Hold state if numSamplesHold is > 0 (taken care of by transition)
                this->transitionToHoldState();
                gain = (1 - (diff * this->ratio)); 
            }
            return in * gain;

        }

        inline void resetHoldTimeout() {
            this->numHoldTimeSamplesLeft = this->holdTimeSamples; //Reset the number of hold samples we have left
        }
        inline void transitionToHoldState() {
            if (this->holdTimeSamples > 0) {
                //Then they have opted for a hold state
                this->resetHoldTimeout();
                this->pCurrentStateFunc = this->pHoldState;
            }
            else {
                //We must directly skip and go to the release state
                this->transitionToReleaseState();
            } 
        }
        inline T state_Hold(T in) {
            /**
             * In the hold state, check the type of incoming sample first
             * 
             * If sample >= threshold, we reset the hold time-out; if attack is not finished then we go back to attacking
             * If sample < threshold, we start decrementing the hold time-out counter until 0 (then release)
             */

            //Check to see the current sample's value
            float magnitude = ::fabs(in);
            float diff = magnitude - this->threshold;
            float gain;
            if (magnitude > this->threshold) { //If sample > threshold
                if (this->ramp < 1) { // If we never finished ramping to begin with before coming here
                    this->transitionToAttackState();
                }
                else {
                    this->resetHoldTimeout();
                }
            }
            else { // sample < threshold
                this->numHoldTimeSamplesLeft--;
                if (this->numHoldTimeSamplesLeft == 0) {
                    //Then the next one will finally be release
                    this->transitionToReleaseState();
                }
            }
            //Include ramp here in case we went into hold without finishing our ramp up
            gain = (1 - diff * this->ramp * this->ratio); //TODO: The gain should be the same either direction the sample is in?
            return in * gain;
        }

        inline void transitionToReleaseState() {
            this->pCurrentStateFunc = this->pReleaseState;
        }
        inline T state_Release(T in) {
            /**
             * In the release state we are diminishing our hold on the signal
             * 
             * If sample < threshold -> keep releasing until ramp = 0
             * 
             * If sample > threshold -> skip releasing and start attack again
             * 
             */
            //Check to see the current sample's value
            float magnitude = ::fabs(in);
            float diff = magnitude - this->threshold;
            float gain;
            if (magnitude > this->threshold) { //If sample > threshold
                this->transitionToAttackState();
            }
            else {
                if (this->ramp > 0) {
                    this->ramp -= this->releaseDecrement;
                }
            }
            gain = (1 - (ramp * diff * this->ratio));
            return in * gain;
        }

        inline void transitionToIdleState() {
            //By this time the ramp value should be 0, so floor it just in case
            this->ramp = 0;
            this->pCurrentStateFunc = this->pIdleState;
        }
        inline T state_Idle(T in) {
            /**
             * IDLE is the default state of the compressor (do nothing)
             * 
             * If sample > threshold -> then we need to enable the attack
             * 
             */
            //Check to see the current sample's value
            float magnitude = ::fabs(in);
            //float diff = magnitude - this->threshold; - no need for that variable here
            if (magnitude > this->threshold) { //If sample > threshold
                this->transitionToAttackState();
            }
            //No matter what this sample will just return and the trigger will happen on the next sample
            return in;
        }
    public:
        Compressor() = delete; //Do not allow an empty constructor, they must pass in a sampleRate
        Compressor(int sampleRate) : sampleRate(sampleRate) {
            //Cache function pointers on creation to avoid address reference operation on every sample
            this->pIdleState = &Compressor<T>::state_Idle;
            this->pAttackState = &Compressor<T>::state_Attack;
            this->pHoldState = &Compressor<T>::state_Hold;
            this->pReleaseState = &Compressor<T>::state_Release;

            this->transitionToIdleState();
        }
        ~Compressor() {} //Empty destructor because we do not allocate any heap memory
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

        T processSample(T in) {
            //Calls the current state function depending on what state we are currently in (see private method definitions for more details)
            return CALL_PTR_TO_MEMBER_FUNCTION(*this, this->pCurrentStateFunc)(in);

            float magnitude = ::fabs(in);
            float diff = magnitude - this->threshold;
            if (hardKnee) {
                //TODO:
                if (magnitude > this->threshold) {
                    //Then we want to start/continue attacking
                    if (this->ramp < 1) {
                        this->ramp += this->attackIncrement;
                    }
                    else {
                        //Then we want to either instantiate or continue the hold time
                        if (this->numHoldTimeSamplesLeft > 0) {
                            this->numHoldTimeSamplesLeft--;
                        }
                    }
                }
                else {
                    //Then we want to start/continue hold/releasing
                    //If we are in the middle of holding
                    if (this->ramp <= 0) {
                        //Then we are done compressing, just return the sample
                        return in;
                    }
                    //Else we want to either start the hold or 
                    if (this->numHoldTimeSamplesLeft < 0) {
                        //Then this means we need to set 
                    }
                    if (this->ramp > 0) {
                        this->ramp -= this->releaseDecrement;
                    }
                }
                return in * (1 - (this->ramp * this->ratio * diff));
            } 
            else {
                float gain = 1.f;
                if (magnitude >= this->threshold) { // if samp > thresh... 
                    if (this->ramp >= 1) { // and ramp is max...
                        this->ramp = 1;
                        gain = (1 - diff * this->ratio);
                    } else { // if ramp is not...
                        this->ramp += attackIncrement;
                        gain = (1 - (ramp * diff * this->ratio)); 
                    }
                } else { // if samp < thresh...
                    if (this->ramp <= 0) { // and ramp is min...
                        this->ramp = 0;
                        gain = 1;
                    } else { // if ramp is not... 
                        this->ramp -= releaseDecrement;
                        gain = (1 - (ramp * diff * this->ratio)); 
                    }
                }
              return in * gain;
            }
        }

        void setAttackTime(float attackMillis) {
            if (attackMillis <= 0) {
                attackMillis = 0.000000000000000001f;
                std::cout << "Attack time set to psuedo-zero value, supply a positive float" << std::endl;
            }
            this->attackIncrement = 1.f / millisToSamples(attackMillis, this->sampleRate);
        }

        void setReleaseTime(float releaseMillis) {
            if (releaseMillis <= 0) {
                releaseMillis = 0.000000000000000001f;
                std::cout << "Release time set to psuedo-zero value, supply a positive float" << std::endl;
            }
            this->releaseDecrement = 1.f / millisToSamples(releaseMillis, this->sampleRate);
        }

        void setRatio(float r) {
            this->ratio = r;
        }

        void setThreshold(float t) {
            this->threshold = dBtoA(t);
        }

        void setHoldTime(float holdMillis) {
            this->holdTimeSamples = millisToSamples(holdMillis, this->sampleRate);
        }

        void setHardKnee(bool knee) {
            this->hardKnee = knee;
        }
        
    };
}
#endif