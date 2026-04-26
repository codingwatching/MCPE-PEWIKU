#ifndef DISABLESTATE_H
#define DISABLESTATE_H

#include "../gles.h"

struct DisableState {
	GLenum state;
	bool wasInited;

	DisableState(GLenum s) : state(s) {
		glDisable2(state);
	}
	~DisableState() {
		glEnable2(state);
	}
};

#endif // DISABLESTATE_H
