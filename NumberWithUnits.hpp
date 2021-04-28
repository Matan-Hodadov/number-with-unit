#pragma once

#include <string>
#include <unordered_map>

namespace ariel
{
    // typedef unsigned int uint;

    typedef struct unit
    {
        double value;
        std::string type;
    }unit;

    class NumberWithUnits
    {
        private:
        double number;
        std::string unit;

        // void set_value(double value);
        // void set_type(std:: string type);

        static std::unordered_map<std::string, ariel::unit> convert_map;
        bool can_be_converted(std::string t1, std::string t2) const;
        std::string find_smallest_common_unit(std::string t1, std::string t2) const; 
        double mul_ratio (std::string t1, std::string t2) const;
        NumberWithUnits change(std::string t1, std::string t2, double val) const;

        public:
        //getters
        // double getValue() const { return this->number; }
        // std::string getUnit() const { return this->unit; }

        NumberWithUnits(){}
        NumberWithUnits(const NumberWithUnits& other)
        {
            this->number = other.number;
            this->unit = other.unit;
        }
        NumberWithUnits(double value,std::string unit);

        static void read_units(std::ifstream& file_name);
        NumberWithUnits operator + ();
        NumberWithUnits operator - ();
        NumberWithUnits operator + (const NumberWithUnits& other);
        NumberWithUnits operator - (const NumberWithUnits& other);
        NumberWithUnits& operator ++ ();
        NumberWithUnits& operator -- ();
        NumberWithUnits operator ++ (int dummy);
        NumberWithUnits operator -- (int dummy);
        NumberWithUnits operator += (const NumberWithUnits& other);
        NumberWithUnits operator -= (const NumberWithUnits& other);

        NumberWithUnits operator * (double num);
        friend NumberWithUnits operator * (double num, const NumberWithUnits& other);
        
        bool operator < (const NumberWithUnits& other) const;
        bool operator > (const NumberWithUnits& other) const;
        bool operator <= (const NumberWithUnits& other) const;
        bool operator >= (const NumberWithUnits& other) const;
        bool operator == (const NumberWithUnits& other) const;
        bool operator != (const NumberWithUnits& other) const;

        friend std::ostream& operator<< (std::ostream& output, const NumberWithUnits& other);
        friend std::istream& operator>> (std::istream& input , NumberWithUnits& other);
    };
}
