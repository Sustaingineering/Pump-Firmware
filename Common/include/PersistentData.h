#pragma once
#include <string.h>
#include <string>
#include "SdCard.h"
using namespace std;

class PairData 
{
private:
    int m_value;
    String m_name;

public:
    PairData(String name, int value);
    int getValue();
    String getName();
};

/**
 * @brief A class that stores persistent data as key value pairs in program memory
 * as extracted from the 'config.txt' file found on (or created on) the SD Card.
 * 
 * The format of the config file MUST be:
 * (string)persistentVariableName=(int)persistentVariableValue
 */
class PersistentData
{
private:

    char * m_configPath = "/config.txt";

    /**
     * @brief Stores all config settings as key-value pairs
     */
    PairData ** m_pairs;
    int m_numEntries;

    /**
     * @brief Stores default values to be used when SD Card object is null
     * or configPath file data is inaccessible for any reason.
     */
    PairData ** m_defaults;

    bool m_useDefaults = true;

    /**
     * @brief Utility function that is used to separate the input strings into 
     * appropriate pieces that will be further processed into key-value pairs
     * 
     * @param str is string consisting of the entirety of the file contents
     * @param splitBy is the delimiter used to split the string. string("\n") is used as default
     * @param tokens is the empty vector passed by reference for temporarily holding string fragments
     * which the parent can use to access all the split pieces
     */
    void stringSplit(string str, string splitBy, string configs[]);

    void setDefaults();
public:

    PersistentData();

    /**
     * @brief Read configuration values stored on SD Card in 'configs.txt'
     * in the root directory. If this file is not present, a new file with 
     * default parameters is created. If some required parameters are missing,
     * default values are added to the file.
     * 
     * @param memory is a reference to the SdCard object that is accessed
     * @return true if initialization was successful
     */
    bool initialize(SdCard* memory);

    /**
     * @brief Get the value associated with a configuration name
     * 
     * @param name is the key in the key-value pair that identifies the correct value
     * @return const int for the value associated with the name. Must be const as 
     * this data cannot be modified
     */
    int getValue(String name);

    /**
     * @brief Debug function to quickly print out all pairs in the vector pairs
     */
    void printAll(void);
};
