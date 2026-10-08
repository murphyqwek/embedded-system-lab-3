#ifndef KEY_QUEUE_H
#define KEY_QUEUE_H
#define EMPTY_QUEUE (-1)
void initKeyQueue(void);
void appendKeyToQueue(int key);
int readKeyFromQueue(void);
void clearKeyQueue(void);
#endif
