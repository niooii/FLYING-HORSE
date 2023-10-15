#pragma once

namespace boss
{
	enum state
	{
		Idle,
		Throwing,
		Ultimate,
		ThrowingBeam,
		ThrowingBeam2,
		Final,
	};

	extern bool isThrowing;
	extern bool siphonPhase;
	extern bool beamPhase;
}