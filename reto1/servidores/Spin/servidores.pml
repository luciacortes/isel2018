
mtype={APAGADO, MIDE_CARGA, ENCENDIDO, PAR, IMPAR};
   ltl spec {
        [] (((state == MIDE_CARGA) && (carga>carga_min && carga<carga_max)) -> <> (state == MIDE_CARGA)) &&
        [] (((stata == MIDE_CARGA) && carga>=carga_max) -> <> (state == ENCENDIDO)) &&
        [] (((stata == MIDE_CARGA) && carga<=carga_min) -> <> (state == APAGADO)) &&
        [] (((state == ENCENDIDO) && f) -> <> (state == PAR)) &&
        [] (((state == ENCENDIDO) && !f) -> <> (state == IMPAR)) &&
        [] (((state == APAGADO) && f) -> <> (state == PAR)) &&
        [] (((state == APAGADO) && !f) -> <> (state == IMPAR)) &&
        [] (((state == IMPAR)) -> <> (state == MIDE_CARGA)) &&
        [] (((state == PAR)) -> <> (state == MIDE_CARGA));
    }

    bit f;
    bit max_equipos;
    byte carga;
    byte carga_max;
    byte carga_min;
    byte state;

    active proctype servers () {
           state = MIDE_CARGA;
           do
           :: (state == MIDE_CARGA) -> atomic {
              if
              :: (carga >= carga_max) -> state = ENCENDIDO;
              :: (carga <= carga_min) -> state = APAGADO;
              :: else -> state = MIDE_CARGA;
              fi
           }
           :: (state == ENCENDIDO) -> atomic {
              if
              :: f -> state = PAR;
              :: else -> state = IMPAR;
              fi
           }
           :: (state == APAGADO) -> atomic {
              if
              :: f -> state = PAR;
              :: else -> state = IMPAR;
              fi
           }
            :: (state == IMPAR) -> atomic {
              f = max_equipos; state = MIDE_CARGA;
           }
            :: (state == PAR) -> atomic {
              f = !max_equipos; state = MIDE_CARGA;
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
