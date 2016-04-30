#ifndef SBX_VECTOR_H
#define SBX_VECTOR_H

#include <vector>
#include <iostream>

namespace sbx {


	////////////////////////////////////////////////////////////////////////////////////
	//
	// sbx class Vector
	//
	// addition       c = a + b         c = operation+(a)
	// subtraction    c = a - b         c = operation-(a) 
	// inner product  c = (a,b)         c = inner_product(a,b)
	// concatenation                    c = cat(a,b)
	// 1-norm        |a|_1=|a_1|_1 +  |a_2|_1 + ... + |a_n|_1 
	//                                  |a|_1 = norm1(a)
    //     overloaded to sum up integration results norm1(a, func)
	//                                  stratify(lowerBound, upperBound, numberOfEntries, vec)
	// output stream                    std::cout << a << std::endl

	
	template <typename T>
	class Vector : public std::vector<T>
	{
	public:
		// constructors
		Vector() : std::vector<T>(){};
		Vector(int numberOfEntries, T value) : std::vector<T>(numberOfEntries, value){};

		////////////////////////////////////////////////////////////////////////////////////
		//
        // addition (deep) 
		//     c = a + b : c = operator+( a, b ) 
		//

		friend Vector operator+(const Vector &a, const Vector &b)
		{
			Vector c(0,0);

			if (a.size() == b.size())
			{
				for (int i = 0; i < a.size(); i++)
					c.push_back(a[i] + b[i]);
			}
			else
				std::cerr << " ERROR - Vector sizes do not match";
			
			return c;
		}

		////////////////////////////////////////////////////////////////////////////////////
		//
		// subtraction (deep) 
		//     c = a + b : c = operator+( a, b ) 
		//
		friend Vector operator-(const Vector &a, const Vector &b)
		{
			Vector c(0, 0);

			if (a.size() == b.size())
			{
				for (int i = 0; i < a.size(); i++)
					c.push_back(a[i] - b[i]);
			}
			else
				std::cerr << " ERROR - Vector sizes do not match";
				
			return c;
		}

		////////////////////////////////////////////////////////////////////////////////////
		//
		// inner product 
		//     c = (a,b)
		//

		friend double inner_product(const Vector &a, const Vector &b)
		{
			double value = 0;

			if (a.size() == b.size())
			{
				for (int i = 0; i < a.size(); i++)
					value += a[i] * b[i];
			}
			else
				std::cerr << " ERROR - Vector sizes do not match";
			
			return value;
		}

		////////////////////////////////////////////////////////////////////////////////////
		//
		// concatenation
		//     c = (a_1, ... a_na, b_1, ..., b_nb)
		//

		friend Vector cat(Vector a, Vector b)
		{
			for (int i = 0; i < b.size(); i++)
				a.push_back(b[i]);

			return a;

		}

		////////////////////////////////////////////////////////////////////////////////////
		//
		// 1-norm  
		//         |a|_1 = |a_1| +  |a_2| +  |a_3| + ... 	
		//

		friend double norm1(const Vector &a, double(*func)(double))
		{
			double value = 0;

			for (int i = 0; i < a.size(); i++)
				value += fabs(a[i]);

			return value;
		}

		////////////////////////////////////////////////////////////////////////////////////
		//
		// output stream
		//

		friend std::ostream& operator<<(std::ostream& stream, Vector a)
		{
			stream << "[";
			for (int i = 0; i < a.size(); i++)
				stream << " " << a[i];

			stream << " ]";
			return stream;
		}
	};

}  // namespace sbx

#endif
