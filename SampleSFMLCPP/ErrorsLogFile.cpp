#include "ErrorsLogFile.h"


void AddElementToErrorsLogFile(std::string _message)
{
	std::ofstream errorsLogFile("Data/Configurations/Errors.log", std::ios::app | std::ios::ate);
	std::string finalMessage;
	char timeString[255] = { RESET };


	if (!errorsLogFile.is_open())
	{
		std::cout << "Error accessing Errors.log file" << std::endl;

		exit(EXIT_FAILURE);
	}

	// Put the cursor at the end as a precaution
	errorsLogFile.eof();

	// Get the current date based on current system
	time_t now = time(0);
	
	// Transforming the time stamp into the formated time string with : [DAYS-MONTHS-YEARS] HOURS:MINUTES:SECONDS
	strftime(timeString, sizeof(timeString), "[%d-%m-%Y] - %X", localtime(&now));

	finalMessage.append(timeString);
	finalMessage.append("   |   ");
	finalMessage.append(_message);

	// Print the message at the end of the errors log file
	errorsLogFile << finalMessage << std::endl;

	errorsLogFile.close();
}