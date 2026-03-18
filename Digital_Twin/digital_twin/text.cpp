#include <iostream>
#include "classes/mileagemodel.h"
#include "functions/mileagemodel.cpp"

int main()
{
    MileageModel car(15.0, 1.75, 100.0);

    double slope = 10;

    std::cout << "Flat Cost: $" << car.costFlat() << std::endl;
    std::cout << "Uphill Cost: $" << car.costUphill(slope) << std::endl;
    std::cout << "Downhill Cost: $" << car.costDownhill(slope) << std::endl;

    car.displayScoreBoard(slope);

    return 0;
} 