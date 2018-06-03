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
		;
		;
		;
		;
		
	case 9: // STATE 27
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC entorno */

	case 10: // STATE 1
		;
		now.button = trpt->bup.oval;
		;
		goto R999;

	case 11: // STATE 2
		;
		now.button = trpt->bup.oval;
		;
		goto R999;

	case 12: // STATE 3
		;
		now.detector = trpt->bup.oval;
		;
		goto R999;

	case 13: // STATE 4
		;
		now.detector = trpt->bup.oval;
		;
		goto R999;

	case 14: // STATE 8
		;
		p_restor(II);
		;
		;
		goto R999;

		 /* PROC lampara_fsm */

	case 15: // STATE 1
		;
		now.state = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 17: // STATE 5
		;
		now.button = trpt->bup.ovals[1];
		now.state = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 19: // STATE 12
		;
		now.button = trpt->bup.ovals[1];
		now.state = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		
	case 20: // STATE 17
		goto R999;

	case 21: // STATE 15
		;
		now.detector = trpt->bup.ovals[1];
		now.state = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 23: // STATE 22
		;
		now.button = trpt->bup.ovals[1];
		now.state = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;

	case 24: // STATE 29
		;
		p_restor(II);
		;
		;
		goto R999;
	}

