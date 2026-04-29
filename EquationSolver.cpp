//
// Created by ritokk on 19.03.26.
//

#include "EquationSolver.h"
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;
bool areNearlyEqualToZero(double a, double b) {
    double diff = std::fabs(a - b);
    if (diff <= std::numeric_limits<double>::min()) {
        return true;
    }
    return diff <= std::numeric_limits<double>::epsilon() * std::max(std::fabs(a), std::fabs(b)) * 32.0;
}

EquationSolver::EquationSolver() {}
EquationSolver::~EquationSolver() {}

double EquationSolver::solveDyhotomia(double a, double b, double epsilon, const std::function<double(double)>& f) {
    int max_iterations = 1000;

    double fa = f(a);
    double fb = f(b);

    if (std::isnan(fa) || std::isnan(fb)) {
        cout << "Помилка (Дихотомія): Відрізок знаходиться поза областю визначення функції (ОДЗ)!" << endl;
        return std::numeric_limits<double>::quiet_NaN();
    }
    if (areNearlyEqualToZero(fa, 0.0)) return a;
    if (areNearlyEqualToZero(fb, 0.0)) return b;

    if (fa * fb > 0.0) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double c = a;
    int i = 0;

    while ((b - a) >= epsilon && i <= max_iterations) {
        c = (a + b) / 2.0;
        double fc = f(c);

        if (areNearlyEqualToZero(fc, 0.0)) {
            break;
        }

        if (fc * fa < 0) {
            b = c;
        } else {
            a = c;
            fa = fc; // Оптимізація: не обчислювати f(a) заново
        }

        i++;
    }
    return c;
}

double EquationSolver::solveNewton(double x0, double epsilon, const std::function<double(double)>& f) {
    double x1;
    int max_iterations = 1000;

    // ДОЦІЛЬНЕ ВИКОРИСТАННЯ ЛЯМБДИ:
    // Локальна лямбда для знаходження похідної функції f у точці x
    auto df = [&](double x) {
        double h = 1e-7;
        return (f(x + h) - f(x)) / h;
    };

    for (int i = 0; i < max_iterations; i++) {
        double fx = f(x0);

        if (std::isnan(fx)) {
            cout << "Помилка (Ньютон): Алгоритм вийшов за область визначення функції (ОДЗ)!" << endl;
            return std::numeric_limits<double>::quiet_NaN();
        }
        if (areNearlyEqualToZero(fx, 0.0)) {
            return x0;
        }

        double dfx = df(x0); // Виклик нашої локальної лямбди для похідної

        if (areNearlyEqualToZero(dfx, 0.0)) {
            x0 += 1e-6; // Уникнення ділення на нуль
            continue;
        }

        x1 = x0 - (fx / dfx);

        if (fabs(x1 - x0) < epsilon) {
            return x1;
        }
        x0 = x1;
    }

    cout << "Попередження: Метод Ньютона не зійшовся за " << max_iterations << " ітерацій." << endl;
    return x1;
}

void startingPoint() {
    double a, b, eps;
    cout << "Введіть початок відрізка a: ";
    cin >> a;
    cout << "Введіть кінець відрізка b: ";
    cin >> b;
    cout << "Введіть точність eps (наприклад, 1e-9): ";
    cin >> eps;

    if (eps <= 0.0) {
        cout << "Помилка: Точність має бути строго більшою за нуль." << endl;
        return;
    }
    if (a > b) {
        cout << "Попередження: Початок відрізка більший за кінець. Міняємо їх місцями." << endl;
        std::swap(a, b);
    }

    // ДОЦІЛЬНЕ ВИКОРИСТАННЯ ЛЯМБДИ:
    // Визначаємо нашу цільову функцію
    auto equationFunc = [](double x) {
        return std::sin(std::sqrt(x)) + (0.35 * x) - 3.8;
    };

    EquationSolver solver;

    // Передаємо рівняння (лямбду) в методи
    double resultD = solver.solveDyhotomia(a, b, eps, equationFunc);
    double resultN = solver.solveNewton(10.0, eps, equationFunc);

    cout << fixed << setprecision(15);
    cout << "Досліджуваний проміжок: [" << a << ", " << b << "]" << endl;

    if (std::isnan(resultD)) {
        cout << "Результат Дихотомії: Коренів не виявлено (або знаки на кінцях не різні)." << endl;
    } else {
        cout << "Результат Дихотомії: Знайдено корінь " << resultD << endl;
    }

    cout << "Результат Ньютона: ";
    if (std::isnan(resultN)) {
        cout << "Помилка ОДЗ або розбіжність." << endl;
    } else {
        cout << "Знайдено корінь " << resultN;

        if (resultN < a || resultN > b) {
            cout << " (Зовнішній)" << endl;
        } else {
            cout << " (Внутрішній)" << endl;
        }
    }
}
