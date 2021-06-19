#include "PersistentData.h"

void PersistentData::initialize(SdCard* memory)
{
    if (memory == NULL) {
      return false;
    }

    string fileContents = string(*memory.readFile(configPath));
    vector<string> configs;

    stringSplit(fileContents, string("\n"), configs);

    int i;
    for (i = 0; i < configs.size(); i++){
        // new creates heap memory, ensure to free later
        char * writable = new char[configs.at(i).size() + 1];
        
        copy(configs.at(i).begin(), configs.at(i).end(), writable);
        writable[configs.at(i).size()] = '\0'; // don't forget the terminating 0
        

        char *dataName = strtok(writable, "=");
        char *dataValue = strtok(NULL, "=");

        pairs.push_back(pair<string, const int>(dataName, atoi(dataValue)));
        
        // Prevent memory leakage
        delete[] writable;
    }

    return true;
}

const int PersistentData::getValue(string name) {
  int i;
  for (i = 0; i < pairs.size(); i ++) {
    if (get<0>(pairs[i]) == name) {
      return get<1>(pairs[i]);
    }
  }
  
  return -1;
}

void PersistentData::stringSplit(string str, string splitBy, vector<string>&tokens)
{
    /* Store the original string in the array, so we can loop the rest
     * of the algorithm. */
    tokens.push_back(str);

    // Store the split index in a 'size_t' (unsigned integer) type.
    size_t splitAt;
    // Store the size of what we're splicing out.
    size_t splitLen = splitBy.size();
    // Create a string for temporarily storing the fragment we're processing.
    std::string frag;
    // Loop infinitely - break is internal.
    while(true)
    {
        /* Store the last string in the vector, which is the only logical
         * candidate for processing. */
        frag = tokens.back();
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
        tokens.back() = frag.substr(0, splitAt);
        /* Push everything from the right side of the split to the next empty
         * index in the vector. */
        tokens.push_back(frag.substr(splitAt+splitLen, frag.size()-(splitAt+splitLen)));
    }
}

void PersistentData::printAll(void) {
  int i;
  for (i = 0; i < pairs.size(); i ++) {
    cout << get<0>(pairs[i]) << " was with ";
    cout << get<1>(pairs[i]) << '\n';
  }
}
