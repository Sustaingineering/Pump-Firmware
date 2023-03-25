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
    LOGGER("Time between messages: " + String(time_btwn_messages));
    LOGGER("Total messages cap: " + String(total_messages_cap));
    LOGGER("Initialized GSM");

    initializeEeprom();

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
    max_operations_per_day = (gsmParameters.maxTotalOperations/NUM_DAYS_IN_MONTH)/gsmParameters.totalParticles;
    max_data_bytes_sent = (MAX_BYTES_PER_DATA_OPERATION - gsmParameters.maxHeaderSize);
    total_messages_cap = (max_data_bytes_sent / gsmParameters.maxMessageSize);
    time_btwn_messages = CEILING((TOTAL_SECONDS_DAY / max_operations_per_day), total_messages_cap);

    LOGGER(String(time_btwn_messages));
    LOGGER(String(time - m_timeFromLastMessage)); 

    if(time_btwn_messages < 1)
    {
        String message;
        message = "ERROR: time between messages is less than 1";
        m_buffer += message;
        time_btwn_messages = TIME_BTWN_MESSAGES;
        if (Particle.publish(pumpId, jsonArrayFormatting_(m_buffer).c_str()))
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

    if (m_buffer == "")
    {
        // Add to empty buffer & set up counter & timer
        m_buffer += message;
        m_buffer += delimiter;
        m_counter++;
        // time at which message was last added to the buffer
        m_timeFromLastMessage = Time.now();

    }
    else if (time - m_timeFromLastMessage >= time_btwn_messages)
    {
        m_buffer += message;
        m_buffer += delimiter;
        m_counter++;
        m_timeFromLastMessage = time;

        if (m_counter == total_messages_cap)
        {
            // Publish method may block (20 secs - 10 mins)
            // https://docs.particle.io/reference/device-os/firmware/#particle-publish-
            if (Particle.publish(pumpId, jsonArrayFormatting_(m_buffer).c_str()))
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

String Gsm::jsonArrayFormatting_(String message)
{
    return ("\"a\":[" + message + "]");
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
    if(gsmParameters.maxTotalOperations <= 0 || gsmParameters.maxHeaderSize <= 0 ||
        gsmParameters.maxMessageSize <= 0 || gsmParameters.totalParticles <= 0)
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
    gsmParameters.totalParticles = newTotalParticles.toInt();
    EEPROM.put(EEPROM_INITIAL_ADDRESS, gsmParameters);
    return 1;
}

int Gsm::setMaxMessageSize(String newMaxMessageSize)
{
    gsmParameters.maxMessageSize = newMaxMessageSize.toInt();
    EEPROM.put(EEPROM_INITIAL_ADDRESS, gsmParameters);
    return 1;
}

int Gsm::setMaxHeaderSize(String newMaxHeaderSize)
{
    gsmParameters.maxHeaderSize = newMaxHeaderSize.toInt();
    EEPROM.put(EEPROM_INITIAL_ADDRESS, gsmParameters);
    return 1;
}

int Gsm::setCloudPumpId(String newCloudPumpId)
{
    gsmParameters.cloudPumpId = newCloudPumpId.toInt();
    EEPROM.put(EEPROM_INITIAL_ADDRESS, gsmParameters);  
    return 1;
}

int Gsm::setMaxOperations(String newMaxOperations)
{
    gsmParameters.maxTotalOperations = newMaxOperations.toInt();
    EEPROM.put(EEPROM_INITIAL_ADDRESS, gsmParameters);
    return 1;
}