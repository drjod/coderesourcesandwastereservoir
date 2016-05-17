#ifndef SBX_INTEGRATION_H
#define SBX_INTEGRATION_H

#include <vector>
#include <iostream>

#include "sbx_vector.h"

namespace sbx {

	////////////////////////////////////////////////////////////////////////////////////
	//
	// sbx class slice
	//     Gauss integration over slice or vector of slices (integration order up to 5)
	//
	//  Example code:
	//      Slice a(0., 1., 5); // one slice over [0,1], integration order is 5
	//      Vector<Slice> vec, vec1, vec2;
	//      stratify(0., 1., 10, vec1);  // vec1 covers [0,1] with 10 slices, integration order is 1 by default
	//      stratify(1., 2., 20, vec2);  // vec2 covers [1,2] with 20 slices
	//      vec = cat(vec1, vec2);          // concatenation - vec covers [0,2] with 30 slices
	//      std::cout << a.integrate(quadratic) << " " << integrate(vec, quadratic) << std::endl;
	//

	class Slice{

		double lowerBound, upperBound;
		int order;
		double gaussPoints[5], gaussWeights[5];

	public:
		// constructor
		Slice() {}
		Slice(int _order) { order = _order, setGaussData(); }
		
		void initialize(double _lowerBound, double _upperBound, int _order)
		{
			order = _order, setGaussData();
			lowerBound = _lowerBound, upperBound = _upperBound;
		}

		void initialize(double _lowerBound, double _upperBound)  // order set with constructor
		    { lowerBound = _lowerBound, upperBound = _upperBound; }
		void setGaussData();

		double integrate(double(*func)(double));
	

	}; // class Slice

	////////////////////////////////////////////////////////////////////////////////////
	//
	// set gauss points and weights
	//     requires integration order
	//     called in constructor
	//

	void Slice::setGaussData()
	{
		switch (order)
		{
		case 1:
			gaussPoints[0] = 0.5;
			gaussWeights[0] = 1.;
			break;
		case 2:
			gaussPoints[0] = 0.211324865405187;
			gaussWeights[0] = 0.5;
		{
			switch (order)
			{
			    case 1:
				    gaussPoints[0] = 0.5;
				    gaussWeights[0] = 1.;
				    break;
				case 2:
					gaussPoints[0] = 0.211324865405187;
					gaussWeights[0] = 0.5;
					gaussPoints[1] = 0.788675134594813;
					gaussWeights[1] = 0.5;
					break; 
				case 3:
					gaussPoints[0] = 0.1127016653792580;
					gaussWeights[0] = 0.2777777777777778;
					gaussPoints[1] = 0.5;
					gaussWeights[1] = 0.4444444444444444;
					gaussPoints[2] = 0.8872983346207420;
					gaussWeights[2] = 0.2777777777777778;
					break;
				case 4:
					gaussPoints[0] = 0.0694318442029737;
					gaussWeights[0] = 0.173927422568727;
					gaussPoints[1] = 0.330009478207572;
					gaussWeights[1] = 0.326072577431273;
					gaussPoints[2] = 0.669990521792428;
					gaussWeights[2] = 0.326072577431273;
					gaussPoints[3] = 0.930568155797026;
					gaussWeights[3] = 0.173927422568727;
					break;
				case 5:
					gaussPoints[0] = 0.046910077030668;
					gaussWeights[0] = 0.118463442528095;
					gaussPoints[1] = 0.230765344947158;
					gaussWeights[1] = 0.239314335249683;
					gaussPoints[2] = 0.5;
					gaussWeights[2] = 0.284444444444444;
					gaussPoints[3] = 0.769234655052841;
					gaussWeights[3] = 0.239314335249683;
					gaussPoints[4] = 0.953089922969332;
					gaussWeights[4] = 0.118463442528095;
					break;
			default: 
				std::cerr << "ERROR - order " << order << " not supported";
			}

		}
			gaussPoints[1] = 0.788675134594813;
			gaussWeights[1] = 0.5;
			break;
		case 3:
			gaussPoints[0] = 0.1127016653792580;
			gaussWeights[0] = 0.2777777777777778;
			gaussPoints[1] = 0.5;
			gaussWeights[1] = 0.4444444444444444;
			gaussPoints[2] = 0.8872983346207420;
			gaussWeights[2] = 0.2777777777777778;
			break;
		case 4:
			gaussPoints[0] = 0.0694318442029737;
			gaussWeights[0] = 0.173927422568727;
			gaussPoints[1] = 0.330009478207572;
			gaussWeights[1] = 0.326072577431273;
			gaussPoints[2] = 0.669990521792428;
			gaussWeights[2] = 0.326072577431273;
			gaussPoints[3] = 0.930568155797026;
			gaussWeights[3] = 0.173927422568727;
			break;
		case 5:
			gaussPoints[0] = 0.046910077030668;
			gaussWeights[0] = 0.118463442528095;
			gaussPoints[1] = 0.230765344947158;
			gaussWeights[1] = 0.239314335249683;
			gaussPoints[2] = 0.5;
			gaussWeights[2] = 0.284444444444444;
			gaussPoints[3] = 0.769234655052841;
			gaussWeights[3] = 0.239314335249683;
			gaussPoints[4] = 0.953089922969332;
			gaussWeights[4] = 0.118463442528095;
			break;
		default:
			std::cerr << "ERROR - order " << order << " not supported";
		}

	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// local integration over function func
	// Requires
	//    support: lowerBound, upperBound
	//    Gauss: order, points, weights (set with constructor)
	//

	double Slice::integrate(double(*func)(double))
	{
		double value = 0.;
		for (int i = 0; i < order; i++)
		{
			value += gaussWeights[i] * func(lowerBound + gaussPoints[i] * (upperBound - lowerBound));
		}

		value *= upperBound - lowerBound;
		return value;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// global integration of function func 


	//
	//     calls member function integrate and sums up results over vector vec of slices 	
	//
    
	//
	//         Vector
	//

	double integrate(Vector<Slice> &vec, double(*func)(double))
	{
		double value = 0;
                
		for (Vector<Slice>::iterator itr = vec.begin(); itr != vec.end(); itr++)
			value += itr->integrate(func);

		return value;
	}

	//
	//         array
	//

	double integrate(Slice* array, int arraySize, double(*func)(double))
	{
		double value = 0;

		for (int i = 0; i < arraySize; i++)
			value += array[i].integrate(func);

		return value;
	}

	//
	//     calls member function integrate and sums up results over for loop
	//

	double integrate(double lowerBound, double upperBound, long numberOfElements, double(*func)(double))
	{
		double value = 0;
		double delta = (upperBound - lowerBound) / numberOfElements;
		double runningLowerBound = lowerBound + delta / 2;

		for (long i = 0; i < numberOfElements; i++)
		{
			value += func(runningLowerBound);
			runningLowerBound += delta;
		}
		value *= delta;


		return value;
	}

	//
	//     quadratic function hardcoded - sums up results over for loop
	//

	double integrateQuadratic(double lowerBound, double upperBound, long numberOfElements)
	{
		double value = 0;
		double delta = (upperBound - lowerBound) / numberOfElements;
		double runningLowerBound = lowerBound + delta / 2;

		for (long i = 0; i < numberOfElements; i++)
		{
			value += runningLowerBound * runningLowerBound;
			runningLowerBound += delta;
		}
		value *= delta;


		return value;
	}

	////////////////////////////////////////////////////////////////////////////////////
	//
	// Set member variables lower and upper bound in slices (Vector<Slice>)

	void initialize(double lowerBound, double upperBound, long numberOfEntries, Vector<Slice> &vec)
	{
		double delta = (upperBound - lowerBound) / numberOfEntries;
		double runningLowerBound = lowerBound;
        
		for (Vector<Slice>::iterator itr = vec.begin(); itr != vec.end(); itr++)
		{
			itr->initialize(runningLowerBound, runningLowerBound + delta);
			runningLowerBound += delta;
		}
	}

	//
	// Set member variables lower and upper bound, order, Gauss data in slices (Slice* array)
	//

	void initialize(double lowerBound, double upperBound, long numberOfEntries, int order, Slice* array)
	{
		double delta = (upperBound - lowerBound) / numberOfEntries;
		double runningLowerBound = lowerBound;

		for (long i = 0; i < numberOfEntries; i++)
		{
			array[i].initialize(runningLowerBound, runningLowerBound + delta, order);
			runningLowerBound += delta;
		}
	}

	

	////////////////////////////////////////////////////////////////////////////////////
	//
	// functions

	inline double linear(double x)
	{
		return x;
	}

	inline double quadratic(double x)
	{
		return x * x;
	}

}  // namespace sbx


#endif
