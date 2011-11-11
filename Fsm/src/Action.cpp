#include "Action.h"
#include "Session.h"
#include "State.h"
#include "Log.h"

#include <string>

using namespace Fsm;
//-----------------------------------------------------------------------------

int Fsm::changeState(
        Fsm::Session* theSession,
        const int theNextStateId)
{
    //exec EXIT FUNC
    {
        State& curState = theSession->getCurState();

        DEBUG(theSession->getSessionName() 
                << "[" << theSession->getSessionId() << "] handleEvent("
                << theSession->getEventName(EXIT_EVT) << ")");
        ActionList& actionList = curState.getActionList(EXIT_EVT);
        ActionList::iterator it = actionList.begin();
        for (; it != actionList.end(); it++)
        {
            (*it)(theSession);
        }
    }

    //exec ENTRY_EVT
    {
        State& nextState = theSession->toNextState(theNextStateId);

        DEBUG(theSession->getSessionName() 
                << "[" << theSession->getSessionId() << "] handleEvent("
                << theSession->getEventName(ENTRY_EVT) << ")");
        ActionList& actionList = nextState.getActionList(ENTRY_EVT);
        ActionList::iterator it = actionList.begin();
        for (; it != actionList.end(); it++)
        {
            (*it)(theSession);
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------

int Fsm::generateEvent(
        Fsm::Session* theSession,
        const int theEventId)
{
    theSession->handleEvent(theEventId);
    return 0;
}

//-----------------------------------------------------------------------------

int Fsm::ignoreEvent(
        Fsm::Session* theSession)
{
    return 0;
}

//-----------------------------------------------------------------------------
int Fsm::newTimer(
        Fsm::Session* theSession,
        const long long theUsec)
{
    theSession->newTimer(theUsec);
    return 0;
}

//-----------------------------------------------------------------------------

int Fsm::newFuncTimer(
		Fsm::Session* theSession,
		TimerGettor theTimerGettor)
{
	long long usec = theTimerGettor();
	return Fsm::newTimer(theSession, usec);
}

//-----------------------------------------------------------------------------
int Fsm::cancelTimer(
        Fsm::Session* theSession)
{
    theSession->cancelTimer();
    return 0;
}

//-----------------------------------------------------------------------------

int Fsm::deleteSession(
        Fsm::Session* theSession)
{
    delete theSession;
    return 0;
}

//-----------------------------------------------------------------------------

