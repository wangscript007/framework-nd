#include "ProcessorSensor.h"
#include "TelnetCmdManager.h"

#include <sstream>

using namespace Net::Protocol;
//-----------------------------------------------------------------------------

ProcessorSensor::ProcessorSensor()
{
}

//-----------------------------------------------------------------------------

ProcessorSensor::~ProcessorSensor()
{

}

//-----------------------------------------------------------------------------

void ProcessorSensor::handle(TelnetCmdManager* theManager, CmdArgsList& theArgs)
{
    ProcessorSensorData* data = (ProcessorSensorData*)theManager->takeOverInputHandler(this);
    data->telnetManagerM = theManager;
    data->statCountM = 0;
    data->intervalM = 3;
    data->timeoutEvtM = NULL;
    data->sensorM = this;
    stat(data);
}

//-----------------------------------------------------------------------------

void ProcessorSensor::handle(
        TelnetCmdManager* theManager, 
        CmdArgsList& theArgs, 
        void* theSessionData)
{
    ProcessorSensorData* data = (ProcessorSensorData*) theSessionData;
    if (data->timeoutEvtM)
    {
        theManager->cancelLocalTimer(data->timeoutEvtM);
        data->timeoutEvtM = NULL;
    }
    data->telnetManagerM = NULL;
    data->statCountM = 0;
    data->intervalM = 3;
    theManager->exitCurCmd();
}

//-----------------------------------------------------------------------------

void ProcessorSensor::stat(ProcessorSensorData* theData)
{
    assert(theData != NULL);
	std::ostringstream oss;
	oss << std::setw(15) << "ProcessorName" << std::setw(10) << "QueueSize" << std::endl;
	oss << std::setw(15) << "Man" << std::setw(10) << 1 << std::endl;
	{
		boost::shared_lock<boost::shared_mutex> lock(processorMapMutexM);
		ProcessorMap::iterator it = processorMapM.begin();
		for (; it != processorMapM.end(); it++)
		{

		}
	}
	std::string str = oss.str();
	theData->telnetManagerM->send(str.c_str(), str.length());

	theData->statCountM++;
	addTimer(theData);
}

//-----------------------------------------------------------------------------

static void onProcessorSensorTimeOut(int theFd, short theEvt, void *theArg)
{
    ProcessorSensorData* data = (ProcessorSensorData*)theArg;
	data->timeoutEvtM = NULL;
    data->sensorM->stat(data);
}

void ProcessorSensor::addTimer(ProcessorSensorData* theData)
{
	if (theData->timeoutEvtM)
	{
		theData->telnetManagerM->cancelLocalTimer(theData->timeoutEvtM);
		theData->timeoutEvtM = NULL;
	}
    struct timeval tv;
    tv.tv_sec = theData->intervalM;
    tv.tv_usec = 0;
    theData->timeoutEvtM = theData->telnetManagerM->addLocalTimer(tv, onProcessorSensorTimeOut, theData);
}

//-----------------------------------------------------------------------------

void ProcessorSensor::registProcessor(
		const std::string& theName, 
		Processor::BoostProcessor* theProcessor)
{
	boost::unique_lock<boost::shared_mutex> lock(processorMapMutexM);
	processorMapM[theName] = theProcessor;
}

//-----------------------------------------------------------------------------

