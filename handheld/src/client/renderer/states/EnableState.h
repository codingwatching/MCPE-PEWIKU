#ifndef ENABLESTATE_H
#define ENABLESTATE_H

#include "../gles.h"

struct EnableState {
	GLenum state;

	EnableState(GLenum s) : state(s) {
		glEnable2(state);
	}
	~EnableState() {
		glDisable2(state);
	}
};

#endif // ENABLESTATE_H
