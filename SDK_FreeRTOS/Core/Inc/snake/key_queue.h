#ifndef KEY_QUEUE_H
#define KEY_QUEUE_H

void initKeyQueue();
void appendKeyToQueue(uint8_t key);
void readKeyFromQueue(uint8_t* keyPointer);

#endif