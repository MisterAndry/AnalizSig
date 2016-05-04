#include "AnSig.h"
AnSig::AnSig()
{
}

//возвращает среднее значение всего массива
double AnSig::avg_value(QList<double> in)
{
    double avg = 0;
    for (int i = 0; i<in.size(); i++)
    {
        avg = avg + in[i];
    }
    return avg=avg/in.size();
}

//СКЗ
double AnSig::skz(QList<double> in, int start, int end)
{
    double s =0;
    for (int i = start; i<end; i++)
    {
        s = s + pow(in[i], 2);
    }
    return s = sqrt(s/(end-start+1));
}

//гистограмма
void AnSig::gist(QList<double> in)
{
    double mean = avg_value(in);

}

//возвращает номер максимального и минимального элементов массива
int AnSig::find_max(QList<double> in, int start, int end)
{
    double max=0;
    int    num_max=0;
    for (int i = start;   i<end; i++)
    {
        if (in[i] > max)
        {
            max = in[i];
            num_max = i;
        }
    }
    return num_max;
}


int AnSig::find_min(QList<double> in, int start, int end)
{
    double min=0;
    int    num_min=0;
    for(int i = start; i<end; i++)
    {
        if(in[i] < min)
        {
            min = in[i];
            num_min = i;
        }
    }
    return num_min;
}

int AnSig::imp_cnt(QList<double> in, int start, int end)
{
    int    impcount=0;
    bool   b = true;
    double max=in[find_max(in, start, end)];
    int    min=in[find_min(in, start, end)];
    for (int i=start; i<end; i++)
    {
        if (b && in[i] > max*0.9)
        {
            impcount++;
            b = false;
        }
        if (!b && in[i] < min*0.9)
        {
            impcount++;
            b = true;
        }
    }
    return impcount;
}

int AnSig::find_min_ampl(QList<double> in, int start, int end)
{
    int     min_ampl = 0;
    double  min = 0;
    int     count = 0,
            uslFront = start,
            uslSrez = end,
            num_min = 0,
            num_max = 0,
            mincount = 0;
    num_max = find_max(in, start, end);
    num_min = find_min(in, start, end);
    min = in[num_min];
    for (int i = num_max; i<end; i++) //находим условнное начало фронта для отрицательной части импульса
    {
        if(in[i] < 0.2*min)
        {
            uslFront = i-1;
            break;
        }
    }

    for (int i = uslFront+1; i<end; i++)   //находим условное начало среза для отрицательной части импульса
    {
        if(in[i] > 0.2*min)
        {
            uslSrez = i;
            break;
        }
    }

    for (int i = uslFront; i<uslSrez; i++)  //находим миниммальную амплитуду
    {
        count = 0;
        for(int j = uslFront; j<uslSrez; j++)
            if(in[j] <= 0.98*in[i] && in[j] >= 1.02*in[i])
                count++;
        if(mincount < count)
        {
            mincount = count;
            min_ampl = i;
        }
    }
    return min_ampl;
}

int AnSig::find_max_ampl(QList<double> in, int start, int end)
{
    double  max=0;
    int     count=0,
            uslFront=start,
            uslSrez=end,
            num_max=0,
            maxcount=0,
           // temp1=0,
            max_ampl=0;

    num_max = find_max(in, start, end);
    max = in[num_max];
 //  temp1 = find_min(in, start, num_max);
    for (int i = start; i<num_max+1; i++)   //находим условное начало фронта
    {
        if(in[i] > 0.2*max)
        {
            uslFront = i;
            break;
        }
    }

    for (int i = uslFront; i<end; i++) //находим условное начало среза
    {
        if(in[i] < 0.2*max)
        {
            uslSrez = i;
            break;
        }
    }

    for (int i = uslFront; i<uslSrez; i++)  //находим амплитуду
    {
        count = 0;
        for(int j = uslFront; j<uslSrez; j++)
            if(in[j]  >= 0.98*in[i] && in[j] <= 1.02*in[i])
                count++;
        if(maxcount <= count)
        {
            maxcount = count;
            max_ampl = i;
        }
    }
    return max_ampl;
}

double AnSig::find_ampl(QList<double> in, int start, int end)
{

    return (in[find_max_ampl(in, start, end)]-in[find_min_ampl(in, start, end)]);
}

int AnSig::find_start_of_cut(QList<double> in, int start, int end)
{
    int    start_cut = find_max_ampl(in, start, end);
    double ampl = in[start_cut];
    for(int i = start_cut; i<end; i++)
    {
        if(in[i] < 0.9*ampl && in[i-1] > 0.9*ampl)
        {
            start_cut = i-1;
            break;
        }
    }
    return start_cut;
}

int AnSig::find_end_of_cut(QList<double> in, int start, int end)
{
    double ampl = in[find_min_ampl(in, start, end)];
    int    end_cut = ttl_find_end_of_cut(in, start, end);
    {
        for(int i = end_cut; i<end;i++)
            if(in[i] < 0.9*ampl)
            {
                end_cut = i;
                break;
            }
    }
    return end_cut;
}

int AnSig::find_start_of_front(QList<double> in, int start, int end)
{
    int    max = find_max(in,start, end);
    int    min = find_min(in, start, max);
    int    start_front=find_min_ampl(in, start,end);
    double ampl=in[start_front];
    for(int i = min; i<max+1; i++)
    {
        if(in[i] > 0.9*ampl)
        {
            start_front = i-1;   /*!!!!!!Возможно надо будет start_front=i-1;*/
            break;
        }
    }
    return start_front;
}

int AnSig::find_end_of_front(QList<double> in, int start, int end)
{
    int    end_front = 0;
    int    start_front = find_start_of_front(in, start,end);
    double ampl = in[find_max_ampl(in,end_front,end)];
    for(int i = start_front; i<end; i++)
    {
        if(in[i] > 0.9*ampl)
        {
            end_front = i;
            break;
        }
    }
    return end_front;
}

double AnSig::find_front_time(QList<double> in, double *in2, int start, int end)
{
    return in2[find_end_of_front(in, start, end)]-in2[find_start_of_front(in, start, end)];
}

double AnSig::find_cut_time(QList<double> in, double *in2, int start, int end)
{
    return in2[find_end_of_cut(in, start, end)]-in2[find_start_of_cut(in, start, end)];
}

double AnSig::find_mid_imp(QList<double> in, int start, int end)
{
    int    max=find_max_ampl(in, start, end);
    int    min=find_min_ampl(in, start, end);
    return (in[max]+in[min])/2;
}

double AnSig::find_imp_time(QList<double> in, double *in2, int start, int end)
{
    int    start_imp=start, end_imp=end;
    int    a=find_start_of_cut(in, start, end);
    int    b=find_end_of_cut(in, start, end);
    int    c=find_start_of_front(in, start, end);
    int    d=find_end_of_front(in, start, end);
    double mid=find_mid_imp(in, start, end);

    for(int i = a; i<b; i++)
    {
        if(in[i] > mid && in[i+1]<mid)
        {
            start_imp = i;
            break;
        }
    }

    for (int i = c; i<d; i++)
    {
        if(in[i] < mid && in[i+1] > mid)
        {
            end_imp = i;
            break;
        }
    }
    return (in2[start_imp] - in2[end_imp]);
}

double AnSig::find_overflow(QList<double> in, int start, int end)
{
    double overflow = 0;
    double max_ampl = in[find_max_ampl(in, start, end)];
    int    a = find_start_of_cut(in, start, end);

    overflow = in[find_max(in,0 ,a)]-max_ampl;
    if (overflow < 0.05*max_ampl)
        overflow = 0;
    return overflow;
}

int AnSig::ttl_find_start_of_front(QList<double> in, int start, int end)
{
    //int     a = find_max(in,start,end);
    int     start_front = start;
    double  ampl = in[find_max_ampl(in, start,end)];
    for(int i = start; i<end; i++)
    {
        if((in[i+1] > 0.1*ampl) && (in[i]<0.1*ampl) && in[i+2] >= in[i+1])
        {
            start_front = i;   /*!!!!!!Возможно надо будет start_front=i-1;*/
            break;
        }
    }
    return start_front;
}

int AnSig::ttl_find_end_of_front(QList<double> in, int start, int end)
{
    int     start_front = ttl_find_start_of_front(in, start,end);
    int     end_front = start_front;
    int     b = find_max(in, start, end)+1;
    //double  ampl = in[find_max_ampl(in,start_front,end)];
    double  ampl = in[find_max_ampl(in,start,end)];
    for(int i = start_front; i<b; i++)
    {
        if(in[i] > 0.9*ampl)
        {
            end_front = i;
            break;
        }
    }
    return end_front;
}

double AnSig::ttl_find_front_time(QList<double> in, double *in2, int start, int end)
{
    return in2[ttl_find_end_of_front(in, start, end)]-in2[ttl_find_start_of_front(in, start, end)];
}

int AnSig::ttl_find_end_of_cut(QList<double> in, int start, int end)
{
    double ampl = in[find_max_ampl(in, start, end)];
    int    end_cut = 0;
    int    start_cut = find_start_of_cut(in, start, end);
    for(int i = start_cut; i<end; i++)
        if(in[i] < 0.1*ampl && in[i-1] > 0.1*ampl)
        {
            end_cut = i;
            break;
        }
    return end_cut;
}

double AnSig::ttl_find_cut_time(QList<double> in, double *in2, int start, int end)
{
    return in2[ttl_find_end_of_cut(in, start, end)]-in2[find_start_of_cut(in, start, end)];
}

int AnSig::ttl_find_min_ampl(QList<double> in, int start, int end)
{
    int      max = find_max(in, start, end);
    double   max_ampl = in[find_max_ampl(in, start, end)]/2;
    double   half_ampl=max_ampl/2;
    int      a = 0;
    int      count = 0,
             mincount = 0,
             min_ampl = 0;
    for (int i = max; i<end; i++)
    {
        if(in[i] < half_ampl)
        {
            a = i;
            break;
        }
    }

    for (int i = a; i<end; i++)  //находим миниммальную амплитуду
    {
        count = 0;
        if(in[i] <= 0)
            for(int j = a; j<end; j++)
            {
                if(in[j] <= 0.98*in[i] && in[j] >= 1.02*in[i])
                    count++;
            }
        if(in[i] > 0)
            for(int j = a; j<end; j++)
            {
                if(in[j] >= 0.98*in[i] && in[j] <= 1.02*in[i])
                    count++;
            }
        if(mincount < count)
        {
            mincount = count;
            min_ampl = i;
        }
    }
    return min_ampl;

}

double AnSig::ttl_find_ampl(QList<double> in, int start, int end)
{
    return (in[find_max_ampl(in, start, end)]-in[ttl_find_min_ampl(in, start, end)]);
}

double AnSig::ttl_find_imp_time(QList<double> in, double *in2, int start, int end)
{
    return (in2[ttl_find_end_of_cut(in, start, end)]-in2[ttl_find_start_of_front(in, start, end)]);
}

void AnSig::res_func_lpi(QList<double> vec, double time)
{
    int     vec_size = vec.size();  //кол-во точек
    double  period = 0;             //период
    double  b[vec_size];            //массив времени
    bool    temp = true;
    double  imp = 0,
            amp = 0,
            ov = 0,
            fr = 0,
            cut = 0;
    QVector<double> start_mas;
    QVector<double> end_mas;

    //очищаем вектор результатов
    resvec.clear();

    b[0] = 0;
    period = time/(vec_size-1);
    for (int i = 1; i<vec_size; i++)   //заполняем массив времени
    {
        b[i] = b[i-1] + period;
    }
    double min = vec[find_min_ampl(vec,0,vec_size)];
    for(int i = 0; i<vec_size - 1; i++)     //2 массива с началми и концами импульсов
    {
        if (vec[i] < min*0.9 && temp)
        {
            start_mas.push_back(i);
            temp=false;
        }
        if (vec[i] < 0 && vec[i+1] > 0 && !temp)
        {
            end_mas.push_back(i+2);
            temp = true;
        }
    }

    end_mas.pop_front();
    end_mas.pop_front();
    start_mas.pop_front();
    start_mas.pop_back();
    start_mas.pop_back();
    int ns = end_mas.size()-1;
    for(int i = 0; i<ns; i++)
    {
        imp = imp + find_imp_time(vec, b, start_mas[i], end_mas[i]);    //время импульса
        amp = amp + find_ampl(vec, start_mas[i], end_mas[i]);          //амплитуда
        ov  = ov  + find_overflow(vec, start_mas[i], end_mas[i]);      //выброс
        fr  = fr  + find_front_time(vec, b ,start_mas[i], end_mas[i]);  //время фронта
        cut = cut + find_cut_time(vec, b, start_mas[i], end_mas[i]);    //время среза
    }
    resvec.push_back(imp/ns);
    resvec.push_back(amp/ns);
    resvec.push_back(ov/ns);
    resvec.push_back(fr/ns);
    resvec.push_back(cut/ns);
    return;
}

void AnSig::res_func_ttl(QList<double> vec, double time)
{
    double  period = 0;            //период
    int     vec_size = vec.size();  //кол-во точек
    double  b[vec_size];       //массив времени
    bool    temp = true;
    double  imp = 0,
            amp = 0,
            ov = 0,
            fr = 0,
            cut = 0;
    QVector<double> start_mas;
    QVector<double> end_mas;

    //очищаем вектор результатов
        resvec.clear();

    b[0] = 0;
    period = time/(vec_size - 1);
    for (int i = 1; i<vec_size; i++)   //заполняем массив времени
    {
        b[i] = b[i-1] + period;
    }
    double max = (vec[find_max(vec, 0, vec_size)])/2;
    for (int i = 0; i<vec_size - 1; i++)     //2 массива с началми и концами импульсов
    {
        if (vec[i] < 0.2*max && temp)
        {
            start_mas.push_back(i);
            temp = false;
        }
        if (vec[i] < max && vec[i+1] > max && !temp)
        {
            end_mas.push_back(i+1);
            temp = true;
        }
    }
    start_mas.pop_back();
    start_mas.pop_back();
    start_mas.pop_front();
    end_mas.pop_front();
    end_mas.pop_front();
    int ns = end_mas.size();

    for (int i = 0; i<ns; i++)
    {
        imp = imp + ttl_find_imp_time(vec, b, start_mas[i], end_mas[i]);    //время импульса
        amp = amp + ttl_find_ampl(vec, start_mas[i], end_mas[i]);          //амплитуда
        ov  = ov  + find_overflow(vec, start_mas[i], end_mas[i]);          //выброс
        fr  = fr  + ttl_find_front_time(vec, b, start_mas[i], end_mas[i]);  //время фронта
        cut = cut + ttl_find_cut_time(vec, b, start_mas[i], end_mas[i]);    //время среза
    }
    resvec.push_back(imp/ns);
    resvec.push_back(amp/ns);
    resvec.push_back(ov/ns);
    resvec.push_back(fr/ns);
    resvec.push_back(cut/ns);
    return;
}

double AnSig::noiseInstab(QList<double> in, QList<double> vec, double time)
{
    bool    temp = true;
    int     size = in.size();
    double  period = 0;
    double  b[size];
    double  half_ampl = 0;
    int     max=find_max(in, 0, size);
    int     end_imp;
    QVector<double> start_imp;
    QVector<double> end_mas;
    QVector<double> start_mas;
    b[0] = 0;

    period = time/(size - 1);
    for (int i = 1; i<size; i++){   //заполняем массив времени
        b[i] = b[i-1] + period;}

    //находим времена начала импульсов
    for(int i = 1; i<size; i++)
    {
        if(in[i] > max*0.1)
            start_imp.push_back(b[i-1]);
    }

    double hmax = (vec[find_max(vec, 0, size)])/2;
    for (int i = 0; i<size-1; i++)     //2 массива с началми и концами импульсов
    {
        if (vec[i] < 0.2*hmax && temp)
        {
            start_mas.push_back(i);
            temp = false;
        }
        if (vec[i] < hmax && vec[i+1] > hmax && !temp)
        {
            end_mas.push_back(i+1);
            temp = true;
        }
    }

    start_mas.pop_back();
    start_mas.pop_back();
    start_mas.pop_front();
    end_mas.pop_front();
    end_mas.pop_front();

    int     imp_cnt = start_mas.size();
    int     t_imp = 0;
    double  max_imp = 0,
            min_imp = 10000;

    for (int i = 0; i<imp_cnt; i++)
    {
        half_ampl = ttl_find_ampl(vec,start_mas[i],end_mas[i])/2;
        for(int j = start_mas[i]; j<end_mas[i]; j++)
        {
            if(vec[j] >= half_ampl && vec[j+1] <= half_ampl)
                end_imp = j;
        }
        if ((vec[end_imp]-in[start_imp[i]]) < min_imp)
            min_imp = (vec[end_imp]-in[start_imp[i]]);

        if ((vec[end_imp]-in[start_imp[i]]) > max_imp)
            max_imp = (vec[end_imp]-in[start_imp[i]]);

        t_imp = t_imp + (vec[end_imp]-in[start_imp[i]]);
    }
    t_imp = t_imp/imp_cnt;
    return ((max_imp-min_imp)/3);
}

//Возвращает время импульса одноп. сигнала
double AnSig::ansig_get_imp_time(QList<double> vect, double timestep)
{
    int size = vect.size();
    double t = (size - 1)*timestep;
    res_func_ttl(vect, t);
    return resvec.at(0);
}

//Возвращает амплитуду одноп. сигнала
double AnSig::ansig_get_ampl(QList<double> vect, double timestep)
{
    int size = vect.size();
    double t = (size - 1)*timestep;
    res_func_ttl(vect, t);
    return resvec.at(1);
}

//Возвращает переполнение одноп. сигнала
double AnSig::ansig_get_ovf(QList<double> vect, double timestep)
{
    int size = vect.size();
    double t = (size - 1)*timestep;
    res_func_ttl(vect, t);
    return resvec.at(2);
}

//Возвращает фронт одноп. сигнала
double AnSig::ansig_get_front(QList<double> vect, double timestep)
{
    int size = vect.size();
    double t = (size - 1)*timestep;
    res_func_ttl(vect, t);
    return resvec.at(3);
}

//Возвращает срез одноп. сигнала
double AnSig::ansig_get_cut(QList<double> vect, double timestep)
{
    int size = vect.size();
    double t = (size - 1)*timestep;
    res_func_ttl(vect, t);
    return resvec.at(4);
}


//Возвращает время импульса двупол. сигнала
double AnSig::ansig_bi_get_imp_time(QList<double> vect, double timestep)
{
    int size = vect.size();
    double t = (size - 1)*timestep;
    res_func_lpi(vect, t);
    return resvec.at(0);
}

//Возвращает амплитуду двупол. сигнала
double AnSig::ansig_bi_get_ampl(QList<double> vect, double timestep)
{
    int size = vect.size();
    double t = (size - 1)*timestep;
    res_func_lpi(vect, t);
    return resvec.at(1);
}

//Возвращает переполнение двупол. сигнала
double AnSig::ansig_bi_get_ovf(QList<double> vect, double timestep)
{
    int size = vect.size();
    double t = (size - 1)*timestep;
    res_func_lpi(vect, t);
    return resvec.at(2);
}

//Возвращает фронт двупол. сигнала
double AnSig::ansig_bi_get_front(QList<double> vect, double timestep)
{
    int size = vect.size();
    double t = (size - 1)*timestep;
    res_func_lpi(vect, t);
    return resvec.at(3);
}

//Возвращает срез двупол. сигнала
double AnSig::ansig_bi_get_cut(QList<double> vect, double timestep)
{
    int size = vect.size();
    double t = (size - 1)*timestep;
    res_func_lpi(vect, t);
    return resvec.at(4);
}


QList<QString> AnSig::full_func_names()
{
    QList<QString> func_names;
    func_names << "Однополярный импульсный сигнал" <<
                "Двухполярный импульсный сигнал" <<
                "Синусоида" <<
                "Чувствительность сигнала" <<
                "Среднее значение сигнала" <<
                "Шумовая нестабильность" <<
                "Поиск максимума" <<
                "Поиск минимума";
    return func_names;
}

QList<QString> AnSig::short_func_names()
{
    QList<QString> func_names;
    func_names << "uni_imp" <<
                "bi_imp" <<
                "sin" <<
                "sens" <<
                "avg" <<
                "noise"
                "max" <<
                "min";
    return func_names;
}

QList<QString> AnSig::full_params_names(int func_num)
{
    QList<QString> params;
    switch(func_num)
    {
    case 0:
        params << "Время импульса" <<
                "Амплитуда импульса" <<
                "Перерегулирование (выброс)" <<
                "Время фронта" <<
                "Время среза";
        break;
    case 1:
        params << "Время импульса" <<
                "Амплитуда импульса" <<
                "Перерегулирование (выброс)" <<
                "Время фронта" <<
                "Время среза";
        break;
    case 2:
        params << "Амплитуда синусоиды" <<
                "Периода" <<
                "Сдвиг по фазе";
        break;
    case 3:
        params << "Чувствительность";
        break;
    case 4:
        params << "Среднее значение сигнала";
        break;
    case 5:
        params << "Шумовая нестабильность";
        break;
    case 6:
        params << "Максимальное значение";
        break;
    case 7:
        params << "Минимальное значение";
        break;
    default:
        params << " ";
        break;
    }
    return params;
}

QList<QString> AnSig::short_params_names(int func_num)
{
    QList<QString> params;
    switch(func_num)
    {
    case 0:
        params << "t_imp" <<
                "ampl" <<
                "ovf" <<
                "front" <<
                "cut";
        break;
    case 1:
        params << "t_imp" <<
                "ampl" <<
                "ovf" <<
                "front" <<
                "cut";
        break;
    case 2:
        params << "ampl" <<
                "period" <<
                "phase";
        break;
    case 3:
        params << "sens";
        break;
    case 4:
        params << "avg";
        break;
    case 5:
        params << "noise";
        break;
    case 6:
        params << "max";
        break;
    case 7:
        params << "min";
        break;
    default:
        params << " ";
        break;
    }
    return params;
}


void AnSig::paramCalc(QString func_name)
{
    QList<double> vec = getVec();
    QList<double> vec2 = getInVec();
    double time = getTime();
    if (func_name == "bi_imp")
    {
        res_func_lpi(vec, time);
        fname = "bi_imp";
    }
    else if (func_name == "uni_imp")
    {
        res_func_ttl(vec, time);
        fname = "uni_imp";
    }
    else if (func_name == "avg")
    {
        resvec.push_back(avg_value(vec));
        fname = "avg";
    }
    else if (func_name == "noise")
    {
        resvec.push_back(noiseInstab(vec2, vec, time));
        fname = "noise";
    }
    else if (func_name == "max")
    {
        resvec.push_back(vec[find_max(vec, 0, vec.size())]);
        fname = "max";
    }
    else if (func_name == "min")
    {
        resvec.push_back(vec[find_min(vec, 0, vec.size())]);
        fname = "min";
    }
    else
    {
        for (int i = 0; i<resvec.size(); i++)
            resvec.remove(i);
    }
    return;
}

double AnSig::paramValue(QString param_name)
{
    double value = 0;
    if(fname == "uni_imp" || fname == "bi_imp")
    {
        if (param_name == "t_imp")
            value = resvec.at(0);
        else if (param_name == "ampl")
            value = resvec.at(1);
        else if (param_name == "ovf")
            value = resvec.at(2);
        else if (param_name == "front")
            value = resvec.at(3);
        else if (param_name == "cut")
            value = resvec.at(4);
    }

    else if (fname == "avg" || fname == "max" || fname == "min" || fname == "noise")
        value = resvec.at(0);
    return value;
}

void AnSig::setVec(QList<double> vec){
    this->vec = vec;}

void AnSig::setInVec(QList<double> in_vec){
    this->in_vec = in_vec;}

void AnSig::setTime(double time){
    this->time = time;}

double AnSig::getTime(){
    return time;}

QList<double> AnSig::getVec(){
    return vec;}

QList<double> AnSig::getInVec(){
    return in_vec;}
