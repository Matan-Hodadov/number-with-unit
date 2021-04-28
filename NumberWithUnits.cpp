#include <iostream>
#include <fstream>
#include <sstream>  
#include <string>
#include <array>
#include <cstring>
#include <cstdio>
#include <exception>
#include "NumberWithUnits.hpp"
#include <algorithm>

using namespace std;
using namespace ariel;
// using namespace ariel::NumberWithUnits;

namespace ariel
{
    unordered_map<string, unit> NumberWithUnits::convert_map = unordered_map<string, ariel::unit>();

    // void NumberWithUnits::set_value(double value)
    // {
    //     this->number = value;
    // }
    // void NumberWithUnits::set_type(string type)
    // {
    //     this->unit = type;
    // }

    bool NumberWithUnits::can_be_converted(string t1, string t2) const
    {
        if(NumberWithUnits::convert_map.find(t1) == convert_map.end() || NumberWithUnits::convert_map.find(t2) == NumberWithUnits::convert_map.end())
        {
            return false;
        }
        if(t1 == t2)
        {
            return true;
        }

        ariel::unit temp;
        temp = NumberWithUnits::convert_map[t1];
        while(temp.type != t2 && temp.type != NumberWithUnits::convert_map[temp.type].type)
        { 
            temp = NumberWithUnits::convert_map[temp.type];
        }

        if (temp.type == t2)
        {
            return true;
        }
        return false;
    }
    
    string NumberWithUnits::find_smallest_common_unit(string t1, string t2) const
    {
        ariel::unit temp1;
        temp1 = NumberWithUnits::convert_map[t1];
        while(temp1.type != convert_map[temp1.type].type)
        {
            temp1 = NumberWithUnits::convert_map[temp1.type];
        }

        ariel::unit temp2;
        temp2 = NumberWithUnits::convert_map[t2];
        while(temp2.type != NumberWithUnits::convert_map[temp2.type].type)
        {
            temp2 = NumberWithUnits::convert_map[temp2.type];
        }
        if (temp2.type == temp1.type)
        {
            return temp1.type;
        }
        else
            return "";
    }

    double NumberWithUnits::mul_ratio(string t1, string t2) const
    {
        double ratio = 1;
        if(t1 == t2)
        {
            return ratio;
        }
        ariel::unit temp;
        temp = NumberWithUnits::convert_map[t1];
        ratio = ratio * temp.value;
        while(temp.type != t2 && convert_map[temp.type].type != temp.type)
        {
            temp = NumberWithUnits::convert_map[temp.type];
            ratio = ratio * temp.value;
        }
        return ratio;
    }

    NumberWithUnits NumberWithUnits::change(string t1, string t2, double val)const 
    { // from t2 to t1 
        NumberWithUnits ret;
        ret.unit = t1;;

        if (t1 == t2)
        {
            ret.number = val;
            return ret;
        }
        if (this->can_be_converted(t2,t1))
        {
            ret.number = (this->mul_ratio(t2, t1)) * val;
            return ret;
        }
        if (this->can_be_converted(t1,t2))
        {
            ret.number = (1.0/this->mul_ratio(t1, t2)) * val;
            return ret;
        }
        string common_unit = this->find_smallest_common_unit(t1,t2);
        if (common_unit != "")
        {
            ret = this->change(common_unit, t2,val);
            return this->change(t1, common_unit, ret.number);
            // return this->change(t1, common_unit, ret.number);
        }
        // throw logic_error("Units do not match - "+t2+" cannot be converted to "+t1);
        throw logic_error(t2 + " cannot be converted to " + t1);
    }

    NumberWithUnits::NumberWithUnits(double value, string unit)
    {
        if(unit == "")
        {
            throw invalid_argument("unit doesn't exists in unit.txt");
        }
        if(convert_map.find(unit) == convert_map.end())
        {
            throw invalid_argument("this unit doesn't exists in unit.txt");
        }
        this->number = value;
        this->unit = unit;
    }
    
    void NumberWithUnits::read_units(ifstream& file_name)
    {
        while(!file_name.eof())
        {
            string big_unit_type, garbage;
            ariel::unit small_unit;
            file_name >> garbage >> big_unit_type >> garbage >> small_unit.value >> small_unit.type;
            convert_map[big_unit_type] = small_unit;
            if(convert_map.find(small_unit.type) == convert_map.end())
            {
                ariel::unit temp;
                temp.type = small_unit.type;
                temp.value = 1;
                convert_map[temp.type] = temp;
            }
        }
    }

    NumberWithUnits NumberWithUnits::operator + ()
    {
        return *this;
    }
    NumberWithUnits NumberWithUnits::operator - ()
    {
        NumberWithUnits temp = *this;
        temp.number = (-1 * temp.number);
        // temp.number = -1 * temp.number;
        return temp;
    }

    NumberWithUnits NumberWithUnits::operator + (const NumberWithUnits& other)
    {
        NumberWithUnits res;
        if(this->unit == other.unit)
        {
            res.unit = this->unit;
            res.number = (this->number + other.number);
            return res;
        }
        res = change(this->unit, other.unit, other.number); 
        return (*this) + res;
    }
    NumberWithUnits NumberWithUnits::operator - (const NumberWithUnits& other)
    {
        NumberWithUnits res;
        if(this->unit == other.unit)
        {
            res.unit = this->unit;
            res.number = this->number - other.number;
            return res;
        }
        res = change(this->unit, other.unit, other.number); 
        return (*this) - res;
    }

    NumberWithUnits& NumberWithUnits::operator ++ ()
    {
        this->number = this->number + 1;
        return (*this);
    }
    NumberWithUnits& NumberWithUnits::operator -- ()
    {
        this->number = this->number - 1;
        return (*this);
    }

    NumberWithUnits NumberWithUnits::operator ++ (int dummy)
    {
        NumberWithUnits temp = (*this);
        this->number = this->number + 1;
        return temp;
    }
    NumberWithUnits NumberWithUnits::operator -- (int dummy)
    {
        NumberWithUnits temp = (*this);
        this->number = this->number - 1;
        return temp;
    }

    NumberWithUnits NumberWithUnits::operator += (const NumberWithUnits& other)
    {
        NumberWithUnits res;
        if(this->unit == other.unit)
        {
            res.unit = this->unit;
            res.number = (this->number + other.number);
            this->number = res.number;
            return res;
        }
        res = change(this->unit, other.unit, other.number); 
        this->number = this->number + res.number;
        return (*this);
    }
    NumberWithUnits NumberWithUnits::operator -= (const NumberWithUnits& other)
    {
        NumberWithUnits res;
        if(this->unit == other.unit)
        {
            res.unit = this->unit;
            res.number = (this->number - other.number);
            this->number = res.number;
            return res;
        }
        res = change(this->unit, other.unit, other.number); 
        this->number = this->number - res.number;
        return (*this);
    }

    NumberWithUnits NumberWithUnits::operator * (double num)
    {
        NumberWithUnits res;
        res.unit = this->unit;
        res.number = this->number * num;
        return res;
    }
    NumberWithUnits operator * (double num, const NumberWithUnits& other)
    {
        NumberWithUnits res = other;
        res.unit = other.unit;
        res.number = res.number * num;
        return res;
    }

    bool NumberWithUnits::operator < (const NumberWithUnits& other) const
    {
        NumberWithUnits temp = change(this->unit,other.unit,other.number);
        if (this->number < temp.number)
        {
            return true;
        }
        return false;
    }
    bool NumberWithUnits::operator > (const NumberWithUnits& other) const
    {
        NumberWithUnits temp = change(this->unit,other.unit,other.number);
        if (this->number > temp.number)
        {
            return true;
        }
        return false;
    }
    bool NumberWithUnits::operator <= (const NumberWithUnits& other) const
    {
        NumberWithUnits temp = change(this->unit,other.unit,other.number);
        if (this->number <= temp.number)
        {
            return true;
        }
        return false;
    }
    bool NumberWithUnits::operator >= (const NumberWithUnits& other) const
    {
        NumberWithUnits temp = change(this->unit,other.unit,other.number);
        if (this->number >= temp.number)
        {
            return true;
        }
        return false;
    }
    bool NumberWithUnits::operator == (const NumberWithUnits& other) const
    {
        double eps = 0.00001;
        NumberWithUnits temp = change(this->unit,other.unit,other.number);
        if (abs(this->number - temp.number) < eps)
        {
            return true;
        }
        return false;
    }
    bool NumberWithUnits::operator != (const NumberWithUnits& other) const
    {
        if((*this) == other)
        {
            return false;
        }
        return true;
    }

    ostream& operator << (ostream& output, const NumberWithUnits& other)
    {
        output << other.number << "[" << other.unit << "]";
        return output;
    }
    istream& operator >> (istream& input , NumberWithUnits& other)
    {
        string g, s1;
        while(!input.eof())
        {
            input >> s1;
            g+=s1;
            if(s1.at(s1.length()-1) == ']')
                break;
        }
        
        replace(g.begin(),g.end(),'[', ' ');
        replace(g.begin(),g.end(),']', ' ');

        istringstream s = istringstream{g};
        if (!s.eof())
            s >> other.number >> other.unit;
        if(NumberWithUnits::convert_map.find(other.unit) == NumberWithUnits::convert_map.end())
            throw logic_error("unit ["+other.unit+"] doesn't exist in the given file.");
        return input;
    }
}