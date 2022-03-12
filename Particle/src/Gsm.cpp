#include "Gsm.h"
#include "Logger.h"

#define LOG_MODULE_SWITCH LOG_GSM_SWITCH

Gsm::Gsm(bool isConnected)
{
    m_isConnected = isConnected;
    if (m_isConnected)
    {
        SYSTEM_MODE(AUTOMATIC)
    }
    else
    {
        SYSTEM_MODE(MANUAL)
    }
}

bool Gsm::initialize()
{
    if (!m_isConnected)
    {
        return true;
    }

    delimiter = ";";
    m_buffer = "";
    m_counter = 0;
    LOGGER("Time between messages: " + String(TIME_BTWN_MESSAGES));
    LOGGER("Total messages cap: " + String(TOTAL_MESSAGES_CAP));
    LOGGER("Initialized GSM");


    //register cloud variable and function
    //the stuff before the Particle.variable... is for testing
    if(
        Particle.variable("maxTotalOps", gsmParameters.maxTotalOperations) &
        Particle.variable("pumpId", gsmParameters.cloudPumpId) &
        Particle.variable("numParticles", gsmParameters.totalParticles) &
        Particle.variable("maxMsgSize", gsmParameters.maxMessageSize) &
        Particle.variable("maxHdrSize", gsmParameters.maxHeaderSize) &

        Particle.function("setPumpId", &Gsm::setCloudPumpId, this) &
        Particle.function("setTotalPumps", &Gsm::setTotalParticles, this) &
        Particle.function("setMaxMsg", &Gsm::setMaxMessageSize, this) &
        Particle.function("setMaxHdr", &Gsm::setMaxHeaderSize, this) &
        Particle.function("setTotalOps", &Gsm::setMaxOperations, this)
    )
    {
        LOGGER("Initialized Cloud Variables and Functions.");
    }
    else
    {
        LOGGER("ERROR: Could not initialize Cloud Variables and Functions!");
    }
    
    return true;
}

String Gsm::Publish(String pumpId, String message)
{
    if (!m_isConnected)
    {
        return "";
    }
    /**
     * 1. If buffer is empty, add to it & start tracking time 
     * 2. If message time < MAXTIME_BTWN_MESSAGES; reject message; else add to buffer
     * 3. If m_counter == TOTAL_MESSAGES_CAP; publish & return message; else return empty string
     * 
     * delimiter = "\n"
     */
    int time = Time.now();

    if (m_buffer == "")
    {
        // Add to empty buffer & set up counter & timer
        m_buffer += message;
        m_buffer += delimiter;
        m_counter++;
        // time at which message was last added to the buffer
        m_timeFromLastMessage = Time.now();

    }
    else if (time - m_timeFromLastMessage >= TIME_BTWN_MESSAGES)
    {
        m_buffer += message;
        m_buffer += delimiter;
        m_counter++;
        m_timeFromLastMessage = time;

        if (m_counter == TOTAL_MESSAGES_CAP)
        {
            // Publish method may block (20 secs - 10 mins)
            // https://docs.particle.io/reference/device-os/firmware/#particle-publish-
            if (Particle.publish(pumpId, m_buffer.c_str()))
            {
                LOGGER("Succesfully Published buffer: " + m_buffer);
                String result = m_buffer;
                m_buffer = "";
                m_counter = 0;
                return result;
            }
            else
            {
                LOGGER("Could not publish message!");
                // Should probably do more here
            }
        }
    }
    LOGGER("Number of messages in buffer: " + String(m_counter));
    return String("");
}

int Gsm::getTotalDataUsage_()
{
    CellularData data;
    if (!Cellular.getDataUsage(data)) {
        LOGGER("Error! Not able to get data.");
        return 0;
    }
    else {
        LOGGER(data);
        return data.tx_total + data.rx_total;
    }
}

int Gsm::initializeEeprom()
{
    EEPROM.get(EEPROM_INITIAL_ADDRESS, gsmParameters);
    if(gsmParameters.maxTotalOperations == 0)
    {
        gsmParameters = {MAX_TOTAL_OPERATIONS, TOTAL_PARTICLES, 0, MAX_MESSAGE_SIZE, MAX_HEADER_SIZE};
        EEPROM.put(EEPROM_INITIAL_ADDRESS, gsmParameters);
        //for testing
        LOGGER("Defaults values were taken");
    }
    //for testing
    LOGGER("Retrieved Values: " + String(gsmParameters.totalParticles));
    return 1;
}

int Gsm::setTotalParticles(String newTotalParticles)
{
        //for testing
        EepromStorage temp_tester;
        EEPROM.get(EEPROM_INITIAL_ADDRESS, temp_tester);
        LOGGER("Old totalParticles: " + String(temp_tester.totalParticles));
    gsmParameters.totalParticles = newTotalParticles.toInt();
    EEPROM.put(EEPROM_INITIAL_ADDRESS, gsmParameters);
        //for testing
        EEPROM.get(EEPROM_INITIAL_ADDRESS, temp_tester);
        LOGGER("New totalParticles: " + String(temp_tester.totalParticles));
    return 1;
}

int Gsm::setMaxMessageSize(String newMaxMessageSize)
{
        //for testing
        EepromStorage temp_tester;
        EEPROM.get(EEPROM_INITIAL_ADDRESS, temp_tester);
        LOGGER("Old maxMessageSize: " + String(temp_tester.maxMessageSize));
    gsmParameters.maxMessageSize = newMaxMessageSize.toInt();
    EEPROM.put(EEPROM_INITIAL_ADDRESS, gsmParameters);
        //for testing
        EEPROM.get(EEPROM_INITIAL_ADDRESS, temp_tester);
        LOGGER("New maxMessageSize: " + String(temp_tester.maxMessageSize));
    return 1;
}

int Gsm::setMaxHeaderSize(String newMaxHeaderSize)
{
        //for testing
        EepromStorage temp_tester;
        EEPROM.get(EEPROM_INITIAL_ADDRESS, temp_tester);
        LOGGER("Old maxHeaderSize Value: " + String(temp_tester.maxHeaderSize));
    gsmParameters.maxHeaderSize = newMaxHeaderSize.toInt();
    EEPROM.put(EEPROM_INITIAL_ADDRESS, gsmParameters);
        //for testing
        EEPROM.get(EEPROM_INITIAL_ADDRESS, temp_tester);
        LOGGER("New maxHeaderSize Value: " + String(temp_tester.maxHeaderSize));
    return 1;
}

int Gsm::setCloudPumpId(String newCloudPumpId)
{
        //for testing
        EepromStorage temp_tester;
        EEPROM.get(EEPROM_INITIAL_ADDRESS, temp_tester);
        LOGGER("Old cloudPumpId Value: " + String(temp_tester.cloudPumpId));
    gsmParameters.cloudPumpId = newCloudPumpId.toInt();
    EEPROM.put(EEPROM_INITIAL_ADDRESS, gsmParameters);  
        //for testing
        EEPROM.get(EEPROM_INITIAL_ADDRESS, temp_tester);
        LOGGER("New cloudPumpId Value: " + String(temp_tester.cloudPumpId));
    return 1;
}

int Gsm::setMaxOperations(String newMaxOperations)
{
        //for testing
        EepromStorage temp_tester;
        EEPROM.get(EEPROM_INITIAL_ADDRESS, temp_tester);
        LOGGER("Old maxTotalOperations Value: " + String(temp_tester.maxTotalOperations));
    gsmParameters.maxTotalOperations = newMaxOperations.toInt();
    EEPROM.put(EEPROM_INITIAL_ADDRESS, gsmParameters);
        //for testing
        EEPROM.get(EEPROM_INITIAL_ADDRESS, temp_tester);
        LOGGER("Old maxTotalOperations Value: " + String(temp_tester.maxTotalOperations));
    return 1;
}