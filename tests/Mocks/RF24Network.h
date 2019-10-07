#ifndef MOCK_RF24_NETWORK_H
#define MOCK_RF24_NETWORK_H

class RF24Network
{};

struct RF24NetworkHeader
{
  uint16_t from_node; /**< Logical address where the message was generated */
  uint16_t to_node; /**< Logical address where the message is going */
  uint16_t id; /**< Sequential message ID, incremented every time a new frame is constructed */
  /**
   * Message Types:
   * User message types 1 through 64 will NOT be acknowledged by the network, while message types 65 through 127 will receive a network ACK.  
   * System message types 192 through 255 will NOT be acknowledged by the network. Message types 128 through 192 will receive a network ACK. <br>
   * <br><br>
   */
  unsigned char type; /**< <b>Type of the packet. </b> 0-127 are user-defined types, 128-255 are reserved for system */
  
  /**
  * During fragmentation, it carries the fragment_id, and on the last fragment
  * it carries the header_type.<br>
  */
  unsigned char reserved; /**< *Reserved for system use* */

  static uint16_t next_id; /**< The message ID of the next message to be sent (unused)*/

  /**
   * Default constructor
   *

   * Simply constructs a blank header
   */
  RF24NetworkHeader() {}

  /**
   * Send constructor  
   *  
   * @note Now supports automatic fragmentation for very long messages, which can be sent as usual if fragmentation is enabled. 
   *
   * Fragmentation is enabled by default for all devices except ATTiny <br>
   * Configure fragmentation and max payload size in RF24Network_config.h
   *  
   * Use this constructor to create a header and then send a message  
   *   
   * @code
   *  uint16_t recipient_address = 011;
   *  
   *  RF24NetworkHeader header(recipient_address,'t');
   *  
   *  network.write(header,&message,sizeof(message));
   * @endcode
   *
   * @param _to The Octal format, logical node address where the message is going
   * @param _type The type of message which follows.  Only 0-127 are allowed for
   * user messages. Types 1-64 will not receive a network acknowledgement.
   */

  RF24NetworkHeader(uint16_t _to, unsigned char _type = 0): to_node(_to), id(next_id++), type(_type) {}
  /**
   * Create debugging string
   *
   * Useful for debugging.  Dumps all members into a single string, using
   * internal static memory.  This memory will get overridden next time
   * you call the method.
   *
   * @return String representation of this object
   */
  const char* toString(void) const;
};



#endif
