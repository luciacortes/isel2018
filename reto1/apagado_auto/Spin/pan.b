	switch (t->back) {
	default: Uerror("bad return move");
	case  0: goto R999; /* nothing to undo */

		 /* CLAIM spec */
;
		;
		;
		;
		;
		;
		;
		;
		
	case 7: // STATE 20
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC entorno */

	case 8: // STATE 1
		;
		now.sensor = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 10: // STATE 7
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC apagado_auto */

	case 11: // STATE 1
		;
		now.state = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 13: // STATE 5
		;
		now.sensor = trpt->bup.ovals[1];
		now.state = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 15: // STATE 11
		;
		now.state = trpt->bup.oval;
		;
		goto R999;

	case 16: // STATE 18
		;
		p_restor(II);
		;
		;
		goto R999;
	}

