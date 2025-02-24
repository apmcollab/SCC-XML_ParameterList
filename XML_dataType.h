//
// XML_dataType.h 
//
// Author: Chris Anderson  
// (C) UCLA 2012 

/*
XML_dataType is a class which provides a minimal wrapping for a subset
of the basic data types available in C++. The data types supported are
bool, int, long, float, double and strings. The class is a
container class which holds the value of the data and it's type.
The constructors and conversions associated with this class have been
implemented so that functions that return an XML_dataType object can
"communicate" with variables of the standard data types. This class is
purely a mechanism for implementing functions with identical signature
but different return types. Algebraic operations (other than assignment)
are not implemented for the objects of this class.
*/

/*
#############################################################################
#
# Copyright  2012- Chris Anderson
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the Lesser GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# For a copy of the GNU General Public License see
# <http://www.gnu.org/licenses/>.
#
#############################################################################
*/
#ifndef XML_DATA_TYPE_
#define XML_DATA_TYPE_
#include <string>

#include <iostream>
#include <cstdlib>

class  XML_dataType
{

public :

    bool    b;
    int     i;
    long    l;
    float   f;
    double  d;
    std::string  s;
    void*   v;
    int  type_name;

public :

//
//  Type Enumerations
//

    enum {TYPE_NULL, TYPE_BOOL, TYPE_INT ,TYPE_LONG ,TYPE_FLOAT ,TYPE_DOUBLE ,TYPE_STRING ,TYPE_BOOL_PTR, TYPE_INT_PTR ,TYPE_LONG_PTR ,TYPE_FLOAT_PTR ,TYPE_DOUBLE_PTR ,TYPE_STRING_PTR  };
//
//  Constructors
//
    XML_dataType();
    XML_dataType(const XML_dataType& A);

    XML_dataType(bool A );
    XML_dataType(int A );
    XML_dataType(long A );
    XML_dataType(float A );
    XML_dataType(double A );
    XML_dataType(const std::string& A );

    XML_dataType(const char* A);

    //XML_dataType(char* A);
    XML_dataType(int* A );
    XML_dataType(bool* A );
    XML_dataType(long* A );
    XML_dataType(float* A );
    XML_dataType(double* A );
    XML_dataType(std::string* A );
//
//  Destructor
//
    ~XML_dataType();

//
//  Initializers
//
    void initialize();
//
//  Assignment
//
    XML_dataType& operator = ( const XML_dataType& A);
//
//  Output
//
    friend  std::ostream& operator <<(std::ostream& out_stream, const XML_dataType& A);
//
//  conversions
//
	//operator const char*();
	operator int()         const;
	operator bool()        const;
	operator long()        const;
	operator float()       const;
	operator double()      const;
	operator std::string() const;
	friend std::istream&  operator >>(std::istream& in_stream, XML_dataType& A);
//
//  assignments
//
	void  operator =(bool A);
    void  operator =(int A);
    void  operator =(long A);
    void  operator =(float A);
    void  operator =(double A);
    void  operator =(const std::string& A);
    void  operator =(const char* A);

    //void  operator =(const char* A);
//
//  Utility for output
//
    std::string toString() const;

//
//  Typing mutatators
//

    bool   isString() const;
    void   setType(int XML_dataTypeType);
    int    getType() const;
    std::string getTypeAsString() const;

    int isNull(){if(getType() == TYPE_NULL){return 1;} return 0;}
//
//  Error Handling
//
    static void  illegalConversion();
    static void  illegalConversion(int typeA, int typeB);
    static void  nullOperand();
    static void  illegalAssignment();
    static void  illegalAssignment(int typeA, int typeB);

};
#endif

