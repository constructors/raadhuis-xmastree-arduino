Raadhuis Xmas3 Arduino
======================

Arduino code to control the Xmas tree (hence the name) RGB
LEDs in Lieshout, the Netherlands.

What?
-----
This code runs on the arduino connected to the tree, enabling control of the lights via TCP/IP. The web
will handle user requests and use this to set and get colors. The xmas3 protocol is pretty simple:

* Open a connection to the arduino
* Send a byte 'g', and the tree returns three bytes corresponding to the RGB values of the current color
* Send four bytes to set a new color, the last three bytes correspond to the RGB values.

A color is set by updating the duty cycle of three (R, G, B) PWM pins. Those PWM signals are brought to 12 volt level
using NPN-transistors and then amplified to be able to source a decent amount of current.

Testing
-------

Since we don't lack confidence, there are no tests.

Get a color:
    
    echo -e 'g' | netcat $host $port
Set a color:
    
    echo -e 's\xff\x00\xff' | netcat $host $port

Artists rendering of the hardware setup:
***

           *             ,
                       _/^\_
                      <     >
     *                 /.-.\         *
              *        `/&\`                   *
                      ,@.*;@,
                     /_o.I %_\    *
        *           (`'--:o(_@;
                   /`;--.,__ `')             *
                  ;@`o % O,*`'`&\
            *    (`'--)_@ ;o %'()\      *
                 /`;--._`''--._O'@;
                /&*,()~o`;-.,_ `""`)
     *          /`,@ ;+& () o*`;-';\
               (`""--.,_0 +% @' &()\
               /-.,_    ``''--....-'`)  *
          *    /@%;o`:;'--,.__   __.'\
              ;*,&(); @ % &^;~`"`o;@();         *
              /(); o^~; & ().o@*&`;&%O\
        jgs   `"="==""==,,,.,="=="==="`
           __.----.(\-''#####---...___...-----._
         '`         \)_`"""""`
                 .--' ')
               o(  )_-\
                 `"""` `
