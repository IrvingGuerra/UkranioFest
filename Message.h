#ifndef MESSAGE_H
#define MESSAGE_H

#define TAM_MAX_DATA 65000

class Message {
	public:
		enum kindMessages: int {request, reply};
		enum allowedOperations: int {book, count, newbook};

		Message::kindMessages messageType;    
		uint64_t requestId;
		Message::allowedOperations operationId;
		char arguments[TAM_MAX_DATA];
		size_t length;
};

#endif
