#include "print.h"
#define DEBUG

void outputData(procinfo pinfo)
{
    std::string formattedVal = format_message(pinfo);
    print_string(formattedVal);
}

void outputData(procinfo pinfo, std::ofstream *outputFile)
{
    *outputFile << format_message(pinfo) << std::endl;
}

// Print message. Prepend with program name.
void print_string(std::string message)
{
    if (message != "")
    {
	printf("%s: %s\n", kpiProg.c_str(), message.c_str());
    }
}

//  Format message for printing, then print it.
std::string format_message (procinfo pinfo)
{
    std::string formattedVal = "";
    formattedVal += pinfo.values[cpu_state] + outputSeparator +
	//pinfo.values[cpu_comm] + "," +
	pinfo.values[cpu_utime] + outputSeparator +
	pinfo.values[cpu_stime] + outputSeparator +
	pinfo.values[cpu_cutime] + outputSeparator +
	pinfo.values[cpu_cstime] + outputSeparator +
	pinfo.values[cpu_priority] + outputSeparator +
	pinfo.values[cpu_vsize] + outputSeparator +
	pinfo.values[cpu_rss] + outputSeparator +
	pinfo.values[cpu_rlim] + outputSeparator +
	pinfo.values[cpu_starttime] + outputSeparator +
	get_cpuLoad(pinfo);
    return formattedVal;
}

std::string get_cpuLoad(procinfo pinfo)
{
    std::ifstream uptimeFile(uptimeFname.c_str());
    int uptime = 0;
    std::string temp = "";
    //uptimeFile >> temp; //ignore the first value...
//#ifdef DEBUG
    //print_string("uptime val1: " + temp);
//#endif
    //temp = "";
    uptimeFile >> temp;
#ifdef DEBUG
    print_string("uptime val2: " + temp);
#endif
    uptime = std::stoi(temp);

    double totalTime = std::stoi(pinfo.values[cpu_utime]) + std::stoi(pinfo.values[cpu_stime]);
#ifdef DEBUG
    print_string("CPU totalTime: " + std::to_string(totalTime));
#endif
    double ctotalTime = totalTime + std::stoi(pinfo.values[cpu_cutime]) + std::stoi(pinfo.values[cpu_cstime]);
#ifdef DEBUG
    print_string("CPU ctotalTime: " + std::to_string(ctotalTime));
#endif

    double totalTime_seconds = uptime - (std::stoi(pinfo.values[cpu_starttime]) / cpu_speed);
#ifdef DEBUG
    print_string("CPU totalTime_seconds: " + std::to_string(totalTime_seconds));
#endif
    double cpuUsage = 100 * ((totalTime / cpu_speed) / totalTime_seconds);
#ifdef DEBUG
    print_string("CPU cpuUsage: " + std::to_string(cpuUsage));
#endif

    temp = std::to_string(totalTime_seconds) + outputSeparator + std::to_string(totalTime) + outputSeparator + std::to_string(cpuUsage) + "%";
    return temp;
}
