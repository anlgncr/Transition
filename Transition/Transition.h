#ifndef Transition_h
#define Transition_h
#include "arduino.h"

#define TRANSITION_MAX_SIZE 50

#define LINEAR 0
#define EASE_IN 1
#define EASE_OUT 2
#define EASE_IN_BOUNCE 3
#define EASE_OUT_BOUNCE 4
#define EASE_IN_BACK 5
#define EASE_OUT_BACK 6
#define EASE_IN_ELASTIC 7 
#define EASE_OUT_ELASTIC 8
#define EASE_IN_OUT 9
#define EASE_OUT_IN 10
#define EASE_OUT_IN_ELASTIC 11 
#define EASE_IN_OUT_ELASTIC 12
#define NUMBER_OF_FUNCTION 13

class Transition{
	public:
		Transition();
		
		struct transitionObject;
		
		typedef void (*fPtr)(transitionObject*);
		struct transitionObject{
			int16_t* var;
			int16_t initVal;
			int16_t endVal;
			uint32_t initTime;
			uint32_t totalTime;
			uint16_t repeatCount;
			uint8_t animation;
			uint8_t finished;
			uint8_t reverse;
			fPtr onFinish;
			fPtr onUpdate;
		};
		
		static transitionObject *transitions[];
		static transitionObject *usedTransitions[];
		static void update();
		static transitionObject* start(int16_t* var, int16_t initVal, int16_t endVal, uint32_t totalTime);
		static uint16_t indexOfTransitions;
		static uint16_t indexOfUsedTransitions;
		
	private:
	
		static transitionObject* findVariable(uint16_t*);
		static void onUpdateEvent(transitionObject*);
		static void dispose();

		typedef float(Transition::*transitionFunction)(float);
		
		static float linear(float); 			
		static float easeIn(float); 			
		static float easeOut(float); 		
		static float easeInBounce(float); 	
		static float easeOutBounce(float); 	
		static float easeInBack(float); 		
		static float easeOutBack(float); 	
		static float easeInElastic(float); 	
		static float easeOutElastic(float); 	
		static float easeInOut(float); 		
		static float easeOutIn(float); 		
		static float easeOutInElastic(float);
		static float easeInOutElastic(float);
};
#endif