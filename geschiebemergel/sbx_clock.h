#ifndef SBX_CLOCK_H_
#define SBX_CLOCK_H_

#include <chrono>

namespace sbx
{

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// class: Clock
	// Task:
	//         Measure run time - writes result in file or on console
    //         (Use: Declare, start, measure)


    class Clock
	{
		std::chrono::steady_clock::time_point _start;
		std::string name;


    public:
		Clock( std::string _name ) : name( _name ) { _start = std::chrono::steady_clock::now(); }
		Clock( void ) { _start = std::chrono::steady_clock::now(); }


		void start( std::string _name ) { name = _name; _start = std::chrono::steady_clock::now(); }
		void start( void ) { _start = std::chrono::steady_clock::now(); }

		template <typename T> void measure(T& outstream)
		{
			std::chrono::steady_clock::duration duration = std::chrono::steady_clock::now() - _start;
			outstream << name << " - Time elapsed (milliseconds): " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << std::endl ;
		}
	};
}

#endif /* SBX_TIME_H_ */
