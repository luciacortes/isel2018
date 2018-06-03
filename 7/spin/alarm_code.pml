#define timeout true

ltl spec {
[] (((state == OFF) && button) -> <> (state == PRE_ON_1)) &&
[] (((state == PRE_ON_1) && button) -> <> (state == PRE_ON_2)) &&
[] (((state == PRE_ON_2) && button) -> <> (state == PRE_ON_3)) &&
[] (((state == PRE_ON_3) && button) -> <> (state == ON)) &&

[] (((state == PRE_ON_1) && timeout) -> <> (state == OFF)) &&
[] (((state == PRE_ON_2) && timeout) -> <> (state == OFF)) &&
[] (((state == PRE_ON_3) && timeout) -> <> (state == OFF)) &&

[] (((state == ON) && detector) -> <> (state == SONANDO)) &&
[] (((state == SONANDO) && button) -> <> (state == ON)) &&

[] (((state == ON) && button) -> <> (state == PRE_OFF_1)) &&
[] (((state == PRE_OFF_1) && button) -> <> (state == PRE_OFF_2)) &&
[] (((state == PRE_OFF_2) && button) -> <> (state == PRE_OFF_3)) &&
[] (((state == PRE_OFF_3) && button) -> <> (state == OFF)) &&

[] (((state == PRE_OFF_1) && timeout) -> <> (state == ON)) &&
[] (((state == PRE_OFF_2) && timeout) -> <> (state == ON)) &&
[] (((state == PRE_OFF_3) && timeout) -> <> (state == ON))
}

bit button;
bit detector;
byte state;
mtype = {
  PRE_ON_1,
  PRE_ON_2,
  PRE_ON_3,
  ON,
  OFF,
  PRE_OFF_1,
  PRE_OFF_2,
  PRE_OFF_3,
  SONANDO

};

active proctype fsm () {
  state = OFF;
  do
  :: (state == OFF) -> atomic {
      if
        :: button -> state = PRE_ON_1; button = 0
      fi
      }
  :: (state == PRE_ON_1)  -> atomic {
      if
        :: button -> state = PRE_ON_2; button = 0
      fi
      if
        :: timeout -> state = OFF; button = 0
      fi
      }
  :: (state == PRE_ON_2)  -> atomic {
      if
        :: button -> state = PRE_ON_3; button = 0
      fi
      if
        :: timeout -> state = OFF; button = 0
      fi
      }
  :: (state == PRE_ON_3)  -> atomic {
      if
        :: button -> state = ON; button = 0
      fi
      if
        :: timeout -> state = OFF; button = 0
      fi
      }

  :: (state == ON) -> atomic {
        if
          :: button -> state = PRE_OFF_1; button = 0
        fi
        if
          :: detector -> state = SONANDO; detector = 0;
        fi
        }
  :: (state == PRE_OFF_1)  -> atomic {
        if
          :: button -> state = PRE_OFF_2; button = 0
        fi
        if
          :: timeout -> state = ON; button = 0
        fi
        }
    :: (state == PRE_ON_2)  -> atomic {
        if
          :: button -> state = PRE_ON_3; button = 0
        fi
        if
          :: timeout -> state = ON; button = 0
        fi
        }
    :: (state == PRE_ON_3)  -> atomic {
        if
          :: button -> state = OFF; button = 0
        fi
        if
          :: timeout -> state = ON; button = 0
        fi
        }

  :: (state == SONANDO)  -> atomic {
      if
        :: button -> state = ON; button = 0
      fi
      }
  od
}

active proctype entorno () {
  do
  :: button = 1
  :: button = 0
  :: detector = 1
  :: detector = 0
  od
}
