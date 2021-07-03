#include "PersistentData.h"

PairData::PairData(String name, int value)
{
  m_name = name;
  m_value = value;
}

String PairData::getName()
{
  return m_name;
}

int PairData::getValue()
{
  return m_value;
}

PersistentData::PersistentData() = default;

bool PersistentData::initialize(SdCard* memory)
{
    if (memory == NULL) {
      setDefaults();
      return false;
    }

    int numEntries = 0;
    string fileContents = string((*memory).readFile(m_configPath));
    for (char const &c: fileContents) {
      if (c == '\n') {
        numEntries++;
      }
    }

    if (numEntries < 3)
    {
      setDefaults();
      return false;
    }

    m_pairs = (PairData**) malloc(sizeof(PairData*) * numEntries);
    string configs[m_numEntries];

    stringSplit(fileContents, string("\n"), configs);

    int i;
    for (i = 0; i < numEntries; i++){
        // new creates heap memory, ensure to free later
        char * writable = new char[configs[i].size() + 1];
        
        copy(configs[i].begin(), configs[i].end(), writable);
        writable[configs[i].size()] = '\0'; // don't forget the terminating 0
        

        char *dataName = strtok(writable, "=");
        char *dataValue = strtok(NULL, "=");

        m_pairs[i] = new PairData(String(dataName), atoi(dataValue));
        
        // Prevent memory leakage
        delete[] writable;
    }

    m_useDefaults = false;
    return true;
}

void PersistentData::setDefaults() 
{
  m_useDefaults = true;
  m_defaults = (PairData**) malloc(sizeof(PairData*) * 3);
  m_defaults[0] = new PairData(String("pumpID"), 0);
  m_defaults[1] = new PairData(String("maxDataOperations"), 100000);
  m_defaults[2] = new PairData(String("numParticles"), 10);
  m_numEntries = 3;
  Serial.println("Using DEFAULTS for Persistent Data");
}

int PersistentData::getValue(String name) {
  int i;
  PairData ** listToCheck = m_useDefaults ? m_defaults : m_pairs; 

  for (i = 0; i < m_numEntries; i ++) {
      if (listToCheck[i]->getName() == name) {
        return listToCheck[i]->getValue();
      }
  }
  
  return -1;
}

void PersistentData::stringSplit(string str, string splitBy, string configs[])
{
    /* Store the original string in the array, so we can loop the rest
     * of the algorithm. */
    configs[0] = str;

    // Store the split index in a 'size_t' (unsigned integer) type.
    size_t splitAt;
    // Store the size of what we're splicing out.
    size_t splitLen = splitBy.size();
    // Create a string for temporarily storing the fragment we're processing.
    string frag;
    // Loop infinitely - break is internal.
    int idxToSplit = 0;
    while(true)
    {
        /* Store the last string in the vector, which is the only logical
         * candidate for processing. */
        frag = configs[idxToSplit];
        /* The index where the split is. */
        splitAt = frag.find(splitBy);
        // If we didn't find a new split point...
        if(splitAt == string::npos)
        {
            // Break the loop and (implicitly) return.
            break;
        }
        /* Put everything from the left side of the split where the string
         * being processed used to be. */
        configs[idxToSplit] = frag.substr(0, splitAt);
        /* Push everything from the right side of the split to the next empty
         * index in the vector. */
        configs[idxToSplit + 1] = frag.substr(splitAt+splitLen, frag.size()-(splitAt+splitLen));
        idxToSplit++;
    }
}

void PersistentData::printAll(void) {
  Serial.println("PRINTING ALL PAIRS IN PERSISTENT STORAGE");
  int i;

  PairData ** listToCheck = m_useDefaults ? m_defaults : m_pairs; 

  for (i = 0; i < m_numEntries; i ++) {
    Serial.printf("%s was with %d\n", listToCheck[i]->getName().c_str(), listToCheck[i]->getValue());
  }
}
