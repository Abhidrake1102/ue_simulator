
#include "MessageSequencer.h"
#include <iostream>

using namespace std;


// constructor of the sequenceri
MessageSequencer::MessageSequencer() {
	currentIndex = 0;
}

// lodaing currend phase 1 messages

void MessageSequencer::sequenceLoad() {
	sequence.clear();

	// prach msg
	Message prach;
	prach.type = PRACH;
	prach.payload.push_back(0x01);
	prach.length = prach.payload.size();


	//sequence creation, based on previous msg make a default next one
	SequencerEntry prachEntry;
	prachEntry.request = prach;
	prachEntry.expectedResponse = RACH_RESPONSE;

	// load into the container
	sequence.push_back(prachEntry);

	// rrc setup request
	Message rrcSetupRequest;
	rrcSetupRequest.type = RRC_SETUP_REQUEST;
	rrcSetupRequest.payload.push_back(0x03);
	rrcSetupRequest.length = rrcSetupRequest.payload.size();

	// sequence creation
	SequencerEntry rrcSetupRequestEntry;
	rrcSetupRequestEntry.request = rrcSetupRequest;
	rrcSetupRequestEntry.expectedResponse = RRC_SETUP;

	// load the sequencer
	sequence.push_back(rrcSetupRequestEntry);

	// rrc Setup complete message creation
	Message rrcSetupComplete;
	rrcSetupComplete.type = RRC_SETUP_COMPLETE;
	rrcSetupComplete.payload.push_back(0x05);

	// load sequencer
	SequencerEntry rrcSetupCompleteEntry;
	rrcSetupCompleteEntry.request = rrcSetupComplete;
	rrcSetupCompleteEntry.expectedResponse  = END_OF_PHASE;  // phase 1 ends here, time being im using rach response expected

	sequence.push_back(rrcSetupCompleteEntry);

	// failsafe
	currentIndex = 0;
}

bool MessageSequencer::isComplete() {
	return currentIndex>=sequence.size();
}

bool MessageSequencer::getNextMessage(Message& message) {
	if(isComplete()){
		return false;
	}
	message = sequence[currentIndex].request;
	return true;
}

bool MessageSequencer::validateResponse(MessageType response){
	if(isComplete()){
		return false;
	}

	return response == sequence[currentIndex].expectedResponse;
}

void MessageSequencer::moveToNext(){
	if(!isComplete()){
		currentIndex++;
	}
}
