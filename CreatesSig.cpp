#include "CreateSig.h"

CreateSig::CreateSig()
{
}

QList<double> CreateSig::createConstSignal(int number_point,double value)
{
    QList<double> sig;
    if (number_point <= 0)
    {
        sig << 0;
    }
    else
    {
        for (int i=0; i<number_point; i++)
            sig << value;
    }
    return sig;
}

QList<double> CreateSig::createImpulseSignal(double time,int number_point,double duration,double low_border,double high_border,double freq)
{
    QList<double> sig;

    //обработка некорректных значений
    if(freq <= 0 || (low_border>high_border) || time <= 0 || number_point <= 0)
    {
        sig << 0;
    }

    //если значения норм
    else
    {
        double T=1/freq;                    //период
        double Tcnt=time/T;                 //кол-во импульсов
        double Tpts=number_point/Tcnt;      //кол-во точек на период
        double imp_points=Tpts*duration;          //кол-во точек на импульс
        double pause_points=Tpts-imp_points;       //кол-во точек на паузу
        bool   imp=false;
        double temp_imp=imp_points;
        double temp_p=pause_points;
        for(int i = 0; i<number_point; i++)
        {
            if(imp)
            {
                sig << high_border;
                temp_imp--;
                if(temp_imp < 1)
                {
                    temp_imp = imp_points;
                    imp = false;
                }
            }
            if(!imp)
            {
                sig << low_border;
                temp_p--;
                if(temp_p < 1)
                {
                    temp_p = pause_points;
                    imp = true;
                }
            }
        }
    }
    return sig;
}


QList<double> CreateSig::createSinSignal(double time, int SinCos, int number_point, double ampl, double freq, double phase)
{
    QList<double> sig;
    //обработка некорректных значений
    if(number_point <= 1 || freq <= 0 || time <= 0)
    {
        sig<<0;
    }
    //если норм
    else
    {
        double discr = time/(number_point-1);
        double t = 0;
        if (SinCos == 1)
        {
            for(int i = 0; i<number_point; i++)
            {
                sig << ampl * sin(freq * t + phase);
                t = t + discr;
            }
        }
        else if(SinCos == 0)
        {
            for(int i = 0; i<number_point; i++)
            {
                sig <<ampl * sin(freq * t + phase + pi/2);
                t = t + discr;
            }
        }
        //если не синус и не косинус, то все значения нули
        else
        {
            for(int i = 0; i<number_point; i++)
                sig << 0;
        }
    }
    return sig;
}
