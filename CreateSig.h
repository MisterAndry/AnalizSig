#ifndef CREATESIG_H
#define CREATESIG_H
#define pi 3.1415926535
#include <iostream>
#include <stdio.h>
#include <QVector>
#include <QList>
#include <stdlib.h>
#include <iomanip>
#include <QString>
#include <cmath>
#include <cstdlib>


using namespace std;
class CreateSig
{
public:
    CreateSig();
    //возвращает лист с постоянным сигналом
    static QList<double> createConstSignal(int number_point, double value);
    //возвращает лист с импульсным сигналом
    static QList<double> createImpulseSignal(double time, int number_point, double duration, double low_border, double high_border, double freq);
    //возвращает лист с синусоидальным сигналом
    //для синуса SinCos=1, для косинуса SinCos=0. Фаза задаётся в pi (pi, pi/2, 2*pi и т.д.)
    static QList<double> createSinSignal (double time, int SinCos, int number_point, double ampl, double freq, double phase);
};

#endif // CREATESIG_H
