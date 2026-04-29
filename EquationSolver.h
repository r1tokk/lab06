//
// Created by ritokk on 19.03.26.
//
#ifndef LAB3_EQUATIONSOLVER_H
#define LAB3_EQUATIONSOLVER_H
#include <functional>
bool areNearlyEqualToZero(double a, double b);
class EquationSolver {
public:
    EquationSolver();
    ~EquationSolver();

    double solveDyhotomia(double a, double b, double epsilon, const std::function<double(double)>& f);
    double solveNewton(double x0, double epsilon, const std::function<double(double)>& f);
};

void startingPoint();
#endif //LAB3_EQUATIONSOLVER_H