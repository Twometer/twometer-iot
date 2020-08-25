package de.twometer.iot.net;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;

public class BridgeDiscovery {

    private static final byte[] DISCOVERY_PAYLOAD = {0x00, 0x42, 0x69};

    private static final int DISCOVERY_PORT = 38711;

    public static String discover() throws IOException {
        InetSocketAddress address = new InetSocketAddress("10.0.2.255", DISCOVERY_PORT);

        DatagramSocket socket = new DatagramSocket();
        socket.setSoTimeout(25000);
        socket.setBroadcast(true);
        socket.send(new DatagramPacket(DISCOVERY_PAYLOAD, DISCOVERY_PAYLOAD.length, address));

        byte[] data = new byte[128];
        DatagramPacket reply = new DatagramPacket(data, data.length);
        socket.receive(reply);
        return new String(data).trim();
    }

}
