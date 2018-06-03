 
#define timeout true
mtype={ON,OFF};
   ltl spec {
        [] (((state == OFF) && sensor) -> <> (state == ON)) &&
        [] (((state == ON) && timeout) -> <> (state == OFF))
    }
    bit sensor;
    byte state;
    active proctype apagado_auto () {
           state = OFF;
           do
           :: (state == OFF) -> atomic {
              if
              :: sensor -> state = ON; sensor = 0;
              fi
           }
           :: (state == ON)  -> atomic {
              if
              :: timeout -> state = OFF;
              fi
           }
           od
    }
    active proctype entorno () {
           do
           :: sensor = 1;
           :: (! sensor) -> skip;
           od
    }
