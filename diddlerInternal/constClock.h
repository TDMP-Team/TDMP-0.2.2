#pragma once
namespace constClock {
	void beginConstantClock(float pulseLengthMS);
	void endConstantClock();
	void cUpdateThread(float msTime, bool* terminator);
	void cUpdateFunction();
}