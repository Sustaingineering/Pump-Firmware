

#define MIN_PUBLISH_INTERVAL 270
#define TCP_HEADER_SIZE 40
#define MAX_BYTES_DATA_OPERATION 622
#define MAX_BYTES_PER_OPERATION (MAX_BYTES_DATA_OPERATION - TCP_HEADER_SIZE)


/**
 * @brief Calls all the GSM Module Unit Tests
 * 
 */
void testGsm();

#if ERTC == 1
/**
 * @brief Tests that the time difference between
 * true publish events is at least MIN_PUBLISH_INTERVAL
 * as calculated above (independent of implementation).
 * 
 * Publishes a message repeatedly, till a true publish event
 * occurs. Then publishes another message repeatedly for another
 * true publish event. Calculates the difference between the two 
 * timestamps
 */
bool testPublishInterval();
#endif

/**
 * @brief Tests that the length of each publish message
 * is strictly less than the MAX_BYTES_PER_OPERATION macro
 * as defined above, based on Spark Particle Free Plan documentation.
 * 
 */
bool testPublishLength();


// /**
//  * @brief Tests whether the Publish method will
//  * correctly reject inputs of sizes larger than the defined
//  * maximum number of characters
//  * 
//  * Sends regular size message
//  * Sends max size message
//  * Sends larger than max size message
//  * 
//  */
// void testMaxLengthRejection();
