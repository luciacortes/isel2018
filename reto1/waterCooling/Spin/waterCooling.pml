
mtype={REPOSO,TUBERIA1, TUBERIA2, TUBERIA3};
   ltl spec {
        [] (((state == REPOSO) && umbral1) -> <> (state == TUBERIA1)) &&
        [] (((state == TUBERIA1) && umbral2) -> <> (state == TUBERIA2)) &&
        [] (((state == TUBERIA2) && umbral3) -> <> (state == TUBERIA3)) &&
        [] (((state == TUBERIA3) && umbral2) -> <> (state == TUBERIA2)) &&
        [] (((state == TUBERIA2) && umbral1) -> <> (state == TUBERIA1)) &&
        [] (((state == TUBERIA1) && umbral0) -> <> (state == REPOSO))
    }
    bit umbral0;
    bit umbral1;
    bit umbral2;
    bit umbral3;
    byte state;
    active proctype apagado_auto () {
           state = REPOSO;
           do
           :: (state == REPOSO) -> atomic {
              if
              :: umbral1 -> state = TUBERIA1;
              fi
           }
           :: (state == TUBERIA1) -> atomic {
              if
              :: umbral2 -> state = TUBERIA2;
              fi
              if
              :: umbral0 -> state = REPOSO;
              fi
           }
           :: (state == TUBERIA2) -> atomic {
              if
              :: umbral3 -> state = TUBERIA3;
              fi
              if
              :: umbral1 -> state = TUBERIA1;
              fi
          :: (state == TUBERIA3) -> atomic {
             if
             :: umbral2 -> state = TUBERIA2;
             fi
           }
           od
    }
    active proctype entorno () {
           do
           :: umbral0 = 1;
           :: umbral0 = 0;
           :: umbral1 = 1;
           :: umbral1 = 0;
           :: umbral2 = 1;
           :: umbral2 = 0;
           :: umbral3 = 1;
           :: umbral3 = 0;
           od
    }
