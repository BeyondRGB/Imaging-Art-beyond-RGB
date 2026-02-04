#ifndef WHITE_PTS_H
#define WHITE_PTS_H

#include "illuminants.hpp"
#include "standard_observer.hpp"

#define VALUE_COUNT 3
#define OBSERVER_COUNT 2
#define ILLUMINANT_COUNT 3

typedef StandardObserver::ObserverType ObserverType;
typedef Illuminants::IlluminantType IlluminantType;

class WhitePoints {

  public:
    enum ValueType { Xn, Yn, Zn };

    WhitePoints(ObserverType observer, IlluminantType illuminant);
    ~WhitePoints() {};
    double get_white_point(ValueType type);

  private:
    ObserverType observer;
    IlluminantType illuminant;
    static double values[VALUE_COUNT][ILLUMINANT_COUNT][OBSERVER_COUNT];
};

#endif // !WHITE_PTS_H
