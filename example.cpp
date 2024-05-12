/*  
   RCSwitch Common Library
   Adaptation of RCSwitch Arduino library to be used on any system

   Example of using the RCSwitch Common Library
    
   https://github.com/latchdevel/rc-switch-lib

   Copyright (c) 2024 Jorge Rivera. All right reserved.
   License GNU Lesser General Public License v3.0.
*/

#include <RCSwitch.h>
#include <stdio.h>

int main(){

   // Instance of an RCSwitch object
   RCSwitch mySwitch = RCSwitch();

   // Declare a variable to pulse list store
   pulse_list_t pulse_list;

   // Optional set protocol (default is 1, will work for most outlets)
   // mySwitch.setProtocol(2);

   // Optional set pulse length
   // mySwitch.setPulseLength(320);

   // Get pulse list from any transmission function
   printf("Encoding: switchOn(\"11111\", \"00010\")\n");       // Represents the setting of the dual 5 DIP switches.
                                                               // In this example it's ON-ON-ON-ON-ON and OFF-OFF-OFF-ON-OFF.
   pulse_list = mySwitch.switchOn("11111", "00010");           // See rc-switch 'TypeA_WithDIPSwitches' example.

   // pulse_list = mySwitch.send(5393, 24);                    // decimal code
   // pulse_list = mySwitch.send("000000000001010100010001");  // binary code
   // pulse_list = mySwitch.sendTriState("00000FFF0F0F");      // tri-state code
   
   // Optional: Display raw pulse list
   printf("raw_pulses[%u]={",static_cast<unsigned int>(pulse_list.size()));
   pulse_list_t::iterator it=pulse_list.begin();
   while (it != pulse_list.end()){
      printf("%i",*it);
      it++;
      if (it != pulse_list.end()){
         printf(",");
      }else{
         printf("};\n");
      }
   }

   // Try to decode the same pulse list
   if (mySwitch.decodePulseTrain(pulse_list)) {

      printf("OK: decoded value: %lu (%u bits) Protocol: %u Delay: %u\n",mySwitch.getReceivedValue(),mySwitch.getReceivedBitlength(),mySwitch.getReceivedProtocol(),mySwitch.getReceivedDelay());
         
      // Optional: Display decoded pulse list
      unsigned int* timings = mySwitch.getReceivedRawdata();
      printf("dec_pulses[%u]={",static_cast<unsigned int>(pulse_list.size()));
      for (unsigned index = 1 ; index < pulse_list.size(); index++){
            printf("%u,",timings[index]);
      }
      printf("%u};\n",timings[0]);

      // Optional: Compare raw and decoded pulse list
      pulse_list_t::iterator it=pulse_list.begin();
      for (unsigned index = 1 ; index < pulse_list.size(); index++){
         //printf("index: %u it: %u timings: %u",index, *it, timings[index]);
         if (*it != timings[index]){
            fprintf(stderr,"ERROR: pulse lists mismatch.\n");
            return 2;
         }else{
            it++;
         }
      }
      if (*it != timings[0]){
         fprintf(stderr,"ERROR: footer pulse mismatch.\n");
         return 3;
      }else{
         printf("OK: pulse lists match.\n");
      }
   }else{
      fprintf(stderr,"ERROR: unable to decode.\n");
      return 1;
   }

   return 0;
}
