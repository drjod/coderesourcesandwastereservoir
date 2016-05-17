#ifndef SBX_STREAM_H_
#define SBX_STREAM_H_

#include <fstream>
#include <string>
#include <iostream>

#define CONSOLE std::cout

namespace sbx {

    int verbosity = 1;  // console and file output

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // class: Stream
    // Task:
    //         parent class for input and output stream class


    class Stream
	{
 	 public:
        Stream() {}
        template <typename T> void _open( std::string filename, T& stream )
        {
        	stream.open( filename );
            if (!stream)
            	std::cout << "Error - Could not open stream for " << filename << std::endl;
        }
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Output : public Stream
	{
		std::ofstream outstream;

	public:
		Output(std::string filename) { _open( filename, outstream); }

		~Output() { outstream.close(); }

		std::ofstream& get_stream(void) { return outstream; }

		template <typename T> void write( std::string variableName, T variable, int verbosityLevel = 0 )
		{ writeVariable( variableName , variable, verbosityLevel, outstream ); }
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Input : public Stream
	{
		std::ifstream instream;

		Input(std::string filename) { _open( filename, instream); }

		~Input() { instream.close(); }
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
	// Non-member functions that support also console output (T2& std::cout)

	template <typename T1, typename T2> void writeVariable( std::string info, T1 variable, int verbosityLevel, T2& outstream  )
	{
		if( verbosityLevel < verbosity )
		{
			outstream << info << ": " << std::endl;
			outstream << variable << std::endl << std::endl;
		}
	}

	template <typename T1, typename T2 > void show( std::string info, T1 variable, int verbosityLevel, T2& outstream )
	{ writeVariable( info, variable, verbosityLevel, outstream ); }

	template <typename T2> void inform( std::string info, int verbosityLevel, T2& outstream )
	{
		if( verbosityLevel < verbosity )
			outstream << info << std::endl << std::endl;
	}


}
#endif /* SBX_STREAMS_H_ */
