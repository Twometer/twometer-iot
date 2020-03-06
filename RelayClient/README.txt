Client for RelayServer running on a server within the IoT network. It's step 3 in the 
following communication from Alexa to Bridge.

0        1         2              3              4
Alexa => Lambda => RelayServer => RelayClient => Bridge

RelayServer and Client are used to avoid having to use port-forwarding
in the private network for the callbacks.

The RelayClient translates Alexa-formatted calls to Twometer IoT-formatted calls
and vice versa.