
ltl spec {
[] (((state == OFF) && button) -> <> (state == ON)) &&
[] (((state == ON) && button) -> <> (state == OFF)) &&
[] (((state == ON) && detector) -> <> (state == SONANDO)) &&
[] (((state == SONANDO) && button) -> <> (state == ON))
}

bit button;
bit detector;
byte state;
mtype = {OFF, ON, SONANDO};

active proctype lampara_fsm () {
  state = OFF;
  do
  :: (state == OFF) -> atomic {
      if
        :: button -> state = ON; button = 0
      fi
      }
    :: (state == ON)  -> atomic {
      if
        :: button -> state = OFF; button = 0
        :: detector -> state = SONANDO; detector = 0
      fi
      }
    :: (state == SONANDO)  -> atomic {
      if
        :: button -> state = OFF; button = 0
      fi
      }
  od
}

active proctype entorno {
  do
  :: button = 1
  :: button = 0
  :: detector = 1
  :: detector = 0
  od
}
