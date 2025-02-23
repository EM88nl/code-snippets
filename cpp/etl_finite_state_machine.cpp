#include <etl/fsm.h>

struct EventId {
  enum {
    kMyFirstEvent,
    kMySecondEvent,
  };
};

/**
 * First event class.
 */

class MyFirstEvent : public etl::message<EventId::kMyFirstEvent> {};

class MySecondEvent : public etl::message<EventId::kMySecondEvent> {};

class MyClass;  // Forward declaration needed.

struct StateId {
  enum { kMyFirstState, kMySecondState, kNumberOfStates };
};

class MyFirstState
    : public etl::fsm_state<MyClass, MyFirstState, StateId::kMyFirstState,
                            MyFirstEvent> {
 public:
  MyFirstState(MyClass& context) : context_(context) {}

  etl::fsm_state_id_t on_event(const MyFirstEvent& event) { return STATE_ID; }

  etl::fsm_state_id_t on_event_unknown(const etl::imessage&) {
    return STATE_ID;
  }

 private:
  MyClass& context_;
};

class MySecondState
    : public etl::fsm_state<MyClass, MySecondState, StateId::kMySecondState,
                            MySecondEvent> {
 public:
  MySecondState(MyClass& context) : context_(context) {}

  etl::fsm_state_id_t on_event(const MySecondEvent& event) { return STATE_ID; }

  etl::fsm_state_id_t on_event_unknown(const etl::imessage&) {
    return STATE_ID;
  }

 private:
  MyClass& context_;
};

class MyClass : private etl::fsm {
 public:
  MyClass();

  void Start() {
    this->set_states(state_list_, StateId::kNumStates);
    this->start();

    this->receive(MyFirstEvent());
    this->receive(MySecondEvent());
  }

 private:
  MyFirstState first_state_;
  MySecondState second_state_;
  etl::ifsm_state* state_list_[StateId::kNumStates];
};

MyClass::MyClass() : fsm(0), first_state_(*this), second_state_(*this) {
  state_list_[0] = &first_state_;
  state_list_[1] = &second_state_;
}

// Main Function
int main() {
  MyClass my_class;
  my_class.Start();

  return 0;
}