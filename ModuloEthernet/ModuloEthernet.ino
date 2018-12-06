
//Incluimos la libreria para controlar el módulo ethernet
#include <EtherCard.h>

// Definimos la clave única que obtumidos desde http://arduino-tweet.appspot.com/
#define TOKEN   "1069784063752499200-3xJvG60Z82BbuFln2qRmPivV1GTaDE"

// establecemos la dirección MAC 
byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

const char website[] PROGMEM = "arduino-tweet.appspot.com";

static byte session;

byte Ethernet::buffer[700];
Stash stash;

static void sendToTwitter () {
  Serial.println("Sending tweet...");
  byte sd = stash.create();

  const char tweet[] = "Texto que se envia a twitter";
  stash.print("token=");
  stash.print(TOKEN);
  stash.print("&status=");
  stash.println(tweet);
  stash.save();
  int stash_size = stash.size();

  // Compose the http POST request, taking the headers below and appending
  // previously created stash in the sd holder.
  Stash::prepare(PSTR("POST http://$F/update HTTP/1.0" "\r\n"
    "Host: $F" "\r\n"
    "Content-Length: $D" "\r\n"
    "\r\n"
    "$H"),
  website, website, stash_size, sd);

  // send the packet - this also releases all stash buffers once done
  // Save the session ID so we can watch for it in the main loop.
  session = ether.tcpSend();
}

void setup () {
  Serial.begin(57600);
  Serial.println("\n[Twitter Client]");

  // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  if (!ether.dnsLookup(website))
    Serial.println(F("DNS failed"));

  ether.printIp("SRV: ", ether.hisip);

  sendToTwitter();
}

void loop () {
  ether.packetLoop(ether.packetReceive());

  const char* reply = ether.tcpReply(session);
  if (reply != 0) {
    Serial.println("Got a response!");
    Serial.println(reply);
  }
}
