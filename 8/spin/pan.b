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

		 /* PROC lampara_fsm */

	case 10: // STATE 1
		;
		now.state = trpt->bup.oval;
		;
		goto R999;
;
		;
		
	case 12: // STATE 5
		;
		now.button = trpt->bup.ovals[1];
		now.state = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 14: // STATE 12
		;
		now.button = trpt->bup.ovals[1];
		now.state = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		
	case 15: // STATE 17
		goto R999;

	case 16: // STATE 15
		;
		now.detector = trpt->bup.ovals[1];
		now.state = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;
;
		;
		
	case 18: // STATE 22
		;
		now.button = trpt->bup.ovals[1];
		now.state = trpt->bup.ovals[0];
		;
		ungrab_ints(trpt->bup.ovals, 2);
		goto R999;

	case 19: // STATE 29
		;
		p_restor(II);
		;
		;
		goto R999;
	}

