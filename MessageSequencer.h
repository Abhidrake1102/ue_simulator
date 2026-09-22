
#ifndef MESSAGE_SEQUENCER_H
#define MESSAGE_SEQUENCER_H

#include "message.h"

using namespace std;

struct SequencerEntry{
	Message request;
	MessageType  expectedResponse;
};

class MessageSequencer {
	private:
		size_t currentIndex;
		vector <SequencerEntry> sequence;
	public:

		MessageSequencer();
		void sequenceLoad();

		bool isComplete();

		bool getNextMessage(Message &message);

		bool validateResponse(MessageType response);

		void moveToNext();
};

#endif
