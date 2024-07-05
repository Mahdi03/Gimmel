#ifndef GIML_FILTER_HPP
#define GIML_FILTER_HPP
namespace giml {
  // Simple One-Pole Filter
  template <typename T>
  class onePole {
  protected:
    T g = 0;
    T y_1 = 0; 

  public:
    //TO-DO: Constructors

    // loPass config: y_0 = (x_0 * (1-g)) + (y_1 * g)
    T lpf(T in) {
      this->y_1 = giml::linMix(in, y_1, g);
      return y_1;
    }

    void setG(T gVal) {
      if (gVal >= 0 && gVal <= 1) {
        this->g = gVal;
      } else {
        std::cout << "Invalid gVal. Limited to [0-1]" << std::endl;
      }
    }
  };
}
#endif
