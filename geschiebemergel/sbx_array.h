/*
 * sbx_array.h
 *
 *  Created on: 07.05.2016
 *      Author: delfs
 */

#ifndef SBX_ARRAY_H_
#define SBX_ARRAY_H_

#include <iostream>
#include <stdlib.h>
namespace sbx {

	////////////////////////////////////////////////////////////////////////////////////
	//
	// sbx class Array
	//
	// addition       c = a + b         c = operation+(a)
	// subtraction    c = a - b         c = operation-(a)
	// inner product  c = (a,b)         c = inner_product(a,b)
	// concatenation                    c = cat(a,b)
	// 1-norm        |a|_1=|a_1|_1 +  |a_2|_1 + ... + |a_n|_1
	//                                  |a|_1 = norm1(a)
	// output stream                    std::cout << a << std::endl
    //

	template <typename T>
	class Array
	{
          T* array;
          long length;

	public:
		// constructors
		//Array(){};
		Array(long _length)
        { array = (T*)malloc(_length*sizeof(T)); length = _length; }

		Array(long _length, T value) // with initialization
        {
            array = (T*)malloc(_length*sizeof(T));
            for(long i = 0; i < _length; i++ )
                *(array + i) = value;
            length = _length;
        }

        void setValue(long ndx, T value)
        { *(array + ndx) = value;}

        void increaseValue(long ndx, T value)
        { *(array + ndx) = *(array + ndx)+ value;}

        T getValue(long ndx)
        { return *(array + ndx);}

        long getLength(void)
        { return length;}

        T operator[](long ndx)
        {
            if (ndx < length)
                return *(array + ndx);
            else
            {
                std::cerr << "ERROR - Segmentation fault" << std::endl;
                return 0;
            }
        }




        /*
           T& operator=(T& other)
        {
            if(this != &other)
            {
                    delete array;
                    T* array;
                  array = (T*)malloc(length*sizeof(T));

             }

            return *this;
        }
        */
        ////////////////////////////////////////////////////////////////////////////////////
		//
        // multiplication (deep)
		//     c = a * b : c = operator*( a, b )
		//

		Array operator*(Array b)
		{
              std::cerr << " HI    ";
           Array c(2);
			//if (length == b.getLength())
			{
                //for (long i = 0; i < 2; i++)
                {
                    T value = 0;

			  	        //value = b;

			        c.increaseValue(0, *(array+0) * value);
                }
			}
			//else
			//	std::cerr << " ERROR - Array sizes do not match" << std::endl;

			return c;



		}

		////////////////////////////////////////////////////////////////////////////////////
		//
        // addition (deep)
		//     c = a + b : c = operator+( a, b )
		//

		Array operator+(Array b)
		{
            Array c(length);
			if (length == b.getLength())
			{
				for (long i = 0; i < length; i++)
				    c.setValue(i, *(array+i) + b.getValue(i));
			}
			else
				std::cerr << "+, ERROR - Array sizes do not match" << std::endl;

			return c;
		}

		////////////////////////////////////////////////////////////////////////////////////
		//
        // subtraction (deep)
		//     c = a - b : c = operator-( a, b )
		//

		Array operator-(Array b)
		{
            Array c(length);
			if (length == b.getLength())
			{
				for (long i = 0; i < length; i++)
				    c.setValue(i, *(array+i) - b.getValue(i));
			}
			else
				std::cerr << "-, ERROR - Array sizes do not match" << std::endl;

			return c;
		}

        ////////////////////////////////////////////////////////////////////////////////////
		//
		// inner product
		//     c = (a,b)
		//

		double inner_product(Array b)
		{
			double value = 0;

			if (length == b.getLength())
			{
				for (long i = 0; i < length; i++)
					value += *(array+i) * b.getValue(i);
			}
			else
				std::cerr << "Inner, ERROR - Vector sizes do not match" << std::endl;

			return value;
		}

		////////////////////////////////////////////////////////////////////////////////////
		//
		// concatenation
		//     c = (a_1, ... a_na, b_1, ..., b_nb)
		//

		Array cat(Array b)
		{
            Array newArray( length + b.getLength());

			for (long i = 0; i < length; i++)
			    newArray.setValue(i, *(array+i));

             for (long i = 0; i <  b.getLength(); i++)
                 newArray.setValue(i + length, b.getValue(i));

			return newArray;
		}

		////////////////////////////////////////////////////////////////////////////////////
		//
		// 1-norm
		//         |a|_1 = |a_1| +  |a_2| +  |a_3| + ...
		//

		double norm1()
		{
			double value = 0;

			for (long i = 0; i < length; i++)
				value += fabs(*(array + i));

			return value;
		}

		////////////////////////////////////////////////////////////////////////////////////
		//
		// output stream
		//

		friend std::ostream& operator<<(std::ostream& stream, Array array)
		{
			stream << "[";
			for (long i = 0; i < array.getLength(); i++)
				stream << " " << array.getValue(i);

			stream << " ]";
			return stream;
		}

	};

}  // namespace sbx



#endif /* SBX_ARRAY_H_ */
