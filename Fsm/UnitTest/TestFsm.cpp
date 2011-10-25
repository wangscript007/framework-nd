#include "Fsm.h"
#include "Action.h"
#include "State.h"
#include "Session.h"
#include "Reactor.h"
#include <iostream>
#include <boost/bind.hpp>
#include <unistd.h>
using namespace std;

int testEnter(Fsm::Session* theSession)
{
    cout << "enter" << endl;
    return 0;
}
int testExit(Fsm::Session* theSession)
{
    cout << "exit" << endl;
    return 0;
}

enum MyState
{
    INIT_STATE = 1,
    TIME_STATE = 2,
    END_STATE  = 3
};

enum MyEvent
{
    START_EVT = 0
};

int main()
{
    Fsm::FiniteStateMachine fsm;
    fsm += NEW_FSM_STATE(INIT_STATE);
    fsm +=      Fsm::Event(ENTRY_EVT,  &testEnter);
    fsm +=      Fsm::Event(START_EVT,  boost::bind(Fsm::changeState, _1, 2));
    fsm +=      Fsm::Event(EXIT_EVT,   &testExit);

    fsm += NEW_FSM_STATE(TIME_STATE);
    fsm +=      Fsm::Event(ENTRY_EVT,   boost::bind(Fsm::newSecTimer, _1, 1));
    fsm +=      Fsm::Event(TIMEOUT_EVT, boost::bind(Fsm::changeState, _1, 3));
    fsm +=      Fsm::Event(EXIT_EVT,    boost::bind(Fsm::cancelTimer, _1   ));

    fsm += NEW_FSM_STATE(END_STATE);
    fsm +=      Fsm::Event(ENTRY_EVT,  &testEnter);
    fsm +=      Fsm::Event(EXIT_EVT,   &testExit);
    cout << "fsm,initstate:" << fsm.getFirstStateId() << endl;
    cout << "fsm,endstate:" << fsm.getLastStateId() << endl;

    Fsm::Session session(&fsm, 0, NULL);
    session.handleEvent(START_EVT);
    sleep(3);
    return 0;
}

