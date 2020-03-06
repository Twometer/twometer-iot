Server running on a dedicated non-lambda server, being step 2 in the 
following communication from Alexa to Bridge.

0        1         2              3              4
Alexa => Lambda => RelayServer => RelayClient => Bridge

RelayServer and Client are used to avoid having to use port-forwarding
in the private network for the callbacks.