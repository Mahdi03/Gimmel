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
    processSample(T in) {
      this->y_1 = (in * (1-g)) + (this->y_1 * g);
      return y_1;
    }
  };
}
#endif