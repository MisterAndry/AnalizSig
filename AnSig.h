#ifndef ANSIGC_H
#define ANSIGC_H
#include <iostream>
#include <stdio.h>
#include <QVector>
#include <QList>
#include <stdlib.h>
#include <iomanip>
#include <QString>
#include <math.h>
using namespace std;
class AnSig
{
public:
    AnSig();
    //Возвращают названия функций и параметров
    static QList<QString> full_func_names();                          //полные названия функций
    static QList<QString> short_func_names();                         //сокращённые названия функций
    static QList<QString> full_params_names(int func_num);            //полные названия параметров
    static QList<QString> short_params_names(int func_num);           //сокращённые названия параметров

    //Возращают значение параметров для однполярного сигнала
    double ansig_get_imp_time(QList<double> vect, double timestep);   //время импульса
    double ansig_get_ampl(QList<double> vect, double timestep);       //амплитуда
    double ansig_get_ovf(QList<double> vect, double timestep);        //выброс
    double ansig_get_front(QList<double> vect, double timestep);      //фронт
    double ansig_get_cut(QList<double> vect, double timestep);        //срез

    //Возращают значение параметров для двуполярного сигнала
    double ansig_bi_get_imp_time(QList<double> vect, double timestep);//время ипульса
    double ansig_bi_get_ampl(QList<double> vect, double timestep);    //амплитуда
    double ansig_bi_get_ovf(QList<double> vect, double timestep);     //выброс
    double ansig_bi_get_front(QList<double> vect, double timestep);   //фронт
    double ansig_bi_get_cut(QList<double> vect, double timestep);     //срез

    //вычисляет и записывает параметры по названию функции
    void paramCalc(QString func_name);

    //Возращает значение параметра по его названию
    double paramValue(QString param_name);

    //Сеттеры
    void setVec(QList<double> vec);         //для измеренного сигнала
    void setInVec(QList<double> in_vec);    //для входного сигнала
    void setTime(double time);              //длительность всего сигнала


private:
    QVector<double> resvec;                 //вектор, в который записваются вычисленные параметры
    QList<double> in_vec;                   //лист с входным сигналом
    QList<double> vec;                      //лист с измеренным сигналом
    QString fname;                          //название функции, параметры которой надо будет вывести
    double time;                            //длительность всего сигнала

    /*ВСПОМОГАТЕЛЬНЫЕ*/
    //гистограмма
    void gist(QList <double> in);
    //возвращает СКЗ
    double skz(QList<double> in, int start, int end);
    //возвращают номера максимального и минимального элементов массива
    int find_max(QList<double> in, int start, int end);
    int find_min(QList<double> in, int start, int end);


    /*ДЛЯ ДВУПОЛЯРНОГО*/
    //возвращает кол-во импульсов
    int imp_cnt(QList<double> in, int start, int end);
    //возвращают номера элементов с макс. и мин. значениями амплитуды
    int find_min_ampl(QList<double> in, int start, int end);
    int find_max_ampl(QList<double> in, int start, int end);
    //возавращает полную ампулитуду
    double find_ampl(QList<double> in, int start, int end);
    //возвращает номер элмента, с которого начинается срез
    int find_start_of_cut(QList<double> in, int start, int end);
    //возвращает номер элмента, на котором заканчивается срез
    int find_end_of_cut(QList<double> in, int start, int end);
    //возвращает номер элмента, с которого начинается фронт
    int find_start_of_front(QList<double> in, int start, int end);
    //возвращает номер элмента, на котором заканчивается фронт
    int find_end_of_front(QList<double> in, int start, int end);
    //возвращает длительность фронта
    double find_front_time(QList<double> in, double *in2, int start, int end);
    //возвращает длительность среза
    double find_cut_time(QList<double> in, double *in2, int start, int end);
    //возаращает значение, соответсвующее середине импульса (для двуполярного)
    double find_mid_imp(QList<double> in,int start, int end);
    //возвращает длительность импульса
    double find_imp_time(QList<double> in, double *in2, int start, int end);
    //возвращает значение выброса
    double find_overflow(QList<double> in, int start, int end);

    /*ДЛЯ ОДНОПОЛЯРНОГО*/
    //возвращает номер элмента, с которого начинается фронт
    int ttl_find_start_of_front(QList<double> in, int start, int end);
    //возвращает номер элмента, на котором заканчивается фронт
    int ttl_find_end_of_front(QList<double> in, int start, int end);
    //возвращает длительность фронта
    double ttl_find_front_time(QList<double> in, double *in2, int start, int end);
    //возвращает номер элмента, на котором заканчивается срез (начало находится функцией для двуполярного)
    int ttl_find_end_of_cut(QList<double> in, int start, int end);
    //возвращает длительность среза
    double ttl_find_cut_time(QList<double> in, double *in2, int start, int end);
    //возвращает номер элемента соответсвующий мин. значению амплитуды
    int ttl_find_min_ampl(QList<double> in, int start, int end);
    //возавращает полную ампулитуду
    double ttl_find_ampl(QList<double> in, int start, int end);
    //возвращает длительность импульса
    double ttl_find_imp_time(QList<double> in, double *in2, int start, int end);
    //шумовая нестабильность
    double noiseInstab(QList<double> in, QList<double> vec, double time);
    //чувствительность
    double Sensitivity(QList<double>);

    /*ДЛЯ СИНУСА*/
    //возвращает амплитуду синусоиды
    double find_sin_ampl(QList<double> in, int start, int end);
    //возвращает период синусоиды
    double find_sin_time(QList<double> in ,double *in2, int start, int end);
    //возвращает середину размаха синусоиды
    double find_mid_sin(QList<double> in, int start, int end);
    //возвращает сдвиг по фазе
    double find_mid_sin(QList<double> in, int SinCos);

    /*ДЛЯ ПОСТОЯННОГО*/
    //возвращает среднее значение всего массива
    double avg_value(QList<double> in);
    //размах пульсаци
    double pulse(QList<double> in);
    //коэффициент пульсаций
    double k_pulse(QList<double> in);
    //выделение переменной составляющей
    static QList<double> alter_comp(QList<double> in);

    //Главные функции
    //записывают в вектор параметр сигнала(время имп, амплитуду, выброс, фронт, срез)
    void res_func_lpi(QList<double> vec, double time);                       //двуполярный сигнал
    void res_func_ttl(QList<double> vec, double time);                       //однополярный
    double res_func_sin(QList<double> vec, QList<double> in, double time);   //синусоидальный

    //Геттеры
    QList<double> getVec();                 //для измеренного сигнала
    QList<double> getInVec();               //для входного сигнала
    double getTime();                       //длительность всего сигнала

};

#endif // ANSIGC_H
