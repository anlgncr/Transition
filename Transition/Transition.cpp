#include "Transition.h"
uint16_t Transition::indexOfTransitions = 0;
uint16_t Transition::indexOfUsedTransitions = 0;
Transition::transitionObject* Transition::transitions[TRANSITION_MAX_SIZE];
Transition::transitionObject* Transition::usedTransitions[TRANSITION_MAX_SIZE];


Transition::Transition(){}

Transition::transitionObject* Transition::start(int16_t* var, int16_t initVal, int16_t endVal, uint32_t totalTime){	
	if((millis() + totalTime) < totalTime)//If timer register overflows
		return;
	
	if(indexOfTransitions >= TRANSITION_MAX_SIZE)
		return;
	
	transitionObject *mTransition = findVariable(var);
	
	if(!mTransition){
		if(indexOfUsedTransitions > 0){
			mTransition = usedTransitions[indexOfUsedTransitions - 1];
			indexOfUsedTransitions--;
		}
		else{
			mTransition = malloc(sizeof(transitionObject));
		}
		transitions[indexOfTransitions++] = mTransition;
	}
	
	mTransition->var = var;
	mTransition->initVal = initVal;
	mTransition->endVal = endVal;
	mTransition->totalTime = totalTime;
	mTransition->initTime = millis();
	mTransition->repeatCount = 1;
	mTransition->animation = LINEAR;
	mTransition->onFinish = NULL;
	mTransition->onUpdate = NULL;
	
	return mTransition;
}


Transition::transitionObject* Transition::findVariable(uint16_t* var){
	for(uint8_t i = 0; i < indexOfTransitions; i++){
		uint16_t* transitionVar = transitions[i]->var;
		if(transitionVar == var){
			return transitions[i];
		}
	}
	return NULL;
}

void Transition::dispose(){
	uint8_t currentIndex = 0;
	for(uint8_t i=0; i<indexOfTransitions; i++){
		if(transitions[i] != NULL){
			if(currentIndex != i){			
				transitions[currentIndex] = transitions[i];
				transitions[i] = NULL;
			}
			currentIndex++;
		}
	}
	indexOfTransitions = currentIndex;
}

void Transition::update()
{		
	for(uint8_t i=0; i< indexOfTransitions; i++)
	{
		transitionObject* mTransition = transitions[i];
	
		uint32_t totalTime = mTransition->totalTime;
		uint32_t elapsed_time = millis() - mTransition->initTime;
		
		if(elapsed_time >= totalTime)
		{	
			onUpdateEvent(mTransition);
			*(mTransition->var) = mTransition->endVal;
			
			uint16_t repeatCount = mTransition->repeatCount;
			
			if(mTransition->reverse && repeatCount % 2 == 0){	
				int16_t temp = mTransition->initVal;
				mTransition->initVal = mTransition->endVal;
				mTransition->endVal = temp;
			}
			
			if(repeatCount > 1){
				mTransition->initTime = millis();
				mTransition->repeatCount--;
			}
			else if(repeatCount == 0){
				mTransition->initTime = millis();
			}
			else{
				fPtr function = mTransition->onFinish;
				mTransition->finished = true;
				transitions[i] = NULL;
				dispose();
				usedTransitions[indexOfUsedTransitions++] = mTransition;

				if(function){
					function(mTransition);
				}
			}
		}
		else{
			int16_t value = mTransition->endVal - mTransition->initVal;
			float ratio = (float)elapsed_time / (float)(mTransition->totalTime);
			
			float cal_ratio = 1;
			switch(mTransition->animation){
				case LINEAR: 					cal_ratio = linear(ratio); 				break;
				case EASE_IN: 					cal_ratio = easeIn(ratio); 				break;
				case EASE_OUT: 					cal_ratio = easeOut(ratio); 			break;
				case EASE_IN_BOUNCE: 			cal_ratio = easeInBounce(ratio); 		break;
				case EASE_OUT_BOUNCE: 			cal_ratio = easeOutBounce(ratio); 		break;
				case EASE_IN_BACK: 				cal_ratio = easeInBack(ratio); 			break;
				case EASE_OUT_BACK: 			cal_ratio = easeOutBack(ratio); 		break;
				case EASE_IN_ELASTIC: 			cal_ratio = easeInElastic(ratio); 		break;
				case EASE_OUT_ELASTIC: 			cal_ratio = easeOutElastic(ratio); 		break;
				case EASE_IN_OUT: 				cal_ratio = easeInOut(ratio); 			break;
				case EASE_OUT_IN: 				cal_ratio = easeOutIn(ratio); 			break;
				case EASE_IN_OUT_ELASTIC: 		cal_ratio = easeInOutElastic(ratio); 	break;
				case EASE_OUT_IN_ELASTIC: 		cal_ratio = easeOutInElastic(ratio); 	break;
			}
			*(mTransition->var) = mTransition->initVal + round(value * cal_ratio);
			onUpdateEvent(mTransition);
		}
	}		
}


void Transition::onUpdateEvent(transitionObject* transition){
	fPtr function = transition->onUpdate;
	
	if(function){
		function(transition);
	}
}

/* Methods for transition */

float Transition::linear(float ratio){
	return ratio;
}

float Transition::easeIn(float ratio){
  return ratio * ratio * ratio;
}

float Transition::easeOut(float ratio){
  float invRatio = ratio - 1;
  return invRatio * invRatio * invRatio +1; 
}

float Transition::easeInElastic(float ratio){
	if(ratio == 1 || ratio ==0)
		return ratio;
	else{
		float p = 0.3;
		float s = p/4.0;
		float invRatio = ratio - 1;
		return -1.0 * pow(2.0, 10.0 * invRatio) * sin((invRatio-s)*(2.0* 3.14)/p);
	}
}

float Transition::easeInBounce(float ratio){
	return 1.0 - easeOutBounce(1.0 - ratio);
}

float Transition::easeOutBounce(float ratio){
	float s= 7.5625;
	float p= 2.75;
	float l;
	if (ratio < (1.0/p)){
		l = s * pow(ratio, 2);
	}
	else{
		if (ratio < (2.0/p)){
			ratio -= 1.5/p;
			l = s * pow(ratio, 2) + 0.75;
		}
		else{
			if (ratio < 2.5/p){
				ratio -= 2.25/p;
				l = s * pow(ratio, 2) + 0.9375;
			}
			else{
				ratio -= 2.625/p;
				l =  s * pow(ratio, 2) + 0.984375;
			}
		}
	}
	return l;
}

float Transition::easeInBack(float ratio){
	float s = 1.70158;
	return pow(ratio, 2) * ((s + 1.0)*ratio - s);
}
		
float Transition::easeOutBack(float ratio){
	float invRatio = ratio - 1.0;            
	float s = 1.70158;
	return pow(invRatio, 2) * ((s + 1.0)*invRatio + s) + 1.0;
}

float Transition::easeOutElastic(float ratio){
	if (ratio == 0 || ratio == 1) return ratio;
	else{
		float p = 0.3;
		float s = p/4.0;                
		return pow(2.0, -10.0*ratio) * sin((ratio-s)*(2.0*3.14)/p) + 1;                
	}            
}      	

float Transition::easeInOut(float ratio){
	return (ratio < 0.5)? easeIn(ratio) : easeOut(ratio);
} 

float Transition::easeOutIn(float ratio){
	return (ratio < 0.5)? easeOut(ratio) : easeIn(ratio);
} 

float Transition::easeInOutElastic(float ratio){
	return (ratio < 0.5)? easeInElastic(ratio) : easeOutElastic(ratio);
} 

float Transition::easeOutInElastic(float ratio){
	return (ratio < 0.5)? easeOutElastic(ratio) : easeInElastic(ratio);	
}








